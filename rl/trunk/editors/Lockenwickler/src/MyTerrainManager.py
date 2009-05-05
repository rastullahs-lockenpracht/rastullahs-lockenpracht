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

from PyQt4.QtCore import *
from PyQt4.QtGui import *

import ogre.renderer.OGRE as og
import ogre.addons.et as ET

import Ui_MyTerrainManagerDock
from GlobDirectoryWalker import GlobDirectoryWalker

class BrushDialog(QDialog):
    def __init__(self, parent = None):
        super(BrushDialog, self).__init__(parent)
        self.setupUi(self)
        self.brushName = ""
        
        for file in GlobDirectoryWalker("./media/terrain/brushes", "*.png"):
            btn = QListWidgetItem()
            btn.setText(file.replace("./media/terrain/brushes/", ""))
            btn.setIcon(QIcon(file))
            self.listWidget.addItem(btn)
        
    def setupUi(self, Dialog):
        Dialog.setObjectName("Dialog")
        Dialog.resize(343, 388)
        self.gridLayout = QGridLayout(Dialog)
        self.gridLayout.setObjectName("gridLayout")
        self.pushButton = QPushButton(Dialog)
        self.pushButton.setObjectName("pushButton")
        self.gridLayout.addWidget(self.pushButton, 1, 0, 1, 1)
        self.pushButton_2 = QPushButton(Dialog)
        self.pushButton_2.setObjectName("pushButton_2")
        self.gridLayout.addWidget(self.pushButton_2, 1, 1, 1, 1)
        self.listWidget = QListWidget(Dialog)
        self.listWidget.setObjectName("listWidget")
        self.gridLayout.addWidget(self.listWidget, 0, 0, 1, 2)

        self.retranslateUi(Dialog)
        QObject.connect(self.pushButton_2, SIGNAL("pressed()"), Dialog.reject)
        QObject.connect(self.pushButton, SIGNAL("pressed()"), Dialog.accept)
        QObject.connect(self.listWidget, SIGNAL("doubleClicked(QModelIndex)"), Dialog.saveBrushName)
        QObject.connect(self.listWidget, SIGNAL("doubleClicked(QModelIndex)"), Dialog.accept)
        QMetaObject.connectSlotsByName(Dialog)

    def retranslateUi(self, Dialog):
        Dialog.setWindowTitle(QApplication.translate("Dialog", "Dialog", None, QApplication.UnicodeUTF8))
        self.pushButton.setText(QApplication.translate("Dialog", "Ok", None, QApplication.UnicodeUTF8))
        self.pushButton_2.setText(QApplication.translate("Dialog", "Cancel", None, QApplication.UnicodeUTF8))
    
    def saveBrushName(self):
        item = self.listWidget.currentItem()
        self.brushName = str(item.text())
        
