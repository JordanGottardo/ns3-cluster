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

	ROFFNode::ROFFNode() {
		NS_LOG_FUNCTION(this);
	}

	ROFFNode::ROFFNode(Ptr<Node> node, Ptr<Socket> socket): m_node(node), m_socket(socket) {
		NS_LOG_FUNCTION(this);
	}

	Ptr<Node> ROFFNode::GetNode() const {
		return m_node;
	}

	Ptr<Socket> ROFFNode::GetSocket() const {
		return m_socket;
	}

	uint32_t ROFFNode::GetId() const {
		return m_node->GetId();
	}

	const Vector ROFFNode::GetPosition() const {
		return m_node->GetObject<MobilityModel>()->GetPosition();
	}

//	void ROFFNode::SetPosition(const Vector& position) {
//		m_position = position;
//	}


	void ROFFNode::SetSocket(Ptr<Socket> socket) {
		m_socket = socket;
	}

	void ROFFNode::SetNode(Ptr<Node> node) {
		m_node = node;
	}

//	Methods

	void ROFFNode::Send(Ptr<Packet> packet) {
		m_socket->Send(packet);
	}


	void ROFFNode::AddOrUpdateNeighbor(uint32_t nodeId, Vector pos, Time timeStamp) {
		m_neighborTable.AddOrUpdateEntry(nodeId, pos, timeStamp);
	}

}



