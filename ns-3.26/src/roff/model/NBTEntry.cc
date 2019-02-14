/*
 * ROFFHeader.cc
 *
 *  Created on: Jan 28, 2019
 *      Author: jordan
 */


#include "NBTEntry.h"

#include "ns3/log.h"

namespace ns3 {
//	NS_LOG_COMPONENT_DEFINE("NBTEntry");
//
//	NS_OBJECT_ENSURE_REGISTERED(NBTEntry);

NBTEntry::NBTEntry() {
}

NBTEntry::NBTEntry(Vector position, std::chrono::milliseconds timeStamp): m_position(position),
																		  m_timeStamp(timeStamp) {

}


Vector NBTEntry::GetPosition() const {
	return m_position;
}

std::chrono::milliseconds NBTEntry::GetTimeStamp() const {
	return m_timeStamp;
}

void NBTEntry::SetPosition(Vector position) {
	m_position = position;
}

void NBTEntry::SetTimeStamp(std::chrono::milliseconds timeStamp) {
	m_timeStamp = timeStamp;
}



}

