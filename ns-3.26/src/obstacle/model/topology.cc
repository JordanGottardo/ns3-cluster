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

// CGAL includes
#include <CGAL/intersections.h>

#include "topology.h"

using namespace ns3;

#define noop	// null-operation


NS_LOG_COMPONENT_DEFINE ("topology");

Topology::Topology () :
  // initially very large values
  // so that obstacle bounding box
  // updates will be made
  m_minX(999999999.0),
  m_minY(999999999.0),
  m_maxX(-999999999.0),
  m_maxY(-999999999.0),
  m_createFile(0),
  m_useFile(0),
  m_mapBasePath(""),
  m_droneTest(0),
  m_highBuildings(0)
//  m_createFile(0)
{
  NS_LOG_FUNCTION (this);
}

Topology::Topology(uint32_t createFile, uint32_t useFile, std::string mapBasePath,
		uint32_t droneTest, uint32_t highBuildings):
  // initially very large values
  // so that obstacle bounding box
  // updates will be made
  m_minX(999999999.0),
  m_minY(999999999.0),
  m_maxX(-999999999.0),
  m_maxY(-999999999.0),
  m_createFile(createFile),
  m_useFile(useFile),
  m_mapBasePath(mapBasePath),
  m_droneTest(droneTest),
  m_highBuildings(highBuildings)
//  m_createFile(0)
{
  NS_LOG_FUNCTION (this);
}

void Topology::CommandSetup (int argc, char **argv)
{
  NS_LOG_FUNCTION (this);

  CommandLine cmd;

  cmd.Parse (argc, argv);
}

void
Topology::Run ()
{
  NS_LOG_FUNCTION (this);

  NS_LOG_INFO ("Topology::Run()");
}

Topology **
Topology::PeekTopology (void)
{
  // ensure no topology exists
  static Topology *topo = 0;
  return &topo;
}

void
Topology::LoadTableFromFile() {

	std::string extension;
	std::string fileName;
	if (m_droneTest) {
		extension = ".3Dlosses";
	}
	else {
		extension = ".losses";
	}

	if (m_highBuildings) {
		fileName = m_mapBasePath + "-100" + extension;
	}
	else {
		fileName = m_mapBasePath + extension;
	}
	std::cout << "Loading losses from file " << fileName << std::endl;
	std::ifstream file (fileName.c_str (), std::ios::in);
//	std::cout << "Topology::LoadTableFromFile filename= " << fileName << std::endl;
	if (!(file.is_open())){
		NS_FATAL_ERROR("Could not open buildings file " << fileName.c_str() << " for reading, aborting here \n");
	}

	while (!file.eof()) {
		std::string line;
		getline(file, line);
		if (line.empty()) {
			continue;
		}
//		std::cout << line << std::endl;
		int pos = line.find(";");
		std::string key = line.substr(0, pos);
		int keySize = key.size();
		int halfKeySize = keySize / 2;
		// A to B is the same B to A
		std::string key2 = key.substr(halfKeySize + 1, keySize) + " " +  key.substr(0, halfKeySize);
//		std::cout << "key1 = " << key << std::endl;
//		std::cout << "key2 = " << key2 << std::endl;
//		std::cout << "key size = " << key.size() << std::endl;
		double loss = stod(line.substr(pos + 1, line.size()));
//		std::cout << "Topology::LoadTableFromFile key = " << key << " loss " << loss << std::endl;
		m_obstructedDistanceMapFromFile.insert(TStrDblPair(key, loss));
		m_obstructedDistanceMapFromFile.insert(TStrDblPair(key2, loss));
	}




}

Topology *
Topology::GetTopology (uint32_t createFile, uint32_t useFile, std::string mapBasePath,
		 uint32_t droneTest, uint32_t highBuildings)
{
  Topology **ptopo = PeekTopology ();
  /* Please, don't include any calls to logging macros in this function
   * or pay the price, that is, stack explosions.
   */
  if (*ptopo == 0)
    {
      // create the topology
//	  std::cout << "new topo" << std::endl;
      *ptopo = new Topology(createFile, useFile, mapBasePath, droneTest, highBuildings);
    }

  return *ptopo;
}

