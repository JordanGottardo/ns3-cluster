/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2017 Università di Padova
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors: Marco Romanelli <marco.romanelli.1@studenti.unipd.it>
 */

#ifndef FBNODE_H
#define FBNODE_H

#include "ns3/core-module.h"
#include "ns3/node.h"
#include "ns3/object.h"
#include "ns3/vector.h"
#include "ns3/socket.h"
#include "ns3/packet.h"


namespace ns3 {

/**
 * \ingroup network
 * \brief A special node used in Fast Broadcast protocol.
 */
class FBNode : public Object
{
public:
	static TypeId GetTypeId (void);

	FBNode();
	virtual ~FBNode();

	/**
	 * \returns the FBNode identifier
	 */
	uint32_t GetId (void) const;

	/**
	 * \returns the node included in the FBNode
	 */
	Ptr<Node> GetNode (void) const;

	/**
   * \returns the value of the CMFR field
   */
	uint32_t GetCMFR (void) const;

	/**
	 * \returns the value of the LMFR field
	 */
	uint32_t GetLMFR (void) const;

	/**
	 * \returns the value of the CMBR field
	 */
	uint32_t GetCMBR (void) const;

	/**
	 * \returns the value of the LMBR field
	 */
	uint32_t GetLMBR (void) const;

	/**
	 * \returns the position of the node
	 */
	Vector GetPosition (void) const;

	/**
	 * \returns the num of the node
	 */
	uint32_t GetHop (void) const;

	/**
	 * \returns the phase of the node
	 */
	int32_t GetPhase (void) const;

	/**
	 * \returns the slot of the node
	 */
	uint32_t GetSlot (void) const;

	/**
	 * \returns true if the node has received an alert
	 */
	bool GetReceived (void) const;

	/**
	 * \returns true if the node has sent an alert
	 */
	bool GetSent (void) const;

	/**
	 * \returns the number of microseconds in the timestamp
	 */
	Time GetTimestamp (void) const;

	/**
	 * \returns true if the node is set as vehicle
	 */
	bool AmIaVehicle (void) const;

	/**
	 * \returns true if the node is inside an junction
	 */
	bool AmIInJunction (void) const;

	/**
	 * \returns the id of the junction the node is in
	 */
	uint64_t GetJunctionId  (void) const;


	bool GetStopSending  (void) const;

	/**
	 * \brief set the node id
	 * \param value id
	 */
	void SetId (uint32_t value);

	/**
	 * \brief set the node
	 * \param node ns-3 node
	 */
	void SetNode (Ptr<Node> node);

	/**
	 * \brief set the socket of the node
	 * \param socket internet socket
	 */
	void SetSocket (Ptr<Socket> socket);

	/**
	 * \brief set the value of the CMFR field
	 * \param value new value of CMFR
	 */
	void SetCMFR (uint32_t value);

	/**
	 * \brief set the value of the LMFR field
	 * \param value new value of LMFR
	 */
	void SetLMFR (uint32_t value);

	/**
	 * \brief set the value of the CMBR field
	 * \param value new value of CMBR
	 */
	void SetCMBR (uint32_t value);

	/**
	 * \brief set the value of the LMBR field
	 * \param value new value of LMBR
	 */
	void SetLMBR (uint32_t value);

	/**
	 * \brief update it's (node) current position
	 * \returns the new position of the node
	 */
	Vector UpdatePosition (void);

	/**
	 * \brief set the num
	 * \param value new value of num
	 */
	void SetHop (uint32_t n);

	/**
	 * \brief set the phase
	 * \param value new value of phase
	 */
	void SetPhase(int32_t value);

	/**
	 * \brief set the slot
	 * \param value new value of slot
	 */
	void SetSlot (uint32_t value);

	/**
	 * \brief set the received field
	 * \param value boolean value
	 */
	void SetReceived (bool value);

	/**
	 * \brief set the sent field
	 * \param value boolean value
	 */
	void SetSent (bool value);

	/**
	 * \brief send a packet
	 * \param packet packet to send
	 */
	void Send (Ptr<Packet> packet);

	/**
	 * \brief set the timestamp field
	 * \param value number of microseconds
	 */
	void SetTimestamp (Time value);

	/**
	 * \brief set the node as a vehicle
	 * \param value true if the node has to be set as a vehicles
	 */
	void SetMeAsVehicle (bool value);

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

	uint32_t								m_id;	// node id
	Ptr<Node>								m_node;	// ns-3 node
	Ptr<Socket> 							m_socket; // ns-3 socket
	uint32_t 	  							m_CMFR;	// Current Maximum Front Range
	uint32_t 	  							m_LMFR;	// Last Maximum Front Range
	uint32_t 	  							m_CMBR;	// Current Maximum Back Range
	uint32_t 	  							m_LMBR; // Last Maximum Back Range
	Vector 									m_position;	// node current position
	uint32_t								m_hop;	// number of hops before the alert message reached this node
	int32_t									m_phase;	// legacy with barichello's code
	uint32_t								m_slot;	// legacy with barichello's code
	bool 									m_received;	// legacy with barichello's code
	bool 									m_sent;	// legacy with barichello's code
	Time									m_timestamp;	// used for statistics
	bool									m_amIaVehicle;	// used for statistics
	bool									m_amIInJunction; // whether the node is inside a junction
	uint64_t								m_junctionId; // id of the junction where the node is
	bool									m_stopSending;
};

} // namespace ns3

#endif /* FBNODE_H */
