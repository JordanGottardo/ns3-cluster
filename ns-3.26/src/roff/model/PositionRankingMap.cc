/*
 * PositionRankingMap.cc
 *
 *  Created on: Jan 28, 2019
 *      Author: jordan
 */

#include "PositionRankingMap.h"

namespace ns3 {
	NS_LOG_COMPONENT_DEFINE("PositionRankingMap");

	NS_OBJECT_ENSURE_REGISTERED(PositionRankingMap);

//	PositionRankingMap::PositionRankingMap() {}

PositionRankingMap::PositionRankingMap(uint32_t distRange): m_distanceRange(distRange) {
}

PositionRankingMap::PositionRankingMap(uint32_t distRange, boost::dynamic_bitset<> esdBitmap):
		m_distanceRange(distRange) {
	uint32_t priority = 1;
	for (uint32_t i = 0; i < esdBitmap.size(); i++) {
//			cout << "PositionRankingMap::PositionRankingMap bmpSize= " << esdBitmap.size() <<
//					" i= " << i << endl;
		if (esdBitmap[i] == 1) {
			uint32_t index = esdBitmap.size() - i - 1;
//				cout << "PositionRankingMap::PositionRankingMap index= " << index << endl;
			AddEntry(index, priority);
			priority++;
		}
	}
}

void PositionRankingMap::AddEntry(uint32_t index, uint32_t priority) {
	uint32_t lowerDistanceLimit = index * m_distanceRange;
	uint32_t upperDistanceLimit = (index + 1) * m_distanceRange - 1;
	PositionRankingKey rankingKey(lowerDistanceLimit, upperDistanceLimit);
//	cout << "PositionRankingMap::AddEntry lowerDistanceLimit = " << lowerDistanceLimit <<
//			"upperDistanceLimit= " << upperDistanceLimit << endl;
	m_positionRanking[rankingKey] = priority;
}

uint32_t PositionRankingMap::GetPriority(uint32_t distance) {
	for (auto pair: m_positionRanking) {
		if (pair.first.IsInRange(distance)) {
			return pair.second;
		}
	}
	NS_LOG_ERROR("Priority not found for distance= " << distance);
	return INT_MAX;
}

uint32_t PositionRankingMap::GetUpperDistanceLimit(uint32_t priority) {
	for (auto pair: m_positionRanking) {
		if (pair.second == priority) {
			return pair.first.GetUpperDistanceLimit();
		}
	}
	NS_LOG_ERROR("Upper distance limit not found for priority= " << priority);
	return INT_MAX;
}

PositionRankingKey PositionRankingMap::GetRange(uint32_t priority) {
	for (auto pair: m_positionRanking) {
		if (pair.second == priority) {
			return pair.first;
		}
	}
	NS_LOG_ERROR("Range not found for priority= " << priority);
	return PositionRankingKey();
}

std::ostream &operator << (std::ostream &os, const PositionRankingMap& map) {
	for (auto entry: map.m_positionRanking) {
		cout << "PositionRankingMap key= " << entry.first << " priority= " << entry.second << endl;
	}
	return os;
}




}



