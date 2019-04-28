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
							  m_position(Vector(0, 0, 0)),
							  m_starterPosition(Vector(0,0,0)),
							  m_phase(0),
							  m_slot(0),
							  m_senderInJunction(0),
							  m_junctionId(0){
	}

	ROFFHeader::ROFFHeader(uint32_t type, Vector position, uint32_t sender, Vector starterPosition,
						   boost::dynamic_bitset<> esdBitmap, uint32_t phase,
						   uint32_t slot, uint8_t senderInJunction, uint64_t junctionId):
																			    m_type(type),
																				m_position(position),
																 	 	 	 	m_senderId(sender),
																				m_starterPosition(starterPosition),
																				m_esdBitmap(esdBitmap),
																				m_phase(phase),
																				m_slot(slot),
																				m_senderInJunction(senderInJunction),
																				m_junctionId(junctionId){
	}

//	Getters

	uint32_t ROFFHeader::GetType() const {
		return m_type;
	}

	Vector ROFFHeader::GetStarterPosition() const {
		return m_starterPosition;
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

	uint32_t ROFFHeader::GetPhase() const {
		return m_phase;
	}

	uint32_t ROFFHeader::GetSlot() const {
		return m_slot;
	}

	uint8_t	ROFFHeader::IsSenderInJunction() const {
		NS_LOG_FUNCTION (this);
		return m_senderInJunction;
	}

	uint64_t ROFFHeader::GetJunctionId() const {
		NS_LOG_FUNCTION (this);
		return m_junctionId;
	}

//	Setters

	void ROFFHeader::SetType(uint32_t type) {
		m_type = type;
	}

	void ROFFHeader::SetSenderId(uint32_t senderId) {
		m_senderId = senderId;
	}

	void ROFFHeader::SetStarterPosition(const Vector& position) {
		m_starterPosition = position;
	}

	void ROFFHeader::SetPosition(const Vector& position) {
		m_position = position;
	}

	void ROFFHeader::SetESDBitmap(const boost::dynamic_bitset<>& esdBitmap) {
		m_esdBitmap = esdBitmap;
	}

	void ROFFHeader::SetPhase(uint32_t phase) {
		m_phase = phase;
	}

	void ROFFHeader::SetSlot(uint32_t slot) {
		m_slot = slot;
	}

	void ROFFHeader::SetSenderInJunction (uint8_t value)
	{
		NS_LOG_FUNCTION (this);
		m_senderInJunction = value;
	}

	void ROFFHeader::SetJunctionId (uint64_t junctionId)
	{
		NS_LOG_FUNCTION (this);
		m_junctionId = junctionId;
	}

//	Methods

	TypeId ROFFHeader::GetInstanceTypeId() const {
		return GetTypeId();
	}

	double ROFFHeader::GetESDBitmapRoundedSizeInBytes(uint32_t bitmapSize) const {
		uint32_t sizeInBytes =  bitmapSize / 8;
		if (bitmapSize % 8 != 0) {
			sizeInBytes++;
		}
		return sizeInBytes;
	}

	uint32_t ROFFHeader::GetSerializedSize() const {
		uint32_t bitmapSize = GetESDBitmapRoundedSizeInBytes(m_esdBitmap.size());
//		cout << "ROFFHeader::GetSerializedSize bitmapSize = " << bitmapSize << endl;
		uint32_t serializedSize =  4 * 4 //m_type, m_senderId, m_phase, m_slot
								   + 8 * 6 // m_position, m_starterPosition
								   + 1 // m_senderInJunction
								   + 8 // m_junctionId
								   + 4 // m_esdBitmap.size
								   + bitmapSize;
//		cout << "ROFFHeader::GetSerializedSize serializedSize = " << serializedSize << endl;
		return serializedSize;
	}

	void ROFFHeader::WriteDouble(Buffer::Iterator* iter, double d) const {
		const double* buf = &(d);
		const uint8_t* buf2 = reinterpret_cast<const uint8_t*> (buf);
		iter->Write(buf2, 8);
	}

	void ROFFHeader::WriteESDBitmap(Buffer::Iterator* iter) const {
		NS_LOG_FUNCTION(this);
		uint32_t size = m_esdBitmap.size();
		if (size == 0) {
			return;
		}
//		cout << "ROFFHeader::WriteESDBitmap original " << m_esdBitmap << endl;
		boost::dynamic_bitset<> esdBitmapToExtend(m_esdBitmap);
		if (size % 8 != 0) {
			uint32_t posToShift = 8 - (size % 8);
			if (posToShift < 8) {
				for (int i = 0; i < posToShift; i++) {
					esdBitmapToExtend.push_back(0);
				}
			}
		}
//				esdBitmapToExtend.to_ulong();
//				cout << "ROFFHeader::WriteESDBitmapToExtend after size= " << esdBitmapToExtend.size() <<
//						" " << esdBitmapToExtend << endl;
				// Writes bitset to buffer chunk by chunk (8 bytes chunk)
//
//				boost::dynamic_bitset<> chunk;
//				chunk.push_back(0);
//				chunk.push_back(1);
//				chunk.push_back(0);
//				chunk.push_back(0);
//				chunk.push_back(0);
//				chunk.push_back(0);
//				chunk.push_back(0);
//				chunk.push_back(1);
//				cout << "chunk= " << chunk << endl;

//				unsigned long ulong = chunk.to_ulong();
//				cout << "ulong= " << ulong << endl;
//				uint8_t byte = ulong;
//				std::bitset<8> x(byte);
////				uint8_t byte = static_cast<uint8_t>(ulong);
//				cout << "byte= " << x << endl;


				for (int i = 0 ; i < esdBitmapToExtend.size(); i+=8) {
//					cout << "for bitmapToExtend i=" << i << endl;
					boost::dynamic_bitset<> chunk(8);
					for (int j = 0; j < 8; j++) {
						chunk[j] = esdBitmapToExtend[i + j];
//						cout << "esdBitmapToExtend[i+j= " << esdBitmapToExtend[i + j] << endl;
					}
					unsigned long ulong = chunk.to_ulong();
//					uint8_t * ptr = reinterpret_cast<uint8_t*>(&ulong);
//					cout << "ptr= " << *(ptr+7) << endl;
//					boost::dynamic_bitset<> bulong(8, ulong);
//					cout << "bulong= " << bulong << endl;
					uint8_t byte = static_cast<uint8_t>(ulong);
					boost::dynamic_bitset<> b(8, byte);
//					cout << "serialize i= " << i << " b= " << b << endl;
					iter->WriteU8(byte);
				}


//				iter->Write(&esdBitmapToExtend, calcolareSize)
//				nb <<= 30;
//				cout << "ROFFHeader::WriteESDBitmap after " << nb << endl;
	}

	void ROFFHeader::Serialize(Buffer::Iterator start) const {
		NS_LOG_FUNCTION(this);
//		cout << "ROFFHeader::Serialize " << endl;
//		cout << "ROFFHEADER Serialize bmp= " << m_esdBitmap << endl;
		start.WriteU32(m_type);
		WriteDouble(&start, m_position.x);
		WriteDouble(&start, m_position.y);
		WriteDouble(&start, m_position.z);
		start.WriteU32(m_senderId);
		WriteDouble(&start, m_starterPosition.x);
		WriteDouble(&start, m_starterPosition.y);
		WriteDouble(&start, m_starterPosition.z);
		start.WriteU32(m_phase);
		start.WriteU32(m_slot);
		start.WriteU8(m_senderInJunction);
		start.WriteU64(m_junctionId);

		start.WriteU32(m_esdBitmap.size());
		WriteESDBitmap(&start);
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

	void ROFFHeader::ConcatBitsets(boost::dynamic_bitset<>& a,
			const boost::dynamic_bitset<>& b, uint32_t count) const {
		for (int i = 0; i < count; i++) {
			a.push_back(b[i]);
		}
	}

	void ROFFHeader::ReadESDBitmap(Buffer::Iterator* iter, uint32_t bitmapSize) {
//		bitmapSize is a multiple of 8

		uint32_t bytesToRead = bitmapSize / 8;
//		cout << "ROFFHeader::ReadESDBitmap bitmapSize= " << bitmapSize << " bytesToRead= " << bytesToRead << endl;
		for (int i = 0; i < bytesToRead; i++) {
			uint8_t value = iter->ReadU8();
//			iter->ReadU8();
//			cout << "value = " << value << endl;
			boost::dynamic_bitset<> b(8, value);
//			cout << "ROFFHeader::readEsdBitmap i= " << i << " esdBitmapBefore " << m_esdBitmap;
//								<< " b= "<< b << endl;
			ConcatBitsets(m_esdBitmap, b);

//			cout << "ROFFHeader::readEsdBitmap after= << " << m_esdBitmap << endl;
//			cout << "i = " << value << endl;
		}

		if (bitmapSize % 8 != 0) {
			uint8_t value = iter->ReadU8();
			boost::dynamic_bitset<> b(8, value);
			uint32_t bitsToCopy = bitmapSize % 8;
			ConcatBitsets(m_esdBitmap, b, bitsToCopy);
		}

//		cout << "ROFFHeader::readEsdBitmap after all read= << " << m_esdBitmap << endl;

	}

	uint32_t ROFFHeader::Deserialize(Buffer::Iterator start) {
		NS_LOG_FUNCTION(this);
//		cout << "ROFFHeader::Deserialize= " << start.GetSize() << endl;
		m_type = start.ReadU32();
		double x = ReadDouble(&start);
		double y = ReadDouble(&start);
		double z = ReadDouble(&start);
		m_position = Vector(x, y, z);
		m_senderId = start.ReadU32();
		double x1 = ReadDouble(&start);
		double y1 = ReadDouble(&start);
		double z1 = ReadDouble(&start);
		m_starterPosition = Vector(x1, y1, z1);

		m_phase = start.ReadU32();
		m_slot = start.ReadU32();
		m_senderInJunction = start.ReadU8();
		m_junctionId = start.ReadU64();
		uint32_t esdBitmapSize = start.ReadU32();
		ReadESDBitmap(&start, esdBitmapSize);
//		cout << "Deserialize m_type = " << m_type << " m_senderId " << m_senderId << "coord= "
//				<< m_position << endl;
		return GetSerializedSize();

	}

	void ROFFHeader::Print(ostream& os) const {
		// todo complete this
		NS_LOG_FUNCTION (this);
		os << "m_type (" << m_type << ") "
		   << "m_senderId (" << m_senderId << ") "
		   << "m_position " << m_position << endl;
	}

}

