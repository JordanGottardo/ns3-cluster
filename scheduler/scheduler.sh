#!/bin/bash

# cron has limited env, setup cluster and everything else
PATH=/export/alt/condor/bin:/export/alt/condor/sbin:/usr/local/cuda/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/export/alt/torque/bin:/export/alt/torque/sbin

echo "Running cron job at $(date)"

#once jobs are scheduled they are moved under this directory
finnished=/home/jgottard/ns-3/scheduler/scheduledJobs
if [ ! -d "$finnished" ]; then
	# Control will enter here if $DIRECTORY doesn't exist.
	mkdir $finnished
fi

#maximum number of jobs that can ben in a running status
number_max_active_jobs=100
#check the number of scheduled jobs
number_running=$(qstat | grep jgottard | wc -l)
#number of jobs we can schedule this turn
number_schedulable=$((number_max_active_jobs-number_running))
#jobs yet to be scheduled
left=$(ls /home/jgottard/ns-3/scheduler/jobsToSchedule | grep job)
#counter used to account for the number of jobs we are scheduling this turn
scheduled=0

echo "Total number of jobs currently running is $number_running. We can schedule $number_schedulable jobs"

if [ "$number_schedulable" -le "0" ]; then
	echo "Cannot schedule anymore"
	exit
fi

for f in $left
do
	echo "Scheduling job $f"
	## schedule job
	qsub /home/jgottard/ns-3/scheduler/jobsToSchedule/$f	
	## move job to scheduled directory
	mv /home/jgottard/ns-3/scheduler/jobsToSchedule/$f $finnished
	scheduled=$((scheduled+1))
	if [ "$scheduled" -eq "$number_schedulable" ]; then
		break
	fi
done

echo "Finnished for this round"

