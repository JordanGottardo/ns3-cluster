#!/usr/bin/python2
#coding=utf-8
#Invocation:
#   ./drawSingleTransmission.py numTransmissionToPlot
# OR
#   ./drawSingleTransmission.py path.csv numTransmissionToPlot
# example: ./drawSingleTransmission.py /home/jordan/MEGA/Universita_mia/Magistrale/Tesi/ns3-cluster/ns-3.26/out/scenario-urbano-con-coord/cw-32-1024/Padova/d25/b0/st500-500/Padova-25-cw-32-1024-b0-st500-500-1550077028283.csv 10

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


def plotSingleTransmission(relativeFileName, outFileBasePath, numTransmissionToPlot):
    print("Plotting single transmission " + relativeFileName)
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

    txRange, startingX, startingY, startingVehicle, vehicleDistance, xReceivedCoords, yReceivedCoords, xNodeCoords, yNodeCoords, transmissionMap, receivedCoordsOnCirc, receivedOnCircIds, transmissionVector = coordUtils.parseFile(relativeFileName, ns2MobilityFile)

    nodeCoordsMap = {}

    color1 = "#840000"

    for i in range(1, numTransmissionToPlot + 1):
        plt.plot(xNodeCoords, yNodeCoords, ".", color="red")
        coordUtils.plotTxRange(circRadius, startingX, startingY, vehicleDistance, color1, True)
        count = 0
        for edge in transmissionVector:
            if (count >= i):
                break
            count += 1
            lineColor = "0.4"
            source = edge.source
            destination = edge.destination
            if (not source in nodeCoordsMap):
                nodeCoordsMap[source] = coordUtils.findCoordsFromFile(edge.source, ns2MobilityFile)
            if (not destination in nodeCoordsMap):
                nodeCoordsMap[destination] = coordUtils.findCoordsFromFile(edge.destination, ns2MobilityFile)
            sourceCoord = nodeCoordsMap[source]
            destCoord = nodeCoordsMap[destination]
            
            c1 = np.array((sourceCoord.x, sourceCoord.y, sourceCoord.z))
            c2 = np.array((destCoord.x, destCoord.y, destCoord.z))
            plt.plot(sourceCoord.x, sourceCoord.y, "ro", color="#af41f4", markersize=5)
            plt.plot([sourceCoord.x, destCoord.x], [sourceCoord.y, destCoord.y], color=lineColor, linewidth=0.3)
            plt.plot(destCoord.x, destCoord.y, ".", color="green", markersize=5)   
            plt.plot(startingX, startingY, "ro", color="blue", markersize=5)

            coordUtils.plotTxRange(circRadius, startingX, startingY, vehicleDistance, color1, True)

            #coordUtils.plotBuildings(polyFilePath)

            #Save file 
            
        if not os.path.exists(os.path.dirname(outFileBasePath)):
            try:
                os.makedirs(os.path.dirname(outFileBasePath))
            except OSError as exc: # Guard against race condition
                if exc.errno != errno.EEXIST:
                    raise
        plt.savefig(outFileBasePath + "-transmission" + str(i) + ".pdf")
        plt.clf()
        

def main():
    print("Draw single transmissions")
    relativeFileName = sys.argv[1]
    numTransmissionToPlot = int(sys.argv[2])
    plotSingleTransmission(relativeFileName, "./out/singlefileSingleTransmission/singleTransmission", numTransmissionToPlot)

if __name__ == "__main__":
    main()
