#!/usr/bin/python

import os
import sys
import getopt
import numpy as np
import matplotlib
matplotlib.use("TKAGG")
import matplotlib.pyplot as plt
import string
import shutil
import csv
import scipy.stats as st
import graphUtils
import math
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.ticker import LinearLocator
import numpy as np

def listsToList(listOfLists, protocols):
	toReturn = []
	for protocol in protocols:
		toReturn.append(listOfLists[protocol][0])
	return toReturn

def printSingleGraphLineComparison():
	fig, ax = plt.subplots()
	rects = []
	count = 0
	colors = ["0.3", "0.5"]
	rects.append((ax.bar([10, 20])))
	plt.show()

def printSingleGraphDistance(outFolder, graphTitle, compoundData, distances, protocol, cw, txRange, junctions, metric, xLabel, yLabel, zLabel, minZ, maxZ, txRanges, color):
	fig = plt.figure()
	ax = fig.gca(projection='3d')

	n = len(distances)
	ind = np.arange(n)
	
	barWidth = float((float(1)/float(4)) * float(0.6))
	#fig, ax = plt.subplots()

	rects = []
	count = 0
	X1 = map(int, txRanges)
	Y1 = map(int, distances)
	X, Y = np.meshgrid(X1, Y1)
	Z = []

	junction = "0"
	metricMean = metric + "Mean"
	metricConfInt = metric + "ConfInt"
	for distance in distances:
		resultList = []
		for txRange in txRanges:
			resultList.append(compoundData[distance][junction][txRange][protocol][metricMean])
		Z.append(resultList)

	Z = np.array(Z)
	ax.set_xticks(X1)
	ax.set_yticks(Y1)
	ax.set_xticklabels(X1, fontsize=22)
	ax.set_yticklabels(Y1, fontsize=22)
	#fig.zticks(fontsize=23)
	ax.zaxis.set_tick_params(labelsize=22)

	ax.set_xlabel("\n" + xLabel, fontsize=25, linespacing=3.2)
	ax.set_ylabel("\n" + yLabel, fontsize=25, linespacing=3.2)
	ax.set_zlabel("\n" + zLabel, fontsize=25, linespacing=3.2)
	ax.set_title(graphTitle, fontsize=30)

	if ("cov" in metric or "Cov" in metric):
		maxZ = maxZ * 1.05
	else:
		maxZ = maxZ * 1.1
	ax.set_zlim(minZ, maxZ)

	surf = ax.plot_surface(X, Y, Z, color=color, alpha=0.7)

	plt.show()

	#colors = ["0.3", "0.5", "0.7"]
	#colors = ["0.1", "0.3", "0.5", "0.7","0.9"]
	
	#widthDistance = [-1, 1]
	#widthDistance = [-1.5, -0.5, 0.5, 1.5]
	#widthDistance = [-1, 0, 1]

	#protocolsList = ["Fast-Broadcast", "SJ Fast-Broadcast", "ROFF", "SJ ROFF"]
	'''
	protocolsList = ["Fast-Broadcast", "ROFF"]
	protocolsListMap = {
		"Fast-Broadcast": "Fast-Broadcast",
		"SJ Fast-Broadcast": "Fast-Broadcast",
		"ROFF": "ROFF",
		"SJ ROFF": "ROFF"
	}

	for prot in protocolsList:
		protocol = protocolsListMap[prot]
		metricMeanList = []
		metricConfIntList = []
		for distance in distances:
			junction = None
			if ("SJ" in prot):
				junction = "1"
			else:
				junction = "0"
			metricMean = metric + "Mean"
			metricConfInt = metric + "ConfInt"
			metricMeanList.append(compoundData[distance][junction][txRange][protocol][metricMean] + count )
			metricConfIntList.append(compoundData[distance][junction][txRange][protocol][metricConfInt])
		plt.plot(ind + widthDistance[count] * barWidth, metricMeanList, barWidth, color=colors[count], label=prot)
		count = count + 1
	
	ax.set_xlabel(xLabel, fontsize=15)
	ax.set_ylabel(yLabel, fontsize=15)
	if ("cov" in metric or "Cov" in metric):
		maxY = maxY * 1.05
	else:
		maxY = maxY * 1.1
	ax.set_ylim(minY, maxY)
	#ax.set_title(graphTitle, fontsize=20)
	ax.set_xticks(ind)
	ax.set_xticklabels(distances)
	#ax.set_xticklabels(["15m", "25m", "35m", "45m"])

	ax.legend(loc='center left', bbox_to_anchor=(1, 0.5), fontsize=15)
	#ax.legend(loc="upper right")
	
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
	
	outPathDirectory = os.path.join("out", outFolder + "-" + cw)
	outPath = os.path.join(outPathDirectory , metric) #todo fix
	if (not os.path.exists(outPathDirectory)):
		os.makedirs(outPathDirectory)
	
	plt.savefig(outPath + ".pdf")
	plt.clf()
	#plt.savefig('b2.pdf', bbox_inches='tight')
	#plt.show()
	'''

