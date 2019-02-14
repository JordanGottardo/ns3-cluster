/*
 * ROFFHeader.h
 *
 *  Created on: Jan 28, 2019
 *      Author: jordan
 */

#ifndef NBTENTRY
#define NBTENTRY

#include "ns3/header.h"
#include "ns3/object-vector.h"
#include "ns3/vector.h"
#include <iostream>
#include <chrono>

using namespace std;

namespace ns3 {

class NBTEntry {
public:

	NBTEntry();

	NBTEntry(Vector position, std::chrono::milliseconds timeStamp);

	Vector GetPosition() const;

	std::chrono::milliseconds GetTimeStamp() const;

	void SetPosition(Vector position);

	void SetTimeStamp(std::chrono::milliseconds timeStamp);





private:
	Vector									m_position; // node's position
	std::chrono::milliseconds				m_timeStamp; // last time the entry was updated



};
}


#endif
