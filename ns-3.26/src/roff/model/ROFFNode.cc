/*
 * ROFFNode.cc
 *
 *  Created on: Jan 28, 2019
 *      Author: jordan
 */

#include "ROFFNode.h"

namespace ns3 {

	ROFFNode::ROFFNode() {
	}

	ROFFNode::ROFFNode(Ptr<Socket> socket): m_socket(socket) {
	}

	const Ptr<Node>& ROFFNode::GetNode() const {
		return m_node;
	}

	Ptr<Socket> ROFFNode::GetSocket() const {
		return m_socket;
	}

	uint32_t ROFFNode::GetId() const {
		return m_node->GetId();
	}

//	const Vector& ROFFNode::GetPosition() const {
//		return m_position;
//	}

//	void ROFFNode::SetPosition(const Vector& position) {
//		m_position = position;
//	}


	void ROFFNode::SetSocket(Ptr<Socket> socket) {
		m_socket = socket;
	}

	void ROFFNode::SetNode(const Ptr<Node>& node) {
		m_node = node;
	}

	void ROFFNode::AddOrUpdateNeighbor(const uint32_t& nodeId, Vector pos, milliseconds timeStamp) {
		m_neighborTable.AddOrUpdateEntry(nodeId, pos, timeStamp);
	}

}



