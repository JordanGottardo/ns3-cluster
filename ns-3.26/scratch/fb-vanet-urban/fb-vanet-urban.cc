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

#include <bits/stdint-uintn.h>
#include <bits/types/struct_timeval.h>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/algorithm/string.hpp>
#include <sys/time.h>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <vector>
#include <thread>

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
#include "ns3/FBApplication.h"

using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("fb-vanet-urban");

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
	void EnableAlternativeFilename(boost::filesystem::path path);

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
	boost::filesystem::path					m_csvFilePath;
	std::stringstream						m_currentRow;
	unsigned int 							runId;
};

CSVManager::CSVManager ()
:	m_csvFilePath (""), runId(0)
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

	m_csvFilePath = filename;
}

void
CSVManager::WriteHeader (std::string header)
{
	NS_LOG_FUNCTION (this);

	boost::filesystem::path parentPath = m_csvFilePath.parent_path();
	boost::filesystem::create_directories(parentPath);
	std::ofstream out (m_csvFilePath.string());
	out << header.c_str() << std::endl;
	out.close ();
}

void
CSVManager::EnableAlternativeFilename(boost::filesystem::path path)
{
	NS_LOG_FUNCTION (this);
	std::string separators = "/_,.";
	std::string extension = ".csv";

	// Get unix time
	struct timeval tp;
	gettimeofday(&tp, NULL);
	long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;

	// Create the new filename
//	new_filename.append(base);
//	new_filename.append(separators, 1, 1);	// only '_'
//	new_filename.append(std::to_string(ms));
	std::string finalPartOfPath = "-" + to_string(ms) + extension;
	m_csvFilePath = path += finalPartOfPath;
	cout << "fbTest= " << m_csvFilePath << endl;
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
}

void
CSVManager::CloseRow (void)
{
	NS_LOG_FUNCTION (this);
//	m_mutex.lock();
	// write current row to file in an atomic way
	std::ofstream out (m_csvFilePath.c_str (), std::ios::app);
	out << runId << ","<< m_currentRow.rdbuf() << std::endl;
	runId++;
	out.close ();
//	m_mutex.unlock();
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
	void Configure (int argc, char *argv[]);

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
	 * \brief Calculates the filepath where the output of the experiment will be saved (depends on input parameters)
	 * \return file path
	 */
	const std::string CalculateOutFilePath() const;


	/**
	 * \brief Calculates configure, runs and print results of the experiment
	 * \return file path
	 */
	void RunAndPrintResults(int argc, char *argv[]);

	/**
	* \brief printToFile getter
	* \return printToFile
	*/
	uint32_t GetPrintToFile() const;

	/**
	* \brief printCoords getter
	* \return printCoords
	*/
	uint32_t GetPrintCoords() const;

	/**
	* \brief droneTest getter
	* \return droneTest
	*/
	uint32_t GetDroneTest() const;

	/**
	* \brief highBuildings getter
	* \return highBuildings
	*/
	uint32_t GetHighBuildings() const;

protected:
	/**
	 * \brief Process command line arguments
	 * \param argc program arguments count
	 * \param argv program arguments
	 * \return none
	 */
	void ParseCommandLineArguments (int argc, char *argv[]);

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
	void ConfigureFBApplication ();

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
	void CommandSetup (int argc, char *argv[]);

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
	* \brief Calculates number of nodes based on .ns2mobility filepath
	* \return Number of nodes (vehicles) in simulation
	*/
	unsigned int CalculateNumNodes() const;

	/**
	* \brief Loads .junction file in m_nodeIdToJunctionIdMap
	* \return none
	*/
	void LoadJunctionsMap();

	/**
   * \brief Prints actual position and velocity when a course change event occurs
   * \return none
   */
	static void	CourseChange (std::ostream *os, std::string foo, Ptr<const MobilityModel> mobility);


	Ptr<FBApplication>						m_fbApplication;
	uint32_t 								m_nNodes;
	NodeContainer							m_adhocNodes;
	Ptr<ListPositionAllocator> 				m_adhocPositionAllocator;
	NetDeviceContainer						m_adhocDevices;
	Ipv4InterfaceContainer					m_adhocInterfaces;
	vector <Ptr<Socket>>					m_adhocSources;
	vector <Ptr<Socket>>					m_adhocSinks;
	string									m_packetSize;
	string									m_rate;
	string									m_phyMode;
	double									m_txp;
	uint32_t								m_port;
	uint32_t								m_actualRange;
	int32_t									m_startingNode;
	uint32_t								m_staticProtocol;
	uint32_t								m_flooding;
	uint32_t								m_alertGeneration;
	uint32_t								m_areaOfInterest;
	uint32_t								m_vehicleDistance;
	uint32_t								m_scenario;
	uint32_t								m_loadBuildings;
	uint32_t								m_cwMin;
	uint32_t								m_cwMax;
	string									m_traceFile;
	string									m_bldgFile;
	string									m_junctionFile;
	string									m_mapBasePath;
	string									m_mapBaseName;
	double									m_TotalSimTime;
	uint32_t								m_printToFile;
	uint32_t								m_printCoords;
	uint32_t								m_createObstacleShadowingLossFile;
	uint32_t								m_useObstacleShadowingLossFile;
	uint32_t								m_propagationLoss;
	uint32_t								m_smartJunctionMode;
	uint32_t								m_errorRate;
	uint32_t								m_forgedCoordTest;
	uint32_t								m_forgedCoordRate;
	uint32_t								m_nVeh;
	uint32_t								m_droneTest;
	uint32_t								m_highBuildings;
	std::map<uint32_t, uint64_t>			m_nodeIdToJunctionIdMap;


};

