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

#include "FBHeader.h"
#include "FBNode.h"
#include "FBApplication.h"

#include <numeric>
#include <math.h>
#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/object-ptr-container.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/constant-velocity-mobility-model.h"
#include "ns3/mobility-module.h"


using namespace std;


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("FBApplication");

NS_OBJECT_ENSURE_REGISTERED (FBApplication);

TypeId
FBApplication::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::FBApplication")
    .SetParent<Application>()
    .SetGroupName("Network");

  return tid;
}

FBApplication::FBApplication ()
	:	m_nNodes (0),
		m_startingNode (0),
		m_staticProtocol (false),
		m_broadcastPhaseStart (0),
		m_cwMin (32),
		m_cwMax (1024),
		m_flooding (true),
		m_actualRange (300),
		m_estimatedRange (0),
		m_aoi (m_actualRange * 2),
		m_aoi_error (0),
		m_packetPayload (100),
		m_received (0),
		m_sent (0),
		m_cwndSum(0),
		m_cwndCount(0),
		m_errorRate(0),
		m_forgedCoordRate(0),
		m_droneTest(0),
		m_collisions (0),
		m_printCoords(0),
		m_vehicleDistance(25),
		m_transmissionList(),
		m_transmissionVector() {
		NS_LOG_FUNCTION (this);

	srand(time(0));
	RngSeedManager::SetSeed(time(0));

}

FBApplication::~FBApplication () {
  NS_LOG_FUNCTION (this);
}

void FBApplication::Install(uint32_t protocol, uint32_t broadcastPhaseStart, uint32_t actualRange, uint32_t aoi,
		uint32_t aoi_error, bool flooding, uint32_t cwMin, uint32_t cwMax, uint32_t printCoords,
		uint32_t vehicleDistance, uint32_t errorRate, uint32_t forgedCoordRate, uint32_t droneTest) {

	if (protocol == PROTOCOL_FB) {
		m_estimatedRange = PROTOCOL_FB;
		m_staticProtocol = false;
	}
	else if (protocol == PROTOCOL_STATIC_100) {
		m_estimatedRange = PROTOCOL_STATIC_100;
		m_staticProtocol = true;
	}
	else if (protocol == PROTOCOL_STATIC_300) {
		m_estimatedRange = PROTOCOL_STATIC_300;
		m_staticProtocol = true;
	}
	else if (protocol == PROTOCOL_STATIC_500) {
		m_estimatedRange = PROTOCOL_STATIC_500;
		m_staticProtocol = true;
	}
	else
		NS_LOG_ERROR ("Protocol not found.");

	m_broadcastPhaseStart = broadcastPhaseStart;
	m_aoi = aoi;
	m_aoi_error = aoi_error;
	m_actualRange = actualRange;
	m_flooding = flooding;
	m_cwMin = cwMin;
	m_cwMax	= cwMax;
	m_printCoords = printCoords;
	m_vehicleDistance = vehicleDistance;
	m_errorRate = errorRate;
	m_forgedCoordRate = forgedCoordRate;
	m_droneTest = droneTest;
	m_randomVariable = CreateObject<UniformRandomVariable>();
//	cout << "connect drop" << endl;
//	Config::Connect("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/MacRxDrop", MakeCallback(&FBApplication::LogCollision, this));
	Config::Connect("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/PhyRxDrop", MakeCallback(&FBApplication::LogCollision, this));
//	Config::Connect("/NodeList/*/DeviceList/*/$ns3::WifiMac/MacRxDrop", MakeCallback(&FBApplication::LogCollision, this));
//	Config::Connect("/NodeList/*/DeviceList/*/$ns3::WifiPhy/PhyRxDrop", MakeCallback(&FBApplication::LogCollision, this));
}

void FBApplication::LogCollision(std::string context, Ptr<const Packet> p) {
		m_collisions++;
}