void
Topology::
CreateVertex(Obstacle &obstacle, std::string vertex)
{
  NS_LOG_FUNCTION (this);

  // x, y valus are comma-separated
  size_t pos = vertex.find(",");
  std::string x = vertex.substr(0, pos);
  std::string y = vertex.substr(pos + 1);

  // convert values to double
  double dx = atof(x.c_str ());
  double dy = atof(y.c_str ());

  // create a 2D x,y point
  Point p(dx, dy);
  // add the point as a vertex to an obstacle
  obstacle.AddVertex(p);

  // if possible, update topology bounding box values
  if (dx < m_minX)
    {
      m_minX = dx;
    }
  if (dx > m_maxX)
    {
      m_maxX = dx;
    }
  if (dy < m_minY)
    {
      m_minY = dy;
    }
  if (dy > m_maxY)
    {
      m_maxY = dy;
    }
}

void
Topology::
CreateShape(std::string id, std::string vertices, std::string height)
{
  // create an obstacle
  Obstacle obstacle;
  // name the obstacle
  obstacle.SetId(id);

	// If height is defined, set it
	if (!height.empty())
		{
			double h = std::abs(atof(height.c_str ()));
			obstacle.SetHeight (h);
		}

  // tokenize each vertex
  size_t pos1 = 0;
  size_t pos2 = vertices.find(" ", pos1);
  std::string vertex;
  while (pos2 != std::string::npos)
    {
      vertex = vertices.substr(pos1, pos2-pos1);

      CreateVertex(obstacle, vertex);
      pos1 = pos2 + 1;
      pos2 = vertices.find(" ", pos1);
    }
  // NOTE:  In OSM data, last vertex is dup of first
  // so, we don't need to get it for polygonal obstacle
  // get last vertex

  // calculate the obstacle center of
  // bounding box and radius(squared).
  obstacle.Locate();

  // load centerpoint into Range Tree
  Point c = obstacle.GetCenter();
  Key k = Key(c, obstacle);

  // add the obstacle to the topolgoy
  m_obstacles.push_back(k);
}

// range tree (binary space partition, BSP)
// for quickly searching for obstacles within a range
static Range_tree_2_type m_rangeTree;

void
Topology::LoadBuildings(std::string bldgFilename, uint32_t createFile, uint32_t useFile, std::string mapBasePath,
		uint32_t droneTest, uint32_t highBuildings)
{
  NS_LOG_INFO ("Load buildings.");
//  std::cout << "Topology::loadbuildings usefile = " << useFile << std::endl;
	uint32_t nBuildings = 0;

  std::ifstream file (bldgFilename.c_str (), std::ios::in);
  if (!(file.is_open ()))
    {
      NS_FATAL_ERROR("Could not open buildings file " << bldgFilename.c_str() << " for reading, aborting here \n");
    }
  else
    {
//	  m_obstacles.clear();
      Topology * topology = Topology::GetTopology(createFile, useFile, mapBasePath, droneTest, highBuildings);
      NS_ASSERT(topology != 0);
      int lineCount = 0;
      topology->m_obstacles.clear();
      NS_LOG_DEBUG ("Reading file: " << bldgFilename);
      while (!file.eof () )
        {
          std::string line;

          getline (file, line);
//          std::cout << line << std::endl;
          lineCount++;
          NS_LOG_DEBUG (line);

          size_t posB = line.find("type=\"building");
          size_t posU = line.find("type=\"unknown");
          if ((posB != std::string::npos) || (posU != std::string::npos))
            {
              // could *possibly* use XML-DOM here, but
              // seems faster to just read through the file

              // found a building
              // get the building id (name) and shape (vertices)

//        	  std::cout << line << std::endl;
              std::string polyid;
              std::string shape;
							std::string height;
              size_t posP = line.find("<poly");
							size_t pos = line.find("id=\"");
							if ((posP != std::string::npos) || (pos != std::string::npos))
                {
                  size_t pos2 = line.find("\"", pos + 4);
                  if (pos2 != std::string::npos)
                    {
                      polyid = line.substr(pos + 4, pos2 - (pos + 4));
//                      std::cout << polyid << std::endl;
                      pos = line.find("shape=\"");
                      if (pos != std::string::npos)
                      {
                        size_t pos2 = line.find("\"", pos + 8);
                        if (pos2 != std::string::npos)
                        {
                          shape = line.substr(pos + 7, pos2 - pos - 7);

													// Search for the height value
													pos = line.find("height=\"");
		                      if (pos != std::string::npos)
														{
															size_t pos2 = line.find("\"", pos + 8);
			                        if (pos2 != std::string::npos)
			                        {
			                          height = line.substr(pos + 8, pos2 - (pos + 8));
															}
														}

													topology->CreateShape(polyid, shape, height);
													nBuildings++;
                        }
                      }
                    }
                }
            }
        }

      if (createFile) {
    	  std::string extension;
    	  if (droneTest) {
    		  extension = ".3Dlosses";
    	  }
    	  else {
    		  extension = ".losses";
    	  }
    	  std::string fileName;
    	  if (highBuildings) {
    		  fileName = mapBasePath + "-100" + extension;
    	  }
    	  else {
    		  fileName = mapBasePath + extension;
    	  }
    	  remove(fileName.c_str());
      }
      if (useFile) {
    	  topology->LoadTableFromFile();
      }
//	  std::cout << "linee lette= " << lineCount << std::endl;
	  NS_LOG_INFO ("Number of buildings found: " << nBuildings << ".");
      NS_LOG_INFO ("Topology buildings bounded by x:" << topology->GetMinX() << "," << topology->GetMaxX() << " y:" << topology->GetMinY() << "," << topology->GetMaxY() << ".");
      // all obstacles have been loaded
      // so now create a searchable range tree based on those obstacles
      topology->MakeRangeTree();
    }
}