def printSingleGraphErrorRate(outFolder, graphTitle, compoundData, errorRates, protocols, cw, txRange, junctions, metric, xLabel, yLabel, minY, maxY, colors=["0.3", "0.5"]):
	n = len(errorRates)
	ind = np.arange(n)
	
	barWidth = float((float(1)/float(4)) * float(0.7))
	fig, ax = plt.subplots()

	rects = []
	count = 0
	
	widthDistance = [-1, 1]
	#widthDistance = [-1.5, -0.5, 0.5, 1.5]
	#widthDistance = [-1, 0, 1]

	#protocolsList = ["Fast-Broadcast", "SJ Fast-Broadcast", "ROFF", "SJ ROFF"]
	protocolsList = ["Fast-Broadcast", "ROFF"]
	protocolsListMap = {
		"Fast-Broadcast": "Fast-Broadcast",
		"SJ Fast-Broadcast": "Fast-Broadcast",
		"ROFF": "ROFF",
		"SJ ROFF": "ROFF"
	}

	for prot in protocolsList:
		protocol = protocolsListMap[prot]
		metricMeanList = []
		metricConfIntList = []
		for errorRate in errorRates:
			junction = None
			if ("SJ" in prot):
				junction = "1"
			else:
				junction = "0"
			metricMean = metric + "Mean"
			metricConfInt = metric + "ConfInt"
			metricMeanList.append(compoundData[errorRate][junction][txRange][protocol][metricMean])
			metricConfIntList.append(compoundData[errorRate][junction][txRange][protocol][metricConfInt])
		rects.append((ax.bar(ind + widthDistance[count] * barWidth, metricMeanList, barWidth, color=colors[count], label=prot, yerr=metricConfIntList, 	capsize=4)))
		count = count + 1
	
	ax.set_xlabel(xLabel, fontsize=35)
	ax.set_ylabel(yLabel, fontsize=28)
	if ("cov" in metric or "Cov" in metric):
		maxY = maxY * 1.05
	else:
		maxY = maxY * 1.1
	ax.set_ylim(minY, maxY)
	ax.set_title(graphTitle, fontsize=20)
	ax.set_xticks(ind)
	ax.set_xticklabels(errorRates, fontsize=28)
	plt.yticks(fontsize=23)
	#ax.set_xticklabels(["15m", "25m", "35m", "45m"])

	ax.legend(loc='center left', bbox_to_anchor=(1, 0.5), fontsize=15)
	#ax.legend(loc="upper right")

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
			if (hasattr(height, "is_integer") and height.is_integer()):
				height = int(height)
			ax.text(rect.get_x() + rect.get_width()*offset[xpos], height,
					'{}'.format(height), ha=ha[xpos], va='bottom', fontsize=28) 

	for rect in rects:
		autolabel(rect)
	#plt.savefig('a1.png')
	#plt.savefig('a2.png', bbox_inches='tight')
	
	outPathDirectory = os.path.join("out", outFolder + "-" + cw)
	outPath = os.path.join(outPathDirectory , metric) #todo fix
	if (not os.path.exists(outPathDirectory)):
		os.makedirs(outPathDirectory)
	
	plt.savefig(outPath + ".pdf", bbox_inches='tight')
	plt.clf()
	#plt.savefig('b2.pdf', bbox_inches='tight')
	#plt.show()


def printSingleGraph(outFolder, graphTitle, compoundData, txRanges, protocols, cw, junction, metric, yLabel, minY, maxY, colors=["0.3", "0.5", "0.7"]):
	n = len(protocols)
	#ind = np.arange(n)
	ind = [0.1, 0.9]
	ind = np.array(ind)
	#print(ind)
	
	#barWidth = float((float(1)/float(4)) * float(0.90))
	barWidth = 0.18	
	fig, ax = plt.subplots()
	rects = []
	count = 0
	#colors = ["0.3", "0.7"]
	
	#widthDistance = [-1, 1]
	#widthDistance = [-1.5, -0.5, 0.5, 1.5]
	widthDistance = [-1.025, 0, 1.025]
	ax.set_axisbelow(True)

	for txRange in txRanges:
		metricMeanList = []
		metricConfIntList = []
		for protocol in protocols:
			metricMean = metric + "Mean"
			metricConfInt = metric + "ConfInt"
			metricMeanList.append(compoundData[txRange][protocol][metricMean])
			confInt = compoundData[txRange][protocol][metricConfInt]
			if (math.isnan(confInt)):
				confInt = 0.35
			metricConfIntList.append(confInt)
		#print(ind + widthDistance[count] * barWidth)
		rects.append((ax.bar(ind + widthDistance[count] * barWidth, metricMeanList, barWidth, color=colors[count], label=txRange + "m", yerr=metricConfIntList, capsize=4)))
		count = count + 1
	
	ax.set_xlim(-0.35, 1.35)
	ax.yaxis.grid(alpha=0.25, color="black")

	ax.set_xlabel("Protocols", fontsize=35)
	ax.set_ylabel(yLabel, fontsize=28)
	if ("cov" in metric or "Cov" in metric):
		maxY = maxY * 1.07
	else:
		maxY = maxY * 1.1
	ax.set_ylim(minY - 0.1, maxY)
	#ax.set_title(graphTitle, fontsize=20)
	ax.set_xticks(ind)
	plt.xticks(fontsize=35)
	plt.yticks(fontsize=23)
	myProtocols = protocols
	if junction == "1":
		myProtocols = list(map(lambda x: "SJ-" + x, protocols))
	ax.set_xticklabels(myProtocols)
	#ax.set_xticklabels(["15m", "25m", "35m", "45m"])

	ax.legend(loc='center left', bbox_to_anchor=(1, 0.5), fontsize=17)
	#ax.legend(loc="upper right")

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
			if (hasattr(height, "is_integer") and height.is_integer()):
				height = int(height)
			ax.text(rect.get_x() + rect.get_width()*offset[xpos], height,
					'{}'.format(height), ha=ha[xpos], va='bottom', fontsize=25) 

	for rect in rects:
		autolabel(rect)
	#plt.savefig('a1.png')
	#plt.savefig('a2.png', bbox_inches='tight')
	
	outPathDirectory = os.path.join("out", outFolder + "-" + cw)
	outPath = os.path.join(outPathDirectory , metric) #todo fix
	if (not os.path.exists(outPathDirectory)):
		os.makedirs(outPathDirectory)
	
	#plt.tight_layout(pad=10.0)
	plt.savefig(outPath + ".pdf")
	plt.clf()
	#plt.savefig('b2.pdf', bbox_inches='tight')
	#plt.show()

# inits an object like this: compoundData["txRange"]["protocol"]["metric"]
def initCompoundData(txRanges, protocols, metrics): 
	compoundData = {}
	for txRange in txRanges:
		compoundData[txRange] = {}
		for protocol in protocols:
			for metric in metrics:
				metricMean = metric + "Mean"
				metricConfInt = metric + "ConfInt"
				compoundData[txRange][protocol] = {}
				compoundData[txRange][protocol][metricMean] = 0
				compoundData[txRange][protocol][metricConfInt] = 0
	return compoundData

def appendCompoundData(basePath, txRanges, protocols, cw, junction, errorRate, compoundData, metrics, alternativeBasePath=""):
	for txRange in txRanges:
		for protocol in protocols:
			path = None
			roff = False
			static = False
			realBasePath = basePath
			protocolPath = protocol
			if "TD" in protocol:
				realBasePath = alternativeBasePath
				protocolPath = protocol.replace("TD-", "")
			if ("STATIC" in protocol and txRange not in protocol):
				static = True
			if (protocol != "ROFF"):
				path = os.path.join(realBasePath, errorRate, "r" + txRange, "j" + junction, cw, protocolPath)
			else: 
				roff = True
				path = os.path.join(realBasePath, errorRate, "r" + txRange, "j" + junction, protocolPath)
			data = graphUtils.readCsvFromDirectory(path, roff, static)
			entry = compoundData[txRange][protocol]
			for metric in metrics:
				metricMean = metric + "Mean"
				metricConfInt = metric + "ConfInt"
				compoundData[txRange][protocol][metricMean] =  data[metricMean]
				compoundData[txRange][protocol][metricConfInt] =  data[metricConfInt]
	return None


