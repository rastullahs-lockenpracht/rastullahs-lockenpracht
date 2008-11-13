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

import sys
import codecs
import glob
import os
from os.path import isfile,  join


import elementtree.ElementTree as xml

import ctypes
import ogre.renderer.OGRE as og

from PyQt4.QtCore import *
from PyQt4.QtGui import *

from MovePivot import *
from GameObjectClassManager import *
from MyRaySceneQueryListener import *

class Map():
    def __init__(self):
        return

class Scene():
    def __init__(self):
        return

class Module():
    def __init__(self,name, modulePath):
        self.name = name
        self.moduleRoot = join(modulePath, name)
        self.__isCommon = False

        self.hasDependencies = False
        self.moduleDependencies = []

        self.mapFiles = [] # a list in case the module has more than one map file
        self.gofFiles = [] # gof File list

    def isCommon(self):
        modConfig = join(self.moduleRoot,  "scripts/moduleconfig.rb")
        if isfile(modConfig): # is the modconfig existing?
            f = codecs.open(modConfig, 'r', 'utf-8')
        else:
            print ("Error: couldn't find module config")
            return

        for i, line in enumerate(f):
            lStripped = line.strip() #strip the whitespace away, not needed here
            if lStripped.startswith("super("):
                split = lStripped.split(",")
                if split[2].strip() == unicode("true"):
                    self.__isCommon = True
                    return True

        return False

    def load(self):
        modConfig = join(self.moduleRoot,  "scripts/moduleconfig.rb")
        if isfile(modConfig): # is the modconfig existing?
            f = codecs.open(modConfig, 'r', 'utf-8')
        else:
            print ("Error: couldn't find module config")
            return

        isDependencieLine = False

        for i, line in enumerate(f):
            lStripped = line.strip() #strip the whitespace away, not needed here

            if isDependencieLine:
                if lStripped == "end":
                    isDependencieLine = False
                else:
                    self.moduleDependencies.append(lStripped.split('"')[1])

            elif lStripped == "def getDependencies()":
                isDependencieLine = True

        self.setResourcePaths()

    def setResourcePaths(self, recurseFolder = ""):
        if recurseFolder == "":
            rootFolder = self.moduleRoot
        else:
            rootFolder = join(self.moduleRoot, recurseFolder)

        for file in os.listdir(rootFolder):
            curFile = join(rootFolder, file)


            if file.startswith('.'): #ignore dot files (hidden)
                continue
            if os.path.isdir(curFile):
                og.ResourceGroupManager.getSingleton().addResourceLocation(curFile, "FileSystem", self.name, False)
                self.setResourcePaths(curFile)
                continue
            if os.path.isfile(curFile):
                pass


#                spl = lStripped.split('"')
#                for a in spl:
#                    if a.endswith(".xml"):
#                        pathToMapFile = self.workingDir + "/maps/" + a
#                        self.mapFiles.append(pathToMapFile)

#        self.setWindowTitle(moduleName)
#
#        self.modelSelectionDialog.scanDirForModels(self.workingDir, moduleName)
#        self.modelSelectionDialog.scanDirForModels(self.workingDirCommon, "common")
#
#        self.setResourcePaths(self.workingDir, moduleName)
#        self.setResourcePaths(self.workingDirCommon, "common")
#        og.ResourceGroupManager.getSingleton().initialiseAllResourceGroups()
#
#        command = (os.path.join(self.workingDir,  "maps") + "/*.xml")
#        for mf in glob.glob(command): # search for all xml files in the maps directory and add them
#            self.mapFiles.append(mf)
#
#        command = (os.path.join(self.workingDir,  "maps") + "/*.scene")
#        for mf in glob.glob(command): # search for all .scene files in the maps directory and add them
#            self.mapFiles.append(mf)
#
#        command = (os.path.join(self.workingDir,  "dsa") + "/*.gof")
#        for gf in glob.glob(command): # search for all .gof files in the dsa directory in the module dir
#            self.gofFiles.append(gf)
#
#        command = (os.path.join(self.workingDirCommon,  "dsa") + "/*.gof")
#        for gf in glob.glob(command): # search for all .gof files in the dsa directory in the common module dir
#            self.gofFiles.append(gf)
#
#        self.moduleManager.load(moduleName,  self.mapFiles,  self.gofFiles)



