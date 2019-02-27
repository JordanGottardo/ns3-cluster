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

using namespace std;

namespace ns3 {

static const uint32_t HELLO_MESSAGE = 0;
static const uint32_t ALERT_MESSAGE = 1;

class ROFFHeader: public Header {
public:

//	Constructor
	ROFFHeader();

//	Getters
	const Vector& GetPosition() const;

	uint32_t GetType() const;

	uint32_t GetSenderId() const;


//	Setters
	void SetType(uint32_t type);

	void SetSenderId(uint32_t senderId);

	void SetPosition(const Vector& position);


	void WriteDouble(Buffer::Iterator* iter, double d) const;

	double ReadDouble(Buffer::Iterator* iter) const;

	virtual TypeId GetInstanceTypeId() const;

	virtual uint32_t GetSerializedSize() const;

	virtual void Serialize(Buffer::Iterator start) const;

	virtual uint32_t Deserialize(Buffer::Iterator start);

	virtual void Print(ostream& os) const;

private:
	//	============== Generic data ================
	uint32_t			m_type;

	//	============== Hello message data ================
//	Position of vehicle sending Hello Message
	uint32_t		m_senderId;
	Vector			m_position;

	//	============== Alert message data ================
// TODO check size of esdBitmap
//					m_esdBitmap



};
}


#endif
