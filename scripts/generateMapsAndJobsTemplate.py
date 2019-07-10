#!/usr/bin/python

# Invocation: 
# 	./generateMapsAndJobsTemplate.py ../maps/testMap/osm.osm.xml 25 
# if you want to specify map and distance by hand, othwerise:
# 	./generateMapsAndJobsTemplate.py 
# if you want to automatically generate jobs for the scenarios and distances set in the main

import sys, os
import string
import shutil

def findNumNodes(mobilityFilePath):
	print(mobilityFilePath)
	file = open(mobilityFilePath)
	maxId = -sys.maxint - 1
	for line in file:
		splitLine = line.split(" ")
		if (len(splitLine) == 4):
			id = int(splitLine[0].split("_")[1].replace("(", "").replace(")", ""))
			if (id > maxId):
				maxId = id
	return maxId + 1

def createJobFile(newJobName, command, jobsPath, jobTemplatePath, tempNewJobPath):
	newJobFilename = newJobName + "-.job"
	newJobPath = os.path.join(jobsPath, newJobFilename)
	#print(command)
	#print(fileName)
	shutil.copy(jobTemplatePath, jobsPath)
	os.rename(tempNewJobPath, newJobPath)
	s = open(newJobPath).read()
	s = s.replace("{**jobName}", newJobName)
	s = s.replace("{**command}", command)
	f = open(newJobPath, "w")
	f.write(s)
	f.close()

