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
from DepthBuffer import *
from MovePivot import *
from GameObjectClassManager import *
from MyRaySceneQueryListener import *


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

# get the light out of a light node
def extractLight(node):
        i = 0
        num = node.numAttachedObjects()
        while i < node.numAttachedObjects():
            c = node.getAttachedObject(i)
            tp = str(type(c))
            if tp == "<class 'ogre.renderer.OGRE._ogre_.Light'>":
                return c
            
            i += 1

# make the xml file more pretty
def indent(elem, level=0):
    i = "\n" + level*"  "
    
    if len(elem):
        if not elem.text or not elem.text.strip():
            elem.text = i + "  "
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
        for elem in elem:
            indent(elem, level+1)
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
    else:
        if level and (not elem.tail or not elem.tail.strip()):
            elem.tail = i

# creates unique names for new entities
def createUniqueEntityName(sceneManager, name = None):
    n = ""
    if name is None:
        n = "dropMesh" + str(ModuleManager.dropCount)
    else:
        n = name
        
    while sceneManager.hasEntity(n):
        n = "dropMesh" + str(ModuleManager.dropCount)
        ModuleManager.dropCount += 1
        
    return n
        
def printVector3(vec):
    print str(vec.x) + ";" + str(vec.y) + ";" + str(vec.z)
    
class EntityCustomOptions(og.UserDefinedObject):
    def __init__(self, receivesShadow = True, staticgeometrygroup = 0, physicsproxytype = "none", renderingdistance = "20000"):
        og.UserDefinedObject.__init__(self)
        self.receivesShadow = receivesShadow
        self.staticgeometrygroup = staticgeometrygroup
        self.physicsproxytype = physicsproxytype
        self.renderingdistance = renderingdistance
        self.materialName = "NotChanged"
        
        ModuleManager.entityCustomOptionsDict.append(self)
        
    def copy(self):
            return EntityCustomOptions(self.receivesShadow, self.staticgeometrygroup, self.physicsproxytype, self.renderingdistance)
        
    def getType(self):
            return "EntityCustomOptions"

