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
import graphUtils



scenarios = ["LA", "Padova"]
buildings = [0, 1]
txRanges = [100, 300, 500]

def printSingleGraph(cw, folder, graphTitle, xList, xLabels, yLabel, figureTitle, yDataDictionary, 
					confIntDictionary, protocols, autoscale=False, yBottomLim=0, yTopLim=100):
	print("xList = ")
	print(xList)
	print("xLabels = ")
	print(xLabels)
	print("yLabel = ")
	print(yLabel)
	print("figureTitle = ")
	print(figureTitle)
	print("yDataDictionary = ")
	print(yDataDictionary)
	print("confIntDictionary = ")
	print(confIntDictionary)
	print("protocols = ")
	print(protocols)

	ind = np.arange(len(xList))
	n = len(xList)
	barWidth = float((float(1)/float(4)) * float(0.90))
	fig, ax = plt.subplots()

	rects = []
	count = 0
	colors = ["0.3", "0.5", "0.7"]
	widthDistance = [-1, 0, 1]
	#widthDistance = [-1.5, -0.5, 0.5, 1.5]
	for protocol in protocols:
		print("protocol = " + protocol)
		rects.append((ax.bar(ind + widthDistance[count] * barWidth, yDataDictionary[protocol], barWidth, color=colors[count], label=protocol, yerr=confIntDictionary[protocol], capsize=4)))
		#rects.append((ax.bar(ind + widthDistance[count] * barWidth, yDataDictionary[protocol], barWidth, color=colors[count], label=protocol, capsize=4)))
		count = count + 1

	ax.set_ylabel(yLabel)
	if not autoscale:
		ax.set_ylim(yBottomLim, yTopLim)
	ax.set_title(graphTitle)
	ax.set_xticks(ind)
	ax.set_xticklabels(xLabels)
	#ax.set_xticklabels(["15m", "25m", "35m", "45m"])

	ax.legend(loc="upper right")

	def autolabel(rects, xpos='center'):
		"""
		Attach a text label above each bar in *rects*, displaying its height.

		*xpos* indicates which side to place the text w.r.t. the center of
		the bar. It can be one of the following {'center', 'right', 'left'}.
		"""

		xpos = xpos.lower()  # normalize the case of the parameter
		ha = {'center': 'center', 'right': 'left', 'left': 'right'}
		offset = {'center': 0.5, 'right': 0.57, 'left': 0.43}  # x_txt = x + w*off

		for rect in rects:
			height = rect.get_height()
			ax.text(rect.get_x() + rect.get_width()*offset[xpos], 1.01*height,
					'{}'.format(height), ha=ha[xpos], va='bottom') 

	for rect in rects:
		autolabel(rect)
	#plt.savefig('a1.png')
	#plt.savefig('a2.png', bbox_inches='tight')
	
	outPathDirectory = os.path.join("out", folder)
	outPath = os.path.join(outPathDirectory , figureTitle)
	if (not os.path.exists(outPathDirectory)):
		os.makedirs(outPathDirectory)
	
	plt.savefig(outPath + ".pdf")
	#plt.savefig('b2.pdf', bbox_inches='tight')
	#plt.show()

def initCompoundData(protocols): 
	totCoverageMeans = {}
	totCoverageConfInts = {}
	covOnCircMeans = {}
	covOnCircConfInts = {}
	hopsMeans = {}
	hopsConfInts = {}
	messageSentMeans = {}
	messageSentConfInts = {}

	compoundData = {}
	compoundData["totCoverageMeans"] = totCoverageMeans
	compoundData["totCoverageConfInts"] = totCoverageConfInts
	compoundData["covOnCircMeans"] = covOnCircMeans
	compoundData["covOnCircConfInts"] = covOnCircConfInts
	compoundData["hopsMeans"] = hopsMeans
	compoundData["hopsConfInts"] = hopsConfInts
	compoundData["messageSentMeans"] = messageSentMeans
	compoundData["messageSentConfInts"] = messageSentConfInts

	for protocol in protocols:
		totCoverageMeans[protocol] = []
		totCoverageConfInts[protocol] = []
		covOnCircMeans[protocol] = []
		covOnCircConfInts[protocol] = []
		hopsMeans[protocol] = []
		hopsConfInts[protocol] = []
		messageSentMeans[protocol] = []
		messageSentConfInts[protocol] = []
	return compoundData

def appendCompoundData(basePath, protocols, compoundData):
	for protocol in protocols:
		path = os.path.join(basePath, protocol)
		data = graphUtils.readCsvFromDirectory(path)
		compoundData["totCoverageMeans"][protocol].append(data["totalCoverageMean"])
		compoundData["totCoverageConfInts"][protocol].append(data["totalCovConfInt"])
		compoundData["covOnCircMeans"][protocol].append(data["covOnCircMean"])
		compoundData["covOnCircConfInts"][protocol].append(data["covOnCircConfInt"])
		compoundData["hopsMeans"][protocol].append(data["hopsMean"])
		compoundData["hopsConfInts"][protocol].append(data["hopsConfInt"])
		compoundData["messageSentMeans"][protocol].append(data["messageSentMean"])
		compoundData["messageSentConfInts"][protocol].append(data["messageSentConfInt"])
	return None

