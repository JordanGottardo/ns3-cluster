#!/usr/bin/python
#coding=utf-8
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

def findSender(id, transmissionMap):
    for entry in transmissionMap.items():
        if id in entry[1]:
            return entry[0]
    return None   
'''
def findSource(transmissionMap):
    s = set()
    for value in transmissionMap.values():
        #print(value)
        for element in value:
            print(element)
            s.add(element)
        #print(s)
    #print(s)
    #print(s)
    k = set(transmissionMap.keys())
    #print(k)
    difference = k - s
    print(difference)
    return difference.pop()
    #for key in transmissionMap.keys():
    #    for entry in transmissionMap.items():
    #        if nodeCoord in entry[1]:
    #            return entry[0]
'''

def main():
    relativeFileName = sys.argv[1]
    ns2MobilityFile = "../../maps/Padova/Padova-25.ns2mobility.xml"
    polyFilePath = "../../maps/Padova/Padova-25.poly.xml"
    plt.rcParams["figure.figsize"] = [10, 10]
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
    receivedOnCircIds = []

    txRange, startingX, startingY, startingVehicle, vehicleDistance, xReceivedCoords, yReceivedCoords, xNodeCoords, yNodeCoords, transmissionMap, receivedCoordsOnCirc, receivedOnCircIds = coordUtils.parseFile(relativeFileName, ns2MobilityFile)

    plt.plot(xNodeCoords, yNodeCoords, ".", color="red")
    plt.plot(xReceivedCoords, yReceivedCoords, ".", color="green")
    plt.plot(startingX, startingY, "ro", color="blue", markersize=10)

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
        baseColor = 0.3
        while(i < size - 1):
            #print("ciclo")
            #baseColor += 0.3
            coord1 = coordUtils.findCoordsFromFile(path[i], ns2MobilityFile)
            coord2 = coordUtils.findCoordsFromFile(path[i + 1], ns2MobilityFile)
            c1 = np.array((coord1.x, coord1.y, coord1.z))
            c2 = np.array((coord2.x, coord2.y, coord2.z))
            print(np.linalg.norm(c1-c2))
            plt.plot(coord1.x, coord1.y, "ro", color="yellow", markersize=10)
            plt.plot([coord1.x, coord2.x], [coord1.y, coord2.y], color=str(baseColor))
            i = i + 1
        
    plt.plot(startingX, startingY, "ro", color="blue", markersize=10)


    # trova coordinate di quelli che hanno ricevuto e che sono all'interno della circonferenza (c++)

    # per ognuno nella circonferenza, traccia il percorso a partire dall'origine fino al nodo stesso
        #parti dal nodo sulla circonferenza (cercare nei values)
        # prendi la key del pair entro cui quel nodo è presente nei values (la key  il nodo da cui il nostro nodo di interesse ha ricevuto il pacchetto)
        # cerca da chi la key ha ricevuto il pacchetto ovvero cerca la key della entry entro cui il nodo  presente
        # ripeti

    color1 = "#840000"
    coordUtils.plotTxRange(circRadius, startingX, startingY, vehicleDistance, color1, True)

    #coordUtils.plotBuildings(polyFilePath)
    
    #plt.savefig("prova.pdf")
    plt.show()
   
        #print(line)


if __name__ == "__main__":
    main()