void
Topology::MakeRangeTree()
{
  NS_LOG_FUNCTION (this);
//  std::cout << "makeRangeTree obstacles= " << m_obstacles.size() << std::endl;
  m_rangeTree.make_tree(m_obstacles.begin(), m_obstacles.end());
}

void
Topology::GetObstructedDistance(const Point_3 &p1, const Point_3 &p2, Obstacle &obs, double & obstructedDistance, int &intersections)
{
  NS_LOG_FUNCTION (this);

  // initialize values as if no intersections found
  obstructedDistance = 0.0;
  intersections = 0;

  // construct a segment, r, from p1 to p2
  // test r for intersection with each edge of obs
  // calculate distance from p1 to nearest (d_min)  and farthest (d_max) intersecting edge
  // obstruction distance is then estimated by d_max - d_min

  double d_min = 999999999.0;
  double d_max = -999999999.0;

  Segment_3 r(p1, p2);

  Polygon_2 &poly = obs.GetPolygon();

  for (EdgeIterator iterEdge = poly.edges_begin(); iterEdge != poly.edges_end(); ++iterEdge)
    {
      Segment_2 s = (Segment_2) (*iterEdge);

			// Create a plane from the edge:
			// first, extract vertices from the segment,
			// then create a plane with the two vertices and a third
			// point made of x and y coordinates of one vertex and the z value
			// as the height of the obstacle
			Point_3 a(s.vertex(0).x (), s.vertex(0).y (), 0.0);
			Point_3 b(s.vertex(1).x (), s.vertex(1).y (), 0.0);
			Point_3 c(b.x (), b.y (), (obs.GetHeight () != 0) ? obs.GetHeight () : 1);	// if height is not provided, take 1 meter
			Plane_3 plane(a, b, c);

			CGAL::Object result = CGAL::intersection(plane, r);

      if (const CGAL::Point_3<K> *ipoint = CGAL::object_cast<CGAL::Point_3<K> >(&result))
        {
					// Check if the intersection is caused only by the generated plane,
					// and not for the obstacle
					Point_3 ip(CGAL::to_double(ipoint->x()), CGAL::to_double(ipoint->y()), CGAL::to_double(ipoint->z()));
					if (!IsInRegion(s.vertex(0), s.vertex(1), obs.GetHeight (), ip))
						continue;

          intersections++;

          // handle the point intersection case with *ipoint.
          double dx = CGAL::to_double(ipoint->x()) - CGAL::to_double(p1.x());
          double dy = CGAL::to_double(ipoint->y()) - CGAL::to_double(p1.y());
					double dz = CGAL::to_double(ipoint->z()) - CGAL::to_double(p1.z());
          // square it, for performance optimization
          // (there will be many compares to this vaule)
          double distP1toIPsq = dx * dx + dy * dy + dz * dz;

          if (distP1toIPsq < d_min)
            {
              d_min = distP1toIPsq;
            }
          if (distP1toIPsq > d_max)
            {
              d_max = distP1toIPsq;
            }
        }
      /*
      else if (const CGAL::Segment_2<K> *iseg = CGAL::object_cast<
		CGAL::Segment_2<K> >(&result))
        {
          // handle the segment intersection case with *iseg.
          //cout << "s and rho intersect at a segment " << *iseg << endl;
        }
      else
        {
          // handle the no intersection case.
          //NS_LOG_UNCOND ("s and rho do not intersect");
          //cout << "s and rho do not intersect." << endl;
        }
      */

    }

	// Handle the case in which one intersection is with the top of the obstacle (roof of the building)
	double p1z = CGAL::to_double(p1.z());
	double p2z = CGAL::to_double(p2.z());
	double highestz = (p1z > p2z) ? CGAL::to_double(p1.z()) : CGAL::to_double(p2.z());

	if ((obs.GetHeight () != 0) && (highestz >= obs.GetHeight ()))
	{
		// Simulate the top of the obstacle with a plane in the 3d space
		Point_3 a(poly.vertex (0).x (), poly.vertex (0).y (), obs.GetHeight ());
		Point_3 b(poly.vertex (1).x (), poly.vertex (1).y (), obs.GetHeight ());
		Point_3 c(poly.vertex (2).x (), poly.vertex (2).y (), (obs.GetHeight () > 0) ? obs.GetHeight () : 1);
		Plane_3 roof(a, b, c);

		CGAL::Object result = CGAL::intersection(roof, r);

		// Check intersection
		if (const CGAL::Point_3<K> *ipoint = CGAL::object_cast<CGAL::Point_3<K> >(&result))
		{
			// Check bounds
			if (PointIsInPolygon (poly, ipoint))
			{
				intersections++;

				double dx = CGAL::to_double(ipoint->x()) - CGAL::to_double(p1.x());
				double dy = CGAL::to_double(ipoint->y()) - CGAL::to_double(p1.y());
				double dz = CGAL::to_double(ipoint->z()) - CGAL::to_double(p1.z());
				double distP1toIPsq = dx * dx + dy * dy + dz * dz;

				if (distP1toIPsq < d_min)
					{
						d_min = distP1toIPsq;
					}
				if (distP1toIPsq > d_max)
					{
						d_max = distP1toIPsq;
					}
			}
		}
	}

  if ((d_min < 999999999.0) && (d_max > 0))
    {
      // if d_min == d_max, then intersection was at a vertex of an edge
      if (d_min == d_max)
        {
          // intersection was at a vertex
        }
      else
        {
          double d1 = sqrt(d_min);
          double d2 = sqrt(d_max);
          obstructedDistance = d2 - d1;
        }
    }
}

