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
import os
from os.path import isdir
from os.path import isfile

from PyQt4.QtGui import *
from PyQt4.QtCore import *

import OgreWidget
import ogre.renderer.OGRE as og

# The drag events are processed in ObgreMainWindow.py

class MaterialListWidget(QListWidget):
    def __init__(self,  parent):
        super(MaterialListWidget, self).__init__(parent)
        self.setDragEnabled(True)

    def startDrag(self,  dropActions):
        data = QByteArray()
        stream = QDataStream(data,  QIODevice.WriteOnly)
        stream << self.currentItem().text()
        mimeData = QMimeData()
        mimeData.setData("application/x-material", data)
        drag = QDrag(self)
        drag.setMimeData(mimeData)
        drag.start(Qt.CopyAction)

class MaterialSelectionDialog(QDialog):
    def __init__(self, ogreRoot, parent=None):
        QDialog.__init__(self, parent)
        self.ogreRoot = ogreRoot

        self.setupUi()

        self.connect(self.materialSearchBox, SIGNAL("textChanged(QString)"),
                               self.updateMaterialList)

        self.connect(self.listWidget, SIGNAL("itemSelectionChanged ()"),
                               self.setPreviewedMaterial)

        self.materialList = []

        self.ogreMaterialPrevWindow.setBackgroundColor(og.ColourValue(0,1,0))

        self.node = self.ogreMaterialPrevWindowSceneMgr.getRootSceneNode().createChildSceneNode()
        self.ent = None
        self.nodeScale = og.Vector3(1,1,1)

        self.lastMousePosX = 0
        self.lastMousePosY = 0



    def setupUi(self):
        self.setObjectName("materialPreviewDialog")
        self.resize(QSize(QRect(0,0,272,744).size()).expandedTo(self.minimumSizeHint()))

        self.gridlayout = QGridLayout(self)
        self.gridlayout.setObjectName("materialSelectionLayout")
        self.gridlayout.setContentsMargins(2, 2, 2, 2)
        
        self.materialSearchBox = QLineEdit(self)
        self.materialSearchBox.setObjectName("materialSearchBox")
        self.gridlayout.addWidget(self.materialSearchBox,0,0,1,1)

        self.splitter = QSplitter(self)
        self.splitter.setOrientation(Qt.Vertical)
        self.splitter.setObjectName("splitter")

        self.listWidget = MaterialListWidget(self.splitter)
        self.listWidget.setObjectName("listWidget")

        self.ogreMaterialPrevWindowSceneMgr = self.ogreRoot.createSceneManager(og.ST_GENERIC,"ogreMaterialPrevWindowSceneMgr")
        self.ogreMaterialPrevWindow = OgreWidget.OgreWidget("MaterialPrevWin", self.ogreRoot, self.ogreMaterialPrevWindowSceneMgr, "MaterialPrevCam",
                                                         self.splitter)
        self.ogreMaterialPrevWindow.setBackgroundColor(og.ColourValue(1.0, 1.0, 1.0, 1.0))
        self.ogreMaterialPrevWindow.setOgreViewportCreatedCallback(self.ogreViewportCreatedCallback)                                                 
        
        
        self.ogreMaterialPrevWindow.setMinimumSize(QSize(200,200))
        self.ogreMaterialPrevWindow.setObjectName("materialPreviewWindow")
        self.gridlayout.addWidget(self.splitter,1,0,1,1)

        self.retranslateUi()

    def ogreViewportCreatedCallback(self):
        self.ogreMaterialPrevWindow.renderWindow.getViewport(0).setOverlaysEnabled(False)
        
    def retranslateUi(self):
        self.setWindowTitle(QApplication.translate("materialPreviewDialog", "Dialog", None, QApplication.UnicodeUTF8))

    def setPreviewedMaterial(self):
        if self.ent == None:
            self.ent = self.ogreMaterialPrevWindowSceneMgr.createEntity("MaterialPrevEntity9993944", "UniCube.mesh")
            self.nodeScale = og.Vector3(1,1,1)

            self.node.attachObject(self.ent)
            self.node.setScale(og.Vector3(6,6,6))
        self.ent.setMaterialName(str(self.listWidget.currentItem().text()))

    def scanDirForMaterials(self, dir):
        for file in os.listdir(dir):
            curFile = dir + "/" + file

            if file.startswith('.'): #ignore dot files (hidden)
                continue
            if isdir(curFile):
                self.scanDirForMaterials(curFile)
                continue
            if isfile(curFile):
                if file.endswith(".material"):
                    f = open(curFile, "r")
                    for line in f:
                        if line.startswith("material "):
                            l = line.replace("material ", "").lstrip().rstrip()
                            l2 = l.split(":")
                            self.materialList.append(l2[0])
                            self.listWidget.addItem(l2[0])
                        
                    f.close()
        self.listWidget.sortItems()

    def updateMaterialList(self, text):
        self.listWidget.clear()
        for material in self.materialList:
            if material.find(text) != -1:
               self.listWidget.addItem(material)

        self.listWidget.sortItems()

    def eventFilter(self, obj, event):
        if event.type() == 5:
            self.startDrag()
            event.accept()
            return True

        return False

    def event(self, event):
        if event.type() == 31: # scroll wheel turned
            if event.delta() < 0:
                self.ogreMaterialPrevWindow.zoomCamera(-5)
            else:
                self.ogreMaterialPrevWindow.zoomCamera( 5)
            return True

        if event.type() == 5: #mouse moved while button down
            rotX = (event.globalX() - self.lastMousePosX) * 0.01
            rotY = (event.globalY() - self.lastMousePosY) * 0.01

            if rotX < 0.1 and rotY < 0.1: # first click, don't do anything at all here
                self.ogreMaterialPrevWindow.orbitCamera(-rotX,  rotY)

            self.lastMousePosX = event.globalX()
            self.lastMousePosY = event.globalY()
            return True

        if event.type() == 3: # mouse released
            self.lastMousePosX = 0
            self.lastMousePosY = 0
            return True

        return False

    def updateRenderWindow(self):
        self.ogreMaterialPrevWindow.update()
