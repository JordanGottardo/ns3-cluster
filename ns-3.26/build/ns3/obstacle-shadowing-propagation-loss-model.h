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

#ifndef OBSTACLE_SHADOWING_PROPAGATION_LOSS_MODEL_H
#define OBSTACLE_SHADOWING_PROPAGATION_LOSS_MODEL_H

#include "ns3/propagation-loss-model.h"

namespace ns3 {

/**
 * \ingroup obstacle
 *
 * \brief the Obstalce Shadowing propagation model
 *
 * This class implements the Obstacle Shadowing propagation loss model.
 * For more information about the model, please see
 * the propagation module documentation in .rst format.
 */
class ObstacleShadowingPropagationLossModel : public PropagationLossModel
{

public:
  // inherited from Object
  static TypeId GetTypeId (void);

  /**
   * \brief Constructor
   * \return none
   */
  ObstacleShadowingPropagationLossModel ();

  /**
   * \brief Deconstructor
   * \return none
   */
  virtual ~ObstacleShadowingPropagationLossModel ();

  /**
   *
   *
   * \param a the first mobility model
   * \param b the second mobility model
   *
   * \return the loss in dBm for the propagation between
   * the two given mobility models
   */
  double GetLoss (Ptr<MobilityModel> a, Ptr<MobilityModel> b) const;

private:

  // inherited from PropagationLossModel
  virtual double DoCalcRxPower (double txPowerDbm,
                                Ptr<MobilityModel> a,
                                Ptr<MobilityModel> b) const;
  virtual int64_t DoAssignStreams (int64_t stream);

	double			m_radius;
	uint32_t 		m_createFile;
	uint32_t 		m_useFile;
	std::string 	m_mapBasePath;
	uint32_t		m_droneTest;
	uint32_t		m_highBuildings;
};

} // namespace ns3


#endif // OBSTACLE_SHADOWING_PROPAGATION_LOSS_MODEL_H