double
Topology::GetObstructedLossBetween(const Point_3 &p1, const Point_3 &p2, double r)
{
  NS_LOG_FUNCTION (this);
  // initially assume no loss
  double obstructedLoss = 0.0;

  double rSq = r * r;

  double p1x = CGAL::to_double(p1.x());
  double p1y = CGAL::to_double(p1.y());
	double p1z = CGAL::to_double(p1.z());
  double p2x = CGAL::to_double(p2.x());
  double p2y = CGAL::to_double(p2.y());
	double p2z = CGAL::to_double(p2.z());

  // test first to see if we have a cached value
  // for loss between these two points
  // using their positions to the nearest 0.1m

  char buff[100];
  char buff2[100];
  std::string key2;
  sprintf(buff, "%010.1f %010.1f %010.1f", p1x, p1y, p1z);
  std::string p1Pos = buff;
  sprintf(buff, "%010.1f %010.1f %010.1f", p2x, p2y, p2z);
  std::string p2Pos = buff;
  sprintf(buff, "%s %s", p1Pos.c_str(), p2Pos.c_str());
  std::string key = buff;
//  std::cout << "topo mapFromFile size= " << m_obstructedDistanceMapFromFile.size() << std::endl;

  // check if cached
  if (m_obstructedDistanceMap.count(key) > 0 or m_obstructedDistanceMapFromFile.count(key) > 0)
    {
      // found it
      TStrDblMap::iterator it;
      it = m_obstructedDistanceMap.find(key);
      if (it == m_obstructedDistanceMap.end()) {
//    	  std::cout << "topo found in file " << std::endl;
          it = m_obstructedDistanceMapFromFile.find(key);
      }
      obstructedLoss = it->second;
      return obstructedLoss;
    }
  else
    {
      // B to A is same as A to B
      sprintf(buff2, "%s %s", p2Pos.c_str(), p1Pos.c_str());
      key2 = buff2;
      if (m_obstructedDistanceMap.count(key) > 0 or m_obstructedDistanceMapFromFile.count(key) > 0)
      {
        // found it
        TStrDblMap::iterator it;
        it = m_obstructedDistanceMap.find(key);
        if (it == m_obstructedDistanceMap.end()) {
//        	std::cout << "topo found in file " << std::endl;
            it = m_obstructedDistanceMapFromFile.find(key);
        }
        obstructedLoss = it->second;
        return obstructedLoss;
        }
    }
//  std::cout << "not found in file key= " << key << std::endl;

  // optimization
  // only if dist between p1 and p2 < 2r
  double dx = p2x - p1x;
  double dy = p2y - p1y;
  double distP1toP2sq = dx * dx + dy * dy;
  // distance must be less then (2r)^2 = 4r^2
  double x4rSq = 4.0 * rSq;
//  std::cout << "distP1toP2sq= " << distP1toP2sq << " x4rSq= " << x4rSq << std::endl;
  if (distP1toP2sq < x4rSq)
    {
      // now search by range tree search
      // get bounding box, and extend by r is all directions
      double xmin = std::min(p1x, p2x) - r;
      double xmax = std::max(p1x, p2x) + r;
      double ymin = std::min(p1y, p2y) - r;
      double ymax = std::max(p1y, p2y) + r;
      Point pLow(xmin, ymin);
      Point pHigh(xmax, ymax);
      Interval win(Interval(pLow, pHigh));
      int count = 0;
      m_outputList.clear();
      m_rangeTree.window_query(win, std::back_inserter(m_outputList));
      std::vector<Key>::iterator current = m_outputList.begin();
			uint32_t index = 0;	// another check
			uint32_t limit = m_outputList.size ();

//	  std::cout << "number of buildings found = " << limit << std::endl;
      while (current != m_outputList.end() && index < limit)
			// don't know why, but without the second condition it goes SIGSEGV
			// because <current> goes off limits
        {
          Obstacle obstacle = (*current).second;
          std::string id = obstacle.GetId();

          Point center = obstacle.GetCenter();
//          std::cout << "obstacle id = " << id << " center= " << center << std::endl;
          double dx1 = CGAL::to_double(center.x()) - p1x;
          double dy1 = CGAL::to_double(center.y()) - p1y;
          double distCtoP1sq = dx1 * dx1 + dy1 * dy1;

          double dx2 = CGAL::to_double(center.x()) - p2x;
          double dy2 = CGAL::to_double(center.y()) - p2y;
          double distCtoP2sq = dx2 * dx2 + dy2 * dy2;
//          std::cout << "distCtoP1sq " << distCtoP1sq << " distCtoP2sq " << distCtoP2sq << std::endl;
//          std::cout << "rsq " << rSq << std::endl;
          if (((distCtoP1sq - rSq) < 0)
              && ((distCtoP2sq - rSq) < 0))
            {
              // obtstacle is within range
//			  std::cout << "obstacle is within range" << std::endl;
              double obstructedDistanceBetween = 0.0;
              int intersections = 0;

							// if both points are over the top of the building, no loss
							double minz = std::min(p1z, p2z);
							if ((obstacle.GetHeight () > 0) && (minz >= obstacle.GetHeight ()))
								{
									noop;	// pass, do nothing
								}
							else
								{
									GetObstructedDistance(p1, p2, obstacle, obstructedDistanceBetween, intersections);
								}
              // From C. Sommer et. al.:
              // A Computationally Inexpensive Empirical Model of IEEE 802.11p
              // Radio Shadowing in Urban Environments, 2011.
              // Additional loss due to propagation through obstacles:
              // Lobs = beta x n + gamma x d_m
              // Where
              // Lobs is the additional loss
              // beta is a (constant) factor for the obstacle type
              // n is the number of intersections through the obstacle
              // gamma is a (constant) factor for the obstacle type
              // d_m is the distance in meters of propagation through the obstacle
              if ((obstructedDistanceBetween > 0.0) && (intersections > 1))
                {
//            	  std::cout << "obstacle id = " << id << " intersections= " << intersections <<
//            			  " distance= " << obstructedDistanceBetween << std::endl;
                  double beta = obstacle.GetBeta();
                  double gamma = obstacle.GetGamma();
                  obstructedLoss = beta * (double) intersections + gamma * obstructedDistanceBetween;
                  count++;
                }
            }
          current++;
					index++;
        }
//      std::cout << "trovati " << count << " buildings nel LOS" << std::endl;
    }

  // cache results
  if (m_obstructedDistanceMap.size() > 1000)
    {
      // clear it every once in a while, to avoid bloat.
      m_obstructedDistanceMap.clear();
    }
  m_obstructedDistanceMap.insert(TStrDblPair(key, obstructedLoss));

  if (m_createFile) {

	  if (m_obstructedDistanceMapFromFile.count(key) == 0 && m_obstructedDistanceMapFromFile.count(key2) == 0) {
		  m_obstructedDistanceMapFromFile.insert(TStrDblPair(key, obstructedLoss));
		  std::string extension;
		  std::string fileName;
		  if (m_droneTest) {
			  extension = ".3Dlosses";
		  }
		  else {
			  extension = ".losses";
		  }

		  if (m_highBuildings) {
			  fileName = m_mapBasePath + "-100" + extension;
		  }
		  else {
			  fileName = m_mapBasePath + extension;
		  }
//		  std::cout << "filename=" << fileName << std::endl;
		  std::ofstream file (fileName.c_str (), std::ios::app);
		  file << key <<";" << obstructedLoss << std::endl;
//		  std::cout << "scrivo" << key <<";" << obstructedLoss << std::endl;
	  }
  }

  return obstructedLoss;
}

