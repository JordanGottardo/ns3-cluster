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
netedit.leftClick(match, 250, 50)

# select exit detector
netedit.changeAdditional("detExit")

# Create Exit detector
netedit.selectAdditionalChild(4, 0)
netedit.leftClick(match, 100, 200)

# apply zoom out
netedit.setZoom("25", "0", "70")

# change to move mode
netedit.moveMode()

# move Exit to left
netedit.moveElement(match, 120, 200, 50, 200)

# move back
netedit.moveElement(match, 50, 200, 120, 200)

# move Exit to right
netedit.moveElement(match, 120, 200, 250, 200)

# move back
netedit.moveElement(match, 250, 200, 120, 200)

# move Exit to left overpassing lane
netedit.moveElement(match, 120, 200, -150, 200)

# move back
netedit.moveElement(match, -100, 200, 120, 200)

# move Exit to right overpassing lane
netedit.moveElement(match, 120, 200, 580, 200)

# move back to another different position of initial
netedit.moveElement(match, 520, 200, 300, 200)

# Check undos and redos
netedit.undo(match, 10)
netedit.redo(match, 10)

# save additionals
netedit.saveAdditionals()

# save newtork
netedit.saveNetwork()

# quit netedit
netedit.quit(neteditProcess)
