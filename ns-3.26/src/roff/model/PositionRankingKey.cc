/*
 * PositionRankingKey.cc
 *
 *  Created on: Jan 28, 2019
 *      Author: jordan
 */

#include "PositionRankingKey.h"

namespace ns3 {
	NS_LOG_COMPONENT_DEFINE("PositionRankingKey");
//
	NS_OBJECT_ENSURE_REGISTERED(PositionRankingKey);

PositionRankingKey::PositionRankingKey(): m_lowerDistanceLimit(0),
										  m_upperDistanceLimit(0) {
}

PositionRankingKey::PositionRankingKey(uint32_t low, uint32_t up):
																	m_lowerDistanceLimit(low),
																	m_upperDistanceLimit(up) {
}

uint32_t PositionRankingKey::GetLowerDistanceLimit() const {
	return m_lowerDistanceLimit;
}

uint32_t PositionRankingKey::GetUpperDistanceLimit() const {
	return m_upperDistanceLimit;
}

bool PositionRankingKey::operator <(const PositionRankingKey& other) const {
	return m_lowerDistanceLimit < other.m_lowerDistanceLimit;
}

bool PositionRankingKey::IsInRange(const uint32_t distance) const {
	return distance <= m_upperDistanceLimit && distance >= m_lowerDistanceLimit;
}

std::ostream& operator << (std::ostream &os, const PositionRankingKey& key) {
	os << "PositionRankingKey( " << key.m_lowerDistanceLimit << "," << key.m_upperDistanceLimit << ")" << std::endl;
	return os;
}

}



