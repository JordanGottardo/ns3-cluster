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
	uint32_t aoi_error, uint32_t vehicleDistance, uint32_t beaconInterval, uint32_t distanceRange,
	uint32_t startingNode, uint32_t printCoords, uint32_t errorRate, uint32_t forgedCoordRate, uint32_t droneTest) {
	NS_LOG_FUNCTION(this);
	m_broadcastPhaseStart = broadcastPhaseStart;
	m_aoi = aoi;
	m_aoi_error = aoi_error;
	m_actualRange = actualRange;
	m_vehicleDistance = vehicleDistance;
	m_beaconInterval = beaconInterval;
	m_distanceRange = distanceRange;
	m_startingNode = startingNode;
	m_printCoords = printCoords;
	m_errorRate = errorRate;
	m_forgedCoordRate = forgedCoordRate;
	m_droneTest = droneTest;
	m_randomVariable = CreateObject<UniformRandomVariable>();
//	NS_LOG_UNCOND("END INSTALL");
}

void ROFFApplication::AddNode(Ptr<Node> node, Ptr<Socket> source, Ptr<Socket> sink, bool onstats,
		bool isNodeInJunction, uint64_t junctionId) {
	NS_LOG_FUNCTION(this);
	Ptr<ROFFNode> roffNode = CreateObject<ROFFNode>(node, source, isNodeInJunction, junctionId, onstats);
//	NS_LOG_UNCOND("post create node");
	sink->SetRecvCallback(MakeCallback(&ROFFApplication::ReceivePacket, this));
//	NS_LOG_UNCOND("post cback");
	m_nodes[roffNode->GetId()] = roffNode;
//	cout << roffNode->GetId() << endl;
//	cout << "ROFFAPP AddNode " << roffNode->GetId() << endl;
//	NS_LOG_UNCOND("end add node");
}

void ROFFApplication::PrintStats(std::stringstream& dataStream) {
	NS_LOG_FUNCTION (this);
//	NS_LOG_INFO("ROFFApplication::PrintStats " << m_received << " nodes have received the message");
	uint32_t cover = 1;	// 'cause we count m_startingNode
	uint32_t coverVehicles = 1;
	uint32_t circ = 0, circCont = 0;

	double radiusMin = m_aoi - m_aoi_error;
	double radiusMax = m_aoi + m_aoi_error;

	long double time_sum = 0;
	long double hops_sum = 0;
	long double slots_sum = 0;

	stringstream receivedOnCircIds;

	for (uint32_t i = 0; i < m_nodes.size(); i++)	{
		Ptr<ROFFNode> current = m_nodes.at (i);
		uint32_t nodeId = current->GetId ();

		// Skip the starting node
		if (nodeId == m_startingNode)
			continue;

		// Update the total cover value
		if (current->GetReceived()) {
//			cout << "cover++" << endl;
			cover++;
			if (current->AmIAVehicle()) {
				coverVehicles++;
			}
		}

		// Compute cover on circumference of radius m_aoi

		Ptr<ROFFNode> startingNode = m_nodes.at(m_startingNode);

		Vector currentPosition = current->GetPosition();
		Vector startingNodePosition = startingNode->GetPosition();

		double distance = ns3::CalculateDistance (currentPosition, startingNodePosition);

		// Check if the current vehicle is in the circumference and within the range
		if ((distance >= radiusMin) && (distance <= radiusMax))	{
			// Update the number of vehicles in the circumference
			circCont++;

			// Update the cover value
			if (current->GetReceived()) {
				circ++;
				receivedOnCircIds << current->GetId() << "_";
				// Update mean time, nums and slots
					hops_sum += current->GetHop();
					slots_sum += current->GetSlot();
					time_sum += current->GetTimestamp().GetMicroSeconds ();
			}
		}
	}
//	Time when the first alert message was sent
	Time timeref = m_nodes[m_startingNode]->GetTimestamp();
	string receivedNodes = StringifyVector(m_receivedNodes);
	stringstream nodeIds;

	for (auto i = m_nodes.begin(); i != m_nodes.end(); ++i) {
		uint32_t id = (*i).second->GetId();
		nodeIds << id << "_";
//		Vector pos = (*i)->GetPosition();
//		if ( pos.x > 1050.0 && pos.x < 1250.0 && pos.y > 1800.0 && pos.y < 1900.0) {
//			cout << id << endl;
//		}
	}

	dataStream << circCont << ","
			<< cover << ","
			<< circ << ","
			<< (time_sum / (double) circ) - timeref.GetMicroSeconds () << ","
			<< (hops_sum / (double) circ) << ","
			<< (slots_sum / (double) circ) << ","
//			<< m_nodes[m_nodes.size() - 1]->GetHop() << ","
//			<< m_nodes[m_nodes.size() - 1]->GetSlot() << ","
			<< m_sent << ","
			<< m_received;
	cout << "totalCoverage= " << cover << "/" << m_nodes.size() << endl;
	cout << "coverageOnCirc= " << circ << "/" << circCont << endl;
	cout << "m_sent=" << m_sent << endl;
	cout << "hops= " << (hops_sum / (double) circ) << endl;
	cout << "slots= " << (slots_sum / (double) circ) << endl;
	if (m_printCoords) {
		 Ptr<ROFFNode> startingNode = m_nodes.at(m_startingNode);
		 string transmissionVector = StringifyVector(m_transmissionVector);
		 dataStream << "," << startingNode->GetPosition().x << "," << startingNode->GetPosition().y << "," << m_startingNode << "," <<
				m_vehicleDistance << "," << receivedNodes << "," << nodeIds.str() << "," << StringifyTransmissionMap() <<
				"," << receivedOnCircIds.str() << "," << transmissionVector;
	}
	if (m_droneTest) {
		uint32_t maxDistance = 0;
		uint32_t maxDistanceNodeReached = IsMaxDistNodeReached(maxDistance);
		dataStream << "," << maxDistance << "," << maxDistanceNodeReached << "," << coverVehicles;
	}
}