class MyTerrainManagerDock(Ui_MyTerrainManagerDock.Ui_TerrainManagerUi, QWidget):
    def __init__(self, terrainManager, onBrushChangedCallback, parent = None):
        super(MyTerrainManagerDock, self).__init__(parent)
        self.setupUi(self)
        self.terrainManager = terrainManager
        QWidget.connect(self.createTerrainButton, SIGNAL("clicked()"), self.onCreateTerrainButtonClicked)
        QWidget.connect(self.deleteTerrainButton, SIGNAL("clicked()"), self.onDeleteTerrainButtonClicked)
        QWidget.connect(self.brushButton, SIGNAL("clicked()"), self.onBrushButtonClicked)
        QWidget.connect(self.terrainListComboBox, SIGNAL("currentIndexChanged ( const QString &)"), self.onTerrainChanged)
        QWidget.connect(self.toggleTerrainVisibilityButton, SIGNAL("toggled ( bool )"), self.onToggleTerrainVisiblity)
        self.onBrushChangedCallback = onBrushChangedCallback
        self.lastTerrain = None
        
    def onBrushButtonClicked(self):
        dlg = BrushDialog(self)
        dlg.exec_()
        self.onBrushChangedCallback(dlg.brushName)
        self.brushButton.setIcon(QIcon("./media/terrain/brushes/" + dlg.brushName))
        self.brushButton.setIconSize(QSize(150, 150))
        self.brushButton.setText(dlg.brushName)
        
    def onCreateTerrainButtonClicked(self):
        terrainData = {}
        terrainData["name"]  = str(self.nameLineEdit.text())
        terrainData["size"]  = int(self.sizeComboBox.currentText())
        terrainData["positionX"]  = self.positionSpinBoxX.value()
        terrainData["positionY"]  = self.positionSpinBoxY.value()
        terrainData["positionZ"]  = self.positionSpinBoxZ.value()
        terrainData["extendsX"]  = self.extendsSpinBoxX.value()
        terrainData["extendsY"]  = self.extendsSpinBoxY.value()
        terrainData["extendsZ"]  = self.extendsSpinBoxZ.value()
        terrainData["terrainSize"] = int(self.sizeComboBox.currentText())
        terrainData["terrainHeight"]  = self.terrainHeightSpinBox.value()
        terrainData["splattingBaseName"]  = str(self.baseNameLineEdit.text())
        terrainData["splattingResGroup"]  = str(self.resGroupLineEdit.text())
        terrainData["splattingTextureSize"]  = int(self.texSizeComboBox.currentText())
        terrainData["splattingNumTextures"]  = int(self.numTexturesComboBox.currentText())
        
        self.emit(SIGNAL("createTerrain"), terrainData)
        
    def onDeleteTerrainButtonClicked(self):
        reply = QtGui.QMessageBox.question(self,  "This cannot be undone!",  "Really delete the terrain?",  QMessageBox.Yes|QMessageBox.Cancel)
        if reply == QMessageBox.Cancel:
            return
        elif reply == QMessageBox.Yes:
            self.terrainManager.deleteTerrain(str(self.terrainListComboBox.currentText()))
        
    def onTerrainChanged(self, text):
        terrain = self.terrainManager.getTerrain(text)
        if terrain is not None:
            self.toggleTerrainVisibilityButton.setChecked(terrain.visible)
            
            self.lastTerrain = terrain
        
    def onToggleTerrainVisiblity(self, visibility):
        if self.lastTerrain is not None:
            self.lastTerrain.setVisible(visibility)
        
class Terrain():
    def __init__(self, sceneManager):
        self.sceneManager = sceneManager
        self.camera = self.sceneManager.getCamera("MainCam")
        self.position = og.Vector3()
        self.extends = og.Vector3(128, 50, 128)
        self.terrainHeight = 0.5
        self.name = ""
        self.terrainSize = 129
        self.splattingBaseName = "ETSplatting"
        self.splattingResGroup = "ET"
        self.splattingTextureSize = 128
        self.splattingNumTextures = 6
        
        self.created = False
        self.terrainInfo = None
        self.visible = True
        
    def __del__(self):
        print "deleting terrain: " + self.name
        if self.terrainManager:
            del self.terrainManager
            del self.splatMgr
        
    def create(self, arg):
        if arg is not None:
            self.name = arg["name"]
            self.position = og.Vector3(arg["positionX"], arg["positionY"], arg["positionZ"])
            self.extends = og.Vector3(arg["extendsX"], arg["extendsY"], arg["extendsZ"])
            self.terrainHeight = arg["terrainHeight"]
            self.terrainSize = arg["terrainSize"] + 1
            self.splattingBaseName = arg["splattingBaseName"]
            self.splattingResGroup = arg["splattingResGroup"]
            self.splattingTextureSize = arg["splattingTextureSize"]
            self.splattingNumTextures = arg["splattingNumTextures"]
        
        self.terrainManager =  ET.TerrainManager(self.sceneManager, self.name)
        self.terrainManager.setLODErrorMargin(2, self.camera.getViewport().getActualHeight())
        self.terrainManager.setUseLODMorphing(True, 0.2, "morphFactor")
        
        ## create a fresh, mid-high terrain for editing
        # Note 
        heightMapValues = og.LodDistanceList() ## ET.stdVectorFloat()
        for i in xrange(self.terrainSize):
            for j in xrange(self.terrainSize):
                heightMapValues.append(self.terrainHeight)
        # width, height, heightmapvalues as a float array
        self.terrainInfo = ET.TerrainInfo (self.terrainSize, self.terrainSize, heightMapValues )
        
        ## save typing self
        terrainInfo  = self.terrainInfo 
        sceneManager = self.sceneManager
        terrainMgr   = self.terrainManager
        
        ## set position and size of the terrain
        half = self.extends / 2
        terrainInfo.setExtents(og.AxisAlignedBox(-half.x, -half.y, -half.z, half.x, half.y, half.z))


        ## now render it
        terrainMgr.createTerrain(terrainInfo)

        ## create the splatting manager
