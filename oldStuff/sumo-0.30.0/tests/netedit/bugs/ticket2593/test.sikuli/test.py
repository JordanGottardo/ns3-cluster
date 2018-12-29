#!/usr/bin/env python
"""
@file    test.py
@author  Pablo Alvarez Lopez
@date    2016-11-25
@version $Id: test.py 23999 2017-04-21 09:04:47Z behrisch $

python script used by sikulix for testing netedit

SUMO, Simulation of Urban MObility; see http://sumo.dlr.de/
Copyright (C) 2009-2017 DLR/TS, Germany

This file is part of SUMO.
SUMO is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.
"""
# import common functions for netedit tests
import os
import sys

testRoot = os.path.join(os.environ.get('SUMO_HOME', '.'), 'tests')
neteditTestRoot = os.path.join(
    os.environ.get('TEXTTEST_HOME', testRoot), 'netedit')
sys.path.append(neteditTestRoot)
import neteditTestFunctions as netedit

# Open netedit
neteditProcess, match = netedit.setupAndStart(neteditTestRoot, False)

# go to additional mode
netedit.additionalMode()

# select busStop
netedit.changeAdditional("busStop")

# create busStop in mode "reference left"
netedit.leftClick(match, 250, 250)

# quit netedit
netedit.quit(neteditProcess, True, True, True, False)

# Open netedit again 
neteditProcess, match = netedit.setupAndStart(neteditTestRoot, False)

# go to additional mode
netedit.additionalMode()

# select busStop
netedit.changeAdditional("busStop")

# create busStop in mode "reference left"
netedit.leftClick(match, 250, 250)

# save newtork but don't save additionals
netedit.saveNetwork()

# quit netedit
netedit.quit(neteditProcess, False, True, True, False)

# Open netedit
neteditProcess, match = netedit.setupAndStart(neteditTestRoot, False)

# go to additional mode
netedit.additionalMode()

# select busStop
netedit.changeAdditional("busStop")

# create busStop in mode "reference left"
netedit.leftClick(match, 250, 250)

# save additionals
netedit.saveAdditionals()

# save newtork
netedit.saveNetwork()

# quit netedit (No dialog has to appear)
netedit.quit(neteditProcess)