class Map():
    def __init__(self, pathToFile, sceneManager, ogreRoot, gocManager, emptyMap = False):
        self.pathToMapFile = pathToFile
        
        mapName = pathToFile.replace("\\", "/")
        mapName = mapName.split("/")
        mapName = mapName[len(mapName) - 1].split(".")
        mapName = mapName[len(mapName) - 3]
        self.mapName = mapName
        self.mapFileName = self.mapName + ".rlmap.xml"
        
        self.sceneManager = sceneManager
        self.mapNode = sceneManager.getRootSceneNode().createChildSceneNode(self.pathToMapFile)
        self.ogreRoot = ogreRoot
        self.gocManager = gocManager
        self.isHidden = False
        
        if not emptyMap:
            xmlTree = xml.parse(pathToFile)
            root = xmlTree.getroot()

            if root.attrib["formatVersion"] == "0.4.0":
                self.parseMapNodes(root.find("nodes"))
                #self.parseMapZones(root.find("zones"))
            else:
                print pathToFile + " has wrong format version. It needs to be 0.4.0"
                return

    def hide(self):
        try:
            self.sceneManager.getRootSceneNode().removeChild(self.mapNode)
        except:
            print "Error: map is already hidden!"
            return
        
        self.isHidden = True
        
    def show(self):
        try:
            self.sceneManager.getRootSceneNode().addChild(self.mapNode)
        except:
            print "Error: map is already shown!"
            return
        
        self.isHidden = False
    
    def parseMapNodes(self, nodeElement):
        nodes = nodeElement.getiterator("gameobject")
        self.createGameObjects(nodes)
        
        nodes = nodeElement.getiterator("entity")
        self.createEntites(nodes)

        nodes = nodeElement.getiterator("light")
        self.createLights(nodes)

        nodes = nodeElement.getiterator("sound")
        self.createSound(nodes)

        nodes = nodeElement.getiterator("particlesystem")
        self.createParticleSystems(nodes)

    def createEntites(self, entityNodes):
        for nodes in entityNodes:
            entityName = nodes.attrib["name"]
            
            if entityName.startswith("dropMesh"):
                num = int(entityName.replace("dropMesh",  ""))
                if ModuleManager.dropCount < num:
                    ModuleManager.dropCount = num
                elif ModuleManager.dropCount < num:
                    ModuleManager.dropCount = num + 1
                    
            meshFile = nodes.attrib["meshfile"]
            
            eco = EntityCustomOptions()
            
            try:
                if nodes.attrib["receivesShadow"] == "False" or nodes.attrib["receivesShadow"] == "false":
                    eco.receivesShadow = "False"
            except:
                pass
            try:
                eco.staticgeometrygroup = int(nodes.attrib["staticgeometrygroup"])
            except:
                pass
            try:
                eco.physicsproxy = nodes.attrib["physicsproxy"]
            except:
                pass
            try:
                eco.renderingdistance = float(nodes.attrib["renderingdistance"])
            except:
                pass
            try:
                eco.renderingdistance
                nodes.attrib["materialName"]
            except:
                pass
                
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
                e = self.sceneManager.createEntity(createUniqueEntityName(self.sceneManager,  entityName), meshFile)
            except:
                print "Warning: Meshfile " + meshFile + " could not be found."
                return
            

            e.setUserObject(eco)
            n = self.mapNode.createChild("entity_" + entityName + "_node")
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
            lightDirection = None
            colourDiffuse = None
            colourSpecular = None
            lightAttenuationRange = None
            lightAttenuationConstant= None
            lightAttenuationLinear = None
            lightAttenuationQuadratic = None
            spotlightinner = None
            spotlightouter = None
            falloff = None 
            

                
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
                    lightAttenuationLinear = float(t.attrib["linear"])
                    lightAttenuationQuadric  = float(t.attrib["quadratic"])
                elif t.tag == "spotlightrange":
                    spotlightinner = float(t.attrib["inner"])
                    spotlightouter = float(t.attrib["outer"])
                    falloff = float(t.attrib["falloff"])
                    
            light = self.sceneManager.createLight(lightName)
            
            if lightType == "point":
                light.setType(og.Light.LT_POINT)            
            elif lightType == "spot":
                light.setType(og.Light.LT_SPOTLIGHT)
            elif lightType == "directional":
                light.setType(og.Light.LT_DIRECTIONAL)
            
            light.setVisible(lightVisible)
            light.setCastShadows(castShadows)
            if lightAttenuationRange is not None and lightAttenuationConstant is not None and lightAttenuationLinear is not None and lightAttenuationQuadric is not None:
                light.setAttenuation(lightAttenuationRange, lightAttenuationConstant, lightAttenuationLinear, lightAttenuationQuadric)
            if colourDiffuse:
                light.setDiffuseColour(colourDiffuse)
            if colourSpecular:
                light.setSpecularColour(colourSpecular)
            if spotlightinner and spotlightouter and spotlightouter: 
                light.setSpotlightRange(spotlightinner, spotlightouter, falloff)
            

                
            e = self.sceneManager.createEntity(lightName + "_ent", "lightbulp.mesh")
            n = self.mapNode.createChild("light_" + lightName + "_node")
            n.attachObject(light)
            n.attachObject(e)
            if lightPosition:
                n.setPosition(lightPosition)
            
    def createSound(self, soundNodes):
        #raise NotImplementedError
        return

    def createGameObjects(self, gameObjectNodes):
        for g in gameObjectNodes:
            classid = g.attrib["class"]
            
            id = int(g.attrib["id"])
            if ModuleManager.dropCount < id:
                ModuleManager.dropCount = id
            elif ModuleManager.dropCount < id:
                ModuleManager.dropCount = id + 1
                
            state = g.attrib["state"]
            nodePosition = None
            nodeRotation = None

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

            go = self.gocManager.getGameObjectWithClassId(classid)
            if go is not None:
                meshFile = go.getMeshFileName()
                
                ent = self.sceneManager.createEntity(createUniqueEntityName(self.sceneManager), str(meshFile))
                dropNode = self.mapNode.createChild("gameobject_" + "dropNode" + str(id))
                dropNode.attachObject(ent)

                if nodePosition:
                    dropNode.setPosition(nodePosition)
                if nodeRotation:
                    dropNode.setOrientation(nodeRotation)

                go = GameObjectRepresentation(id, classid, dropNode, meshFile)
                self.gocManager.addGameObjectRepresentation(go)
                go.inWorldId = id
                go.state = state
                ent.setUserObject(go)


    def createParticleSystems(self, particleNodes):
        #raise NotImplementedError
        return
        
    def save(self):
        root = xml.Element("rastullahmap")
        root.attrib["formatVersion"] = "0.4.0"
        
        nodesElem = xml.SubElement(root, "nodes")
        
        i = 0
        while i < self.mapNode.numChildren():
            n = self.mapNode.getChild(i)
            if n.numAttachedObjects() > 0:
                if n.name.startswith("entity_"):
                    entElem = xml.SubElement(nodesElem, "entity")
                    entElem.attrib["name"] = n.getAttachedObject(0).getName()
                    entElem.attrib["meshfile"] = n.getAttachedObject(0).getMesh().getName()
   
                    entElem.attrib["receivesShadow"] = str(n.getAttachedObject(0).getUserObject().receivesShadow).lower()
                    entElem.attrib["staticgeometrygroup"] = str(n.getAttachedObject(0).getUserObject().staticgeometrygroup)
                    entElem.attrib["physicsproxytype"] = str(n.getAttachedObject(0).getUserObject().physicsproxytype)
                    entElem.attrib["renderingdistance"] = str(n.getAttachedObject(0).getUserObject().renderingdistance)
                    
                    posElem = xml.SubElement(entElem, "position")
                    posElem.attrib["x"] = str(n.getPosition().x)
                    posElem.attrib["y"] = str(n.getPosition().y)
                    posElem.attrib["z"] = str(n.getPosition().z)
                    
                    rotElem = xml.SubElement(entElem, "rotation")
                    rotElem.attrib["qw"] = str(n.getOrientation().w)
                    rotElem.attrib["qx"] = str(n.getOrientation().x)
                    rotElem.attrib["qy"] = str(n.getOrientation().y)
                    rotElem.attrib["qz"] = str(n.getOrientation().z)
                    
                    scaleElem = xml.SubElement(entElem, "scale")
                    scaleElem.attrib["x"] = str(n.getScale().x)
                    scaleElem.attrib["y"] = str(n.getScale().y)
                    scaleElem.attrib["z"] = str(n.getScale().z)
                    
                elif n.name.startswith("gameobject_"):
                    goElem = xml.SubElement(nodesElem, "gameobject")
                    mname = n.name

                    goElem.attrib["class"] = str(n.getAttachedObject(0).getUserObject().gocName)
                    goElem.attrib["state"] = str(n.getAttachedObject(0).getUserObject().state)
                    goElem.attrib["id"] = str(n.getAttachedObject(0).getUserObject().inWorldId)
                    
                    posElem = xml.SubElement(goElem, "position")
                    posElem.attrib["x"] = str(n.getPosition().x)
                    posElem.attrib["y"] = str(n.getPosition().y)
                    posElem.attrib["z"] = str(n.getPosition().z)
                    
                    rotElem = xml.SubElement(goElem, "rotation")
                    rotElem.attrib["qw"] = str(n.getOrientation().w)
                    rotElem.attrib["qx"] = str(n.getOrientation().x)
                    rotElem.attrib["qy"] = str(n.getOrientation().y)
                    rotElem.attrib["qz"] = str(n.getOrientation().z)
                    
                elif n.name.startswith("light_"):
                    light = extractLight(n)
                    lightName = light.getName()
                    lightType = light.getType()
                    isVisible = "true"
                    if not light.getVisible():
                        isVisible = "false"
                    
                    castShadows = "false"
                    if light.getCastShadows():
                        castShadows = "true"
                    
                    if lightType == og.Light.LT_POINT:
                        lightType = "point"
                    elif lightType == og.Light.LT_SPOTLIGHT:
                        lightType = "spot"
                    elif lightType == og.Light.LT_DIRECTIONAL:
                        lightType = "directional"
                    
                    
                    lightElem = xml.SubElement(nodesElem, "light")
                    lightElem.attrib["name"] = lightName
                    lightElem.attrib["type"] = lightType
                    lightElem.attrib["visible"] = isVisible
                    lightElem.attrib["castShadows"] = castShadows
                    
                    if lightType == "point" or lightType == "spot":
                        posElem = xml.SubElement(lightElem, "position")
                        posElem.attrib["x"] = str(n.getPosition().x)
                        posElem.attrib["y"] = str(n.getPosition().y)
                        posElem.attrib["z"] = str(n.getPosition().z)
                    
                    colDiffuseElem = xml.SubElement(lightElem, "colourDiffuse")
                    colDiffuseElem.attrib["r"] = str(light.getDiffuseColour().r)
                    colDiffuseElem.attrib["g"] = str(light.getDiffuseColour().g)
                    colDiffuseElem.attrib["b"] = str(light.getDiffuseColour().b)

                    colSpecularElem = xml.SubElement(lightElem, "colourSpecular")
                    colSpecularElem.attrib["r"] = str(light.getSpecularColour().r)
                    colSpecularElem.attrib["g"] = str(light.getSpecularColour().g)
                    colSpecularElem.attrib["b"] = str(light.getSpecularColour().b)
                    
                    lightAttenuationElem = xml.SubElement(lightElem, "lightAttenuation")
                    lightAttenuationElem.attrib["range"] = str(light.getAttenuationRange())
                    lightAttenuationElem.attrib["constant"] = str(light.getAttenuationConstant())
                    lightAttenuationElem.attrib["linear"] = str(light.getAttenuationLinear())
                    lightAttenuationElem.attrib["quadratic"] = str(light.getAttenuationQuadric())
                    
                    if lightType == "spot":
                        spotligthRangeElem = xml.SubElement(lightElem, "spotlightrange")
                        spotligthRangeElem.attrib["inner"] = str(light.getSpotlightInnerAngle().valueDegrees())
                        spotligthRangeElem.attrib["outer"] = str(light.getSpotlightOuterAngle().valueDegrees())
                        spotligthRangeElem.attrib["falloff"] = str(light.getSpotlightFalloff())
                        
                    if lightType == "spot" or lightType == "directional":
                        directionElem = xml.SubElement(lightElem, "direction")
                        dir = og.Vector3()
                        n.getOrientation().ToAxes(dir)
                        directionElem.attrib["x"] = str(dir.x)
                        directionElem.attrib["y"] = str(dir.y)
                        directionElem.attrib["z"] = str(dir.z)
                        
            i = i+1
            
        indent(root)
        xml.ElementTree(root).write(self.pathToMapFile)
        
