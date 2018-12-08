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

# Rebuild network
netedit.rebuildNetwork()

# zoom in central node
netedit.setZoom("50", "50", "50")

# set crossing mode
type("r")

# select central node
netedit.leftClick(match, 325, 225)

# select two left edges and create crossing in edges 3 and 7
netedit.leftClick(match, 150, 200)
netedit.leftClick(match, 150, 250)
netedit.createCrossing()
netedit.rebuildNetwork()

# create manual crossing
netedit.modifyCrossingDefaultValue(3, "1 5")
netedit.createCrossing()
netedit.rebuildNetwork()

# try to create again the same crossing (cannot be possible, show warning
# instead)
netedit.modifyCrossingDefaultValue(3, "1 5")
netedit.createCrossing()
netedit.rebuildNetwork()

# create single crossing (fail)
netedit.modifyCrossingDefaultValue(3, "4")
netedit.createCrossing()
netedit.rebuildNetwork()

# create split crossing
netedit.modifyCrossingDefaultValue(3, "4")
netedit.createCrossing()
netedit.modifyCrossingDefaultValue(3, "8")
netedit.createCrossing()
netedit.rebuildNetwork()

# create manual crossing with different priority and width
netedit.modifyCrossingDefaultValue(3, "6 2")
netedit.modifyCrossingDefaultBoolValue(4)
netedit.modifyCrossingDefaultValue(5, "5")
netedit.createCrossing()
netedit.rebuildNetwork()

# Check buttons
netedit.leftClick(match, 150, 200)
netedit.leftClick(match, 150, 250)
netedit.crossingInvertEdges()
netedit.crossingClearEdges()

# save newtork
netedit.saveNetwork()

# quit netedit
netedit.quit(neteditProcess)
