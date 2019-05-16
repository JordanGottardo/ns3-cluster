/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014 North Carolina State University
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
 * Author: Scott E. Carpenter <scarpen@ncsu.edu>
 *
 */

#include "ns3/log.h"
#include "ns3/double.h"
#include "ns3/enum.h"
#include "ns3/mobility-model.h"
#include <cmath>
#include "ns3/topology.h"

#include "obstacle-shadowing-propagation-loss-model.h"

NS_LOG_COMPONENT_DEFINE ("ObstacleShadowingPropagationLossModel");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (ObstacleShadowingPropagationLossModel);

TypeId
ObstacleShadowingPropagationLossModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::ObstacleShadowingPropagationLossModel")

  .SetParent<PropagationLossModel> ()
	.SetGroupName ("Propagation")
  .AddConstructor<ObstacleShadowingPropagationLossModel> ()
	.AddAttribute ("Radius",
								 "Radius used for optimization (meters)",
								 DoubleValue (200),
								 MakeDoubleAccessor (&ObstacleShadowingPropagationLossModel::m_radius),
								 MakeDoubleChecker<double> ())
	 .AddAttribute("CreateFile",
				   "Whether or not to create file to save obstacle losses keyed by senderId, receiverId",
				   IntegerValue(0),
				   MakeIntegerAccessor(&ObstacleShadowingPropagationLossModel::m_createFile),
				   MakeIntegerChecker<uint32_t>())
     .AddAttribute("UseFile",
				   "Whether or not to use file with saved losses keyed by senderId, receiverId",
				   IntegerValue(0),
				   MakeIntegerAccessor(&ObstacleShadowingPropagationLossModel::m_useFile),
				   MakeIntegerChecker<uint32_t>())
    .AddAttribute("MapBasePath",
				  "Base path of file which contains obstacle losses",
				  StringValue(""),
				  MakeStringAccessor(&ObstacleShadowingPropagationLossModel::m_mapBasePath),
				  MakeStringChecker())
	.AddAttribute("DroneTest",
				  "Whether it is a drone test",
				  IntegerValue(0),
				  MakeIntegerAccessor(&ObstacleShadowingPropagationLossModel::m_droneTest),
				  MakeIntegerChecker<uint32_t>())
	.AddAttribute("HighBuildings",
				  "Whether buildings are higher than drones ( eg 100m) ",
				  IntegerValue(0),
				  MakeIntegerAccessor(&ObstacleShadowingPropagationLossModel::m_highBuildings),
			      MakeIntegerChecker<uint32_t>());

  return tid;
}

ObstacleShadowingPropagationLossModel::ObstacleShadowingPropagationLossModel ()
  : PropagationLossModel ()
{
}

ObstacleShadowingPropagationLossModel::~ObstacleShadowingPropagationLossModel ()
{
}

double
ObstacleShadowingPropagationLossModel::GetLoss (Ptr<MobilityModel> a, Ptr<MobilityModel> b) const
{
  NS_LOG_FUNCTION (this);

  // initialize = no loss

  double L_obs = 0.0;

  // get the topology instance, to search for obstacles
  Topology * topology = Topology::GetTopology(m_createFile, m_useFile, m_mapBasePath, m_droneTest, m_highBuildings);
  NS_ASSERT(topology != 0);
  if (topology->HasObstacles() == true)
    {
      // additional loss for obstacles
      double p1_x = a->GetPosition ().x;
      double p1_y = a->GetPosition ().y;
			double p1_z = a->GetPosition ().z;
      double p2_x = b->GetPosition ().x;
      double p2_y = b->GetPosition ().y;
			double p2_z = b->GetPosition ().z;
      // for two points, p1 and p2
      Point_3 p1(p1_x, p1_y, p1_z);
      Point_3 p2(p2_x, p2_y, p2_z);

//      std::cout << "GetLoss tra pos " << p1 << " e pos " << p2 << std::endl;

      // and testing for obstacles within m_radius=200m
      // get the obstructed loss, from the topology class
      L_obs = topology->GetObstructedLossBetween(p1, p2, m_radius);
    }

  return L_obs;
}

double
ObstacleShadowingPropagationLossModel::DoCalcRxPower (double txPowerDbm,
						Ptr<MobilityModel> a,
						Ptr<MobilityModel> b) const
{
//	std::cout << "GetLoss tra pos " << a->GetPosition() << " e pos " << b->GetPosition() <<
//			" con txPower= " << txPowerDbm << std::endl;
  double retVal = 0.0;
//  if (txPowerDbm < -96.0) {
//	  std::cout << "droppo" << std::endl;
//	  return txPowerDbm;
//  }
  double loss = GetLoss (a, b);
  retVal = txPowerDbm - loss;
//  std::cout << "GetLoss tra pos " << a->GetPosition() << " e pos " << b->GetPosition()
//		  << " valore after loss " << retVal << std::endl;
  return (retVal);
}

int64_t
ObstacleShadowingPropagationLossModel::DoAssignStreams (int64_t stream)
{
  return 0;
}

} // namespace ns3