# caused a linux crash
#        iter = self.mapNode.getChildIterator()
#        while iter.hasMoreElements():
#            name = iter.getNext().getName()
#            print name

class Scene():
    def __init__(self, moduleroot, pathToFile, sceneManager, ogreRoot, gocManager, emptyScene = False, sceneName = "NewScene"):
        self.moduleRoot = moduleroot
        self.pathToFile = pathToFile
        self.sceneManager = sceneManager
        self.ogreRoot = ogreRoot
        self.gocManager = gocManager
        self.mapFiles = [] # a list in case the module has more than one map file
        mappaths = []
        self.name = sceneName

        
        if not emptyScene:
            xmlTree = xml.parse(pathToFile)
            root = xmlTree.getroot()
            self.name = root.attrib["name"]
        
            maps = root.getiterator("map")
            for m in maps:
                mappaths.append(join(self.moduleRoot, join("maps", m.attrib["file"])))
                
            for m in mappaths:
                self.mapFiles.append(Map(m, self.sceneManager, self.ogreRoot, self.gocManager))
            
    def addMap(self, name):
        path = join(self.moduleRoot, join("maps", name + ".rlmap.xml"))
        self.mapFiles.append(Map(path, self.sceneManager, self.ogreRoot, self.gocManager, True))
        
    def save(self):
        root = xml.Element("scene")
        root.attrib["name"] = self.name
        
        for m in self.mapFiles:
            sub = xml.SubElement(root, "map")
            sub.attrib["file"] = m.mapFileName
            m.save()

        indent(root)
        xml.ElementTree(root).write(self.pathToFile)



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

        self.modConfig = join(self.moduleRoot,  "scripts/moduleconfig.rb")

        self.gofFiles = [] # gof File list

        self.scenes =[]

        self.isLoaded = False
        
        self.playerStart = None
        
    def addScene(self, name):
        self.scenes.append(Scene(self.moduleRoot, join(self.moduleRoot, ("maps/" + name + ".rlscene")), self.sceneManager, self.ogreRoot, self.gocManager, True, name))
    
    def addMapToScene(self, sceneName, mapName):
        for scene in self.scenes:
            if scene.name == sceneName:
                scene.addMap(mapName)
                return
        
        print "ERROR: could not find scene: " + sceneName
        
    
    def isCommon(self):
        if isfile(self.modConfig): # is the modconfig existing?
            f = codecs.open(self.modConfig, 'r', 'utf-8')
        else:
            print ("Module.isCommon() Error: couldn't find module config")
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
                    pl = lStripped.split('"')
                    i = 1
                    while i < 100: 
                        try:
                            self.moduleDependencies.append(pl[i])
                            i += 2
                        except IndexError, e:
                            break

            elif lStripped == "def getDependencies()":
                isDependencieLine = True

        return False

    def load(self):
        if self.isLoaded:
            return

        self.isLoaded = True
        self.modConfig = join(self.moduleRoot,  "scripts/moduleconfig.rb")
        if isfile(self.modConfig): # is the modconfig existing?
            f = codecs.open(self.modConfig, 'r', 'utf-8')
        else:
            print ("Module.load: Error: couldn't find module config")
            return

        for line in f:
            lStripped = line.strip() #strip the whitespace away, not needed here
            if lStripped.startswith("hero = $GOM.getGameObject("):
                try:
                    self.playerStart = int(line.split("(")[1].split(")")[0])
                except ValueError, e:
                    print self.modConfig + " ValueError: " + str(e)
                    self.playerStart = None
                    continue
                    
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
        for s in sceneFiles:
            self.scenes.append(Scene(self.moduleRoot, s, self.sceneManager, self.ogreRoot, self.gocManager))

    def save(self):
        for s in self.scenes:
            s.save()
            
        self.saveModuleConfig()

    def saveModuleConfig(self):
        if self.playerStart is not None:
            f = open(self.modConfig, "r")
            
            newconfig = ""
            for line in f:
                if line.startswith("       hero = $GOM.getGameObject("):
                    newconfig += "       hero = $GOM.getGameObject(" + str(self.playerStart) + ");\n"
                else:
                    newconfig += line
            f.close()
            
            f = open(self.modConfig, "w")
            f.write(newconfig)
            f.close()

    def setResourcePaths(self, recurseFolder = ""):
        if recurseFolder == "":
            rootFolder = self.moduleRoot
        else:
            rootFolder = join(self.moduleRoot, recurseFolder)

        for file in os.listdir(rootFolder):
            curFile = join(rootFolder, file)

            if file.startswith('.'): #ignore dot files (hidden)
                continue
            elif os.path.isdir(curFile):
                og.ResourceGroupManager.getSingleton().addResourceLocation(curFile, "FileSystem", self.name, False)
                self.setResourcePaths(curFile)
            elif os.path.isfile(curFile):
                continue
                
    def getMap(self, mapName, sceneName = None):
        if sceneName is not None:
            for s in self.scenes:
                if s.name == sceneName:
                    for m in s.mapFiles:
                        if m.mapName == mapName:
                            return m
        else:
            for s in self.scenes:
                for m in s.mapFiles:
                        if m.mapName == mapName:
                            return m
                            