/* -----------------------------------------------------------------------------
*			METHOD DEFINITIONS
* ------------------------------------------------------------------------------
*/

FBVanetExperiment::FBVanetExperiment ()
	:	m_nNodes(0),	// random value, it will be set later
		m_packetSize("68"), //added
		m_rate("2048bps"),
		m_phyMode("DsssRate11Mbps"),
		m_txp(20),
		m_port(9),
		m_actualRange(300),
		m_startingNode(-1),
		m_staticProtocol(1),
		m_flooding(0),
		m_alertGeneration(20),
		m_areaOfInterest(1000),
		m_vehicleDistance(25),
		m_scenario(1),
		m_loadBuildings(1),
		m_cwMin(32),
		m_cwMax(1024),
		m_traceFile(""),
		m_bldgFile(""),
		m_junctionFile(""),
		m_TotalSimTime(30),
		m_printToFile(1),
		m_printCoords(0),
		m_createObstacleShadowingLossFile(0),
		m_useObstacleShadowingLossFile(0),
		m_propagationLoss(1),
		m_smartJunctionMode(0),
		m_errorRate(0),
		m_forgedCoordTest(0),
		m_forgedCoordRate(0),
		m_nVeh(0),
		m_droneTest(0),
		m_highBuildings(0) {
	srand (time (0));

	RngSeedManager::SetSeed (time (0));
}

FBVanetExperiment::~FBVanetExperiment ()
{
}

void
FBVanetExperiment::Configure (int argc, char *argv[])
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

	ConfigureFBApplication ();

	// Run simulation and print some results
	RunSimulation ();
}

void FBVanetExperiment::ProcessOutputs () {
	NS_LOG_FUNCTION (this);

	NS_LOG_INFO ("Process outputs.");
	std::stringstream dataStream;
	m_fbApplication->PrintStats (dataStream);
	if (m_printToFile) {
		g_csvData.AddValue((int) m_scenario);
		g_csvData.AddValue((int) m_actualRange);
		g_csvData.AddValue((int) m_staticProtocol);
		g_csvData.AddValue((int) m_loadBuildings);
		g_csvData.AddValue((int) m_nNodes);
		g_csvData.AddMultipleValues(dataStream);
		g_csvData.CloseRow ();
	}
}