double
Topology::GetMinX()
{
  NS_LOG_FUNCTION (this);

  return m_minX;
}

double
Topology::GetMaxX()
{
  NS_LOG_FUNCTION (this);

  return m_maxX;
}

double
Topology::GetMinY()
{
  NS_LOG_FUNCTION (this);

  return m_minY;
}

double
Topology::GetMaxY()
{
  NS_LOG_FUNCTION (this);

  return m_maxY;
}

bool
Topology::HasObstacles()
{
  NS_LOG_FUNCTION (this);

  bool HasObstacles = false;

  if (m_obstacles.size() > 0)
    {
      HasObstacles = true;
    }

  return HasObstacles;
}

bool
Topology::PointIsInPolygon(const Polygon_2 &polygon, const Point_3 *ipoint)
{
	NS_LOG_FUNCTION (this);

	// Create the 2d point from a 3d point
	Point ip(CGAL::to_double(ipoint->x ()), CGAL::to_double(ipoint->y ()));

	CGAL::Bounded_side bside = polygon.bounded_side (ip);

	if (bside == CGAL::ON_UNBOUNDED_SIDE)
	{
		// The point is outside
		return false;
	}
	else
	{
		// otherwise the point is inside or on the border
		return true;
	}
}

bool
Topology::IsInRegion(const Point &s1, const Point &s2, double h, const Point_3 &ip)
{
	NS_LOG_FUNCTION (this);

	double xMin = std::min (CGAL::to_double(s1.x ()), CGAL::to_double(s2.x ()));
	double yMin = std::min (CGAL::to_double(s1.y ()), CGAL::to_double(s2.y ()));
	double zMin = 0;

	double xMax = std::max (CGAL::to_double(s1.x ()), CGAL::to_double(s2.x ()));
	double yMax = std::max (CGAL::to_double(s1.y ()), CGAL::to_double(s2.y ()));
	double zMax = zMin + h;

	double ipx = CGAL::to_double(ip.x ());
	double ipy = CGAL::to_double(ip.y ());
	double ipz = CGAL::to_double(ip.z ());

	if (ipx < xMin)
		return false;
	if (ipx > xMax)
		return false;

	if (ipy < yMin)
		return false;
	if (ipy > yMax)
		return false;

	if (ipz < zMin)
		return false;
	if (ipz > zMax)
		return false;

	return true;
}