class ModuleManager():
    dropCount = 0
    entityCustomOptionsDict = []
    
    def __init__(self,  ogreRoot,  sceneManager):
        self.sceneManager = sceneManager
        self.ogreRoot = ogreRoot
        self.modelSelectionDialog = None
        self.materialSelectionDialog = None

        self.moduleCfgPath = ""

        self.gocManager = GameObjectClassManager()
        

        self.mainModule = None
        self.mainModuledependencieList =[]
        self.moduleList = []
        self.userSelectionList = []
        self.cutList = [] # selection objects that has been cut out and wait to be pasted again
        self.cutListPreviousNodes = [] # contains the nodes they where copnnected to before the cut
        self.currentMap = None
        self.moduleExplorer = None

        self.lastRay = None
        self.rayLine = None

        # pivot is initialzed and set in the Lockenwickler.setUpOgre function
        self.pivot = None
        self.movingPivot = False

        self.leftMouseDown = False
        self.middleMouseDown = False
        self.rightMouseDown = False

       
        self.dropNode = None
        self.dropEntity = None
        self.dropCollisionPlane = og.Plane(og.Vector3().UNIT_Y, og.Vector3().ZERO)
        self.dropMat = None
        
        self.numerOfCopys = 0 #everytime a copy is made this numer is increased to generate unique node and mesh names
        self.moduleConfigIsParsed = False

        self.selectionBuffer = None
        self.depthBuffer = None
        self.propertyWindow = None
    
        self.oneClickEntityPlacement = False
        
        self.onContextMenuCallback = None
        self.contextMenuClickPosition = None
        self.contextMenuRay = None
        
        self.playerStartGameObjectId = None
        
        self.entityCustomOptionsDict = []
        
        self.raySceneQueryListener = MyRaySceneQueryListener()
        
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
                                self.materialSelectionDialog.scanDirForMaterials(m2.moduleRoot)
                                self.mainModuledependencieList.append(m2)

                m.load()
                self.modelSelectionDialog.scanDirForModels(m.moduleRoot)
                self.materialSelectionDialog.scanDirForMaterials(m.moduleRoot)
                self.mainModule = m
                self.moduleExplorer.setCurrentModule(m)
                
