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


import sys
import platform
import os

from PyQt4 import QtGui
import ogre.renderer.OGRE as og

class OgreWidget(QtGui.QWidget):
    def __init__(self, renderWindowName, ogreRoot, sceneManager, cameraName, parent,  camDistFromFocusNode=100):
        QtGui.QWidget.__init__(self, parent)
        self.painted = False
        self.renderWindowName = renderWindowName
        self.parent = parent
        self.ogreRoot = ogreRoot
        self.sceneManager = sceneManager
        self.cameraName = cameraName
        self.camDistFromFocusNode = camDistFromFocusNode
        self.initOgreWindow(renderWindowName,cameraName)
        self.resizeEventListener = []
        self.ogreViewportCreatedCallback = None
        self.renderWindow = None

    def initOgreWindow(self, renderWindowName, cameraName):
        self.renderParameters = og.NameValuePairList()
        

        if platform.system() == "Windows" or platform.system() == "MAC":
            hwnd = int(self.winId())
            self.renderParameters['externalWindowHandle'] = str(hwnd)
        else:
            win = str(int(self.winId()))
            self.renderParameters['parentWindowHandle'] = win

#        else:
#            import sip
#            info = self.x11Info()
#            disp =  str(sip.unwrapinstance(info.display()))
#            scr = str(info.screen())
#            win = str(int(self.winId()))
#            winHandle = disp + ':' + scr + ':' + win
#            
#            self.renderParameters['parentWindowHandle'] = win


    def setBackgroundColor(self, colorValue):
        if self.painted:
            self.viewport.BackgroundColour = colorValue
        else:
            self.backGroundColor = colorValue

    def setOgreViewportCreatedCallback(self, callback):
        self.ogreViewportCreatedCallback = callback
    
    def paintEvent(self, event):
        if not self.painted:
            renderWindow = self.ogreRoot.createRenderWindow(self.renderWindowName, self.width(), self.height(),
                                                False, self.renderParameters)

            renderWindow.active = True
            self.renderWindow = renderWindow
            
            self.camera = self.sceneManager.createCamera(self.cameraName)
            self.camera.NearClipDistance = 0.5

            # Create focus node (camera always points at this)
            self.camFocusNode = self.sceneManager.getRootSceneNode().createChildSceneNode()
            self.camFocusNode.setFixedYawAxis(True, og.Vector3().UNIT_Y)
            # camera node is offset a ways along the Z axis of focus node
            self.camNode = self.camFocusNode.createChildSceneNode()
            # fix yaw on this one too for when we manipulate it directly
            self.camNode.setFixedYawAxis(True, og.Vector3().UNIT_Y)
            self.camNode.setPosition(0, 0, self.camDistFromFocusNode)
            self.camNode.attachObject(self.camera)
            
            self.viewport = self.renderWindow.addViewport(self.camera, 0, 0.0, 0.0, 1.0, 1.0)
            self.viewport.setClearEveryFrame(True)
            self.viewport.BackgroundColour = self.backGroundColor
            
            if self.ogreViewportCreatedCallback:
                self.ogreViewportCreatedCallback()
            
            self.painted = True
        
    def resizeEvent(self, event):
        if self.renderWindow:
            self.renderWindow.resize(event.size().width(), event.size().height())
            self.renderWindow.windowMovedOrResized()
            self.viewport.update()

            self.renderWindow.update(True)
            self.ogreRoot.renderOneFrame()
            
            if self.camera:
                self.camera.setAspectRatio(float(event.size().width()) / float(event.size().height()));
                
        for listener in self.resizeEventListener:
            listener(event.size().width(), event.size().height())

    def addResizeEventListener(self, listener):
        self.resizeEventListener.append(listener)

    def addOgreResourceLocation(self, location, locType, resGroup, recursive=False):
        og.ResourceGroupManager.getSingleton().addResourceLocation(location, locType, resGroup, recursive)

    def getCamera(self):
        assert(self.camera)
        return self.camera
    
    def getWidth():
        return self.renderWindow.getActualWidth()
        
    def getHeight():
        return self.renderWindow.getActualHeight()
    
    #(const Ogre::Radian& horz, const Ogre::Radian& vert)
    def orbitCamera( self, horz, vert):
        #orbit around focus node
        #yaw around world Y, pitch around local
        self.camFocusNode.yaw(horz, og.Node.TS_WORLD)
        self.camFocusNode.pitch(-vert, og.Node.TS_LOCAL)

    #void ViewOgreWindow::zoomCamera(float zoom)
    def zoomCamera(self, zoom):
        #in perspective mode, zoom by altering camera orbit distance from focus
        self.camNode.translate(0, 0, -zoom * 2, og.Node.TS_LOCAL)
        #make sure we don't go into -Z or even beyond clip distance to focus
        pos = self.camNode.getPosition()
        if pos.z < self.camera.getNearClipDistance():
            self.camNode.setPosition(pos.x, pos.y, self.camera.getNearClipDistance())

    #Ogre::Vector3& trans
    def dollyCamera(self,  trans):
        self.camFocusNode.translate(trans, og.Node.TS_LOCAL)

    #(const Ogre::Radian& horz, const Ogre::Radian& vert)
    def panCamera( self, horz, vert):
        self.camNode.yaw(horz, og.Node.TS_WORLD)
        self.camNode.pitch(vert, og.Node.TS_LOCAL)
        
    def update(self):
        if self.renderWindow is not None:
            self.renderWindow.update(True)

