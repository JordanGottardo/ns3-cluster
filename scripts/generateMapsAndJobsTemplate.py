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

def runScenario(cw, scenario, distance):
	# Protocols and transmission ranges
	#buildings = ["0", "1"]
	buildings = ["1"]
	protocols = ["1", "2", "3", "4"]
	#protocols = ["2", "3", "4"]
	#txRanges = ["100", "300", "500"]
	txRanges = ["300", "500"]
	protocolsMap = {
		"1": "fb",
		"2": "st100",
		"3": "st300",
		"4": "st500" 
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
	mapBaseNameWithDistance = mapBaseName + "-" + vehicleDistance
	mapPathWithoutExtension = os.path.join(os.path.dirname(mapPath), mapBaseNameWithDistance)

	

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
				protocolName = protocolsMap[protocol]
				#Removes creation of jobs where static protocol and txRange are different (e.g. STATIC100 with 500 tx range)
				#if (len(protocolName) > 3 and protocolName[-3:] != txRange):
				#	print(protocolName)
				#	print(protocolName[-3:])
				#	print(txRange)
				#	continue
				command = "NS_GLOBAL_VALUE=\"RngRun=1\" /home/jgottard/ns-3/ns-3.26/build/scratch/fb-vanet-urban/fb-vanet-urban --buildings={0} --actualRange={1} --protocol={2} --flooding=0 --area=1000 --mapBasePath={3} --cwMin={4} --cwMax={5}".format(b, txRange, protocol, mapPathWithoutExtension, cwMin, cwMax)
				newJobName = "urban-" + mapBaseName + "-d" + str(vehicleDistance) + "-cw-" +str(cwMin) + "-" + str(cwMax) + "-b" + b + "-" + protocolsMap[protocol] + "-" + txRange
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
	#scenarios = ["Padova", "LA"]
	#contentionWindows = [{"cwMin": 32, "cwMax": 1024}, {"cwMin": 16, "cwMax": 128}]
	#contentionWindows = [{"cwMin": 16, "cwMax": 128}]
	contentionWindows = [{"cwMin": 32, "cwMax": 1024}]
	#distances = ["15", "25", "35", "45"]
	scenarios = ["Padova"]
	distances = ["25"]
	
	# Removes all previous job templates in output directory
	thisScriptPath = os.path.realpath(__file__)
	thisScriptParentPath = os.path.dirname(thisScriptPath)
	jobsPath = os.path.join(os.path.dirname(thisScriptParentPath), "jobsTemplate")
	map( os.unlink, (os.path.join(jobsPath,f) for f in os.listdir(jobsPath)) )

	if (len(sys.argv) < 3):
		for cw in contentionWindows:
			for scenario in scenarios:
				for distance in distances:
					runScenario(cw, scenario, distance)
	else:
		runScenario(None, None)
if __name__ == "__main__":
	main()