def printDistanceComparison(cw, vehicleDistances, protocols, xList, xLabels, figurePrefix, graphTitleExtension, folder):	
	plt.rcParams["figure.figsize"] = [18, 10]
	basePath = os.path.join("/home/jordan/MEGA/Universita_mia/Magistrale/Tesi/ns3-cluster/ns-3.26/out/scenario-urbano-old", cw, "Padova")
	compoundData = initCompoundData(protocols)
	for distance in vehicleDistances:
		basePathWithDistance = os.path.join(basePath, "d" + str(distance), "b1")
		appendCompoundData(basePathWithDistance, protocols, compoundData)
	# Print graphs
	printSingleGraph(cw,
					folder,
					"Padua scenario with buildings, total coverage with varying vehicle distance (" + graphTitleExtension + ")",
					xList,
					xLabels, 
					"Total coverage (%)",
					figurePrefix + "DistanceVsTotalCover", 
					compoundData["totCoverageMeans"],
					compoundData["totCoverageConfInts"],
					protocols)
	printSingleGraph(cw,
					folder,
					"Padua scenario with buildings, coverage on circumference with varying vehicle distance (" + graphTitleExtension + ")", 
					xList,
					xLabels,
					"Coverage on circumference (%)",
					figurePrefix + "DistanceVsCoverOnCircumference",
					compoundData["covOnCircMeans"],
					compoundData["covOnCircConfInts"],
					protocols)
	printSingleGraph(cw,
					folder,
					"Padua scenario with buildings, number of hops with varying vehicle distance (" + graphTitleExtension + ")", 
					xList,
					xLabels,
					"Number of hops",
					figurePrefix + "DistanceVsNumberOfHops",
					compoundData["hopsMeans"],
					compoundData["hopsConfInts"],
					protocols,
					True)
	printSingleGraph(cw,
					folder,
					"Padua scenario with buildings, number of alert messages sent with varying vehicle distance (" + graphTitleExtension + ")",
					xList,
					xLabels, 
					"Number of sent alert messages",
					figurePrefix + "DistanceVsAlertMessagesSent",
					compoundData["messageSentMeans"],
					compoundData["messageSentConfInts"],
					protocols,
					True)

def printCwComparison(cws, vehicleDistance, protocols, xList, xLabels, figurePrefix, graphTitleExtension, folder):	
	plt.rcParams["figure.figsize"] = [18, 10]
	basePath = "/home/jordan/MEGA/Universita_mia/Magistrale/Tesi/ns3-cluster/ns-3.26/out/scenario-urbano-old"
	basePath2 = os.path.join("Padova", "d" + str(vehicleDistance), "b1")
	compoundData = initCompoundData(protocols)
	for cw in cws:
		basePathWithDistance = os.path.join(basePath, cw, basePath2)
		appendCompoundData(basePathWithDistance, protocols, compoundData)
	# Print graphs
	printSingleGraph(cw,
					folder,
					"Padua scenario with buildings, total coverage with varying vehicle distance (" + graphTitleExtension + ")",
					xList,
					xLabels, 
					"Total coverage (%)",
					figurePrefix + "DistanceVsTotalCover", 
					compoundData["totCoverageMeans"],
					compoundData["totCoverageConfInts"],
					protocols)
	printSingleGraph(cw,
					folder,
					"Padua scenario with buildings, coverage on circumference with varying vehicle distance (" + graphTitleExtension + ")", 
					xList,
					xLabels,
					"Coverage on circumference (%)",
					figurePrefix + "DistanceVsCoverOnCircumference",
					compoundData["covOnCircMeans"],
					compoundData["covOnCircConfInts"],
					protocols)
	printSingleGraph(cw,
					folder,
					"Padua scenario with buildings, number of hops with varying vehicle distance (" + graphTitleExtension + ")", 
					xList,
					xLabels,
					"Number of hops",
					figurePrefix + "DistanceVsNumberOfHops",
					compoundData["hopsMeans"],
					compoundData["hopsConfInts"],
					protocols,
					True)
	printSingleGraph(cw,
					folder,
					"Padua scenario with buildings, number of alert messages sent with varying vehicle distance (" + graphTitleExtension + ")",
					xList,
					xLabels, 
					"Number of sent alert messages",
					figurePrefix + "DistanceVsAlertMessagesSent",
					compoundData["messageSentMeans"],
					compoundData["messageSentConfInts"],
					protocols,
					True)

#def prin(cw, protocols, figurePrefix, graphTitleExtension, folder):


if __name__ == "__main__":
	main()