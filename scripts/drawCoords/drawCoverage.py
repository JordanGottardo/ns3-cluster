#!/usr/bin/python

#Invocation:
#   ./drawVehicles.py fileName.csv

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
    relativeFileName = sys.argv[1]
    plt.rcParams["figure.figsize"] = [10, 10]
    polyFilePath = "../../maps/Padova/Padova-25.poly.xml"
    circRadius = 1000

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

    txRange, startingX, startingY, startingVehicle, vehicleDistance, xReceivedCoords, yReceivedCoords, xNodeCoords, yNodeCoords, transmissionMap, receivedCoordsOnCirc = coordUtils.parseFile(relativeFileName)

    plt.plot(xNodeCoords, yNodeCoords, ".", color="red")
    plt.plot(xReceivedCoords, yReceivedCoords, ".", color="green")
    plt.plot(startingX, startingY, "ro", color="blue", markersize=10)

    color1 = "#840000"
    coordUtils.plotTxRange(circRadius, startingX, startingY, vehicleDistance, color1, True)

    coordUtils.plotBuildings(polyFilePath)
    
    #plt.savefig("prova.pdf")
    #plt.show()
   
        #print(line)


if __name__ == "__main__":
    main()