def printLineComparison():
	protocols = ["fast-broadcast", "roff"]
	compoundData = initCompoundData(protocols)
	basePath = "/home/jordan/MEGA/Universita_mia/Magistrale/Tesi/ns3-cluster/ns-3.26/out/line"
	appendCompoundData(basePath, protocols, compoundData)
	printSingleGraphLineComparison()

def printGridComparison():
	print("PrintGridComparison")
	protocols = ["fast-broadcast", "roff"]
	compoundDatab0 = initCompoundData(protocols)
	compoundDatab1 = initCompoundData(protocols)
	basePathb0 = "/home/jordan/MEGA/Universita_mia/Magistrale/Tesi/ns3-cluster/ns-3.26/out/Grid/b0"
	basePathb1 = "/home/jordan/MEGA/Universita_mia/Magistrale/Tesi/ns3-cluster/ns-3.26/out/Grid/b1"
	appendCompoundData(basePathb0, protocols, compoundDatab0)
	appendCompoundData(basePathb1, protocols, compoundDatab1)

	#printSingleGraphLineComparison()

#Given a scenario path and buildings/nobuildings, prints graphs for all txRanges and protocols
#Grid-300: contentionWindows = [{"cwMin": 16, "cwMax": 128}], buildings = ["0"], junctions = ["0"], txRanges = ["100", "300", "500"]
def printProtocolComparison():
	print("PrintProtocolComparison")
	#plt.rcParams["figure.figsize"] = [18, 6]
	plt.rcParams["figure.figsize"] = [18, 14]
	initialBasePath = "/home/jordan/MEGA/Universita_mia/Magistrale/Tesi/ns3-cluster/ns-3.26/out/scenario-urbano"
	#scenarios = ["Grid-200", "Grid-300", "Grid-400", "LA-15", "LA-25", "LA-35", "LA-45", "Padova-15", "Padova-25", "Padova-35", "Padova-45"]
	scenarios = ["LA-25"]
	buildings = ["0", "1"]
	errorRate = "e0"
	#txRanges = ["100", "300", "500"]
	txRanges = ["100", "300", "500"]
	#protocols = ["Fast-Broadcast", "STATIC-100", "STATIC-300", "STATIC-500", "ROFF"]
	protocols = ["Fast-Broadcast", "ROFF"]
	cws = ["cw[32-1024]"]
	#cws = ["cw[16-128]", "cw[32-1024]"]
	#junctions = ["0", "1"]
	junctions = ["0", "1"]
	metrics = ["totCoverage", "covOnCirc", "hops", "slotsWaited", "messageSent"]
	metricYLabels = {}
	metricYLabels["totCoverage"] = "Total Delivery Ratio (%)"
	metricYLabels["covOnCirc"] = "Total Delivery Ratio On Circ. (%)"
	metricYLabels["hops"] = "Number Of Hops"
	metricYLabels["slotsWaited"] = "Number Of Slots"
	metricYLabels["messageSent"] = "Forwarding Node Number"
	
	graphTitles = {}
	graphTitles["totCoverage"] = "Total Delivery Ratio"
	graphTitles["covOnCirc"] = "Total Delivery Ratio On Circumference"
	graphTitles["hops"] = "Number Of Hops"
	graphTitles["slotsWaited"] = "Number Of Slots"
	graphTitles["messageSent"] = "Forwarding Node Number"

	additionalTitle = {}
	additionalTitle["0"] = {} #no buildings
	additionalTitle["0"]["0"] = " (without buildings, without junctions)"
	additionalTitle["0"]["1"] = " (without buildings, with junctions)"


	additionalTitle["1"] = {} #with buildings
	additionalTitle["1"]["0"] = " (with buildings, without junctions)" 
	additionalTitle["1"]["1"] = " (with buildings, with junctions)" 

	maxMetricValues = {}
	for metric in metrics:
		maxMetricValues[metric] = -1

	colors = {}
	colors["0"] = {} 
	colors["0"]["0"] = ["#B5B7FF", "#5155D5", "#00034D"] #buildings=0, junctions=0 blu
	#colors["0"]["1"] = ["#9EDE9E", "#368B36", "#003C00"] #buildings=0, junctions=1 verde

	colors["0"]["1"] = ["#D1AFD1", "#864A89", "#3C003F"] #buildings=0, junctions=1 viola

	colors["1"] = {} # 1=buildings
	colors["1"]["0"] = ["#FFA6A6", "#BD2525", "#510000"] #buildings=1, junctions=0 rosso
	colors["1"]["1"] = ["#FFC497", "#c27230", "#7a3806"] #buildings=1, junctions=1 arancione

	

	for scenario in scenarios:
		if ("Platoon" in scenario):
			buildings = ["0"]
		for building in buildings:
			for cw in cws:
				for junction in junctions:
					basePath = os.path.join(initialBasePath, scenario, "b" + building)
					compoundData = initCompoundData(txRanges, protocols, metrics)
					appendCompoundData(basePath, txRanges, protocols, cw, junction, errorRate, compoundData, metrics)
					graphOutFolder = os.path.join(scenario, "b" + building, "j" + junction)
					for metric in metrics:
						yLabel = metricYLabels[metric]
						if (metric == "totCoverage" or metric == "covOnCirc"):
							maxMetricValues[metric] = 100
						else:
							for txRange in txRanges:
								for protocol in protocols:
									metricMean = metric + "Mean"
									value = compoundData[txRange][protocol][metricMean] 
									if ( value > maxMetricValues[metric]):
										maxMetricValues[metric] = value

	for scenario in scenarios:
		for building in buildings:
			for cw in cws:
				for junction in junctions:
					basePath = os.path.join(initialBasePath, scenario, "b" + building)
					compoundData = initCompoundData(txRanges, protocols, metrics)
					appendCompoundData(basePath, txRanges, protocols, cw, junction, errorRate, compoundData, metrics)
					graphOutFolder = os.path.join(scenario, "b" + building, "j" + junction)
					for metric in metrics:
						yLabel = metricYLabels[metric]
						additionalTitle = ""
						#additionalTitle = additionalTitle[building][junction]
						printSingleGraph(graphOutFolder, graphTitles[metric] + additionalTitle, compoundData, txRanges, protocols, cw, junction, metric, yLabel, 0, maxMetricValues[metric], 
						colors[building][junction])


