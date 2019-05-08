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

	ROFFHeader(uint32_t type,  Vector position, uint32_t sender,  Vector starterPosition,
			   boost::dynamic_bitset<> esdBitmap, uint32_t phase = 0,
			   uint32_t slot = 0, uint8_t senderInJunction = 0, uint64_t junctionId = 0);

//	Getters
	const Vector& GetPosition() const;

	uint32_t GetType() const;

	Vector GetStarterPosition() const;

	uint32_t GetSenderId() const;

	/**
	 * \returns whether the header has been sent by a node inside a junction
	 */
	uint8_t IsSenderInJunction() const;

	/**
	 * \returns whether the id of the junction the sender is in
	 */
	uint64_t GetJunctionId() const;

	boost::dynamic_bitset<> GetESDBitmap() const;

	uint32_t GetPhase() const;

	uint32_t GetSlot() const;


//	Setters
	void SetType(uint32_t type);

	void SetSenderId(uint32_t senderId);

	void SetStarterPosition(const Vector& position);

	void SetPosition(const Vector& position);

	void SetESDBitmap(const boost::dynamic_bitset<>& esdBitmap);

	void SetPhase(uint32_t phase);

	void SetSlot(uint32_t slot);

	/**
	 * \brief set the sender inside a junction
	 * \param value whether the sender is in a junction
	 */
	void SetSenderInJunction (uint8_t value);

	/**
	 * \brief set the id of the junction the sender is in
	 * \param junctionId the id of the junction the sender is in
	 */
	void SetJunctionId (uint64_t junctionId);


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
	Vector								m_position;

	//	============== Hello message data ================
	uint32_t							m_senderId;


	//	============== Alert message data ================
	Vector 								m_starterPosition;
	uint32_t							m_phase;
	uint32_t							m_slot;
	uint8_t								m_senderInJunction;
	uint64_t							m_junctionId;
	boost::dynamic_bitset<>				m_esdBitmap;




};
}


#endif
