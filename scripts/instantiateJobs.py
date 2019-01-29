#!/usr/bin/python

# Invocation: 
#	./instantiateJobs.py 30
# if you want 30 jobs, otherwise
#	./instantiateJobs.py 
# which defaults to 30

import sys, os
import string
import shutil

def main():
	if (len(sys.argv) > 1):
		jobsNumber = int(sys.argv[1])
	else:
		jobsNumber = 30
	thisScriptPath = os.path.realpath(__file__)
	thisScriptParentPath = os.path.dirname(thisScriptPath)
	jobsTemplatePath = os.path.join(os.path.dirname(thisScriptParentPath), "jobsTemplate")
	jobsToSchedulePath = os.path.join(os.path.dirname(thisScriptParentPath), "scheduler/jobsToSchedule")
	if (not os.path.isdir(jobsToSchedulePath)):
		if (os.path.exists(jobsToSchedulePath)):
			os.remove(jobsToSchedulePath)
		os.mkdir(jobsToSchedulePath)
	for fileName in os.listdir(jobsTemplatePath):
		for i in range(0, jobsNumber):
			jobTemplateCompletePath = os.path.join(jobsTemplatePath, fileName)
			jobCompletePath = os.path.join(jobsToSchedulePath, fileName)
			shutil.copy(jobTemplateCompletePath, jobsToSchedulePath)
			s = open(jobCompletePath).read()
			s = s.replace("{**timestamp}", "-" + str(i))
			f = open(jobCompletePath, "w")
			f.write(s)
			f.close()
			lastDashPos = fileName.rfind("-")
			fromEndTillDash = len(fileName) - lastDashPos - 1
			newJobFileName = fileName[:-fromEndTillDash] + str(i) + ".job"
			jobCompletePathAfterRename =  os.path.join(jobsToSchedulePath, newJobFileName)
			os.rename(jobCompletePath, jobCompletePathAfterRename)
			
if __name__ == "__main__":
	main()