#!/usr/bin/python

#Invocation:
#   ./drawAll.py filePath.csv ns2mobility.ns2mobility.xml [polyFile.poly.xml]

# example: ./drawAll.py /home/jordan/MEGA/Universita_mia/Magistrale/Tesi/ns3-cluster/ns-3.26/out/scenario-urbano-con-coord/cw-32-1024/Padova/d25/b0/st500-500/Padova-25-cw-32-1024-b0-st500-500-1550077028283.csv /home/jordan/MEGA/Universita_mia/Magistrale/Tesi/ns3-cluster/maps/Padova/Padova-25.poly.xml /home/jordan/MEGA/Universita_mia/Magistrale/Tesi/ns3-cluster/maps/Padova/Padova-25.poly.xml

import os
import sys
import getopt
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import csv
import xml.etree.ElementTree as ET
import coordUtils as coordUtils
from multiprocessing import Pool

def run_process(process):
    os.system('python {}'.format(process))


def main():
    
    #os.system("./drawCoverage.py")
    #os.system("./drawAlertPaths.py")
    pathToDraw = sys.argv[1]
    ns2MobilityFile = sys.argv[2]
    polyFilePath = None
    if (len(sys.argv) > 3):
        polyFilePath = sys.argv[3]

    argList = pathToDraw + " " + ns2MobilityFile
    if (polyFilePath is not None):
        argList = argList + " " + polyFilePath


    processes = ("./drawCoverage.py " + argList, "./drawAlertPaths.py " + argList, "./drawSingleHops.py " + argList, "./drawMultipleTransmissions.py " + argList)
    
    pool = Pool(processes=4)
    pool.map(run_process, processes)

if __name__ == "__main__":
    main()
