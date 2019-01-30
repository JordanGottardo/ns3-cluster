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

/* -----------------------------------------------------------------------------
*			HEADERS
* ------------------------------------------------------------------------------
*/

#include <fstream>
#include <iostream>
#include <sys/time.h>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include "ns3/FBApplication.h"
#include "ns3/core-module.h"
#include "ns3/node-list.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/wifi-module.h"
#include "ns3/applications-module.h"
#include "ns3/topology.h"
#include "ns3/wifi-80211p-helper.h"
#include "ns3/wave-mac-helper.h"
#include "ns3/netanim-module.h"


using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("fb-vanet-3d");

/* -----------------------------------------------------------------------------
*			CLASS AND METHODS PROTOTIPES
* ------------------------------------------------------------------------------
*/

class CSVManager
{
public:
	/**
	 * \brief Constructor
	 * \return none
	 */
	CSVManager ();

	/**
	 * \brief Destructor
	 * \return none
	 */
	~CSVManager ();

	/**
	 * \brief Set the filename of the csv file
	 * \param filename path of the file
	 * \return none
	 */
	void Setup (std::string filename);

	/**
	 * \brief Write the header of the csv
	 * \param header header of the csv
	 * \return none
	 */
	void WriteHeader(std::string header);

	/**
	 * \brief Create a new filename adding a timestamp to a provided base
	 * \param base first part of the new filename
	 * \return none
	 */
	void EnableAlternativeFilename(std::string base);

	/**
	 * \brief Add a value (cell) in the current row
	 * \param value int value to be written
	 * \return none
	 */
	void AddValue(int value);

	/**
	 * \brief Add a value (cell) in the current row
	 * \param value double value to be written
	 * \return none
	 */
	void AddValue(double value);

	/**
	 * \brief Add a value (cell) in the current row
	 * \param value string value to be written
	 * \return none
	 */
	void AddValue(std::string value);

	/**
	 * \brief Add a value (cell) in the current row
	 * \param value stream value to be written
	 * \return none
	 */
	void AddValue(std::stringstream value);

	/**
	 * \brief Add multiple values in the current row
	 * \param value stream to be written
	 * \return none
	 */
	void AddMultipleValues(std::stringstream& value);

	/**
	 * \brief Write the current row and initilize a new one
	 * \return none
	 */
	void CloseRow(void);



private:
	std::string					m_csvFilename;
	std::stringstream		m_currentRow;
};

CSVManager::CSVManager ()
:	m_csvFilename ("")
{
	NS_LOG_FUNCTION (this);
}

CSVManager::~CSVManager ()
{
	NS_LOG_FUNCTION (this);
}

void
CSVManager::Setup (std::string filename)
{
	NS_LOG_FUNCTION (this);

	m_csvFilename = filename;
}

void
CSVManager::WriteHeader (std::string header)
{
	NS_LOG_FUNCTION (this);

	std::ofstream out (m_csvFilename.c_str ());
	out << header.c_str() << std::endl;
	out.close ();
}

void
CSVManager::EnableAlternativeFilename(std::string base)
{
	NS_LOG_FUNCTION (this);

	std::string new_filename;
	std::string separators = "/_,.";
	std::string extension = ".csv";

	// Get unix time
	struct timeval tp;
	gettimeofday(&tp, NULL);
	long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;

	// Create the new filename
	new_filename.append(base);
	new_filename.append(separators, 1, 1);	// only '_'
	new_filename.append(std::to_string(ms));
	new_filename.append(extension);

	m_csvFilename = new_filename;
}


void
CSVManager::AddValue(std::stringstream value)
{
	NS_LOG_FUNCTION (this);

	m_currentRow << value.str() << ",";
}

void
CSVManager::AddValue(int value)
{
	NS_LOG_FUNCTION (this);

	m_currentRow << value << ",";
}

void
CSVManager::AddValue(double value)
{
	NS_LOG_FUNCTION (this);

	m_currentRow << value << ",";
}

void
CSVManager::AddValue(std::string value)
{
	NS_LOG_FUNCTION (this);

	m_currentRow << value << ",";
}

void
CSVManager::AddMultipleValues(std::stringstream& value)
{
	NS_LOG_FUNCTION (this);

	m_currentRow << value.str();
	std::cout << "multVal " << value.str() << std::endl;

}

