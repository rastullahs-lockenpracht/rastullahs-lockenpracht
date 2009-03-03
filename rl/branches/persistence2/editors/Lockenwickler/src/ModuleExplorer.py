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

#dienstag 24.03 15.50
import sys
from PyQt4.QtCore import *
from PyQt4.QtGui import *
import ogre.renderer.OGRE as og

class NameInputDlg(QDialog):
    def __init__(self, parent = None):
        super(NameInputDlg, self).__init__(parent)
        
        self.nameInput = QLineEdit(self)
        buttonBox = QDialogButtonBox(QDialogButtonBox.Ok | QDialogButtonBox.Cancel)
        layout = QVBoxLayout()
        layout.addWidget(self.nameInput)
        layout.addWidget(buttonBox)
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

#        clearAction= QAction("Clear Window",  self)
#        self.consoleWindow.textEdit.addAction(clearAction)
#        self.consoleWindow.textEdit.setContextMenuPolicy(Qt.ActionsContextMenu)
#        clearAction.setShortcut("Ctrl + R")
#        self.connect(clearAction, SIGNAL("triggered()"), self.consoleWindow.textEdit.clear)

    def setMenuCallback(self, callback):
        self.onMenuCallback = callback
        
    def doMenu(self, point):
        self.onMenuCallback(self.mapToGlobal(point))

class ModuleExplorer(QWidget):
    def __init__(self, parent=None):
        QWidget.__init__(self, parent)
        self.sceneTreeView = ModuleTreeWidget()
        
        self.sceneTreeView.setMenuCallback(self.onMenu)
        self.connect(self.sceneTreeView, SIGNAL("itemClicked (QTreeWidgetItem *,int)"), self.onClick)
        
        vBoxLayout = QVBoxLayout()
        vBoxLayout.addWidget(self.sceneTreeView)

        self.setLayout(vBoxLayout)
        
        self.nodeDict = {}
        
        self.moduleManager = None
        self.mapSelectedCallback = None
        
        self.lastSelectedMap = None
        
    def onClick(self, item, column):
        if self.mapSelectedCallback is None:
            return
        
        name = str(item.text(0))
        if name.startswith("Map: "):
            self.mapSelectedCallback(str(item.parent().text(0)).replace("Scene: ", ""), name.replace("Map: ", ""))
            self.lastSelectedMap = name
        elif name.startswith("Scene: "):
            if item.childCount > 0:
                self.mapSelectedCallback(name.replace("Scene: ", ""), None)
                return
            self.mapSelectedCallback(name.replace("Scene: ", ""), str(item.child(0).text(0)).replace("Map: ", ""))
            self.lastSelectedMap = name
        else:
            self.mapSelectedCallback(str(item.parent().parent().text(0)).replace("Scene: ", ""), str(item.parent().text(0)).replace("Map: ", ""))
            self.lastSelectedMap = name
            

        
    def onMenu(self, point):
        if self.moduleManager is not None:
            menu = QMenu(self)
            
            newSceneAction= QAction("New Scene",  self)
            menu.addAction(newSceneAction)
            self.connect(newSceneAction, SIGNAL("triggered()"), self.onNewScene)

            if self.sceneTreeView.currentItem() is not None and str(self.sceneTreeView.currentItem().text(0)).startswith("Scene:"):
                newMapAction= QAction("New Map",  self)
                menu.addAction(newMapAction)
                self.connect(newMapAction, SIGNAL("triggered()"), self.onNewMap)

            deleteAction= QAction("Delete",  self)
            menu.addAction(deleteAction)
            self.connect(deleteAction, SIGNAL("triggered()"), self.onDelete)
            
            menu.exec_(point)
      
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
        
    def onDelete(self):
        print "delete"
      
    def updateView(self):
        self.sceneTreeView.clear()
        
        for s in self.module.scenes:
            sceneRootItem = QTreeWidgetItem(self.sceneTreeView)
            sceneRootItem.setText(0, "Scene: " + s.name)
            
            for m in s.mapFiles:
                self.parseMap(m, sceneRootItem)


    def parseMap(self, map, sceneRootItem):
        childItem =  QTreeWidgetItem(sceneRootItem)
        mn = "Map: " + map.mapName
        childItem.setText(0, mn)
        if mn == self.lastSelectedMap:
            childItem.setSelected(True)
            childItem.parent().setExpanded(True)
        
        i = 0
        while i < map.mapNode.numChildren():
            childItem2 = QTreeWidgetItem(childItem) 
            childItem2.setText(0, map.mapNode.getChild(i).getName())
            i = i+1


# this crashed in linux
#        iter = map.mapNode.getChildIterator()
#        while iter.hasMoreElements():
#            childItem2 = QTreeWidgetItem(childItem) 
#            val = iter.getNext()
#            if  val is not None:
#                childItem2.setText(0, val.getName())

        
    def setCurrentModule(self, module):
        self.module = module
        self.updateView()

    def setModuleManager(self, moduleManager):
        self.moduleManager = moduleManager
        
    def setMapSelectedCallback(self, callback):
        self.mapSelectedCallback = callback