def printDroneComparison():
	print("PrintDroneComparison")
	plt.rcParams["figure.figsize"] = [18, 14]
	initialBasePath = "/home/jordan/MEGA/Universita_mia/Magistrale/Tesi/ns3-cluster/ns-3.26/out/scenario-droni"
	#scenarios = ["Grid-200", "Grid-300", "Grid-400", "LA-15", "LA-25", "LA-35", "LA-45", "Padova-15", "Padova-25", "Padova-35", "Padova-45"]
	scenarios = ["LA-25"]
	highBuildings = ["0", "1"]
	buildings = ["0", "1"]
	errorRate = "e0"
	#txRanges = ["100", "300", "500"]
	txRanges = ["100", "300", "500"]
	#protocols = ["Fast-Broadcast", "STATIC-100", "STATIC-300", "STATIC-500", "ROFF"]
	protocols = ["Fast-Broadcast", "ROFF"]
	#cws = ["cw[32-1024]"]
	cws = ["cw[32-1024]"]
	#junctions = ["0", "1"]
	junctions = ["0"]
	metrics = ["totCoverage", "covOnCirc", "hops", "slotsWaited", "messageSent"]
	metricYLabels = {}
	metricYLabels["totCoverage"] = "Total Delivery Ratio (%)"
	metricYLabels["covOnCirc"] = "Total Delivery Ratio On Circ. (%)"
	metricYLabels["hops"] = "Number Of Hops"
	metricYLabels["slotsWaited"] = "Number Of Slots"
	metricYLabels["messageSent"] = "Forwarding Node Number"
	
	graphTitles = {}
	graphTitles["totCoverage"] = "Total Delivery Ratio"
	graphTitles["covOnCirc"] = "Total Delivery Ratio On Circumference"
	graphTitles["hops"] = "Number Of Hops"
	graphTitles["slotsWaited"] = "Number Of Slots"
	graphTitles["messageSent"] = "Forwarding Node Number"

	additionalTitle = {}
	additionalTitle["0"] = {} #no buildings
	additionalTitle["0"]["0"] = " (without buildings)" #no buildings, no highBuildings


	additionalTitle["1"] = {} #with buildings
	additionalTitle["1"]["0"] = " (with buildings, real heights)" 
	additionalTitle["1"]["1"] = " (with buildings, 100m heights)"
	
	colors = {}
	colors["0"] = {} 
	colors["0"]["0"] = ["#B5B7FF", "#5155D5", "#00034D"] #buildings=0, highBuildings=0 blu

	colors["1"] = {} # 1=buildings
	colors["1"]["0"] = ["#FFA6A6", "#BD2525", "#510000"] #buildings=1, highBuildings=0 rosso
	colors["1"]["1"] = ["#D1AFD1", "#864A89", "#3C003F"] #buildings=1, highBuildings=1 viola


	maxMetricValues = {}
	for metric in metrics:
		maxMetricValues[metric] = -1

	
	for scenario in scenarios:
		myBuildings = buildings
		myInitialBasePath = initialBasePath
		if ("Platoon" in scenario):
			myBuildings = ["0"]
		for highBuilding in highBuildings:
			if (highBuilding == "1"):
				myInitialBasePath += "-high"
				myBuildings = ["1"]
			for building in myBuildings:
				for cw in cws:
					for junction in junctions:
						basePath = os.path.join(myInitialBasePath, scenario, "b" + building)
						compoundData = initCompoundData(txRanges, protocols, metrics)
						appendCompoundData(basePath, txRanges, protocols, cw, junction, errorRate, compoundData, metrics)
						graphOutFolder = os.path.join(scenario, "b" + building, "j" + junction)
						for metric in metrics:
							yLabel = metricYLabels[metric]
							if (metric == "totCoverage" or metric == "covOnCirc"):
								maxMetricValues[metric] = 100
							else:
								for txRange in txRanges:
									for protocol in protocols:
										metricMean = metric + "Mean"
										value = compoundData[txRange][protocol][metricMean] 
										if ( value > maxMetricValues[metric]):
											maxMetricValues[metric] = value

	for scenario in scenarios:
		myBuildings = buildings
		myInitialBasePath = initialBasePath
		for highBuilding in highBuildings:
			if (highBuilding == "1"):
				myInitialBasePath += "-high"
				myBuildings = ["1"]
			for building in myBuildings:
				for cw in cws:
					for junction in junctions:
						basePath = os.path.join(myInitialBasePath, scenario, "b" + building)
						compoundData = initCompoundData(txRanges, protocols, metrics)
						appendCompoundData(basePath, txRanges, protocols, cw, junction, errorRate, compoundData, metrics)
						graphOutFolder = os.path.join(scenario, "drones", "b" + building + "-h" + highBuilding)
						for metric in metrics:
							yLabel = metricYLabels[metric]
							print("before printSingleGraph h=" + highBuilding + " b=" + building)
							printSingleGraph(graphOutFolder, graphTitles[metric] + additionalTitle[building][highBuilding], compoundData, txRanges, protocols, cw, junction, metric, yLabel, 0, maxMetricValues[metric], colors[building][highBuilding])