void
CSVManager::CloseRow (void)
{
	NS_LOG_FUNCTION (this);

	// write current row
	std::ofstream out (m_csvFilename.c_str (), std::ios::app);
	out << m_currentRow.rdbuf() << std::endl;
	out.close ();

	// Delete (old) row
	m_currentRow.str("");
}

CSVManager			g_csvData; // CSV file manager

/**
 * \ingroup obstacle
 * \brief The VanetRoutingExperiment class implements an application that
 * allows this VANET experiment to be simulated
 */
class FBVanetExperiment
{
public:
	/**
	 * \brief Constructor
	 * \return none
	 */
	FBVanetExperiment ();

	/**
	 * \brief Destructor
	 * \return none
	 */
	virtual ~FBVanetExperiment ();

	/**
	 * \brief Configure simulation of an ns-3  application
	 * \param argc program arguments count
	 * \param argv program arguments
	 * \return none
	 */
	void Configure (int argc, char **argv);

	/**
	 * \brief Enacts simulation of an ns-3  application
	 * \return none
	 */
	void Simulate ();

	/**
	 * \brief Process outputs
	 * \return none
	 */
	void ProcessOutputs ();

	/**
	* \brief Print fields on std out
	* \return none
	*/
	void PrintFields();

protected:
	/**
	 * \brief Process command line arguments
	 * \param argc program arguments count
	 * \param argv program arguments
	 * \return none
	 */
	void ParseCommandLineArguments (int argc, char **argv);

	/**
	 * \brief Configure default attributes
	 * \return none
	 */
	void ConfigureDefaults ();

	/**
	 * \brief Configure nodes
	 * \return none
	 */
	void ConfigureNodes ();

	/**
	 * \brief Configure devices
	 * \return none
	 */
	void ConfigureDevices ();

	/**
	 * \brief Configure mobility
	 * \return none
	 */
	void ConfigureMobility ();

	/**
	 * \brief Set up the adhoc devices
	 * \return none
	 */
	void SetupAdhocDevices ();

	/**
	 * \brief Configure connections
	 * \return none
	 */
	void ConfigureConnections ();

	/**
	 * \brief Configure tracing and logging
	 * \return none
	 */
	void ConfigureTracingAndLogging ();

	/**
	 * \brief Configure the FB application
	 * \return none
	 */
	void ConfigureApplications ();

	/**
	 * \brief Run the simulation
	 * \return none
	 */
	void RunSimulation ();

private:
	/**
	 * \brief Run the simulation
	 * \return none
	 */
	void Run ();

	/**
	 * \brief Run the simulation
	 * \return none
	 */
	void CommandSetup (int argc, char **argv);

	/**
	 * \brief Set up a prescribed scenario
	 * \return none
	 */
	void SetupScenario ();

	/**
	 * \brief Set up receivers socket
	 * \param node node to configure
	 * \return socket created
	 */
	Ptr<Socket> SetupPacketReceive (Ptr<Node> node);

	/**
	 * \brief Set up senders socket
	 * \param addr address of the node
	 * \param node node to configure
	 * \return socket created
	 */
	Ptr<Socket> SetupPacketSend (Ipv4Address addr, Ptr<Node> node);

	/**
   * \brief Prints actual position and velocity when a course change event occurs
   * \return none
   */
	static void
	CourseChange (std::ostream *os, std::string foo, Ptr<const MobilityModel> mobility);

	/**
	 * \brief Implements a dummy forwarding mechanism
	 * \return none
	 */
	void DummyForwarding (Ptr<Socket> socket);

	Ptr<FBApplication>								m_fbApplication;
	uint32_t 													m_nNodes; //vehicles + sensors
	uint32_t 													m_nVehicles;
	uint32_t 													m_nVehiclesEnabled;
	uint32_t 													m_nSensors;
	NodeContainer											m_adhocNodes;
	NetDeviceContainer								m_adhocDevices;
	Ipv4InterfaceContainer						m_adhocInterfaces;
	std::vector <Ptr<Socket>>					m_adhocSources;
	std::vector <Ptr<Socket>>					m_adhocSinks;
	std::string												m_packetSize;
	std::string												m_rate;
	std::string												m_phyMode;
	double														m_txp;
	uint32_t													m_port;
	uint32_t													m_actualRange;
	uint32_t													m_startingNode;
	uint32_t													m_staticProtocol;
	uint32_t													m_flooding;
	uint32_t													m_alertGeneration;
	uint32_t													m_areaOfInterest;
	uint32_t													m_vehiclesDistance;
	uint32_t													m_scenario;
	uint32_t													m_loadBuildings;
	std::string												m_traceFile;
	std::string												m_bldgFile;
	uint32_t													m_nDisabled;
	uint32_t													m_enableSensors;
	double														m_TotalSimTime;
	std::map<uint32_t, uint32_t> 			m_sensorsMemory;
	uint32_t													m_sensorsMsgSent;
	uint32_t													m_sensorsMsgRecv;
};