#        self.moduleExplorer.updateView()
#        ModuleManager.dropCount += 1
#        n = self.sceneManager.getRootSceneNode().createChildSceneNode()
#        e = self.sceneManager.createEntity("west342wt346t",  "UniCube.mesh")
#        e.setMaterialName("DepthMap")
#
#        e2 = self.sceneManager.createEntity("west342wt34635t",  "UniSphere.mesh")
#        e2.setMaterialName("PlainColor")
#        e2.getSubEntity(0).setCustomParameter(1, og.Vector4(0, 1, 0, 1))
#        n.attachObject(e)
#        n.attachObject(e2)
#        n.setScale(og.Vector3(10, 5, 20))
        
        if self.selectionBuffer is None:
            self.selectionBuffer = SelectionBuffer(self.sceneManager, self.ogreRoot.getRenderTarget("OgreMainWin"))

#        if self.depthBuffer is None:
#            self.depthBuffer = DepthBuffer(self.sceneManager, self.ogreRoot.getRenderTarget("OgreMainWin"))

    def addSceneToModule(self, name):
        if self.mainModule is not None:
            self.mainModule.addScene(name)

    def addMapToScene(self, sceneName, mapName):
        if self.mainModule is not None:
            self.mainModule.addMapToScene(sceneName, mapName)

    def setModuleExplorer(self, moduleExplorer):
        self.moduleExplorer = moduleExplorer
        self.moduleExplorer.setMapSelectedCallback(self.selectMapCallback)
        self.moduleExplorer.setModuleManager(self)
    
    def setPropertyWindow(self, propertyWin):
        self.propertyWindow = propertyWin
        
    def selectMapCallback(self, sceneName, mapName):
        self.currentMap = self.mainModule.getMap(mapName, sceneName)
        if self.currentMap is None:
            print "Don't forget to select a map"

        
    # called when a click into Main Ogre Window occurs
    def selectionClick(self, screenX, screenY, ray,  controlDown=False,  shiftDown=False):
        if self.oneClickEntityPlacement:
            if self.modelSelectionDialog.listWidget.currentItem() is not None:
                meshFile = str(self.modelSelectionDialog.listWidget.currentItem().text())
                self.startDropModelAction(meshFile, ray)
                return
            else:
                print "Warning: OneClickEntityPlacement still runed on without any selected mesh!"
                return
                
        #self.depthBuffer.onSelectionClick(screenX, screenY)
        
        so = None
        if self.selectionBuffer is not None:
            so = self.selectionBuffer.onSelectionClick(screenX, screenY)
        
        if so is not None:
            if not so.isPivot:
                self.propertyWindow.showProperties(so)
                
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
            self.propertyWindow.clear()
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
            node = so.entity.getParentNode()

            if node.getName().startswith("light_"):
                light = extractLight(node)
                self.sceneManager.destroyLight(light)
                
            node.detachAllObjects()
            self.sceneManager.destroySceneNode(node)
            self.sceneManager.destroyEntity(so.entity)
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
        if len(self.userSelectionList) < 1 or self.currentMap is None:
            print "Warning: No map selected!"
            return

        newSelectionList = []

        for so in self.userSelectionList:
            if so.entity.getUserObject() is not None:
                if str(so.entity.getParentNode().getName()).startswith("gameobject_"):
                    go = self.gocManager.getGameObjectWithClassId(so.entity.getUserObject().gocName)
                    meshFile = go.getMeshFileName()

                    if go is not None:
                        newEntity = self.sceneManager.createEntity(createUniqueEntityName(self.sceneManager), str(meshFile))
                        newNode = self.currentMap.mapNode.createChild("gameobject_dropNode" + str(ModuleManager.dropCount))
                        newNode.attachObject(newEntity)
                        newNode.setPosition(so.entity.getParentNode().getPosition())

                        newGO = GameObjectRepresentation(ModuleManager.dropCount, so.entity.getUserObject().gocName, newNode, meshFile)
                        self.gocManager.addGameObjectRepresentation(newGO)
                        newEntity.setUserObject(newGO)
                        #newGO.setPosition(og.Vector3(0, 0, 0))

                        newSO = SelectionObject(newEntity)
                        newSO.setSelected(True)
                        newSelectionList.append(newSO)
                        ModuleManager.dropCount += 1
                elif str(so.entity.getParentNode().getName()).startswith("entity_"):
                    nodeName = "entity_dropNode" + str(ModuleManager.dropCount)
                    newNode = self.currentMap.mapNode.createChild(nodeName)

                    entityName = createUniqueEntityName(self.sceneManager)
                    newEntity = self.sceneManager.createEntity(entityName, so.entity.getMesh().getName())

                    eco = so.entity.getUserObject().copy()
                    newEntity.setUserObject(eco)

                    newNode.attachObject(newEntity)
                    newNode.setPosition(so.entity.getParentNode().getPosition())
                    newNode.setOrientation(so.entity.getParentNode().getOrientation())
                    newNode.setScale(so.entity.getParentNode().getScale())

                    newSO = SelectionObject(newEntity)
                    newSO.setSelected(True)
                    newSelectionList.append(newSO)
                    ModuleManager.dropCount += 1
                elif str(so.entity.getParentNode().getName()).startswith("light_"):
                    print "Can't copy lights yet :)"

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
            self.propertyWindow.updateProperties()
            self.moduleExplorer.updateView()
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
        self.mainModule.save()

    def startDropGameObjectAction(self, classid, ray):
        if self.currentMap is None:
            print "No map selected!"
            return
            
        go = self.gocManager.getGameObjectWithClassId(classid)

        if go is not None:
            meshFile = go.getMeshFileName()
            dropEntity = self.sceneManager.createEntity(createUniqueEntityName(self.sceneManager), str(meshFile))
            dropNode = self.currentMap.mapNode.createChild("gameobject_dropNode" + str(ModuleManager.dropCount))
            dropNode.attachObject(dropEntity)

            result = og.Math.intersects(ray, self.dropCollisionPlane)
            if result.first == True:
                dropNode.setPosition(ray.getPoint(result.second))
            else:
                dropNode.setPosition(ray.getPoint(50))

            self.dropGO = GameObjectRepresentation(ModuleManager.dropCount, classid, dropNode, meshFile)
            self.gocManager.addGameObjectRepresentation(self.dropGO)
            dropEntity.setUserObject(self.dropGO)

        ModuleManager.dropCount += 1

    def moveDropGameObjectAction(self, ray):
        if self.currentMap is None:
            return
        
        result = og.Math.intersects(ray, self.dropCollisionPlane)
        if result.first == True:
            self.dropGO.setPosition(ray.getPoint(result.second))
        else:
            self.dropGO.setPosition(ray.getPoint(50))

    def finishDropGameObjectAction(self, ray):
        self.dropGO = None
        return

    def startDropModelAction(self, meshFile, ray):
        if self.currentMap is None:
            print "No map selected!"
            return
            
        self.dropEntity = self.sceneManager.createEntity(createUniqueEntityName(self.sceneManager), meshFile)
        
        eco = EntityCustomOptions()
        self.dropEntity.setUserObject(eco)
        
        self.dropNode = self.currentMap.mapNode.createChild("entity_dropNode" + str(ModuleManager.dropCount))
        self.dropNode.attachObject(self.dropEntity)

        result = og.Math.intersects(ray, self.dropCollisionPlane)
        if result.first == True:
            self.dropNode.setPosition(ray.getPoint(result.second))
        else:
            self.dropNode.setPosition(ray.getPoint(50))

        ModuleManager.dropCount += 1

    def moveDropModelAction(self, ray):
        if self.currentMap is None:
            return
            
        result = og.Math.intersects(ray, self.dropCollisionPlane)
        if result.first == True:
            self.dropNode.setPosition(ray.getPoint(result.second))
        else:
            self.dropNode.setPosition(ray.getPoint(50))
    
    def finishDropModelAction(self, ray):
        return

    def startDropMaterialAction(self, text):
        self.dropMat = text
        
    def moveDropMaterialAction(self, event):
        return

    def finishDropMaterialAction(self, screenX, screenY):
        so = self.selectionBuffer.onSelectionClick(screenX, screenY)
        if so is not None:
            if not so.entity.getNumSubEntities() > 1:
                so.entity.setMaterialName(self.dropMat)
                so.entity.getUserObject().materialName = self.dropMat
            else:
                i = 0
                text = "Warning this Entity has more than one SubEntities with the folloing materials: \n\n"
                while i < so.entity.getNumSubEntities():
                    text += "SubMesh" + str(i) + ":  " + so.entity.getSubEntity(i).getMaterialName() + "\n"
                    i += 1
                
                text += "\n Replace the materials?"
                reply = QMessageBox.question(None, "Warning: multiple materials",  text,  QMessageBox.Yes|QMessageBox.No)
                if reply == QMessageBox.Cancel:
                    return
                if reply == QMessageBox.Yes:
                    so.entity.setMaterialName(self.dropMat)
                    so.entity.getUserObject().materialName = self.dropMat
        
    def setOneClickEntityPlacement(self, state):
        self.oneClickEntityPlacement = state
    
    def createLight(self, name):
        pos = og.Vector3()
        
        query = self.sceneManager.createRayQuery(self.contextMenuRay)
        query.ray = self.contextMenuRay
        query.setSortByDistance(True)
        query.execute(self.raySceneQueryListener)
        if self.raySceneQueryListener.dist < 100000:
            pos = self.contextMenuRay.getPoint(self.raySceneQueryListener.dist)
            self.raySceneQueryListener.dist = 100000
            
