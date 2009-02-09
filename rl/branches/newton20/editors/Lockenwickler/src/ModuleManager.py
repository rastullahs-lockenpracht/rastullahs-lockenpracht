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

import xml.etree.cElementTree as xml

import ctypes
import ogre.renderer.OGRE as og

from PyQt4.QtCore import *
from PyQt4.QtGui import *

from SelectionBuffer import *
from MovePivot import *
from GameObjectClassManager import *


#                <zone name="Testzone">
#                        <area type="sphere">
#                                <position x="-10" y="0" z="-5"/>
#                                <scale x="6" y="6" z="6"/>
#                                <transition_distance>0.5</transition_distance>
#                        </area>
#                        <area type="mesh" meshfile="arc_UnbHaus_07.mesh">
#                                <position x="25" y="0" z="-50"/>
#                                <transition_distance>0.5</transition_distance>
#                        </area>
#                        <area type="sphere" subtract="true">
#                                <position x="-11" y="0" z="-4"/>
#                                <scale x="2" y="2" z="2"/>
#                        </area>
#                        <light name="red pointlight"/>
#                        <light name="green spotlight"/>
#                        <sound name="ruchin001.ogg"/>
#                        <trigger name="test" classname="TestTrigger">
#                                <property name="message" type="STRING" data="You triggered the dooms day device!" />
#                        </trigger>
#                </zone>

