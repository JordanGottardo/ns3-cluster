#!/usr/bin/env python
"""
@file    test.py
@author  Pablo Alvarez Lopez
@date    2016-11-25
@version $Id: test.py 24017 2017-04-22 14:14:36Z palcraft $

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

# select chargingStation
netedit.changeAdditional("chargingStation")

# set invalid charging power
netedit.modifyAdditionalDefaultValue(2, "-200")

# try to create chargingStation in mode "reference left"
netedit.leftClick(match, 250, 250)

# set invalid charging
netedit.modifyAdditionalDefaultValue(2, "12000")

# create chargingStation in mode "reference left"
netedit.leftClick(match, 250, 250)

# change reference to right
netedit.modifyAdditionalDefaultValue(7, "reference right")

# set invalid efficiency
netedit.modifyAdditionalDefaultValue(3, "2")

# try create chargingStation in mode "reference right"
netedit.leftClick(match, 240, 250)

# set valid efficiency
netedit.modifyAdditionalDefaultValue(3, "0.3")

# create chargingStation in mode "reference right"
netedit.leftClick(match, 240, 250)

# change reference to center
netedit.modifyAdditionalDefaultValue(7, "reference center")

# Change change in transit
netedit.modifyAdditionalDefaultBoolValue(4)

# create chargingStation in mode "reference center"
netedit.leftClick(match, 425, 250)

# return to mode "reference left"
netedit.modifyAdditionalDefaultValue(7, "reference left")

# Change length
netedit.modifyAdditionalDefaultValue(9, "30")

# try to create a chargingStation in mode "reference left" (error)
netedit.leftClick(match, 500, 250)

# change reference to right
netedit.modifyAdditionalDefaultValue(7, "reference right")

# try chargingStation in mode "reference right" (Warning)
netedit.leftClick(match, 110, 250)

# enable force position
netedit.modifyAdditionalDefaultBoolValue(10)

# change reference to "reference left"
netedit.modifyAdditionalDefaultValue(7, "reference left")

# set invalid charge delay
netedit.modifyAdditionalDefaultValue(5, "-5")

# try to create a chargingStation in mode "reference left" forcing poisition
netedit.leftClick(match, 500, 250)

# valid charge delay
netedit.modifyAdditionalDefaultValue(5, "7")

# create a chargingStation in mode "reference left" forcing poisition
netedit.leftClick(match, 500, 250)

# change reference to "reference right"
netedit.modifyAdditionalDefaultValue(7, "reference right")

# create a chargingStation in mode "reference right" forcing position
netedit.leftClick(match, 110, 250)

# Check undo redo
netedit.undo(match, 6)
netedit.redo(match, 6)

# save additionals
netedit.saveAdditionals()

# save newtork
netedit.saveNetwork()

# quit netedit
netedit.quit(neteditProcess)
