/*
 * ROFFHeader.h
 *
 *  Created on: Jan 28, 2019
 *      Author: jordan
 */

#ifndef NEIGHBORTABLE_H
#define NEIGHBORTABLE_H

#include "NBTEntry.h"

#include <iostream>
#include <chrono>
#include "ns3/nstime.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "float.h"
#include "PositionRankingKey.h"

#include <boost/dynamic_bitset.hpp>

using namespace std;
using namespace std::chrono;

namespace ns3 {

class NeighborTable: public Object {


public:

	static TypeId GetTypeId();

	void AddOrUpdateEntry(uint32_t nodeId, Vector pos, Time timeStamp = Simulator::Now());

	uint32_t GetNBTSize() const;

	boost::dynamic_bitset<> GetESDBitmap(Vector pos, uint32_t distanceRange) const;

	Vector GetCoordsOfVehicleInRange(PositionRankingKey range, Vector nodePosition, int32_t& dist) const;

	bool IsNodeWinnerInContention(uint32_t id, uint32_t dist, Vector pos) const;

private:

	uint32_t GetMaxDistance(Vector pos) const;

	uint32_t ExistsNodeAtDistance(uint32_t dist, uint32_t distanceRange, Vector pos) const;

	map<uint32_t, NBTEntry>									m_table; //NBT




};
}


#endif