def printErrorComparison():
	print("PrintErrorComparison")
	plt.rcParams["figure.figsize"] = [18, 10]
	initialBasePath = "/home/jordan/MEGA/Universita_mia/Magistrale/Tesi/ns3-cluster/ns-3.26/out/scenario-urbano"
	#scenarios = ["Grid-200", "Grid-300", "Grid-400", "LA-15", "LA-25", "LA-35", "LA-45", "Padova-15", "Padova-25", "Padova-35", "Padova-45"]
	scenarios = ["Padova-25"]
	buildings = ["0" , "1"]
	txRanges = ["100", "300", "500"]
	protocols = ["Fast-Broadcast", "ROFF"]
	#cws = ["cw[16-128]", "cw[32-1024]"]
	cws = ["cw[16-128]"]
	errorRates = ["0", "10", "20", "30", "40", "50", "100"]
	junctions = ["0", "1"]
	xLabel = "Error in scheduling (%)"
	metrics = ["totCoverage", "covOnCirc", "hops", "slotsWaited", "messageSent"]
	metricYLabels = {}
	metricYLabels["totCoverage"] = "Total Coverage (%)"
	metricYLabels["covOnCirc"] = "Coverage on circumference (%)"
	metricYLabels["hops"] = "Number of hops to reach circumference"
	metricYLabels["slotsWaited"] = "Number of slots waited to reach circumference"
	metricYLabels["messageSent"] = "Number of alert messages sent"
	
	maxMetricValues = {}
	for metric in metrics:
		maxMetricValues[metric] = -1

	for scenario in scenarios:
		for building in buildings:
			for cw in cws:
				errorRateCompoundData = {}
				for errorRate in errorRates:
					errorRateCompoundData[errorRate] = {}
				for junction in junctions:
					for errorRate in errorRates:
						basePath = os.path.join(initialBasePath, scenario, "b" + building)
						#print("basePath= " + basePath)
						compoundData = initCompoundData(txRanges, protocols, metrics)
						appendCompoundData(basePath, txRanges, protocols, cw, junction, "e" + errorRate, compoundData, metrics)
						errorRateCompoundData[errorRate][junction] = compoundData
				for metric in metrics:
					yLabel = metricYLabels[metric]
					if (metric == "totCoverage" or metric == "covOnCirc"):
							maxMetricValues[metric] = 100
					else:
						for junction in junctions:
							for errorRate in errorRates:
								for txRange in txRanges:
									for protocol in protocols:
										metricMean = metric + "Mean"
										value = errorRateCompoundData[errorRate][junction][txRange][protocol][metricMean] 
										if (value > maxMetricValues[metric]):
											maxMetricValues[metric] = value

	for scenario in scenarios:
		for building in buildings:
			for cw in cws:
				errorRateCompoundData = {}
				for errorRate in errorRates:
					errorRateCompoundData[errorRate] = {}
				for junction in junctions:
					for errorRate in errorRates:
						basePath = os.path.join(initialBasePath, scenario, "b" + building)
						#print("basePath= " + basePath)
						compoundData = initCompoundData(txRanges, protocols, metrics)
						appendCompoundData(basePath, txRanges, protocols, cw, junction, "e" + errorRate, compoundData, metrics)
						errorRateCompoundData[errorRate][junction] = compoundData
				graphOutFolder = os.path.join(scenario, "error", "b" + building)
				for metric in metrics:
					yLabel = metricYLabels[metric]
					printSingleGraphErrorRate(graphOutFolder, "graphTitle", errorRateCompoundData, errorRates, protocols, cw, "100", junctions, metric, xLabel, yLabel, 0, maxMetricValues[metric])



def printForgedComparison():
	print("PrintForgedComparison")
	plt.rcParams["figure.figsize"] = [18, 10]
	initialBasePath = "/home/jordan/MEGA/Universita_mia/Magistrale/Tesi/ns3-cluster/ns-3.26/out/scenario-urbano"
	#scenarios = ["Grid-200", "Grid-300", "Grid-400", "LA-15", "LA-25", "LA-35", "LA-45", "Padova-15", "Padova-25", "Padova-35", "Padova-45"]
	scenarios = ["LA-25"]
	buildings = ["0"]
	#txRanges = ["100", "300", "500"]
	txRanges = ["300"]
	protocols = ["Fast-Broadcast", "ROFF"]
	cws = ["cw[32-1024]"]
	#cws = ["cw[16-128]"]
	forgedRates = ["0", "10", "20", "30", "40", "50"]
	junctions = ["0"]
	xLabel = "% of vehicles affected by forging"
	metrics = ["totCoverage", "covOnCirc", "hops", "slotsWaited", "messageSent"]
	metricYLabels = {}
	metricYLabels["totCoverage"] = "Total Delivery Ratio (%)"
	metricYLabels["covOnCirc"] = "Total Delivery Ratio On Circ. (%)"
	metricYLabels["hops"] = "Number Of Hops"
	metricYLabels["slotsWaited"] = "Number Of Slots"
	metricYLabels["messageSent"] = "Forwarding Node Number"
	
	graphTitles = {}
	graphTitles["totCoverage"] = "Total Delivery Ratio"
	graphTitles["covOnCirc"] = "Total Delivery Ratio On Circumference"
	graphTitles["hops"] = "Number Of Hops"
	graphTitles["slotsWaited"] = "Number Of Slots"
	graphTitles["messageSent"] = "Forwarding Node Number"

	colors = ["#B5B7FF", "#5155D5"]

	maxMetricValues = {}
	for metric in metrics:
		maxMetricValues[metric] = -1

	for scenario in scenarios:
		for building in buildings:
			for cw in cws:
				forgedRateCompoundData = {}
				for forgedRate in forgedRates:
					forgedRateCompoundData[forgedRate] = {}
				for junction in junctions:
					for forgedRate in forgedRates:
						basePath = os.path.join(initialBasePath, scenario, "b" + building)
						compoundData = initCompoundData(txRanges, protocols, metrics)
						appendCompoundData(basePath, txRanges, protocols, cw, junction, "f" + forgedRate, compoundData, metrics)
						forgedRateCompoundData[forgedRate][junction] = compoundData
				for metric in metrics:
					yLabel = metricYLabels[metric]
					if (metric == "totCoverage" or metric == "covOnCirc"):
							maxMetricValues[metric] = 100
					else:
						for junction in junctions:
							for forgedRate in forgedRates: 
								for txRange in txRanges:
									for protocol in protocols:
										metricMean = metric + "Mean"
										value = forgedRateCompoundData[forgedRate][junction][txRange][protocol][metricMean] 
										if ( value > maxMetricValues[metric]):
											maxMetricValues[metric] = value

	for scenario in scenarios:
		for building in buildings:
			for cw in cws:
				forgedRateCompoundData = {}
				for forgedRate in forgedRates:
					forgedRateCompoundData[forgedRate] = {}
				for junction in junctions:
					for forgedRate in forgedRates:
						basePath = os.path.join(initialBasePath, scenario, "b" + building)
						#print("basePath= " + basePath)
						compoundData = initCompoundData(txRanges, protocols, metrics)
						appendCompoundData(basePath, txRanges, protocols, cw, junction, "f" + forgedRate, compoundData, metrics)
						forgedRateCompoundData[forgedRate][junction] = compoundData
				graphOutFolder = os.path.join(scenario, "forged", "b" + building)
				for metric in metrics:
					yLabel = metricYLabels[metric]
					printSingleGraphErrorRate(graphOutFolder, graphTitles[metric], forgedRateCompoundData, forgedRates, protocols, cw, "300", junctions, metric, xLabel, yLabel, 0, maxMetricValues[metric], colors)

