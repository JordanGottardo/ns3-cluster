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


contentionWindows = ["cw-16-128", "cw-32-1024"]
scenarios = ["LA", "Padova"]
vehicleDistances = [15, 25, 35, 45]
buildings = [0, 1]
txRanges = [100, 300, 500]

def printSingleGraph(graphTitle, yLabel, figureTitle, dataDictionary, confIntDictionary, protocols, autoscale=False, yBottomLim=0, yTopLim=100):
	ind = np.arange(len(vehicleDistances))
	n = len(vehicleDistances)
	barWidth = float((float(1)/float(4)) * float(0.90))
	fig, ax = plt.subplots()

	rects = []
	count = 0
	colors = ["0.3", "0.5", "0.7"]
	widthDistance = [-1, 0, 1]
	#widthDistance = [-1.5, -0.5, 0.5, 1.5]
	for protocol in protocols:
		rects.append((ax.bar(ind + widthDistance[count] * barWidth, dataDictionary[protocol], barWidth, color=colors[count], label=protocol, yerr=confIntDictionary[protocol], capsize=4)))
		count = count + 1

	ax.set_ylabel(yLabel)
	if not autoscale:
		ax.set_ylim(yBottomLim, yTopLim)
	ax.set_title(graphTitle)
	ax.set_xticks(ind)
	ax.set_xticklabels(("15m", "25m", "35m", "45m"))

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
	plt.savefig("out/" + figureTitle + ".pdf")
	#plt.savefig('b2.pdf', bbox_inches='tight')
	#plt.show()

def printDistanceVsTotalCover(protocols, figurePrefix, graphTitleExtension):
	# Data to plot
	totCoverageMeans = {}
	totCoverageConfInts = {}
	covOnCircMeans = {}
	covOnCircConfInts = {}
	hopsMeans = {}
	hopsConfInts = {}
	messageSentMeans = {}
	messageSentConfInts = {}

	for protocol in protocols:
		totCoverageMeans[protocol] = []
		totCoverageConfInts[protocol] = []
		
		covOnCircMeans[protocol] = []
		covOnCircConfInts[protocol] = []

		hopsMeans[protocol] = []
		hopsConfInts[protocol] = []

		messageSentMeans[protocol] = []
		messageSentConfInts[protocol] = []



	plt.rcParams["figure.figsize"] = [18,10]
	pathBase = "/home/jordan/MEGA/Universita_mia/Magistrale/Tesi/ns3-cluster/ns-3.26/out/scenario-urbano-old/cw-32-1024/Padova/"
	print("1")
	for distance in vehicleDistances:
		for protocol in protocols:
			path = os.path.join(pathBase, "d" + str(distance), "b1", protocol)
			data = graphUtils.readCsvFromDirectory(path)
			totCoverageMeans[protocol].append(data["totalCoverageMean"])
			totCoverageConfInts[protocol].append(data["totalCovConfInt"])
			covOnCircMeans[protocol].append(data["covOnCircMean"])
			covOnCircConfInts[protocol].append(data["covOnCircConfInt"])
			hopsMeans[protocol].append(data["hopsMean"])
			hopsConfInts[protocol].append(data["hopsConfInt"])
			messageSentMeans[protocol].append(data["messageSentMean"])
			messageSentConfInts[protocol].append(data["messageSentConfInt"])
	print(hopsMeans)
	printSingleGraph("Padua scenario with buildings, total coverage with varying vehicle distance (" + graphTitleExtension + ")", 
					"Total coverage (%)",
					figurePrefix + "DistanceVsTotalCover", 
					totCoverageMeans,
					totCoverageConfInts,
					protocols)
	print("2")
	printSingleGraph("Padua scenario with buildings, coverage on circumference with varying vehicle distance (" + graphTitleExtension + ")", 
					"Coverage on circumference (%)",
					figurePrefix + "DistanceVsCoverOnCircumference",
					covOnCircMeans,
					covOnCircConfInts,
					protocols)

	printSingleGraph("Padua scenario with buildings, number of hops with varying vehicle distance (" + graphTitleExtension + ")", 
					"Number of hops",
					figurePrefix + "DistanceVsNumberOfHops",
					hopsMeans,
					hopsConfInts,
					protocols,
					True)
	print("3")
	printSingleGraph("Padua scenario with buildings, number of alert messages sent with varying vehicle distance (" + graphTitleExtension + ")", 
					"Number of sent alert messages",
					figurePrefix + "DistanceVsAlertMessagesSent",
					messageSentMeans,
					messageSentConfInts,
					protocols,
					True)
	print("4")


if __name__ == "__main__":
	main()