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
#include "ns3/log.h"
#include "ns3/mobility-model.h"
#include "ns3/constant-velocity-mobility-model.h"

#include "NeighborTable.h"
#include "PositionRankingKey.h"
#include <iostream>
#include <boost/dynamic_bitset.hpp>


using namespace std;
using namespace std::chrono;

namespace ns3 {

class ROFFNode: public Object {

public:

	ROFFNode();


	ROFFNode(Ptr<Node> node, Ptr<Socket> socket);

	//	Getters
	Ptr<Node> GetNode() const;

	Ptr<Socket> GetSocket() const;

	bool GetReceived() const;

	bool GetSent() const;

	uint32_t GetId() const;

	const Vector GetPosition() const;

	int32_t GetPhase() const;

//	Setters
	void SetNode(Ptr<Node> node);

	void SetSocket(Ptr<Socket> socket);

	void SetReceived(bool received);

	void SetSent(bool sent);

	void SetPhase(int32_t phase);

//	Methods
	void Send(Ptr<Packet> packet);

	void AddOrUpdateNeighbor(uint32_t nodeId, Vector pos, Time timeStamp = Simulator::Now());

	uint32_t GetNBTSize() const;

	boost::dynamic_bitset<> GetESDBitmap(uint32_t distanceRange) const;

	Vector GetCoordsOfVehicleInRange(PositionRankingKey range, Vector senderCoords, int32_t& dist) const;

	bool IsNodeWinnerInContention(uint32_t dist, Vector pos) const;

private:

	Ptr<Node>			m_node; // ns-3 node
	Ptr<Socket> 		m_socket; // ns-3 socket
	NeighborTable		m_neighborTable;
	bool				m_received; // whether the node has received an alert message
	bool				m_sent; // whether the node has sent an alert message
	int32_t				m_phase; // number of hops before the node received the alert message

};

}



#endif /* ROFFNODE_H*/
