#!/usr/bin/env python
"""
@file    test.py
@author  Pablo Alvarez Lopez
@date    2016-11-25
@version $Id: test.py 24005 2017-04-21 12:54:13Z palcraft $

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

# apply zoom
netedit.setZoom("25", "0", "25")

# go to additional mode
netedit.additionalMode()

# select rerouter
netedit.changeAdditional("rerouter")

# try to create rerouter without edge child (Warning)
netedit.leftClick(match, 100, 100)

# select edge child
netedit.selectAdditionalChild(8, 0)

# create rerouter with default parameters
netedit.leftClick(match, 200, 100)

# set invalid probability
netedit.modifyAdditionalDefaultValue(2, "2")

# try to create rerouter with different frequency
netedit.leftClick(match, 300, 100)

# set valid probability
netedit.modifyAdditionalDefaultValue(2, "0.3")

# create rerouter with different probability
netedit.leftClick(match, 300, 100)

# change off
netedit.modifyAdditionalDefaultBoolValue(3)

# try to create rerouter with different timeTreshold
netedit.leftClick(match, 400, 100)

# Check undo redo
netedit.undo(match, 3)
netedit.redo(match, 3)

# save additionals
netedit.saveAdditionals()

# save newtork
netedit.saveNetwork()

# quit netedit
netedit.quit(neteditProcess)