void ROFFApplication::StartApplication(void) {
	NS_LOG_FUNCTION(this);

	m_startingNode = this->GetNode()->GetId();
	if (m_forgedCoordRate > 0) {
		GenerateForgedHelloTraffic();
	}
	GenerateHelloTraffic(15);
	Simulator::Schedule(Seconds(m_broadcastPhaseStart), &ROFFApplication::StartBroadcastPhase, this);
}

void ROFFApplication::StopApplication(void) {
	NS_LOG_FUNCTION(this);
}

void ROFFApplication::GenerateForgedHelloTraffic() {
	uint32_t nAffectedNodes = m_nodes.size() * ((double)m_forgedCoordRate / 100);
	NS_LOG_INFO("GeneratedForgedHelloTraffic affected " << nAffectedNodes << " nodes");
	uint32_t forgedSenderNodeId = m_nodes.size();
	set<uint32_t> affectedNodes;
	while (affectedNodes.size() < nAffectedNodes) {
		uint32_t nodeId = m_randomVariable->GetInteger(0, m_nodes.size() - 1);
		affectedNodes.insert(nodeId);
	}



	for (auto id: affectedNodes) {
		double startingX = m_nodes[id]->UpdatePosition().x + m_actualRange; //low sev
		double startingY = m_nodes[id]->UpdatePosition().y;
//		double startingX = 10000; //high sev
		NS_LOG_DEBUG("affecting node " << id << " with forging");
		for (uint32_t i = 1; i < 151; i++) {
			uint32_t headerType = HELLO_MESSAGE;
			Vector position = Vector(startingX + i * m_distanceRange, startingY, 0);
			ROFFHeader header(headerType, position, forgedSenderNodeId + i, position, boost::dynamic_bitset<>(), 0, 0);
			HandleHelloMessage(m_nodes.at(id), header);
		}
	}
}

