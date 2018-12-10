#!/bin/bash

# Reading Parameters
mapPath=$1
vehicleDistance=$2

# Calculating directories
thisScriptPath="$(dirname "$(readlink -f $0)")"
thisScriptParentPath="$(dirname "$thisScriptPath")" 

mapBaseName=$(basename "$mapPath" .osm.xml)
mapParentPath="$(dirname "$mapPath")" 
mapParentDir=${mapParentPath##*/}
absoluteMapParentPath=$thisScriptParentPath/maps/$mapParentDir

generateSumoFilesPath=$thisScriptPath/generate-sumo-files.sh

# Defining paths of files necessary for ns3
mobilityFilePath=$mapParentPath/$mapBaseName.ns2mobility.xml
polygonFilePath=$mapParentPath/$mapBaseName.poly.xml


# Generating sumo files
$generateSumoFilesPath "$mapPath" "$vehicleDistance"



echo $mapBaseName
echo $mobilityFilePath
echo "abs"
echo $absoluteMapParentPath

# TODO
# specificare files necessari a ns3 (.ns2mobility, 3Dpoly, poly, )

# identificare veicolo che genera il segnale
# aggiungere parametri a ns3 in modo che prenda i file corretti (e calcoli correttamente il veicolo iniziale?)
# far sì che i gli .out vengano messi in cartelle corrette
# lanciare jobs in automatico
# grafici??
