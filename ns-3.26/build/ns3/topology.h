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

#ifndef TOPOLOGY_H
#define TOPOLOGY_H

#include "obstacle.h"
#include <stdio.h>

namespace ns3 {

// CGAL types
typedef CGAL::Range_tree_map_traits_2<K, Obstacle> Traits;
typedef CGAL::Range_tree_2<Traits> Range_tree_2_type;
typedef Traits::Key Key;
typedef Traits::Interval Interval;
typedef std::map<std::string, double> TStrDblMap;
typedef std::pair<std::string, double> TStrDblPair;

/**
 * \ingroup obstacle
 * \brief The Topology class manages a list of obstacles
 * and can be used to load a set of obstacles from a file
 */
class Topology
{
public:
  /**
   * \brief Constructor
   * \return none
   */
  Topology ();


  Topology (uint32_t createFile, uint32_t useFile, std::string mapBasePath,
		  uint32_t droneTest, uint32_t highBuildings);
  /**
   * \brief Run
   * \return none
   */
  void Run ();

  /**
   * \brief CommandSetup
   * \param argc number of arguments
   * \param argv command line arguments
   * \return tbd
   */
  void CommandSetup (int argc, char **argv);

  /**
   * \brief Gets the topology instance
   * \param createFile whether to create file with saved losses
   * \param useFile whether to load file with saved losses
   * \return the topology instance
   */
  static Topology * GetTopology(uint32_t createFile, uint32_t useFile, std::string mapBasePath,
		  uint32_t droneTest, uint32_t highBuildings);

  /**
   * \brief Loads table with saved losses (dBm) from file
   * \return none
   */
  void LoadTableFromFile();
  /**
   * \brief Load buildings into the topology
   * \param bldgFilename the filename that contains buildings data
   * \return none
   */
  static void LoadBuildings(std::string bldgFilename, uint32_t createFile = 0, uint32_t useFile = 0, std::string mapBasePath = "",
		  uint32_t droneTest = 0, uint32_t highBuildings = 0);


  /**
   * \brief Gets the minimum X value of buildings in the topology
   * \return minimum X value of buildings in the topology
   */
  double GetMinX();

  /**
   * \brief Gets the maximum X value of buildings in the topology
   * \return maximum X value of buildings in the topology
   */
  double GetMaxX();

  /**
   * \brief Gets the minimum Y value of buildings in the topology
   * \return minimum Y value of buildings in the topology
   */
  double GetMinY();

  /**
   * \brief Gets the maximum Y value of buildings in the topology
   * \return maximum Y value of buildings in the topology
   */
  double GetMaxY();

  /**
   * \brief Gets the obstructed propagation loss between two points
   * \param p1 point1
   * \param p2 point2
   * \param r limiting radius for obstacles between p1 and p2
   * \return tbd
   */
  double GetObstructedLossBetween(const Point_3 &p1, const Point_3 &p2, double r);

  /**
   * \brief Tests if the topology has any obstacles (loaded within it)
   * \return true if the topology has obstacles, false otherwise
   */
  bool HasObstacles();

  /**
   * \brief Make a range tree (binary space partition, BSP) of obstacles, for searching.
   * Called after all obstacle have been loaded into the topology
   * \return none
   */
  void MakeRangeTree();

  /**
   * \brief Get the topology instance (create if necessary)
   * \return the topology instance
   */
  static Topology ** PeekTopology();

  /**
   * \brief Create a shape for the topolgy
   * \param id identifier (i.e., name) of the shape
   * \param vertices string of vertices that define the shape
	 * \param height string containing the value of the height
   * \return none
   */
  void CreateShape(std::string id, std::string vertices, std::string height);

  /**
   * \brief Create a vertex
   * \param obstacle the obstacle to which the vertex should be added
   * \param vertex information that represents the vertex to be created
   * (and added to the obstacle)
   * \return none
   */
  void CreateVertex(Obstacle &obstacle, std::string vertex);

  /**
   * \brief Get the obstructed distance between two points
   * \param p1 point1
   * \param p2 point2
   * \param obs obstacle that may lie between p1 and p2
   * \param obstructedDistanceBetween the total length within obs
   * traversed by a line between p1 and p2
   * \param intersections the number of intersections of the obstacle for
   * a line between p1 and p2
   * \return tbd
   */
  void GetObstructedDistance(const Point_3 &p1b, const Point_3 &p2b, Obstacle &obs, double &obstructedDistanceBetween, int &intersections);

	/**
	 * \brief Check if a point is inside a special region
	 * \param s1, the first point of the segment
	 * \param s2, the second point of the segment
	 * \param h the height of the polygon
	 * \param p point to test
	 * \return bool true if the point is inside the region
	 */
	bool IsInRegion(const Point &s1, const Point &s2, double h, const Point_3 &ip);

	/**
	 * \brief Check if a point is inside a polygon
	 * upon the segment made up of the first two point and
	 * perpendicular in respect of the ground with given height
	 * \param polygon the polygon
	 * \param ipoint, the point to be checked
	 * \return bool true if the point is inside the polygon
	 */
	bool PointIsInPolygon(const Polygon_2 &polygon, const Point_3 *ipoint);

  // list of obstacles in the topology
  std::vector<Key> m_obstacles;

  // output list, used for sorting
  std::vector<Key> m_outputList;

  // BSP, for searching for obstacles
  Range_tree_2_type m_rangeTree;

  // minimum x value of obstacles in the topology
  double m_minX;

  // minimum y value of obstacles in the topology
  double m_minY;

  // maximum x value of obstacles in the topology
  double m_maxX;

  // maximum y value of obstacles in the topology
  double m_maxY;

  // a cache of obstructed distances between two points
  // (used for performance optimization).
  // Assume that two points that have not moved more than
  // 0.1m from their last locations will have the same
  // line of sight obstacle intersections and interior
  // distances.  This prevent these values from being
  // recalculated deterministically at every evaluation,
  // e.g., when nodes are stationary (obstacle are, too) so
  // there is no change to previously calculated results.
  TStrDblMap m_obstructedDistanceMap;

  //Same as above, except that
// 1) it is loaded from file
// 2) it is not emptied limited to size 1000
  TStrDblMap m_obstructedDistanceMapFromFile;

  uint32_t 		m_createFile;
  uint32_t 		m_useFile;
  std::string 	m_mapBasePath;
  uint32_t		m_droneTest;
  uint32_t		m_highBuildings;
};

} // namespace ns3

#endif  // TOPOLOGY_H