def runScenario(cw, scenario, distance, startingNode, vehiclesNumber, area=1000):
	print(scenario)
	# Protocols and transmission ranges
	highBuildings = ["0"]
	drones = ["0"]
	buildings = ["0"]
	#buildings = ["0"]
	#errorRates = ["0", "10", "20", "30", "40", "50"]
	errorRates = ["0"]
	#forgedCoordRates = ["0", "10", "20", "30", "40", "50"]
	forgedCoordRates = ["0"]
	#buildings = ["1"]
	junctions = ["0"]
	protocols = ["1", "2", "3", "4"]
	#protocols = ["1", "2", "3", "4", "5"]
	#txRanges = ["100"]
	txRanges = ["100", "300", "500"]
	protocolsMap = {
		"1": "Fast-Broadcast",
		"2": "STATIC-100",
		"3": "STATIC-300",
		"4": "STATIC-500",
		"5": "ROFF"
	}
	cwMin = cw["cwMin"]
	cwMax = cw["cwMax"]

	# Some necessary paths
	thisScriptPath = os.path.realpath(__file__)
	thisScriptParentPath = os.path.dirname(thisScriptPath)
	nsPath = os.path.join(os.path.dirname(thisScriptParentPath), "ns-3.26")
	jobsPath = os.path.join(os.path.dirname(thisScriptParentPath), "jobsTemplate")
	tempNewJobPath = os.path.join(jobsPath, "jobTemplate.job")
	jobTemplatePath = os.path.join(thisScriptParentPath ,"jobTemplate.job")
	#mapsPath = os.path.join(os.path.dirname(thisScriptParentPath), "maps")

	# Input parameters
	if (scenario is None or distance is None): 
		mapPath = sys.argv[1]
		vehicleDistance = sys.argv[2]
	else:
		mapPath = "../maps/" + scenario + "/" + scenario + ".osm.xml"
		vehicleDistance = distance
	
	# Calculates directories
	absMapPath = os.path.abspath(mapPath)
	absMapParentPath = os.path.dirname(absMapPath)
	mapBaseName = os.path.basename(mapPath).split(".")[0]
	mapBaseNameWithDistance = mapBaseName
	mapPathWithoutExtension = os.path.join(os.path.dirname(mapPath), mapBaseNameWithDistance)
	print(mapBaseNameWithDistance)
	print(mapPathWithoutExtension)

	

	# Defines paths of files necessary for ns3
	mobilityFilePath = absMapParentPath + "/" + mapBaseNameWithDistance + ".ns2mobility.xml"
	polygonFilePath = absMapParentPath + "/" + mapBaseNameWithDistance + ".poly.xml"
	polygonFilePath3D = absMapParentPath + "/" + mapBaseNameWithDistance + ".3D.poly.xml"
	
	# Runs generate sumo files
	sumoFileGenerator = thisScriptParentPath + "/generate-sumo-files.sh " + " " + mapPath + " " + vehicleDistance
	#Uncomment to generate sumoFiles again
	#os.system(sumoFileGenerator)

	# Creates jobs templates inside jobTemplates/
	for highBuilding in highBuildings:
		for drone in drones:
			for b in buildings:
				for txRange in txRanges:
					for protocol in protocols:
						for junction in junctions:
							for errorRate in errorRates:
								protocolName = protocolsMap[protocol]
								# skips job generation for error rates > 0 with STATIC protocols
								if (errorRate != "0" and "STATIC" in protocolName):
									continue
								propagationLoss = "1"
								if ("Cube" in scenario):
									propagationLoss = "0"
								if (protocol == "5"): #ROFF
									command = "NS_LOG=\"*=error\" NS_GLOBAL_VALUE=\"RngRun=1\" /home/jgottard/ns-3/ns-3.26/build/scratch/roff-test/roff-test --buildings={0} --actualRange={1} --mapBasePath={2} --vehicleDistance={3} --startingNode={4} --propagationLoss={5} --area={6} --smartJunctionMode={7} --errorRate={8} --nVehicles={9} --droneTest={10} --highBuildings={11} --printToFile=1 --printCoords=0  --createObstacleShadowingLossFile=0 --useObstacleShadowingLossFile=1  --beaconInterval=100 --distanceRange=1 --forgedCoordTest=0 --forgedCoordRate=0".format(b, txRange, mapPathWithoutExtension, distance, startingNode, propagationLoss, area, junction, errorRate, vehiclesNumber, drone, highBuilding)
								else: 
									command = "NS_LOG=\"*=error\" NS_GLOBAL_VALUE=\"RngRun=1\" /home/jgottard/ns-3/ns-3.26/build/scratch/fb-vanet-urban/fb-vanet-urban --buildings={0} --actualRange={1} --mapBasePath={2} --cwMin={3} --cwMax={4} --vehicleDistance={5} --startingNode={6} --propagationLoss={7} --protocol={8} --area={9} --smartJunctionMode={10} --errorRate={11} --nVehicles={12} --droneTest={13} --highBuildings={14} --flooding=0  --printToFile=1 --printCoords=0 --createObstacleShadowingLossFile=0 --useObstacleShadowingLossFile=1 --forgedCoordTest=0 --forgedCoordRate=0".format(b, txRange, mapPathWithoutExtension, cwMin, cwMax, distance, startingNode, propagationLoss, protocol, area, junction, errorRate, vehiclesNumber, drone, highBuilding)

								newJobName = "urban-" + mapBaseName + "-highBuildings" + str(highBuilding) + "-drones" + str(drone) + "-d" + str(vehicleDistance) + "-cw-" +str(cwMin) + "-" + str(cwMax) + "-b" + b + "-e" + errorRate + "-j" + junction + "-" + protocolsMap[protocol] + "-" + txRange
								createJobFile(newJobName, command, jobsPath, jobTemplatePath, tempNewJobPath)
							'''
							# FORGED COORD SCENARIO
							if (scenario == "LA-25" and distance == "25"):
								for forgedCoordRate in forgedCoordRates:
									propagationLoss = "1"
									if (protocol == "5"): #ROFF
										command = "NS_GLOBAL_VALUE=\"RngRun=1\" /home/jgottard/ns-3/ns-3.26/build/scratch/roff-test/roff-test --buildings={0} --actualRange={1} --mapBasePath={2} --vehicleDistance={3} --startingNode={4} --propagationLoss={5} --area={6} --smartJunctionMode={7} --errorRate=0 --printToFile=1 --printCoords=0  --createObstacleShadowingLossFile=0 --useObstacleShadowingLossFile=1  --beaconInterval=100 --distanceRange=1 --forgedCoordTest=1 --forgedCoordRate={8}".format(b, txRange, mapPathWithoutExtension, distance, startingNode, propagationLoss, area, junction, forgedCoordRate)
									else: 
										command = "NS_GLOBAL_VALUE=\"RngRun=1\" /home/jgottard/ns-3/ns-3.26/build/scratch/fb-vanet-urban/fb-vanet-urban --buildings={0} --actualRange={1} --mapBasePath={2} --cwMin={3} --cwMax={4} --vehicleDistance={5} --startingNode={6} --propagationLoss={7} --protocol={8} --area={9} --smartJunctionMode={10} --errorRate=0 --flooding=0  --printToFile=1 --printCoords=0 --createObstacleShadowingLossFile=0 --useObstacleShadowingLossFile=1 --forgedCoordTest=1 --forgedCoordRate={11}".format(b, txRange, mapPathWithoutExtension, cwMin, cwMax, distance, startingNode, propagationLoss, protocol, area, junction, forgedCoordRate)
									newJobName = "urban-" + mapBaseName + "-d" + str(vehicleDistance) + "-cw-" +str(cwMin) + "-" + str(cwMax) + "-b" + b + "-f" + forgedCoordRate + "-j" + junction + "-" + protocolsMap[protocol] + "-" + txRange
									createJobFile(newJobName, command, jobsPath, jobTemplatePath, tempNewJobPath)
							'''
						
					
	print("\n")

