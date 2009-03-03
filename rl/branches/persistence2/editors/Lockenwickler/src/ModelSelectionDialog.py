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

class ModelListWidget(QListWidget):
    def __init__(self,  parent):
        super(ModelListWidget, self).__init__(parent)
        self.setDragEnabled(True)

    def startDrag(self,  dropActions):
        data = QByteArray()
        stream = QDataStream(data,  QIODevice.WriteOnly)
        stream << self.currentItem().text()
        mimeData = QMimeData()
        mimeData.setData("application/x-static_model", data)
        drag = QDrag(self)
        drag.setMimeData(mimeData)
        drag.start(Qt.CopyAction)

class ModelSelectionDialog(QDialog):
    def __init__(self, ogreRoot, parent=None):
        QDialog.__init__(self, parent)
        self.ogreRoot = ogreRoot

        self.setupUi()

        self.connect(self.modelSearchBox, SIGNAL("textChanged(QString)"),
                               self.updateModelList)

        self.connect(self.listWidget, SIGNAL("itemSelectionChanged ()"),
                               self.setPreviewedModel)

        self.modelList = []

        self.ogreModelPrevWindow.setBackgroundColor(og.ColourValue(0,1,0))

        self.node = self.ogreModelPrevWindowSceneMgr.getRootSceneNode().createChildSceneNode()
        self.ent = None
        self.nodeScale = og.Vector3(1,1,1)

        self.lastMousePosX = 0
        self.lastMousePosY = 0



    def setupUi(self):
        self.setObjectName("modelPreviewDialog")
        self.resize(QSize(QRect(0,0,272,744).size()).expandedTo(self.minimumSizeHint()))

        self.gridlayout = QGridLayout(self)
        self.gridlayout.setObjectName("gridlayout")

        self.modelSearchBox = QLineEdit(self)
        self.modelSearchBox.setObjectName("modelSearchBox")
        self.gridlayout.addWidget(self.modelSearchBox,0,0,1,1)

        self.splitter = QSplitter(self)
        self.splitter.setOrientation(Qt.Vertical)
        self.splitter.setObjectName("splitter")

        self.listWidget = ModelListWidget(self.splitter)
        self.listWidget.setObjectName("listWidget")

        self.ogreModelPrevWindowSceneMgr = self.ogreRoot.createSceneManager(og.ST_GENERIC,"ogreModelPrevWindowSceneMgr")
        self.ogreModelPrevWindow = OgreWidget.OgreWidget("ModelPrevWin", self.ogreRoot, self.ogreModelPrevWindowSceneMgr, "PrevCam",
                                                         self.splitter)
        self.ogreModelPrevWindow.setOgreViewportCreatedCallback(self.ogreViewportCreatedCallback)                                                 
        
        
        self.ogreModelPrevWindow.setMinimumSize(QSize(200,200))
        self.ogreModelPrevWindow.setObjectName("modelPreviewWindow")
        self.gridlayout.addWidget(self.splitter,1,0,1,1)

        self.retranslateUi()

    def ogreViewportCreatedCallback(self):
        self.ogreModelPrevWindow.renderWindow.getViewport(0).setOverlaysEnabled(False)
        
    def retranslateUi(self):
        self.setWindowTitle(QApplication.translate("modelPreviewDialog", "Dialog", None, QApplication.UnicodeUTF8))

    def setPreviewedModel(self):
        if self.ent != None:
            self.ogreModelPrevWindowSceneMgr.destroyEntity(self.ent.getName())

        self.nodeScale = og.Vector3(1,1,1)

        self.ent = self.ogreModelPrevWindowSceneMgr.createEntity(str(self.listWidget.currentItem().text()),
                                                                 str(self.listWidget.currentItem().text()))
        self.node.attachObject(self.ent)
        self.node.setScale(og.Vector3(1,1,1))
        bb = self.ent.getBoundingBox()

    def scanDirForModels(self, dir):
        for file in os.listdir(dir):
            curFile = dir + "/" + file

            if file.startswith('.'): #ignore dot files (hidden)
                continue
            if isdir(curFile):
                self.scanDirForModels(curFile)
                continue
            if isfile(curFile):
                if file.endswith(".mesh"):
                    self.modelList.append(file)
                    self.listWidget.addItem(file)

        self.listWidget.sortItems()

    def updateModelList(self, text):
        self.listWidget.clear()
        for model in self.modelList:
            if model.find(text) != -1:
               self.listWidget.addItem(model)

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
                self.ogreModelPrevWindow.zoomCamera(-5)
            else:
                self.ogreModelPrevWindow.zoomCamera( 5)
            return True

        if event.type() == 5: #mouse moved while button down
            rotX = (event.globalX() - self.lastMousePosX) * 0.01
            rotY = (event.globalY() - self.lastMousePosY) * 0.01

            if rotX < 0.1 and rotY < 0.1: # first click, don't do anything at all here
                self.ogreModelPrevWindow.orbitCamera(-rotX,  rotY)

            self.lastMousePosX = event.globalX()
            self.lastMousePosY = event.globalY()
            return True

        if event.type() == 3: # mouse released
            self.lastMousePosX = 0
            self.lastMousePosY = 0
            return True

        return False

    def updateRenderWindow(self):
        self.ogreModelPrevWindow.update()
