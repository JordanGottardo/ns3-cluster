/*
 * ROFFApplication.cc
 *
 *  Created on: Jan 28, 2019
 *      Author: jordan
 */
#include "ROFFApplication.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("ROFFApplication");

NS_OBJECT_ENSURE_REGISTERED(ROFFApplication);

TypeId ROFFApplication::GetTypeId() {
	  static TypeId tid = TypeId ("ns3::ROFFApplication")
	    .SetParent<Object>()
	    .SetGroupName("Network");
	  return tid;
}


void ROFFApplication::Install(uint32_t broadcastPhaseStart, uint32_t actualRange, uint32_t aoi,
	uint32_t aoi_error, uint32_t vehicleDistance, uint32_t beaconInterval, uint32_t distanceRange) {
	NS_LOG_FUNCTION(this);
	m_broadcastPhaseStart = broadcastPhaseStart;
	m_aoi = aoi;
	m_aoi_error = aoi_error;
	m_actualRange = actualRange;
	m_vehicleDistance = vehicleDistance;
	m_beaconInterval = beaconInterval;
	m_distanceRange = distanceRange;
	m_randomVariable = CreateObject<UniformRandomVariable>();
//	NS_LOG_UNCOND("END INSTALL");
}

void ROFFApplication::AddNode(Ptr<Node> node, Ptr<Socket> source, Ptr<Socket> sink) {
	NS_LOG_FUNCTION(this);
	Ptr<ROFFNode> roffNode = CreateObject<ROFFNode>(node, source);
//	NS_LOG_UNCOND("post create node");
	sink->SetRecvCallback(MakeCallback(&ROFFApplication::ReceivePacket, this));
//	NS_LOG_UNCOND("post cback");
	m_nodes[roffNode->GetId()] = roffNode;
//	cout << "ROFFAPP AddNode " << roffNode->GetId() << endl;
//	NS_LOG_UNCOND("end add node");
}

void ROFFApplication::PrintStats(std::stringstream& dataStream) {
//	TODO
}

void ROFFApplication::StartApplication(void) {
	NS_LOG_FUNCTION(this);

	m_startingNode = this->GetNode()->GetId();
	GenerateHelloTraffic();
	Simulator::Schedule(Seconds(m_broadcastPhaseStart), &ROFFApplication::StartBroadcastPhase, this);
}

void ROFFApplication::StopApplication(void) {
	NS_LOG_FUNCTION(this);
}

void ROFFApplication::GenerateHelloTraffic() {
	NS_LOG_FUNCTION(this);
	Ptr<ROFFNode> roffNode = m_nodes.at(0);
//
//	for (int i = 0; i < 100; i++) {
//		for (auto entry: m_nodes) {
//			Ptr<ROFFNode> roffNode = entry.second;
//			// Generate random waiting time between [0, beaconInterval*2]. Average is beaconInterval
//			uint32_t waitingTime = m_randomVariable->GetInteger(0, m_beaconInterval * 2);
//			Simulator::Schedule(MilliSeconds(waitingTime), &ROFFApplication::GenerateHelloMessage, this, roffNode);
//		}
//	}
}

void ROFFApplication::StartBroadcastPhase(void) {
	NS_LOG_FUNCTION(this);
	for (auto entry: m_nodes) {
		Ptr<ROFFNode> roffNode = entry.second;
//		cout << "ROFFApplication::StartBroadcastPhase nbt size= " << roffNode->GetNBTSize() << endl;
	}
	GenerateAlertMessage(m_nodes.at(m_startingNode));
}

void ROFFApplication::GenerateHelloMessage(Ptr<ROFFNode> node) {
	uint32_t headerType = HELLO_MESSAGE;
	uint32_t nodeId = node->GetId();
	Vector position = node->GetPosition();

	ROFFHeader header(headerType, nodeId, position, boost::dynamic_bitset<>());

	Ptr<Packet> packet = Create<Packet>(m_packetPayload);
	packet->AddHeader(header);
	node->Send(packet);

}