/* -----------------------------------------------------------------------------
*			METHOD DEFINITIONS
* ------------------------------------------------------------------------------
*/

FBVanetExperiment::FBVanetExperiment ()
	:	m_nNodes (0),
		m_nVehicles (0),
		m_nVehiclesEnabled (0),
		m_nSensors (0),
		m_packetSize ("64"),
		m_rate ("2048bps"),
		m_phyMode ("DsssRate11Mbps"),
		m_txp (20),
		m_port (9),
		m_actualRange (300),
		m_startingNode (0),
		m_staticProtocol (1),
		m_flooding (0),
		m_alertGeneration (20),
		m_areaOfInterest (1000),
		m_vehiclesDistance (250),
		m_scenario (1),
		m_loadBuildings (0),
		m_traceFile (""),
		m_bldgFile (""),
		m_nDisabled (0),
		m_enableSensors (0),
		m_TotalSimTime (30),
		m_sensorsMsgSent (0),
		m_sensorsMsgRecv (0)
{
	srand (time (0));

	RngSeedManager::SetSeed (time (0));
}

FBVanetExperiment::~FBVanetExperiment ()
{
}

void
FBVanetExperiment::Configure (int argc, char **argv)
{
	// Initial configuration and parameters parsing
	ParseCommandLineArguments (argc, argv);
	ConfigureDefaults ();

	ConfigureTracingAndLogging ();
}

void
FBVanetExperiment::Simulate ()
{
	// Configure the network and all the elements in it
	ConfigureNodes ();
	ConfigureMobility ();
	SetupAdhocDevices ();
	ConfigureConnections ();

	ConfigureApplications ();

	// Run simulation and print some results
	RunSimulation ();
}

void
FBVanetExperiment::ConfigureDefaults ()
{
	NS_LOG_FUNCTION (this);

	Config::SetDefault ("ns3::OnOffApplication::PacketSize",StringValue (m_packetSize));
	Config::SetDefault ("ns3::OnOffApplication::DataRate",  StringValue (m_rate));
	Config::SetDefault ("ns3::WifiRemoteStationManager::NonUnicastMode", StringValue (m_phyMode));

	if (m_staticProtocol == 1)
		m_staticProtocol = PROTOCOL_FB;
	else if (m_staticProtocol == 2)
		m_staticProtocol = PROTOCOL_STATIC_300;
	else if (m_staticProtocol == 3)
		m_staticProtocol = PROTOCOL_STATIC_500;
}

void
FBVanetExperiment::ParseCommandLineArguments (int argc, char **argv)
{
	NS_LOG_FUNCTION (this);

	CommandSetup (argc, argv);
	SetupScenario ();
}

void
FBVanetExperiment::ConfigureNodes ()
{
	NS_LOG_FUNCTION (this);
	NS_LOG_INFO ("Setup nodes.");

	m_adhocNodes.Create (m_nNodes);
}

void
FBVanetExperiment::ConfigureMobility ()
{
	NS_LOG_FUNCTION (this);
	NS_LOG_INFO ("Configure current mobility mode.");

	// Nodes positions
	// Create Ns2MobilityHelper with the specified trace log file as parameter
	Ns2MobilityHelper ns2 = Ns2MobilityHelper (m_traceFile);
	NS_LOG_INFO ("Loading vehicle (ns2) mobility file \"" << m_traceFile << "\".");

	// Disable node movements
	ns2.DisableNodeMovements ();

	// configure movements for all nodes
	ns2.Install (ns3::NodeList::Begin(), ns3::NodeList::End());

	// Configure callback for logging
	std::ofstream m_os;
	Config::Connect ("/NodeList/*/$ns3::MobilityModel/CourseChange",
									 MakeBoundCallback (&FBVanetExperiment::CourseChange, &m_os));
}