const std::string FBVanetExperiment::CalculateOutFilePath() const {
	std::string fileName = "";
	std::string cwMin = std::to_string(m_cwMin);
	std::string cwMax = std::to_string(m_cwMax);
	std::string vehicleDistance = std::to_string(m_vehicleDistance);
	std::string buildings = std::to_string(m_loadBuildings);
	std::string errorOrForged = "e" + std::to_string(m_errorRate);
	std::string protocol = "";
	std::string actualRange = std::to_string(m_actualRange);
	std::string junctions = std::to_string(m_smartJunctionMode);
	if (m_forgedCoordTest) {
		errorOrForged = "f" + std::to_string(m_forgedCoordRate);
	}

	if (m_staticProtocol == PROTOCOL_FB) {
//		protocol = "Fast-Broadcast[" + std::to_string(m_cwMin) + "-" + std::to_string(m_cwMax) + "]";
		protocol = "Fast-Broadcast";
	}
	else if (m_staticProtocol == PROTOCOL_STATIC_100) {
		protocol = "STATIC-100";
	}
	else if (m_staticProtocol == PROTOCOL_STATIC_300) {
		protocol = "STATIC-300";
	}
	else if (m_staticProtocol == PROTOCOL_STATIC_500) {
		protocol = "STATIC-500";
	}

	vector<string> strings;
	boost::split(strings, m_traceFile, boost::is_any_of("/"));
	std::string scenarioName = strings[strings.size() - 1];
	int dotPos =scenarioName.find(".");
	scenarioName = scenarioName.substr(0, dotPos);

	fileName.append(scenarioName + "/b" + buildings + "/" + errorOrForged +  "/r" + actualRange +  "/j" + junctions
			+ "/" + "cw[" + std::to_string(m_cwMin) + "-" + std::to_string(m_cwMax) + "]/" + protocol + "/" +
			scenarioName + "-b" + buildings + "-" +errorOrForged + "-r" + actualRange + "-j" + junctions + "-" + protocol);
	cout << "fileName=" << fileName << endl;
//	fileName.append("cw-" + cwMin + "-" + cwMax + "/" + m_mapBaseNameWithoutDistance + "/d" + vehicleDistance + "/b" + buildings
//			+ "/" + protocol + "-" + actualRange + "/" + m_mapBaseName + "-cw-" + cwMin + "-" + cwMax + "-b"
//			+ buildings + "-" + protocol + "-" + actualRange);

	return fileName;
}

void FBVanetExperiment::RunAndPrintResults(int argc, char *argv[]) {
	Configure(argc, argv);
	Simulate();
	ProcessOutputs();
}

uint32_t FBVanetExperiment::GetPrintToFile() const {
	return m_printToFile;
}

uint32_t FBVanetExperiment::GetPrintCoords() const {
	return m_printCoords;
}

uint32_t FBVanetExperiment::GetDroneTest() const {
	return m_droneTest;
}

uint32_t FBVanetExperiment::GetHighBuildings() const {
	return m_highBuildings;
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
		m_staticProtocol = PROTOCOL_STATIC_100;
	else if (m_staticProtocol == 3)
		m_staticProtocol = PROTOCOL_STATIC_300;
	else if (m_staticProtocol == 4)
		m_staticProtocol = PROTOCOL_STATIC_500;
}

void
FBVanetExperiment::ParseCommandLineArguments (int argc, char *argv[])
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

	// Create Ns2MobilityHelper with the specified trace log file as parameter
	Ns2MobilityHelper ns2 = Ns2MobilityHelper (m_traceFile);
	NS_LOG_INFO ("Loading ns2 mobility file \"" << m_traceFile << "\".");

	// Disable node movements
	ns2.DisableNodeMovements();

	ns2.Install(); // configure movements for each node, while reading trace file

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
	if (m_propagationLoss == 0) {
		wifiChannel.AddPropagationLoss("ns3::RangePropagationLossModel", "MaxRange", DoubleValue(m_actualRange));
	} else if (m_propagationLoss == 1) {
		wifiChannel.AddPropagationLoss("ns3::TwoRayGroundPropagationLossModel", "Frequency", DoubleValue(freq), "HeightAboveZ", DoubleValue(2.0));
	} else {
		NS_LOG_ERROR("m_propagationLoss not recognized");
	}
	if (m_loadBuildings != 0)
	{
		wifiChannel.AddPropagationLoss ("ns3::ObstacleShadowingPropagationLossModel",
										"Radius", DoubleValue(500),
										"CreateFile", IntegerValue(m_createObstacleShadowingLossFile),
										"UseFile", IntegerValue(m_useObstacleShadowingLossFile),
										"MapBasePath", StringValue(m_mapBasePath),
										"DroneTest", IntegerValue(m_droneTest),
										"HighBuildings", IntegerValue(m_highBuildings));
	}
	wifiPhy.SetChannel (wifiChannel.Create ());
	wifiPhy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11);

	if (m_actualRange == 100) {
		m_txp = -7.0;
	}
	else if (m_actualRange == 300) {
		m_txp = 4.6;
	}
	else if (m_actualRange == 500) {
		m_txp = 13.4;
	}

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

void FBVanetExperiment::ConfigureTracingAndLogging () {
	NS_LOG_FUNCTION (this);

	Packet::EnablePrinting ();
}

