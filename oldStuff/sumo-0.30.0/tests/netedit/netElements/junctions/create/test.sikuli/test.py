#!/usr/bin/env python
"""
@file    test.py
@author  Pablo Alvarez Lopez
@date    2016-11-25
@version $Id: test.py 24066 2017-04-25 17:19:15Z palcraft $

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
neteditProcess, match = netedit.setupAndStart(neteditTestRoot, True)

# Focus netedit window
netedit.leftClick(match, 0, -105)

# Change to create edge mode
netedit.createEdgeMode()

# Create one way edge
netedit.leftClick(match, -50, 50)
netedit.leftClick(match, 50, 50)

# try to create an edge with the same start and end (musn't be allowed)
netedit.leftClick(match, -50, 50)
netedit.leftClick(match, -50, 50)
netedit.cancelEdge()

# Create two way edges
netedit.leftClick(match, 150, 50)
netedit.leftClick(match, 250, 50)
netedit.leftClick(match, 250, 50)
netedit.leftClick(match, 150, 50)

# select two-way mode
netedit.changeTwoWayOption()

# Create two way edges
netedit.leftClick(match, 350, 50)
netedit.leftClick(match, 450, 50)

# select chain mode
netedit.changeChainOption()

# create square
netedit.leftClick(match, -50, 150)
netedit.leftClick(match, 50, 150)
netedit.leftClick(match, 50, 250)
netedit.leftClick(match, -50, 250)
netedit.leftClick(match, -50, 150)
netedit.cancelEdge()

# try to create a duplicated edge (musn't be allowed)
netedit.leftClick(match, 350, 50)
netedit.leftClick(match, 450, 50)
netedit.cancelEdge()

# abort creation of edge
netedit.leftClick(match, 300, 250)
netedit.cancelEdge()

# create a circular road
netedit.leftClick(match, 300, 150)
netedit.leftClick(match, 400, 150)
netedit.leftClick(match, 400, 250)
netedit.leftClick(match, 400, 350)
netedit.leftClick(match, 300, 350)
netedit.leftClick(match, 200, 350)
netedit.leftClick(match, 200, 250)
netedit.leftClick(match, 200, 150)
netedit.leftClick(match, 300, 150)
netedit.cancelEdge()

# disable chain mode
netedit.changeChainOption()

# create a complex intersection
netedit.leftClick(match, 300, 250)
netedit.leftClick(match, 300, 150)

netedit.leftClick(match, 300, 250)
netedit.leftClick(match, 400, 250)

netedit.leftClick(match, 300, 250)
netedit.leftClick(match, 300, 350)

netedit.leftClick(match, 300, 250)
netedit.leftClick(match, 200, 250)

# rebuild network
netedit.rebuildNetwork()

# Check undo and redo
netedit.undo(match, 20)
netedit.redo(match, 20)

# save newtork
netedit.saveNetwork()

# quit netedit
netedit.quit(neteditProcess)
