#!/usr/bin/python

#Invocation:
#   ./drawJunctions.py netFilePath




import os
import sys
import getopt
import numpy as np  
import matplotlib
import matplotlib.pyplot as plt
import coordUtils
import xml.etree.ElementTree as ET


def main():
    netFilePath = sys.argv[1]
    print(netFilePath)
    print("Main!!")
    color1 = "#840000"
    color2 = "#677a04"
    color3 = "#000000"
    
    coordUtils.plotJunctions(netFilePath)
    plt.show()
   
        


if __name__ == "__main__":
    main()
