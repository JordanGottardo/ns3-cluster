/*
 * ROFFNode.h
 *
 *  Created on: Jan 28, 2019
 *      Author: jordan
 */

#ifndef ROFFNODE_H
#define ROFFNODE_H

#include "ns3/core-module.h"
#include "ns3/node.h"
#include "ns3/object.h"
#include "ns3/vector.h"
#include "ns3/socket.h"
#include "ns3/packet.h"
#include "NeighborTable.h"
#include <iostream>

using namespace std;
using namespace std::chrono;

namespace ns3 {

class ROFFNode: public Object {

public:
	const Ptr<Node>& GetNode() const;

	void SetNode(const Ptr<Node>& node);

	const Vector& GetPosition() const;

	void SetPosition(const Vector& position);

	void AddOrUpdateNeighbor(const uint32_t& nodeId, Vector pos, milliseconds timeStamp = duration_cast<milliseconds>(
		    system_clock::now().time_since_epoch()));

private:

	Ptr<Node>			m_node; // ns-3 node
	Vector				m_position; // node current position
	NeighborTable		m_neighborTable;
//						m_esdBitmap boost dynamic_bitset
//	TODO 				esdBitmap size?
};

}



#endif /* ROFFNODE_H*/
