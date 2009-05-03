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

class MyTerrainManagerDock(Ui_MyTerrainManagerDock.Ui_TerrainManagerUi, QWidget):
    def __init__(self, parent):
        super(MyTerrainManagerDock, self).__init__(parent)
        self.setupUi(self)
        
class Terrain():
    def __init__(self, sceneManager):
        self.sceneManager = sceneManager
        self.camera = self.sceneManager.getCamera("MainCam")
        
        self.terrainSize = 129
        
        ## create terrain manager
        self.terrainManager =  ET.TerrainManager(self.sceneManager)
        self.terrainManager.setLODErrorMargin(2, self.camera.getViewport().getActualHeight())
        self.terrainManager.setUseLODMorphing(True, 0.2, "morphFactor")
        
        ## create a fresh, mid-high terrain for editing
        # Note 
        heightMapValues = og.LodDistanceList() ## ET.stdVectorFloat()
        for i in xrange(self.terrainSize):
            for j in xrange(self.terrainSize):
                heightMapValues.append(float(0.50))
        # width, height, heightmapvalues as a float array
        self.terrainInfo = ET.TerrainInfo (self.terrainSize, self.terrainSize, heightMapValues )
        
        ## save typing self
        terrainInfo  = self.terrainInfo 
        sceneManager = self.sceneManager
        terrainMgr   = self.terrainManager
        
        ## set position and size of the terrain
        terrainInfo.setExtents(og.AxisAlignedBox(-1500, -150, -1500, 1500, 150, 1500))

        ## now render it
        terrainMgr.createTerrain(terrainInfo)

        ## create the splatting manager
#        * @param baseName   base name for the map textures (will be appended by their number)
#        * @param group      the resource group the textures will be placed in
#        * @param width      width of the textures in pixels
#        * @param height     height of the textures in pixels
#        * @param channels   Number of channels per texture (must be in {1, 2, 3, 4})
        self.textureBaseName = "ETSplatting"
        self.resGroup = "ET"
        self.textureSize = 128
        self.splatMgr = ET.SplattingManager(self.textureBaseName, self.resGroup, self.textureSize, self.textureSize, 3)
        ## specify number of splatting textures we need to handle
        self.splatMgr.setNumTextures(6)

        ## create a manual lightmap texture
        lightmapTex = og.TextureManager.getSingleton().createManual(
        self.textureBaseName, self.resGroup, og.TEX_TYPE_2D, self.textureSize, self.textureSize, 1, og.PF_BYTE_RGB)

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
        
class MyTerrainManager():
    def __init__(self, sceneManager):
        self.sceneManager = sceneManager
        self.dockWidget = None
        #self.terrain = Terrain(sceneManager)

    def getDockWidget(self, parent):
        if self.dockWidget is None:
            self.dockWidgetContents = MyTerrainManagerDock(parent)
            self.myTerrainManagerDock = QDockWidget("Terrain Tools", parent)
            self.myTerrainManagerDock.setObjectName("TerrainToolsDockWindow")
            self.myTerrainManagerDock.setAllowedAreas(Qt.LeftDockWidgetArea | Qt.RightDockWidgetArea)
            self.myTerrainManagerDock.setWidget(self.dockWidgetContents)
            self.myTerrainManagerDock.setWindowIcon(QIcon("media/icons/terrain_small.png"))
            parent.addDockWidget(Qt.LeftDockWidgetArea, self.myTerrainManagerDock)
            
        return self.myTerrainManagerDock
