#!/usr/bin/python

# Invocation: 
#	./instantiateJobs.py

import sys, os
import string
import shutil

def main():
	thisScriptPath = os.path.realpath(__file__)
	thisScriptParentPath = os.path.dirname(thisScriptPath)
	jobsTemplatePath = os.path.join(os.path.dirname(thisScriptParentPath), "jobsTemplate")
	jobsToSchedulePath = os.path.join(os.path.dirname(thisScriptParentPath), "scheduler/jobsToSchedule")

	for fileName in os.listdir(jobsTemplatePath):
		for i in range(0, 1):
			jobTemplateCompletePath = os.path.join(jobsTemplatePath, fileName)
			jobCompletePath = os.path.join(jobsToSchedulePath, fileName)
			shutil.copy(jobTemplateCompletePath, jobsToSchedulePath)
			s = open(jobCompletePath).read()
			s = s.replace("{**timestamp}", "-" + str(i))
			f = open(jobCompletePath, "w")
			f.write(s)
			f.close()
			lastDashPos = fileName.rfind("-")
			print("lastDashPos = " + str(lastDashPos))
			fromEndTillDash = len(fileName) - lastDashPos - 1
			print("fromEndTillDash = " + str(fromEndTillDash))
			newJobFileName = fileName[:-fromEndTillDash] + str(i) + ".job"
			jobCompletePathAfterRename =  os.path.join(jobsToSchedulePath, newJobFileName)
			os.rename(jobCompletePath, jobCompletePathAfterRename)
			
if __name__ == "__main__":
	main()