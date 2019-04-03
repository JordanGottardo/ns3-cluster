#!/usr/bin/python2
#coding=utf-8
#Invocation:
#   ./drawAlertPaths.py
# OR
#   ./drawAlertPaths.py path.csv
# example: ./drawAlertPaths.py /home/jordan/MEGA/Universita_mia/Magistrale/Tesi/ns3-cluster/ns-3.26/out/scenario-urbano-con-coord/cw-32-1024/Padova/d25/b0/st500-500/Padova-25-cw-32-1024-b0-st500-500-1550077028283.csv

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

def findSender(id, transmissionMap):
    for entry in transmissionMap.items():
        if id in entry[1]:
            return entry[0]
    return None   

def plotAlertPath(relativeFileName, outFilePath, ns2MobilityFile, polyFilePath):
    print("Plotting alet path " + relativeFileName)
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

    plt.plot(xNodeCoords, yNodeCoords, ".", markersize=5, color="red")
    plt.plot(xReceivedCoords, yReceivedCoords, ".", color="green")

    pathsToPlot = {item: [item] for item in receivedOnCircIds}
    for idOnCirc in receivedOnCircIds:
        sender = findSender(idOnCirc, transmissionMap)
        while (sender is not None):
            pathsToPlot[idOnCirc].append(sender)
            sender = findSender(sender, transmissionMap)
        pathsToPlot[idOnCirc].append(str(startingVehicle))

    count = 0
    for entry in pathsToPlot.items():
        #if (count > 0):
        #    continue
        #count = count + 1
        path = entry[1]
        #print("path = ")
        #print(path)
        size = len(path)
        #print(size)
        i = 0
        lineColor = "0.4"
        while(i < size - 1):
            #print("ciclo")
            #baseColor += 0.3
            coord1 = coordUtils.findCoordsFromFile(path[i], ns2MobilityFile)
            coord2 = coordUtils.findCoordsFromFile(path[i + 1], ns2MobilityFile)
            c1 = np.array((coord1.x, coord1.y, coord1.z))
            c2 = np.array((coord2.x, coord2.y, coord2.z))
            #print(np.linalg.norm(c1-c2))
            if (i > 0):
                plt.plot(coord1.x, coord1.y, "ro", color="#560589", markersize=5)
            plt.plot([coord1.x, coord2.x], [coord1.y, coord2.y], color=lineColor, linewidth=0.3)
            i = i + 1
        
    plt.plot(startingX, startingY, "ro", color="blue", markersize=5)

    color1 = "#840000"
    coordUtils.plotTxRange(circRadius, startingX, startingY, vehicleDistance, color1, True)

    #coordUtils.plotBuildings(polyFilePath)

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
    print("Draw alert paths")
    if (len(sys.argv) > 1):
        relativeFileName = sys.argv[1]
        ns2MobilityFile = sys.argv[2]
        polyFilePath = None
        if (len(sys.argv) > 3):
            polyFilePath = sys.argv[3]
        plotAlertPath(relativeFileName, "./out/singlefileAlertPath/alertPath.pdf", ns2MobilityFile, polyFilePath)
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
                            outFilePath = os.path.join("./out/alertPaths", buildingFolder, protocolFolder, os.path.splitext(csvFilename)[0] + ".pdf")
                            plotAlertPath(relativeFileName, outFilePath)
                        

if __name__ == "__main__":
    main()
