import ctypes as ctypes
import random

import ogre.renderer.OGRE as og

# class to handle material switching without having to modify scene materials individually
class MaterialSwitcher( og.MaterialManager.Listener ):
    def __init__(self):
        og.MaterialManager.Listener.__init__(self)
      
        self.currentColor = og.ColourValue(0.0, 0.0, 0.0)
        self.lastEntity = ""
        self.lastTechnique = None
        
       
    # takes into account that one Entity can have multiple SubEntities
    def handleSchemeNotFound(self, index, name, material, lod, subEntity):
        temp = str(type(subEntity))
        if temp == "<class 'ogre.renderer.OGRE._ogre_.SubEntity'>":
            if self.lastEntity == subEntity.getParent().getName():
                subEntity.setCustomParameter(1, og.Vector4(self.currentColor.r, self.currentColor.g, self.currentColor.b, 1.0))
                return self.lastTechnique
            else:
                self.lastTechnique = og.MaterialManager.getSingleton().load("PlainColor", og.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME).getTechnique(0)
                
                self.randomizeColor()
                subEntity.setCustomParameter(1, og.Vector4(self.currentColor.r, self.currentColor.g, self.currentColor.b, 1.0))
                

                self.lastEntity = subEntity.getParent().getName()
                return self.lastTechnique
        

    def randomizeColor(self):
        r = random.randrange(1, 255)
        g = random.randrange(1, 255)
        b = random.randrange(1, 255)
        
        var = 1.0 / 255.0

        self.currentColor = og.ColourValue(r * var, g * var, b * var)

        
    def reset(self):
        self.currentColor = og.ColourValue(0.0, 0.0, 0.0)
        self.lastEntity = ""
        
# We need this attached to the depth target, otherwise we get problems with the compositor
# MaterialManager.Listener should NOT be running all the time - rather only when we're
# specifically rendering the target that needs it
class SelectionRenderListener(og.RenderTargetListener):
    def __init__(self, materialListener):
        og.RenderTargetListener.__init__(self)
        self.materialListener = materialListener
 
    def preRenderTargetUpdate(self, evt):
        og.MaterialManager.getSingleton().addListener( self.materialListener )
 
    def postRenderTargetUpdate(self, evt):
        og.MaterialManager.getSingleton().removeListener( self.materialListener )

        
class SelectionBuffer():
    def __init__(self, sceneManager,  renderTarget):
        self.sceneMgr = sceneManager
        self.camera = sceneManager.getCamera("MainCam")

        self.viewport = renderTarget
        
        # This is the material listener - Note: it is controlled by a seperate
        # RenderTargetListener, not applied globally to all targets
        self.materialSwitchListener = MaterialSwitcher()
        
        self.selectionTargetListener = SelectionRenderListener( self.materialSwitchListener )
        
#        self.texture = og.TextureManager.getSingleton().createManual("SelectionPassTex", 
#                                                                    og.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME, 
#                                                                    og.TEX_TYPE_2D, 
#                                                                    self.viewport.getActualWidth(), 
#                                                                    self.viewport.getActualHeight(), 
#                                                                    0, og.PixelFormat.PF_R8G8B8, og.TU_RENDERTARGET)
                                                                    
        width = self.viewport.getWidth()
        height = self.viewport.getHeight()
        
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
        self.renderTexture.addListener( self.selectionTargetListener )
        self.renderTexture.getViewport(0).setMaterialScheme("aa")
        
        self.createRTTOverlays()

    def update(self):
        self.renderTexture.update()        
        self.materialSwitchListener.reset()
        
        pixelBuffer = self.texture.getBuffer()
        bufferSize = pixelBuffer.getSizeInBytes()
        #buffersize2 = self.renderTexture.getWidth()*self.renderTexture.getHeight()*4
        
        storageclass = ctypes.c_uint8 * (bufferSize)
        buff = storageclass()
        
        VoidPointer = og.CastVoidPtr(ctypes.addressof(buff))

        pBox = og.PixelBox(pixelBuffer.getWidth(), pixelBuffer.getHeight(),pixelBuffer.getDepth(), pixelBuffer.getFormat(), VoidPointer)
        self.renderTexture.copyContentsToMemory(pBox, og.RenderTarget.FrameBuffer.FB_FRONT)
        self.renderTexture.writeContentsToTimestampedFile("editor", ".png")
        i = 0
        
        while i < len(buff):
            #print str(buff[i + 2]) + " " + str(buff[i+1]) + " " + str(buff[i])
            
            i += 4
        #for a in buff:
            #print a

        #pBox = pixelBuffer.getCurrentLock()
        
        #storageclass = ctypes.c_uint8 * (self.renderTexture.getWidth()*self.renderTexture.getHeight()*3) 
        
        
        #pixelBuffer.unlock()
        

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