void FBVanetExperiment::ConfigureFBApplication () {
	NS_LOG_FUNCTION (this);
	NS_LOG_INFO ("Configure FB application.");

	// Delete pre-existing application
	if (m_fbApplication)
		m_fbApplication = 0;

	// Create the application and schedule start and end time
	m_fbApplication = CreateObject<FBApplication> ();
//	m_fbApplication->Install

	m_fbApplication->Install(m_staticProtocol,
							m_alertGeneration,
							m_actualRange,
							m_areaOfInterest,
							m_vehicleDistance,
							(m_flooding==1) ? true : false,
							m_cwMin, m_cwMax, m_printCoords,
							m_vehicleDistance, m_errorRate, m_forgedCoordRate, m_droneTest
							);
	m_fbApplication->SetStartTime (Seconds (1));
	m_fbApplication->SetStopTime (Seconds (m_TotalSimTime));

	// Add nodes to the application
	for (uint32_t i = 0; i < m_nNodes; i++)
	{
		uint64_t junctionId = 0;
		bool nodeInsideJunction = false;

		if (m_smartJunctionMode) {
			if (m_nodeIdToJunctionIdMap.count(i) != 0) {
				nodeInsideJunction = true;
				junctionId = m_nodeIdToJunctionIdMap.at(i);
			}
		}
		bool isNodeVehicle = true;
		if (i >= m_nVeh) {
			isNodeVehicle = false;
		}
//		cout << "addNode " << i << " " << isNodeVehicle << endl;
		m_fbApplication->AddNode (m_adhocNodes.Get (i), m_adhocSources.at (i), m_adhocSinks.at (i), isNodeVehicle,
				nodeInsideJunction, junctionId);
	}

	// Add the application to a node
	m_adhocNodes.Get (m_startingNode)->AddApplication (m_fbApplication);
}

void FBVanetExperiment::RunSimulation () {
	NS_LOG_FUNCTION (this);
	NS_LOG_INFO ("Run simulation.");

	Run ();
}

void FBVanetExperiment::CommandSetup (int argc, char *argv[]) {
	NS_LOG_FUNCTION (this);
	NS_LOG_INFO ("Parsing command line arguments.");

	CommandLine cmd;

	// allow command line overrides
//	cmd.AddValue ("nnodes", "Number of nodes (i.e. vehicles)", m_nNodes);
	cmd.AddValue ("startingNode", "Id of the first node who will start an aler", m_startingNode);
	cmd.AddValue ("actualRange", "Actual transimision range (meters)", m_actualRange);
	cmd.AddValue ("protocol", "Estimantion protocol: 1=FB, 2=C100, 3=C300, 4=C500", m_staticProtocol);
	cmd.AddValue ("flooding", "Enable flooding", m_flooding);
	cmd.AddValue ("alertGeneration", "Time at which the first Alert Message should be generated.", m_alertGeneration);
	cmd.AddValue ("area", "Radius of the area of interest", m_areaOfInterest);
	cmd.AddValue("vehicleDistance", "Distance between vehicles", m_vehicleDistance);
//	cmd.AddValue ("scenario", "1=Padova, 2=Los Angeles", m_scenario);
	cmd.AddValue ("buildings", "Load building (obstacles)", m_loadBuildings);
	cmd.AddValue ("poly", "Buildings trace file (poly format)", m_bldgFile);
	cmd.AddValue ("trace", "Vehicles trace file (ns2mobility format)", m_traceFile);
	cmd.AddValue ("junctions", "Junction file", m_junctionFile);
	cmd.AddValue ("totalTime", "Simulation end time", m_TotalSimTime);
	cmd.AddValue ("cwMin", "Minimum contention window", m_cwMin);
	cmd.AddValue ("cwMax", "Maximum contention window", m_cwMax);

	cmd.AddValue ("mapBasePath", "Base path of map required for simulation "
			"(e.g. ../maps/Padova", m_mapBasePath);
	cmd.AddValue ("printToFile", "Print data to file or not: 0 not print, 1 print ", m_printToFile);
	cmd.AddValue ("printCoords", "Print coords to file or not: 0 not print, 1 print ", m_printCoords);
	cmd.AddValue ("createObstacleShadowingLossFile", "Create file which saves obstacle losses (dBm) keyed by "
			"senderCoord, receiverCoord : 0 not create, 1 create ", m_createObstacleShadowingLossFile);
	cmd.AddValue ("useObstacleShadowingLossFile", "Use optimization based on file which saves obstacle losses *dBm) "
			"keyed by senderCoord, receiverCoord:  0 don't use it, 1 use it ", m_useObstacleShadowingLossFile);
	cmd.AddValue("propagationLoss", "Type of propagation loss model: 0=RangePropagation, 1=TwoRayGround", m_propagationLoss);
	cmd.AddValue("smartJunctionMode", "Whether to activate smart junction mode: 0=disabled, 1=enabled", m_smartJunctionMode);
	cmd.AddValue("errorRate", "Probability to incur in an error in transmission schedule (sending 1 slot earlier or later", m_errorRate);
	cmd.AddValue("forgedCoordTest", "Whether to run the forged hello messages attack test 0=disabled, 1=enabled", m_forgedCoordTest);
	cmd.AddValue("forgedCoordRate", "Percentage of affected vehicle by forged hello messages attack", m_forgedCoordRate);

	cmd.AddValue("nVehicles", "Number of vehicles (to be used in drones+vehicles scenario", m_nVeh);
	cmd.AddValue("droneTest", "Whether to read drones from ns2mobilityFile and run test with drones", m_droneTest);
	cmd.AddValue("highBuildings", "Whether buildings are very high (higher than any drones, e.g. 100m)", m_highBuildings);

	// only one of these tests is possible at a given time
	if (m_forgedCoordTest) {
		m_errorRate = 0;
	}

	cmd.Parse (argc, argv);
}