void FBApplication::AddNode(Ptr<Node> node, Ptr<Socket> source, Ptr<Socket> sink, bool onstats,
		bool isNodeInJunction, uint64_t junctionId) {
	NS_LOG_FUNCTION(this << node);

	Ptr<FBNode> fbNode = CreateObject<FBNode>();
	fbNode->SetNode(node);
	fbNode->SetId(node->GetId());
	fbNode->SetSocket(source);
	sink->SetRecvCallback(MakeCallback (&FBApplication::ReceivePacket, this));
	fbNode->SetCMFR(m_estimatedRange);
	fbNode->SetLMFR(m_estimatedRange);
	fbNode->SetCMBR(m_estimatedRange);
	fbNode->SetLMBR(m_estimatedRange);
	fbNode->UpdatePosition();
	fbNode->SetHop(0);
	fbNode->SetPhase(-1);
	fbNode->SetSlot(0);
	fbNode->SetReceived(false);
	fbNode->SetSent(false);
	fbNode->SetMeAsVehicle(onstats);
//	cout << "AddNode id= " << node->GetId() << " onstats= " << onstats << endl;

	fbNode->SetMeInJunction(isNodeInJunction);
	fbNode->SetJunctionId(junctionId);
//	if (fbNode->AmIInIntersection()) {
//		cout << "node " << fbNode->GetId() << " is inside intersection " << fbNode->GetIntersectionId() << endl;
//	}

	// misc stuff
	m_nodes.push_back (fbNode);
	m_id2id[fbNode->GetId()] = m_nodes.size() - 1;
	m_nNodes++;
}


void FBApplication::PrintStats(std::stringstream &dataStream) {

	NS_LOG_FUNCTION (this);
//	cout << "cwndAvg " << (m_cwndSum / m_cwndCount) << endl;
//	cout << "collisions= " << m_collisions << endl;
	uint32_t cover = 1;	// 'cause we count m_startingNode
	uint32_t coverVehicles = 1;
	uint32_t circ = 0, circCont = 0;
//	cout << "PrintStats area " << m_aoi << endl;
	double radiusMin = m_aoi - m_aoi_error;
	double radiusMax = m_aoi + m_aoi_error;

	long double time_sum = 0;
	long double hops_sum = 0;
	long double slots_sum = 0;

	stringstream receivedOnCircIds;

	for (uint32_t i = 0; i < m_nNodes; i++)	{
		Ptr<FBNode> current = m_nodes.at (i);
		uint32_t nodeId = current->GetId ();

		// Skip the starting node
		if (nodeId == m_startingNode)
			continue;



		// Update the total cover value
		if (current->GetReceived()) {
//			cout << "cover++" << endl;
			cover++;
			if (current->AmIaVehicle()) {
				coverVehicles++;
			}
		}

		// Compute cover on circumference of radius m_aoi
		Ptr<FBNode> startingNode = this->GetFBNode(m_startingNode);

		Vector currentPosition = current->GetPosition();
		Vector startingNodePosition = startingNode->GetPosition();

		double distance = ns3::CalculateDistance (currentPosition, startingNodePosition);

		// Check if the current vehicle is in the circumference and within the range
		if ((distance >= radiusMin) && (distance <= radiusMax))	{
			// Update the number of vehicles in the circumference
			circCont++;

			// Update the cover value
			if (current->GetReceived()) {
				circ++;
				receivedOnCircIds << current->GetId() << "_";
				// Update mean time, nums and slots
//				cout << "current get hop= "  << current->GetHop() << endl;
				hops_sum += current->GetHop();
				slots_sum += current->GetSlot();
				time_sum += current->GetTimestamp().GetMicroSeconds ();
			}
		}
	}
//	Time when the first alert message was sent
	Time timeref = this->GetFBNode(m_startingNode)->GetTimestamp();
	string receivedNodes = StringifyVector(m_receivedNodes);
	stringstream nodeIds;

	for (auto i = m_nodes.begin(); i != m_nodes.end(); ++i) {
		uint32_t id = (*i)->GetId();
		nodeIds << id << "_";
//		Vector pos = (*i)->GetPosition();
//		if ( pos.x > 1050.0 && pos.x < 1250.0 && pos.y > 1800.0 && pos.y < 1900.0) {
//			cout << id << endl;
//		}
	}

	dataStream << circCont << ","
			<< cover << ","
			<< circ << ","
			<< (time_sum / (double) circ) - timeref.GetMicroSeconds () << ","
			<< (hops_sum / (double) circ) << ","
			<< (slots_sum / (double) circ) << ","
//			<< m_nodes[m_nodes.size() - 1]->GetHop() << ","
//			<< m_nodes[m_nodes.size() - 1]->GetSlot() << ","
//			<< (hops_sum / (double) circ) << ","
//			<< (slots_sum / (double) circ) << ","
			<< m_sent << ","
			<< m_received;
	NS_LOG_DEBUG("totalCoverage= " << cover << "/" << m_nNodes);
	cout << "totalCoverage= " << cover << "/" << m_nNodes << endl;
	cout << "coverageOnCirc= " << circ << "/" << circCont << endl;
	cout << "m_sent=" << m_sent << endl;
	cout << "hops= " << (hops_sum / (double) circ) << endl;
	cout << "slots= " << (slots_sum / (double) circ) << endl;

//	cout << "hopssum= " << hops_sum << " circ= "  << circ << " hops= " << (hops_sum / (double) circ) << endl;

	if (m_printCoords) {
		 Ptr<FBNode> startingNode = GetFBNode(m_startingNode);
		 string transmissionVector = StringifyVector(m_transmissionVector);
//		 cout << "FBApplication::PrintStats coords" << endl;
//		 cout << "receivedNodes" << endl;
//		 cout << receivedNodes << endl;
//		 cout << "nodeIds" << endl;
//		 cout << nodeIds.str() << endl;
//		 cout << "transmissionMap" << endl;
//		 cout << StringifyTransmissionMap() << endl;
//		 cout << "receivedOnCircIds" << endl;
//		 cout << receivedOnCircIds.str() << endl;
//		 cout << "transmissionVector" << endl;
//		 cout << transmissionVector << endl;

		 dataStream << "," << startingNode->GetPosition().x << "," << startingNode->GetPosition().y << "," << m_startingNode << "," <<
				m_vehicleDistance << "," << receivedNodes << "," << nodeIds.str() << "," << StringifyTransmissionMap() <<
				"," << receivedOnCircIds.str() << "," << transmissionVector;
	}
	if (m_droneTest) {
		uint32_t maxDistance = 0;
		uint32_t maxDistanceNodeReached = IsMaxDistNodeReached(maxDistance);
		dataStream << "," << maxDistance << "," << maxDistanceNodeReached << "," << coverVehicles;
	}


//	NS_LOG_UNCOND("aoi = " << m_aoi << "aoi error " << m_aoi_error);
}

