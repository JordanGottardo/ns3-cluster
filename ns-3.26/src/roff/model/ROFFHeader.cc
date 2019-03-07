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

	ROFFHeader::ROFFHeader(uint32_t type, uint32_t sender,
						   Vector position, boost::dynamic_bitset<> esdBitmap): m_type(type),
																 	 	 	 	m_senderId(sender),
																				m_position(position),
																				m_esdBitmap(esdBitmap) {
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

	boost::dynamic_bitset<> ROFFHeader::GetESDBitmap() const {
		return m_esdBitmap;
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

	void ROFFHeader::SetESDBitmap(const boost::dynamic_bitset<>& esdBitmap) {
		m_esdBitmap = esdBitmap;
	}

//	Methods

	TypeId ROFFHeader::GetInstanceTypeId() const {
		return GetTypeId();
	}

	uint32_t ROFFHeader::GetSerializedSize() const {
//		uint32_t 4 * 2
//		uint64_t (double) 8 * 3
//		return 8 + 32;
//		TODO da rendere dinamica in base a quello letto
		return 4 * 2 + 8 * 3 + 4;
//		return 34;
	}

	void ROFFHeader::WriteDouble(Buffer::Iterator* iter, double d) const {
		const double* buf = &(d);
		const uint8_t* buf2 = reinterpret_cast<const uint8_t*> (buf);
		iter->Write(buf2, 8);
	}

	void ROFFHeader::WriteESDBitmap(Buffer::Iterator* iter) const {
		uint32_t size = m_esdBitmap.size();
		cout << "ROFFHeader::WriteESDBitmap " << m_esdBitmap <<
		if (size > 0 && size % 8 != 0) {
			uint32_t posToShift = 8 - (size & 8);
			if (posToShift < 8) {
				m_esdBitmap >>= posToShift;
			}

		}
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
		start.WriteU32(m_esdBitmap.size());

//		WriteDouble(&start, 5);
//		WriteDouble(&start, 5);
//		WriteDouble(&start, 5);
//		WriteDouble(&start, 5);
//		WriteDouble(&start, 5);
//		start.WriteU16(123);
//		stringstream ss;
//		cout << sizeof (ss) << endl;
//		boost::archive::text_oarchive oa{ss};
//		oa << m_esdBitmap;
//		cout << sizeof (ss) << endl;
//		cout << "ROFFHeader::Serialize " << m_esdBitmap << endl;
//		boost::dynamic_bitset<> a;
//		boost::archive::text_iarchive ia{ss};
//		ia >> a;
//		cout << "a dopo deserialize= " << a << endl;

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
		cout << "ROFFHeader::Deserialize= " << start.GetSize() << endl;
		m_type = start.ReadU32();
		m_senderId = start.ReadU32();
		double x = ReadDouble(&start);
		double y = ReadDouble(&start);
		double z = ReadDouble(&start);
		m_position = Vector(x, y, z);
		uint32_t test = start.ReadU32();
		cout << "test= " << test << endl;
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

