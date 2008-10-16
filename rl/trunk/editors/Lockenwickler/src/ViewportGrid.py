#-----------------------------------------------------------------------------
#Copyright (c) 2007 Jeroen Dierckx
#Python Version by Stefan Stammberger
#
#This program is free software you can redistribute it and/or modify it under
#the terms of the GNU Lesser General Public License as published by the Free Software
#Foundation either version 2 of the License, or (at your option) any later
#version.
#
#This program is distributed in the hope that it will be useful, but WITHOUT
#ANY WARRANTY without even the implied warranty of MERCHANTABILITY or FITNESS
#FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
#
#You should have received a copy of the GNU Lesser General Public License along with
#this program if not, write to the Free Software Foundation, Inc., 59 Temple
#Place - Suite 330, Boston, MA 02111-1307, USA, or go to
#http://www.gnu.org/copyleft/lesser.txt.
#-----------------------------------------------------------------------------

import ogre.renderer.OGRE as og

RL_BEHIND = 1
INFRONT = 2

class ViewportGrid(og.RenderTargetListener):
    def __init__(self, pSceneMgr, pViewport):
        super(ViewportGrid, self).__init__()

        self.sceneManager = pSceneMgr
        self.viewport = pViewport
        self.enabled = False
        self.layer = None

        self.prevCamera = None
        self.prevOrtho = False
        self.prevCamPos = None
        self.prevNear = None
        self.prevOrtho = None
        self.prevAspectRatio = None
        self.bForceUpdate = True

        self.pGrid = None
        self.created = False
        self.pNode = None

        self.colour1 = og.ColourValue(0.7, 0.7, 0.7)
        self.colour2 = og.ColourValue(0.7, 0.7, 0.7)
        self.division = 10
        self.perspSize = 100
        self.renderScale = True
        self.renderMiniAxes = True

        self.sMatName = "ViewportGrid"

        self.__createGrid()
        self.setRenderLayer(RL_BEHIND)

        # Add this as a render target listener
        self.viewport.getTarget().addListener(self)


    def getColour(self):
        return colour1

    def setColour(self, colour):
        # Force alpha = 1 for the primary colour
        self.colour1 = colour
        self.colour1.a = 1.0
        self.colour2 = colour1
        self.forceUpdate()

    #Grid division (the number of new lines that are created when zooming in).
    def getDivision(self):
        return division

    def setDivision(self, division):
        self.division = division
        self.forceUpdate()

    #//! Grid render layer (behind of in front of the objects).
    def getRenderLayer(self):
        return layer

    def setRenderLayer(self, layer):
        self.layer = layer

        if self.layer == RL_BEHIND:
            # Render just before the world geometry
            self.pGrid.setRenderQueueGroup(og.RENDER_QUEUE_WORLD_GEOMETRY_1 - 1)
        elif self.layer == RL_INFRONT:
            # Render just before the overlays
            self.pGrid.setRenderQueueGroup(og.RENDER_QUEUE_OVERLAY - 1)


    def getPerspectiveSize(self):
        return perspSize

    def setPerspectiveSize(self, size):
        self.perspSize = size
        self.forceUpdate()

#       //! Render scaling info? Defaults to true.
#       //! @todo Implement this
    def getRenderScale(self):
        return renderScale

    def setRenderScale(self, enabled = True):
        self.renderScale = enabled
        self.forceUpdate()