def main():
	#Edit these to launch automatically 	forgedCoordRates = ["0", "10", "20", "30", "40", "50", "100"]
	scenarios = ["LA-25"]
	#scenarios = ["LA-25"]
	#scenarios = ["Padova-5", "Padova-15", "Padova-25", "Padova-35", "Padova-45"] 
	#scenarios = ["Padova-15", "Padova-25", "Padova-35", "Padova-45", "LA-15", "LA-25", "LA-35", "LA-45"]
	#contentionWindows = [{"cwMin": 32, "cwMax": 1024}, {"cwMin": 16, "cwMax": 128}]
	contentionWindows = [{"cwMin": 32, "cwMax": 1024}]
	#contentionWindows = [{"cwMin": 16, "cwMax": 128}]
	#distances = ["15", "25", "35", "45"]
	#scenarios = ["Padova"]
	startingNodeMap = {
		"Padova-5": 1212,
		"Padova-15":1182,
		"Padova-25":310,
		"Padova-35":1273,
		"Padova-45":824,
		"LA-5":124,
		"LA-15":2355,
		"LA-25":1009,
		"LA-35":459,
		"LA-45":354,
		"Grid-200":2024,
		"Grid-300":4896,
		"Grid-400":1248,
		"Platoon": 0,
		"Platoon-15km": 0,
		#"Cube-75:"13965,
		"Cube-150":4209
	}
	vehiclesNumber = {
		"LA-5":2984,
		"LA-15":2396,
		"LA-25":1465,
		"LA-35":1083,
		"LA-45":861,
		"Padova-5":0,
		"Padova-15":0,
		"Padova-25":0,
		"Padova-35":0,
		"Padova-45":0,
		"Platoon-15km":0,
		"Grid-200":0,
		"Grid-300":0,
		"Grid-400":0
	}


	area = 1000

	# Removes all previous job templates in output directory
	thisScriptPath = os.path.realpath(__file__)
	thisScriptParentPath = os.path.dirname(thisScriptPath)
	jobsPath = os.path.join(os.path.dirname(thisScriptParentPath), "jobsTemplate")
	map( os.unlink, (os.path.join(jobsPath,f) for f in os.listdir(jobsPath)) )

	if (len(sys.argv) < 3):
		for cw in contentionWindows:
			for scenario in scenarios:
				if ("Platoon" in scenario):
					area = 14000
				if ("Grid" in scenario):
					area = 2000
				if ("Grid" in scenario or "Platoon" in scenario):
					runScenario(cw, scenario, "25", startingNodeMap[scenario], vehiclesNumber[scenario], area)
				else:
					distance = scenario.split("-")[1]
					runScenario(cw, scenario, distance, startingNodeMap[scenario], vehiclesNumber[scenario], area)
	else:
		runScenario(None, None)
if __name__ == "__main__":
	main()