void FBApplication::StartApplication(void) {
	NS_LOG_FUNCTION(this);

	// Get startingNode as node and as fbNode
	m_startingNode = this->GetNode()->GetId();

	if (m_id2id.count(m_startingNode) == 0) {
		NS_LOG_ERROR ("Starting node is not a fb node!");
	}

	if (!m_staticProtocol) {
		// Start Estimation Phase
		NS_LOG_INFO ("Start Estimation Phase.");
		if (m_forgedCoordRate > 0) {
			NS_LOG_INFO ("Start Forged Messages Generation Phase.");
			GenerateForgedHelloTraffic();
		}
//		GenerateHelloTraffic(1);
		GenerateHelloTraffic(5);
	}
	// Schedule Broadcast Phase
	Simulator::Schedule(Seconds(m_broadcastPhaseStart), &FBApplication::StartBroadcastPhase, this);
}

void FBApplication::StopApplication(void) {
	NS_LOG_FUNCTION(this);
}

void FBApplication::GenerateForgedHelloTraffic() {
	cout << m_nNodes << endl;
	uint32_t nAffectedNodes = m_nNodes * ((double)m_forgedCoordRate / 100);
	NS_LOG_INFO("GeneratedForgedHelloTraffic affected " << nAffectedNodes << " nodes");
	uint32_t forgedSenderNodeId = m_nNodes;
	set<uint32_t> affectedNodes;
	while (affectedNodes.size() < nAffectedNodes) {
		uint32_t nodeId = m_randomVariable->GetInteger(0, m_nNodes - 1);
		affectedNodes.insert(nodeId);
	}

	for (auto id: affectedNodes) {
		double startingX = m_nodes[id]->UpdatePosition().x + m_actualRange; //low sev
		double startingY = m_nodes[id]->UpdatePosition().y;
//		double startingX = 10000; //high sev
		for (uint32_t i = 1; i < 151; i++) {
			uint32_t headerType = HELLO_MESSAGE;
			Vector position = Vector(startingX + i, startingY, 0);

			FBHeader fbHeader;
			fbHeader.SetType (HELLO_MESSAGE);
			fbHeader.SetMaxRange (m_actualRange + i);
			fbHeader.SetStarterPosition (position);
			fbHeader.SetPosition (position);
			fbHeader.SetSenderId(forgedSenderNodeId + i); // added
			fbHeader.SetSenderInJunction(false);
			fbHeader.SetJunctionId(0);
			HandleHelloMessage(m_nodes.at(id), fbHeader);
		}
	}
}

