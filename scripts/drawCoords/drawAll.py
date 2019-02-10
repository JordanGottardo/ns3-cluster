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
    os.system("./drawCoverage.py")
    os.system("./drawAlertPaths.py")

if __name__ == "__main__":
    main()