class Map():
    def __init__(self, pathToFile, sceneManager, ogreRoot, gocManager):
        self.pathToMapFile = pathToFile
        
        mapName = pathToFile.replace("\\", "/")
        mapName = mapName.split("/")
        mapName = mapName[len(mapName) - 1].split(".")
        mapName = mapName[len(mapName) - 3]
        self.mapName = mapName
        
        self.sceneManager = sceneManager
        self.mapNode = sceneManager.getRootSceneNode().createChildSceneNode(self.pathToMapFile)
        self.ogreRoot = ogreRoot
        self.gocManager = gocManager

        xmlTree = xml.parse(pathToFile)
        root = xmlTree.getroot()

        if root.attrib["formatVersion"] == "0.4.0":
            self.parseMapNodes(root.find("nodes"))
            #self.parseMapZones(root.find("zones"))
        else:
            print pathToFile + " has wrong format version. It needs to be 0.4.0"
            return

    def parseMapNodes(self, nodeElement):
        nodes = nodeElement.getiterator("entity")
        
        self.createEntites(nodes)

        nodes = nodeElement.getiterator("light")
        self.createLights(nodes)

        nodes = nodeElement.getiterator("sound")
        self.createSound(nodes)

        nodes = nodeElement.getiterator("gameobject")
        self.createGameObjects(nodes)

        nodes = nodeElement.getiterator("particlesystem")
        self.createParticleSystems(nodes)

    def createEntites(self, entityNodes):
        for nodes in entityNodes:
            entityName = nodes.attrib["name"]
            meshFile = nodes.attrib["meshfile"]
            nodePosition = None
            nodeScale = None
            qw = qx = qy = qz = None

            transformations = nodes.getiterator()
            for t in transformations:
                if t.tag == "position":
                    posx = float(t.attrib["x"])
                    posy = float(t.attrib["y"])
                    posz = float(t.attrib["z"])
                    nodePosition = og.Vector3(posx, posy, posz)
                elif t.tag == "rotation":
                    qw = float(t.attrib["qw"])
                    qx = float(t.attrib["qx"])
                    qy = float(t.attrib["qy"])
                    qz = float(t.attrib["qz"])
                elif t.tag == "scale":
                    scalex = float(t.attrib["x"])
                    scaley = float(t.attrib["y"])
                    scalez = float(t.attrib["z"])
                    nodeScale = og.Vector3(scalex, scaley, scalez)

            try:
                e = self.sceneManager.createEntity(entityName, meshFile)
            except:
                print "Warning: Meshfile " + meshFile + " could not be found."
                continue

            n = self.mapNode.createChild(entityName + "_node")
            n.attachObject(e)
            n.setPosition(nodePosition)
            n.setOrientation(qw, qx, qy, qz)
            n.setScale(nodeScale)

    def createLights(self, lightNodes):
        for l in lightNodes:
            lightName = l.attrib["name"]
            lightType = l.attrib["type"]
            lightVisible = bool(l.attrib["visible"])
            castShadows = bool(l.attrib["castShadows"])
            lightPosition = None
            colourDiffuse = None
            colourSpecular = None
            lightAttenuationRange = None
            lightAttenuationConstant= None
            lightAttenuationlinear = None
            lightAttenuationQuadratic = None

            transformations = l.getiterator()
            for t in transformations:
                if t.tag == "position":
                    x = float(t.attrib["x"])
                    y = float(t.attrib["y"])
                    z = float(t.attrib["z"])
                    lightPosition = og.Vector3(x, y, z)
                elif t.tag == "colourDiffuse":
                    r = float(t.attrib["r"])
                    g = float(t.attrib["g"])
                    b= float(t.attrib["b"])
                    colourDiffuse = og.ColourValue(r, g, b)
                elif t.tag == "colourSpecular":
                    r = float(t.attrib["r"])
                    g = float(t.attrib["g"])
                    b= float(t.attrib["b"])
                    colourSpecular = og.ColourValue(r, g, b)
                elif t.tag == "lightAttenuation":
                    lightAttenuationRange = float(t.attrib["range"])
                    lightAttenuationConstant= float(t.attrib["constant"])
                    lightAttenuationlinear = float(t.attrib["linear"])
                    lightAttenuationQuadratic = float(t.attrib["quadratic"])

            e = self.sceneManager.createEntity(lightName, "lightbulp.mesh")
            n = self.mapNode.createChild(lightName + "_node")
            n.attachObject(e)
            n.setPosition(lightPosition)

    def createSound(self, soundNodes):
        #raise NotImplementedError
        return

    def createGameObjects(self, gameObjectNodes):
        for g in gameObjectNodes:
            classid = g.attrib["class"]
            id = int(g.attrib["id"])
            state = g.attrib["state"]
            nodePosition = None
            nodeRotation = None
            nodeScale = None

            transformations = g.getiterator()
            for t in transformations:
                if t.tag == "position":
                    x = float(t.attrib["x"])
                    y = float(t.attrib["y"])
                    z = float(t.attrib["z"])
                    nodePosition = og.Vector3(x, y, z)
                elif t.tag == "rotation":
                    qw = float(t.attrib["qw"])
                    qx = float(t.attrib["qx"])
                    qy = float(t.attrib["qy"])
                    qz = float(t.attrib["qz"])
                    nodeRotation = og.Quaternion(qw, qx, qy, qz)
                elif t.tag == "scale":
                    x = float(t.attrib["x"])
                    y = float(t.attrib["y"])
                    z = float(t.attrib["z"])
                    nodeScale = og.Vector3(x, y, z)

            go = self.gocManager.getGameObjectWithClassId(classid)
            if go is not None:
                meshFile = go.getMeshFileName()
                ent = self.sceneManager.createEntity("dropMesh" + str(id), str(meshFile))
                dropNode = self.sceneManager.getRootSceneNode().createChild("dropNode" + str(id))
                dropNode.attachObject(ent)

                if nodePosition:
                    dropNode.setPosition(nodePosition)
                if nodeRotation:
                    dropNode.setOrientation(nodeRotation)
                if nodeScale:
                    dropNode.setScale(nodeScale)

                go = GameObjectRepresentation(id, classid, dropNode, meshFile)
                go.inWorldId = id
                ent.setUserObject(go)


    def createParticleSystems(self, particleNodes):
        #raise NotImplementedError
        return

