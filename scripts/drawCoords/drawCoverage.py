#!/usr/bin/python2
#coding=utf-8
#Invocation:
#   ./drawCoverage.py
# OR
#   ./drawCoverage.py path.csv
# example: ./drawCoverage.py /home/jordan/MEGA/Universita_mia/Magistrale/Tesi/ns3-cluster/ns-3.26/out/scenario-urbano-con-coord/cw-32-1024/Padova/d25/b0/st500-500/Padova-25-cw-32-1024-b0-st500-500-1550077028283.csv

import os
import sys
import getopt
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import csv
import xml.etree.ElementTree as ET
import coordUtils as coordUtils


ns2MobilityFile = "../../maps/Padova/Padova-25.ns2mobility.xml"
polyFilePath = "../../maps/Padova/Padova-25.poly.xml"
plt.rcParams["figure.figsize"] = [10, 10]
circRadius = 1000
baseFolder = "../../ns-3.26/out/scenario-urbano-con-coord/cw-32-1024/Padova/d25/"

def plotCoverage(relativeFileName, outFilePath):
    print("Plotting coverage " + relativeFileName)
    startingVehicle = 0
    vehicleDistance = 0
    txRange = 0
    xReceivedCoords = []
    yReceivedCoords = []
    xNodeCoords = []
    yNodeCoords = []
    startingX = 0
    startingY = 0
    transmissionMap = {}
    receivedCoordsOnCirc = []
    receivedOnCircIds = []
    transmissionVector = []
    nodeIds = []

    txRange, startingX, startingY, startingVehicle, vehicleDistance, xReceivedCoords, yReceivedCoords, xNodeCoords, yNodeCoords, transmissionMap, receivedCoordsOnCirc, receivedOnCircIds, transmissionVector, nodeIds = coordUtils.parseFile(relativeFileName, ns2MobilityFile) 

    plt.plot(xNodeCoords, yNodeCoords, ".",markersize=5, color="red")
    plt.plot(xReceivedCoords, yReceivedCoords, ".", color="green")
    plt.plot(startingX, startingY, "ro", color="blue", markersize=5)

    color1 = "#840000"
    coordUtils.plotTxRange(circRadius, startingX, startingY, vehicleDistance, color1, True)
    coordUtils.plotBuildings(polyFilePath)
    
    #Save file
    if not os.path.exists(os.path.dirname(outFilePath)):
        try:
            os.makedirs(os.path.dirname(outFilePath))
        except OSError as exc: # Guard against race condition
            if exc.errno != errno.EEXIST:
                raise
    plt.savefig(outFilePath)
    plt.clf()

def main():
    print("Draw coverage")
    if (len(sys.argv) > 1):
        relativeFileName = sys.argv[1]
        plotCoverage(relativeFileName, "./out/singlefileCoverage/coverage.pdf")
    else:
        for buildingFolder in os.listdir(baseFolder):
            buildingPath = os.path.join(baseFolder, buildingFolder)
            if (os.path.isdir(buildingPath)):
                for protocolFolder in os.listdir(buildingPath):
                    protocolPath = os.path.join(buildingPath, protocolFolder)
                    if (os.path.isdir(protocolPath)):
                        count = 0
                        for csvFilename in os.listdir(protocolPath):
                            relativeFileName = os.path.join(protocolPath, csvFilename)
                            if (count > 2 or not coordUtils.isFileComplete(relativeFileName)):
                                continue
                            count += 1
                            outFilePath = os.path.join("./out/coverages", buildingFolder, protocolFolder, os.path.splitext(csvFilename)[0] + ".pdf")
                            plotCoverage(relativeFileName, "./outCoverage.pdf")
    

if __name__ == "__main__":
    main()
