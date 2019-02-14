/*
 * ROFFApplication.cc
 *
 *  Created on: Jan 28, 2019
 *      Author: jordan
 */
#include "ROFFApplication.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("ROFFApplication");

NS_OBJECT_ENSURE_REGISTERED (ROFFApplication);

TypeId ROFFApplication::GetTypeId() {
	  static TypeId tid = TypeId ("ns3::ROFFApplication")
	    .SetParent<Object>()
	    .SetGroupName("Network");
	  return tid;
}

ROFFApplication::ROFFApplication() {
//	TODO
}

ROFFApplication::~ROFFApplication() {
//	TODO
}

void ROFFApplication::Install() {
//	TODO
}

void ROFFApplication::AddNode(Ptr<Node> node, Ptr<Socket> source,
		Ptr<Socket> sink) {
//	TODO
}

void ROFFApplication::PrintStats(std::stringstream& dataStream) {
//	TODO
}

void ROFFApplication::StartApplication(void) {
//	TODO
}

void ROFFApplication::StopApplication(void) {
//	TODO
}

void ROFFApplication::GenerateHelloTraffic(uint32_t count) {
//	TODO
//	Generate some hello traffic: how often should we send beacons?
//	Each vehicle should send beacons every x ms
//	Evaluate whether to use redundant beacon transmissions
}

void ROFFApplication::StartBroadcastPhase(void) {
//	TODO
}

void ROFFApplication::GenerateHelloMessage(Ptr<ROFFNode> fbNode) {
// TODO
// Generate hello message, attaching current position and node id
// Broadcast hello message
}

void ROFFApplication::GenerateAlertMessage(Ptr<ROFFNode> fbNode) {
// TODO
//	Generate alert message
//	Generate ESD bitmap from neighbor table (NBT)
//		ESI bitmap size is delta+1, where delta is the maximum distance between the
//		the node and the PFCs in the NBT
// Include ESD bitmap and node's current position in alert message
//	Broadcast alert message
//
}

void ROFFApplication::ReceivePacket(Ptr<Socket> socket) {
	NS_LOG_FUNCTION(this << socket);

	Ptr<Node> node = socket->GetNode();

	Address senderAddress;
	Ptr<Packet> packet;

	while ((packet = socket->RecvFrom(senderAddress))) {
		ROFFHeader header;
		packet->RemoveHeader(header);
	}
//	TODO
//	Check if hello or alert
//		if hello, handle hello message
//		if alert, handle alert message
}

void ROFFApplication::HandleHelloMessage(Ptr<ROFFNode> fbNode,
		ROFFNode fbHeader) {
//	update relative's entry to sender in receiver's NBT
}

void ROFFApplication::HandleAlertMessage(Ptr<ROFFNode> fbNode,
		ROFFNode fbHeader, uint32_t distance) {
//	TODO
//	initiate forward priority acquisition
//	after that, calculate minDiff
//		then wait minDiff
//		then broadcast the message
}

double ROFFApplication::CalculateMinDiff() {
//	TODO
//	calculate minDiff between nodes
}

void ROFFApplication::WaitAgain(Ptr<ROFFNode> fbNode, ROFFNode fbHeader,
		uint32_t waitingTime) {
}

void ROFFApplication::ForwardAlertMessage(Ptr<ROFFNode> fbNode,
		ROFFNode oldFBHeader, uint32_t waitingTime) {
// TODO
//	broadcast alert message
}

void ROFFApplication::StopNode(Ptr<ROFFNode> fbNode) {
}

Ptr<ROFFNode> ROFFApplication::GetFBNode(Ptr<Node> node) {
}

Ptr<ROFFNode> ROFFApplication::GetFBNode(uint32_t id) {
}

uint32_t ROFFApplication::ComputeContentionWindow(uint32_t maxRange,
		uint32_t distance) {
}
}


