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

#from elementtree.ElementTree import parse
from xml.etree.cElementTree import parse

from GameObjectClass import *

class GameObjectClassManager():
    def __init__(self):
        self.fileDict = {}
        self.callback = None

        # we need to hold a reference to the game object representaions ourself
        # python does not recognize the a reference to a c++ object (Entity in our case) is passed
        # and deletes the object
        self.gameObjectRepresentationDict = []

    def inWorldIdExists(self, id):
        for rep in self.gameObjectRepresentationDict:
            if str(rep.inWorldId) == str(id):
                return True
        
        return False

    def addGameObjectRepresentation(self, gocRep):
        self.gameObjectRepresentationDict.append(gocRep)

    def parseGOFFiles(self, filePaths):
        for filePath in filePaths:
            tree = parse(filePath)
            gocList = []
            for parent in tree.getiterator("gameobjectclass"): # return all allements with the "gameobjectclass" tag
                go = GameObjectClass(parent)
                gocList.append(go)

            self.fileDict[filePath] = gocList

        if self.callback is not None:
            self.callback(self.fileDict)

    def getGameObjectWithClassId(self, id):
        for key in self.fileDict:
            for go in self.fileDict[key]:
                if go.classid == id:
                    return go

        return None

        # whenever a gameobject changes outside of the view, the view has to update itself wth the new values
    def setGameObjectsViewUpdateCallback(self, callback):
        self.callback = callback

