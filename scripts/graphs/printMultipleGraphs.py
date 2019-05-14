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

def printSingleGraphDistance(outFolder, graphTitle, compoundData, distances, protocols, cw, txRange, junctions, metric, xLabel, yLabel, minY, maxY):
	n = len(distances)
	ind = np.arange(n)
	
	barWidth = float((float(1)/float(4)) * float(0.6))
	fig, ax = plt.subplots()

	rects = []
	count = 0
	#colors = ["0.3", "0.5", "0.7"]
	colors = ["0.3", "0.5", "0.7","0.9"]
	
	#widthDistance = [-1, 1]
	widthDistance = [-1.5, -0.5, 0.5, 1.5]
	#widthDistance = [-1, 0, 1]

	protocolsList = ["Fast-Broadcast", "SJ Fast-Broadcast", "ROFF", "SJ ROFF"]
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
	
	ax.set_xlabel(xLabel, fontsize=11)
	ax.set_ylabel(yLabel, fontsize=11)
	if ("cov" in metric or "Cov" in metric):
		maxY = maxY * 1.05
	else:
		maxY = maxY * 1.1
	ax.set_ylim(minY, maxY)
	#ax.set_title(graphTitle, fontsize=20)
	ax.set_xticks(ind)
	ax.set_xticklabels(distances)
	#ax.set_xticklabels(["15m", "25m", "35m", "45m"])

	ax.legend(loc='center left', bbox_to_anchor=(1, 0.5))
	#ax.legend(loc="upper right")
	'''
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
	'''	
	outPathDirectory = os.path.join("out", outFolder + "-" + cw)
	outPath = os.path.join(outPathDirectory , metric) #todo fix
	if (not os.path.exists(outPathDirectory)):
		os.makedirs(outPathDirectory)
	
	plt.savefig(outPath + ".pdf")
	plt.clf()
	#plt.savefig('b2.pdf', bbox_inches='tight')
	#plt.show()


def printSingleGraphErrorRate(outFolder, graphTitle, compoundData, errorRates, protocols, cw, txRange, junctions, metric, xLabel, yLabel, minY, maxY):
	n = len(errorRates)
	ind = np.arange(n)
	
	barWidth = float((float(1)/float(4)) * float(0.6))
	fig, ax = plt.subplots()

	rects = []
	count = 0
	#colors = ["0.3", "0.5", "0.7"]
	colors = ["0.3", "0.5", "0.7","0.9"]
	
	#widthDistance = [-1, 1]
	widthDistance = [-1.5, -0.5, 0.5, 1.5]
	#widthDistance = [-1, 0, 1]

	protocolsList = ["Fast-Broadcast", "SJ Fast-Broadcast", "ROFF", "SJ ROFF"]
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
	
	ax.set_xlabel(xLabel, fontsize=11)
	ax.set_ylabel(yLabel, fontsize=11)
	if ("cov" in metric or "Cov" in metric):
		maxY = maxY * 1.05
	else:
		maxY = maxY * 1.1
	ax.set_ylim(minY, maxY)
	#ax.set_title(graphTitle, fontsize=20)
	ax.set_xticks(ind)
	ax.set_xticklabels(errorRates)
	#ax.set_xticklabels(["15m", "25m", "35m", "45m"])

	ax.legend(loc='center left', bbox_to_anchor=(1, 0.5))
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


def printSingleGraph(outFolder, graphTitle, compoundData, txRanges, protocols, cw, junction, metric, yLabel, minY, maxY):
	n = len(protocols)
	ind = np.arange(n)
	
	barWidth = float((float(1)/float(4)) * float(0.90))
	fig, ax = plt.subplots()

	rects = []
	count = 0
	colors = ["0.3", "0.5", "0.7"]
	#colors = ["0.3", "0.7"]
	
	#widthDistance = [-1, 1]
	#widthDistance = [-1.5, -0.5, 0.5, 1.5]
	widthDistance = [-1, 0, 1]

	for txRange in txRanges:
		metricMeanList = []
		metricConfIntList = []
		for protocol in protocols:
			metricMean = metric + "Mean"
			metricConfInt = metric + "ConfInt"
			metricMeanList.append(compoundData[txRange][protocol][metricMean])
			metricConfIntList.append(compoundData[txRange][protocol][metricConfInt])
		rects.append((ax.bar(ind + widthDistance[count] * barWidth, metricMeanList, barWidth, color=colors[count], label=txRange + "m", yerr=metricConfIntList, capsize=4)))
		count = count + 1
	
	ax.set_xlabel("Protocols", fontsize=15)
	ax.set_ylabel(yLabel, fontsize=15)
	if ("cov" in metric or "Cov" in metric):
		maxY = maxY * 1.05
	else:
		maxY = maxY * 1.1
	ax.set_ylim(minY, maxY)
	#ax.set_title(graphTitle, fontsize=20)
	ax.set_xticks(ind)
	plt.xticks(fontsize=15)
	plt.yticks(fontsize=15)
	ax.set_xticklabels(protocols)
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
					'{}'.format(height), ha=ha[xpos], va='bottom', fontsize=15) 

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

