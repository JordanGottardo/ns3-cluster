/*
 * ROFFHeader.cc
 *
 *  Created on: Jan 28, 2019
 *      Author: jordan
 */


#include "ROFFHeader.h"

#include "ns3/log.h"

namespace ns3 {
	NS_LOG_COMPONENT_DEFINE ("ROFFHeader");

	NS_OBJECT_ENSURE_REGISTERED (ROFFHeader);

	const Vector& ROFFHeader::GetPosition() const {
		return m_position;
	}

	void ROFFHeader::SetPosition(const Vector& position) {
		m_position = position;
	}

	uint32_t ROFFHeader::GetType() const {
		return m_type;
	}

	void ROFFHeader::SetType(uint32_t type) {
		m_type = type;
	}

}

