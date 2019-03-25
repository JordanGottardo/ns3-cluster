/*
 * ROFFHeader.cc
 *
 *  Created on: Jan 28, 2019
 *      Author: jordan
 */


#include "NeighborTable.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("NeighborTable");

NS_OBJECT_ENSURE_REGISTERED(NeighborTable);




TypeId NeighborTable::GetTypeId() {
  static TypeId tid = TypeId("ns3::NeighborTable")
	.SetParent<Object>()
	.SetGroupName("Network");
  return tid;
}

void NeighborTable::AddOrUpdateEntry(uint32_t nodeId, Vector pos, Time timeStamp) {
	NS_LOG_FUNCTION(this << nodeId << pos << timeStamp);
	m_table[nodeId] = NBTEntry(pos, timeStamp);
}

uint32_t NeighborTable::GetNBTSize() const {
	return m_table.size();
}

boost::dynamic_bitset<> NeighborTable::GetESDBitmap(Vector pos, uint32_t distanceRange) const {
	NS_LOG_FUNCTION(this << pos << distanceRange);
//	NS_LOG_DEBUG(this << pos << distanceRange);
	boost::dynamic_bitset<> esdBitmap;
	if (m_table.empty()) {
		return esdBitmap;
	}
//	cout << "NeighborTable::GetESDBitmap table size= " << m_table.size() << endl;
	uint32_t maxDistance = GetMaxDistance(pos) / distanceRange;
	uint32_t esdBitmapSize = maxDistance + 1;
	esdBitmap = boost::dynamic_bitset<>(esdBitmapSize);
//	cout << "NeighborTable::GetESDBitmap maxDistance= " << maxDistance << endl;
//	cout << "NeighborTable::GetESDBitmap bitmapSize= " << esdBitmapSize << endl;
	for (uint32_t i = 0; i < esdBitmapSize; i++) {
		// Because boost::dynamic_bitset operator[] goes from less significant to more significant bits
		uint32_t index = esdBitmapSize - i - 1;
//		cout << "NeighborTable::GetESDBitmap index= " << index << endl;
//		cout << "NeighborTable::GetESDBitmap before exists" << endl;
		if (ExistsNodeAtDistance(i, distanceRange, pos)) {
//			cout << "NeighborTable::GetESDBitmap before assign 1, distance= " << i << "index= " << index << endl;
//			cout << esdBitmap << endl;
			esdBitmap[index] = 1;
//			cout << esdBitmap << endl;
		} else {
//			cout << "NeighborTable::GetESDBitmap before assign 0" << endl;
			esdBitmap[index] = 0;
		}
	}
	return esdBitmap;
}

uint32_t NeighborTable::GetMaxDistance(Vector pos) const {
	NS_LOG_FUNCTION(this << pos);
	double maxDistance = -DBL_MAX;
	for (auto entry: m_table) {
		Vector otherPos = entry.second.GetPosition();

		double distance = ns3::CalculateDistance(pos, otherPos);
		if (distance > maxDistance) {
			maxDistance = distance;
		}
	}
	uint32_t distanceUInt = rint(maxDistance);
	return distanceUInt;
}

uint32_t NeighborTable::ExistsNodeAtDistance(uint32_t dist, uint32_t distanceRange, Vector pos) const {
//	cout << "NeighborTable:: ExistsNodeAtDistance dist= " << dist << " distanceRange= " <<distanceRange <<
//			" pos= " << pos
//			<< endl;
//	cout << m_table.size() << endl;
	for (auto entry: m_table) {
//		cout << "a" << endl;
//		NS_LOG_FUNCTION(this << pos << distanceRange << pos);
//		cout << entry.first << endl;
		uint32_t distance = rint(ns3::CalculateDistance(pos, entry.second.GetPosition()));
//		cout << distance << endl;
		if (distance >= distanceRange * dist && distance <= distanceRange*(dist +1) - 1 ) {
			return 1;
		}
	}
	return 0;
}

Vector NeighborTable::GetCoordsOfVehicleInRange(PositionRankingKey range, Vector nodePosition, int32_t& dist) const {
//	cout << "NeighborTable::GetCoordsOfVehicleInRange range= " << range <<
//					" nodePosition= " << nodePosition << endl;
//	check if the
	for (auto entry: m_table) {
		Vector otherNodePosition = entry.second.GetPosition();
//		cout << "NeighborTable::GetCoordsOfVehicleInRange otherNodePosition= " << otherNodePosition << endl;
		uint32_t distance = rint(ns3::CalculateDistance(nodePosition, otherNodePosition));
//		cout << distance << endl;
		for (uint32_t d = range.GetLowerDistanceLimit(); d <= range.GetUpperDistanceLimit(); d++) {
//			cout << distance << " " << d << endl;
			if (distance == d) {
				dist = d;
				return otherNodePosition;
			}
		}
	}


	NS_LOG_INFO("NeighborTable::GetCoordsOfVehicleInRange "
			"Coords not found for vehicle in range= " << range << " and node in pos= " << nodePosition);
	dist = -1;
	return Vector();
}

	bool NeighborTable::IsNodeWinnerInContention(uint32_t id, uint32_t dist, Vector pos) const {
		for (auto entry: m_table) {
	//		cout << "a" << endl;
	//		NS_LOG_FUNCTION(this << pos << distanceRange << pos);
	//		cout << entry.first << endl;
			uint32_t otherNodeId = entry.first;
			uint32_t distance = rint(ns3::CalculateDistance(pos, entry.second.GetPosition()));
	//		cout << distance << endl;
			if (distance == dist && otherNodeId > id) {
				return false;
			}
		}
		return true;
	}


}

