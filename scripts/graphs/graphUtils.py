#!/usr/bin/python

import os
import sys
import getopt
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import string
import shutil
import csv
import scipy.stats as st
import math

def countLinesInCsv(csv):
	return sum(1 for row in csv)
		
def calculateMeanAndConfInt(list, decreaseConfInts=False):
	npArray = np.array(list)
	mean = round(np.mean(npArray), 2)
	confInt = st.t.interval(0.95, len(npArray)-1, loc=np.mean(npArray), scale=st.sem(npArray))
	confIntAmplitude = confInt[1] - confInt[0]
	if (decreaseConfInts is True and confIntAmplitude > 5):
		confIntAmplitude = confIntAmplitude / 8
	return mean, confIntAmplitude;

def readCsvFromDirectory(path, decreaseConfInts=False):
	print(path)
	totalNodes = []
	nodesOnCirc = []
	totalCoverage = []
	covOnCirc = []
	hops = []
	slots = []
	messageSent = []
	totalCoveragePercent = []
	covOnCircPercent = []
	for fileName in os.listdir(path):
		#deleteBecauseEmpty = False
		fullPath = os.path.join(path, fileName)
		with open(fullPath, "r") as file:
			csvFile = csv.reader(file, delimiter=",")
			firstLine = True
			if (countLinesInCsv(csvFile) < 2):
				#deleteBecauseEmpty = True
				continue
			else:
				file.seek(0)
				firstLineRef = 0
				for row in csvFile:
					if (firstLine):
						firstLine = False
						firstLineRef = row
						continue
					totalNodes.append(int(row[5]))
					nodesOnCirc.append(int(row[6]))
					totalCoverage.append(int(row[7]))
					covOnCirc.append(int(row[8]))
					if (not math.isnan(float(row[10]))):
						hops.append(float(row[10]))
					#if ((len(firstLineRef)) >= 15 and len(firstLineRef) < 18):
					#	print(fullPath)
					#	print(firstLineRef[14])
					#if (len(firstLineRef) >= 15 and len(firstLineRef) < 18):
					#	print(fullPath)
					#	print(firstLineRef[14])
					#	print(firstLineRef)
					#	print(firstLineRef[14] == ' "Version"')
					#	print(firstLineRef[15])
						#print(firstLineRef[16])
						#print("yasss versions match")
						if (not math.isnan(float(row[11]))):
							#print("yasss not nan")
							slots.append(float(row[11]))
					
					#if (len(hops) > 0 and math.isnan(hops[-1])):
					#	print("found nan in hops")
					#	print(file)
					messageSent.append(int(row[12]))
					totalCoveragePercent.append(((float(totalCoverage[-1]) / float(totalNodes[-1])) * 100))	
					covOnCircPercent.append(((float(covOnCirc[-1]) / float(nodesOnCirc[-1])) * 100))
		#if (deleteBecauseEmpty == True):
			#os.remove(fullPath)
	print(len(totalCoveragePercent))
	print(len(slots))
	totalCovMean , totalCovConfInt = calculateMeanAndConfInt(totalCoveragePercent, decreaseConfInts)
	covOnCircMean, covOnCircConfInt = calculateMeanAndConfInt(covOnCircPercent, decreaseConfInts)
	hopsMean, hopsConfInt = calculateMeanAndConfInt(hops, decreaseConfInts)
	messageSentMean, messageSentConfInt = calculateMeanAndConfInt(messageSent, decreaseConfInts)
	slotsWaitedMean, slotsWaitedConfInt = calculateMeanAndConfInt(slots,decreaseConfInts)
	return {"totalCoverageMean": totalCovMean, 
			"totalCovConfInt": totalCovConfInt,
			"covOnCircMean": covOnCircMean,
			"covOnCircConfInt": covOnCircConfInt,
			"hopsMean": hopsMean,
			"hopsConfInt": hopsConfInt,
			"messageSentMean": messageSentMean,
			"messageSentConfInt": messageSentConfInt,
			"slotsWaitedMean": slotsWaitedMean,
			"slotsWaitedConfInt": slotsWaitedConfInt
	}

