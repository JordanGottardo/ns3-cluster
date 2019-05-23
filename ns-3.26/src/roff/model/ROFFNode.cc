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

	ROFFNode::ROFFNode(): m_received(false), m_sent(false), m_scheduled(false),
			m_phase(0), m_slot(0), m_hop(0), m_amIaVehicle(true), m_amIInJunction(false), m_junctionId(0), m_stopSending(false) {
		NS_LOG_FUNCTION(this);
	}

	ROFFNode::ROFFNode(Ptr<Node> node, Ptr<Socket> socket, bool isNodeInJunction, uint64_t junctionId, bool amIaVehicle):
			m_node(node), m_socket(socket), m_amIInJunction(isNodeInJunction), m_junctionId(junctionId), m_amIaVehicle(amIaVehicle),
			m_neighborTable(), m_received(false), m_sent(false), m_scheduled(false), m_phase(-1), m_slot(0), m_hop(0),
			m_stopSending(false) {
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

	bool ROFFNode::GetScheduled() const {
		return m_scheduled;
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

	bool ROFFNode::AmIInJunction() const {
		NS_LOG_FUNCTION (this);
		return m_amIInJunction;
	}

	uint64_t ROFFNode::GetJunctionId() const {
		NS_LOG_FUNCTION (this);
		return m_junctionId;
	}

	bool ROFFNode::GetStopSending (void) const	{
		NS_LOG_FUNCTION (this);
		return m_stopSending;
	}

	bool ROFFNode::AmIAVehicle() const {
		return m_amIaVehicle;
	}

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

	void ROFFNode::SetScheduled(bool scheduled) {
		m_scheduled = scheduled;
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

	void ROFFNode::SetMeInJunction (bool value) {
		NS_LOG_FUNCTION (this << value);
		m_amIInJunction = value;
	}

	void ROFFNode::SetJunctionId (uint64_t junctionId) {
		NS_LOG_FUNCTION (this << junctionId);
		m_junctionId = junctionId;
	}

	void ROFFNode::SetStopSending (bool stopSending) {
		NS_LOG_FUNCTION (this << stopSending);
		m_stopSending = stopSending;
	}

	void ROFFNode::SetMeAsVehicle(bool vehicle) {
		m_amIaVehicle = vehicle;
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



