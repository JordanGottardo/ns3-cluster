/*
 * ROFFNode.h
 *
 *  Created on: Jan 28, 2019
 *      Author: jordan
 */

#ifndef POSITIONRANKINGKEY_H
#define POSITIONRANKINGKEY_H

#include "ns3/log.h"
#include <iostream>
#include <climits>
using namespace std;

namespace ns3 {

class PositionRankingKey {

public:

	PositionRankingKey(uint32_t low, uint32_t up);

	uint32_t GetUpperDistanceLimit() const;

	bool operator <(const PositionRankingKey& other) const;

	bool IsInRange(const uint32_t distance) const;

private:

	uint32_t m_lowerDistanceLimit;
	uint32_t m_upperDistanceLimit;

};

}



#endif /* POSITIONRANKINGKEY_H*/
