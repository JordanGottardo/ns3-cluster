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


namespace ns3 {

static const uint32_t HELLO_MESSAGE = 0;
static const uint32_t ALERT_MESSAGE = 1;

class ROFFHeader: public Header {
public:

	const Vector& GetPosition() const;

	void SetPosition(const Vector& position);

	uint32_t GetType() const;

	void SetType(uint32_t type);

private:
	//	============== Generic data ================
	uint32_t			m_type;

	//	============== Hello message data ================
//	Position of vehicle sending Hello Message
	Vector			m_position;

	//	============== Alert message data ================
// TODO check size of esdBitmap
//					m_esdBitmap



};
}


#endif /* NS_3_26_SRC_ROFF_MODEL_ROFFHEADER_H_ */
