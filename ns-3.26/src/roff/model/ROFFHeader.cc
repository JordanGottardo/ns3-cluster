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

//	Constructor

	ROFFHeader::ROFFHeader(): m_type(HELLO_MESSAGE),
							  m_senderId(0),
							  m_position(Vector(0, 0, 0)) {
	}

	ROFFHeader::ROFFHeader(uint32_t type, uint32_t sender, Vector position): m_type(type),
																 m_senderId(sender),
																 m_position(position) {

	}

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
//		uint64_t (double) 8 * 3
//		return 8 + 32;
		return 4 * 2 + 8 * 3;
	}

	void ROFFHeader::WriteDouble(Buffer::Iterator* iter, double d) const {
		const double* buf = &(d);
		const uint8_t* buf2 = reinterpret_cast<const uint8_t*> (buf);
		iter->Write(buf2, 8);
	}

	void ROFFHeader::Serialize(Buffer::Iterator start) const {
		NS_LOG_FUNCTION(this);

		start.WriteU32(m_type);
		start.WriteU32(m_senderId);
//		cout << "Serialize m_type = " << m_type << " m_senderId" << m_senderId
//				<< " coord " << m_position << endl;
		WriteDouble(&start, m_position.x);
		WriteDouble(&start, m_position.y);
		WriteDouble(&start, m_position.z);
	}

	double ROFFHeader::ReadDouble(Buffer::Iterator* iter) const {
		uint64_t x;
		uint8_t* buf = reinterpret_cast<uint8_t*>(&x);
		iter->Read(buf, 8);
		double d = *(reinterpret_cast<double*>(buf));
		return d;
	}

	uint32_t ROFFHeader::Deserialize(Buffer::Iterator start) {
		NS_LOG_FUNCTION(this);

		m_type = start.ReadU32();
		m_senderId = start.ReadU32();
		double x = ReadDouble(&start);
		double y = ReadDouble(&start);
		double z = ReadDouble(&start);
		m_position = Vector(x, y, z);
//		cout << "Deserialize m_type = " << m_type << " m_senderId " << m_senderId << "coord= "
//				<< m_position << endl;
		return GetSerializedSize();

	}

	void ROFFHeader::Print(ostream& os) const {
		NS_LOG_FUNCTION (this);
		os << "m_type (" << m_type << ") "
		   << "m_senderId (" << m_senderId << ") "
		   << "m_position " << m_position << endl;
	}

}

