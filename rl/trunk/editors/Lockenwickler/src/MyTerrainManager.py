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
import ctypes

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
    def __init__(self, terrainManager, parent = None):
        super(MyTerrainManagerDock, self).__init__(parent)
        self.setupUi(self)
        self.terrainManager = terrainManager
        QWidget.connect(self.createTerrainButton, SIGNAL("clicked()"), self.onCreateTerrainButtonClicked)
        QWidget.connect(self.deleteTerrainButton, SIGNAL("clicked()"), self.onDeleteTerrainButtonClicked)
        QWidget.connect(self.editBrushButton, SIGNAL("clicked()"), self.onEditBrushButtonClicked)
        QWidget.connect(self.paintBrushButton, SIGNAL("clicked()"), self.onPaintBrushButtonClicked)
        QWidget.connect(self.terrainListComboBox, SIGNAL("currentIndexChanged ( const QString &)"), self.onTerrainChanged)
        QWidget.connect(self.toggleTerrainVisibilityButton, SIGNAL("toggled ( bool )"), self.onToggleTerrainVisiblity)
        QWidget.connect(self.toolBox, SIGNAL("currentChanged( int )"), self.onModeChanged)
        QWidget.connect(self.toolBox, SIGNAL("currentChanged( int )"), self.onModeChanged)
        QWidget.connect(self.editBrushSizeSlider, SIGNAL("valueChanged( int )"), self.terrainManager.onEditBrushSizeChanged)
        QWidget.connect(self.editIntensitySlider, SIGNAL("valueChanged( int )"), self.terrainManager.onEditBrushIntensityChanged)
        QWidget.connect(self.paintBrushSizeSlider, SIGNAL("valueChanged( int )"), self.terrainManager.onPaintBrushSizeChanged)
        QWidget.connect(self.paintIntensitySlider, SIGNAL("valueChanged( int )"), self.terrainManager.onPaintBrushIntensityChanged)
        QWidget.connect(self.setHeightButton, SIGNAL("toggled( bool )"), self.onToggleSetHeight)
        QWidget.connect(self.editSmoothButton, SIGNAL("toggled( bool )"), self.onEditSmoothToolToggled)
        
        self.lastTerrain = None
        
    def onEditBrushButtonClicked(self):
        dlg = BrushDialog(self)
        dlg.exec_()
        self.terrainManager.setEditBrush(dlg.brushName)
        self.editBrushButton.setIcon(QIcon("./media/terrain/brushes/" + dlg.brushName))
        self.editBrushButton.setIconSize(QSize(150, 150))
        self.editBrushButton.setText(dlg.brushName)
        
    def onPaintBrushButtonClicked(self):
        dlg = BrushDialog(self)
        dlg.exec_()
        self.terrainManager.setPaintBrush(dlg.brushName)
        self.paintBrushButton.setIcon(QIcon("./media/terrain/brushes/" + dlg.brushName))
        self.paintBrushButton.setIconSize(QSize(150, 150))
        self.paintBrushButton.setText(dlg.brushName)
        
    def onPaintBrushSizeChanged(self, val):
        self.paintBrushSize = val
        
    def onEditBrushSizeChanged(self, val):
        self.editBrushSize = val
        
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
        reply = QMessageBox.question(self,  "This cannot be undone!",  "Really delete the terrain?",  QMessageBox.Yes|QMessageBox.Cancel)
        if reply == QMessageBox.Cancel:
            return
        elif reply == QMessageBox.Yes:
            self.terrainManager.deleteTerrain(str(self.terrainListComboBox.currentText()))
        self.lastTerrain = None
    
    def onModeChanged(self, mode):
        self.terrainManager.setEditMode(mode)
    
    def onEditSmoothToolToggled(self, ison):
        self.terrainManager.setSmoothMode(ison)
        
    def onTerrainChanged(self, text):
        self.terrainManager.setCurrentTerrain(text)
        terrain = self.terrainManager.getTerrain(text)
        if terrain is not None:
            QWidget.disconnect(self.toggleTerrainVisibilityButton, SIGNAL("toggled ( bool )"), self.onToggleTerrainVisiblity)
            self.toggleTerrainVisibilityButton.setChecked(terrain.visible)
            QWidget.connect(self.toggleTerrainVisibilityButton, SIGNAL("toggled ( bool )"), self.onToggleTerrainVisiblity)
            
            self.lastTerrain = terrain
        
        
    def onToggleTerrainVisiblity(self, visibility):
        if self.lastTerrain is not None:
            self.lastTerrain.setVisible(visibility)
            
    def updateTerrainListBox(self, terrainList):
        self.terrainListComboBox.clear()
        if len(terrainList) == 0:
            self.terrainListComboBox.addItem("No Terrain")
            return
            
        for terrain in terrainList:
            self.terrainListComboBox.addItem(terrain.name)
        
    def onToggleSetHeight(self, setHeights):
        if setHeights:
            box = QMessageBox.information(QApplication.focusWidget(), "", "Pick the height", QMessageBox.Ok)
          
            self.terrainManager.setTerrainHeightMode(True)
        else:
            self.terrainManager.setTerrainHeightMode(False)
            
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
            #if self.parentSceneNode.getChild(self.sceneNode.getName()):
            if not visibility:
                self.parentSceneNode.removeChild(self.sceneNode)
                self.visible = visibility
            else:
                self.parentSceneNode.addChild(self.sceneNode)
                self.visible = visibility
        except og.OgreException, e:
            print e
            
    def deform(self, brush, pos, brushIntensity):
        ## translate our cursor position to vertex indexes
        x = int( self.terrainManager.getTerrainInfo().posToVertexX(pos.x) )
        z = int( self.terrainManager.getTerrainInfo().posToVertexZ(pos.z) )

        ## choose a brush intensity, this determines
        ## how extreme our brush works on the terrain
        ## and tell the ETM to deform the terrain
        self.terrainManager.deform(x, z, brush, brushIntensity * 0.0004)

    def setHeights(self, brush, pos):
        ## translate our cursor position to vertex indexes
        x = int( self.terrainManager.getTerrainInfo().posToVertexX(pos.x) )
        z = int( self.terrainManager.getTerrainInfo().posToVertexZ(pos.z) )

        self.terrainManager.setHeights(x, z, brush)
    def getHeights(self, brush, pos):
        ## translate our cursor position to vertex indexes
        x = int( self.terrainManager.getTerrainInfo().posToVertexX(pos.x) )
        z = int( self.terrainManager.getTerrainInfo().posToVertexZ(pos.z) )

        self.terrainManager.getHeights(x, z, brush)
        
    def smooth(self, brush, pos, intensity):
        x = int( self.terrainManager.getTerrainInfo().posToVertexX(pos.x) )
        z = int( self.terrainManager.getTerrainInfo().posToVertexZ(pos.z) )
        
        smooth = self.averageFilter(x, z, brush, intensity)
        self.terrainManager.setHeights(x, z, smooth)
        
    def averageFilter(self, x, z, brush, intensity):
        #When you're doing a loop possibly thousands of times, it's worth setting these
        #aside rather than calling a function every iteration.
        iWidth = brush.getWidth()
        iHeight = brush.getHeight()

        storageclass = ctypes.c_float * (iWidth * iHeight)
        vecReturnBuffer =  storageclass()
        vecHeightBuffer = storageclass()
       
        ptr = ctypes.pointer(vecReturnBuffer)
       
        brushReturn = ET.Brush(ctypes.pointer(vecReturnBuffer), iWidth, iHeight)
        brushHeights = ET.Brush(ctypes.pointer(vecHeightBuffer), iWidth, iHeight)

        self.terrainManager.getHeights(x, z, brushHeights)

        fSumHeights = 0.0
        iNumHeights = iWidth * iHeight

        # Find the sum of all the heights within the sample
        i = 0
        j = 0
        while i < iWidth:
            while j < iHeight:
                fSumHeights += brushHeights.at(i, j)
                j += 1
            i += 1
       

        #Find the average height within the sample
        fAvgHeight = fSumHeights / iNumHeights

        i = 0
        j = 0
        while i < iWidth:
            while j < iHeight:
                fHeight = brushHeights.at(i, j)
                fDelta = fHeight - fAvgHeight
                fShapeMask = shape.at(i, j)

                fDelta = fDelta * fShapeMask * fIntensity

                val = brushReturn.at(i, j) 
                val = fHeight - fDelta
                
                j += 1
            i += 1
        return brushReturn

        
    def updateLightMap(self):
        lightmap = og.Image()
        ET.createTerrainLightmap(
                             self.terrainManager.getTerrainInfo() ,
                             lightmap, 128, 128 ,
                             og.Vector3(1, -1, 1) ,
                             og.ColourValue(1 ,1, 1) ,      
                             og.ColourValue(0.3, 0.3,  0.3) )

        ## get our dynamic texture and update its contents
        tex = og.TextureManager.getSingleton().getByName("ETLightmap")
        l = lightmap.getPixelBox(0, 0)
        tex.getBuffer(0, 0).blitFromMemory(lightmap.getPixelBox(0, 0))
        
        
