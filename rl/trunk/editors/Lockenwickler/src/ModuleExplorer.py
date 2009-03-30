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
from PyQt4.QtCore import *
from PyQt4.QtGui import *
import ogre.renderer.OGRE as og

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

class ExplorerOptionsDlg(QDialog):
    def __init__(self, lights, gameObjects, entities, zones, parent = None):
        super(ExplorerOptionsDlg, self).__init__(parent)
        
        buttonBox = QDialogButtonBox(QDialogButtonBox.Ok | QDialogButtonBox.Cancel)
        layout = QVBoxLayout()
        
        self.lightCheckBox = QCheckBox("Show Lights")
        self.lightCheckBox.setChecked(lights)        
        layout.addWidget(self.lightCheckBox)
        
        self.gameObjectsCheckBox = QCheckBox("Show Game-Objects")
        self.gameObjectsCheckBox.setChecked(gameObjects)        
        layout.addWidget(self.gameObjectsCheckBox)
        
        self.entitiesCheckBox = QCheckBox("Show Entities")
        self.entitiesCheckBox.setChecked(entities)        
        layout.addWidget(self.entitiesCheckBox)
        
        self.zonesCheckBox = QCheckBox("Show Zones")
        self.zonesCheckBox.setChecked(zones)        
        layout.addWidget(self.zonesCheckBox)
        
        layout.addWidget(buttonBox)
        layout.setContentsMargins(2, 2, 2, 2)
        self.setLayout(layout)
        
        self.connect(buttonBox, SIGNAL("accepted()"), self, SLOT("accept()"))        
        self.connect(buttonBox, SIGNAL("rejected()"), self, SLOT("reject()"))

class NameInputDlg(QDialog):
    def __init__(self, parent = None):
        super(NameInputDlg, self).__init__(parent)
        
        self.nameInput = QLineEdit(self)
        buttonBox = QDialogButtonBox(QDialogButtonBox.Ok | QDialogButtonBox.Cancel)
        layout = QVBoxLayout()
        layout.addWidget(self.nameInput)
        layout.addWidget(buttonBox)
        layout.setContentsMargins(2, 2, 2, 2)
        self.setLayout(layout)
        
        self.connect(buttonBox, SIGNAL("accepted()"), self, SLOT("accept()"))        
        self.connect(buttonBox, SIGNAL("rejected()"), self, SLOT("reject()"))
        
class ModuleTreeWidget(QTreeWidget):
    def __init__(self, parent = None):
        super(ModuleTreeWidget, self).__init__(parent)
        
        self.setContextMenuPolicy(Qt.CustomContextMenu)        
        self.connect(self, SIGNAL("customContextMenuRequested(const QPoint &)"), self.doMenu)

        self.onMenuCallback = None
        self.setAnimated(True)

        self.setHeaderLabels(["Structure", "Visibility"])
        self.setSelectionMode(QAbstractItemView.ExtendedSelection)
        
    def setMenuCallback(self, callback):
        self.onMenuCallback = callback
        
    def doMenu(self, point):
        self.onMenuCallback(point)

