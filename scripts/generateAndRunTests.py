#!/usr/bin/python

# Invocation: ./generateAndRunTests.py ../mappe/testMap/osm.osm.xml 25

import sys, os
import string

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



def runScenario():
	# Protocols and transmission ranges
	buildings = ["0", "1"]
	protocols = ["1", "2", "3"]
	txRanges = ["100", "300", "500"]
	finalLetters = ["a", "b", "c", "d"]
	

	# Input parameters
	mapPath = sys.argv[1]
	vehicleDistance = sys.argv[2]

	# Calculates directories
	absMapPath = os.path.abspath(mapPath)
	absMapParentPath = os.path.dirname(absMapPath)
	mapBaseName = os.path.basename(mapPath).split(".")[0] + "-" + vehicleDistance
	mapPathWithoutExtension = os.path.join(os.path.dirname(mapPath), mapBaseName)

	thisScriptPath = os.path.realpath(__file__)
	thisScriptParentPath = os.path.dirname(thisScriptPath)

	# Defines paths of files necessary for ns3
	mobilityFilePath = absMapParentPath + "/" + mapBaseName + ".ns2mobility.xml"
	polygonFilePath = absMapParentPath + "/" + mapBaseName + ".poly.xml"
	polygonFilePath3D = absMapParentPath + "/" + mapBaseName + ".3D.poly.xml"

	

	
	# Runs generate sumo files
	sumoFileGenerator = thisScriptParentPath + "/generate-sumo-files.sh " + " ".join(sys.argv[1:])
	os.system(sumoFileGenerator)

	# Creates jobs and runs them on cluster

	# Pass this when running tests
	numNodes = findNumNodes(mobilityFilePath)

	for b in buildings:
		for txRange in txRanges:
			for protocol in protocols:
				for finalLetter in range(0, 26): 
					command = "NS_GLOBAL_VALUE=\"RngRun=2\" ./waf --run 'vanet-urban {0} --buildings={1} --actualRange={2} --protocol={3} --flooding=0 --area=1000 --nnodes={4} --mapBasePath={5}'".format(string.ascii_lowercase[finalLetter], b, txRange, protocol, numNodes, mapPathWithoutExtension)
					print(command)

	nsPath = os.path.join(os.path.dirname(thisScriptParentPath), "ns-3.26")
	os.chdir(nsPath)
	print(os.getcwd())
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
	runScenario()

if __name__ == "__main__":
	main()