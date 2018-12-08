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

# go to additional mode
netedit.additionalMode()

# select calibrator
netedit.changeAdditional("calibrator")

# create calibrator
netedit.leftClick(match, 240, 250)

# change frequency with an invalid value (dummy)
netedit.modifyAdditionalDefaultValue(2, "dummyValue")

# create calibrator with an invalid parameter (Default value will be used)
netedit.leftClick(match, 400, 215)

# change frequency with an invalid value (negative)
netedit.modifyAdditionalDefaultValue(2, "-30")

# create calibrator with an invalid parameter (Default value will be used)
netedit.leftClick(match, 400, 215)

# change frequency with a valid value
netedit.modifyAdditionalDefaultValue(2, "250")

# create calibrator with a valid parameter in other lane
netedit.leftClick(match, 400, 180)

# Check undo redo
netedit.undo(match, 4)
netedit.redo(match, 4)

# save additionals
netedit.saveAdditionals()

# save newtork
netedit.saveNetwork()

# quit netedit
netedit.quit(neteditProcess)
