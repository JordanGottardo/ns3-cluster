#!/usr/bin/python

import sys, os

def main():
	# Input parameters
	mapPath = sys.argv[1]
	vehicleDistance = sys.argv[2]

	# Calculates directories
	absMapPath = os.path.abspath(mapPath)
	absMapParentPath = os.path.dirname(absMapPath)
	mapBaseName = os.path.basename(mapPath).split(".")[0] + "-" + vehicleDistance

	thisScriptPath = os.path.realpath(__file__)
	thisScriptParentPath = os.path.dirname(thisScriptPath)

	# Defines paths of files necessary for ns3
	mobilityFilePath = absMapParentPath + "/" + mapBaseName + "ns2mobility.xml"
	polygonFilePath = absMapParentPath + "/" + mapBaseName + "poly.xml"
	polygonFilePath3D = absMapParentPath + "/" + mapBaseName + "3D.poly.xml"
	# identificare veicolo che genera il segnale TODO MERC

	# aggiungere parametri a ns3 in modo che prenda i file corretti (e calcoli correttamente il veicolo iniziale?)
	# i file si chiameranno PD-25.poly.xml, ecc, dove 25 e' la distanza


	# far si che i gli .out vengano messi in cartelle corrette
	# lanciare jobs in automatico
	# grafici??

	
	# Runs generate sumo files
	sumoFileGenerator = thisScriptParentPath + "/generate-sumo-files.sh " + " ".join(sys.argv[1:])
	os.system(sumoFileGenerator)

	# Creates jobs and runs them on cluster
	


if __name__ == "__main__":
	main()