void
FBVanetExperiment::SetupAdhocDevices ()
{
	NS_LOG_FUNCTION (this);
	NS_LOG_INFO ("Configure channels.");

	double freq = 2.4e9;	// 802.11b 2.4 GHz

	WifiHelper wifi;
	wifi.SetStandard (WIFI_PHY_STANDARD_80211b);

	YansWifiPhyHelper wifiPhy =  YansWifiPhyHelper::Default ();
	YansWifiChannelHelper wifiChannel;
	wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
	wifiChannel.AddPropagationLoss ("ns3::TwoRayGroundPropagationLossModel", "Frequency", DoubleValue (freq), "HeightAboveZ", DoubleValue (1.5));
	if (m_loadBuildings != 0)
	{
		wifiChannel.AddPropagationLoss ("ns3::ObstacleShadowingPropagationLossModel", "Radius", DoubleValue (200));
	}
	wifiPhy.SetChannel (wifiChannel.Create ());
	wifiPhy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11);

	// Set Tx Power
	if (m_actualRange == 300)
		m_txp = 4.6;
	else if (m_actualRange == 500)
		m_txp = 13.4;

	WifiMacHelper wifiMac;
	wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
																"DataMode",StringValue (m_phyMode),
																"ControlMode",StringValue (m_phyMode));
	wifiPhy.Set ("TxPowerStart",DoubleValue (m_txp));
	wifiPhy.Set ("TxPowerEnd", DoubleValue (m_txp));
	// wifiPhy.Set ("EnergyDetectionThreshold", DoubleValue ());
	wifiMac.SetType ("ns3::AdhocWifiMac");

	m_adhocDevices = wifi.Install (wifiPhy, wifiMac, m_adhocNodes);
}

void
FBVanetExperiment::ConfigureConnections ()
{
	NS_LOG_FUNCTION (this);
	NS_LOG_INFO ("Configure connections.");

	InternetStackHelper internet;
	internet.Install (m_adhocNodes);

	Ipv4AddressHelper ipv4;
	ipv4.SetBase ("10.1.0.0", "255.255.0.0");
	m_adhocInterfaces = ipv4.Assign (m_adhocDevices);

	// TODO
	OnOffHelper onoff1 ("ns3::UdpSocketFactory", Address ());
	onoff1.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1.0]"));
	onoff1.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0.0]"));

	// Set receiver (for each node in the application)
	for (uint32_t i = 0; i < m_nNodes; i++)
	{
		SetupPacketReceive (m_adhocNodes.Get (i));
		AddressValue remoteAddress (InetSocketAddress (ns3::Ipv4Address::GetAny (), m_port));
		onoff1.SetAttribute ("Remote", remoteAddress);
	}

	// Set unicast sender (for each node in the application)
	for (uint32_t i = 0; i < m_nNodes; i++)
	{
		SetupPacketSend (ns3::Ipv4Address("10.1.255.255"),  m_adhocNodes.Get (i));
	}
}

void
FBVanetExperiment::ConfigureTracingAndLogging ()
{
	NS_LOG_FUNCTION (this);

	Packet::EnablePrinting ();
}

