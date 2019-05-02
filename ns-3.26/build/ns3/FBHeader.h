/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2017 Università di Padova
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors: Marco Romanelli <marco.romanelli.1@studenti.unipd.it>
 */

#ifndef FBHEADER_H
#define FBHEADER_H

#include "ns3/header.h"
#include "ns3/object-vector.h"
#include "ns3/vector.h"

namespace ns3 {

static const uint32_t HELLO_MESSAGE = 0;
static const uint32_t ALERT_MESSAGE = 1;

/**
 * \ingroup network
 * \brief A special header used in Fast Broadcast protocol.
 */
class FBHeader : public Header
{
public:
	/**
	 * \brief Get the type ID.
	 * \return the object TypeId
	 */
	static TypeId GetTypeId (void);
	virtual ~FBHeader();

	/**
	 * \brief set the spatial location (gps) of the sender
	 * \param pos the x-y-z location
	 */
	void SetPosition (Vector pos);

	/**
	 * \brief set the spatial location (gps) of the starter node
	 * \param pos the x-y-z location
	 */
	void SetStarterPosition (Vector pos);

	/**
	 * \brief set the maximum range
	 * \param value maximum range
	 */
	void SetMaxRange (uint32_t value);

	/**
	 * \brief set the type of the message
	 * \param value type of the message
	 */
	void SetType (uint32_t value);

	/**
	 * \brief set the slot of the message
	 * \param value slot of the message
	 */
	void SetSlot (uint32_t value);

	/**
	 * \brief set the phase of the message
	 * \param value phase of the message
	 */
	void SetPhase (uint32_t value);

	/**
	 * \brief set the senderId of the message
	 * \param value senderId of the message
	 */
	void SetSenderId(uint32_t value);

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



	/**
	 * \returns the spatial location (gps) of the sender
	 */
	Vector GetPosition (void) const;

	/**
	 * \returns the spatial location (gps) of the starter node
	 */
	Vector GetStarterPosition (void) const;

	/**
	 * \returns the maximum range
	 */
	uint32_t GetMaxRange (void) const;

	/**
	 * \returns the type of the message
	 */
	uint32_t GetType (void) const;

	/**
	 * \returns the slot of the message
	 */
	uint32_t GetSlot (void) const;

	/**
	 * \returns the phase of the message
	 */
	uint32_t GetPhase (void) const;

	/**
	 * \returns the senderId of the message
	 */
	uint32_t GetSenderId(void) const;

	/**
	 * \returns whether the header has been sent by a node inside a junction
	 */
	uint8_t IsSenderInJunction(void) const;

	/**
	 * \returns whether the id of the junction the sender is in
	 */
	uint64_t GetJunctionId(void) const;

	/**
   * Get the most derived TypeId for this Object.
   *
   * This method is typically implemented by ns3::Object::GetInstanceTypeId
   * but some classes which derive from ns3::ObjectBase directly
   * have to implement it themselves.
   *
   * \return The TypeId associated to the most-derived type
   *          of this instance.
   */
  virtual TypeId GetInstanceTypeId (void) const;

	/**
	 * \returns the expected size of the header.
	 *
	 * This method is used by Packet::AddHeader
	 * to store a header into the byte buffer of a packet. This method
	 * should return the number of bytes which are needed to store
	 * the full header data by Serialize.
	 */
	virtual uint32_t GetSerializedSize (void) const;

	/**
	 * \param start an iterator which points to where the header should
	 *        be written.
	 *
	 * This method is used by Packet::AddHeader to
	 * store a header into the byte buffer of a packet.
	 * The data written
	 * is expected to match bit-for-bit the representation of this
	 * header in a real network.
	 */
	virtual void Serialize (Buffer::Iterator start) const;

	/**
   * \param start an iterator which points to where the header should
   *        read from.
   * \returns the number of bytes read.
   *
   * This method is used by Packet::RemoveHeader to
   * re-create a header from the byte buffer of a packet.
   * The data read is expected to
   * match bit-for-bit the representation of this header in real
   * networks.
   *
   * Note that data is not actually removed from the buffer to
   * which the iterator points.  Both Packet::RemoveHeader() and
   * Packet::PeekHeader() call Deserialize(), but only the RemoveHeader()
   * has additional statements to remove the header bytes from the
   * underlying buffer and associated metadata.
   */
  virtual uint32_t Deserialize (Buffer::Iterator start);

	/**
   * \param os output stream
   * This method is used by Packet::Print to print the
   * content of a header as ascii data to a c++ output stream.
   * Although the header is free to format its output as it
   * wishes, it is recommended to follow a few rules to integrate
   * with the packet pretty printer: start with flags, small field
   * values located between a pair of parens. Values should be separated
   * by whitespace. Follow the parens with the important fields,
   * separated by whitespace.
   * i.e.: (field1 val1 field2 val2 field3 val3) field4 val4 field5 val5
   */
	virtual void Print (std::ostream &os) const;

private:
	Vector 				m_position;
	Vector 				m_starterPosition;
	uint32_t			m_maxRange;
	uint32_t			m_type;
	uint32_t			m_slot;
	uint32_t			m_phase;

	uint32_t			m_senderId;
	uint8_t				m_senderInJunction;
	uint64_t			m_junctionId;
};

} // namespace ns3

#endif /* FBHEADER_H */