void ROFFApplication::GenerateHelloTraffic(uint32_t count) {
	NS_LOG_FUNCTION(this);
//	cout << "generateHelloTraffic" << count << endl;
	NS_LOG_INFO("Generate hello traffic " << count);
	if (count == 0) {
		return;
	}
	Ptr<ROFFNode> roffNode = m_nodes.at(0);
	cout << "nodes size = " << m_nodes.size() << endl;
	int toSend = m_nodes.size() * 0.4;
	cout << "toSend= " << toSend << endl;
	map<uint32_t, bool> sentMap;

	while (sentMap.size() < toSend) {
		uint32_t send = m_randomVariable->GetInteger(0, m_nodes.size() - 1);
		if (sentMap.count(send) == 0) {
			sentMap[send] = true;
			Ptr<ROFFNode> roffNode = m_nodes.at(send);
			uint32_t waitingTime = m_randomVariable->GetInteger(0, m_beaconInterval * 2);
			Simulator::Schedule(MilliSeconds(waitingTime), &ROFFApplication::GenerateHelloMessage, this, roffNode);

		}
	}

//	for (auto entry: m_nodes) {
//		Ptr<ROFFNode> roffNode = entry.second;
//		// Generate random waiting time between [0, beaconInterval*2]. Average is beaconInterval
//		uint32_t waitingTime = m_randomVariable->GetInteger(0, m_beaconInterval * 2);
//		Simulator::Schedule(MilliSeconds(waitingTime), &ROFFApplication::GenerateHelloMessage, this, roffNode);
//	}
	Simulator::Schedule (MilliSeconds(m_beaconInterval * 2 + 1), &ROFFApplication::GenerateHelloTraffic, this, count - 1);

}

void ROFFApplication::StartBroadcastPhase(void) {
	NS_LOG_FUNCTION(this);
	NS_LOG_INFO("Start broadcast phase");
	for (auto entry: m_nodes) {
		Ptr<ROFFNode> roffNode = entry.second;
//		cout << "ROFFApplication::StartBroadcastPhase nbt size= " << roffNode->GetNBTSize() << endl;
	}
			cout << "ROFFApplication::StartBroadcastPhase nbt size= " << m_nodes.at(m_startingNode)->GetNBTSize() << endl;

	GenerateAlertMessage(m_nodes.at(m_startingNode));
	if (m_errorRate > 0) {
		Simulator::Schedule(MilliSeconds(1), &ROFFApplication::GenerateAlertMessage, this, m_nodes.at(m_startingNode));
	}
}

void ROFFApplication::GenerateHelloMessage(Ptr<ROFFNode> node) {
	uint32_t headerType = HELLO_MESSAGE;
	uint32_t nodeId = node->GetId();
	Vector position = node->UpdatePosition();

	ROFFHeader header(headerType, position, nodeId, position, boost::dynamic_bitset<>(), 0, 0);

	Ptr<Packet> packet = Create<Packet>(m_packetPayload);
//	cout << "packetPayload = " << m_packetPayload
	packet->AddHeader(header);
	node->Send(packet);

}

