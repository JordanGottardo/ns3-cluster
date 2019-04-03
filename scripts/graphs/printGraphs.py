#!/usr/bin/python

import printMultipleGraphs
import sys

def main():
	#contentionWindows = ["cw-32-1024"]
	contentionWindows = ["cw-16-128", "cw-32-1024"]
	vehicleDistances = [15, 25, 35, 45]
	fbProtocols = ["fb-100", "fb-300", "fb-500"]
	stProtocols = ["st100-100", "st300-300", "st500-500"]
	#allProtocols = ["fb-300", "fb-500", "st300-300", "st300-500", "st500-300", "st500-500"]
	allProtocols = ["fb-300", "fb-500"]
	if (len(sys.argv) <= 1):
		# vehicle distance comparison
		distanceXLabels = list(map(lambda distance: str(distance) + "m", vehicleDistances))
		printMultipleGraphs.printDistanceComparison(contentionWindows[1], vehicleDistances, fbProtocols, vehicleDistances, distanceXLabels,
													"fb", "Fast Broadcast protocols", "vehicleDistanceComparison", False)
		printMultipleGraphs.printDistanceComparison(contentionWindows[1], vehicleDistances, stProtocols, vehicleDistances, distanceXLabels,
													"st", "Static protocols", "vehicleDistanceComparison", False)

		# contention window comparison
		cwXLabels = ["cw-16-128", "cw-32-1024"]
		printMultipleGraphs.printCwComparison(contentionWindows, 25, fbProtocols, contentionWindows, cwXLabels,
													"fb", "Fast Broadcast protocols", "contentionWindowComparison", False)
		printMultipleGraphs.printCwComparison(contentionWindows, 25, stProtocols, contentionWindows, cwXLabels,
													"st", "Static protocols", "contentionWindowComparison", False)

		# Romanelli comparison
		basePath1 = "/home/jordan/MEGA/Universita_mia/Magistrale/Tesi/ns3-cluster/ns-3.26/out/scenario-urbano"
		
		#allProtocols = ["fb-300", "fb-500", "st300-300", "st500-500"]
		printMultipleGraphs.printRomanelliComparison(contentionWindows[1], 25, allProtocols, allProtocols, allProtocols,
													"rom", "", "romanelliComparison", basePath1, False)

	# Romanelli comparison with coord (fixed starting vehicle)
	basePath2 = "/home/jordan/MEGA/Universita_mia/Magistrale/Tesi/ns3-cluster/ns-3.26/out/scenario-urbano-con-coord"
	printMultipleGraphs.printRomanelliComparison(contentionWindows[1], 25, allProtocols, allProtocols, allProtocols,
												"rom", "", "coord/romanelliComparison", basePath2, False)

if __name__ == "__main__":
	main()

