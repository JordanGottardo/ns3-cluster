#!/usr/bin/python

#Invocation:
#   ./drawJunctions.py netFilePath ns2MobilityFile [polyFilePath]




import os
import sys
import getopt
import numpy as np  
import matplotlib
import matplotlib.pyplot as plt
import coordUtils
import xml.etree.ElementTree as ET

startingNodeId = "310" 

def main():
    polyFilePath = None
    netFilePath = sys.argv[1]
    ns2MobilityFile = sys.argv[2]
    if (len(sys.argv) > 3):
        polyFilePath = sys.argv[3]
    print(netFilePath)
    print("Main!!")
    color1 = "#840000"
    color2 = "#677a04"
    color3 = "#000000"
    
    coordUtils.plotBuildings(polyFilePath)
    coordUtils.plotJunctions(netFilePath)
    coordUtils.plotNodeList(ns2MobilityFile)
    coordUtils.plotStartingNode(startingNodeId, ns2MobilityFile)
    plt.legend(loc='best')
    plt.show()
   
        


if __name__ == "__main__":
    main()
