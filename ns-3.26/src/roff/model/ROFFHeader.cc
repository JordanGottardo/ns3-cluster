/*
 * ROFFHeader.cc
 *
 *  Created on: Jan 28, 2019
 *      Author: jordan
 */


#include "ROFFHeader.h"



namespace ns3 {
	NS_LOG_COMPONENT_DEFINE("ROFFHeader");

	NS_OBJECT_ENSURE_REGISTERED(ROFFHeader);

//	Getters

	uint32_t ROFFHeader::GetType() const {
		return m_type;
	}

	uint32_t ROFFHeader::GetSenderId() const {
		return m_senderId;
	}

	const Vector& ROFFHeader::GetPosition() const {
		return m_position;
	}

//	Setters

	void ROFFHeader::SetType(uint32_t type) {
		m_type = type;
	}

	void ROFFHeader::SetSenderId(uint32_t senderId) {
		m_senderId = senderId;
	}

	void ROFFHeader::SetPosition(const Vector& position) {
		m_position = position;
	}

//	Methods

	TypeId ROFFHeader::GetInstanceTypeId() const {
		return GetTypeId();
	}

	uint32_t ROFFHeader::GetSerializedSize() const {
//		uint32_t 4 * 2
//		uint64_t (double) 8 * 4
		return 8 + 32;
	}

	void ROFFHeader::Serialize(Buffer::Iterator start) const {
		NS_LOG_FUNCTION(this);
		Buffer::Iterator i = start;

		i.WriteU32(m_type);
		i.WriteU32(m_senderId);
		const double* const buf = &(m_position.x);
		const uint8_t* const buf2 = reinterpret_cast<const uint8_t* const> (buf);
		i.Write(buf2, 8);
		i.WriteU64(m_position.y);
		i.WriteU64(m_position.z);

//		DoubleValue v(m_position.x);
//		v.
	}

	uint32_t ROFFHeader::Deserialize(Buffer::Iterator start) {
		NS_LOG_FUNCTION(this);
		Buffer::Iterator i = start;

		m_type = i.ReadU32();
		m_senderId = i.ReadU32();
		uint32_t x, y, z;
		uint8_t* buf;
		i.Read(buf, 8);
		double x1 = *(reinterpret_cast<double*>(buf));
		cout << x1 << endl;
		y = i.ReadU64();
		z = i.ReadU64();
		m_position = Vector(x, y, z);
//		TODO serializzare meglio i double
		return GetSerializedSize();

	}

	void ROFFHeader::Print(ostream& os) const {
		NS_LOG_FUNCTION (this);
		os << "m_type (" << m_type << ") "
		   << "m_senderId (" << m_senderId << ") "
		   << "m_position " << m_position << endl;
	}

}