def appendCompoundData(basePath, txRanges, protocols, cw, junction, errorRate, compoundData, metrics):
	for txRange in txRanges:
		for protocol in protocols:
			path = None
			roff = False
			static = False
			if ("STATIC" in protocol and txRange not in protocol):
				static = True
			if (protocol != "ROFF"):
				path = os.path.join(basePath, errorRate, "r" + txRange, "j" + junction, cw, protocol)
			else: 
				roff = True
				path = os.path.join(basePath, errorRate, "r" + txRange, "j" + junction, protocol)
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
	print(compoundData)
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

	print("compoundDatab0")
	print(compoundDatab0)
	print("compoundDatab1")
	print(compoundDatab1)
	#printSingleGraphLineComparison()

#Given a scenario path and buildings/nobuildings, prints graphs for all txRanges and protocols
#Grid-300: contentionWindows = [{"cwMin": 16, "cwMax": 128}], buildings = ["0"], junctions = ["0"], txRanges = ["100", "300", "500"]
def printProtocolComparison():
	print("PrintProtocolComparison")
	plt.rcParams["figure.figsize"] = [18, 10]
	initialBasePath = "/home/jordan/MEGA/Universita_mia/Magistrale/Tesi/ns3-cluster/ns-3.26/out/scenario-urbano"
	#scenarios = ["Grid-200", "Grid-300", "Grid-400", "LA-15", "LA-25", "LA-35", "LA-45", "Padova-15", "Padova-25", "Padova-35", "Padova-45"]
	scenarios = ["Padova-25"]
	buildings = ["0", "1"]
	errorRate = "e0"
	#txRanges = ["100", "300", "500"]
	txRanges = ["100", "300", "500"]
	protocols = ["Fast-Broadcast", "STATIC-100", "STATIC-300", "STATIC-500", "ROFF"]
	#protocols = ["Fast-Broadcast", "STATIC-100", "STATIC-300"ROFF"]
	#cws = ["cw[16-128]"]
	cws = ["cw[16-128]", "cw[32-1024]"]
	#junctions = ["0", "1"]
	junctions = ["0", "1"]
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
						printSingleGraph(graphOutFolder, "graphTitle", compoundData, txRanges, protocols, cw, junction, metric, yLabel, 0, maxMetricValues[metric])


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
	cws = ["cw[16-128]", "cw[32-1024]"]
	#cws = ["cw[16-128]"]
	forgedRates = ["0", "10", "20", "30", "40", "50", "100"]
	junctions = ["0", "1"]
	xLabel = "% of vehicles affected by forging"
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
					printSingleGraphErrorRate(graphOutFolder, "graphTitle", forgedRateCompoundData, forgedRates, protocols, cw, "300", junctions, metric, xLabel, yLabel, 0, maxMetricValues[metric])

def printDistanceComparison():
	print("PrintDistanceComparison")
	plt.rcParams["figure.figsize"] = [18, 10]
	initialBasePath = "/home/jordan/MEGA/Universita_mia/Magistrale/Tesi/ns3-cluster/ns-3.26/out/scenario-urbano"
	#scenarios = ["Grid-200", "Grid-300", "Grid-400", "LA-15", "LA-25", "LA-35", "LA-45", "Padova-15", "Padova-25", "Padova-35", "Padova-45"]
	scenarios = ["Padova"]
	#distances = ["15", "25", "35", "45"]
	distances = ["25", "25", "25", "25"]
	buildings = ["0" , "1"]
	txRanges = ["100", "300", "500"]
	protocols = ["Fast-Broadcast", "ROFF"]
	errorRate = "e0"
	#cws = ["cw[16-128]", "cw[32-1024]"]
	cws = ["cw[16-128]"]
	junctions = ["0", "1"]
	xLabel = "Distances between vehicle (m)"
	metrics = ["totCoverage", "covOnCirc", "hops", "slotsWaited", "messageSent"]
	metricYLabels = {}
	metricYLabels["totCoverage"] = "Total Coverage (%)"
	metricYLabels["covOnCirc"] = "Coverage on circumference (%)"
	metricYLabels["hops"] = "Number of hops to reach circumference"
	metricYLabels["slotsWaited"] = "Number of slots waited to reach circumference"
	metricYLabels["messageSent"] = "Number of alert messages sent"
	

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
				for metric in metrics:
					yLabel = metricYLabels[metric]
					printSingleGraphDistance(graphOutFolder, "graphTitle", distanceCompoundData, distances, protocols, cw, "500", junctions, metric, xLabel, yLabel, 0, maxMetricValues[metric])

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