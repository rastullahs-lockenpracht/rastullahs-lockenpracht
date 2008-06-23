import sys
import os
from os.path import isdir
from os.path import isfile

import OgreWidget
import ogre.renderer.OGRE as og
from PyQt4 import QtGui, QtCore

class ModelSelectionDialog(QtGui.QDialog):
    def __init__(self, ogreRoot, parent=None):
        QtGui.QDialog.__init__(self, parent)
        self.ogreRoot = ogreRoot

        self.setupUi()

        QtCore.QObject.connect(self.modelSearchBox, QtCore.SIGNAL("textChanged(QString)"),
                               self.updateModelList)

        QtCore.QObject.connect(self.listWidget, QtCore.SIGNAL("itemSelectionChanged ()"),
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
        self.resize(QtCore.QSize(QtCore.QRect(0,0,272,744).size()).expandedTo(self.minimumSizeHint()))

        self.gridlayout = QtGui.QGridLayout(self)
        self.gridlayout.setObjectName("gridlayout")

        self.modelSearchBox = QtGui.QLineEdit(self)
        self.modelSearchBox.setObjectName("modelSearchBox")
        self.gridlayout.addWidget(self.modelSearchBox,0,0,1,1)

        self.splitter = QtGui.QSplitter(self)
        self.splitter.setOrientation(QtCore.Qt.Vertical)
        self.splitter.setObjectName("splitter")

        self.listWidget = QtGui.QListWidget(self.splitter)
        self.listWidget.setObjectName("listWidget")

        self.ogreModelPrevWindowSceneMgr = self.ogreRoot.createSceneManager(og.ST_GENERIC,"ogreModelPrevWindowSceneMgr")
        self.ogreModelPrevWindow = OgreWidget.OgreWidget("ModelPrevWin", self.ogreRoot, self.ogreModelPrevWindowSceneMgr, "PrevCam",
                                                         self.splitter)
        self.ogreModelPrevWindow.setMinimumSize(QtCore.QSize(200,200))
        self.ogreModelPrevWindow.setObjectName("modelPreviewWindow")
        self.gridlayout.addWidget(self.splitter,1,0,1,1)

        self.retranslateUi()
        QtCore.QObject.connect(self.modelSearchBox,QtCore.SIGNAL("textChanged(QString)"),self.listWidget.clearSelection)
        QtCore.QMetaObject.connectSlotsByName(self)

    def retranslateUi(self):
        self.setWindowTitle(QtGui.QApplication.translate("modelPreviewDialog", "Dialog", None, QtGui.QApplication.UnicodeUTF8))

    def setPreviewedModel(self):
        if self.ent != None:
            self.ogreModelPrevWindowSceneMgr.destroyEntity(self.ent.getName())

        self.nodeScale = og.Vector3(1,1,1)

        self.ent = self.ogreModelPrevWindowSceneMgr.createEntity(str(self.listWidget.currentItem().text()),
                                                                 str(self.listWidget.currentItem().text()))
        self.node.attachObject(self.ent)
        self.node.setScale(og.Vector3(1,1,1))
        bb = self.ent.getBoundingBox()

    def scanDirForModels(self, dir, name):
        for file in os.listdir(dir):
            curFile = dir + "/" + file

            if file.startswith('.'): #ignore dot files (hidden)
                continue
            if isdir(curFile):
                self.scanDirForModels(curFile, file)
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


    def event(self, event):
        if event.type() == 31: # scroll wheel turned
            if event.delta() < 0:
                self.ogreModelPrevWindow.zoomCamera(-5)
            else:
                self.ogreModelPrevWindow.zoomCamera( 5)

        if event.type() == 5: #mouse moved while button down
           rotX = (event.globalX() - self.lastMousePosX) * 0.01
           rotY = (event.globalY() - self.lastMousePosY) * 0.01

           if rotX < 0.1 and rotY < 0.1: # first click, don't do anything at all here
               self.ogreModelPrevWindow.orbitCamera(-rotX,  rotY)

           self.lastMousePosX = event.globalX()
           self.lastMousePosY = event.globalY()

        if event.type() == 3: # mouse released
            self.lastMousePosX = 0
            self.lastMousePosY = 0

        return False