void
FBVanetExperiment::ConfigureApplications ()
{
	NS_LOG_FUNCTION (this);
	NS_LOG_INFO ("Configure FB application.");

	// Extract randomly which vehicles must be disabled
	// First manage extremities
	std::vector<uint32_t> nogood;

	if (m_nDisabled != 0)
	{
		uint32_t portion = std::floor(m_nVehicles / 100.0 * m_nDisabled);
		uint32_t candidate = 0;
		bool found = false;

		NS_LOG_INFO (portion << " vehicles down (with V2V comm disabled).");

		for (uint32_t i = 0; i < portion; i++)
		{
			found = true;
			while (found) {
				candidate = (rand() % m_nVehicles) + 1;

				// m_startingNode can't be in here
				if (candidate == m_startingNode)
					continue;
				// if candidate is not in nogood, it's a good candidate
				if (std::find(nogood.begin(), nogood.end(), candidate) == nogood.end()) {
					found = false;
				}
			}

			nogood.push_back (candidate);
		}
	}

	// Delete pre-existing application
	if (m_fbApplication)
		m_fbApplication = 0;

	// Create the application and schedule start and end time
	m_fbApplication = CreateObject<FBApplication> ();
	m_fbApplication->Install (m_staticProtocol,
														m_alertGeneration,
														m_actualRange,
														m_areaOfInterest,
														m_vehiclesDistance,
														(m_flooding==1) ? true : false,
														32, 1024);
	m_fbApplication->SetStartTime (Seconds (1));
	m_fbApplication->SetStopTime (Seconds (m_TotalSimTime));

	// Add vehicles to the application except whose who are in ids
	for (uint32_t i = 0; i < m_nVehicles; i++)
	{
		// If this vehicles is disabled, skip
		if (std::find(nogood.begin(), nogood.end(), i) != nogood.end()) {
			continue;
		}
		m_nVehiclesEnabled++;
		m_fbApplication->AddNode (m_adhocNodes.Get (i),
														m_adhocSources.at (i),
														m_adhocSinks.at (i),
														true);
	}

	// Add the application to a node
	m_adhocNodes.Get (m_startingNode)->AddApplication (m_fbApplication);

	if (m_enableSensors != 0)
	{
		// if (m_scenario == 2)
		// {
		// 	// sensor use FB app
		// 	for (uint32_t i = m_nVehicles; i < m_nNodes; i++)
		// 	{
		// 		m_fbApplication->AddNode (m_adhocNodes.Get (i),
		// 															m_adhocSources.at (i),
		// 															m_adhocSinks.at (i),
		// 															false);
		// 	}
		// }
		// else
		// {
			// sensor as dummy forwarder

			for (uint32_t id = m_nVehicles; id < m_nNodes; id++)
			{
				m_sensorsMemory[id] = 0;	// never see a packet

				m_adhocSinks.at (id)->SetRecvCallback (MakeCallback (&FBVanetExperiment::DummyForwarding, this));
			}
		// }
	}
}

void
FBVanetExperiment::CommandSetup (int argc, char **argv)
{
	NS_LOG_FUNCTION (this);
	NS_LOG_INFO ("Parsing command line arguments.");

	CommandLine cmd;

	// allow command line overrides
	cmd.AddValue ("sensors", "Enable or disable sensors", m_enableSensors);
	cmd.AddValue ("scenario", "1=Config1, 2=Config2", m_scenario);
	cmd.AddValue ("disabled", "Portion of vehicles to disable (no V2V capabilities), in percentage", m_nDisabled);
	cmd.AddValue ("actualRange", "Actual transimision range (meters)", m_actualRange);
	cmd.AddValue ("protocol", "Estimantion protocol: 1=FB, 2=C300, 3=C500", m_staticProtocol);
	cmd.AddValue ("flooding", "Enable flooding", m_flooding);
	cmd.AddValue ("alertGeneration", "Time at which the first Alert Message should be generated.", m_alertGeneration);
	cmd.AddValue ("area", "Radius of the area of interest", m_areaOfInterest);
	cmd.AddValue ("buildings", "Load building (obstacles)", m_loadBuildings);
	cmd.AddValue ("totalTime", "Simulation end time", m_TotalSimTime);

	cmd.Parse (argc, argv);

	if (m_nDisabled < 0 || m_nDisabled > 99)
		NS_LOG_ERROR("Percentage of disable vehciles must be greather than 0 and less than 100.");
}

void
FBVanetExperiment::SetupScenario ()
{
	NS_LOG_FUNCTION (this);
	NS_LOG_INFO ("Configure scenario (" << m_scenario << ").");

	m_alertGeneration = 9;	// 10 -1 (start time of the application)
	m_TotalSimTime = 990000.0;
	m_areaOfInterest = 400;	// meters, radius
	m_bldgFile = "LA-1x1.3Dpoly.xml";

	if (m_scenario == 1)
	{		// Setup #1
		m_traceFile = "LA-1x1.conf1.ns2mobility.xml";
		m_vehiclesDistance = 50;

		m_nVehicles = 295;
		m_startingNode = 190;		// shoud be 0 <= m_startingNode < m_nVehicles
	}
	else
	{		// Setup #2
		m_traceFile = "LA-1x1.conf2.ns2mobility.xml";
		m_vehiclesDistance = 100;

		m_nVehicles = 166;
		m_startingNode = 109;		// shoud be 0 <= m_startingNode < m_nVehicles
	}
	m_nSensors = 66;

	m_nNodes = m_nVehicles + m_nSensors;

	if (m_loadBuildings != 0)
	{
		NS_LOG_INFO ("Loading buildings file \"" << m_bldgFile << "\".");
		Topology::LoadBuildings (m_bldgFile);
	}
}