void FBVanetExperiment::SetupScenario () {
	NS_LOG_FUNCTION (this);
//	NS_LOG_INFO ("Configure current scenario (" << m_scenario << ").");

	m_alertGeneration = 9;	// 10 -1 (start time of the application)
	m_TotalSimTime = 990000.0;
//	m_areaOfInterest = 1000;	// meters

	if (m_bldgFile.empty()) {
		string extension;
		if (m_droneTest) {
			extension = ".3Dpoly.xml";
		}
		else {
			extension = ".poly.xml";
		}
		if (m_highBuildings) {
			m_bldgFile = m_mapBasePath + "-100" + extension;
		}
		else {
			m_bldgFile = m_mapBasePath + extension;
		}
	}

	if (m_traceFile.empty()) {
		if (m_droneTest) {
			m_traceFile = m_mapBasePath + ".3Dns2mobility.xml";
		} else {
			m_traceFile = m_mapBasePath + ".ns2mobility.xml";
		}
	}

	if (m_junctionFile.empty()) {
		m_junctionFile = m_mapBasePath + ".junctions";
	}

	m_nNodes = CalculateNumNodes();
	if (!m_droneTest) {
		m_nVeh = m_nNodes;
	}
	cout << "numNodes = " << m_nNodes << endl;
	if (m_startingNode == -1) {
		m_startingNode = rand() % m_nNodes;
	}
	cout << "numNodes = " << m_nNodes << endl;
	cout << "numVeh = " << m_nVeh << endl;
	cout << "startingNode = " << m_startingNode << endl;
	cout << "using tracefile " << m_traceFile << endl;

	if (m_loadBuildings != 0) {
		cout << "Loading buildings file \"" << m_bldgFile << endl;
		Topology::LoadBuildings(m_bldgFile, m_createObstacleShadowingLossFile, m_useObstacleShadowingLossFile, m_mapBasePath,
				m_droneTest, m_highBuildings);
	}

	if (m_smartJunctionMode) {
		LoadJunctionsMap();
	}
}

unsigned int FBVanetExperiment::CalculateNumNodes() const {
	NS_LOG_FUNCTION (this);
	ifstream ns2mobilityTraceFile;
	ns2mobilityTraceFile.open(m_traceFile, ios::in);
	if (!ns2mobilityTraceFile.is_open()) {
		NS_LOG_ERROR("Could not open ns2MobilityTraceFile");
	}
	string line;
	unsigned int numNodes = 0;
	while(getline(ns2mobilityTraceFile, line)) {
		vector<string> strings;
		boost::split(strings, line, boost::is_any_of(" "));
		if (strings.size() == 4) {
			string str = strings.at(0);
			unsigned int openParensPos = str.find_first_of("(");
			unsigned int closeParensPos = str.find_first_of(")");
			unsigned int numNodeCandidate = stoi(str.substr(openParensPos + 1, closeParensPos - openParensPos - 1));
			if (numNodeCandidate > numNodes) {
				numNodes = numNodeCandidate;
			}
		}
	}
	return numNodes + 1;
}