class Scene():
    def __init__(self, moduleroot, pathToFile, sceneManager, ogreRoot, gocManager):
        self.moduleRoot = moduleroot
        self.sceneManager = sceneManager
        self.ogreRoot = ogreRoot
        self.gocManager = gocManager
        
        xmlTree = xml.parse(pathToFile)
        root = xmlTree.getroot()
        self.name = root.attrib["name"]
        
        self.mapFiles = [] # a list in case the module has more than one map file
        mappaths = []
        
        maps = root.getiterator("map")
        for m in maps:
            mappaths.append(join(self.moduleRoot, join("maps", m.attrib["file"])))
            
        for m in mappaths:
            self.mapFiles.append(Map(m, self.sceneManager, self.ogreRoot, self.gocManager))
            
        #cmd = join(self.moduleRoot, "maps/*.rlmap.xml")
        #maps = glob.glob(cmd)
        #for m in maps:
            #self.mapFiles.append(Map(m, self.sceneManager, self.ogreRoot, self.gocManager))


class Module():
    def __init__(self,name, modulePath, sceneManager, ogreRoot, gameObjectManager):
        self.sceneManager = sceneManager
        self.ogreRoot = ogreRoot
        self.gocManager = gameObjectManager
        
        self.name = name
        self.moduleRoot = join(modulePath, name)
        self.__isCommon = False

        self.hasDependencies = False
        self.moduleDependencies = []


        self.gofFiles = [] # gof File list

        self.scenes =[]

        self.isLoaded = False

    def isCommon(self):
        modConfig = join(self.moduleRoot,  "scripts/moduleconfig.rb")
        if isfile(modConfig): # is the modconfig existing?
            f = codecs.open(modConfig, 'r', 'utf-8')
        else:
            print ("Error: couldn't find module config")
            return
        isDependencieLine = False
        for i, line in enumerate(f):
            lStripped = line.strip() #strip the whitespace away, not needed here
            if lStripped.startswith("super("):
                split = lStripped.split(",")
                if split[2].strip() == unicode("true"):
                    self.__isCommon = True
                    return True


            elif isDependencieLine:
                if lStripped == "end":
                    isDependencieLine = False
                else:
                    self.hasDependencies = True
                    self.moduleDependencies.append(lStripped.split('"')[1])

            elif lStripped == "def getDependencies()":
                isDependencieLine = True

        return False

    def load(self):
        if self.isLoaded:
            return

        self.isLoaded = True
        modConfig = join(self.moduleRoot,  "scripts/moduleconfig.rb")
        if isfile(modConfig): # is the modconfig existing?
            f = codecs.open(modConfig, 'r', 'utf-8')
        else:
            print ("Error: couldn't find module config")
            return

        #for i, line in enumerate(f):
            #lStripped = line.strip() #strip the whitespace away, not needed here

        self.setResourcePaths()
        
        try:
            og.ResourceGroupManager.getSingleton().initialiseAllResourceGroups()
        except og.OgreException, e:
            print e
        
        cmd = join(self.moduleRoot, "dsa/*.gof")
        self.gofFiles = glob.glob(cmd)
        self.gocManager.parseGOFFiles(self.gofFiles)

        if not self.isCommon():
            cmd = join(self.moduleRoot, "maps/*.rlscene")
            sceneFile = glob.glob(cmd)
            self.loadScenes(sceneFile)
            
        
    def loadScenes(self, sceneFiles):
        for f in sceneFiles:
            self.scenes.append(Scene(self.moduleRoot, f, self.sceneManager, self.ogreRoot, self.gocManager))

    def saveMaps(self):
        return

    def saveScenes(self):
        return

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
            if os.path.isfile(curFile):
                pass