void
FBVanetExperiment::RunSimulation ()
{
	NS_LOG_FUNCTION (this);
	NS_LOG_INFO ("Run simulation.");

	Run ();
}

void
FBVanetExperiment::ProcessOutputs ()
{
	NS_LOG_FUNCTION (this);
	NS_LOG_INFO ("Process outputs.");

	std::stringstream dataStream;
	m_fbApplication->PrintStats (dataStream);
//	TODO controllare se è giusto qua sotto
	dataStream << ",";

	g_csvData.AddValue((int) RngSeedManager::GetRun ());
	g_csvData.AddValue((int) m_actualRange);
	g_csvData.AddValue((int) m_staticProtocol);
	g_csvData.AddValue((int) m_loadBuildings);
	g_csvData.AddValue((int) m_enableSensors);
	g_csvData.AddValue((int) m_nDisabled);
	g_csvData.AddValue((int) m_nNodes);
	g_csvData.AddValue((int) m_nVehiclesEnabled);
	g_csvData.AddMultipleValues(dataStream);
	g_csvData.AddValue((int) m_sensorsMsgSent);
	g_csvData.AddValue((int) m_sensorsMsgRecv);
	g_csvData.CloseRow ();
//	std::cout << "GetRun " << RngSeedManager::GetRun () << std::endl;
//	std::cout << "m_actualRange " << m_actualRange << std::endl;
//	std::cout << "m_staticProtocol " << m_staticProtocol << std::endl;
//	std::cout << "m_loadBuildings " << m_loadBuildings << std::endl;
//	std::cout << "m_enableSensors " << m_enableSensors << std::endl;
//	std::cout << "m_nDisabled " << m_nDisabled << std::endl;
//	std::cout << "m_nNodes " << m_nNodes << std::endl;
//	std::cout << "m_nVehiclesEnabled " << m_nVehiclesEnabled << std::endl;
//	std::cout << "m_sensorsMsgSent " << m_sensorsMsgSent << std::endl;
//	std::cout << "m_sensorsMsgRecv " << m_sensorsMsgRecv << std::endl;
//	NS_LOG_UNCOND(m_sensorsMsgSent << m_sensorsMsgRecv);
}

void
FBVanetExperiment::Run ()
{
	NS_LOG_FUNCTION (this);

	Simulator::Stop (Seconds (m_TotalSimTime));
	AnimationInterface anim ("tessssst.xml");
	Simulator::Run ();

	Simulator::Destroy ();
}

void
FBVanetExperiment::CourseChange (std::ostream *os, std::string foo, Ptr<const MobilityModel> mobility)
{
	NS_LOG_FUNCTION ( &os << foo << mobility);	// problem with the argument *os

  Vector pos = mobility->GetPosition (); // Get position
  Vector vel = mobility->GetVelocity (); // Get velocity

  int nodeId = mobility->GetObject<Node> ()->GetId ();

  NS_LOG_DEBUG ("Changing pos for node " << nodeId << " at " << Simulator::Now ().GetSeconds ()
	 							<< "; POS: (" << pos.x << ", " << pos.y << ", " << pos.z << ")"
								<< "; VEL: (" << vel.x << ", " << vel.y << ", " << vel.z << ").");
}

Ptr<Socket>
FBVanetExperiment::SetupPacketReceive (Ptr<Node> node)
{
	NS_LOG_FUNCTION (this << node);

	TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
	Ptr<Socket> sink = Socket::CreateSocket (node, tid);
	InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (), m_port);
	sink->Bind (local);

	// Store socket
	m_adhocSinks.push_back (sink);

	return sink;
}

