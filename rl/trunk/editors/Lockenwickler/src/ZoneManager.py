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

import functools
import xml.etree.cElementTree as xml

from PyQt4.QtCore import *
from PyQt4.QtGui import *
 
import ogre.renderer.OGRE as og
 
from ModuleExplorer import NameInputDlg
from TriggerManager import TriggerManager


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
 
       
class Area(og.UserDefinedObject):
    def __init__(self,sceneManager, type, parentZone, id, position, orientation, scale, meshFile = None, transitiondistance = "0.5", subtract = False):
        og.UserDefinedObject.__init__(self)
        self.type = type
        self.parentZone = parentZone
        if not parentZone:
           raise Exception("No parentZone")

        self.id = id # editor internal id for entity and node creation
        self.areaNode = parentZone.zoneNode.createChildSceneNode("area_" + parentZone.name + " " + str(id))
        self.meshFile = meshFile
        self.areaEntity = None
        self.transitiondistance = transitiondistance
        self.__subtract = subtract
        self.sceneManager = sceneManager
        
        if type == "box":
            self.areaEntity = self.sceneManager.createEntity("area_" + parentZone.name + str(id) + "_entity", "UniCube.mesh")
        elif type == "sphere":
            self.areaEntity = self.sceneManager.createEntity("area_" + parentZone.name + str(id) + "_entity", "UniSphere.mesh")        
        elif type == "ellipsoid":
            print "not yet"
        elif type == "capsule":
            print "not yet"
        elif type == "pyramid":
            print "not yet"
        elif type == "mesh":
            try:
                self.areaEntity = sceneManager.createEntity("area_" + parentZone.name + str(id) + "_entity", self.meshFile)
            except:
                print "Mesh " + self.meshFile + " not found!"
                return None
                
        self.areaEntity.setMaterialName("Lockenwickler_Area")
        if self.__subtract:
            self.areaEntity.setMaterialName("Lockenwickler_Area_Subtract")
            
        self.areaEntity.setUserObject(self)
        self.areaNode.attachObject(self.areaEntity)
        self.areaNode.setPosition(position)
        if orientation is not None:
            self.areaNode.setOrientation(orientation.w, orientation.x, orientation.y, orientation.z)
        if scale is not None:
            self.areaNode.setScale(scale)
    
    def __del__(self):
        self.areaNode.detachAllObjects()
        self.areaNode.getParent().removeAndDestroyChild(self.areaNode.getName())
        self.sceneManager.destroyEntity(self.areaEntity.getName())
    
    def getsubtract(self):
        return self.__subtract
        
    def setsubtract(self, val):
        if not val:
            self.areaEntity.setMaterialName("Lockenwickler_Area")
        else:
            self.areaEntity.setMaterialName("Lockenwickler_Area_Subtract")
        
        self.__subtract = val
        
    subtract = property(getsubtract, setsubtract)
    
class Zone():
    def __init__(self,sceneManager, map, name):
        self.map = map
        self.name = name
        self.areaList = []
        self.lightList = []
        self.soundList = []
        self.triggerList = []
        self.areaCounter = 0
        self.sceneManager = sceneManager
        self.isHidden = False
        
        self.zoneNode =  self.sceneManager.getRootSceneNode().createChildSceneNode("zone_" + name + "_node")
        
    def addArea(self, type, position, orientation, scale, meshFile = None):
        area = Area(self.sceneManager, type, self, self.areaCounter, position, orientation, scale, meshFile)
        if area is not None:
            self.areaList.append(area)
            self.areaCounter = self.areaCounter + 1
    
    def deleteArea(self, area):
        for a in self.areaList:
            if a.id == area.id:
                self.areaList.remove(a)
                del a
    
    def addTrigger(self):
        trigger = TriggerManager.instance.createTrigger()
    
    def hide(self):
        self.sceneManager.getRootSceneNode().removeChild(self.zoneNode)
        self.isHidden = True
    
    def show(self):
        self.sceneManager.getRootSceneNode().addChild(self.zoneNode)
        self.isHidden = False
        
        
