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

def plotTxRange(txRange, starterCoordX, starterCoordY, vehicleDistance, color, plotInterval):
    x = np.linspace(0, 3000, 100)
    y = np.linspace(0, 3000, 100)
    X, Y = np.meshgrid(x, y)
    print starterCoordX
    print starterCoordY
    realTxRange = (X - starterCoordX) ** 2 + (Y - starterCoordY) ** 2 - txRange ** 2
    CS = plt.contour(X, Y, realTxRange, [0], colors = color)

    if (plotInterval):
        outerTxRange = (X - starterCoordX) ** 2 + (Y - starterCoordY) ** 2 - (txRange + vehicleDistance) ** 2 
        innerTxRange = (X - starterCoordX) ** 2 + (Y - starterCoordY) ** 2 - (txRange - vehicleDistance) ** 2
        plt.contour(X, Y, outerTxRange, [0], colors = color, linestyles = "dashed")
        plt.contour(X, Y, innerTxRange, [0], colors = color, linestyles = "dashed")
    # F = X**2 + Y**2 - 90000
    # if (color == "#840000"):
    plt.clabel(CS, inline=1, fontsize=10)
    CS.collections[0].set_label(str(txRange) + " m")

def retrieveCoords(coords):
    xCoords = []
    yCoords = []
    coords = coords.split("_")
    for coord in coords:
        splitCoord = coord.split(":")
        if (len(splitCoord) != 3):
            continue;
        xCoords.append(float(splitCoord[0]))
        yCoords.append(float(splitCoord[1]))
    return xCoords, yCoords

def main():
    relativeFileName = sys.argv[1]
    plt.rcParams["figure.figsize"] = [10, 10]
    polyFilePath = "../../maps/Padova/Padova-25.poly.xml"
    print("Main!!")
    startingVehicle = 0
    vehicleDistance = 0
    txRange = 0
    circRadius = 1000
    xReceivedCoords = []
    yReceivedCoords = []
    xNodeCoords = []
    yNodeCoords = []
    startingX = 0
    startingY = 0

    with open(relativeFileName, "r") as file:
        csvFile = csv.reader(file, delimiter=",")
        next(csvFile)
        line = next(csvFile)
        txRange = int(line[2])
        startingX = float(line[14])
        startingY = float(line[15])
        startingVehicle = int(line[16])
        vehicleDistance = int(line[17])
        receivedCoords = line[18]
        nodeCoords = line[19]
        xReceivedCoords, yReceivedCoords = retrieveCoords(receivedCoords)
        xNodeCoords, yNodeCoords = retrieveCoords(nodeCoords)
    print(startingX)
    print(startingY)
    plt.plot(xNodeCoords, yNodeCoords, ".", color="red")
    plt.plot(xReceivedCoords, yReceivedCoords, ".", color="green")
    plt.plot(startingX, startingY, "ro", color="blue", markersize=10)

    color1 = "#840000"
    plotTxRange(circRadius, startingX, startingY, vehicleDistance, color1, True)

    tree = ET.parse(polyFilePath)
    root = tree.getroot()
    polyList = list(root.iter("poly"))
    count = 0
    print(len(polyList))
    for poly in polyList:
        polyType = poly.get("type")
        if (polyType != "building" and polyType != "amenity"):
        #if (polyType == "water" or polyType == "residential" or polyType == "landuse" or polyType == "natural"
        #or polyType == "historic"):
        #amenity
        #leisure
        
            continue
        count = count + 1
        coords = poly.get("shape")
        splitCoords = coords.split( )
        xShapeCoords = []
        yShapeCoords = []
        for coord in splitCoords:
            splitCoords2 = coord.split(",")
            xShapeCoords.append(float(splitCoords2[0]))
            yShapeCoords.append(float(splitCoords2[1]))
        plt.fill(xShapeCoords, yShapeCoords, color="red", alpha=0.15)
    
    #plt.savefig("prova.pdf")
    plt.show()
   
        #print(line)
'''
    color1 = "#840000"
    color2 = "#677a04"
    color3 = "#000000"
    
    plt.xlabel("x")
    plt.ylabel("y")
    plt.title(graphTitle)

    plt.plot(xPos, yPos, ".")
    plt.plot(starterCoordX, starterCoordY, "ro")


    plotTxRange(maxTxRange, starterCoordX, starterCoordY, vehicleDistance, color2, False)
    plotTxRange(1000, starterCoordX, starterCoordY, vehicleDistance, color3, True)
    plt.legend(loc = "upper left")
   
    plt.show()
    print(str(starterCoordX) + " " + str(starterCoordY))
    print("dopo plot")
'''


if __name__ == "__main__":
    main()
