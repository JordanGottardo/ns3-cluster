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

def runScenario(cw, scenario, distance, startingNode, area=1000):
	print(scenario)
	# Protocols and transmission ranges
	buildings = ["0", "1"]
	errorRates = ["0", "10", "20", "30", "40", "50"]
	#buildings = ["1"]
	junctions = ["0", "1"]
	protocols = ["1", "2", "3", "4", "5"]
	txRanges = ["100", "300", "500"]
	#txRanges = ["300", "500"]
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

	for b in buildings:
		for txRange in txRanges:
			for protocol in protocols:
				for junction in junctions:
					for errorRate in errorRates:
						protocolName = protocolsMap[protocol]
						executablePath = None
						propagationLoss = "1"
						if (protocol == "5"): #ROFF
							command = "NS_GLOBAL_VALUE=\"RngRun=1\" /home/jgottard/ns-3/ns-3.26/build/scratch/roff-test/roff-test --buildings={0} --actualRange={1} --mapBasePath={2} --vehicleDistance={3} --startingNode={4} --propagationLoss={5} --area={6} --smartJunctionMode={7} --errorRate={8} --printToFile=1 --printCoords=0  --createObstacleShadowingLossFile=0 --useObstacleShadowingLossFile=1  --beaconInterval=100 --distanceRange=1".format(b, txRange, mapPathWithoutExtension, distance, startingNode, propagationLoss, area, junction, errorRate)
						else: 
							executablePath = "/home/jgottard/ns-3/ns-3.26/build/scratch/fb-vanet-urban/fb-vanet-urban"
							command = "NS_GLOBAL_VALUE=\"RngRun=1\" /home/jgottard/ns-3/ns-3.26/build/scratch/fb-vanet-urban/fb-vanet-urban --buildings={0} --actualRange={1} --mapBasePath={2} --cwMin={3} --cwMax={4} --vehicleDistance={5} --startingNode={6} --propagationLoss={7} --protocol={8} --area={9} --smartJunctionMode={10} --errorRate={11} --flooding=0  --printToFile=1 --printCoords=0 --createObstacleShadowingLossFile=0 --useObstacleShadowingLossFile=1".format(b, txRange, mapPathWithoutExtension, cwMin, cwMax, distance, startingNode, propagationLoss, protocol, area, junction, errorRate)
						newJobName = "urban-" + mapBaseName + "-d" + str(vehicleDistance) + "-cw-" +str(cwMin) + "-" + str(cwMax) + "-b" + b + "-e" + errorRate + "-j" + junction + "-" + protocolsMap[protocol] + "-" + txRange
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

					#print(command)

	#os.chdir(nsPath)
	#print(os.getcwd())
	#os.system(command)


	print("\n")

def main():
	#Edit these to launch automatically 
	#scenarios = ["Padova", "LA", "Grid-200", "Grid-300", "Grid-400"]
	scenarios = ["Padova"]
	contentionWindows = [{"cwMin": 32, "cwMax": 1024}, {"cwMin": 16, "cwMax": 128}]
	#distances = ["15", "25", "35", "45"]
	distances = ["25"]
	#scenarios = ["Padova"]
	startingNodeMap = {
		"Padova":310,
		"LA":-1,
		"Grid-200":2024,
		"Grid-300": -1,
		"Grid-400":-1,
		"Platoon": 0,
		"Platoon-15km": 0,
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
					runScenario(cw, scenario, "25", startingNodeMap[scenario], area)
				else:
					for distance in distances:
						scenarioName = scenario + "-" + str(distance)
						runScenario(cw, scenarioName, distance, startingNodeMap[scenario], area)
	else:
		runScenario(None, None)
if __name__ == "__main__":
	main()