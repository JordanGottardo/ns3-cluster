/*
 * ROFFHeader.cc
 *
 *  Created on: Jan 28, 2019
 *      Author: jordan
 */


#include "NBTEntry.h"



namespace ns3 {
//NS_LOG_COMPONENT_DEFINE("NBTEntry");

//NS_OBJECT_ENSURE_REGISTERED(NBTEntry);

NBTEntry::NBTEntry() {
}

NBTEntry::NBTEntry(Vector position, Time timeStamp): m_position(position),
										   	   	   	 m_timeStamp(timeStamp) {

}


Vector NBTEntry::GetPosition() const {
	return m_position;
}

Time NBTEntry::GetTimeStamp() const {
	return m_timeStamp;
}

void NBTEntry::SetPosition(Vector position) {
	m_position = position;
}

void NBTEntry::SetTimeStamp(Time timeStamp) {
	m_timeStamp = timeStamp;
}



}

