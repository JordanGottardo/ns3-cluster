/*
 * ROFFHeader.h
 *
 *  Created on: Jan 28, 2019
 *      Author: jordan
 */

#ifndef ROFFHEADER_H_
#define ROFFHEADER_H_

#include "ns3/header.h"
#include "ns3/object-vector.h"
#include "ns3/vector.h"
#include "ns3/double.h"
#include "ns3/log.h"
#include <bitset>
#include <boost/dynamic_bitset.hpp>
#include <boost/dynamic_bitset/serialization.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

using namespace std;

namespace ns3 {

static const uint32_t HELLO_MESSAGE = 0;
static const uint32_t ALERT_MESSAGE = 1;

class ROFFHeader: public Header {
public:

//	Constructor
	ROFFHeader();

	ROFFHeader(uint32_t type, uint32_t sender, Vector position,
			   boost::dynamic_bitset<> esdBitmap, uint32_t phase = 0, uint32_t slot = 0);

//	Getters
	const Vector& GetPosition() const;

	uint32_t GetType() const;

	uint32_t GetSenderId() const;

	boost::dynamic_bitset<> GetESDBitmap() const;

	uint32_t GetPhase() const;

	uint32_t GetSlot() const;


//	Setters
	void SetType(uint32_t type);

	void SetSenderId(uint32_t senderId);

	void SetPosition(const Vector& position);

	void SetESDBitmap(const boost::dynamic_bitset<>& esdBitmap);

	void SetPhase(uint32_t phase);

	void SetSlot(uint32_t slot);


	virtual TypeId GetInstanceTypeId() const;

	virtual uint32_t GetSerializedSize() const;

	virtual void Serialize(Buffer::Iterator start) const;

	virtual uint32_t Deserialize(Buffer::Iterator start);

	virtual void Print(ostream& os) const;

private:

	void WriteDouble(Buffer::Iterator* iter, double d) const;

	void WriteESDBitmap(Buffer::Iterator* iter) const;

	void ReadESDBitmap(Buffer::Iterator* iter, uint32_t bitmapSize);

	double ReadDouble(Buffer::Iterator* iter) const;

	double GetESDBitmapRoundedSizeInBytes(uint32_t bitmapSize) const;

	void ConcatBitsets(boost::dynamic_bitset<>& a, const boost::dynamic_bitset<>& b, uint32_t count=8) const;

	//	============== Generic data ================
	uint32_t							m_type;

	//	============== Hello message data ================
	uint32_t							m_senderId;
	Vector								m_position;

	//	============== Alert message data ================
	uint32_t							m_phase;
	uint32_t							m_slot;
	boost::dynamic_bitset<>				m_esdBitmap;




};
}


#endif