void FBApplication::GenerateHelloTraffic(uint32_t count) {

//	NS_LOG_INFO (this << count);
	NS_LOG_INFO("GenerateHelloTraffic" << count);
	NS_LOG_DEBUG("GenerateHelloTraffic " << count);
	std::vector<int> he;
	uint32_t hel = (int) m_nNodes / 100 * 50;		// 50% of total nodes
//	uint32_t hel = (int) m_nNodes;		// 100% of total nodes
	uint32_t time_factor = 10;
//	cout << "hel= " << hel << endl;
	if (count > 0)
	{
		for (uint32_t i = 0; i < hel; i++)
		{
			int pos = rand() % m_nNodes;
			he.push_back (pos);
			Ptr<FBNode> fbNode = m_nodes.at(pos);
			Simulator::ScheduleWithContext (fbNode->GetNode()->GetId(),
																			MicroSeconds(i * time_factor),
																			&FBApplication::GenerateHelloMessage, this, fbNode);
//			Ptr<FBNode> fbNode = m_nodes.at(i);
//			Simulator::ScheduleWithContext (fbNode->GetNode()->GetId(),
//																			MicroSeconds(i * time_factor),
//																			&FBApplication::GenerateHelloMessage, this, fbNode);
		}

		// Other nodes must send Hello messages
		double s = ceil((hel * time_factor) / 1000000.0);
		auto start = std::chrono::system_clock::now();
		std::time_t start_time = std::chrono::system_clock::to_time_t(start);
		Simulator::Schedule (Seconds (s), &FBApplication::GenerateHelloTraffic, this, count - 1);
	}
}

void FBApplication::StartBroadcastPhase(void) {
	NS_LOG_FUNCTION(this);
	NS_LOG_INFO("Start Broadcast Phase.");

	Ptr<FBNode> fbNode = this->GetFBNode(m_startingNode);

	// Generate the first alert message
	GenerateAlertMessage(fbNode);
	if (m_errorRate > 0) {
		Simulator::Schedule(MilliSeconds(1), &FBApplication::GenerateAlertMessage, this, m_nodes.at(m_startingNode));
	}
}

void FBApplication::GenerateHelloMessage (Ptr<FBNode> fbNode) {
	NS_LOG_FUNCTION (this << fbNode);
    auto start = std::chrono::system_clock::now();
    std::time_t start_time = std::chrono::system_clock::to_time_t(start);
//	NS_LOG_DEBUG("generateHelloMessage (" << fbNode->GetId() << ")." << "at time= " << std::ctime(&start_time));
//	NS_LOG_DEBUG ("Generate Hello Message (" << fbNode->GetNode ()->GetId () << ").");

	// Create a packet with the correct parameters taken from the node
	Vector position = fbNode->UpdatePosition ();
	FBHeader fbHeader;
	fbHeader.SetType (HELLO_MESSAGE);
	fbHeader.SetMaxRange (fbNode->GetCMBR ());
	fbHeader.SetStarterPosition (position);
	fbHeader.SetPosition (position);

	fbHeader.SetSenderId(fbNode->GetId()); // added

	fbHeader.SetSenderInJunction(false);
	fbHeader.SetJunctionId(0);


	Ptr<Packet> packet = Create<Packet> (m_packetPayload);
	packet->AddHeader(fbHeader);

	fbNode->Send(packet);
}

void FBApplication::GenerateAlertMessage(Ptr<FBNode> fbNode) {
	NS_LOG_FUNCTION(this << fbNode);
	NS_LOG_DEBUG("Generate Alert Message (" << fbNode->GetNode()->GetId() << ").");

	// Create a packet with the correct parameters taken from the node
	uint32_t LMBR, CMBR, maxi;
	LMBR = fbNode->GetLMBR();
	CMBR = fbNode->GetCMBR();
	maxi = std::max(LMBR, CMBR);

	Vector position = fbNode->UpdatePosition();

	FBHeader fbHeader;
	fbHeader.SetType(ALERT_MESSAGE);
	fbHeader.SetMaxRange(maxi);
	fbHeader.SetStarterPosition(position);
	fbHeader.SetPosition(position);
	fbHeader.SetPhase(0);
	fbHeader.SetSlot(0);

	fbHeader.SetSenderId(fbNode->GetId());
	fbHeader.SetSenderInJunction(fbNode->AmIInJunction());
	fbHeader.SetJunctionId(fbNode->GetJunctionId());

	Ptr<Packet> packet = Create<Packet>(m_packetPayload);
	packet->AddHeader(fbHeader);

	fbNode->Send(packet);
	fbNode->SetSent(true);
	m_sent++;

	// Store current time
	fbNode->SetTimestamp(Simulator::Now());
}

