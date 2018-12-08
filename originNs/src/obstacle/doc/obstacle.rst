Obstacle Module Documentation
----------------------------

.. include:: replace.txt
.. highlight:: cpp

.. heading hierarchy:
   ------------- Chapter
   ************* Section (#.#)
   ============= Subsection (#.#.#)
   ############# Paragraph (no number)

The obstacle module for |ns3| implements an efficient model that
deterministically calculates the propagation loss of shadowing effects
caused by 2D obstacles within a topology.

Model Description
*****************

Obstacles, such as buildings, vehicles, trees, and hills and crests
interfere with radio wave propagation by contributing fading and
shadowing effects.  While modelers often supplement deterministic
free space radio wave attenuation models (e.g., Friis, Two-Ray Ground)
with stochastic models (e.g., lognormal, Rice, Rayleigh, Nakagami-m)
to account for fading and/or shadowing effects, the stochastic nature
of such models determine loss in a completely probabilistic manner
without considering underlying geometry. Results, therefore, could
deviate significantly from realistic behavior, negatively impacting
performance assessments.

The |ns3| Obstacle Shadowing Model provides an accurate, deterministic
obstacle shadowing model that follows the work of [sommer].  Obstacles
are modeled as 2D polygons using the Compuational Geometry Algorithms
Libraray (CGAL) [cgal] that have per-wall and per-interior-meter attenuation
loss properties.

Readily available buildings footprint data may be obtained from
Open Street Map (OSM) [osm] and loaded into a Topogoly that the
Obstacle Shadowing Model subsequently queries to test for potential
obstuctions and then calculates shadowing loss as necessary.

Testing results show the Obstalce Shadowing Model to be efficient
and on the time complexity order of the stochastic Nakagami-m
model in |ns3|.

The source code for the obstacle module lives in the directory
``src/obstacle``.

The current code represents an initial contribution to support
Obstacle Shadowing using 2D obstacles and has been tested in
vehicular ad hoc network (VANET) scenarios, following the work
of [sommer].  Progation loss in |ns3|, however, may be pertinent
to several other modules, such as LTE, Wimax, and LR-WPAN.  Thus,
we see potential future expansion of the Obstacle Shadowing Model
to support many more simulation scenarios.

Design
======

The |ns3| reference model, shown below, is extended in two areas:
propagation and core.

::


        +--------------------------------------------------------+
        |                         test                           |
        +--------------------------------------------------------+
        |                        helper                          |
        +--------------------------------------------------------+
        | protocols | applications | devices | PROPAGATION | ... |
        +--------------------------------------------------------+
        |          internet          |       mobility            |
        +--------------------------------------------------------+
        |                        network                         |
        +--------------------------------------------------------+
        |                         CORE                           |
        +--------------------------------------------------------+

Core
####

Obstacles have physical properaties that lie outside of the network
physical layer, i.e., PHY.  Thus, obstacles and a topology manager
may be considered CORE functionality, as these items can be useful
to any other |ns3| source module.

Propagation
###########

In our implementation, the CORE obstacle and topology management
features are used to implement the Obstacle Shadowing Model that
extends the PROPAGATION model by providing a propagation loss model
that calculates deterministically the path loss of the shadowing
effects of buildings and is suitable for propagation loss chaining.

Scope and Limitations
=====================

What can the model do?  What can it not do?  Please use this section to
describe the scope and limitations of the model.

1. Does the model support 3D obstalces?

Currently, the model supports only 2D obstacles as CGAL Polygon_2
instances, although the model could be easily extended to support
3D obstacles.  However, a sufficient data source for 3D obstacle
representation would have to be identified.

2. What data sources does the model support for buildings footprint
information?

The model current processes 2D obstacle data readily available from
Open Street Map (OSM} [osm].  However, additional data source could
be explored and the model could be extended to load buildings data
from other sources into the Topology manager.

3. What |ns3| wireless models support the Obstacle Shadowing Model?

The Obstacle Shadowing Model is implemented as an extension to
the PropagationLossModel, and therefore may potentially be used
using propagation loss chaining by any |ns3| simulation.  Initially,
the model has been tested in simualtions that use the WiFi wireless
model, particularly the VANET scenarios that use the wave extensions.
However, the design of the model does not prohibit it from being
used by other simulation scenarios, such as LTE and Wimax.

4. How does the model treat different buildings differently?

Each building (i.e., and obstacle) has its own per-wall and
per-interior-meter attenuation loss parameters.  Following [sommer],
default values are used that every building currently uses.
However, the model allows buildings to be differentiated, for
example by structure type or size.  In this way, "brick and mortar"
buildings can be modeled differently than, say, residential
houses and garages.  However, such differentiation of buildings
by type would be greatly benefited with data sources (or empirically
validated research) that give their path loss characteristics.

5. What dependencies does the the model introduce?

The model relies on the Computational Geometry Algorithm
Library (CGAL).  Currently, this is added by extension of
the wscript file to specific compile, link, and library
options.  However, it is noted that the CGAL installs its
libraries, for example, in different locations on different systems.
As such, a system-independent dependency on CGAL for |ns3| is
still needed.

6. What did you put the code in a new model, instead of
just adding it to propagation

Toe-MAY-toe.  Ta-MAH-toe.  First, the code is a new
contribution specific to obstacles, so it made sense to
collect it all in one place.  Second, the obstacle module
has a dependency on CGAL [cgal] that is not necessary if
one does not want to work with geometric concepts of the
obstacle model, so the module can be more easily included/
excluded based on need.  Third, the obstacle model
and/or the idea of a topology can be used for things aside
from propagation loss.  For example, in addition to buildings,
nodes themselves could be added to a topology, especially if
nodes are mobile ones, or vehicles.  Eventually, users may
need to do things like find areas, or volumes of spaces, and
those things should be collected into a common place - e.g.,
an obstacle model.

References
==========

.. [sommer] Sommer, C. , Eckhoff, D., German, R., and Dressler, F.
A computationally inexpensive empirical model of IEEE 802.11p radio
shadowing in urban environments.  In Wireless On-Demand Network
Systems and Services (WONS), 2011 Eighth International Conference on.
IEEE, 2011, 84-90.

.. [cgal] Computational Geometry Algorithms Library (CGAL).
https://www.cgal.org.

.. [osm] Open Street Maps (OSM). http://www.openstreetmap.org.

Usage
*****

Three class comprise the Obstacle Shadowing Model, briefly
described here and covered in more detail in the APIs section below.

Obstacle retains the geometric elements to represent an obstacle.

Topology uses Obstalce to load obstacle data points into a
managed collection.

ObstacleShadowingPropagationLossModel extends PropagationLossModel
and uses both the Obstacle and Topology classes to implement
the calculation propagation loss of obstacle shadowing effects.

Building New Module
===================

To build the module, first install CGAL [cgal] and modify the
obstacle/wscript file if necessary, especially LINKFLAGS, to
point to the correct location of the CGAL libraries.
Performing a ./waf as normal should then build the
Obstacle Shadowing Model and link it with CGAL.

Helpers
=======

No helpers are provided.

APIs
=======

Key APIs are discussed below in terms of the model's three
primary classes.

A. Obstacle

1. SetId, GetId
   Sets/Gets the id (i.e., name) of the obstacle

2. AddVertex
   Adds a vertex to the obstacle.  Used as the obstacle is being "built"

3. Locate, GetCenter
   Calculates, and gets, the location of the centerpoint
   of the bounding box of the polygon, used for
   optimization of searching for obstacles.  To be
   called after the last vertex is added to the
   obstacle (so that the centerpoint can then be located)

4. GetRadiusSq
   Gets the radius of the obstacles region
   (squared for performance optimizations)

5. GetPolygon
   Gets the polygonal region (as a CGAL Polygon_2) that defines the obstacle

6. GetBeta, GetGamma
   Gets the value of beta, the per-wall
   attenuation parameter or
   gamma, the per-meter
   attenuation parameter

B. Topology

1. GetTopology
   Gets the (singl, static) topology instance

2. LoadBuildings
   Load buildings into the topology given a
   filename that contains buildings data.
   The buildings data is in the format of data
   as obtained from Open Street Map (OSM) [osm]
   and converted using the Simulation for Urban
   Mobility (SUMO) utility, Polyconvert.  The
   resulting file is XML-like, and contains
   buildings footprint data.  It may also contain
   other stuff (depending on what data has been
   placed into OSM for the region) that may need
   to be manually removed.

3. GetMinX, GetMaxX, GetMinY, GetMaxY
   Gets the minimum/maximum X/Y value of buildings
   (i.e., bounding box) in the topology

4. GetObstructedLossBetween
   For two given points, examine the obstacles
   between them and calculate the obstacle shadowing loss
   based on total wall intersections and interior distances
   traversed.

5. HasObstacles
   Tests if the topology has any obstacles (loaded within it)
   and returns true if the topology has obstacles, false otherwise

6. MakeRangeTree
   Called after all obstacles have been loaded into the topology
   to make a range tree (binary space partition, BSP) of obstacles,
   for searching.

C. ObstacleShadowingPropagationLoss

1. GetLoss
   Return the loss in dBm for the propagation loss
   due to obstacle shadowing between
   the two given mobility models

2. DoCalcRxPower
   Inherited from PropagationLossModel

Attributes
==========

A. Obstacle

1. m_id
   Name of the obstacle

2. m_obstacle
   CGAL Polygon_2 obstacle that represent the obstacle,
   i.e., a 2D polygon (vertices, or edges)

3. m_center
   The centerpoint of the obstacles bounding box,
   used for search optimization

4. m_radiusSq
   The distance from center to the vertex of the bounding
   box, squared.  (Squaring is for performance optimization,
   as there are many compares - is distance to obstacle < r
   is similar to distance^2 < r^2, and avoids taking
   square root).

5. m_beta, m_gamma
   Value of per-wall attenuation (default = 9dB) and
   value of per-meter attenuation (default = 0.4 dBm per meter)

B. Topology

1. m_obstacles
   The collection of obstacles for the topology.

2. m_rangeTree
   Binary space partition (BSP), or range tree for
   searching for obstacles within the topology

3. m_min/max X/Y
   Bounding box of all obstacles in the topology

4. m_obstructedDistanceMap
   Cache of obstructed loss between two points.
   For performance optimization, assume that
   obstructed loss between two points is the
   same if the objects have not moved more than
   0.1m.

Output
======

The model does not generate any output or have logging, other
than standard logging calls (e.g., each method invocation).

Advanced Usage
==============

1. Include file
#include "ns3/topology.h"

2. To load buildings into a topology
Topology::LoadBuildings(bldgFile);

3. To add the Obstacle Shadowing Model in propagation loss chaining
wifiChannel.AddPropagationLoss ("ns3::ObstacleShadowingPropagationLossModel");

Examples
========

The model does not include any examples of ONLY using
the Obstacle Shaddowing Model by itself.

But...

src/wave/examples/vanet-routing-compare.cc optionally
uses the Obstacle Shadowing Model.

An additional scenario (i.e., --scenario=3) is added to
vanet-routing-compare that simulates 50 vehicles moving
throughout downtown Raleigh, NC USA for 199 seconds, with
optional support for using the Obstacle Shadowing
Model by loading the downtown Raleigh buildings footprint
data (i.e., --buildings=1).

The downtown Raleigh, NC USA buildings footprint data file
can be found in: src/wave/examples/Downtown_Raleigh.buildings.xml.

An ns-3 mobility file of 50 vehicles moving throughout
the downtown streets of Raleigh, for 199 seconds, is provided
in: src/wave/examples/Raleigh_Downtown50.ns2.

A new scenario, i.e., --scenario=3, is added to the
src/wave/examples/vanet-routing-compare.cc script.

Thus, the simulation scenario may be run and repeated
with different options to evaluate the effects of
obstacle shadowing and compare the results with other
stochastic fading models, or no fading.  For example:

A. To run scenario 3 with no Obstacle Shadowing Model effects
(and no stochastic fading effects, either):

./waf --run "src/wave/examples/vanet-routing-compare --scenario=3"

B. To run scenario 3 with Obstacle Shadowing Model effects
(and no stochastic fading effects, either):

./waf --run "src/wave/examples/vanet-routing-compare --scenario=3 --buildings=1"

Note that the --buildings=1 command line flag instructs the
script to load the buildings footprint data (for the scenario)
which outputs the results of loading the buildings data.

C. To run scenario 3 with stochastic Nakagami-m fading instead
of the Obstacle Shadowing Model effects:

./waf --run "src/wave/examples/vanet-routing-compare --scenario=3 --fading=1"

Note that this is similar to (A) with no buildings data but
includes fading (1=Nakagami-m) propagation loss.

Troubleshooting
===============

Creating the buildings footprint data requires some knowledge
of Open Street Map (OSM) [oms] (i.e., to get the data for a
region, but that will get roads, and buildings, and other items)
and Simulator for Urban Mobility (SUMO), to use SUMO's Polyconvert
utility to extract the buildings data.

Validation
**********

No test cases are included in the model.

The model was tested extensively using the
src/wave/examples/vanet-routing-compare.cc script
to execute 30 trials each of 50-750 vehicles moving
through highway, residential neighborhood, and urban
downtown (i.e., Raleigh, NC USA) scenarios for 2000s
of simulation time.  All nodes (i.e., vehicles) emitted
an IEEE 1609/WAVE message (i.e., DSRC Basic Safety
Message, BSM) 10 times per simulation second (i.e.,
2000 seconds x 10 messages for each of up to 750 vehicles
constantly moving).  To calculate potential obstacle
shadowing effects, the obstacle model calculates the
potential loss between every pair of nodes (within
range) at every (transmission) event.  Simulations were
run on the High Performance Computing (i.e., HPC)
services at North Carolina State University (NCSU)
with the longest simulation running for almost six
clock days (with no core |ns3| or obstacle model faults).