class ZoneManager():
    instance = None
    
    def __init__(self, sceneManager):
        if ZoneManager.instance == None:
            ZoneManager.instance = self
        
            self.currentMap = None
            self.zoneList = []
            self.menuList = []
            self.entityUnderMouse = None
            self.newAreaPosition = None
            self.sceneManager = sceneManager
        
        
        
    def createZone(self, name):
        if self.currentMap == None:
            print "No map selected!"
            return
            
        z = Zone(self.sceneManager, self.currentMap, name)
        self.zoneList.append(z)
        self.currentMap.zoneList.append(z)
        return z
        
    def parseZonesFromXml(self, zoneXmlNode, map):
        if zoneXmlNode is None:
            return
        
        self.currentMap = map
        
        zoneNodes = zoneXmlNode.getiterator("zone")
        for zone in zoneNodes:
            zoneName = zone.attrib["name"]
            z = self.createZone(zoneName)
            
            areaNodes = zone.getiterator("area")
            for area in areaNodes:
                type = area.attrib["type"]
                meshFile = None
                if type == "mesh":
                    meshFile = area.attrib["meshfile"]
                
                pos = og.Vector3()
                qw = qx = qy = qz = 0
                scale = None
                hasRotation = False
                
                transformations = area.getiterator()
                for t in transformations:
                    if t.tag == "position":
                        posx = float(t.attrib["x"])
                        posy = float(t.attrib["y"])
                        posz = float(t.attrib["z"])
                        pos = og.Vector3(posx, posy, posz)
                    elif t.tag == "rotation":
                        qw = float(t.attrib["qw"])
                        qx = float(t.attrib["qx"])
                        qy = float(t.attrib["qy"])
                        qz = float(t.attrib["qz"])
                        hasRotation = True
                    elif type == "mesh" and t.tag == "scale":
                        scalex = float(t.attrib["x"])
                        scaley = float(t.attrib["y"])
                        scalez = float(t.attrib["z"])
                        scale = og.Vector3(scalex, scaley, scalez)
                    elif t.tag == "size":
                        scalex = float(t.attrib["x"])
                        scaley = float(t.attrib["y"])
                        scalez = float(t.attrib["z"])
                        scale = og.Vector3(scalex, scaley, scalez)
                
                rot = None
                if hasRotation:
                    rot = og.Quaternion(qw, qx, qy, qz)
                    
                z.addArea(type, pos, rot, scale, meshFile)
            
            
            lightNodes = zone.getiterator("light")
            for light in lightNodes:
                name = light.attrib["name"]
                z.lightList.append(name)
                
            soundNodes = zone.getiterator("sound")
            for sound in soundNodes:
                name = sound.attrib["name"]
                z.soundList.append(name)
            

    def saveZonesToXml(self, root, map):
        if len(map.zoneList) == 0:
            return
        
        rootZonesElem = xml.SubElement(root, "zones")
        for zone in map.zoneList:
            zoneElem = xml.SubElement(rootZonesElem, "zone")
            zoneElem.attrib["name"] = zone.name
            
            for area in zone.areaList:
                areaElem = xml.SubElement(zoneElem, "area")
                areaElem.attrib["type"] = area.type
                #print "Saving Zone: " + zone.name + " - Area " + str(area.id)
                if area.type == "mesh" and area.meshFile is not None:
                    areaElem.attrib["meshfile"] = area.meshFile
                    
                posElem = xml.SubElement(areaElem, "position")
                posElem.attrib["x"] = str(area.areaNode.getPosition().x)
                posElem.attrib["y"] = str(area.areaNode.getPosition().y)
                posElem.attrib["z"] = str(area.areaNode.getPosition().z)
                
                rotElem = xml.SubElement(areaElem, "rotation")
                rotElem.attrib["qw"] = str(area.areaNode.getOrientation().w)
                rotElem.attrib["qx"] = str(area.areaNode.getOrientation().x)
                rotElem.attrib["qy"] = str(area.areaNode.getOrientation().y)
                rotElem.attrib["qz"] = str(area.areaNode.getOrientation().z)
                
                scaleElem = None
                if area.type == "mesh":
                    scaleElem = xml.SubElement(areaElem, "scale")
                else:
                    scaleElem = xml.SubElement(areaElem, "size")

                scaleElem.attrib["x"] = str(area.areaNode.getScale().x)
                scaleElem.attrib["y"] = str(area.areaNode.getScale().y)
                scaleElem.attrib["z"] = str(area.areaNode.getScale().z)
                
            for lightName in zone.lightList:
                lightElem = xml.SubElement(zoneElem, "light")
                lightElem.attrib["name"] = str(lightName)
            
            for soundName in zone.soundList:
                soundElem = xml.SubElement(zoneElem, "sound")
                soundElem.attrib["name"] = str(soundName)
                
    def deleteArea(self, area):
        for z in self.zoneList:
            for a in z.areaList:
                if a.id == area.id:
                    z.deleteArea(area)
    
    def getZoneMenu(self):
        self.menuList = []
        menu = QMenu("Add Area")
        
        for zone in self.zoneList:
            m = QMenu(zone.name)
            self.menuList.append(m)
            menu.addMenu(m)
            
            boxAction = self.createAction("box", functools.partial(self.onZoneNameTriggered, zone, "box"))
            self.menuList.append(boxAction)
            m.addAction(boxAction)
            
            sphereAction = self.createAction("sphere", functools.partial(self.onZoneNameTriggered, zone, "sphere"))
            self.menuList.append(sphereAction)
            m.addAction(sphereAction)
            
            ellipsoidAction = self.createAction("ellipsoid", functools.partial(self.onZoneNameTriggered, zone, "ellipsoid"))
            self.menuList.append(ellipsoidAction)
            m.addAction(ellipsoidAction)
            
            capsuleAction = self.createAction("capsule", functools.partial(self.onZoneNameTriggered, zone, "capsule"))
            self.menuList.append(capsuleAction)
            m.addAction(capsuleAction)
            
            pyramidAction = self.createAction("pyramid", functools.partial(self.onZoneNameTriggered, zone, "pyramid"))
            self.menuList.append(pyramidAction)
            m.addAction(pyramidAction)
            
            meshAction = self.createAction("mesh", functools.partial(self.onZoneNameTriggered, zone, "mesh"))
            self.menuList.append(meshAction)
            m.addAction(meshAction)
            
        return menu
        
    def onZoneNameTriggered(self, zone, type):
        if self.newAreaPosition is not None:
            if type == "box":
                zone.addArea("box", self.newAreaPosition, None, None)
            elif type == "sphere":
                zone.addArea("sphere", self.newAreaPosition, None, None)
            elif type == "mesh":
                dlg = NameInputDlg(QApplication.focusWidget())
                resName = ""
                if dlg.exec_():
                    resName = str(dlg.nameInput.text())

                zone.addArea("mesh", self.newAreaPosition, None, None, resName)
                
    def getZone(self, name):
        for zone in self.zoneList:
            if zone.name == name:
                return zone
        
    def getZoneList(self):
        list = []
        for zone in self.zoneList:
            list.append(zone.name)
            
        return list
        
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
        
        
        