void FBApplication::ReceivePacket(Ptr<Socket> socket) {
	NS_LOG_FUNCTION(this << socket);

	// Get the node who received this message and the corresponding FBNode
	Ptr<Node> node = socket->GetNode();
	Ptr<FBNode> fbNode = GetFBNode(node);

	Ptr<Packet> packet;
	Address senderAddress;

  while ((packet = socket->RecvFrom(senderAddress)))
  {
		FBHeader fbHeader;
		packet->RemoveHeader(fbHeader);
		Vector currentPosition = fbNode->UpdatePosition();
//		if (fbHeader.GetType() == ALERT_MESSAGE && currentPosition.z > 0) {
//			cout << "ricevuto alert da drone in pos " << currentPosition << endl;
//		}
		// Get the position of the sender node
		Vector senderPosition = fbHeader.GetPosition();
		double distance = ns3::CalculateDistance(currentPosition, senderPosition);
		if (m_forgedCoordRate == 0 && distance > m_actualRange) {
			continue;
		}
//		cout << "received packet at distance= " << ns3::CalculateDistance(currentPosition, senderPosition) << endl;
//		NS_LOG_DEBUG ("Packet received by node " << node->GetId() << " from node " << fbHeader.GetSenderId() << ".");
		// Get the type of the message (Hello or Alert)
		uint32_t messageType = fbHeader.GetType();
//
		if (messageType == HELLO_MESSAGE) {
			HandleHelloMessage(fbNode, fbHeader);
		}
		else if (messageType == ALERT_MESSAGE) {
			HandleAlertMessage(fbNode, fbHeader);
//			m_received++;
//			// Get the phase
//			int32_t phase = fbHeader.GetPhase();
//
//			// Get the position of the node who start the broadcast
//			Vector starterPosition = fbHeader.GetStarterPosition();
//
//			// Compute the two distances
//			double distanceSenderToStarter = ns3::CalculateDistance(senderPosition, starterPosition);
//			double distanceCurrentToStarter = ns3::CalculateDistance(currentPosition, starterPosition);

			// If starter-to-sender distance is less than starter-to-current distance,
			// then the message is coming from the front and it needs to be menaged,
			// otherwise do nothing
//			if (distanceCurrentToStarter > distanceSenderToStarter && !fbNode->GetReceived()) {
//
//				// Store when the current has received the first packet
//				fbNode->SetTimestamp(Simulator::Now());
//
//				uint32_t sl = fbHeader.GetSlot();
//				fbNode->SetSlot(fbNode->GetSlot() + sl);
//				fbNode->SetReceived(true);
//
//				uint32_t senderId = fbHeader.GetSenderId();
//				uint32_t receiverId = fbNode->GetId();
//
//				m_receivedNodes.push_back(fbNode->GetId());
//				auto it = m_transmissionList.find(senderId);
//				if (it == m_transmissionList.end()) {
////					cout << "senderId = " << senderId << endl;
//					m_transmissionList[senderId] = vector<uint32_t>();
//				}
//				m_transmissionList[senderId].push_back(receiverId);
//				m_transmissionVector.push_back(Edge(senderId, receiverId, phase));
//
//				if (fbNode->GetNum() == 0) {
//					fbNode->SetNum(phase);
//				}
//
//				// check if the message is coming fron the front
//				if (phase > fbNode->GetPhase()) {
//					fbNode->SetPhase(phase);
////					m_transmissionList.AddEdge(KeyableVector(fbHeader.GetPosition()), KeyableVector(fbNode->GetPosition()));
//					HandleAlertMessage(fbNode, fbHeader, distanceSenderToCurrent_uint);
//				}
//			}
//			else {
//
//			}
		}
  }
}

void FBApplication::HandleHelloMessage (Ptr<FBNode> fbNode, FBHeader fbHeader) {
	NS_LOG_FUNCTION (this << fbNode << fbHeader);
	uint32_t nodeId = fbNode->GetNode()->GetId();

	//	NS_LOG_DEBUG ("Handle a Hello Message (" << nodeId << ").");

	// Retrieve CMFR from the packet received and CMBR from the current node
	uint32_t otherCMFR = fbHeader.GetMaxRange();
	uint32_t myCMBR = fbNode->GetCMBR();

	// Retrieve the position of the current node
	Vector currentPosition = fbNode->UpdatePosition();

	// Retrieve the position of the sender node
	Vector senderPosition = fbHeader.GetPosition();

	// Compute distance
	double distance_double = ns3::CalculateDistance(senderPosition, currentPosition);
	uint32_t distance = std::floor(distance_double);

	// Update new values
	uint32_t maxi = std::max(std::max(myCMBR, otherCMFR), distance);

	fbNode->SetCMBR(maxi);

	// Override the old values
	fbNode->SetLMBR(myCMBR);
}