def printDistanceComparison():
	print("PrintDistanceComparison")
	plt.rcParams["figure.figsize"] = [18, 10]
	initialBasePath = "/home/jordan/MEGA/Universita_mia/Magistrale/Tesi/ns3-cluster/ns-3.26/out/scenario-urbano"
	#scenarios = ["Grid-200", "Grid-300", "Grid-400", "LA-15", "LA-25", "LA-35", "LA-45", "Padova-15", "Padova-25", "Padova-35", "Padova-45"]
	scenarios = ["Padova"]
	#distances = ["15", "25", "35", "45"]
	distances = ["5", "15", "25", "35", "45"]
	buildings = ["0"]
	txRanges = ["100", "300", "500"]
	protocols = ["Fast-Broadcast", "ROFF"]
	errorRate = "e0"
	#cws = ["cw[16-128]", "cw[32-1024]"]
	cws = ["cw[32-1024]"]
	junctions = ["0"]
	xLabel = "Vehicle distance (m)"
	yOrigLabel = "Transmission range (m)"
	metrics = ["totCoverage", "covOnCirc", "hops", "slotsWaited", "messageSent"]
	metricYLabels = {}
	metricYLabels["totCoverage"] = "Total Delivery Ratio (%)"
	metricYLabels["covOnCirc"] = "Total Delivery Ratio On Circ. (%)"
	metricYLabels["hops"] = "Number Of Hops"
	metricYLabels["slotsWaited"] = "Number Of Slots"
	metricYLabels["messageSent"] = "Forwarding Node Number"
	
	colors = {}
	colors["Fast-Broadcast"] = "#5155D5"
	colors["ROFF"] = "#BD2525"

	#chiari
	#colors["Fast-Broadcast"] = "#B5B7FF"
	#colors["ROFF"] = "#FFA6A6"

	maxMetricValues = {}

	for scenario in scenarios:
		for building in buildings:
			for cw in cws:
				distanceCompoundData = {}
				for distance in distances:
					distanceCompoundData[distance] = {}
				for junction in junctions:
					for distance in distances:
						basePath = os.path.join(initialBasePath, scenario + "-" + distance, "b" + building)
						compoundData = initCompoundData(txRanges, protocols, metrics)
						appendCompoundData(basePath, txRanges, protocols, cw, junction, errorRate, compoundData, metrics)
						distanceCompoundData[distance][junction] = compoundData
				for metric in metrics:
					yLabel = metricYLabels[metric]
					maxMetricValues[metric] = -1
					if (metric == "totCoverage" or metric == "covOnCirc"):
							maxMetricValues[metric] = 100
					else:
						for junction in junctions:
							for distance in distances: 
								for txRange in txRanges:
									for protocol in protocols:
										metricMean = metric + "Mean"
										value = distanceCompoundData[distance][junction][txRange][protocol][metricMean] 
										if ( value > maxMetricValues[metric]):
											maxMetricValues[metric] = value

	for scenario in scenarios:
		for building in buildings:
			for cw in cws:
				distanceCompoundData = {}
				for distance in distances:
					distanceCompoundData[distance] = {}
				for junction in junctions:
					for distance in distances:
						basePath = os.path.join(initialBasePath, scenario + "-" + distance, "b" + building)
						compoundData = initCompoundData(txRanges, protocols, metrics)
						appendCompoundData(basePath, txRanges, protocols, cw, junction, errorRate, compoundData, metrics)
						distanceCompoundData[distance][junction] = compoundData
				graphOutFolder = os.path.join(scenario, "distance", "b" + building) #todo aggiungere cw nel out path?
				for protocol in protocols:
					for metric in metrics:
						yLabel = metricYLabels[metric]
						#print(distanceCompoundData)
						graphTitle = metricYLabels[metric] + " (" + protocol + " )"
						printSingleGraphDistance(graphOutFolder, graphTitle, distanceCompoundData, distances, protocol, cw, "500", junctions, metric, yOrigLabel, xLabel , metricYLabels[metric], 0, maxMetricValues[metric], txRanges, colors[protocol])