class ModuleManager():
    def __init__(self,  ogreRoot,  sceneManager):
        self.sceneManager = sceneManager
        self.ogreRoot = ogreRoot
        self.modelSelectionDialog = None

        self.moduleCfgPath = ""

        self.gocManager = GameObjectClassManager()
        # we need to hold a reference to the game object representaions ourself
        # python does not recognize the a reference to a c++ object (Entity in our case) is passed
        # and deletes the object
        self.gameObjectRepresentationDict = []

        self.mainModule = []
        self.mainModuledependencieList =[]
        self.moduleList = []
        self.userSelectionList = []
        self.cutList = [] # selection objects that has been cut out and wait to be pasted again
        self.cutListPreviousNodes = [] # contains the nodes they where copnnected to before the cut

        self.moduleExplorer = None

        self.lastRay = None
        self.rayLine = None

        # pivot is initialzed and set in the Lockenwickler.setUpOgre function
        self.pivot = None
        self.movingPivot = False

        self.leftMouseDown = False
        self.middleMouseDown = False
        self.rightMouseDown = False

        self.dropCount = 0
        self.dropNode = None
        self.dropEntity = None
        self.dropCollisionPlane = og.Plane(og.Vector3().UNIT_Y, og.Vector3().ZERO)

        self.numerOfCopys = 0 #everytime a copy is made this numer is increased to generate unique node and mesh names
        self.moduleConfigIsParsed = False

        self.selectionBuffer = None
    
    def resetParsedModuleConfig(self):
        self.moduleConfigIsParsed = False
        self.moduleList = []

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
                self.moduleList.append(Module(str, self.moduleCfgPath.replace("/modules.cfg",  ""), self.sceneManager, self.ogreRoot, self.gocManager))

        self.moduleConfigIsParsed = True


    def moduleExists(self, name):
        lowerA = str(name).lower()
        
        for m in self.moduleList:
            lowerB = m.name.lower()
            if lowerA == lowerB:
                return True
        
        return False

    def openLoadModuleDialog(self):
        self.moduleFolder = str(self.moduleCfgPath.replace("modules.cfg", ""))

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
            self.loadModule(str(list.currentItem().text()))

    # I'm sorry for this
    def loadModule(self, moduleName):
        for m in self.moduleList:
            if m.name == moduleName:
                if m.hasDependencies: # load modules on wich the main module depends before the main module is loaded
                    for moduleDependencie in m.moduleDependencies:
                        for m2 in self.moduleList:
                            if m2.name == moduleDependencie:
                                m2.load()
                                self.modelSelectionDialog.scanDirForModels(m2.moduleRoot)
                                self.mainModuledependencieList.append(m2)

                m.load()
                self.modelSelectionDialog.scanDirForModels(m.moduleRoot)
                self.mainModule = m
                self.moduleExplorer.setCurrentModule(m)
                
#        self.moduleExplorer.updateView()
#        n = self.sceneManager.getRootSceneNode().createChildSceneNode()
#        e = self.sceneManager.createEntity("west342wt346t",  "UniCube.mesh")
#        e.setMaterialName("PlainColor")
#        e.getSubEntity(0).setCustomParameter(1, og.Vector4(0.0, 0.0, 1.0, 1.0))
#
#        e2 = self.sceneManager.createEntity("west342wt34635t",  "UniSphere.mesh")
#        e2.setMaterialName("PlainColor")
#        e2.getSubEntity(0).setCustomParameter(1, og.Vector4(0, 1, 0, 1))
#        n.attachObject(e)
#        n.attachObject(e2)
#        n.setScale(og.Vector3(10, 5, 20))
#        
        if self.selectionBuffer is None:
            self.selectionBuffer = SelectionBuffer(self.sceneManager, self.ogreRoot.getRenderTarget("OgreMainWin"))

    # called when a click into Main Ogre Window occurs
    def selectionClick(self, screenX, screenY, ray,  controlDown=False,  shiftDown=False):
        so = None
        
        if self.selectionBuffer is not None:
            so = self.selectionBuffer.onSelectionClick(screenX, screenY)
        
        
        
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

        if self.rayLine == None:
            self.rayLine = self.sceneManager.createManualObject("rayLine")
            self.rayLine.setDynamic(True)
            self.sceneManager.getRootSceneNode().createChildSceneNode("raynode").attachObject(self.rayLine)

            self.rayLine.begin("BaseWhiteNoLighting", og.RenderOperation.OT_LINE_STRIP)

            self.rayLine.position(ray.getOrigin())
            self.rayLine.position( ray.getPoint(10000))

            self.rayLine.end()

        else:
            self.rayLine.beginUpdate(0)

            self.rayLine.position(ray.getOrigin())
            self.rayLine.position( ray.getPoint(10000))

            self.rayLine.end()

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

        if go is not None:
            meshFile = go.getMeshFileName()
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

