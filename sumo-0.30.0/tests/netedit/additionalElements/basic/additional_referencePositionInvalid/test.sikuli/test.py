#!/usr/bin/env python
"""
@file    test.py
@author  Pablo Alvarez Lopez
@date    2017-03-12
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

# select busStop
netedit.changeAdditional("busStop")

# set invalid reference (dummy)
netedit.modifyAdditionalDefaultValue(6, "dummy reference")

# try to create busStop with the dummy reference
netedit.leftClick(match, 240, 250)

# set valid reference
netedit.modifyAdditionalDefaultValue(6, "reference right")

# create busStop with the valid reference
netedit.leftClick(match, 300, 250)

# save additionals
netedit.saveAdditionals()

# save newtork
netedit.saveNetwork()

# quit netedit
netedit.quit(neteditProcess)
