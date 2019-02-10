#!/usr/bin/python

#Invocation:
#   ./drawVehicles.py

import os
import sys
import getopt
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import csv
import xml.etree.ElementTree as ET
import coordUtils as coordUtils

def main():
    print("Draw coverage")

    ns2MobilityFile = "../../maps/Padova/Padova-25.ns2mobility.xml"
    polyFilePath = "../../maps/Padova/Padova-25.poly.xml"
    plt.rcParams["figure.figsize"] = [10, 10]
    circRadius = 1000
    baseFolder = "./in/scenario-urbano-con-coord/cw-32-1024/Padova/d25/"


    for buildingFolder in os.listdir(baseFolder):
        buildingPath = os.path.join(baseFolder, buildingFolder)
        if (os.path.isdir(buildingPath)):
            for protocolFolder in os.listdir(buildingPath):
                protocolPath = os.path.join(buildingPath, protocolFolder)
                if (os.path.isdir(protocolPath)):
                    for csvFilename in os.listdir(protocolPath):
                        relativeFileName = os.path.join(protocolPath, csvFilename)
                        print("Plotting" + relativeFileName)
                        outFilePath = os.path.join("./out/coverages", buildingFolder, protocolFolder, os.path.splitext(csvFilename)[0] + ".pdf")

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

                        txRange, startingX, startingY, startingVehicle, vehicleDistance, xReceivedCoords, yReceivedCoords, xNodeCoords, yNodeCoords, transmissionMap, receivedCoordsOnCirc, receivedOnCircIds = coordUtils.parseFile(relativeFileName, ns2MobilityFile) 

                        plt.plot(xNodeCoords, yNodeCoords, ".", color="red")
                        plt.plot(xReceivedCoords, yReceivedCoords, ".", color="green")
                        plt.plot(startingX, startingY, "ro", color="blue", markersize=10)

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

if __name__ == "__main__":
    main()
