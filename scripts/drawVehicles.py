#!/usr/bin/python

import os
import sys
import getopt
import numpy as np
import matplotlib
import matplotlib.pyplot as plt


def main():
    graphTitle = sys.argv[1]
    print(graphTitle)

    print("Main!!")
    startingVehicle = "313"
    vehicleDistance = 25
    minTxRange = 300
    maxTxRange = 900
    color1 = "#840000"
    color2 = "#677a04"
    
    thisScriptPath = os.path.dirname(os.path.realpath(__file__))
    ns2MobilityPath = os.path.dirname(thisScriptPath) + "/ns-3.26/Padova.ns2mobility.xml"
    print("ns2mobility = " + ns2MobilityPath)
    print(thisScriptPath)
    print(os.getcwd())
    ns2MobilityFile = open(ns2MobilityPath, "r")
    line = ns2MobilityFile.readline()
    xPos = []
    yPos = []
    starterCoordX = 0
    starterCoordY = 0
    while line and len(line.split(" ")) == 4:
        found = False
        strings = line.split(" ")
        string = strings[0]
        vehicleNumber = string[string.find("(")+1:string.find(")")]
        coord = float(strings[3].rstrip())
        if (vehicleNumber == startingVehicle):
            print("trovatoooooooooooo")
            found = True
            starterCoordX = coord
        xPos.append(coord)

        line = ns2MobilityFile.readline()
        strings = line.split(" ")
        coord = float(strings[3].rstrip())
        yPos.append(coord)
        if (found):
            starterCoordY = coord
        ns2MobilityFile.readline()
        ns2MobilityFile.readline()
        line = ns2MobilityFile.readline()
        found = False
    

    plt.xlabel("x")
    plt.ylabel("y")
    plt.title(graphTitle)

    plt.plot(xPos, yPos, ".")
    plt.plot(starterCoordX, starterCoordY, "ro")

    plotTxRange(minTxRange, starterCoordX, starterCoordY, vehicleDistance, color1)
    plotTxRange(maxTxRange, starterCoordX, starterCoordY, vehicleDistance, color2)
    plt.legend(loc = "upper left")
   
    plt.show()
    print(str(starterCoordX) + " " + str(starterCoordY))
    print("dopo plot")
    
    
        
def plotTxRange(txRange, starterCoordX, starterCoordY, vehicleDistance, color):
    x = np.linspace(-500, 3500, 100)
    y = np.linspace(-500, 3500, 100)
    X, Y = np.meshgrid(x, y)

    realTxRange = (X - starterCoordX) ** 2 + (Y - starterCoordY) ** 2 - txRange**2
    outerTxRange = (X - starterCoordX) ** 2 + (Y - starterCoordY) ** 2 - (txRange + vehicleDistance) ** 2 
    innerTxRange = (X - starterCoordX) ** 2 + (Y - starterCoordY) ** 2 - (txRange - vehicleDistance) ** 2

    # F = X**2 + Y**2 - 90000
    CS = plt.contour(X, Y, realTxRange, [0], colors = color)
    plt.contour(X, Y, outerTxRange, [0], colors = color, linestyles = "dashed")
    plt.contour(X, Y, innerTxRange, [0], colors = color, linestyles = "dashed")
    # if (color == "#840000"):
    plt.clabel(CS, inline=1, fontsize=10)
    CS.collections[0].set_label(str(txRange) + " m")
        


if __name__ == "__main__":
    main()
