/*
 * ROFFNode.h
 *
 *  Created on: Jan 28, 2019
 *      Author: jordan
 */

#ifndef POSITIONRANKINGMAP_H
#define POSITIONRANKINGMAP_H

#include "ns3/log.h"
#include "ns3/object.h"
#include "PositionRankingKey.h"
#include <iostream>

using namespace std;

namespace ns3 {

class PositionRankingMap: public Object {

public:

	PositionRankingMap(uint32_t distRange);

	// index is calculated as esdBitmap.size - i - 1
	void AddEntry(uint32_t index, uint32_t priority);

	uint32_t GetPriority(uint32_t distance);

	uint32_t GetUpperDistanceLimit(uint32_t priority);

private:

	uint32_t 									m_distanceRange;
	map<PositionRankingKey, uint32_t> 			m_positionRanking;

};

}



#endif /* POSITIONRANKINGMAP_H*/
