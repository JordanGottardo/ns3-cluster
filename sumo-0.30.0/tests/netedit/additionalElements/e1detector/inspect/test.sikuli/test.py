#!/usr/bin/env python
"""
@file    test.py
@author  Pablo Alvarez Lopez
@date    2016-11-25
@version $Id: test.py 24076 2017-04-26 17:49:30Z palcraft $

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

# select E1
netedit.changeAdditional("e1Detector")

# create E1 1
netedit.leftClick(match, 250, 150)

# create E1 2
netedit.leftClick(match, 450, 150)

# go to inspect mode
netedit.inspectMode()

# inspect first E1
netedit.leftClick(match, 250, 150)

# Change parameter 0 with a non valid value (Duplicated ID)
netedit.modifyAttribute(0, "e1Detector_gneE2_1_1")

# Change parameter 0 with a valid value
netedit.modifyAttribute(0, "correct ID")

# Change parameter 1 with a non valid value (dummy lane)
netedit.modifyAttribute(1, "dummy lane")

# Change parameter 1 with a valid value (different edge)
netedit.modifyAttribute(1, "gneE0_0")

# Change parameter 1 with a valid value (original edge, same lane)
netedit.modifyAttribute(1, "gneE2_1")

# Change parameter 1 with a valid value (original edge, different lane)
netedit.modifyAttribute(1, "gneE2_0")

# Change parameter 2 with a non valid value (negative)
netedit.modifyAttribute(2, "-5")

# Change parameter 2 with a non valid value (> endPos)
netedit.modifyAttribute(2, "400")

# Change parameter 2 with a valid value
netedit.modifyAttribute(2, "20")

# Change parameter 3 with a non valid value (non numeral)
netedit.modifyAttribute(3, "dummyFrequency")

# Change parameter 3 with a non valid value (negative)
netedit.modifyAttribute(3, "-100")

# Change parameter 3 with a valid value
netedit.modifyAttribute(3, "120")

# Change parameter 4 with an empty value
netedit.modifyAttribute(4, "")

# Change parameter 4 with an non valid value
netedit.modifyAttribute(4, "%%%&&%$%$")

# Change parameter 4 with a duplicated value
netedit.modifyAttribute(4, "e1Detector_gneE2_1_1.txt")

# Change parameter 4 with a valid value
netedit.modifyAttribute(4, "myOwnOutput.txt")

# Change boolean parameter 5
netedit.modifyBoolAttribute(5)

# click over an empty area
netedit.leftClick(match, 0, 0)

# Check undos and redos
netedit.undo(match, 12)
netedit.redo(match, 12)

# save additionals
netedit.saveAdditionals()

# save newtork
netedit.saveNetwork()

# quit netedit
netedit.quit(neteditProcess)
