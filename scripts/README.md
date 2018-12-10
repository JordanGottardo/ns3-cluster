# Scripts
Various bash and python scripts, mainly for OSM data manipulation using SUMO utilities and libs.

* _fixedPositions.py_: use it to generate a trace file where vehicle are placed at a fixed distance
* _generate-sumo-files.sh_: generate a polygon file and a ns2mobility file from a OSM data file.
* _polyconvertEnch.py_: generate a polygon data file with heights

## Usage
### _fixedPositions.py_
This script takes as input a netfile and (optionally) a distance and produces a trace file.
Example:
```
python fixedPositions.py -n map.net.xml -d 25 -o map.trace.xml
```

### _generate-sumo-files.sh_
Given a OSM data file produces two files needed to run simulations with ns-3.
```
bash generate-sumo-files.sh map.osm.xml
```

### _polyconvertEnch.py_
Enhance a polygon data file with heights, given a osm data file and a poly file
```
python polyconvertEnch.py -i map.osm.xml -p map.poly.xml -o map.3Dpoly.xml
```

### _sensors-fixedPositions.py_
Generate positions for devices like sensors at each intersection at a give height.
```
pythonsensors-fixedPositions.py -n map.net.xml -o map.ns2mobility.xml -z 10
```

__N.B.__:
Some of the above python scripts use sumolib, so make sure it is installed before run them.