#        * @param baseName   base name for the map textures (will be appended by their number)
#        * @param group      the resource group the textures will be placed in
#        * @param width      width of the textures in pixels
#        * @param height     height of the textures in pixels
#        * @param channels   Number of channels per texture (must be in {1, 2, 3, 4})
        self.splatMgr = ET.SplattingManager(self.splattingBaseName, self.splattingResGroup, self.splattingTextureSize, self.splattingTextureSize, 3)
        ## specify number of splatting textures we need to handle
        self.splatMgr.setNumTextures(self.splattingNumTextures)
        
        ## create a manual lightmap texture
        lightmapTex = og.TextureManager.getSingleton().createManual(
        "ETLightmap", self.splattingResGroup, og.TEX_TYPE_2D, self.splattingTextureSize, self.splattingTextureSize, 1, og.PF_BYTE_RGB)

        lightmap = og.Image()
        ET.createTerrainLightmap(terrainInfo, lightmap, 128, 128,\
                                 og.Vector3(1, -1, 1),\
                                 og.ColourValue().White,\
                                 og.ColourValue(0.3, 0.3, 0.3,1.0))
        lightmapTex.getBuffer(0, 0).blitFromMemory(lightmap.getPixelBox(0, 0))

        ##  load the terrain material and assign it
        material = og.MaterialManager.getSingleton().getByName("ETTerrainMaterial")
#        material = og.MaterialManager.getSingleton().getByName("Lockenwickler_Area")
        self.terrainManager.setMaterial(material)
        
        self.sceneNode = self.sceneManager.getSceneNode(self.name + "/Terrain")
        self.sceneNode.setPosition(self.position)
        self.parentSceneNode = self.sceneNode.getParentSceneNode()
        self.created = True
        
    def setVisible(self, visibility):
        try:
            if self.parentSceneNode.getChild(self.sceneNode.getName()):
                self.parentSceneNode.addChild(self.sceneNode)
                self.visible = visibility
            else:
                self.parentSceneNode.attachChild(self.sceneNode)
                self.visible = visibility
        except og.OgreException, e:
            print e
            
class MyTerrainManager():
    def __init__(self, sceneManager):
        self.sceneManager = sceneManager
        self.dockWidgetContents = None
        og.ResourceGroupManager.getSingleton().addResourceLocation("./media/terrain", "FileSystem", "ET", True)
        self.terrainList = []
    
    def deleteTerrain(self, name):
        for terrain in self.terrainList:
            if terrain.name == name:
                self.terrainList.remove(terrain)
                del terrain
    
    def getDockWidget(self, parent):
        if self.dockWidgetContents is None:
            self.dockWidgetContents = MyTerrainManagerDock(self, self.onBrushChanged, parent)
            self.myTerrainManagerDock = QDockWidget("Terrain Tools", parent)
            self.myTerrainManagerDock.setObjectName("TerrainToolsDockWindow")
            self.myTerrainManagerDock.setAllowedAreas(Qt.LeftDockWidgetArea | Qt.RightDockWidgetArea)
            self.myTerrainManagerDock.setWidget(self.dockWidgetContents)
            self.myTerrainManagerDock.setWindowIcon(QIcon("media/icons/terrain_small.png"))
            parent.addDockWidget(Qt.LeftDockWidgetArea, self.myTerrainManagerDock)
            
            QWidget.connect(self.dockWidgetContents, SIGNAL("createTerrain"), self.onCreateTerrain)
            
            
        return self.myTerrainManagerDock

    def getTerrain(self, name):
        for terrain in self.terrainList:
            if terrain.name == name:
                return terrain

    def onBrushChanged(self, brushName):
        print brushName

    def onCreateTerrain(self, arg):
        terrain = Terrain(self.sceneManager)
        terrain.create(arg)
        self.terrainList.append(terrain)
        
        og.ResourceGroupManager.getSingleton().initialiseAllResourceGroups()
        
        self.updateTerrainListBox()
        
#        i = 0
#        numChildren = self.sceneManager.getRootSceneNode().numChildren()
#        while i < numChildren:
#            n = self.sceneManager.getRootSceneNode().getChild(i)
#            name = n.getName()
#            print name
#            i += 1

    def updateTerrainListBox(self):
        self.dockWidgetContents.terrainListComboBox.clear()
        if len(self.terrainList) == 0:
            self.dockWidgetContents.terrainListComboBox.addItem("No Terrain")
            return
            
        for terrain in self.terrainList:
            self.dockWidgetContents.terrainListComboBox.addItem(terrain.name)
