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

from PyQt4.QtCore import *
from PyQt4.QtGui import *
 
import ogre.renderer.OGRE as og
 
 
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
 
class Area():
    def __init__(self,sceneManager, type, parentZone, id, position, orientation, scale, meshFile = None, transitiondistance = "0.5"):
        self.type = type
        self.parentZone = parentZone
        if not parentZone:
           raise Exception("No parentZone")

        
        self.areaNode = parentZone.zoneNode.createChildSceneNode("area_" + parentZone.name + " " + str(id))
        self.meshFile = meshFile
        self.areaEntity = None
        self.transitiondistance = transitiondistance
        
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
            self.areaEntity = sceneManager.createEntity("area_" + parentZone.name + str(id) + "_entity", self.meshFile)
            
        self.areaNode.attachObject(self.areaEntity)
        self.areaNode.setPosition(position)
        if orientation is not None:
            self.areaNode.setOrientation(orientation)
        if scale is not None:
            self.areaNode.setScale(scale)
        
        if type == "mesh":
            self.areaNode.setScale(self.areaNode.getScale() + 0.05)
            
class Zone():
    def __init__(self,map, name):
        self.map = map
        self.name = name
        self.areaList = []
        self.lightList = []
        self.soundListList = []
        self.triggerList = []
        self.areaCounter = 0
        
        self.zoneNode =  self.map.mapNode.createChildSceneNode("zone_" + name + "_node")
        
    def addArea(self, sceneManager , type, position, orientation, scale, meshFile = None):
        area = Area(sceneManager, type, self, self.areaCounter, position, orientation, scale, meshFile)
        self.areaList.append(area)
        
        self.areaCounter = self.areaCounter + 1
        
    def __del__(self):
        self.areaNode.getParentSceneNode().detachChild(self.areaNode)
        self.areaNode.detachAllObjects()
        
        self.sceneMgr.destroySceneNode(self.areaNode)
        self.sceneMgr.destroyEntity(self.areaEntity)
        
class ZoneManager():
    def __init__(self, sceneManager):
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
        
        self.zoneList.append(Zone(self.currentMap, name))
        
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
            
            meshAction = self.createAction("convex hull from: " + self.entityUnderMouse.getName(), functools.partial(self.onZoneNameTriggered, zone, "mesh"))
            self.menuList.append(meshAction)
            m.addAction(meshAction)
            
        return menu
        
    def onZoneNameTriggered(self, zone, type):
        if self.newAreaPosition is not None:
            if type == "box":
                zone.addArea(self.sceneManager, "box", self.newAreaPosition, None, None)
        
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
        
        
        
