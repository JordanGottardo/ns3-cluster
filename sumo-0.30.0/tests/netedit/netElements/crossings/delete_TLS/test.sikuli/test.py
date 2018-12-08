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

# go to delete mode
netedit.deleteMode()

# delete junction
netedit.leftClick(match, 300, 250)

# Rebuild network
netedit.rebuildNetwork()

# check undo
netedit.undo(match, 1)

# Rebuild network
netedit.rebuildNetwork()

# go to  traffic light mode
netedit.selectTLS()

# select traffic light
netedit.leftClick(match, 300, 250)

# delete traffic light
netedit.leftClick(match, -120, 150)

# go to reference
netedit.leftClick(match, 0, 0)

# Rebuild network
netedit.rebuildNetwork()

# check undo
netedit.undo(match, 1)
netedit.redo(match, 1)

# save newtork
netedit.saveNetwork()

# quit netedit
netedit.quit(neteditProcess)
