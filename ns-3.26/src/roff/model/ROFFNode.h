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


	ROFFNode(Ptr<Node> node, Ptr<Socket> socket, bool isNodeInJunction = false,
			uint64_t junctionId = 0, bool amIaVehicle = true);

	//	Getters
	Ptr<Node> GetNode() const;

	Ptr<Socket> GetSocket() const;

	bool GetReceived() const;

	bool GetSent() const;

	bool GetScheduled() const;

	uint32_t GetId() const;

	const Vector GetPosition() const;

	int32_t GetPhase() const;

	uint32_t GetSlot() const;

	uint32_t GetHop() const;

	Time GetTimestamp() const;

	bool AmIAVehicle() const;

	/**
	 * \returns true if the node is inside an junction
	 */
	bool AmIInJunction() const;

	/**
	 * \returns the id of the junction the node is in
	 */
	uint64_t GetJunctionId () const;


	bool GetStopSending  (void) const;



//	Setters
	void SetNode(Ptr<Node> node);

	void SetSocket(Ptr<Socket> socket);

	void SetReceived(bool received);

	void SetSent(bool sent);

	void SetScheduled(bool scheduled);

	void SetPhase(int32_t phase);

	void SetSlot(uint32_t m_slot);

	void SetHop(uint32_t m_hop);

	void SetTimestamp(Time timestamp);

	/**
	 * \brief set the node in junction
	 * \param value true if the node is in an junction
	 */
	void SetMeInJunction (bool value);

	/**
	 * \brief set the id of the junction the node is in
	 * \param JunctionId id of the junction the node is in
	 */
	void SetJunctionId (uint64_t junctionId);

	void SetStopSending (bool stopSending);

	void SetMeAsVehicle(bool vehicle);

//	Methods
	void Send(Ptr<Packet> packet);

	void AddOrUpdateNeighbor(uint32_t nodeId, Vector pos, Time timeStamp = Simulator::Now());

	uint32_t GetNBTSize() const;

	boost::dynamic_bitset<> GetESDBitmap(uint32_t distanceRange) const;

	Vector GetCoordsOfVehicleInRange(PositionRankingKey range, Vector senderCoords, int32_t& dist) const;

	bool IsNodeWinnerInContention(uint32_t dist, Vector pos) const;

	const Vector UpdatePosition();


private:

	Ptr<Node>			m_node; // ns-3 node
	Ptr<Socket> 		m_socket; // ns-3 socket
	NeighborTable		m_neighborTable;
	bool				m_received; // whether the node has received an alert message
	bool				m_sent; // whether the node has sent an alert message
	bool				m_scheduled; // whether the node has already scheduled a forward of an alert message
	int32_t				m_phase; // latest phase heard by the node
	Vector				m_position; // position of node
	uint32_t			m_slot; // number of slots waited before the node has received the alert message
	uint32_t			m_hop; // number of hops before the alert message reached this node
	Time				m_timestamp; // time of reception of alert message
	bool				m_amIaVehicle;	// used for statistics
	bool				m_amIInJunction; // whether the node is inside a junction
	uint64_t			m_junctionId; // id of the junction where the node is
	bool				m_stopSending;
};

}



#endif /* ROFFNODE_H*/