#       //! Render mini axes? Defaults to true.
#       //! @todo Implement this
    def getRenderMiniAxes(self):
        return renderMiniAxes

    def setRenderMiniAxes(self, enabled = True):
        self.renderMiniAxes = enabled
        self.forceUpdate()

    #// Enable / disable
    def isEnabled(self):
        return self.enabled

    def enable(self):
        self.enabled = True

        if not self.pGrid.isAttached():
            self.pNode.attachObject(self.pGrid)

        self.forceUpdate()


    def disable(self):
        self.enabled = False

        if self.pGrid.isAttached():
            self.pNode.detachObject(self.pGrid)



    def  toggle(self):
        self.setEnabled(not self.enabled)

    def setEnabled(self, enabled):
        if enabled:
            self.enable()
        else:
            self.disable()


    def forceUpdate(self):
        self.bForceUpdate = True

    def preViewportUpdate(self, evt):
       #TODO find out why evt.souce and self.viewport differ in their memory address
       # if evt.source != self.viewport:
           # return

        self.pGrid.setVisible(True)

        if self.enabled:
            self.__update()

    def postViewportUpdate(self, evt):
       # if evt.source != self.viewport:
           # return
        self.pGrid.setVisible(False)

    def __createGrid(self):
        name = self.viewport.getTarget().getName() + "::"
        name += str(self.viewport.getZOrder()) + "::ViewportGrid"

        #// Create the manual object
        self.pGrid = self.sceneManager.createManualObject(name)
        self.pGrid.setDynamic(True)

        #// Create the scene node (not attached yet)
        self.pNode = self.sceneManager.getRootSceneNode().createChildSceneNode(name)
        self.enabled = False

        #// Make sure the material is created
        #//! @todo Should we destroy the material somewhere?
        matMgr = og.MaterialManager.getSingleton()
        if not matMgr.resourceExists(self.sMatName):
            pMaterial = matMgr.create(self.sMatName, og.ResourceGroupManager.DEFAULT_RESOURCE_GROUP_NAME)
            pMaterial.setLightingEnabled(False)
            pMaterial.setSceneBlending(og.SBT_TRANSPARENT_ALPHA)



    def __destroyGrid(self):
        self.sceneManager.destroyManualObject(self.pGrid)
        self.pGrid = None

        self.sceneManager.destroySceneNode(self.pNode.getName())
        self.pNode = None


    def __update(self):
        if not self.enabled:
            return

        pCamera = self.viewport.getCamera()
        if not pCamera:
            return

        # Check if an update is necessary
        if not self.__checkUpdate() and not self.bForceUpdate:
            return

        if pCamera.getProjectionType() == og.PT_ORTHOGRAPHIC:
            self.__updateOrtho()
        else:
            self.__updatePersp()

        self.bForceUpdate = False



    def __updateOrtho(self):
        # Screen dimensions
        width = self.viewport.getActualWidth()
        height = self.viewport.getActualHeight()

        #Camera information
        pCamera = pViewport.getCamera()
        camPos = pCamera.getPosition()
        camDir = pCamera.getDirection()
        camUp = pCamera.getUp()
        camRight = pCamera.getRight()

        # Translation in grid space
        dx = camPos.dotProduct(camRight)
        dy = camPos.dotProduct(camUp)

        # Frustum dimensions
        # Note: Tan calculates the opposite side of a _right_ triangle given its angle, so we make sure it is one, and double the result
        worldWidth = 2 * og.Math.Tan(pCamera.getFOVy() / 2) * pCamera.getAspectRatio() * pCamera.getNearClipDistance()
        worldHeight = worldWidth / pCamera.getAspectRatio()
        worldLeft = dx - worldWidth / 2
        worldRight = dx + worldWidth / 2
        worldBottom = dy - worldHeight / 2
        worldTop = dy + worldHeight / 2

        # Conversion values (note: same as working with the height values)
        worldToScreen = width / worldWidth
        screenToWorld = worldWidth / width

        # TODO Treshold should be dependent on window width/height (min? max?) so there are no more then division full alpha-lines
        treshold = 10 # Treshhold in pixels

        # Calculate the spacing multiplier
        mult = 0
        exp = 0
        temp = worldToScreen # 1 world unit
        if worldToScreen < treshold:
            while temp < treshold:
                exp = exp +1
                temp = temp * treshold # TODO maybe wrong

            mult = og.Math.Pow(self.division, exp)
        else:
            while temp > (self.division * treshold):
                exp = exp +1
                temp = temp / treshold # TODO maybe wrong

            mult = og.Math.Pow(1.0 / self.division, exp)

        # Interpolate alpha for (multiplied) spacing between treshold and division * treshold
        self.colour2.a = worldToScreen * mult / (division * treshold - treshold)
        if colour2.a > 1.0:
           self.colour2.a = 1.0

        # Calculate the horizontal zero-axis color
        camRightX = og.Math.Abs(camRight.x)
        camRightY = og.Math.Abs(camRight.y)
        camRightZ = og.Math.Abs(camRight.z)

        if og.Math.RealEqual(camRightX, 1.0):
            horAxisColor = og.ColourValue.Red
        elif og.Math.RealEqual(camRightY, 1.0):
            horAxisColor = og.ColourValue.Green
        elif og.Math.RealEqual(camRightZ, 1.0):
            horAxisColor = og.ColourValue.Blue
        else:
            horAxisColor = self.colour1

        # Calculate the vertical zero-axis color
        camUpX = og.Math.Abs(camUp.x)
        camUpY = og.Math.Abs(camUp.y)
        camUpZ = og.Math.Abs(camUp.z)

        if og.Math.RealEqual(camUpX, 1.0):
            horAxisColor = og.ColourValue.Red
        elif og.Math.RealEqual(camUpY, 1.0):
            horAxisColor = og.ColourValue.Green
        elif og.Math.RealEqual(camUpZ, 1.0):
            horAxisColor = og.ColourValue.Blue
        else:
            horAxisColor = self.colour1

        # The number of lines
        numLinesWidth = int((worldWidth / mult) + 1)
        numLinesHeight = int((worldHeight / mult) + 1)

        # Start creating or updating the grid
        self.pGrid.estimateVertexCount(2 * numLinesWidth + 2 * numLinesHeight)
        if created:
            self.pGrid.beginUpdate(0)
        else:
            self.pGrid.begin(self.sMatName, og.RenderOperation.OT_LINE_LIST)
            self.created = True

        # Vertical lines
        startX = mult * int(worldLeft / mult)
        x = startX
        while x <= worldRight:
            # Get the right color for this line
            if x == 0:
                multX = x
            elif x < 0:
                multX = int(x / mult - 0.5)
            else:
                multX = int(x / mult + 0.5)


            if multX == 0:
                colour = vertAxisColor
            elif multX % (int(self.division)):
                self.colour2
            else:
                self.colour1




            # Add the line
            self.pGrid.position(x, worldBottom, 0)
            self.pGrid.colour(colour)
            self.pGrid.position(x, worldTop, 0)
            self.pGrid.colour(colour)

            x += mult

        # Horizontal lines
        startY = mult * int(worldBottom / mult)
        y = startY
        while y <= worldTop:
            # Get the right color for this line
            #multY = (y == 0) ? y : (y < 0) ? int(y / mult - 0.5f) : int(y / mult + 0.5f)
            if y == 0:
                multY = y
            elif y < 0:
                multY = int(y / mult - 0.5)
            else:
                multY = int(y / mult + 0.5)

            #colour = (multY == 0) ? horAxisColor : (multY % int(division)) ? self.colour2 : self.colour1
            if multY == 0:
                colour = horAxisColor
            elif multY % (int(self.division)):
                self.colour2
            else:
                self.colour1

            self.pGrid.position(worldLeft, y, 0)
            self.pGrid.colour(colour)
            self.pGrid.position(worldRight, y, 0)
            self.pGrid.colour(colour)

            y += mult

        self.pGrid.end()

        self.pNode.setOrientation(pCamera.getOrientation())


    def __updatePersp(self):
        #! @todo Calculate the spacing multiplier
        mult = 1

        #! @todo Interpolate alpha
        self.colour2.a = 0.5
        #if(colour2.a > 1.0f) colour2.a = 1.0f

        # Calculate the horizontal zero-axis color
        horAxisColor = og.ColourValue.Red

        # Calculate the vertical zero-axis color
        vertAxisColor = og.ColourValue.Blue

        # The number of lines
        numLines = int(self.perspSize / mult) + 1

        # Start creating or updating the grid
        self.pGrid.estimateVertexCount(4 * numLines)
        if self.created:
            self.pGrid.beginUpdate(0)
        else:
            self.pGrid.begin(self.sMatName, og.RenderOperation.OT_LINE_LIST)
            self.created = True

        # Vertical lines
        start = mult * int(-self.perspSize / 2 / mult)
        x = start
        while x <= (self.perspSize / 2):
            # Get the right color for this line
            #multX = (x == 0) ? x : (x < 0) ? int(x / mult - 0.5f) : int(x / mult + 0.5f)
            if x == 0:
                multX = x
            elif x < 0:
                multX = int(x / mult - 0.5)
            else:
                multX = int(x / mult +0.5)

            #colour = (multX == 0) ? vertAxisColor : (multX % (int) self.division) ? self.colour2 : self.colour1
            if multX == 0:
                colour = vertAxisColor
            elif multX % int(self.division):
                colour = self.colour2
            else:
                colour = self.colour1

            # Add the line
            self.pGrid.position(x, 0, -self.perspSize / 2)
            self.pGrid.colour(colour)
            self.pGrid.position(x, 0, self.perspSize / 2)
            self.pGrid.colour(colour)

            x += mult

        # Horizontal lines
        y = start
        while y <= (self.perspSize / 2):
            # Get the right color for this line
            #multY = (y == 0) ? y : (y < 0) ? int(y / mult - 0.5f) : int(y / mult + 0.5f)
            if y == 0:
                multY = y
            elif y < 0:
                multY = int(y / mult - 0.5)
            else:
                multY = int(y / mult +0.5)

            #colour = (multY == 0) ? horAxisColor : (multY % int(self.division)) ? self.colour2 : self.colour1
            if multY == 0:
                colour = horAxisColor
            elif multY % int(self.division):
                colour = self.colour2
            else:
                colour = self.colour1
            # Add the line
            self.pGrid.position(-self.perspSize / 2, 0, y)
            self.pGrid.colour(colour)
            self.pGrid.position(self.perspSize / 2, 0, y)
            self.pGrid.colour(colour)

            y += mult

        self.pGrid.end()

        # Normal orientation, grid in the X-Z plane
        self.pNode.resetOrientation()


    def __checkUpdate(self):
        update = False

        pCamera = self.viewport.getCamera()
        if not pCamera:
            return False

        if pCamera is not self.prevCamera:
            self.prevCamera = pCamera
            update = True


        ortho = (pCamera.getProjectionType() == og.PT_ORTHOGRAPHIC)
        if ortho is not self.prevOrtho:
            self.prevOrtho = ortho
            update = True

            # Set correct material properties
            pMaterial = og.MaterialManager.getSingleton().getByName(self.sMatName)

            pMaterial.setDepthWriteEnabled(not ortho)
            pMaterial.setDepthCheckEnabled(not ortho)

        #return update || ortho ? checkUpdateOrtho() : checkUpdatePersp()
        if update:
            if ortho:
                return self.__checkUpdateOrtho()
            else:
                return self.__checkUpdatePersp()

        return False


    def __checkUpdateOrtho(self):
        update = False

        pCamera = self.viewport.getCamera()
        if not pCamera:
            return False

        if pCamera.getPosition() is not self.prevCamPos:
            self.prevCamPos = pCamera.getPosition()
            update = True

        if pCamera.getNearClipDistance() is not self.prevNear:
            self.prevNear = pCamera.getNearClipDistance()
            update = True

        if pCamera.getFOVy() is not self.prevOrtho:
            self.prevOrtho = pCamera.getFOVy()
            update = True

        if pCamera.getAspectRatio() is not self.prevAspectRatio:
            self.prevAspectRatio = pCamera.getAspectRatio()
            update = True

        return update


    def __checkUpdatePersp(self):
        return False