def printOldFBComparison():
	print("PrintOldFBComparison")
	plt.rcParams["figure.figsize"] = [18, 6]
	initialBasePath = "/home/jordan/MEGA/Universita_mia/Magistrale/Tesi/ns3-cluster/ns-3.26/out/scenario-urbano"
	alternativeInitialBasePath = "/home/jordan/MEGA/Universita_mia/Magistrale/Tesi/ns3-cluster/ns-3.26/out/scenario-urbano-oldFB"
	#scenarios = ["Grid-200", "Grid-300", "Grid-400", "LA-15", "LA-25", "LA-35", "LA-45", "Padova-15", "Padova-25", "Padova-35", "Padova-45"]
	scenarios = ["LA-25"]
	buildings = ["1"]
	errorRate = "e0"
	#txRanges = ["100", "300", "500"]
	txRanges = ["100", "300", "500"]
	#protocols = ["Fast-Broadcast", "STATIC-100", "STATIC-300", "STATIC-500", "ROFF"]
	protocols = ["Fast-Broadcast", "STATIC-100", "STATIC-300", "STATIC-500","TD-Fast-Broadcast", "TD-STATIC-100", "TD-STATIC-300", "TD-STATIC-500"]
	cws = ["cw[32-1024]"]
	#cws = ["cw[16-128]", "cw[32-1024]"]
	junctions = ["0"]
	tds = ["0", "1"]
	#junctions = ["0"]
	metrics = ["totCoverage", "covOnCirc", "hops", "slotsWaited", "messageSent"]
	metricYLabels = {}
	metricYLabels["totCoverage"] = "Total Delivery Ratio (%)"
	metricYLabels["covOnCirc"] = "Total Delivery Ratio On Circ. (%)"
	metricYLabels["hops"] = "Number Of Hops"
	metricYLabels["slotsWaited"] = "Number Of Slots"
	metricYLabels["messageSent"] = "Forwarding Node Number"
	
	graphTitles = {}
	graphTitles["totCoverage"] = "Total Delivery Ratio"
	graphTitles["covOnCirc"] = "Total Delivery Ratio On Circumference"
	graphTitles["hops"] = "Number Of Hops"
	graphTitles["slotsWaited"] = "Number Of Slots"
	graphTitles["messageSent"] = "Forwarding Node Number"

	additionalTitle = {}
	additionalTitle["0"] = {} #no buildings
	additionalTitle["0"]["0"] = " (without buildings)"
	additionalTitle["0"]["1"] = " (without buildings)"


	additionalTitle["1"] = {} #with buildings
	additionalTitle["1"]["0"] = " (with buildings)" 
	additionalTitle["1"]["1"] = " (with buildings)" 

	maxMetricValues = {}
	for metric in metrics:
		maxMetricValues[metric] = -1
 
	colors = {}
	colors["0"] = ["#B5B7FF", "#5155D5", "#00034D"] # td0 blu
	colors["1"] = ["#FFA6A6", "#BD2525", "#510000"] # td1 rosso

	for scenario in scenarios:
		if ("Platoon" in scenario):
			buildings = ["0"]
		for building in buildings:
			for cw in cws:
				for junction in junctions:
						basePath = os.path.join(initialBasePath, scenario, "b" + building)
						alternativeBasePath = os.path.join(alternativeInitialBasePath, scenario, "b" + building)
						compoundData = initCompoundData(txRanges, protocols, metrics)
						appendCompoundData(basePath, txRanges, protocols, cw, junction, errorRate, compoundData, metrics, alternativeBasePath)
						graphOutFolder = os.path.join(scenario, "b" + building, "j" + junction)
						for metric in metrics:
							yLabel = metricYLabels[metric]
							if (metric == "totCoverage" or metric == "covOnCirc"):
								maxMetricValues[metric] = 100
							else:
								for txRange in txRanges:
									for protocol in protocols:
										metricMean = metric + "Mean"
										value = compoundData[txRange][protocol][metricMean] 
										if ( value > maxMetricValues[metric]):
											maxMetricValues[metric] = value

	for scenario in scenarios:
		for building in buildings:
			for cw in cws:
				for junction in junctions:
					basePath = os.path.join(initialBasePath, scenario, "b" + building)
					alternativeBasePath = os.path.join(alternativeInitialBasePath, scenario, "b" + building)
					compoundData = initCompoundData(txRanges, protocols, metrics)
					appendCompoundData(basePath, txRanges, protocols, cw, junction, errorRate, compoundData, metrics, alternativeBasePath)
					for metric in metrics:
						for td in tds:
							graphOutFolder = os.path.join(scenario + "-old-fb", "td-" + td)
							myProtocols = filter(lambda x: ((td == "0" and "TD" not in x) or (td == "1" and "TD" in x)), protocols)
							yLabel = metricYLabels[metric]
							printSingleGraph(graphOutFolder, graphTitles[metric] + additionalTitle[building][junction], compoundData, txRanges, myProtocols, cw, junction, metric, yLabel, 0, maxMetricValues[metric], 
							colors[td])



if __name__ == "__main__":
	main()