Ptr<Socket>
FBVanetExperiment::SetupPacketSend (Ipv4Address addr, Ptr<Node> node)
{
	NS_LOG_FUNCTION (this << addr << node);

	TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
	Ptr<Socket> sender = Socket::CreateSocket (node, tid);
	InetSocketAddress remote = InetSocketAddress (addr, m_port);
	sender->SetAllowBroadcast (true);
	sender->Connect (remote);

	// Store socket
	m_adhocSources.push_back (sender);

	return sender;
}

void
FBVanetExperiment::DummyForwarding (Ptr<Socket> socket)
{
	NS_LOG_FUNCTION (this << socket);

	// Get the node who received this message and the corresponding FBNode
	Ptr<Node> node = socket->GetNode ();
	uint32_t id = node->GetId ();

	Ptr<Packet> packet;
	Address senderAddress;

	while ((packet = socket->RecvFrom (senderAddress)))
	{
		NS_LOG_DEBUG ("Packet received by dummy forwarding on node " << id << ".");
		m_sensorsMsgRecv++;

		// If I never saw this packet
		if (m_sensorsMemory[id] == 0)
		{
			m_sensorsMemory[id] = 1;	// mark as seen
			m_adhocSources.at (id)->Send(packet);
			m_sensorsMsgSent++;

			NS_LOG_DEBUG ("Packet forwarded by dummy forwarding on node " << node->GetId () << ".");
		}
	}
}

void
FBVanetExperiment::PrintFields() {
	NS_LOG_UNCOND(
			"Num nodes = " << m_nNodes << endl <<
			"Num vehicles = " << m_nVehicles << endl <<
			"Vehicles enabled = " << m_nVehiclesEnabled << endl <<
			"Num sensors = " << m_nSensors << endl <<
			"Packet size = " << m_packetSize << endl <<
			"Rate = " << m_rate << endl <<
			"Phy mode = " << m_phyMode << endl <<
			"Txp = " << m_txp << endl <<
			"Port = " << m_port << endl <<
			"Actual range = " << m_actualRange << endl <<
			"Starting node = " << m_startingNode << endl <<
			"Static protocol = " << m_staticProtocol << endl <<
			"Flooding = " << m_flooding << endl <<
			"Alert generation = " << m_alertGeneration << endl <<
			"Area of interest = " << m_areaOfInterest << endl <<
			"Vehicles distance = " << m_vehiclesDistance << endl <<
			"Scenario = " << m_scenario << endl <<
			"LoadBuildings = " << m_loadBuildings << endl <<
			"TraceFile = " << m_traceFile << endl <<
			"Buildings file = " << m_bldgFile << endl <<
			"Disabled = " << m_nDisabled << endl <<
			"Enable sensors = " << m_enableSensors << endl <<
			"TotalSimTime = " << m_TotalSimTime << endl
			);
}


/* -----------------------------------------------------------------------------
*			MAIN
* ------------------------------------------------------------------------------
*/

int main (int argc, char *argv[])
{
	NS_LOG_UNCOND ("FB Vanet Experiment.");


	uint32_t maxRun = RngSeedManager::GetRun ();

	// Manage data storage

	boost::filesystem::path path = boost::filesystem::current_path() /= "/out/scenario-sensori/scenario-sensori";
	cout << path.string() << endl;
	g_csvData.EnableAlternativeFilename (path.string());	// cluster
	//g_csvData.EnableAlternativeFilename ("out/fb-vanet-3d");
	g_csvData.WriteHeader ("\"id\",\
\"Actual Range\",\
\"Protocol\",\
\"Buildings\",\
\"Sensors\",\
\"Nodes disabled\",\
\"Total nodes\",\
\"Vehicles enabled\",\
\"Vehicles on circ\",\
\"Total vehicles coverage\",\
\"Vehicles coverage on circ\",\
\"Alert received mean time\",\
\"Mean hops\",\
\"Mean slots\",\
\"Messages sent\",\
\"Messages received\",\
\"Messages sent by sensors\",\
\"Messages received by sensors\"");

	for (uint32_t runId = 1; runId <= maxRun; runId++)
	{
        NS_LOG_UNCOND("run = " << runId);

		RngSeedManager::SetRun (runId);

		{
			FBVanetExperiment experiment;

			experiment.Configure (argc, argv);
			if (runId == 1) {
				experiment.PrintFields();
			}
			experiment.Simulate ();
			experiment.ProcessOutputs ();
		}
	}
}

