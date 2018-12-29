#!/usr/bin/python

# Invocation: 
# 	./generateAndRunTests.py ../maps/testMap/osm.osm.xml 25 
# if you want to specify map and distance by hand, othwerise:
# 	./generateAndRunTests.py 
# if you want to automatically generate jobs for the scenarios and distances set in the main

import sys, os
import string
import shutil

def main():

	if (len(sys.argv) < 3):
		for scenario in scenarios:
			for distance in distances:
				runScenario(scenario, distance)
	else:
		runScenario(None, None)
if __name__ == "__main__":
	main()