void FBApplication::HandleAlertMessage(Ptr<FBNode> fbNode, FBHeader fbHeader) {
	int32_t phase = fbHeader.GetPhase();
	Vector currentPosition = fbNode->UpdatePosition();
	// Get the position of the sender node
	Vector senderPosition = fbHeader.GetPosition();
	uint32_t senderId = fbHeader.GetSenderId();
	uint32_t receiverId = fbNode->GetId();
	NS_LOG_DEBUG ("Packet received by node " << fbNode->GetId() << " from node " << fbHeader.GetSenderId() << ".");

	// Compute the distance between the sender and me (the node who received the message)
	double distanceSenderToCurrent = ns3::CalculateDistance(senderPosition, currentPosition);
	uint32_t distanceSenderToCurrent_uint = std::floor (distanceSenderToCurrent);
	if (distanceSenderToCurrent > m_actualRange + 100) {
		return;
	}
	// Get the position of the node who start the broadcast
	Vector starterPosition = fbHeader.GetStarterPosition();

	// Compute the two distances
	double distanceSenderToStarter = ns3::CalculateDistance(senderPosition, starterPosition);
	double distanceCurrentToStarter = ns3::CalculateDistance(currentPosition, starterPosition);

	// Message coming from the back

	if (fbNode->AmIInJunction()) {
		NS_LOG_LOGIC("node " << fbNode->GetId() << "is inside a junction and has received an alert message");
		//I am in a junction and I receive a message from the same junction -> I have to defer transmission
		if (fbHeader.IsSenderInJunction() && fbNode->GetJunctionId() == fbHeader.GetJunctionId()) {
			if (phase > fbNode->GetPhase()) {
				fbNode->SetPhase(phase);
//				NS_LOG_LOGIC("node " << node->GetId() << "is inside a junction: updates phase from " << node->GetPhase() << " to " << phase);
			}
		}
	}
	else {
		// I am not in a junction and I receive a message from a node farther than me -> I have to defer tranmission
//		if ((phase > fbNode->GetPhase()) && (distanceSenderToStarter > distanceCurrentToStarter)) {
		if (phase > fbNode->GetPhase()) { //todo abilitare per urbano
			fbNode->SetPhase(phase);
//			NS_LOG_LOGIC("node " << node->GetId() << "is not inside a junction: updates phase from " << node->GetPhase() << " to " << phase);
		}
	}
	if (fbNode->GetReceived()) {
		return;
	}

	if (!fbNode->GetReceived() ) {
		fbNode->SetReceived(true);
		fbNode->SetTimestamp(Simulator::Now());
		fbNode->SetSlot(fbHeader.GetSlot());
		fbNode->SetHop(phase + 1);
		fbNode->SetPhase(phase);
		m_received++;
		// save transmission for stats and metrics
		m_receivedNodes.push_back(receiverId);
		auto it = m_transmissionList.find(senderId);
		if (it == m_transmissionList.end()) {
			m_transmissionList[senderId] = vector<uint32_t>();
		}
		m_transmissionList[senderId].push_back(receiverId);
		m_transmissionVector.push_back(Edge(senderId, receiverId, phase));
	}

	// If starter-to-sender distance is less than starter-to-current distance,
	// then the message is coming from the front and it needs to be menaged,
	// otherwise do nothing
//	if (distanceCurrentToStarter <= distanceSenderToStarter) { //todo togliere
//		return;
//		NS_LOG_DEBUG("Alert message received by " << fbNode->GetId() << " from node " << fbHeader.GetSenderId() <<
//			" is being considered for forwarding since distanceCurrentToStarter > distanceSenderToStarter " <<
//			distanceCurrentToStarter << " > " << distanceSenderToStarter);
//	}
	// Compute the size of the contention window
	uint32_t bmr = fbNode->GetCMBR();
	uint32_t cwnd = ComputeContetionWindow(bmr, distanceSenderToCurrent_uint);

//		cout << "cwnd = " << cwnd << endl;
//		m_cwndSum += cwnd;
//		m_cwndCount++;
	// Compute a random waiting time (1 <= waitingTime <= cwnd)
	uint32_t waitingTime = m_randomVariable->GetInteger(1, cwnd);
	int32_t errorDelay = ComputeErrorDelay();
//		cout << "errorDelay= " << errorDelay << endl;
	if (!m_flooding) {
		if (errorDelay == 0) {
			Simulator::Schedule(MilliSeconds(waitingTime), &FBApplication::ForwardAlertMessage,
					this, fbNode, fbHeader, waitingTime, false);
		 }
		else {
			uint32_t firstTransmissionTime;
			uint32_t secondTransmissionTime;
			firstTransmissionTime = errorDelay > 0 ? waitingTime : waitingTime + errorDelay;
			secondTransmissionTime = errorDelay < 0 ? waitingTime : waitingTime + errorDelay;
			Simulator::Schedule(MilliSeconds(firstTransmissionTime), &FBApplication::ForwardAlertMessage,
								this, fbNode, fbHeader, firstTransmissionTime, false);
			Simulator::Schedule(MilliSeconds(secondTransmissionTime), &FBApplication::ForwardAlertMessage,
								this, fbNode, fbHeader, secondTransmissionTime, true);
			}
		}
		else {
			Simulator::Schedule(MilliSeconds(0),
								&FBApplication::ForwardAlertMessage, this, fbNode, fbHeader, waitingTime, false);
		}
//	}
}

