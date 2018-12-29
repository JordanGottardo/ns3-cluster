#!/usr/bin/python

# Invocation: 
# 	./generateAndRunTests.py ../maps/testMap/osm.osm.xml 25 
# if you want to specify map and distance by hand, othwerise:
# 	./generateAndRunTests.py 
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

def runScenario(scenario, distance):
	# Protocols and transmission ranges
	buildings = ["0", "1"]
	protocols = ["1", "2", "3", "4"]
	txRanges = ["100", "300", "500"]
	finalLetters = string.ascii_lowercase
	protocolsMap = {
		"1": "fb",
		"2": "st100",
		"3": "st300",
		"4": "st500" 
	}

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
	mapBaseNameWithDistance = mapBaseName + "-" + vehicleDistance
	mapPathWithoutExtension = os.path.join(os.path.dirname(mapPath), mapBaseNameWithDistance)

	

	# Defines paths of files necessary for ns3
	mobilityFilePath = absMapParentPath + "/" + mapBaseNameWithDistance + ".ns2mobility.xml"
	polygonFilePath = absMapParentPath + "/" + mapBaseNameWithDistance + ".poly.xml"
	polygonFilePath3D = absMapParentPath + "/" + mapBaseNameWithDistance + ".3D.poly.xml"
	
	# Runs generate sumo files
	sumoFileGenerator = thisScriptParentPath + "/generate-sumo-files.sh " + " " + mapPath + " " + vehicleDistance
	os.system(sumoFileGenerator)

	# Creates jobs and runs them on cluster

	# Pass this when running tests
	numNodes = findNumNodes(mobilityFilePath)

	for b in buildings:
		for txRange in txRanges:
			for protocol in protocols:
				command = "NS_GLOBAL_VALUE=\"RngRun=2\" ./waf --run 'vanet-urban --buildings={0} --actualRange={1} --protocol={2} --flooding=0 --area=1000 --mapBasePath={3}'".format(b, txRange, protocol, mapPathWithoutExtension)
				newJobName = "urban-" + mapBaseName + "-d" + str(vehicleDistance) +  "-b" + b + "-" + protocolsMap[protocol] + "-" + txRange + "-"
				newJobFilename = newJobName + ".job"
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

	#job
	#PBS -N urban-la-b0-fb300
	#PBS -e localhost:${HOME}/ns-3/log/urban-la-b0-fb300.err
	#PBS -o localhost:${HOME}/ns-3/log/urban-la-b0-fb300.out
	#set > /home/jgottard/ns-3/log/urban-la-b0-fb300
	# command

	# TODO
	# lanciare jobs in automatico
	# grafici??


def main():
	#Edit these to launch automatically 
	#scenarios = ["Padova", "LA"]
	#distances = ["15", "25", "35", "45"]
	scenarios = ["Padova"]
	distances = ["25"]
	
	if (len(sys.argv) < 3):
		for scenario in scenarios:
			for distance in distances:
				runScenario(scenario, distance)
	else:
		runScenario(None, None)
if __name__ == "__main__":
	main()