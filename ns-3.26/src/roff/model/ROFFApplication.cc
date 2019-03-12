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

	for (int i = 0; i < 3; i++) {
		for (auto entry: m_nodes) {
			Ptr<ROFFNode> roffNode = entry.second;
			// Generate random waiting time between [0, beaconInterval*2]. Average is beaconInterval
			uint32_t waitingTime = m_randomVariable->GetInteger(0, m_beaconInterval * 2);
			Simulator::Schedule(MilliSeconds(waitingTime), &ROFFApplication::GenerateHelloMessage, this, roffNode);
		}
	}
}

void ROFFApplication::StartBroadcastPhase(void) {
	NS_LOG_FUNCTION(this);
	for (auto entry: m_nodes) {
		Ptr<ROFFNode> roffNode = entry.second;
		cout << "ROFFApplication::StartBroadcastPhase nbt size= " << roffNode->GetNBTSize() << endl;
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
	cout << "ROFFApplication::Generate ALert message " << endl;

	uint32_t headerType = ALERT_MESSAGE;
	uint32_t nodeId = node->GetId();
	Vector position = node->GetPosition();
	boost::dynamic_bitset<> esdBitmap = node->GetESDBitmap(m_distanceRange);
//	cout << "ROFFApplication::GenerateAlertMessage esdBitmap = " << esdBitmap << " size= " <<
//			esdBitmap.size() << endl;
	ROFFHeader header(headerType, nodeId, position, esdBitmap);

	Ptr<Packet> packet = Create<Packet>(m_packetPayload);
//	cout << "add " << node->GetPosition() << endl;
	packet->AddHeader(header);
	node->Send(packet);
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
//		cout << "received packet by node " << node->GetId() <<
//				" from node in pos " << senderPosition << " distance= " << distance << endl;

		uint32_t packetType = header.GetType();
		if (packetType == HELLO_MESSAGE) {
			HandleHelloMessage(roffNode, header);
		} else if (packetType == ALERT_MESSAGE) {
			HandleAlertMessage(roffNode, header, distance);
		} else {
			NS_LOG_ERROR("Packet type not recognized");
		}
	}
//	TODO
//	Check if hello or alert
//		if hello, handle hello message
//		if alert, handle alert message
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
	boost::dynamic_bitset<> esdBitmap =  header.GetESDBitmap();
	Vector senderPosition = header.GetPosition();
	Vector nodePosition = node->GetPosition();
	uint32_t dist = rint(ns3::CalculateDistance(senderPosition, nodePosition));
	uint32_t posToCheck = dist / m_distanceRange;
	if (esdBitmap[posToCheck] == 0) {
		return;
	}
	PositionRankingMap rankingOfPositions = CreatePositionsRanking(esdBitmap);
	uint32_t priority = rankingOfPositions.GetPriority(dist);
	uint32_t waitingTime = ComputeWaitingTime(node, dist, rankingOfPositions, priority);

	// calcola distanza tra node e header.position
	// controlla se in header.esdBitmap[position*distanceRange ecc) è uguale a 1
	// se sì, node è papabile all'inoltro
	// altrimenti, node deferra e non invia

//	TODO
//	initiate forward priority acquisition
//	after that, calculate minDiff
//		then wait minDiff
//		then broadcast the message
}

double ROFFApplication::CalculateMinDiff() {
//	TODO
//	calculate minDiff between nodes
}


void ROFFApplication::ForwardAlertMessage(Ptr<ROFFNode> fbNode,
		ROFFNode oldHeader, uint32_t waitingTime) {
// TODO
//	broadcast alert message
}

void ROFFApplication::StopNode(Ptr<ROFFNode> fbNode) {
}



PositionRankingMap ROFFApplication::CreatePositionsRanking(boost::dynamic_bitset<> esdBitmap) {
	PositionRankingMap rankingMap(m_distanceRange);
	uint32_t priority = 1;
	for (uint32_t i = 0; i < esdBitmap.size(); i++) {
		if (esdBitmap[i] == 1) {
			uint32_t index = esdBitmap.size() - i - 1;
			rankingMap.AddEntry(index, priority);
			priority++;
		}
	}
	return rankingMap;
}

uint32_t ROFFApplication::ComputeWaitingTime(Ptr<ROFFNode> node, uint32_t distSenderToNode,
		PositionRankingMap rankingMap, uint32_t priority) {
	uint32_t waitingTime = 0;
	for (uint32_t pr = priority - 1; pr > 0; pr ++) {
		uint32_t upperDistanceLimit = rankingMap.GetUpperDistanceLimit(pr);
		waitingTime += ComputeMinDiff(distSenderToNode, upperDistanceLimit);
	}
	return waitingTime;
}

uint32_t ROFFApplication::ComputeMinDiff(uint32_t distSenderToNode, uint32_t distSenderToAnotherNode) {
	uint32_t rxTx = 2;
	uint32_t ccaTime = 8;
	uint32_t c = 299792458; //speed of light
	uint32_t maxDist = distSenderToNode + distSenderToAnotherNode;
	uint32_t propagationDelay = (((double)maxDist) / ((double)c)) * 1000 * 1000; //in microseconds
	return  propagationDelay + rxTx + ccaTime;
}

uint32_t ROFFApplication::ComputeMinDiffArmir() {
	//todo
}

}