void ROFFApplication::GenerateAlertMessage(Ptr<ROFFNode> node) {
	NS_LOG_FUNCTION (this << node);
	NS_LOG_DEBUG("ROFFApplication::Generate Alert message");

	uint32_t headerType = ALERT_MESSAGE;
	uint32_t nodeId = node->GetId();
	Vector position = node->GetPosition();
	boost::dynamic_bitset<> esdBitmap = node->GetESDBitmap(m_distanceRange);
//	cout << "ROFFApplication::GenerateAlertMessage esdBitmap = " << esdBitmap << " size= " <<
//			esdBitmap.size() << endl;
	ROFFHeader header(headerType, nodeId, position, esdBitmap, 0);

	Ptr<Packet> packet = Create<Packet>(m_packetPayload);
//	cout << "add " << node->GetPosition() << endl;
	packet->AddHeader(header);
	NS_LOG_DEBUG("ROFFApplication::GenerateAlertMessage before send " << Simulator::Now());
	node->Send(packet);
	NS_LOG_DEBUG("ROFFApplication::GenerateAlertMessage after send " << Simulator::Now());
	node->SetSent(true);
	node->SetReceived(true);
	node->SetPhase(0);
}

void ROFFApplication::ReceivePacket(Ptr<Socket> socket) {
	NS_LOG_FUNCTION(this << socket);
	Ptr<Node> node = socket->GetNode();
	Ptr<ROFFNode> roffNode = m_nodes.at(node->GetId());
//	cout << "received packet by node " << node->GetId() << " at time=" << Simulator::Now().GetSeconds() << endl;
	Address senderAddress;
	Ptr<Packet> packet;

	Vector currentPosition = node->GetObject<MobilityModel>()->GetPosition();

	while ((packet = socket->RecvFrom(senderAddress))) {
		ROFFHeader header;
		packet->RemoveHeader(header);
		Vector senderPosition = header.GetPosition();
		double distance =  ns3::CalculateDistance(senderPosition, currentPosition);
		cout << "received packet by node " << node->GetId() <<
				" from node in pos " << senderPosition << " distance= " << distance << endl;

		uint32_t packetType = header.GetType();
		if (packetType == HELLO_MESSAGE) {
			HandleHelloMessage(roffNode, header);
		} else if (packetType == ALERT_MESSAGE) {
			NS_LOG_DEBUG("ROFFApplication::ReceivePacket node " << node->GetId() <<
					" has received an alertMessage from node "
					<< header.GetSenderId() <<
					" at time= " << Simulator::Now());
//			cout << "nbt size= " << roffNode->GetNBTSize() << endl;
			HandleAlertMessage(roffNode, header, distance);
		} else {
			NS_LOG_ERROR("Packet type not recognized");
		}
	}
}

void ROFFApplication::HandleHelloMessage(Ptr<ROFFNode> node,
		ROFFHeader header) {
	NS_LOG_FUNCTION(this << node << header);
	uint32_t senderId = header.GetSenderId();
	Vector senderPosition = header.GetPosition();
	node->AddOrUpdateNeighbor(senderId, senderPosition, Simulator::Now());
}

