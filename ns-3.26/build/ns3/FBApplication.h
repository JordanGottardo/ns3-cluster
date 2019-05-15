/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2017 University of Padova
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
 * Author: Marco Romanelli <marco.romanelli.1@studenti.unipd.it>
 *
 */

#ifndef FBAPPLICATION_H
#define FBAPPLICATION_H

#include "FBHeader.h"
#include "FBNode.h"
#include "ns3/application.h"
#include "ns3/network-module.h"
#include "ns3/object-vector.h"
#include "Edge.h"
#include <chrono>
#include <ctime>
#include <set>

using namespace std;


namespace ns3 {

static const uint32_t PROTOCOL_FB = 0;
static const uint32_t PROTOCOL_STATIC_100 = 100;
static const uint32_t PROTOCOL_STATIC_300 = 300;
static const uint32_t PROTOCOL_STATIC_500 = 500;

/**
 * \ingroup network
 * \brief A special application that implements Fast Broadcast protocol.
 */
class FBApplication : public Application
{
public:
	/**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);

	FBApplication ();
	virtual ~FBApplication();

	 /**
	 * \brief Set up some application parameters
	 * \param protocol fb or static protocol
	 * \param broadcastPhaseStart time after which broadcast phase will start (seconds)
	 * \param actualRange actual transmission range (meters)
	 * \param aoi radius of area of interest (meters)
	 * \param aoi_error distance +/- with respect to the radius (meters)
	 * \param flooding enable or disable flooding
	 * \param cwMin minumum size of the contention window (slots)
	 * \param cwMin maximum size of the contention window (slots)
	 * \param printCoords whether to print coordinates on file (1=true, 0=false)
	 * \param vehicleDistance distance between vehicles
	 * \param errorRate Probability to incur in an error in transmission schedule (sending 1 slot earlier or later)
	 * \param forgedCoordRate % of vehicles affected by forging
	 * \param droneTest whether drones are present in the simulation
	 * \return none
	 */
	virtual void Install (uint32_t protocol, uint32_t broadcastPhaseStart, uint32_t actualRange, uint32_t aoi,
				  uint32_t aoi_error, bool flooding, uint32_t cwMin, uint32_t cwMax, uint32_t printCoords,
				  uint32_t vehicleDistance, uint32_t errorRate, uint32_t forgedCoordRate, uint32_t droneTest);

	/**
	 * \brief Add a new node to the application and set up protocol parameters
	 * \param node node to add
	 * \param source source socket of the node
	 * \param sink sink socket of the node
	 * \param onstats if this node is a vehicle
	 * \param isNodeinIntersection if this node is inside an intersection
	 * \param intersectionId id of the intersection the node is inside, 0 if none
	 * \return none
	 */
	void AddNode (Ptr<Node> node, Ptr<Socket> source, Ptr<Socket> sink, bool onstats,
			bool isNodeInJunction, uint64_t junctionId = 0);

	/**
	 * \brief Print value of some useful field
	 * \param dataStream output data
	 * \return none
	 */
	void PrintStats (stringstream &dataStream);

private:

	void LogCollision(std::string context, Ptr<const Packet> p);

	/**
	 * \brief Application specific startup code
	 *
	 * The StartApplication method is called at the start time specified by Start
	 * This method should be overridden by all or most application
	 * subclasses.
	 */
	virtual void StartApplication (void);

	/**
	 * \brief Application specific shutdown code
	 *
	 * The StopApplication method is called at the stop time specified by Stop
	 * This method should be overridden by all or most application
	 * subclasses.
	 */
    virtual void StopApplication (void);

	/**
	 * \brief Generates forged hello messages with fake coords
	 * \return none
	 */
  	void GenerateForgedHelloTraffic();

	/**
	 * \brief Start the estimation phase
	 * \param count count
	 * \return none
	 */
	void GenerateHelloTraffic (uint32_t count);

	/**
	 * \brief Start the broadcast phase
	 * \return none
	 */
	void StartBroadcastPhase (void);

	/**
	 * \brief Send a Hello message to all nodes in its range
	 * \return none
	 */
	void GenerateHelloMessage (Ptr<FBNode> fbNode);

	/**
	 * \brief Send a Alert message
	 * \return none
	 */
	void GenerateAlertMessage (Ptr<FBNode> fbNode);

	/**
   * \brief Process a received packet
   * \param socket the receiving socket
   * \return none
   */
	void ReceivePacket (Ptr<Socket> socket);

	/**
	 * \brief Handle a Hello message
	 * \param fbNode node that received the message
	 * \param fbHeader header received in the message
	 * \return none
	 */
	void HandleHelloMessage (Ptr<FBNode> fbNode, FBHeader fbHeader);

