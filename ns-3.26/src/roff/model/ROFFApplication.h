/*
 * ROFFApplication.h
 *
 *  Created on: Jan 28, 2019
 *      Author: jordan
 */

#ifndef ROFFAPPLICATION_H
#define ROFFAPPLICATION_H

#include "ROFFHeader.h"
#include "ROFFNode.h"
#include "Edge.h"
#include "ns3/application.h"
#include "ns3/network-module.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/object-ptr-container.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/constant-velocity-mobility-model.h"
#include "ns3/mobility-module.h"
#include "ns3/nstime.h"
#include "PositionRankingMap.h"
#include <boost/dynamic_bitset.hpp>
#include <set>

namespace ns3 {

/**
 * \ingroup network
 * \brief A special application that implements ROFF protocol.
 */
class ROFFApplication: public Application {

public:

	static TypeId GetTypeId();

//	virtual ~ROFFApplication();

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
	* \param errorRate Probability to incur in an error in transmission schedule (sending 1 slot earlier or later)
	* \param forgedCoordRate % of vehicles affected by forging
	* \param droneTest whether drones are present in the simulation
	* \return none
	*/
	void Install(uint32_t broadcastPhaseStart, uint32_t actualRange, uint32_t aoi,
			uint32_t aoi_error, uint32_t m_vehicleDistance, uint32_t beaconInterval,
			uint32_t m_distanceRange, uint32_t startingNode, uint32_t printCoords,
			uint32_t errorRate, uint32_t forgedCoordRate, uint32_t droneTest);

	/**
	* \brief Add a new node to the application and set up protocol parameters
	* \param node node to add
	* \param source source socket of the node
	* \param sink sink socket of the node
	* \param onstats if this node is a vehicle
	* \return none
	*/
	void AddNode(Ptr<Node> node, Ptr<Socket> source, Ptr<Socket> sink, bool onstats,
			bool isNodeInJunction, uint64_t junctionId = 0);

	/**
	* \brief Print value of some useful field
	* \param dataStream output data
	* \return none
	*/
	void PrintStats(std::stringstream& dataStream);

private:
	/**
	* \brief Application specific startup code
	*
	* The StartApplication method is called at the start time specified by Start
	* This method should be overridden by all or most application
	* subclasses.
	*/
	virtual void StartApplication(void);

	/**
	* \brief Application specific shutdown code
	*
	* The StopApplication method is called at the stop time specified by Stop
	* This method should be overridden by all or most application
	* subclasses.
	*/
	virtual void StopApplication(void);

	/**
	* \brief Start the estimation phase
	* \param count count
	* \return none
	*/
	void GenerateHelloTraffic(uint32_t count);

	/**
	 * \brief Generates forged hello messages with fake coords
	 * \return none
	 */
  	void GenerateForgedHelloTraffic();

	/**
	* \brief Start the broadcast phase
	* \return none
	*/
	void StartBroadcastPhase(void);

	/**
	 * \brief Send a Hello message to all nodes in its range
	 * \return none
	 */
	void GenerateHelloMessage(Ptr<ROFFNode> node);

	/**
	* \brief Send a Alert message
	* \return none
	*/
	void GenerateAlertMessage(Ptr<ROFFNode> node);

	/**
	* \brief Process a received packet
	* \param socket the receiving socket
	* \return none
	*/
	void ReceivePacket(Ptr<Socket> socket);

	/**
	* \brief Handle a Hello message
	* \param fbNode node that received the message
	* \param fbHeader header received in the message
	* \return none
	*/
	void HandleHelloMessage(Ptr<ROFFNode> node, ROFFHeader header);

	/**
	* \brief Handle an Alert message
	* \param fbNode node that received the message
	* \param fbHeader header received in the message
	* \param distance distance between the sender of the message and the node (meters)
	* \return none
	*/
	void HandleAlertMessage(Ptr<ROFFNode> node, ROFFHeader header, uint32_t distance);

	/**
	* \brief Forward an Alert message
	* \param fbNode node that received the message
	* \param fbHeader header received in the message
	* \param waitingTime contention window value
	* \param forceSend whether to force forwarding of message due to error in schedule
	* \return none
	*/
	void ForwardAlertMessage(Ptr<ROFFNode> node, ROFFHeader oldHeader, uint32_t waitingTime, bool forceSend);

	/**
	* \brief Stop a node
	* \param fbNode node to be stopped
	* \return none
	*/
	void StopNode(Ptr<ROFFNode> node);


	/**
	* \brief Compute contention window
	* \param maxRange estimated range (meters)
	* \param distance distance between nodes (meters)
	* \return the value of the contention window
	*/

	uint32_t ComputeWaitingTime(Ptr<ROFFNode> node, uint32_t distSenderToNode,
			PositionRankingMap rankingMap, uint32_t priority);

	uint32_t ComputeMinDiff(uint32_t distSenderToNode, uint32_t distSenderToAnotherNode);

	uint32_t ComputeWaitingTimeArmir(Ptr<ROFFNode> node, Vector senderCoords,
				PositionRankingMap rankingMap, uint32_t priority);

	uint32_t ComputeMinDiffArmir(Vector fwdCoords, Vector lowerPriorityNodeCoords,
			Vector higherPriorityNodeCoords);

	double ComputePropagationDelay(Vector coord1, Vector coord2);

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

//	Application data
	uint32_t							m_startingNode; // index of the node that will generate the Alert Message
	uint32_t							m_broadcastPhaseStart; // broadcast phase start time (seconds)
	uint32_t							m_actualRange; // real transmission range
	uint32_t							m_aoi; // radius of the area of interest (meters)
	uint32_t							m_aoi_error;	// meters +/- with respect to the radius
	uint32_t							m_packetPayload; // size of the packet payload
	uint32_t							m_vehicleDistance; //distance between vehicles
	uint32_t 							m_beaconInterval;
	uint32_t							m_distanceRange;
	Ptr<UniformRandomVariable> 			m_randomVariable;
	uint32_t 							m_errorRate; //probability to incur in an error in transmission schedule (sending 1 slot earlier or later)
	uint32_t							m_forgedCoordRate; // % of nodes which receive forged hello messages with fake coords
	uint32_t							m_droneTest;

	map<uint32_t, Ptr<ROFFNode>>		m_nodes; // nodes that run this application

//	Output data
	uint32_t							m_received;	// number of hello messages sent
	uint32_t							m_sent; // // number of alert messages sent
uint32_t								m_collisions; // number of collisions
	vector<uint32_t>					m_receivedNodes; // ids of nodes which have received alert messages, duplicates allowed
	uint32_t							m_printCoords; // 1 to print coordinates, 0 otherwise
	map<uint32_t, vector<uint32_t>>		m_transmissionList; //list to discover path of alert messages
	vector<Edge>						m_transmissionVector; //vector to discover paths of alert messages (single broadcasts ordered by time of reception)

};

}



#endif /* ROFFAPPLICATION_H_*/
