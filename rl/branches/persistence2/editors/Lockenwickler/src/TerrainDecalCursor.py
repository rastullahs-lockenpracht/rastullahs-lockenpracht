# @file DecalCursor.cpp
# @author Jesse Wright - www.cutthroatstudios.com
# @date Created on 12/25/2007
# @date Last modified 5/19/2007
# @note Modified from Brocan's example on the Ogre forums
# Translated to Python by Stefan Stammberger

import ogre.renderer.OGRE as og
import ogre.addons.et as ET


#   /**
#    * @brief The length of one meter in ogre units
#    * @todo Set this to something meaningful for your project!
#    */
METER_SCALE = 1.0

class TerrainDecalCursor():
    def __init__(self, sceneManager, terrainMat, size, cursorTexName):
        self.m_bVisible = False
        self.m_nodeProj  = None
        self.m_pass = None
        self.m_frustProj = None
        self.m_texState = None
        self.m_sceneMgr = sceneManager
        self.m_terrainMat = terrainMat
        self.m_pos = og.Vector3()
        self.m_size = og.Vector2()
        self.m_sTexName = None
        
        self.init(size, cursorTexName)


    def __del__(self):
        self.hide()

        if self.m_pass is not None:
            self.m_terrainMat.getTechnique(0).removePass( self.m_pass.getIndex() )
            self.m_pass = None

        #delete frustum
        self.m_nodeProj.detachAllObjects()
        del self.m_frustProj

        #destroy node
        self.m_nodeProj.getParentSceneNode().removeAndDestroyChild(self.m_nodeProj.getName())


    def init(self, size, tex ):

        #create a new pass in the material to render the decal
        self.m_pass = self.m_terrainMat.getTechnique(0).getPass("Decal")
        if not self.m_pass:
            techPref = self.m_terrainMat.getTechnique(0)
            self.m_pass = techPref.createPass()
            self.m_pass.setName("Decal")
            self.m_pass.setLightingEnabled(False)
            self.m_pass.setSceneBlending(og.SBT_TRANSPARENT_ALPHA)
            self.m_pass.setDepthBias(2.5, 2.5)
            self.m_pass.setFog(True)
            self.m_pass.createTextureUnitState("decalBase.png")

        #init projective decal
        #set up the main decal projection frustum
        self.m_frustProj = og.Frustum()
        self.m_nodeProj = self.m_sceneMgr.getRootSceneNode().createChildSceneNode()
        self.m_nodeProj.attachObject(self.m_frustProj)
        self.m_frustProj.setProjectionType(og.PT_ORTHOGRAPHIC)
        self.m_nodeProj.setOrientation(og.Quaternion(og.Degree(90), og.Vector3().UNIT_X))
   
#      /* Commented out for general use. If you are using mouse picking in your scene, set this to an unselectable flag otherwise you may find the frustum in your ray queries. */
#   //m_frustProj.setQueryFlags(Sanguis.UNSELECTABLE)

#      // set given values
        self.setSize(size)
        self.m_sTexName = tex      # texture to apply

#       load the images for the decal and the filter
        og.TextureManager.getSingleton().load(self.m_sTexName, og.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME, og.TEX_TYPE_2D, 1)

        self.m_bVisible = False


    def getPosition(self):
        return self.m_pos


    def show(self):
        if not self.m_bVisible:
            self.m_bVisible = True
            self.showTerrainDecal()
            self.setPosition(self.m_pos)   


    def hide(self):
        if self.m_bVisible:
            self.m_bVisible = False
            self.hideTerrainDecal()
           
    def setPosition(self, pos):

        self.m_pos = pos

        #Set the projection coming from some distance above the current position.
        self.m_nodeProj.setPosition(pos.x, pos.y + (10.0 * METER_SCALE), pos.z)

    def setSize(self, size):
       if self.m_size is not size:
            self.m_size = size
            #m_frustProj.setAspectRatio(m_size.x / m_size.y)
            self.m_frustProj.setOrthoWindow ( self.m_size.x, self.m_size.y )
       
            # set fovy so that tan = 1, so 45 degrees
            self.m_frustProj.setFOVy(og.Degree(45))

             # set near clip plane according to fovy:
            self.m_frustProj.setNearClipDistance(self.m_size.y)
      
    def showTerrainDecal(self):
        if not self.m_texState:
            #set up the decal's texture unit
            self.m_texState = self.m_pass.createTextureUnitState(self.m_sTexName)
            self.m_texState.setProjectiveTexturing(True, self.m_frustProj)
            self.m_texState.setTextureAddressingMode(og.TextureUnitState.TAM_CLAMP)
            self.m_texState.setTextureFiltering(og.FO_POINT, og.FO_LINEAR, og.FO_NONE)
            self.m_texState.setAlphaOperation(og.LBX_ADD)

    def hideTerrainDecal(self):
        if self.m_texState:
            self.m_pass.removeTextureUnitState(self.m_pass.getTextureUnitStateIndex(self.m_texState))
            self.m_texState = 0
