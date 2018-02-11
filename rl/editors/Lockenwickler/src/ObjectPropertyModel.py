 #################################################
# This source file is part of Rastullahs Lockenwickler.
# Copyright (C) 2003-2009 Team Pantheon. http://www.team-pantheon.de
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  US
 #################################################

import sys

from Property import *

class ObjectPropertyModel():
    def __init__(self):
        self.numProperties = 0
        self.currentObject = None
        self.properties = []

    def getNumProperies(self):
        return self.numProperties

    # sets the current object whose properties are displayed
    def setSingleObject(self, so):
        self.currentObject = so
        self.__parseProperties()

    def __parseProperties(self):
        node = self.currentObject.entity.getParentNode()