void FBVanetExperiment::Run() {
	NS_LOG_FUNCTION (this);

	Simulator::Stop (Seconds (m_TotalSimTime));
//	AnimationInterface anim ("testAnimationWithPacketMetadata.xml");
//	anim.EnablePacketMetadata (true);
	Simulator::Run ();

	Simulator::Destroy ();
}

void FBVanetExperiment::CourseChange (std::ostream *os, std::string foo, Ptr<const MobilityModel> mobility) {
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
FBVanetExperiment::LoadJunctionsMap() {
	NS_LOG_FUNCTION (this);
	ifstream junctionFile;
	cout << m_junctionFile << endl;
	junctionFile.open(m_junctionFile, ios::in);
	if (!junctionFile.is_open()) {
		NS_LOG_ERROR("Could not open junctionFile");
	}
	string line;
	while(getline(junctionFile, line)) {
		vector<string> strings;
		boost::split(strings, line, boost::is_any_of(" "));
		if (strings.size() == 2) {
			uint32_t nodeId = stoi(strings.at(0));
			uint64_t junctionId = stol(strings.at(1));
			m_nodeIdToJunctionIdMap[nodeId] = junctionId;
		}
	}
//	cout << m_nodeIdToJunctionIdMap.size() << endl;
//	for (auto pair: m_nodeIdToJunctionIdMap) {
//		cout << "nodeId= " << pair.first << " intId= " << pair.second << endl;
//	}
}

/* -----------------------------------------------------------------------------
*			MAIN
* ------------------------------------------------------------------------------
*/

int main (int argc, char *argv[])
{
    cout << "Start main urban" << endl;
	NS_LOG_UNCOND ("FB Vanet Experiment URBAN");
	unsigned int maxRun = RngSeedManager::GetRun();

//	Before launching experiments, calculate output file path
	FBVanetExperiment experiment;
	experiment.Configure (argc, argv);
	if (experiment.GetPrintToFile()) {
		string filePath = experiment.CalculateOutFilePath();
		string additionalPath;
		string header;

		if(experiment.GetPrintCoords()) {
			additionalPath = "/out/scenario-urbano-con-coord/";
			header = "\"id\",\"Scenario\",\"Actual Range\",\"Protocol\",\"Buildings\",\"Total nodes\","
					"\"Nodes on circ\",\"Total coverage\",\"Coverage on circ\",\"Alert received mean time\",\"Hops\","
					"\"Slots\",\"Messages sent\",\"Messages received\", \"Starting x\", \"Starting y\","
					"\"Starting node\", \"Vehicle distance\", \"Received node ids\", "
					"\"Node ids\", \"Transmission map\", \"Received on circ nodes\", \"Transmission vector\"";
		}
		else if (experiment.GetHighBuildings()) {
			additionalPath = "/out/scenario-droni-high/";
			header = "\"id\",\"Scenario\",\"Actual Range\",\"Protocol\",\"Buildings\",\"Total nodes\","
							 "\"Nodes on circ\",\"Total coverage\",\"Coverage on circ\",\"Alert received mean time\",\"Hops\","
							 "\"Slots\",\"Messages sent\",\"Messages received\", \"Max distance\", \"Reached maxDist node\","
							 "\"Vehicles cover\"";
		}
		else if (experiment.GetDroneTest()) {
			additionalPath = "/out/scenario-droni/";
			header = "\"id\",\"Scenario\",\"Actual Range\",\"Protocol\",\"Buildings\",\"Total nodes\","
								"\"Nodes on circ\",\"Total coverage\",\"Coverage on circ\",\"Alert received mean time\",\"Hops\","
								"\"Slots\",\"Messages sent\",\"Messages received\", \"Max distance\", \"Reached maxDist node\","
					 	 	 	"\"Vehicles cover\"";
		}

		else {
			additionalPath = "/out/scenario-urbano/";
			header = "\"id\",\"Scenario\",\"Actual Range\",\"Protocol\",\"Buildings\",\"Total nodes\","
								"\"Nodes on circ\",\"Total coverage\",\"Coverage on circ\",\"Alert received mean time\",\"Hops\","
								"\"Slots\",\"Messages sent\",\"Messages received\"";
		}
		boost::filesystem::path path = boost::filesystem::current_path() /= additionalPath;
		path /= filePath;
		g_csvData.EnableAlternativeFilename(path);
		g_csvData.WriteHeader(header);
	}
	for(unsigned int i = 0; i < maxRun; i++) {
		cout << "run = " << i << endl;
		FBVanetExperiment experiment;
		experiment.RunAndPrintResults(argc, argv);
	}

}
