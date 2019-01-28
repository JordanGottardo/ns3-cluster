/*
 * ROFFNode.cc
 *
 *  Created on: Jan 28, 2019
 *      Author: jordan
 */

#include "ROFFNode.h"

namespace ns3 {

	const Ptr<Node>& ROFFNode::GetNode() const {
		return m_node;
	}

	void ROFFNode::SetNode(const Ptr<Node>& node) {
		m_node = node;
	}

	const Vector& ROFFNode::GetPosition() const {
		return m_position;
	}

	void ROFFNode::SetPosition(const Vector& position) {
		m_position = position;
	}

}