class ModuleExplorer(QWidget):
    def __init__(self, parent=None):
        QWidget.__init__(self, parent)
        self.sceneTreeView = ModuleTreeWidget()
        
        self.sceneTreeView.setMenuCallback(self.onMenu)
        self.connect(self.sceneTreeView, SIGNAL("itemClicked (QTreeWidgetItem *,int)"), self.onClick)
        self.connect(self.sceneTreeView, SIGNAL("itemSelectionChanged ()"), self.onSelectionChanged)

        vBoxLayout = QVBoxLayout()
        vBoxLayout.addWidget(self.sceneTreeView)
        vBoxLayout.setContentsMargins(0, 0, 0, 0)
        
        self.setLayout(vBoxLayout)
        self.resize(QSize(QRect(0,0,272,450).size()).expandedTo(self.minimumSizeHint()))

        self.nodeDict = {}
        
        self.moduleManager = None
        self.mapSelectedCallback = None
        self.selectionChangedCallback = None
        self.mapItems = []
        
        self.lastSelectedMap = None
        self.onMenuPoint = None
        
        self.showLights = True
        self.showGameObjects = True
        self.showEntities = True
        self.showZones = True

    def selectItems(self, selectedItems):
        self.disconnect(self.sceneTreeView, SIGNAL("itemSelectionChanged ()"), self.onSelectionChanged)
        self.deselectAll()
        
        if selectedItems is None:
            return
        
        for so in selectedItems:
            nodeName = so.entity.getParentNode().getName()
            items = self.sceneTreeView.findItems(nodeName, Qt.MatchFixedString | Qt.MatchRecursive)
            for item in items:
                self.sceneTreeView.setItemSelected(item, True)
        self.connect(self.sceneTreeView, SIGNAL("itemSelectionChanged ()"), self.onSelectionChanged)
    
    def selectItem(self, so, select):
        self.disconnect(self.sceneTreeView, SIGNAL("itemSelectionChanged ()"), self.onSelectionChanged)
        nodeName = so.entity.getParentNode().getName()
       
        items = None
        
        if nodeName.startswith("light_") and self.showLights: 
            items = self.sceneTreeView.findItems(extractLight(so.entity.getParentNode()).getName(), Qt.MatchFixedString | Qt.MatchRecursive)    
        elif nodeName.startswith("gameobject_") and self.showGameObjects:
            go = so.entity.getUserObject()
            items = self.sceneTreeView.findItems(go.gocName + " id:" + str(go.inWorldId), Qt.MatchFixedString | Qt.MatchRecursive)    
        elif nodeName.startswith("entity_") and self.showEntities:
            items = self.sceneTreeView.findItems(so.entityName, Qt.MatchFixedString | Qt.MatchRecursive)    
        elif nodeName.startswith("area_") and self.showZones:
            area = so.entity.getUserObject()
            items = self.sceneTreeView.findItems("Area " + str(area.id), Qt.MatchFixedString | Qt.MatchRecursive)    
        
        if select and items is not None:
            for item in items:
                self.sceneTreeView.setItemSelected(item, True)
                self.sceneTreeView.expandItem(item)
        else:
            if items is not None:
                for item in items:
                    self.sceneTreeView.setItemSelected(item, False)
        self.connect(self.sceneTreeView, SIGNAL("itemSelectionChanged ()"), self.onSelectionChanged)
        
    def onSelectionChanged(self):
        if self.selectionChangedCallback is None:
            return
            
        nodeNames = []
        items = self.sceneTreeView.selectedItems()
        for item in items:
            name = str(item.data(0, Qt.UserRole).toString())
            if len > 0:
                nodeNames.append(name)
                
        self.selectionChangedCallback(nodeNames)
    def deselectAll(self):
        self.disconnect(self.sceneTreeView, SIGNAL("itemSelectionChanged ()"), self.onSelectionChanged)
        
        for item in self.sceneTreeView.selectedItems():
            self.sceneTreeView.setItemSelected(item, False)
            
        self.connect(self.sceneTreeView, SIGNAL("itemSelectionChanged ()"), self.onSelectionChanged)

    def onClick(self, item, column):
        if self.mapSelectedCallback is None:
            return
        
        name = str(item.text(0))
        if name.startswith("Map: "):
            if column == 1:
                if self.module.getMap(name.replace("Map: ", "")).isHidden:
                    item.setIcon(1 , QIcon("media/icons/14_layer_visible.png"))
                    self.module.getMap(name.replace("Map: ", "")).show()
                else:
                    item.setIcon(1 , QIcon("media/icons/14_layer_invisible.png"))
                    self.module.getMap(name.replace("Map: ", "")).hide()
            
        elif name.startswith("Zone: "):
            if self.moduleManager and column == 1:
                if self.moduleManager.zoneManager.getZone(name.replace("Zone: ", "")).isHidden:
                    item.setIcon(1 , QIcon("media/icons/14_layer_visible.png"))
                    self.moduleManager.zoneManager.getZone(name.replace("Zone: ", "")).show()
                else:
                    item.setIcon(1 , QIcon("media/icons/14_layer_invisible.png"))
                    self.moduleManager.zoneManager.getZone(name.replace("Zone: ", "")).hide()
            
    def onMenu(self, point):
        if self.moduleManager is not None:
            index = self.sceneTreeView.indexAt(point)
            if not index.isValid():
                return
            self.onMenuPoint = point
            
            menu = QMenu(self)
            
            newSceneAction= QAction("New Scene",  self)
            menu.addAction(newSceneAction)
            self.connect(newSceneAction, SIGNAL("triggered()"), self.onNewScene)

            if self.sceneTreeView.currentItem() is not None and str(self.sceneTreeView.currentItem().text(0)).startswith("Scene:"):
                newMapAction= QAction("New Map",  self)
                menu.addAction(newMapAction)
                self.connect(newMapAction, SIGNAL("triggered()"), self.onNewMap)
            elif self.sceneTreeView.currentItem() is not None and str(self.sceneTreeView.currentItem().text(0)).startswith("Map:"):
                item = self.sceneTreeView.itemAt(point)
                self.onClick(item, None)
                
                hideMapAction = QAction("Hide",  self)
                menu.addAction(hideMapAction)
                self.connect(hideMapAction, SIGNAL("triggered()"), self.onHideMap)
                revealMapAction = QAction("Show",  self)
                menu.addAction(revealMapAction)
                self.connect(revealMapAction, SIGNAL("triggered()"), self.onShowMap)
                setActiveMapAction = QAction("Set Active",  self)
                menu.addAction(setActiveMapAction)
                self.connect(setActiveMapAction, SIGNAL("triggered()"), self.onSetActiveMap)
                
            deleteAction= QAction("Delete",  self)
            menu.addAction(deleteAction)
            self.connect(deleteAction, SIGNAL("triggered()"), self.onDelete)
                
            if self.sceneTreeView.currentItem() is not None and str(self.sceneTreeView.currentItem().text(0)).startswith("Map:"):
                menu.addSeparator()
                
                newZoneAction = QAction("New Zone",  self)
                menu.addAction(newZoneAction)
                self.connect(newZoneAction, SIGNAL("triggered()"), self.onNewZone)
            
            menu.addSeparator()
            optionsAction= QAction("Open Explorer Options",  self)
            menu.addAction(optionsAction)
            self.connect(optionsAction, SIGNAL("triggered()"), self.onOptions)
            
            menu.exec_(QCursor().pos())
    
    def onOptions(self):
        dlg = ExplorerOptionsDlg(self.showLights, self.showGameObjects, self.showEntities, self.showZones, self)
        if dlg.exec_():
            self.showLights = dlg.lightCheckBox.isChecked()
            self.showGameObjects = dlg.gameObjectsCheckBox.isChecked()
            self.showEntities = dlg.entitiesCheckBox.isChecked()
            self.showZones = dlg.zonesCheckBox.isChecked()
            self.updateView()
            
    def onHideMap(self):
        if self.module:
            item = self.sceneTreeView.itemAt(self.onMenuPoint)
            item.setIcon(1 , QIcon("media/icons/14_layer_invisible.png"))
            self.module.getMap(self.lastSelectedMap.replace("Map: ", "")).hide()   
            
    def onShowMap(self):
        if self.module:
            item = self.sceneTreeView.itemAt(self.onMenuPoint)
            item.setIcon(1, QIcon("media/icons/14_layer_visible.png"))
            self.module.getMap(self.lastSelectedMap.replace("Map: ", "")).show()
      
    def onNewScene(self):
        dlg = NameInputDlg(self)
        if dlg.exec_():
            self.moduleManager.addSceneToModule(str(dlg.nameInput.text()))
            self.updateView()
            self.onNewMap()
            
    def onNewMap(self):
        dlg = NameInputDlg(self)
        if dlg.exec_():
            sceneName = str(self.sceneTreeView.currentItem().text(0)).replace("Scene: ", "")
            self.moduleManager.addMapToScene(sceneName, str(dlg.nameInput.text()))
            self.updateView()
    
    def onNewZone(self):
        dlg = NameInputDlg(self)
        if dlg.exec_():
            self.moduleManager.addZoneToMap(str(dlg.nameInput.text()))
            self.updateView()
    
    def onDelete(self):
        print "delete"
        
    def paintLastSelectedMapBlue(self):
        print self.lastSelectedMap
        for item in self.mapItems:
            if str(item.text(0)) == self.lastSelectedMap:
                brush = item.foreground(0)
                brush.setColor(QColor("blue"))
                item.setForeground(0, brush)
                item.setIcon(0, QIcon("media/icons/2rightarrow.png"))
            else:
                brush = item.foreground(0)
                brush.setColor(QColor("black"))
                item.setForeground(0, brush)
                item.setIcon(0, QIcon())
                
    def onSetActiveMap(self):
        item = self.sceneTreeView.currentItem()
        self.lastSelectedMap = str(item.text(0))
        self.paintLastSelectedMapBlue()
        sceneName = str(item.parent().text(0).replace("Scene: ", ""))
        mapName = str(item.text(0).replace("Map: ", ""))
        self.mapSelectedCallback(sceneName, mapName)
        
    def updateView(self):
        self.mapItems = []
        self.sceneTreeView.clear()
        
        for s in self.module.scenes:
            sceneRootItem = QTreeWidgetItem(self.sceneTreeView)
            sceneRootItem.setText(0, "Scene: " + s.name)
            
            for m in s.mapFiles:
                self.parseMap(m, sceneRootItem)
        
        self.paintLastSelectedMapBlue()

    def parseMap(self, map, sceneRootItem):
        childItem =  QTreeWidgetItem(sceneRootItem)
        self.mapItems.append(childItem)
        sceneRootItem.setExpanded(True)
        mn = "Map: " + map.mapName
        
        childItem.setIcon(1 , QIcon("media/icons/14_layer_visible.png"))
        if map.isHidden:
            childItem.setIcon(1 , QIcon("media/icons/14_layer_invisible.png"))
        
        childItem.setText(0, mn)
        if mn == self.lastSelectedMap:
            childItem.setSelected(True)
            childItem.setExpanded(True)
            
        if self.showZones:
            self.parseZone(map, childItem)
            
        i = 0
        while i < map.mapNode.numChildren(): 
            if map.mapNode.getChild(i).getName().startswith("light_") and self.showLights:
                childItem2 = QTreeWidgetItem(childItem) 
                childItem2.setData(0, Qt.UserRole, QVariant(map.mapNode.getChild(i).getName()))
                childItem2.setText(0, extractLight(map.mapNode.getChild(i)).getName()) 
            elif map.mapNode.getChild(i).getName().startswith("gameobject_") and self.showGameObjects:
                childItem2 = QTreeWidgetItem(childItem) 
                go = map.mapNode.getChild(i).getAttachedObject(0).getUserObject()
                childItem2.setData(0, Qt.UserRole, QVariant(map.mapNode.getChild(i).getName()))
                childItem2.setText(0, go.gocName + " id:" + str(go.inWorldId)) 
            elif map.mapNode.getChild(i).getName().startswith("entity_") and self.showEntities:
                childItem2 = QTreeWidgetItem(childItem) 
                childItem2.setData(0, Qt.UserRole, QVariant(map.mapNode.getChild(i).getName()))
                childItem2.setText(0, map.mapNode.getChild(i).getAttachedObject(0).getName()) 
            i = i+1

        
