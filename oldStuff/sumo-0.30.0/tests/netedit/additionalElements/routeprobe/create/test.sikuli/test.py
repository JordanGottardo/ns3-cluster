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

# select routeProbe
netedit.changeAdditional("routeProbe")

# create routeProbe (camera will be moved)
netedit.leftClick(match, 250, 220)

# create another routeProbe with the same default attributes (camera will be moved)
netedit.leftClick(match, 250, 220)

# set invalid frequency
netedit.modifyAdditionalDefaultValue(2, "-20")

# try to create routeProbe
netedit.leftClick(match, 250, 220)

# set valid default frequency
netedit.modifyAdditionalDefaultValue(2, "120")

# create routeProbe (camera will be moved)
netedit.leftClick(match, 250, 220)

# set invalid default begin
netedit.modifyAdditionalDefaultValue(3, "-11")

# try to create routeProbe
netedit.leftClick(match, 250, 220)

# set valid default begin
netedit.modifyAdditionalDefaultValue(3, "10")

# create routeProbe (camera will be moved)
netedit.leftClick(match, 250, 220)

# Check undo redo
netedit.undo(match, 4)
netedit.redo(match, 4)

# save additionals
netedit.saveAdditionals()

# save newtork
netedit.saveNetwork()

# quit netedit
netedit.quit(neteditProcess)