class ModuleManager():
    def __init__(self,  ogreRoot,  sceneManager):
        self.sceneManager = sceneManager
        self.ogreRoot = ogreRoot
        self.raySceneQuery = self.sceneManager.createRayQuery(og.Ray())

        self.gocManager = GameObjectClassManager()

        self.moduleList = []
        self.userSelectionList = []
        self.cutList = [] # selection objects that has been cut out and wait to be pasted again
        self.cutListPreviousNodes = [] # contains the nodes they where copnnected to before the cut

        # we need to hold a reference to the game object representaions ourself
        # python does not recognize the a reference to a c++ object (Entity in our case) is passed
        # and deletes the object
        self.gameObjectRepresentationDict = []

        self.listenerDings = MyRaySceneQueryListener()

        self.lastRay = None
#        self.rayLine = None

        self.pivot = None
        self.movingPivot = False

        self.leftMouseDown = False
        self.middleMouseDown = False
        self.rightMouseDown = False

        self.dropCount = 0
        self.dropNode = None
        self.dropEntity = None
        self.dropCollisionPlane = og.Plane(og.Vector3.UNIT_Y, og.Vector3.ZERO)

        self.numerOfCopys = 0 #everytime a copy is made this numer is increased to generate unique node and mesh names
        self.moduleConfigIsParsed = False


    def parseModuleConfig(self):
        if self.moduleConfigIsParsed:
            return

        import codecs
        f = codecs.open(self.moduleCfgPath, 'r', 'utf-8')

        for line in f:
            if line.startswith('#'):
                continue

            if line.startswith('module='):
                splines = line.split('=')
                str = splines[1].rstrip().rstrip()
                self.moduleList.append(Module(str, self.moduleCfgPath.replace("/modules.cfg",  "")))

        self.moduleConfigIsParsed = True

    def openLoadModuleDialog(self, moduleConfigPath, lw):
        self.moduleCfgPath = str(moduleConfigPath)
        self.moduleFolder = str(moduleConfigPath.replace("modules.cfg", ""))

        self.parseModuleConfig()

        dlg = QDialog()
        list = QListWidget()
        btnBox = QDialogButtonBox(QDialogButtonBox.Ok | QDialogButtonBox.Cancel)
        dlg.connect(btnBox, SIGNAL("accepted()"), dlg.accept)
        dlg.connect(btnBox, SIGNAL("rejected()"), dlg.reject)

        for m in self.moduleList:
            if not m.isCommon():
                list.addItem(m.name)

        layout = QVBoxLayout()
        layout.addWidget(list)
        layout.addWidget(btnBox)
        dlg.setLayout(layout)
        if dlg.exec_():
            self.loadModule(list.currentItem().text())

    def loadModule(self, moduleName):
        for m in self.moduleList:
            if m.name == moduleName:
                m.load()

    # called when a click into Main Ogre Window occurs
    def selectionClick(self,  ray,  controlDown=False,  shiftDown=False):
        self.listenerDings.reset()
        self.lastRay = ray
        self.listenerDings.currentRay = ray
        self.raySceneQuery.Ray = ray
        self.raySceneQuery.execute(self.listenerDings)

        so = self.listenerDings.rayCastToPolygonLevel(ray)

        if so is not None:
            if not so.isPivot:
                if not controlDown and not shiftDown:
                    self.resetSelection()
                    so.setSelected(True)
                    self.userSelectionList.append(so)
                    self.updatePivots()
                elif controlDown and not shiftDown:
                    so.setSelected(True)

                    for soFromList in self.userSelectionList:
                        if soFromList == so:
                            return # object already selected

                    self.userSelectionList.append(so)
                    self.updatePivots()


                elif not controlDown and shiftDown:
                    for selo in self.userSelectionList:
                        if so == selo:
                            so.setSelected(False)
                            self.userSelectionList.remove(selo)
                    self.updatePivots()
            else:
                #so.entity is the pivot direction that was clicked
                self.pivot.startTransforming(so.entity,  self.userSelectionList)
        else:
            self.resetSelection() # click in empty space, deselect everything
            if self.pivot is not None:
                self.pivot.hide()