void ROFFApplication::HandleAlertMessage(Ptr<ROFFNode> node,
		ROFFHeader header, uint32_t distance) {
	NS_LOG_FUNCTION(this << node << header << distance);
	NS_LOG_DEBUG("ROFFApplication::HandleAlertMessage Node " << node->GetId() << " received alert message from node " << header.GetSenderId());
	int32_t phase = header.GetPhase();
	node->SetPhase(phase);
	if (node->GetReceived()) {
		return;
	}
	node->SetReceived(true);
	boost::dynamic_bitset<> esdBitmap =  header.GetESDBitmap();
	uint32_t esdBitmapSize = esdBitmap.size();
//	NS_LOG_DEBUG("ROFFApplication::HandleAlertMessage Received bitmap in alert= " << esdBitmap);
	Vector senderPosition = header.GetPosition();
	Vector nodePosition = node->GetPosition();
	uint32_t dist = rint(ns3::CalculateDistance(senderPosition, nodePosition));
//	NS_LOG_DEBUG("ROFFApplication::HandleAlertMessage dist= " << dist);
	uint32_t posOfDist = dist / m_distanceRange; //todo controllare se funziona anche con distanceRange > 1
	uint32_t posToCheck = esdBitmapSize - posOfDist - 1;
//	cout << "ROFFHeader::HandleAlertMessage dist= " << dist << " posOfDist= " << posOfDist <<
//				" posToCheck= " << posToCheck << endl;
//	cout << esdBitmapSize << " " << posOfDist << " " << posToCheck << endl;;
	if ( posToCheck >= esdBitmapSize || esdBitmap[posToCheck] == 0) {
		NS_LOG_DEBUG("ROFFApplication::HandleAlertMessage Node " << node->GetId()
				<< " won't participate in contention: not present in esd bitmap");
		return;
	}
	if (!node->IsNodeWinnerInContention(dist, senderPosition)) {
		NS_LOG_DEBUG("ROFFApplication::HandleAlertMessage Node " << node->GetId()
				<< " won't participate in contention: it has lost the contention");
		return;
	}
	PositionRankingMap rankingOfPositions = PositionRankingMap(m_distanceRange, esdBitmap);
//	cout << "ROFFHeader::HandleAlertMessage " << rankingOfPositions << endl;
	uint32_t priority = rankingOfPositions.GetPriority(dist);
//	cout << "ROFFHeader::HandleAlertMessage priority= " << priority << endl;
//	uint32_t waitingTime = ComputeWaitingTime(node, dist, rankingOfPositions, priority);

	uint32_t waitingTime = ComputeWaitingTimeArmir(node, senderPosition, rankingOfPositions, priority);
//	cout << "ROFFApplication::HandleAlertMessage waitingTime= " << waitingTime << endl << endl << endl;
//	Simulator::Schedule(MilliSeconds(waitingTime), &ROFFApplication::ForwardAlertMessage,
//			this, node, header, waitingTime); //todo riabilitare
}

void ROFFApplication::ForwardAlertMessage(Ptr<ROFFNode> node, ROFFHeader oldHeader, uint32_t waitingTime) {
	uint32_t phase = oldHeader.GetPhase();
	NS_LOG_FUNCTION(this << node << oldHeader << waitingTime);
	if (node->GetPhase() > phase) {
		NS_LOG_DEBUG("ROFFApplication::ForwardAlertMessage node "
				<< node->GetId() << " defers because of phase");
		return;
	}
	if (node->GetSent()) {
		NS_LOG_DEBUG("ROFFApplication::ForwardAlertMessage node "
				<< node->GetId() << " defers because of getSent");
	}
	uint32_t headerType = ALERT_MESSAGE;
	uint32_t forwarderId = node->GetId();
	Vector forwarderPosition = node->GetPosition();
	boost::dynamic_bitset<> esdBitmap = node->GetESDBitmap(m_distanceRange);

	ROFFHeader header(headerType, forwarderId, forwarderPosition, esdBitmap, phase + 1);

	Ptr<Packet> packet = Create<Packet>(m_packetPayload);
	packet->AddHeader(header);
	NS_LOG_DEBUG("ROFFApplication::ForwardAlertMessage node " << node->GetId()
			<< " forwards after waitingTime= " << waitingTime << " at time= " << Simulator::Now());
	node->Send(packet);
	node->SetSent(true);
}

void ROFFApplication::StopNode(Ptr<ROFFNode> fbNode) {
}


uint32_t ROFFApplication::ComputeWaitingTime(Ptr<ROFFNode> node, uint32_t distSenderToNode,
		PositionRankingMap rankingMap, uint32_t priority) {
	uint32_t waitingTime = 0;
	for (uint32_t pr = priority - 1; pr > 0; pr--) {
//		cout << "ROFFApplication::ComputeWaitingTime priority= " << pr << endl;
		uint32_t upperDistanceLimit = rankingMap.GetUpperDistanceLimit(pr);
		waitingTime += ComputeMinDiff(distSenderToNode, upperDistanceLimit);
	}
	return waitingTime;
}

uint32_t ROFFApplication::ComputeMinDiff(uint32_t distSenderToNode, uint32_t distSenderToAnotherNode) {
//	cout << "ROFFApplication::ComputeMinDiff distSenderToNode = " << distSenderToNode <<
//			" distSenderToAnotherNode= " << distSenderToAnotherNode << endl;
	uint32_t rxTx = 2;
	uint32_t ccaTime = 8;
	uint32_t c = 299792458; //speed of light
	uint32_t maxDist = distSenderToNode + distSenderToAnotherNode;
	uint32_t propagationDelay = ceil((((double)maxDist) / ((double)c)) * 1000 * 1000); //in microseconds
//	cout << "ROFFApplication::ComputeMinDiff maxDist= " << maxDist <<
//			" propagationDelay= " << propagationDelay << endl;
	return  propagationDelay + rxTx + ccaTime;
}

