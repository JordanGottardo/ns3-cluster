/*
 * ROFFHeader.cc
 *
 *  Created on: Jan 28, 2019
 *      Author: jordan
 */


#include "NeighborTable.h"

#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("NeighborTable");
//
NS_OBJECT_ENSURE_REGISTERED(NeighborTable);


TypeId NeighborTable::GetTypeId() {
  static TypeId tid = TypeId("ns3::NeighborTable")
	.SetParent<Object>()
	.SetGroupName("Network");
  return tid;
}

void NeighborTable::AddOrUpdateEntry(uint32_t nodeId, Vector pos, milliseconds timeStamp) {
	NS_LOG_FUNCTION(this << nodeId << pos << to_string(timeStamp.count()));
	m_table[nodeId] = NBTEntry(pos, timeStamp);
}


}