#            thetype = None
#            try:
#                thetype = str(type(map.mapNode.getChild(i).getAttachedObject(0)))
#            except:
#                i = i+1
#                continue
#                
#            childItem3 = QTreeWidgetItem(childItem2) 
#            childItem3.setText(0, thetype)
#            i = i+1
            

# this crashed in linux
#        iter = map.mapNode.getChildIterator()
#        while iter.hasMoreElements():
#            childItem2 = QTreeWidgetItem(childItem) 
#            val = iter.getNext()
#            if  val is not None:
#                childItem2.setText(0, val.getName())

    def parseZone(self, map, parentItem):
        for zone in map.zoneList:
            childItem = QTreeWidgetItem(parentItem) 
            childItem.setText(0, "Zone: " + zone.name)
            childItem.setData(0, Qt.UserRole, QVariant(zone.zoneNode.getName()))
            childItem.setIcon(0, QIcon("media/icons/dissociatecell.png"))
            childItem.setIcon(1, QIcon("media/icons/14_layer_visible.png"))
            
            i = 0
            for area in zone.areaList:
                childItem2 = QTreeWidgetItem(childItem)
                childItem2.setText(0, "Area " + str(area.id))
                childItem2.setData(0, Qt.UserRole, QVariant(area.areaNode.getName()))
                i += 1
                
    def setCurrentModule(self, module):
        self.module = module
        self.updateView()

    def setModuleManager(self, moduleManager):
        self.moduleManager = moduleManager
        
    def setMapSelectedCallback(self, callback):
        self.mapSelectedCallback = callback

    def setSelectionChangedCallback(self, callback):
        self.selectionChangedCallback = callback