uint32_t ROFFApplication::ComputeWaitingTimeArmir(Ptr<ROFFNode> node, Vector senderCoords,
				PositionRankingMap rankingMap, uint32_t priority) {
//	cout << "ROFFApplication::ComputeWaitingTimeArmir node= " << node->GetId() <<
//				" senderCoords= " << senderCoords << " priority= " << priority << endl;
	if (priority <= 1) {
		return 0;
	}
	int32_t dist = 0;
//	if (priority == 2) {
//		PositionRankingKey range = rankingMap.GetRange(priority - 1);
//		Vector higherPriorityNodeCoords = node->GetCoordsOfVehicleInRange(range, senderCoords, dist);
//		return ComputeMinDiffArmir(senderCoords, lowerPriorityNodeCoords, higherPriorityNodeCoords);
//	}
	// priority >= 3
	double waitingTime = 0;
	uint32_t pr = priority - 1;
	PositionRankingKey range = rankingMap.GetRange(pr);
	Vector higherPriorityNodeCoords = node->GetCoordsOfVehicleInRange(range, senderCoords, dist);
	if (dist != -1) {
		waitingTime += ComputeMinDiffArmir(senderCoords, node->GetPosition(), higherPriorityNodeCoords);
	}

	for (; pr > 1; pr--) {
		PositionRankingKey lowerPriorityRange = rankingMap.GetRange(pr);
		PositionRankingKey higherPriorityRange = rankingMap.GetRange(pr - 1);
		int32_t lowerPriorityDist;
		int32_t higherPriorityDist;

		Vector higherPriorityNodeCoords =
				node->GetCoordsOfVehicleInRange(higherPriorityRange, senderCoords, lowerPriorityDist);
		Vector lowerPriorityNodeCoords =
						node->GetCoordsOfVehicleInRange(higherPriorityRange, senderCoords, higherPriorityDist);
//		cout << "ROFFApplication::ComputeWaitingTimeArmir higherPriorityNodeCoords= " <<
//				higherPriorityNodeCoords << endl;
		if (lowerPriorityDist != -1 && higherPriorityDist != -1) {
			waitingTime += ComputeMinDiffArmir(senderCoords,
					lowerPriorityNodeCoords, higherPriorityNodeCoords);
		}
	}
	return ceil(waitingTime);
}

uint32_t ROFFApplication::ComputeMinDiffArmir(Vector fwdCoords, Vector lowerPriorityNodeCoords,
		Vector higherPriorityNodeCoords) {
	uint32_t rxTx = 0; //2
//	uint32_t ccaTime = 269000;
	double ccaTime = 269;
	double pdFwdToHigherPriority = ComputePropagationDelay(fwdCoords, higherPriorityNodeCoords);
	double pdFwdToLowerPriority = ComputePropagationDelay(fwdCoords, lowerPriorityNodeCoords);
	double pdHigherToLowerPriority = ComputePropagationDelay(higherPriorityNodeCoords, lowerPriorityNodeCoords);
//	cout << "ROFFApplication::ComputeMinDiffArmir fwdToHigher= " << pdFwdToHigherPriority <<
//			" fwdToLower= " << pdFwdToLowerPriority << " higherToLower= " << pdHigherToLowerPriority << endl;
	uint32_t minDiff = ceil(pdFwdToHigherPriority - pdFwdToLowerPriority + pdHigherToLowerPriority + rxTx + ccaTime);
//	cout << "ROFFApplication::ComputeMinDiffArmir minDiff= " << minDiff << endl;
	return minDiff;


}

double ROFFApplication::ComputePropagationDelay(Vector coord1, Vector coord2) {
	double c = 299792458; //speed of light
	double dist = ns3::CalculateDistance(coord1, coord2) * 50;
	double pd = dist / c * 1000 * 1000; //in microseconds
//	cout << "ROFFApplication::ComputePropagationDelay tra " << coord1 << " e " <<
//			coord2 << " pd= " << pd << endl;
	return pd;

}

}



