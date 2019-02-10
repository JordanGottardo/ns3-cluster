#!/bin/bash

cd ../../ns-3.26
#rm -rf ./out/scenario-urbano-con-coord


for building in {0..1}
do
	echo $building
	# FB300
	for i in {0..2} 
	do
		NS_GLOBAL_VALUE="RngRun=1" ./waf --run "fb-vanet-urban --startingNode=310 --buildings=$building --actualRange=300 --protocol=1 --flooding=0 --area=1000 --mapBasePath=../maps/Padova/Padova-25 --cwMin=32 --cwMax=1024 --printToFile=1 --printCoords=1" &
	done

	# FB500
	for i in {0..2} 
	do
		NS_GLOBAL_VALUE="RngRun=1" ./waf --run "fb-vanet-urban --startingNode=310 --buildings=$building --actualRange=500 --protocol=1 --flooding=0 --area=1000 --mapBasePath=../maps/Padova/Padova-25 --cwMin=32 --cwMax=1024 --printToFile=1 --printCoords=1" &
	done

	# ST300-300
	for i in {0..2} 
	do
		NS_GLOBAL_VALUE="RngRun=1" ./waf --run "fb-vanet-urban --startingNode=310 --buildings=$building --actualRange=300 --protocol=3 --flooding=0 --area=1000 --mapBasePath=../maps/Padova/Padova-25 --cwMin=32 --cwMax=1024 --printToFile=1 --printCoords=1" &
	done

	# ST300-500
	for i in {0..2} 
	do
		NS_GLOBAL_VALUE="RngRun=1" ./waf --run "fb-vanet-urban --startingNode=310 --buildings=$building --actualRange=500 --protocol=3 --flooding=0 --area=1000 --mapBasePath=../maps/Padova/Padova-25 --cwMin=32 --cwMax=1024 --printToFile=1 --printCoords=1" &
	done

	# ST500-300
	for i in {0..2} 
	do
		NS_GLOBAL_VALUE="RngRun=1" ./waf --run "fb-vanet-urban --startingNode=310 --buildings=$building --actualRange=300 --protocol=3 --flooding=0 --area=1000 --mapBasePath=../maps/Padova/Padova-25 --cwMin=32 --cwMax=1024 --printToFile=1 --printCoords=1" &
	done

	# ST500-500
	for i in {0..2} 
	do
		NS_GLOBAL_VALUE="RngRun=1" ./waf --run "fb-vanet-urban --startingNode=310 --buildings=$building --actualRange=500 --protocol=4 --flooding=0 --area=1000 --mapBasePath=../maps/Padova/Padova-25 --cwMin=32 --cwMax=1024 --printToFile=1 --printCoords=1" &
	done
done

wait

cd ../scripts/drawCoords
./drawAll
