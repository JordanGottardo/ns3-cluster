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
	boost::dynamic_bitset<> esdBitmap;
	if (m_table.empty()) {
		return esdBitmap;
	}
//	cout << "NeighborTable::GetESDBitmap table size= " << m_table.size() << endl;
	uint32_t maxDistance = GetMaxDistance(pos) / distanceRange;
	esdBitmap = boost::dynamic_bitset<>(maxDistance + 1);
//	cout << "NeighborTable::GetESDBitmap maxDistance= " << maxDistance << endl;
//	cout << "NeighborTable::GetESDBitmap bitmapSize= " << esdBitmap.size() << endl;
	for (uint32_t i = 0; i <= maxDistance; i++) {
		// Because boost::dynamic_bitset operator[] goes from less significant to more significant bits
		uint32_t index = maxDistance - i;
//		cout << "NeighborTable::GetESDBitmap index= " << index << endl;
//		cout << "NeighborTable::GetESDBitmap before exists" << endl;
		if (ExistsNodeAtDistance(i, distanceRange, pos)) {
//			cout << "NeighborTable::GetESDBitmap before assign 1" << endl;
			esdBitmap[index] = 1;
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
		NS_LOG_FUNCTION(this << pos << distanceRange << pos);
//		cout << entry.first << endl;
		uint32_t distance = rint(ns3::CalculateDistance(pos, entry.second.GetPosition()));
//		cout << distance << endl;
		if (distance >= distanceRange * dist && distance <= distanceRange*(dist +1) - 1 ) {
			return 1;
		}
	}
	return 0;
}

}

