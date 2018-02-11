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

import ctypes as ctypes
import random
import platform

import ogre.renderer.OGRE as og

from SelectionBuffer import SelectionObject

# class to handle material switching without having to modify scene materials individually
class MaterialSwitcher( og.MaterialManager.Listener ):
    def __init__(self):
        og.MaterialManager.Listener.__init__(self)

        self.lastTechnique = None
 
        self.lastTechnique = og.MaterialManager.getSingleton().load("DepthMap", og.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME).getBestTechnique(0)
        
    # takes into account that one Entity can have multiple SubEntities
    def handleSchemeNotFound(self, index, name, material, lod, subEntity):
        return self.lastTechnique

        
# We need this attached to the depth target, otherwise we get problems with the compositor
# MaterialManager.Listener should NOT be running all the time - rather only when we're
# specifically rendering the target that needs it
class DepthBufferListener(og.RenderTargetListener):
    def __init__(self, materialListener):
        og.RenderTargetListener.__init__(self)
        self.materialListener = materialListener
 
    def preRenderTargetUpdate(self, evt):
        og.MaterialManager.getSingleton().addListener( self.materialListener )
 
    def postRenderTargetUpdate(self, evt):
        og.MaterialManager.getSingleton().removeListener( self.materialListener )

        
class DepthBuffer():
    def __init__(self, sceneManager,  renderTarget):
        self.sceneMgr = sceneManager
        self.camera = sceneManager.getCamera("MainCam")

        self.renderTarget = renderTarget
        
        # This is the material listener - Note: it is controlled by a seperate
        # RenderTargetListener, not applied globally to all targets
        self.materialSwitchListener = MaterialSwitcher()
        
        self.depthBufferListener = DepthBufferListener( self.materialSwitchListener )
        
        width = self.renderTarget.getWidth()
        height = self.renderTarget.getHeight()
        
        self.texture = og.TextureManager.getSingleton().createManual("SelectionPassTex", 
                                                                    og.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME, 
                                                                    og.TEX_TYPE_2D, 
                                                                    width, 
                                                                    height, 
                                                                    0, og.PixelFormat.PF_R8G8B8, og.TU_RENDERTARGET)
                                                                    
        self.renderTexture = self.texture.getBuffer().getRenderTarget()
        self.renderTexture.setAutoUpdated(False)
        self.renderTexture.setPriority(0)                                                         
        self.renderTexture.addViewport( self.camera )
        self.renderTexture.getViewport(0).setOverlaysEnabled(False)
        self.renderTexture.getViewport(0).setClearEveryFrame(True)
        self.renderTexture.addListener( self.depthBufferListener )
        self.renderTexture.getViewport(0).setMaterialScheme("aa")
        
#        self.createRTTOverlays()

    def update(self):
        self.updateBufferSize()
        
        self.renderTexture.update()        
        
        pixelBuffer = self.texture.getBuffer()
        bufferSize = pixelBuffer.getSizeInBytes()
        
        storageclass = ctypes.c_uint8 * (bufferSize)
        self.buffer = storageclass()
        
        VoidPointer = og.CastVoidPtr(ctypes.addressof(self.buffer))

        self.pBox = og.PixelBox(pixelBuffer.getWidth(), pixelBuffer.getHeight(),pixelBuffer.getDepth(), pixelBuffer.getFormat(), VoidPointer)
        self.renderTexture.copyContentsToMemory(self.pBox, og.RenderTarget.FrameBuffer.FB_FRONT)
        self.renderTexture.writeContentsToFile("depthbuffer.png")
#        i = 0
#        
#        while i < len(self.buffer):
#            #print str(self.buffer[i + 2]) + " " + str(self.buffer[i+1]) + " " + str(self.buffer[i])
#            
#            i += 4
    
    def updateBufferSize(self):
        width = self.renderTarget.getWidth()
        height = self.renderTarget.getHeight()
        needsSizeUpdate = False
        
        if width is not self.renderTexture.getWidth():
            needsSizeUpdate = True
        if height is not self.renderTexture.getHeight():
            needsSizeUpdate = True            
        
        if needsSizeUpdate:
            og.TextureManager.getSingleton().unload("SelectionPassTex")
            
            self.texture = og.TextureManager.getSingleton().createManual("SelectionPassTex", 
                                                            og.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME, 
                                                            og.TEX_TYPE_2D, 
                                                            width, 
                                                            height, 
                                                            0, og.PixelFormat.PF_R8G8B8, og.TU_RENDERTARGET)
                                                                    
            self.renderTexture = self.texture.getBuffer().getRenderTarget()
            self.renderTexture.setAutoUpdated(False)
            self.renderTexture.setPriority(0)                                                         
            self.renderTexture.addViewport( self.camera )
            self.renderTexture.getViewport(0).setOverlaysEnabled(False)
            self.renderTexture.getViewport(0).setClearEveryFrame(True)
            self.renderTexture.addListener( self.depthBufferListener )
            self.renderTexture.getViewport(0).setMaterialScheme("aa")
        else:
            return
            
    def onSelectionClick(self, x, y):
        self.update()
        
        posInStream = (self.pBox.getWidth() * y - 1)*4
        posInStream += x*4
        
        colVec = og.Vector3(self.buffer[posInStream + 2], self.buffer[posInStream+1], self.buffer[posInStream])
        print str(colVec.x) + " " + str(colVec.y) + " " + str(colVec.z)
        
        return None
        
    def createRTTOverlays(self):
        baseWhite = og.MaterialManager.getSingletonPtr().getByName("Lockenwickler_Pivot_X")
        SelectionBufferTexture = baseWhite.clone("SelectionDebugMaterial")
        textureUnit = SelectionBufferTexture.getTechnique(0).getPass(0).createTextureUnitState()
 
        textureUnit.setTextureName("SelectionPassTex")
 
 
        overlayManager = og.OverlayManager.getSingleton()
        # Create an overlay
        self.mDebugOverlay = overlayManager.create("OverlayName")
 
        # Create a panel
        panel = overlayManager.createOverlayElement("Panel", "PanelName0")
        panel.setMetricsMode(og.GMM_PIXELS)
        panel.setPosition(10, 10)
        panel.setDimensions(400, 280)
        panel.setMaterialName("SelectionDebugMaterial") 
        self.mDebugOverlay.add2D(panel)
 
        self.mDebugOverlay.show()