#        if self.rayLine == None:
#            self.rayLine = self.sceneManager.createManualObject("rayLine")
#            self.rayLine.setDynamic(True)
#            self.sceneManager.getRootSceneNode().createChildSceneNode("raynode").attachObject(self.rayLine)
#
#            self.rayLine.begin("BaseWhiteNoLighting", og.RenderOperation.OT_LINE_STRIP)
#
#            self.rayLine.position(ray.getOrigin())
#            self.rayLine.position( ray.getPoint(10000))
#
#            self.rayLine.end()
#
#        else:
#            self.rayLine.beginUpdate(0)
#
#            self.rayLine.position(ray.getOrigin())
#            self.rayLine.position( ray.getPoint(10000))
#
#            self.rayLine.end()

    def deleteObjects(self):
        if len(self.userSelectionList) < 1:
            return

        self.pivot.hide()

        for so in self.userSelectionList:
            self.sceneManager.destroySceneNode(so.entity.getParentNode().getName())
            del so

        self.userSelectionList = []

    def incrementNameSuffixNumber(self, name):
        newName = ""
        split = name.split("_")
        lastPart = len(split)-1
        newName = name.rstrip(split[lastPart])
        newName = newName + str(self.numerOfCopys)

#        if split[lastPart].isdigit() and not split[lastPart].startswith("0"):
#            num = int(split[lastPart])
#            num = num + 1
#            newName = name.rstrip(split[lastPart])
#            newName = newName + str(num)
#        else:
#            newName = name + "_1"

        self.numerOfCopys = self.numerOfCopys + 1
        return newName

    def copyObjects(self):
        if len(self.userSelectionList) < 1:
            return

        newSelectionList = []

        for so in self.userSelectionList:
            if so.entity.getUserObject() is not None:
                if so.entity.getUserObject().getType() == "GAME_OBJECT_REPRESENTATION":
                    go = self.gocManager.getGameObjectWithClassId(so.entity.getUserObject().gocName)
                    meshFile = go.getMeshFileName()

                    if go is not None:
                        newEntity = self.sceneManager.createEntity("dropMesh" + str(self.dropCount), str(meshFile))
                        newNode = self.sceneManager.getRootSceneNode().createChild("dropNode" + str(self.dropCount))
                        newNode.attachObject(newEntity)
                        newNode.setPosition(so.entity.getParentNode().getPosition())

                        newGO = GameObjectRepresentation(self.dropCount, so.entity.getUserObject().gocName, newNode, meshFile)
                        self.gameObjectRepresentationDict.append(newGO)
                        newEntity.setUserObject(newGO)
                        newGO.setPosition(og.Vector3(0, 0, 0))

                        newSO = SelectionObject(newEntity, so.distance)
                        newSO.setSelected(True)
                        newSelectionList.append(newSO)
                        self.dropCount += 1
            else:
                nodeName = self.incrementNameSuffixNumber(so.entity.getParentNode().getName())
                newNode = self.sceneManager.getRootSceneNode().createChild(nodeName)

                entityName = self.incrementNameSuffixNumber(so.entity.getName())
                newEntity = self.sceneManager.createEntity(entityName, so.entity.getMesh().getName())

                newNode.attachObject(newEntity)
                newNode.setPosition(so.entity.getParentNode().getPosition())
                newNode.setOrientation(so.entity.getParentNode().getOrientation())
                newNode.setScale(so.entity.getParentNode().getScale())

                newSO = SelectionObject(newEntity, so.distance)
                newSO.setSelected(True)
                newSelectionList.append(newSO)

        self.resetSelection()
        self.userSelectionList = newSelectionList

    def cutObjects(self):
        if len(self.userSelectionList) < 1:
            return

        self.cutList = []
        for so in self.userSelectionList:
            self.cutListPreviousNodes.append(so.entity.getParentNode().getParent())
            so.entity.getParentNode().getParent().removeChild(so.entity.getParentNode())
            # set the "point of gravity" of all the cutted nodes to world origin at 0,0,0
            # so we only have to translate them to their new destination when they get pasted
            # the position of the pivot point is considered as the center of gravity
            so.entity.getParentNode().setPosition(so.entity.getParentNode().getPosition() - self.pivot.getPosition())
            self.cutList.append(so)
        self.resetSelection()

    def pasteObjects(self,  ray):
        if len(self.cutList) < 1:
            return

        result = og.Math.intersects(ray, self.dropCollisionPlane)
        if result.first == True:
            i=0
            while i < len(self.cutList):
                self.cutListPreviousNodes[i].addChild(self.cutList[i].entity.getParentNode())
                self.cutList[i].entity.getParentNode().translate(ray.getPoint(result.second))
                i = i+1
        else:
            i=0
            while i < len(self.cutList):
                self.cutListPreviousNodes[i].addChild(self.cutList[i].entity.getParentNode())
                self.cutList[i].entity.getParentNode().translate(ray.getPoint(100.0))
                i = i+1
        self.cutList = []

    def leftMouseUp(self):
        if self.pivot is not None and self.pivot.isTransforming:
            self.pivot.stopTransforming()

    def resetSelection(self):
        for so in self.userSelectionList:
            so.setSelected(False)

        self.userSelectionList = []

        self.listenerDings.reset()
        pass


    def updatePivots(self):
        newPivotPosition = og.Vector3(0, 0, 0)

        for so in self.userSelectionList:
            newPivotPosition += so.entity.getParentNode().getPosition()
        if self.pivot is not None:
            self.pivot.setPosition(newPivotPosition / len(self.userSelectionList))

    def unload(self,  saveOnUnload=True):
        pass

    def save(self):
        pass

    def startDropGameObjectAction(self, classid, ray):
        go = self.gocManager.getGameObjectWithClassId(classid)
        meshFile = go.getMeshFileName()

        if go is not None:
            dropEntity = self.sceneManager.createEntity("dropMesh" + str(self.dropCount), str(meshFile))
            dropNode = self.sceneManager.getRootSceneNode().createChild("dropNode" + str(self.dropCount))
            dropNode.attachObject(dropEntity)

            result = og.Math.intersects(ray, self.dropCollisionPlane)
            if result.first == True:
                dropNode.setPosition(ray.getPoint(result.second))
            else:
                dropNode.setPosition(ray.getPoint(50))

            self.dropGO = GameObjectRepresentation(self.dropCount, classid, dropNode, meshFile)
            dropEntity.setUserObject(self.dropGO)

        self.dropCount += 1

    def moveDropGameObjectAction(self, ray):
        result = og.Math.intersects(ray, self.dropCollisionPlane)
        if result.first == True:
            self.dropGO.setPosition(ray.getPoint(result.second))
        else:
            self.dropGO.setPosition(ray.getPoint(50))

    def stopDropGameObjectAction(self, ray):
        print "sd"

    def startDropModelAction(self, meshFile, ray):
        self.dropEntity = self.sceneManager.createEntity("dropMesh" + str(self.dropCount), str(meshFile))
        self.dropNode = self.sceneManager.getRootSceneNode().createChild("dropNode" + str(self.dropCount))
        self.dropNode.attachObject(self.dropEntity)

        result = og.Math.intersects(ray, self.dropCollisionPlane)
        if result.first == True:
            self.dropNode.setPosition(ray.getPoint(result.second))
        else:
            self.dropNode.setPosition(ray.getPoint(50))

        self.dropCount += 1

    def moveDropModelAction(self, ray):
        result = og.Math.intersects(ray, self.dropCollisionPlane)
        if result.first == True:
            self.dropNode.setPosition(ray.getPoint(result.second))
        else:
            self.dropNode.setPosition(ray.getPoint(50))


    def stopDropModelAction(self, ray):
        pass

