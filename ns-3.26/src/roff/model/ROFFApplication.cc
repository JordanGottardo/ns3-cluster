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

ns3::ROFFApplication::ROFFApplication() {
//	TODO
}

ns3::ROFFApplication::~ROFFApplication() {
//	TODO
}

void ns3::ROFFApplication::Install() {
//	TODO
}

void ns3::ROFFApplication::AddNode(Ptr<Node> node, Ptr<Socket> source,
		Ptr<Socket> sink) {
//	TODO
}

void ns3::ROFFApplication::PrintStats(std::stringstream& dataStream) {
//	TODO
}

void ns3::ROFFApplication::StartApplication(void) {
//	TODO
}

void ns3::ROFFApplication::StopApplication(void) {
//	TODO
}

void ns3::ROFFApplication::GenerateHelloTraffic(uint32_t count) {
//	TODO
//	Generate some hello traffic: how often should we send beacons?
//	Each vehicle should send beacons every x ms
//	Evaluate whether to use redundant beacon transmissions
}

void ns3::ROFFApplication::StartBroadcastPhase(void) {
//	TODO
}

void ns3::ROFFApplication::GenerateHelloMessage(Ptr<ROFFNode> fbNode) {
// TODO
// Generate hello message, attaching current position and node id
// Broadcast hello message
}

void ns3::ROFFApplication::GenerateAlertMessage(Ptr<ROFFNode> fbNode) {
// TODO
//	Generate alert message
//	Generate ESD bitmap from neighbor table (NBT)
//		ESI bitmap size is delta+1, where delta is the maximum distance between the
//		the node and the PFCs in the NBT
// Include ESD bitmap and node's current position in alert message
//	Broadcast alert message
//
}

void ns3::ROFFApplication::ReceivePacket(Ptr<Socket> socket) {
//	TODO
//	Check if hello or alert
//		if hello, handle hello message
//		if alert, handle alert message
}

void ns3::ROFFApplication::HandleHelloMessage(Ptr<ROFFNode> fbNode,
		ROFFNode fbHeader) {
//	update relative's entry to sender in receiver's NBT
}

void ns3::ROFFApplication::HandleAlertMessage(Ptr<ROFFNode> fbNode,
		ROFFNode fbHeader, uint32_t distance) {
//	TODO
//	initiate forward priority acquisition
//	after that, calculate minDiff
//		then wait minDiff
//		then broadcast the message
}

void ns3::ROFFApplication::WaitAgain(Ptr<ROFFNode> fbNode, ROFFNode fbHeader,
		uint32_t waitingTime) {
}

void ns3::ROFFApplication::ForwardAlertMessage(Ptr<ROFFNode> fbNode,
		ROFFNode oldFBHeader, uint32_t waitingTime) {
// TODO
//	broadcast alert message
}

void ns3::ROFFApplication::StopNode(Ptr<ROFFNode> fbNode) {
}

Ptr<ROFFNode> ns3::ROFFApplication::GetFBNode(Ptr<Node> node) {
}

Ptr<ROFFNode> ns3::ROFFApplication::GetFBNode(uint32_t id) {
}

uint32_t ns3::ROFFApplication::ComputeContentionWindow(uint32_t maxRange,
		uint32_t distance) {
}
}


