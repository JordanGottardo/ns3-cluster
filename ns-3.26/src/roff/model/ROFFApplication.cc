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
	uint32_t aoi_error, uint32_t vehicleDistance) {
	NS_LOG_FUNCTION(this);
	m_broadcastPhaseStart = broadcastPhaseStart;
	m_aoi = aoi;
	m_aoi_error = aoi_error;
	m_actualRange = actualRange;
	m_vehicleDistance = vehicleDistance;
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
	GenerateHelloTraffic(0);
//	Simulator::Schedule(Seconds(m_broadcastPhaseStart), &ROFFApplication::StartBroadcastPhase, this);
}

void ROFFApplication::StopApplication(void) {
	NS_LOG_FUNCTION(this);
}

void ROFFApplication::GenerateHelloTraffic(uint32_t count) {
	NS_LOG_FUNCTION(this);
	Ptr<ROFFNode> roffNode = m_nodes.at(0);
	Simulator::Schedule(Seconds(2), &ROFFApplication::GenerateHelloMessage, this, roffNode);
//	TODO
//	Generate some hello traffic: how often should we send beacons?
//	Each vehicle should send beacons every x ms
//	Evaluate whether to use redundant beacon transmissions
}

void ROFFApplication::StartBroadcastPhase(void) {
	NS_LOG_FUNCTION(this);
	GenerateAlertMessage(m_nodes.at(m_startingNode));
}

void ROFFApplication::GenerateHelloMessage(Ptr<ROFFNode> node) {
	uint32_t headerType = HELLO_MESSAGE;
	uint32_t nodeId = node->GetId();
	Vector position = node->GetPosition();

	ROFFHeader header(headerType, nodeId, position);

	Ptr<Packet> packet = Create<Packet>(m_packetPayload);
	packet->AddHeader(header);
	node->Send(packet);

}

void ROFFApplication::GenerateAlertMessage(Ptr<ROFFNode> node) {
	NS_LOG_FUNCTION (this << node);

// TODO
//	Generate alert message
//	Generate ESD bitmap from neighbor table (NBT)
//		ESI bitmap size is delta+1, where delta is the maximum distance between the
//		the node and the PFCs in the NBT
// Include ESD bitmap and node's current position in alert message
//	Broadcast alert message
//
	ROFFHeader header;
	Vector pos = node->GetPosition();
	cout << pos << endl;
	header.SetPosition(node->GetPosition());

	Ptr<Packet> packet = Create<Packet>(m_packetPayload);
//	cout << "add " << node->GetPosition() << endl;
	packet->AddHeader(header);
	node->Send(packet);
}

void ROFFApplication::ReceivePacket(Ptr<Socket> socket) {
	NS_LOG_FUNCTION(this << socket);

	Ptr<Node> node = socket->GetNode();
	Ptr<ROFFNode> roffNode = m_nodes.at(node->GetId());
	cout << "received packet by node " << node->GetId() << " at time=" << Simulator::Now().GetSeconds() << endl;
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
//			HandleAlertMessage(node, header, distance)
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
		ROFFNode header, uint32_t distance) {
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

void ROFFApplication::WaitAgain(Ptr<ROFFNode> fbNode, ROFFNode header,
		uint32_t waitingTime) {
}

void ROFFApplication::ForwardAlertMessage(Ptr<ROFFNode> fbNode,
		ROFFNode oldHeader, uint32_t waitingTime) {
// TODO
//	broadcast alert message
}

void ROFFApplication::StopNode(Ptr<ROFFNode> fbNode) {
}


uint32_t ROFFApplication::ComputeContentionWindow(uint32_t maxRange,
		uint32_t distance) {
}


}


