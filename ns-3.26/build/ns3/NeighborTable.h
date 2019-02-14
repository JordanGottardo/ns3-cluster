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
using namespace std;
using namespace std::chrono;

namespace ns3 {

class NeighborTable: public Object {
public:

	static TypeId GetTypeId();

	void AddOrUpdateEntry(uint32_t nodeId, Vector pos, milliseconds timeStamp = duration_cast<milliseconds>(
		    system_clock::now().time_since_epoch()));


private:
	map<uint32_t, NBTEntry>									m_table; //NBT




};
}


#endif
