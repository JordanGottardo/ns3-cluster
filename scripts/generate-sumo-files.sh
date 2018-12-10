#!/bin/bash

SIMULATION_END_TIME=50

OSM_FILE=$1
VEHICLE_DISTANCE=$2
echo $@
echo "OSM FILE = $OSM_FILE"
echo "VEHICLEDISTANCE=$VEHICLE_DISTANCE"
BASEFILENAME=$(basename "$OSM_FILE" .osm.xml)
BASEFILENAMEWITHDISTANCE=$BASEFILENAME-$VEHICLE_DISTANCE
BASEDIR="$(dirname "$1")"
BASENAME=$BASEDIR/$BASEFILENAME
BASENAMEWITHDISTANCE=$BASEDIR/$BASEFILENAMEWITHDISTANCE
echo $BASEFILENAMEWITHDISTANCE
echo $BASENAMEWITHDISTANCE
NETCONVERT_CC="$(which netconvert)"
POLYCONVERT_CC="$(which polyconvert)"
RANDOM_TRIPS_CC="$(which sumo-randomTrips)"
SUMO_CC="$(which sumo)"
TRACE_EXPORTER="$(which sumo-traceExporter)"

CUSTOM_SCRIPT_PATH=$(readlink -f $0)
CUSTOM_SCRIPT_DIR="$(dirname "$CUSTOM_SCRIPT_PATH")"
POSITIONER="$CUSTOM_SCRIPT_DIR/fixedPositions.py"
POLYGON_HEIGHT_ADDER="$CUSTOM_SCRIPT_DIR/polyconvertEnch.py"

echo "CUSTOM_SCRIPT_PATH=$CUSTOM_SCRIPT_PATH"
echo "CUSTOM_SCRIPT_DIR=$CUSTOM_SCRIPT_DIR"
echo "POSITIONER=$POSITIONER"
PRUNE_POLY=$CUSTOM_SCRIPT_DIR:prunePoly.py

# Check if software exitsts
I="0"
if [ "$NETCONVERT_CC" == "" ]; then
	echo "[!] Error: netconvert not found."
	I=$((I + 1))
fi
if [ "$POLYCONVERT_CC" == "" ]; then
	echo "[!] Error: polyconvert not found."
	I=$((I + 1))
fi
if [ "$RANDOM_TRIPS_CC" == "" ]; then
	echo "[!] Error: randomTrips not found."
	I=$((I + 1))
fi
if [ "$SUMO_CC" == "" ]; then
	echo "[!] Error: sumo not found."
	I=$((I + 1))
fi
if [ "$TRACE_EXPORTER" == "" ]; then
	echo "[!] Error: traceExporter not found."
	I=$((I + 1))
fi

if [ "$I" != "0" ]; then
	echo
	exit 2
fi


# Generate data
NET_FILE="$BASENAME.net.xml"
BUILDINGS_FILE="$BASENAMEWITHDISTANCE.poly.xml"
BUILDINGS_FILE_2="$BASENAME.poly.2.xml"
ROUTES_FILE="$BASENAMEWITHDISTANCE.trips.xml"
SUMO_CFG_FILE="$BASENAME.sumo.cfg"
TRACE_FILE="$BASENAME.trace.xml"
MOBILITY_FILE="$BASENAMEWITHDISTANCE.ns2mobility.xml"
BUILDINGS_FILE_3D="$BASENAMEWITHDISTANCE.3Dpoly.xml"

# Just filenames for sumo config file
BASE_NET_FILE="$BASEFILENAME.net.xml"
BASE_ROUTE_FILE="$BASEFILENAMEWITHDISTANCE.trips.xml"
BASE_BUILDINGS_FILE="$BASEFILENAMEWITHDISTANCE.poly.xml"
BASE_TRACE_FILE="$BASEFILENAME.trace.xml"

echo "OSM_FILE=$OSM_FILE"
echo "BASENAME=$BASENAME"
echo "OSM_FILE=$OSM_FILE"
echo "BASEDIR=$BASEDIR"
echo "NET_FILE=$NET_FILE"


# Generate config file
cat > "$SUMO_CFG_FILE" <<EOF
<configuration>
    <input>
        <net-file value="$BASE_NET_FILE"/>
        <route-files value="$BASE_ROUTE_FILE"/>
        <additional-files value="$BASE_BUILDINGS_FILE"/>
    </input>
		<output>
			<fcd-output value="$BASE_TRACE_FILE" />
		</output>
    <time>
        <begin value="0"/>
        <end value="$SIMULATION_END_TIME"/>
    </time>
</configuration>
EOF
echo 0
# Generate network
$NETCONVERT_CC --osm-files="$OSM_FILE" -o "$NET_FILE"
echo 1
# Generate buildings: creates .poly.xml file
echo $OSM_FILE
echo $NET_FILE
$POLYCONVERT_CC --osm-files="$OSM_FILE" --net-file="$NET_FILE" --shapefile.add-param=true --prune.in-net=true --prune.explicit="fountain" -o "$BUILDINGS_FILE"
echo 2
# Generate routes: creates .trips.xml
$POSITIONER -n "$NET_FILE" -d "$VEHICLE_DISTANCE" -o "$ROUTES_FILE"
echo 3
# Run sumo: creates .trace.xml
$SUMO_CC -c "$SUMO_CFG_FILE"
echo 4

sleep 2

echo 5
# Generate mobility: creates .ns2mobility.xml
$TRACE_EXPORTER -i "$TRACE_FILE" --ns2mobility-out "$MOBILITY_FILE"
echo 6
# Enhances polyigon adding 3D: creates .3Dpoly.xml
$POLYGON_HEIGHT_ADDER -i "$OSM_FILE" -p "$BUILDINGS_FILE" -o "$BUILDINGS_FILE_3D"

# Exit
echo
exit 0
