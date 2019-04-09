/*
 * ROFFNode.cc
 *
 *  Created on: Jan 28, 2019
 *      Author: jordan
 */

#include "ROFFNode.h"

namespace ns3 {
	NS_LOG_COMPONENT_DEFINE("ROFFNode");

	NS_OBJECT_ENSURE_REGISTERED(ROFFNode);

	ROFFNode::ROFFNode(): m_received(false), m_sent(false), m_phase(0), m_slot(0), m_hop(0) {
		NS_LOG_FUNCTION(this);
	}

	ROFFNode::ROFFNode(Ptr<Node> node, Ptr<Socket> socket): m_node(node), m_socket(socket), m_neighborTable(),
			m_received(false), m_sent(false), m_phase(-1), m_slot(0), m_hop(0) {
		NS_LOG_FUNCTION(this);
	}

//	Getters
	Ptr<Node> ROFFNode::GetNode() const {
		return m_node;
	}

	Ptr<Socket> ROFFNode::GetSocket() const {
		return m_socket;
	}

	bool ROFFNode::GetReceived() const {
		return m_received;
	}

	bool ROFFNode::GetSent() const {
		return m_sent;
	}

	uint32_t ROFFNode::GetId() const {
		return m_node->GetId();
	}

	const Vector ROFFNode::GetPosition() const {
		return m_position;
	}

	int32_t ROFFNode::GetPhase() const {
		return m_phase;
	}

	uint32_t ROFFNode::GetSlot() const {
		return m_slot;
	}

	uint32_t ROFFNode::GetHop() const {
		return m_hop;
	}

	Time ROFFNode::GetTimestamp() const {
		return m_timestamp;
	}

//	void ROFFNode::SetPosition(const Vector& position) {
//		m_position = position;
//	}


//	Setters
	void ROFFNode::SetNode(Ptr<Node> node) {
		m_node = node;
	}

	void ROFFNode::SetSocket(Ptr<Socket> socket) {
		m_socket = socket;
	}

	void ROFFNode::SetReceived(bool received) {
//		cout << "ROFFNode::SetReceived" << received << endl;
		m_received = received;
	}

	void ROFFNode::SetSent(bool sent) {
		m_sent = sent;
	}

	void ROFFNode::SetPhase(int32_t phase) {
		m_phase = phase;
	}

	void ROFFNode::SetSlot(uint32_t slot) {
		m_slot = slot;
	}

	void ROFFNode::SetHop(uint32_t hop) {
		m_hop = hop;
	}

	void ROFFNode::SetTimestamp(Time timestamp) {
		m_timestamp = timestamp;
	}



//	Methods

	void ROFFNode::Send(Ptr<Packet> packet) {
		m_socket->Send(packet);
	}


	void ROFFNode::AddOrUpdateNeighbor(uint32_t nodeId, Vector pos, Time timeStamp) {
		NS_LOG_FUNCTION(this << nodeId << pos << timeStamp);
		m_neighborTable.AddOrUpdateEntry(nodeId, pos, timeStamp);
	}

	uint32_t ROFFNode::GetNBTSize() const {
		return m_neighborTable.GetNBTSize();
	}

	boost::dynamic_bitset<> ROFFNode::GetESDBitmap(uint32_t distanceRange) const {
		NS_LOG_FUNCTION("distanceRange=" << distanceRange);
		Vector thisNodePos = GetPosition();
		return m_neighborTable.GetESDBitmap(thisNodePos, distanceRange);
	}

	Vector ROFFNode::GetCoordsOfVehicleInRange(PositionRankingKey range, Vector senderCoords, int32_t& dist) const {
		return m_neighborTable.GetCoordsOfVehicleInRange(range, senderCoords, dist);
	}

	bool ROFFNode::IsNodeWinnerInContention(uint32_t dist, Vector pos) const {
		return m_neighborTable.IsNodeWinnerInContention(GetId(), dist, pos);
	}

	const Vector ROFFNode::UpdatePosition() {
		NS_LOG_FUNCTION(this);
//		cout << "ROFFNode::GetPosition" << endl;
//		m_node->GetObject<MobilityModel>();
//		cout << "ROFFNode::GetPosition 2" << endl;
		Vector pos = m_node->GetObject<MobilityModel>()->GetPosition();
		m_position = pos;
		return pos;
	}

}



