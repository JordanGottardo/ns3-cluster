/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015 North Carolina State University
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

#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <fstream>
#include <iostream>
#include <map>
#include "ns3/core-module.h"

// CGAL includes
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Ray_2.h>
#include <CGAL/Point_2.h>
#include <CGAL/Point_3.h>
#include <CGAL/Segment_2.h>
#include <CGAL/Segment_3.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Bbox_2.h>
#include <CGAL/Cartesian.h>
#include <CGAL/Range_segment_tree_traits.h>
#include <CGAL/Range_tree_k.h>
#include <CGAL/Plane_3.h>

// CGAL types
typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::FT Coord_type;
typedef CGAL::Ray_2<K> Ray_2;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Point_3<K> Point_3;
typedef CGAL::Segment_2<K> Segment_2;
typedef CGAL::Segment_3<K> Segment_3;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef CGAL::Bbox_2 Bbox_2;
typedef CGAL::Plane_3<K> Plane_3;
typedef CGAL::Polygon_2<K>::Edge_const_iterator EdgeIterator;

namespace ns3 {
/**
 * \ingroup Obstacle
 * \brief The Obstacle class maintains the information for a
 * polygon representation of an Obstacle and per-wall and per-meter
 * attenuation values.
 */
class Obstacle
{
public:
  /**
   * \brief Constructor
   * \return none
   */
  Obstacle ();

  /**
   * \brief Sets the id (i.e., name) of the Obstacle
   * \param id the identifier (i.e., name) of the Obstacle
   * \return none
   */
  void SetId(std::string id);

  /**
   * \brief Gets the id (i.e., name) of the Obstacle
   * \return the id (i.e., name) of the Obstacle
   */
  const std::string GetId();

  /**
   * \brief Adds a vertex to the Obstacle
   * \param p the vertex (a CGAL Point)
   * \return none
   */
  void AddVertex(Point p);

  /**
   * \brief Calculates the location of the centerpoint
   * of the bounding box of the polygon, used for
   * optimization of searching for Obstacle3Ds.  To be
   * called after the last vertex is added to the
   * Obstacle (so that the centerpoint can then be located)
   * \return none
   */
  void Locate();

  /**
   * \brief Get the centerpoint of the Obstacle
   * \return the centerpoint (i.e., midpoint of longest
   * ray between vertices that traverses the interior)
   */
  const Point &GetCenter();

  /**
   * \brief Gets the radius of the Obstacle3Ds region
   * (squared for performance optimizations)
   * \return the radius (squared) =
   * (1/2 of longest interior ray) ^ 2
   */
  double GetRadiusSq();

  /**
   * \brief Gets the polygonal region that defines the Obstacle
   * \return The polygonal region defining the Obstacle
   * (i.e., a CGAL Polygon_2)
   */
  Polygon_2 &GetPolygon();

  /**
   * \brief Gets the value of beta, the per-wall
   * attenuation parameter
   * \return beta, the per-wall attenuation parameter
   */
  double GetBeta();

  /**
   * \brief Gets the value of gamma, the per-meter
   * attenuation parameter
   * \return gamma, the per-meter attenuation parameter
   */
  double GetGamma();

	/**
   * \brief Gets the height of the obstacle in meters
   * \return the height
   */
	double GetHeight();

  /**
   * \brief Sets the value of beta, the per-wall
   * attenuation parameter
   * \param beta, the per-meter attenuation parameter
   * \return none
   */
  void SetBeta(double beta);

  /**
   * \brief Sets the value of gamma, the per-meter
   * attenuation parameter
   * \param gamma, the per-meter attenuation parameter
   * \return none
   */
  void SetGamma(double gamma);

	/**
   * \brief Sets the value of the height in meters
   * \param height, the value of the height
   * \return none
   */
	void SetHeight(double height);

private:

  // the Obstacle identifier (i.e., name)
  std::string m_id;

  // 2D polygonal represenation of the obsstacle (i.e., a CGAL Polygon_2)
  Polygon_2 m_obstacle;

  // centerpoint of Obstacle bounding box
  // i.e., the midpoint of the longest ray between vertices that
  // traverses the interior of the polygon.  used for search optimizations)
  Point m_center;

  // radius squared from centerpoint to bounding box vertex
  double m_radiusSq;

  // For IEEE 802.11p propagation loss through Obstacle3Ds
  // See C. Sommer et. al.:
  // A Computationally Inexpensive Empirical Model of IEEE 802.11p
  // Radio Shadowing in Urban Environments;
  double m_beta;  // per-wall attenuation parameter
  double m_gamma; // per-meter attenuation parameter

	double m_height; // height of the obstacle [in meters]
};

}

#endif /* OBSTACLE_H */
