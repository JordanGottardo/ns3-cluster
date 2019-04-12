#!/usr/bin python

from subprocess import check_output
from re import search
cmd    = ['qstat', '-xml']
# get the xml output
output = check_output (cmd)
keys   = [] # the feature names
vals   = [] # the jobs including all features
job    = [] # the features
haskey = 0  # whether I have the extra 'state' key
lens   = [] # str length of different field
states = {} # counting jobs with different state
for line in output.split("\n"):
    # The start line of the job
    m1 = search (r'<job_list state="(.+)"', line)
    # The feature line
    m2 = search (r'<(.+?)>(.*?)</.+>', line)
    if m1:
        # Get the state
        state = m1.group(1)
        job = [state]
        # Count
        if state not in states:
            states[state] = 0
        states[state] += 1
        # Add the job
        vals.append(job)

        # If 'state' key is not added
        if haskey == 0:
            keys = ['state']
            haskey = 1
        # If it's already added
        elif haskey == 1:
            haskey = 2
    elif m2:
        # The feature name and value
        title = m2.group(1)
        value = m2.group(2)
        # Make sure 'state' is the last feature
        col   = 0 if len (job) == 1 else -1
        # job is a reference, referred to the job in vals
        job.insert (col, value)

        if haskey == 1:
            # Make sure 'state' is the last key
            col = 0 if len (keys) == 1 else -1
            keys.insert (col, title)

# calculate the length
for i, key in enumerate(keys):
    klen = len (key)
    vlen = max (map(lambda x: len(x[i]), vals))
    lens.append (max(klen, vlen))

print "  ".join ([x.upper().ljust(lens[i]) for i,x in enumerate(keys)])
print "+".join (['-'*(lens[i]+1) for i,_ in enumerate(keys)])
for val in vals:
    print "  ".join ([x.ljust(lens[i]) for i,x in enumerate(val)])

print "\nTotal jobs: %s%s" % (len(vals), "".join([", " + k + ": " + str(v) for k,v in states.items()]))