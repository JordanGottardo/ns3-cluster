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


plt.rcParams["figure.figsize"] = [10, 10]
circRadius = 1000
baseFolder = "../../ns-3.26/out/scenario-urbano-con-coord/cw-32-1024/Padova/d25/"

def plotCoverage(relativeFileName, outFilePath, ns2MobilityFile, polyFilePath):
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

    plt.plot(xNodeCoords, yNodeCoords, ".",markersize=5, color="#A00000", label="Not reached by Alert Message")
    plt.plot(xReceivedCoords, yReceivedCoords, ".", color="#32DC32", label="Reached by Alert Message")
    plt.plot(startingX, startingY, "ro", color="yellow", markersize=5, markeredgecolor="blue", label="Source of Alert Message")
    plt.legend(loc="best", framealpha=1.0)
    #plt.xlim(300, 2700)
    #plt.ylim(300, 2700)
    color1 = "black"
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
        ns2MobilityFile = sys.argv[2]
        polyFilePath = None
        if (len(sys.argv) > 3):
            polyFilePath = sys.argv[3]
        plotCoverage(relativeFileName, "./out/singlefileCoverage/coverage.pdf", ns2MobilityFile, polyFilePath)
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