#        so = self.selectionBuffer.onSelectionClick(int(self.contextMenuClickPosition.x), int(self.contextMenuClickPosition.y))
#        if so is not None:
#            result = og.Math.intersects(self.contextMenuRay, so.entity.getBoundingBox())
#            
#            if result.first:
#                pos = self.contextMenuRay.getPoint(result.second)
#                pos += so.entity.getParentNode().getPosition()
        
        light = None
        if not self.sceneManager.hasLight(name):
            light = self.sceneManager.createLight(name)
            
        return light,  pos
        
    def addPointLight(self):
        if self.currentMap is None:
            print "No map selected!"
            return
            
        lightName = "pointLight" + str(ModuleManager.dropCount)
        ModuleManager.dropCount += 1
        
        light, pos = self.createLight(lightName)
        printVector3(pos)
        
        if not light:
            print "Error while creating light"
            return
            
        light.setType(og.Light.LT_POINT)
        
        e = self.sceneManager.createEntity(lightName + "_ent", "lightbulp.mesh")
        n = self.currentMap.mapNode.createChild("light_" + lightName + "_node")
        n.attachObject(light)
        n.attachObject(e)
        n.setPosition(pos)
        
    def addSpotLight(self):
        if self.currentMap is None:
            print "No map selected!"
            return
            
        lightName = "spotLight" + str(ModuleManager.dropCount)
        ModuleManager.dropCount += 1
        
        light, pos = self.createLight(lightName)
        printVector3(pos)
        
        if not light:
            print "Error while creating light"
            return
            
        light.setType(og.Light.LT_SPOTLIGHT)
        
        e = self.sceneManager.createEntity(lightName + "_ent", "lightbulp.mesh")
        n = self.currentMap.mapNode.createChild("light_" + lightName + "_node")
        n.attachObject(light)
        n.attachObject(e)
        n.setPosition(pos)
        
    def createZone(self):
        print "creating zone here..."
    
    def setPlayerStart(self):
        self.mainModule.playerStart = str(self.playerStartGameObjectId)
        print "setting Player Start to " + str(self.playerStartGameObjectId)
    
    def onContextMenu(self, screenX, screenY, ray):
        menus = []
        actions = []
        actions.append(self.createAction("Create Zone here", self.createZone))
        pla = self.createAction("Pointlight", self.addPointLight, None, "idea.png")
        pls = self.createAction("Spotlight", self.addSpotLight, None, "idea.png")
        
        lightMenu = QMenu("Add Light")
        lightMenu.addAction(pla)
        lightMenu.addAction(pls)
        menus.append(lightMenu)
        
        so = self.selectionBuffer.onSelectionClick(screenX, screenY)
        self.contextMenuClickPosition = og.Vector2(screenX, screenY)
        self.contextMenuRay = ray
        
        if so is not None and so.entity.getParentNode().getName().startswith("gameobject_"):
            actions.append(self.createAction("Set Player Starterpoint", self.setPlayerStart))
            self.playerStartGameObjectId = so.entity.getUserObject().inWorldId
        
        if self.onContextMenuCallback is not None:
            self.onContextMenuCallback(actions,  menus)

    def setContextMenuCallback(self, callback):
        self.onContextMenuCallback = callback

    def createAction(self, text, slot=None, shortcut=None, icon=None, tip=None, checkable=False, signal="triggered()"):
        action = QAction(text, None)
        if icon is not None:
            action.setIcon(QIcon("media/icons/%s" % icon))
        if shortcut is not None:
            action.setShortcut(shortcut)
        if tip is not None:
            action.setToolTip(tip)
            action.setStatusTip(tip)
        if slot is not None:
            QWidget.connect(action, SIGNAL(signal), slot)

        action.setCheckable(checkable)

        return action
