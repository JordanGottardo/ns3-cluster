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

# select E3
netedit.changeAdditional("e3Detector")

# create E3
netedit.leftClick(match, 100, 50)

# select entry detector
netedit.changeAdditional("detEntry")

# Create Entry detector for E3
netedit.selectAdditionalChild(4, 3)
netedit.leftClick(match, 100, 200)

# select Exit detector
netedit.changeAdditional("detExit")

# Create Exit detector for E3
netedit.selectAdditionalChild(4, 3)
netedit.leftClick(match, 250, 200)

# Undo creation of E3, Entry and Exit
netedit.leftClick(match, 0, 0)
netedit.undo(match, 3)

# go to additional mode
netedit.additionalMode()

# select E3
netedit.changeAdditional("e3Detector")

# create E3
netedit.leftClick(match, 100, 50)

# select entry detector
netedit.changeAdditional("detEntry")

# Create Entry detector for E3
netedit.selectAdditionalChild(4, 3)
netedit.leftClick(match, 100, 200)

# select Exit detector
netedit.changeAdditional("detExit")

# Create Exit detector for E3
netedit.selectAdditionalChild(4, 3)
netedit.leftClick(match, 250, 200)

# Undo creation of Entry and Exit
netedit.leftClick(match, 0, 0)
netedit.undo(match, 2)

# go to additional mode
netedit.additionalMode()

# select entry detector
netedit.changeAdditional("detEntry")

# Create Entry detector for E3
netedit.selectAdditionalChild(4, 3)
netedit.leftClick(match, 100, 200)

# select Exit detector
netedit.changeAdditional("detExit")

# Create Exit detector for E3
netedit.selectAdditionalChild(4, 3)
netedit.leftClick(match, 250, 200)

# Change to delete
netedit.deleteMode()

# Delete the four Entry/exits
netedit.leftClick(match, 100, 200)
netedit.leftClick(match, 250, 200)
netedit.leftClick(match, 450, 200)
netedit.leftClick(match, 600, 200)

# Check undo redo
netedit.undo(match, 7)
netedit.redo(match, 7)

# save additionals
netedit.saveAdditionals()

# save newtork
netedit.saveNetwork()

# quit netedit
netedit.quit(neteditProcess)