	/**
	 * \brief Handle an Alert message
	 * \param fbNode node that received the message
	 * \param fbHeader header received in the message
	 * \param distance distance between the sender of the message and the node (meters)
	 * \return none
	 */
	void HandleAlertMessage (Ptr<FBNode> fbNode, FBHeader fbHeader);

	/**
	 * \brief Wait a specific amount of time
	 * \param fbNode node that received the message
	 * \param fbHeader header received in the message
	 * \param waitingTime contention window value
	 * \return none
	 */
	void WaitAgain (Ptr<FBNode> fbNode,  FBHeader fbHeader, uint32_t waitingTime);

	/**
	 * \brief Forward an Alert message
	 * \param fbNode node that received the message
	 * \param fbHeader header received in the message
	 * \param waitingTime contention window value
	 * \param forceSend whether to force forwarding of message due to error in schedule
	 * \return none
	 */
	void ForwardAlertMessage (Ptr<FBNode> fbNode, FBHeader oldFBHeader, uint32_t waitingTime, bool forceSend);

	/**
	 * \brief Stop a node
	 * \param fbNode node to be stopped
	 * \return none
	 */
	void StopNode (Ptr<FBNode> fbNode);

	/**
	 * \brief Retrieve a fbNode from a ns3::Node
	 * \param node original node
	 * \return a fbNode
	 */
	Ptr<FBNode> GetFBNode (Ptr<Node> node);

	/**
	 * \brief Retrieve a fbNode from a ns3::Node
	 * \param id original node id
	 * \return a fbNode
	 */
	Ptr<FBNode> GetFBNode (uint32_t id);

	/**
	 * \brief Compute contention window
	 * \param maxRange estimated range (meters)
	 * \param distance distance between nodes (meters)
	 * \return the value of the contention window
	 */
	uint32_t ComputeContetionWindow (uint32_t maxRange, uint32_t distance);

	int32_t ComputeErrorDelay();

	/**
	* \brief Returns a string from a vector
	* \return the string with the content of the vector
	 */
	template <typename T>
	string StringifyVector(const vector<T>& v);

	/**
	* \brief Returns a string representation of m_transmissionMap
	* \return  a string representation of m_transmissionMap	 */
	string StringifyTransmissionMap() const;

	/**
	* \brief Returns whether the most distant node from m_startingNode has been reached and also calculates maxDist
	* \param maxDist max distance between a node and m_startingNode)
	* \return  whether the most distant node from m_startingNode has been reached	 */
	uint32_t IsMaxDistNodeReached(uint32_t& maxDist) const;

private:
	uint32_t																m_nNodes;	// number of nodes
	vector<Ptr<FBNode>>														m_nodes;	// nodes that run this application
	map<uint32_t, uint32_t> 												m_id2id;	// map node id with index in m_nodes
	uint32_t																m_startingNode; // index of the node that will generate the Alert Message
	bool																	m_staticProtocol;	// true if static protocol is used
	uint32_t													 			m_broadcastPhaseStart;	// broadcast phase start time (seconds)
	uint32_t													 			m_cwMin;	// min size of the contention window (in slot)
	uint32_t													 			m_cwMax;	// max size of the contention window (in slot)
	bool															 		m_flooding;	// used for control the flooding of the Alert messages
	uint32_t													 			m_actualRange;	// real transmission range
	uint32_t													 			m_estimatedRange;	// range of transmission to be estimated
	uint32_t																m_aoi;	// radius of the area of interest (meters)
	uint32_t																m_aoi_error;	// meters +/- with respect to the radius
	uint32_t													 			m_packetPayload; // size of the packet payload
	uint32_t													 			m_received;	// number of hello messages sent
	uint32_t																m_sent; // // number of alert messages sent
	uint32_t																m_cwndSum;
	uint32_t																m_cwndCount;
	uint32_t																m_errorRate; //probability to incur in an error in transmission schedule (sending 1 slot earlier or later)
	Ptr<UniformRandomVariable> 												m_randomVariable;
	uint32_t																m_forgedCoordRate; // % of nodes which receive forged hello messages with fake coords
	uint32_t																m_droneTest;

	uint32_t																m_collisions; // number of collisions
	vector<uint32_t>														m_receivedNodes; // ids of nodes which have received alert messages, duplicates allowed
	uint32_t																m_printCoords; // 1 to print coordinates, 0 otherwise
	uint32_t																m_vehicleDistance; //distance between vehicles
	map<uint32_t, vector<uint32_t>>											m_transmissionList; //list to discover path of alert messages
	vector<Edge>															m_transmissionVector; //vector to discover paths of alert messages (single broadcasts ordered by time of reception)

	//	TransmissionList														m_transmissionList; //list to discover path of alert messages
//	int																		counter = 0;
};

} // namespace ns3

#endif /* FBAPPLICATION_H */