class MyTerrainManager():
    def __init__(self, sceneManager):
        self.sceneManager = sceneManager
        self.dockWidgetContents = None
        og.ResourceGroupManager.getSingleton().addResourceLocation("./media/terrain", "FileSystem", "ET", False)        
        og.ResourceGroupManager.getSingleton().addResourceLocation("./media/terrain/brushes", "FileSystem", "ET", False)
        
        self.terrainList = []
        
        self.currentRay = None
        self.currentTerrain = None
        self.isEditing = False
        
        self.currentEditBrush = None
        self.currentEditBrushName = None
        self.editBrushes = {}        
        self.editBrushSize = 2    
        self.editBrushIntensity = 2
        
        self.currentPaintBrush = None
        self.currentPaintBrushName = None
        self.paintBrushes = {}
        self.paintBrushSize = 2
        self.paintBrushIntensity = 2
    
        self.leftMouseDown = False
        
        self.mainTimer = QTimer(None)
        self.mainTimer.start(5)
    
        self.setupFinished = False
        
        self.setHeigthMode = False
        self.setHeigthPickMode = False
        self.editSmoothMode = False
        
        self.ogreMainWindow = None

        
    def createEditingCircle(self):
        self.editingCircle = self.sceneManager.createManualObject("TerrainManagerEditingCircle")
        self.editingCircle.setDynamic(True)
        self.editingCircleAccuracy = 20
        
        self.editingCircle.begin("BaseWhiteNoLighting", og.RenderOperation.OT_LINE_STRIP)
        
        point_index = 0
        theta = 0
        while theta < 2 * og.Math.PI:
            self.editingCircle.position(10 * og.Math.Cos(theta), 0, 10 * og.Math.Sin(theta))
            self.editingCircle.index(point_index)
            point_index += 1
            theta += og.Math.PI / self.editingCircleAccuracy
            
        self.editingCircle.index(0)
        
        self.editingCircle.end()
        
        self.pointerNode.createChildSceneNode("TerrainManagerEditingCircleNode").attachObject(self.editingCircle)

        
    def deleteTerrain(self, name):
        for terrain in self.terrainList:
            if terrain.name == name:
                self.terrainList.remove(terrain)
                del terrain
        self.updateTerrainListBox()
        
    def getDockWidget(self, parent):
        if self.dockWidgetContents is None:
            self.dockWidgetContents = MyTerrainManagerDock(self, parent)
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

    def leftMouseUp(self):
        if self.currentTerrain:
            self.currentTerrain.updateLightMap()

    def onPaintBrushSizeChanged(self, val):
        self.paintBrushSize = val
        self.updatePaintBrush()
        
    def onPaintBrushIntensityChanged(self, val):
        self.paintBrushIntensity = val
        
    def onEditBrushSizeChanged(self, val):
        self.editBrushSize = val
        self.updateEditBrush()
        
    def onEditBrushIntensityChanged(self, val):
        self.editBrushIntensity = val
        
    def onCreateTerrain(self, arg):
        if not self.setupFinished:
            self.pointer = self.sceneManager.createEntity("TerrainManagerPointerNodeMesh", "UniCube.mesh")
            self.pointerNode = self.sceneManager.getRootSceneNode().createChild("TerrainManagerPointerNode")
            self.pointerNode.attachObject(self.pointer)

            self.createEditingCircle()
            
            self.setupFinished = True
        
        
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
        self.dockWidgetContents.updateTerrainListBox(self.terrainList)

    def updateEditBrush(self):
        if self.currentEditBrushName is None:
            return
            
        if self.currentEditBrush is None:
            self.currentEditBrush = None
            
        image = og.Image()
        image.load(self.currentEditBrushName, "ET")
        image.resize(self.editBrushSize, self.editBrushSize)
        self.currentEditBrush = ET.loadBrushFromImage(image)
        
    def updatePaintBrush(self):
        if self.currentPaintBrush is not None:
            self.currentPaintBrush = None
        image = og.Image()
        image.load(self.currentPaintBrushName, "ET")
        image.resize(self.paintBrushSize, self.paintBrushSize)
        self.currentPaintBrush = ET.loadBrushFromImage(image)

    def setCurrentTerrain(self, text):
        self.currentTerrain = self.getTerrain(text)
        
    def setEditBrush(self, brush):
        self.currentEditBrushName = brush
        self.updateEditBrush()
        
    def setPaintBrush(self, brush):
        self.currentPaintBrushName = brush
        self.updatePaintBrush()
        
    def setEditMode(self, mode):
        self.editMode = mode
        
        if self.editMode == 0:
            self.mainTimer.disconnect(self.mainTimer, SIGNAL("timeout()"), self.update)
                
        elif self.editMode == 1:
            self.mainTimer.connect(self.mainTimer, SIGNAL("timeout()"), self.update)
        elif self.editMode == 2:
            self.mainTimer.connect(self.mainTimer, SIGNAL("timeout()"), self.update)
    
    def setSmoothMode(self, ison):
        self.editSmoothMode = ison
        
        if not ison:
            self.updateEditBrush()
            
    def setTerrainHeightMode(self, setHeight):
        self.setHeigthMode = setHeight
        self.setHeigthPickMode = setHeight
        
        if not setHeight:
            self.updateEditBrush()
        
    def update(self):
        self.currentRay = self.ogreMainWindow.getCameraToViewportRay()
        
        self.updatePointer()
        
        if self.setHeigthPickMode and self.leftMouseDown:
            self.currentTerrain.getHeights(self.currentEditBrush, self.pointerNode.getPosition())
            self.setHeigthPickMode = False
            return
        
        if self.editMode == 1 and self.isEditing:
            if self.currentEditBrushName is not None and self.editBrushIntensity is not 0 and self.leftMouseDown and not self.setHeigthMode and not self.editSmoothMode:
                self.currentTerrain.deform(self.currentEditBrush, self.pointerNode.getPosition(), self.editBrushIntensity)
            elif self.leftMouseDown and self.setHeigthMode:
                self.currentTerrain.setHeights(self.currentEditBrush, self.pointerNode.getPosition())  
            elif self.leftMouseDown and self.editSmoothMode:
                self.currentTerrain.smooth(self.currentEditBrush, self.pointerNode.getPosition(), self.editBrushIntensity)  
        elif self.editMode == 2 and self.isEditing:
            if self.currentPaintBrushName is not None :
                print self.currentPaintBrushName + " " + str(self.paintBrushSize) + " " + str(self.paintBrushIntensity)
    
    def updateEditingCircle(self):
        self.editingCircle.beginUpdate(0)

        point_index = 0
        theta = 0
        while theta < 2 * og.Math.PI:
            x = self.editBrushSize * og.Math.Cos(theta)
            z = self.editBrushSize * og.Math.Sin(theta)
            
            y = self.currentTerrain.terrainManager.getTerrainInfo().getHeightAt(self.pointerNode.getPosition().x + x, self.pointerNode.getPosition().z +  z)
            
            self.editingCircle.position(x, (y - self.pointerNode.getPosition().y) + 0.15, z)
            self.editingCircle.index(point_index)
            point_index += 1
            theta += og.Math.PI / self.editingCircleAccuracy
            
        self.editingCircle.index(0)

        self.editingCircle.end()

    def updatePointer(self):
        if self.currentTerrain is not None:    #float getHeightAt(float x, float z) const;
            result = self.currentTerrain.terrainManager.getTerrainInfo().rayIntersects(self.currentRay)
            intersects = result[0]
            ## update pointer's position
            if (intersects):
                self.pointerNode.setVisible(True)
                x = result[1][0]
                y = result[1][1]
                z = result[1][2]
#                print ("Intersect %f, %f, %f " % ( x, y, z) )
                self.pointerNode.setPosition(og.Vector3(x, y, z))
                self.updateEditingCircle()
            else:
                self.pointerNode.setVisible(False)