void ROFFApplication::GenerateAlertMessage(Ptr<ROFFNode> node) {
	NS_LOG_FUNCTION (this << node);
	NS_LOG_DEBUG("ROFFApplication::Generate Alert message");
	m_nodes.at(m_startingNode)->UpdatePosition();
//	for (auto it = m_nodes.begin(); it != m_nodes.end(); it++) {
//		cout << "ROFFApplication::GenerateAlertMessage" << it->second->GetPosition() << endl;
//	}

	uint32_t headerType = ALERT_MESSAGE;
	uint32_t nodeId = node->GetId();
	Vector position = node->UpdatePosition();
	boost::dynamic_bitset<> esdBitmap = node->GetESDBitmap(m_distanceRange);
//	cout << "ROFFApplication::GenerateAlertMessage esdBitmap = " << esdBitmap << " size= " <<
//			esdBitmap.size() << endl;
	ROFFHeader header(headerType, position, nodeId, position, esdBitmap, 0, 0, node->AmIInJunction(), node->GetJunctionId());

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
//	NS_LOG_INFO("ROFFApplication::ReceivePacket received packet by node " << node->GetId() << " at time=" <<
//			Simulator::Now().GetSeconds());
	Address senderAddress;
	Ptr<Packet> packet;

	Vector currentPosition = roffNode->UpdatePosition();

	while ((packet = socket->RecvFrom(senderAddress))) {
		ROFFHeader header;
		packet->RemoveHeader(header);
		Vector senderPosition = header.GetPosition();
//		cout << "received packet by node " << node->GetId() <<
//				" from node in pos " << senderPosition << " distance= " << distance << endl;
		double distance = ns3::CalculateDistance(currentPosition, senderPosition);
		if (m_forgedCoordRate == 0 && distance > m_actualRange) {
			continue;
		}

		uint32_t packetType = header.GetType();
		if (packetType == HELLO_MESSAGE) {
			HandleHelloMessage(roffNode, header);
		} else if (packetType == ALERT_MESSAGE) {
//			NS_LOG_DEBUG("ROFFApplication::ReceivePacket node " << node->GetId() <<
//					" has received an alertMessage from node "
//					<< header.GetSenderId() <<
//					" at time= " << Simulator::Now());
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
//	NS_LOG_INFO("ROFFApplication::HandleAlertMessage Node " << node->GetId() << " received alert message from node " << header.GetSenderId());

//	if (node->GetScheduled()) {
//		return;
//	}

	int32_t phase = header.GetPhase();
	uint32_t senderId = header.GetSenderId();
	uint32_t receiverId = node->GetId();
	Vector currentPosition = node->UpdatePosition();
	Vector senderPosition = header.GetPosition();
	Vector starterPosition = header.GetStarterPosition();

	double distanceSenderToCurrent = ns3::CalculateDistance(senderPosition, currentPosition);

	double distanceSenderToStarter = ns3::CalculateDistance(senderPosition, starterPosition);
	double distanceCurrentToStarter = ns3::CalculateDistance(currentPosition, starterPosition);


	if (node->AmIInJunction()) {
		NS_LOG_LOGIC("node " << node->GetId() << "is inside a junction and has received an alert message");
		if (header.IsSenderInJunction() && node->GetJunctionId() == header.GetJunctionId()) {
			if (phase > node->GetPhase()) {
				node->SetPhase(phase);
//				NS_LOG_LOGIC("node " << node->GetId() << "is inside a junction: updates phase from " << node->GetPhase() << " to " << phase);
			}
		}
	}
	else {
//		if (phase > node->GetPhase() && (distanceSenderToStarter > distanceCurrentToStarter)) {
		if (phase > node->GetPhase()) { //todo abilitare per urbano
			NS_LOG_LOGIC("node " << node->GetId() << "is not inside a junction: updates phase from " << node->GetPhase() << " to " << phase);
			node->SetPhase(phase);
		}
	}

	if (node->GetReceived()) {
		return;
	}
	if (!node->GetReceived()) {
		node->SetReceived(true);
		node->SetTimestamp(Simulator::Now());
		node->SetSlot(header.GetSlot());
		node->SetHop(phase + 1);
		m_received++;
			// save transmission for stats and metrics
		m_receivedNodes.push_back(receiverId);
		auto it = m_transmissionList.find(senderId);
		if (it == m_transmissionList.end()) {
			m_transmissionList[senderId] = vector<uint32_t>();
		}
		m_transmissionList[senderId].push_back(receiverId);
		m_transmissionVector.push_back(Edge(senderId, receiverId, phase));
	}
//	NS_LOG_DEBUG("ROFFApplication::HandleAlertMessage Node " << node->GetId() << " "
//			"received alert message from node " << header.GetSenderId() <<
//			" with phase= " << phase);


	boost::dynamic_bitset<> esdBitmap =  header.GetESDBitmap();
	uint32_t esdBitmapSize = esdBitmap.size();
//	NS_LOG_DEBUG("ROFFApplication::HandleAlertMessage Received bitmap in alert= " << esdBitmap);

	uint32_t dist = rint(ns3::CalculateDistance(senderPosition, currentPosition));
//	NS_LOG_DEBUG("ROFFApplication::HandleAlertMessage dist= " << dist);
	uint32_t posOfDist = dist / m_distanceRange;
	uint32_t posToCheck = esdBitmapSize - posOfDist - 1;

//	cout << "ROFFHeader::HandleAlertMessage dist= " << dist << " posOfDist= " << posOfDist <<
//				" posToCheck= " << posToCheck << endl;
//	cout << esdBitmapSize << " " << posOfDist << " " << posToCheck << endl;;

//	if (distanceCurrentToStarter > distanceSenderToStarter) {

	if ( posToCheck >= esdBitmapSize || esdBitmap[posToCheck] == 0) {
		NS_LOG_DEBUG("ROFFApplication::HandleAlertMessage Node " << node->GetId() << " pos " << node->UpdatePosition()
				<< " won't participate in contention: not present in esd bitmap");
		return;
	}
	if (!node->IsNodeWinnerInContention(dist, senderPosition)) {
		NS_LOG_DEBUG("ROFFApplication::HandleAlertMessage Node " << node->GetId() << " pos " << node->UpdatePosition()
				<< " won't participate in contention: it has lost the contention");
		return;
	}
//	cout << "receivedEsd = " << esdBitmap << endl;
	PositionRankingMap rankingOfPositions = PositionRankingMap(m_distanceRange, esdBitmap);
//	cout << "ROFFHeader::HandleAlertMessage " << rankingOfPositions << endl;
	uint32_t priority = rankingOfPositions.GetPriority(dist);
//	cout << "priority = " << priority << endl;
//	cout << "ROFFHeader::HandleAlertMessage priority= " << priority << endl;
//	uint32_t waitingTime = ComputeWaitingTime(node, dist, rankingOfPositions, priority);

	uint32_t waitingTime = ComputeWaitingTimeArmir(node, senderPosition, rankingOfPositions, priority);
	int32_t errorDelay = ComputeErrorDelay();

	if (errorDelay == 0) {
		Simulator::Schedule(MilliSeconds(waitingTime), &ROFFApplication::ForwardAlertMessage,
				this, node, header, waitingTime, false);
 	}
	else {
		uint32_t firstTransmissionTime;
		uint32_t secondTransmissionTime;
		firstTransmissionTime = errorDelay > 0 ? waitingTime : waitingTime + errorDelay;
		secondTransmissionTime = errorDelay < 0 ? waitingTime : waitingTime + errorDelay;
		Simulator::Schedule(MilliSeconds(firstTransmissionTime), &ROFFApplication::ForwardAlertMessage,
						this, node, header, firstTransmissionTime, false);
		Simulator::Schedule(MilliSeconds(secondTransmissionTime), &ROFFApplication::ForwardAlertMessage,
								this, node, header, secondTransmissionTime, true);
	}

//	cout << "ROFFApplication::HandleAlertMessage waitingTime= " << waitingTime << endl << endl << endl;


	//	node->SetScheduled(true);
//	}
}

void ROFFApplication::ForwardAlertMessage(Ptr<ROFFNode> node, ROFFHeader oldHeader, uint32_t waitingTime, bool forceSend) {
	int32_t phase = oldHeader.GetPhase();
	NS_LOG_FUNCTION(this << node << oldHeader << waitingTime << forceSend);

	Vector oldPos = oldHeader.GetPosition();
	Vector position = node->UpdatePosition();
	double distance = ns3::CalculateDistance(position, oldPos);

	if (node->GetStopSending()) {
		NS_LOG_DEBUG("node " << node->GetId() << " defers because of StopSending");
		return;
	}
	if (!(node->GetSent() && forceSend)) {
		if (node->GetPhase() > phase) {
			NS_LOG_DEBUG("ROFFApplication::ForwardAlertMessage node "
					<< node->GetId() << " pos " << node->UpdatePosition() << " defers because of phase (" << node->GetPhase() << " > " << phase << ")");
			return;
		}
		if (node->GetSent()) {
			NS_LOG_DEBUG("ROFFApplication::ForwardAlertMessage node "
					<< node->GetId() << " pos " << node->UpdatePosition() << " defers because of getSent");
			return;
		}
	}
	if (forceSend) {
		node->SetStopSending(true);
	}
//	cout << "id= " << node->GetId() << " nodePhase= " << node->GetPhase() << " headerPhase= "
//			<< phase << endl;
	uint32_t headerType = ALERT_MESSAGE;
	uint32_t forwarderId = node->GetId();
	uint32_t slot = oldHeader.GetSlot();
	Vector forwarderPosition = node->UpdatePosition();
	Vector starterPosition = oldHeader.GetStarterPosition();
	boost::dynamic_bitset<> esdBitmap = node->GetESDBitmap(m_distanceRange);

	ROFFHeader header(headerType, forwarderPosition, forwarderId, starterPosition, esdBitmap, phase + 1, slot + waitingTime);

	Ptr<Packet> packet = Create<Packet>(m_packetPayload);
	packet->AddHeader(header);
	NS_LOG_DEBUG("ROFFApplication::ForwardAlertMessage node " << node->GetId() << " pos " << node->UpdatePosition()
			<< " forwards after waitingTime= " << waitingTime << " at distance" << distance);
	node->Send(packet);
	node->SetSent(true);
	m_sent++;
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
	return priority;
//	int32_t dist = 0;
//	if (priority == 2) {
//		PositionRankingKey range = rankingMap.GetRange(priority - 1);
//		Vector higherPriorityNodeCoords = node->GetCoordsOfVehicleInRange(range, senderCoords, dist);
//		return ComputeMinDiffArmir(senderCoords, lowerPriorityNodeCoords, higherPriorityNodeCoords);
//	}
	// priority >= 3


//	double waitingTime = 0;
//	uint32_t pr = priority - 1;
//	PositionRankingKey range = rankingMap.GetRange(pr);
//	Vector higherPriorityNodeCoords = node->GetCoordsOfVehicleInRange(range, senderCoords, dist);
//	if (dist != -1) {
//		waitingTime += ComputeMinDiffArmir(senderCoords, node->UpdatePosition(), higherPriorityNodeCoords);
//	}
//
//	for (; pr > 1; pr--) {
//		PositionRankingKey lowerPriorityRange = rankingMap.GetRange(pr);
//		PositionRankingKey higherPriorityRange = rankingMap.GetRange(pr - 1);
//		int32_t lowerPriorityDist;
//		int32_t higherPriorityDist;
//
//		Vector higherPriorityNodeCoords =
//				node->GetCoordsOfVehicleInRange(higherPriorityRange, senderCoords, lowerPriorityDist);
//		Vector lowerPriorityNodeCoords =
//						node->GetCoordsOfVehicleInRange(higherPriorityRange, senderCoords, higherPriorityDist);
////		cout << "ROFFApplication::ComputeWaitingTimeArmir higherPriorityNodeCoords= " <<
////				higherPriorityNodeCoords << endl;
//		if (lowerPriorityDist != -1 && higherPriorityDist != -1) {
//			waitingTime += ComputeMinDiffArmir(senderCoords,
//					lowerPriorityNodeCoords, higherPriorityNodeCoords);
//		}
//	}
//	return ceil(waitingTime);
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

int32_t ROFFApplication::ComputeErrorDelay() {
	if (m_errorRate == 0) {
		return 0;
	}
	int32_t delay = 0;
	uint32_t percent = m_randomVariable->GetInteger(0, 100);
	if (percent <= m_errorRate) {
		uint32_t plusOrMinusOne = m_randomVariable->GetInteger(0, 1);
		if (plusOrMinusOne == 0) {
			delay = 1;
		} else {
			delay = -1;
		}
	}
	return delay;
}

template <typename T>
string ROFFApplication::StringifyVector(const vector<T>& v) {
	NS_LOG_FUNCTION(this);
	stringstream ss;
//	cout << "ROFFApplication::PrintStuff" << m_receivedCoords.size() << " " << m_received << endl;
	for (auto i = v.begin(); i != v.end(); ++i) {
		ss << *i <<"_";
	}
	return ss.str();
}

string ROFFApplication::StringifyTransmissionMap() const {
	NS_LOG_FUNCTION(this);
	stringstream ss;
	for (auto it = m_transmissionList.begin(); it != m_transmissionList.end(); ++it) {
		ss << it->first << ":{";
		for (auto el: it->second) {
			ss << el << ";";
		}
		ss << "}";
	}
	return ss.str();
}

uint32_t ROFFApplication::IsMaxDistNodeReached(uint32_t& maxDist) const {
	NS_LOG_UNCOND("ROFFApplication::IsMaxDistNodeReached");
	Ptr<ROFFNode> startingNode =  m_nodes.at(m_startingNode);
	Vector startingNodePos = startingNode->GetPosition();
	uint32_t nodeId = 0;
	for (auto pair: m_nodes) {
		uint32_t dist = round(ns3::CalculateDistance(pair.second->GetPosition(), startingNodePos));
		if (dist > maxDist) {
			maxDist = dist;
			nodeId = pair.second->GetId();
		}
	}
	return m_nodes.at(nodeId)->GetReceived();
}

}