'''
def printDistanceComparison(cw, vehicleDistances, protocols, xList, xLabels, figurePrefix, graphTitleExtension, folder, decreaseConfInts=False):	
	plt.rcParams["figure.figsize"] = [18, 10]
	basePath = os.path.join("/home/jordan/MEGA/Universita_mia/Magistrale/Tesi/ns3-cluster/ns-3.26/out/scenario-urbano", cw, "Padova")
	xLabel = "Vehicle distance"
	compoundData = initCompoundData(protocols)
	for distance in vehicleDistances:
		basePathWithDistance = os.path.join(basePath, "d" + str(distance), "b1")
		appendCompoundData(basePathWithDistance, protocols, compoundData, decreaseConfInts)
	# Print graphs
	printSingleGraph(cw,
					folder,
					"Padua scenario with buildings, total coverage with varying vehicle distance (" + graphTitleExtension + ")",
					xList,
					xLabels, 
					xLabel, 
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
					xLabel, 
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
					xLabel, 
					"Number of hops",
					figurePrefix + "DistanceVsNumberOfHops",
					compoundData["hopsMeans"],
					compoundData["hopsConfInts"],
					protocols,
					False, 
					0,
					20)
	printSingleGraph(cw,
					folder,
					"Padua scenario with buildings, number of alert messages sent with varying vehicle distance (" + graphTitleExtension + ")",
					xList,
					xLabels, 
					xLabel, 
					"Number of sent alert messages",
					figurePrefix + "DistanceVsAlertMessagesSent",
					compoundData["messageSentMeans"],
					compoundData["messageSentConfInts"],
					protocols,
					False,
					0,
					250)
	printSingleGraph(cw,
					folder,
					"Padua scenario with buildings, number of slots waited with varying vehicle distance (" + graphTitleExtension + ")",
					xList,
					xLabels, 
					xLabel, 
					"Number of slots waited",
					figurePrefix + "DistanceVsSlotsWaited",
					compoundData["slotsWaitedMeans"],
					compoundData["slotsWaitedConfInts"],
					protocols,
					True)

def printCwComparison(cws, vehicleDistance, protocols, xList, xLabels, figurePrefix, graphTitleExtension, folder, decreaseConfInts=False):	
	plt.rcParams["figure.figsize"] = [18, 10]
	basePath = "/home/jordan/MEGA/Universita_mia/Magistrale/Tesi/ns3-cluster/ns-3.26/out/scenario-urbano"
	basePath2 = os.path.join("Padova", "d" + str(vehicleDistance), "b1")
	compoundData = initCompoundData(protocols)
	xLabel = "Contention window"
	for cw in cws:
		basePathWithDistance = os.path.join(basePath, cw, basePath2)
		appendCompoundData(basePathWithDistance, protocols, compoundData, decreaseConfInts)
	# Print graphs
	printSingleGraph(cw,
					folder,
					"Padua scenario with buildings, total coverage with varying contention window (" + graphTitleExtension + ")",
					xList,
					xLabels, 
					xLabel,
					"Total coverage (%)",
					figurePrefix + "CwVsTotalCover", 
					compoundData["totCoverageMeans"],
					compoundData["totCoverageConfInts"],
					protocols)
	printSingleGraph(cw,
					folder,
					"Padua scenario with buildings, coverage on circumference with varying contention window (" + graphTitleExtension + ")", 
					xList,
					xLabels,
					xLabel,
					"Coverage on circumference (%)",
					figurePrefix + "CwVsCoverOnCircumference",
					compoundData["covOnCircMeans"],
					compoundData["covOnCircConfInts"],
					protocols)
	printSingleGraph(cw,
					folder,
					"Padua scenario with buildings, number of hops with varying contention window (" + graphTitleExtension + ")", 
					xList,
					xLabels,
					xLabel,
					"Number of hops",
					figurePrefix + "CwVsNumberOfHops",
					compoundData["hopsMeans"],
					compoundData["hopsConfInts"],
					protocols,
					False,
					0,
					20)
	printSingleGraph(cw,
					folder,
					"Padua scenario with buildings, number of alert messages sent with varying contention window (" + graphTitleExtension + ")",
					xList,
					xLabels, 
					xLabel,
					"Number of sent alert messages",
					figurePrefix + "CwVsAlertMessagesSent",
					compoundData["messageSentMeans"],
					compoundData["messageSentConfInts"],
					protocols,
					False,
					0,
					160)
	printSingleGraph(cw,
					folder,
					"Padua scenario with buildings, number of slots waited with varying contention window (" + graphTitleExtension + ")",
					xList,
					xLabels, 
					xLabel, 
					"Number of slots waited",
					figurePrefix + "CwVsSlotsWaited",
					compoundData["slotsWaitedMeans"],
					compoundData["slotsWaitedConfInts"],
					protocols,
					True)
 
def printRomanelliComparison(cw, vehicleDistance, protocols, xList, xLabels, figurePrefix, graphTitleExtension, folder, basePath, decreaseConfInts=False):
	plt.rcParams["figure.figsize"] = [18, 10]
	buildings = ["0", "1"]
	for b in buildings:
		actualFolder = os.path.join(folder, "b" + b)
		print("folder = ")
		print(folder)
		print("actual folder = ")
		print(actualFolder)
		actualBasePath = os.path.join(basePath, cw, "Padova/d25/", "b" + b)
		xLabel = "Protocol-transmission range"
		compoundData = initCompoundData(protocols)
		appendCompoundData(actualBasePath, protocols, compoundData, decreaseConfInts)
		romTotCov = [45.49, 94.35, 47.45, 56.93, 50.30, 94.11]
		romCovCirc = [23.81, 94.75, 22.06, 64.80, 27.78, 93.88]
		romNumHops = [7.30, 2.14, 6.62, 3.41, 7.57, 2.07]
		romAlertSent = [219, 109, 236, 45, 253, 107]
		romSlotsWaited = [0, 0, 0, 0, 0, 0]
		#romTotCov = [45.49, 94.35, 47.45, 94.11]
		#romCovCirc = [23.81, 94.75, 22.06, 93.88]
		#romNumHops = [7.30, 2.14, 6.62, 2.07]
		#romAlertSent = [219, 109, 236, 107]

		print(romTotCov)
		printSingleGraphRomanelliComparison(cw,
						actualFolder,
						"Padua scenario with buildings, total coverage",
						xList,
						xLabels, 
						xLabel,
						"Total coverage (%)",
						figurePrefix + "TotalCoverage",
						compoundData["totCoverageMeans"],
						compoundData["totCoverageConfInts"],
						romTotCov,
						protocols,
						True)
		printSingleGraphRomanelliComparison(cw,
						actualFolder,
						"Padua scenario with buildings, coverage on circumference",
						xList,
						xLabels,
						xLabel,
						"Coverage on circumference (%)",
						figurePrefix + "CoverageOnCirc",
						compoundData["covOnCircMeans"],
						compoundData["covOnCircConfInts"],
						romCovCirc,
						protocols,
						True)
		printSingleGraphRomanelliComparison(cw,
						actualFolder,
						"Padua scenario with buildings, number of hops",
						xList,
						xLabels,
						xLabel,
						"Number of hops",
						figurePrefix + "NumberOfHops",
						compoundData["hopsMeans"],
						compoundData["hopsConfInts"],
						romNumHops,
						protocols,
						True,
						0,
						10)
		printSingleGraphRomanelliComparison(cw,
						actualFolder,
						"Padua scenario with buildings, number of alert messages sent",
						xList,
						xLabels, 
						xLabel,
						"Number of sent alert messages",
						figurePrefix + "AlertMessagesSent",
						compoundData["messageSentMeans"],
						compoundData["messageSentConfInts"],
						romAlertSent,
						protocols,
						True,
						0,
						120)
		printSingleGraphRomanelliComparison(cw,
						actualFolder,
						"Padua scenario with buildings, number of slots waited",
						xList,
						xLabels, 
						xLabel, 
						"Number of slots waited",
						figurePrefix + "SlotsWaited",
						compoundData["slotsWaitedMeans"],
						compoundData["slotsWaitedConfInts"],
						romSlotsWaited,
						protocols,
						True,
						0,
						1500)
'''

'''
def printSingleGraphRomanelliComparison(cw, folder, graphTitle, xList, xLabels, xLabel, yLabel, figureTitle, yDataDictionary, 
					confIntDictionary, romData, protocols, autoscale=False, yBottomLim=0, yTopLim=100):
	
	yDataDictionary = listsToList(yDataDictionary, protocols)
	confIntDictionary = listsToList(confIntDictionary, protocols)
	
	ind = np.arange(len(xLabels))
	n = len(xLabels)
	#barWidth = float((float(1)/float(n)) * float(0.90))
	barWidth = 0.35
	fig, ax = plt.subplots()
	rects = []
	count = 0
	colors = ["0.3", "0.5"]
	#widthDistance = [0, 1]
	#widthDistance = [-1, 0, 1]
	#widthDistance = [-1.5, -0.5, 0.5, 1.5]
	#for protocol in protocols:
		#rects.append((ax.bar(ind + widthDistance[count] * barWidth, yDataDictionary[protocol], barWidth, color=colors[count], label=protocol, yerr=confIntDictionary[protocol], capsize=4)))
	rects.append((ax.bar(ind, yDataDictionary, barWidth, color=colors[0], yerr=confIntDictionary, capsize=4)))
	#rects.append((ax.bar(ind + barWidth / 2, romData, barWidth, color=colors[1], label="Romanelli")))

	ax.set_xlabel(xLabel, fontsize=15)
	ax.set_ylabel(yLabel, fontsize=15)
	if not autoscale:
		ax.set_ylim(yBottomLim, yTopLim)
	#ax.set_title(graphTitle, fontsize=20)
	ax.set_xticks(ind)
	ax.set_xticklabels(xLabels)
	#ax.set_xticklabels(["15m", "25m", "35m", "45m"])

	box = ax.get_position()
	ax.set_position([box.x0, box.y0, box.width * 0.8, box.height])
	ax.legend(loc='center left', bbox_to_anchor=(1, 0.5))

	#ax.legend(loc="upper center")

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
	plt.clf()
	#plt.savefig('b2.pdf', bbox_inches='tight')
	#plt.show()

'''