void FBApplication::WaitAgain(Ptr<FBNode> fbNode, FBHeader fbHeader, uint32_t waitingTime) {
	 NS_LOG_FUNCTION (this);

//	 // Get the phase
//	 int32_t phase = fbHeader.GetPhase();
//
//	 if (phase >= fbNode->GetPhase()) {
//		 uint32_t rnd = (rand() % 20)+1;
//		 uint32_t rnd1 = (rand() % 20)+1;
//		 uint32_t rnd2 = (rand() % 20)+1;
//		 uint32_t rnd3 = (rand() % 20)+1;
//		 Simulator::Schedule (MilliSeconds (10* (waitingTime+rnd+rnd1+rnd2+rnd3) * 200 * 3), /
//		 											&FBApplication::ForwardAlertMessage, this, fbNode, fbHeader, waitingTime);
//	 }
}

void FBApplication::ForwardAlertMessage(Ptr<FBNode> fbNode, FBHeader oldFBHeader, uint32_t waitingTime, bool forceSend) {
	NS_LOG_FUNCTION (this << fbNode << oldFBHeader);
	// Get the phase
	int32_t phase = oldFBHeader.GetPhase();
	Vector oldPos = oldFBHeader.GetPosition();
	Vector position = fbNode->UpdatePosition();
	double distance = ns3::CalculateDistance(position, oldPos);
	if (fbNode->GetStopSending()) {
		NS_LOG_DEBUG("node " << fbNode->GetId() << " defers because of StopSending");
		return;
	}
	if (!(fbNode->GetSent() && forceSend)) {
		if (fbNode->GetSent()) {
			NS_LOG_DEBUG("node " << fbNode->GetId() << " defers because of GetSent");
			return;
		}
		// If I'm not the first to wake up, I must not forward the message
		if (!m_flooding && fbNode->GetPhase() > phase) {
			NS_LOG_DEBUG("node " << fbNode->GetId() << " defers because of phase");
			return;
		}
	}
	if (forceSend) {
		fbNode->SetStopSending(true);
	}
	NS_LOG_DEBUG ("Forwarding Alert Message (" << fbNode->GetNode()->GetId() << ") after " << waitingTime <<
			" at distance= " << distance <<".");
//		NS_LOG_UNCOND ("Forwarding Alert Message (" << fbNode->GetNode()->GetId() <<
//				"at pos " << fbNode->GetPosition() << ") after " << waitingTime << ".");
	// Create a packet with the correct parameters taken from the node
	uint32_t LMBR, CMBR, maxi;
	LMBR = fbNode->GetLMBR();
	CMBR = fbNode->GetCMBR();
	maxi = std::max(LMBR, CMBR);

//		Vector position = fbNode->UpdatePosition();
	Vector starterPosition = oldFBHeader.GetStarterPosition();

	FBHeader fbHeader;
	fbHeader.SetType(ALERT_MESSAGE);
	fbHeader.SetMaxRange(maxi);
	fbHeader.SetStarterPosition(starterPosition);
	fbHeader.SetPosition(position);
	fbHeader.SetPhase(phase + 1);
	fbHeader.SetSlot(fbNode->GetSlot() + waitingTime);
	fbHeader.SetSenderId(fbNode->GetId());
	fbHeader.SetSenderInJunction(fbNode->AmIInJunction());
	fbHeader.SetJunctionId(fbNode->GetJunctionId());

//		cout << "forward alert message senderId = " << fbNode->GetId() << endl;

	Ptr<Packet> packet = Create<Packet>(m_packetPayload);
	packet->AddHeader(fbHeader);

	//
	//
	//

//		cout << "invio distance = " <<  distance << " time= " << waitingTime <<endl;

	// Forward
	fbNode->Send(packet);
	fbNode->SetSent(true);

	m_sent++;
//	else {
//		cout << "deferro distance= " << distance << " waitingTime= "<< waitingTime << endl;
//	}
}

