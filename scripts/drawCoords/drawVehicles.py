#!/usr/bin/python

#Invocation:
#   ./drawVehicles.py graphTitle ../../maps/Padova/Padova-25.ns2mobility.xml

import os
import sys
import getopt
import numpy as np  
import matplotlib
import matplotlib.pyplot as plt


def main():
    graphTitle = sys.argv[1]
    ns2MobilityRelativePath = sys.argv[2]
    print(graphTitle)

    print("Main!!")
    startingVehicle = "313"
    vehicleDistance = 25
    minTxRange = 300
    maxTxRange = 900
    color1 = "#840000"
    color2 = "#677a04"
    color3 = "#000000"
    
    thisScriptPath = os.path.dirname(os.path.realpath(__file__))
    ns2MobilityPath = os.path.join(thisScriptPath, ns2MobilityRelativePath)
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
    print starterCoordX
    print starterCoordY
    plt.plot(starterCoordX, starterCoordY, "ro")

    print starterCoordX
    print starterCoordY
    plotTxRange(minTxRange, starterCoordX, starterCoordY, vehicleDistance, color1, False)
    plotTxRange(maxTxRange, starterCoordX, starterCoordY, vehicleDistance, color2, False)
    plotTxRange(1000, starterCoordX, starterCoordY, vehicleDistance, color3, True)
    plt.legend(loc = "upper left")
   
    plt.show()
    print(str(starterCoordX) + " " + str(starterCoordY))
    print("dopo plot")
    
    
        
def plotTxRange(txRange, starterCoordX, starterCoordY, vehicleDistance, color, plotInterval):
    x = np.linspace(-500, 3500, 100)
    y = np.linspace(-500, 3500, 100)
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
        


if __name__ == "__main__":
    main()
