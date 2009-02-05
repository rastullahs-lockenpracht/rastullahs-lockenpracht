#################################################
 # Copyright (C) 2008  Stefan Stammberger
 #
 # This library is free software; you can redistribute it and/or
 # modify it under the terms of the GNU Lesser General Public
 # License as published by the Free Software Foundation; either
 # version 2.1 of the License, or (at your option) any later version.
 #
 # This library is distributed in the hope that it will be useful,
 # but WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 # Lesser General Public License for more details.
 #
 # You should have received a copy of the GNU Lesser General Public
 # License along with this library; if not, write to the Free Software
 # Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 #################################################

#from elementtree.ElementTree import parse
from xml.etree.cElementTree import parse

from GameObjectClass import *

class GameObjectClassManager():
    def __init__(self):
        self.fileDict = {}
        self.callback = None

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

