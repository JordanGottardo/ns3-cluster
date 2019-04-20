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

#include "FBHeader.h"

#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "ns3/object-vector.h"

namespace ns3 {
	NS_LOG_COMPONENT_DEFINE ("FBHeader");

	NS_OBJECT_ENSURE_REGISTERED (FBHeader);

	TypeId
	FBHeader::GetTypeId (void)
	{
	  static TypeId tid = TypeId ("ns3::FBHeader")
	    .SetParent<Header> ()
	    .SetGroupName("Network");

	  return tid;
	}

	FBHeader::~FBHeader ()
	{
	  NS_LOG_FUNCTION (this);
	}

	void
	FBHeader::SetPosition (Vector pos)
	{
		NS_LOG_FUNCTION (this);
		m_position = pos;
	}

	void
	FBHeader::SetStarterPosition (Vector pos)
	{
		NS_LOG_FUNCTION (this);
		m_starterPosition = pos;
	}

	void
	FBHeader::SetMaxRange (uint32_t value)
	{
		NS_LOG_FUNCTION (this);
		m_maxRange = value;
	}

	void
	FBHeader::SetType (uint32_t value)
	{
		NS_LOG_FUNCTION (this);
		m_type = value;
	}

	void
	FBHeader::SetSlot (uint32_t value)
	{
		NS_LOG_FUNCTION (this);
		m_slot = value;
	}

	void
	FBHeader::SetPhase (uint32_t value)
	{
		NS_LOG_FUNCTION (this);
		m_phase = value;
	}

	void
	FBHeader::SetSenderId (uint32_t value)
	{
		NS_LOG_FUNCTION (this);
		m_senderId = value;
	}

	void
	FBHeader::SetSenderInJunction (uint8_t value)
	{
		NS_LOG_FUNCTION (this);
		m_senderInJunction = value;
	}

	void
	FBHeader::SetJunctionId (uint64_t junctionId)
	{
		NS_LOG_FUNCTION (this);
		m_junctionId = junctionId;
	}

	Vector
	FBHeader::GetPosition (void) const
	{
		NS_LOG_FUNCTION (this);
		return m_position;
	}

	Vector
	FBHeader::GetStarterPosition (void) const
	{
		NS_LOG_FUNCTION (this);
		return m_starterPosition;
	}

	uint32_t
	FBHeader::GetMaxRange (void) const
	{
		NS_LOG_FUNCTION (this);
		return m_maxRange;
	}

	uint32_t
	FBHeader::GetType (void) const
	{
		NS_LOG_FUNCTION (this);
		return m_type;
	}

	uint32_t
	FBHeader::GetSlot (void) const
	{
		NS_LOG_FUNCTION (this);
		return m_slot;
	}

	uint32_t
	FBHeader::GetPhase (void) const
	{
		NS_LOG_FUNCTION (this);
		return m_phase;
	}

	uint32_t
	FBHeader::GetSenderId (void) const
	{
		NS_LOG_FUNCTION (this);
		return m_senderId;
	}

	uint8_t
	FBHeader::IsSenderInJunction (void) const
	{
		NS_LOG_FUNCTION (this);
		return m_senderInJunction;
	}

	uint64_t
	FBHeader::GetJunctionId (void) const
	{
		NS_LOG_FUNCTION (this);
		return m_junctionId;
	}

	TypeId
	FBHeader::GetInstanceTypeId (void) const
	{
		return GetTypeId ();
	}

	uint32_t
	FBHeader::GetSerializedSize (void) const
	{
		// Vector3D = 24  * 2
		// uint32_t = 5	* 4
		// uint8_t = 1 * 1
		// uint64_t = 1 * 8

		NS_LOG_FUNCTION (this);

		uint32_t length = 77;
		return length;
	}

	void
	FBHeader::Serialize (Buffer::Iterator start) const
	{
		NS_LOG_FUNCTION (this);
		Buffer::Iterator i = start;
		i.WriteU64(m_position.x);
		i.WriteU64(m_position.y);
		i.WriteU64(m_position.z);
		i.WriteU64(m_starterPosition.x);
		i.WriteU64(m_starterPosition.y);
		i.WriteU64(m_starterPosition.z);
		i.WriteU32(m_maxRange);
		i.WriteU32(m_type);
		i.WriteU32(m_slot);
		i.WriteU32(m_phase);

		i.WriteU32(m_senderId);
		i.WriteU8(m_senderInJunction);
		i.WriteU64(m_junctionId);
	}

	uint32_t
	FBHeader::Deserialize (Buffer::Iterator start)
	{
		NS_LOG_FUNCTION (this);
		Buffer::Iterator i = start;
		uint32_t x, y, z;

		x = i.ReadU64 ();
		y = i.ReadU64 ();
		z = i.ReadU64 ();
		m_position = Vector (x, y, z);
		x = i.ReadU64 ();
		y = i.ReadU64 ();
		z = i.ReadU64 ();
		m_starterPosition = Vector (x, y, z);
		m_maxRange = i.ReadU32 ();
		m_type = i.ReadU32 ();
		m_slot = i.ReadU32 ();
		m_phase = i.ReadU32 ();

		m_senderId = i.ReadU32();
		m_senderInJunction = i.ReadU8();
		m_junctionId = i.ReadU64();

		return  GetSerializedSize  ();
	}

	void
	FBHeader::Print (std::ostream &os) const
	{
		NS_LOG_FUNCTION (this);
		os << "m_position (" << m_position << ") "
			<< "m_starterPosition (" << m_starterPosition << ") "
			<< "m_maxRange " << m_maxRange << " "
			<< "m_type " << m_maxRange << " "
			<< "m_slot " << m_slot << " "
			<< "m_phase " << m_phase << " "
			<< "m senderId " << m_senderId <<  " "
			<< "m_senderInJunction " << m_senderInJunction << " "
			<< "m_junctionId " << m_junctionId << std::endl;
	}

} // namespace ns3