void FBApplication::StopNode (Ptr<FBNode> fbNode) {
	NS_LOG_FUNCTION (this);

	Ptr<Node> node = fbNode->GetNode ();

	Ptr<ConstantVelocityMobilityModel> mob = node->GetObject<ConstantVelocityMobilityModel>();
	mob->SetVelocity(Vector (0, 0, 0));

}

Ptr<FBNode> FBApplication::GetFBNode (Ptr<Node> node) {
	NS_LOG_FUNCTION (this);

	if (m_id2id.count(node->GetId ()) == 0) {
		// We got a problem: key not found
		NS_LOG_ERROR ("Error: key for node " << node->GetId () << " not found in fb application.");
	}

	return this->GetFBNode (node->GetId ());
}

Ptr<FBNode> FBApplication::GetFBNode (uint32_t id) {
	NS_LOG_FUNCTION (this);

	if (m_id2id.count(id) == 0) {
		// We got a problem: key not found
		NS_LOG_ERROR ("Error: key for node " << id << " not found in fb application.");
	}

	uint32_t idin = m_id2id[id];
	return m_nodes.at (idin);
}

uint32_t FBApplication::ComputeContetionWindow (uint32_t maxRange, uint32_t distance) {
	NS_LOG_FUNCTION (this << maxRange << distance);
	double cwnd = 0.0;
	double rapp = 0.0;
//	cout << "maxRange= " << maxRange << " distance= " << distance << endl;

	if (maxRange != 0)
		rapp = ( ((double)maxRange) - ((double)distance) ) / (double) maxRange;
	else
		rapp = 0;
//	cout << "rapp pre= " << rapp << endl;
	rapp = (rapp < 0) ? 0 : rapp;
//	cout << "rapp post= " << rapp << endl;

	cwnd = (rapp * (m_cwMax - m_cwMin)) + m_cwMin;
//	cout << "FBApplication computeCW= " << std::floor (cwnd)<< endl << " con maxRange= " << maxRange << " e distance= " <<
//			distance << endl;
	return std::floor (cwnd);
}

int32_t FBApplication::ComputeErrorDelay() {
	if (m_errorRate == 0) {
		return 0;
	}
	int32_t delay = 0;
	uint32_t percent = m_randomVariable->GetInteger(0, 100);
	if (percent <= m_errorRate) {
		uint32_t plusOrMinusOne = m_randomVariable->GetInteger(0, 1);
		if (plusOrMinusOne == 0) {
			delay = 1;
		} else {
			delay = -1;
		}
	}
	return delay;
}

template <typename T>
string FBApplication::StringifyVector(const vector<T>& v) {
	stringstream ss;
//	cout << "FbApplication::PrintStuff" << m_receivedCoords.size() << " " << m_received << endl;
	for (auto i = v.begin(); i != v.end(); ++i) {
		ss << *i <<"_";
	}
//	cout << "FBApplication::StringifyVector" + ss.str() << endl;
	return ss.str();
}

string FBApplication::StringifyTransmissionMap() const {
	stringstream ss;
	for (auto it = m_transmissionList.begin(); it != m_transmissionList.end(); ++it) {
		ss << it->first << ":{";
		for (auto el: it->second) {
			ss << el << ";";
		}
		ss << "}";
	}
//	cout << "FBApplication::StringifyTransmissionMap" + ss.str() << endl;
	return ss.str();
}

uint32_t FBApplication::IsMaxDistNodeReached(uint32_t& maxDist) const {
	cout << "FBApplication::IsMaxDistNodeReached" << endl;
	Ptr<FBNode> startingNode =  m_nodes.at(m_startingNode);
	Vector startingNodePos = startingNode->GetPosition();
	uint32_t nodeId = 0;
	for (auto node: m_nodes) {
		uint32_t dist = round(ns3::CalculateDistance(node->GetPosition(), startingNodePos));
		if (dist > maxDist) {
			maxDist = dist;
			nodeId = node->GetId();
		}
	}
	return m_nodes.at(nodeId)->GetReceived();
}

} // namespace ns3
