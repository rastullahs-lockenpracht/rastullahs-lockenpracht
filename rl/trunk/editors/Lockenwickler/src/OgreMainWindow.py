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


import os
import sys
import platform

from PyQt4.QtGui import *
from PyQt4.QtCore import *

from ViewportGrid import *

import OgreWidget
import ogre.renderer.OGRE as og

# this class is the heart of the 3d part
# it manages the two ogre render windows and recieves events from the windows through the event filter
class OgreMainWindow(QWidget):
    def __init__(self, moduleManager,  ogreRoot, OgreMainWinSceneMgr,  parent):
        QWidget.__init__(self, parent)
        self.moduleManager = moduleManager
        self.ogreRoot = ogreRoot
        self.OgreMainWinSceneMgr = OgreMainWinSceneMgr

        self.ogreWidget = None

        self.leftMouseDown = False
        self.middleMouseDown = False
        self.rightMouseDown = False

        self.mDollyCamera = False

        self.moveCamForward = False
        self.moveCamBackward = False
        self.strafeCamLeft = False
        self.strafeCamRight = False

        self.camUpdateTimer = QTimer(self)
        self.camUpdateTimer.connect(self.camUpdateTimer, SIGNAL("timeout()"), self.updateCamera)

        self.lastSelectionClick = None # Qpoint wich saves the last selection click mouse position
        
        self.setupUi(self)

    def setupUi(self, Form):
        Form.setObjectName("Form")
        Form.resize(QSize(QRect(0,0,935,843).size()).expandedTo(Form.minimumSizeHint()))

        self.gridlayout = QGridLayout(Form)
        self.gridlayout.setContentsMargins(0, 2, 0, 0)
        self.gridlayout.setObjectName("gridlayout")

        # create the vertical splitter ( contains the preferences buttons and the horizontal splitter with the two render windows )
        self.splitterV = QSplitter(Form)

        sizePolicy = QSizePolicy(QSizePolicy.MinimumExpanding,QSizePolicy.MinimumExpanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.splitterV.sizePolicy().hasHeightForWidth())
        self.splitterV.setSizePolicy(sizePolicy)
        self.splitterV.setOrientation(Qt.Vertical)
        self.splitterV.setObjectName("splitter")

        # create the horizontal splitter wich contains the two ogre render windows and add it to the vertical splitter

        ##################################
        self.ogreWidget = OgreWidget.OgreWidget("OgreMainWin", self.ogreRoot, self.OgreMainWinSceneMgr, "MainCam", self.splitterV,  0)
        self.ogreWidget.setMinimumSize(QSize(250,250))

        sizePolicy = QSizePolicy(QSizePolicy.Maximum,QSizePolicy.Maximum)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.ogreWidget.sizePolicy().hasHeightForWidth())
        self.ogreWidget.setSizePolicy(sizePolicy)
        self.ogreWidget.setObjectName("ogreWidget")
        self.splitterV.addWidget(self.ogreWidget)
        self.ogreWidget.setBackgroundColor(og.ColourValue(0, 1, 1))
        ####################################

        self.gridlayout.addWidget(self.splitterV,0,0,1,1)

        # register the eventfilters for the render windows
        # this is needed to catch mouse enter and mouse leave events for these windows
        self.ogreWidget.installEventFilter(self)
        self.ogreWidget.setAcceptDrops(True)
        self.lastMousePosX = 0
        self.lastMousePosY = 0

        self.retranslateUi(Form)
        QMetaObject.connectSlotsByName(Form)
        
        self.ogreWidget.setOgreViewportCreatedCallback(self.ogreViewportCreatedCallback)
        
    def ogreViewportCreatedCallback(self):
        self.viewportGrid = ViewportGrid(self.OgreMainWinSceneMgr, self.ogreWidget.viewport)
        self.viewportGrid.enable()
        
    def retranslateUi(self, Form):
        Form.setWindowTitle(QApplication.translate("Form", "Form", None, QApplication.UnicodeUTF8))

    def keyPressEvent(self,  event):
        if event.key() == Qt.Key_W:
            self.moveCamForward = True
        elif event.key() == Qt.Key_S:
            self.moveCamBackward = True
        elif event.key() == Qt.Key_A:
            self.strafeCamLeft = True
        elif event.key() == Qt.Key_D:
            self.strafeCamRight= True

    def keyReleaseEvent(self,  event):
        if event.key() == Qt.Key_W:
            self.moveCamForward = False
        elif event.key() == Qt.Key_S:
            self.moveCamBackward = False
        elif event.key() == Qt.Key_A:
            self.strafeCamLeft = False
        elif event.key() == Qt.Key_D:
            self.strafeCamRight= False

    def eventFilter(self, obj, event):
        if event.type() == 2:
            self.ogreWidget.setFocus()
            if event.button() == 1: # left mouse button is pressed
                self.leftMouseDown = True
                self.moduleManager.leftMouseDown = True

                if self.rightMouseDown: #if right mouse button is already pressed dolly the camera
                    self.mDollyCamera = True
                else:
                    self.calculateSelectionRay(event)

            elif event.button() == 2: # right mouse button is pressed
                self.rightMouseDown = True
                self.moduleManager.rightMouseDown = True
                QTimer.singleShot(50, self.onContextMenuTimer)

            elif event.button() == 4: # middle mouse button is pressed
                self.middleMouseDown = True
                self.moduleManager.middleMouseDown = True

            self.camUpdateTimer.start(15)

        elif event.type() == 3:
            if event.button() == 1: # left mouse button is released
                self.leftMouseDown = False
                self.moduleManager.leftMouseDown = False
                self.moduleManager.leftMouseUp()

                if self.mDollyCamera == True: #if we dolly the camera set it to false
                    self.mDollyCamera = False

            elif event.button() == 2: # right mouse button is released
                self.rightMouseDown = False
                self.moduleManager.rightMouseDown = False
                
            elif event.button() == 4: # middle mouse button is released
                self.middleMouseDown = False
                self.moduleManager.middleMouseDown = False

            if not self.rightMouseDown:
                self.mDollyCamera = False
                self.camUpdateTimer.stop()

            self.lastMousePosX = 0
            self.lastMousePosY = 0

        elif event.type() == 5: #mouse moved while button down
            if self.lastMousePosX == 0: # check to avoid to huge values which may happen when the user clicks and lastMousePosX/Y is zero
                self.lastMousePosX = event.globalX()
            if self.lastMousePosY == 0:# check to avoid to huge values which may happen when the user clicks and lastMousePosX/Y is zero
                self.lastMousePosY = event.globalY()

            incX =  (event.globalX() - self.lastMousePosX)
            incY =  (event.globalY() - self.lastMousePosY)

            if self.moduleManager.pivot is not None and  self.leftMouseDown and not self.middleMouseDown and not self.rightMouseDown:
                self.moduleManager.pivot.onMouseMoved(event.globalX,  event.globalY,  incX,  incY)

            rotX = incX * 0.01
            rotY = incY * 0.01

            if self.mDollyCamera:
                obj.dollyCamera(og.Vector3(rotX, -rotY,  0) * 3)
            elif self.rightMouseDown:
                obj.orbitCamera(-rotX,  rotY)

            self.lastMousePosX = event.globalX()
            self.lastMousePosY = event.globalY()

        if event.type() == 60: #drag enter
            self.dragEnterEvent(event)
        if event.type() == 61: #drag move
            self.dragMoveEvent(event)
#        if event.type() == 62:
#            self.finishDropEvent(event)
        if event.type() == 63:
            self.finishDropEvent(event)

        return False

    def dragEnterEvent(self, event):
        if event.mimeData().hasFormat("application/x-static_model"):
            data = event.mimeData().data("application/x-static_model")
            stream = QDataStream(data, QIODevice.ReadOnly)
            text = QString()
            stream >> text

            self.moduleManager.startDropModelAction(str(text), self.getCameraToViewportRay()) #start the model draging

            event.setDropAction(Qt.CopyAction)
            event.accept()

        elif event.mimeData().hasFormat("application/x-game_object"):
            data = event.mimeData().data("application/x-game_object")
            stream = QDataStream(data, QIODevice.ReadOnly)
            text = QString()
            stream >> text

            self.moduleManager.startDropGameObjectAction(str(text), self.getCameraToViewportRay()) #start the model draging

            event.setDropAction(Qt.CopyAction)
            event.accept()
        elif event.mimeData().hasFormat("application/x-material"):
            data = event.mimeData().data("application/x-material")
            stream = QDataStream(data, QIODevice.ReadOnly)
            text = QString()
            stream >> text

            self.moduleManager.startDropMaterialAction(str(text)) #start the material draging

            event.setDropAction(Qt.CopyAction)
            event.accept()
        else:
            event.ignore()


    def dragMoveEvent(self, event):
        if event.mimeData().hasFormat("application/x-static_model"):
            self.moduleManager.moveDropModelAction(self.getCameraToViewportRay()) #move it with the mouse
            event.accept()
        elif event.mimeData().hasFormat("application/x-game_object"):
            self.moduleManager.moveDropGameObjectAction(self.getCameraToViewportRay()) #move it with the mouse
            event.accept()
        elif event.mimeData().hasFormat("application/x-material"):
            self.moduleManager.moveDropMaterialAction(event) #move it with the mouse
            event.accept()
        else:
            event.ignore()

    def finishDropEvent(self, event):
        if event.mimeData().hasFormat("application/x-static_model"):
            self.moduleManager.finishDropModelAction(self.getCameraToViewportRay())
            event.accept()
        elif event.mimeData().hasFormat("application/x-game_object"):
            self.moduleManager.finishDropGameObjectAction(self.getCameraToViewportRay()) 
            event.accept()
        elif event.mimeData().hasFormat("application/x-material"):
            relMousePos = self.ogreWidget.mapFromGlobal(QCursor.pos())
            self.moduleManager.finishDropMaterialAction(relMousePos.x(), relMousePos.y()) 
            event.accept()
        else:
            event.ignore()

    def getCameraToViewportRay(self):
        relMousePos = self.ogreWidget.mapFromGlobal(QCursor.pos())

        screenX = relMousePos.x()/float(self.ogreWidget.viewport.getActualWidth())
        screenY = relMousePos.y()/float(self.ogreWidget.viewport.getActualHeight())

        return self.ogreWidget.getCamera().getCameraToViewportRay(screenX, screenY)

    #calculates the the selection ray and notifies the ModuleManager that something is about to be selected
    def calculateSelectionRay(self,  event):
        relMousePos = self.ogreWidget.mapFromGlobal(QPoint(event.globalX(),  event.globalY())) # get the mose position relative to the ogre window

#        if self.lastSelectionClick != None:
#            if self.lastSelectionClick.x() == relMousePos.x() and self.lastSelectionClick.y() == relMousePos.y(): # mouse didn't move
#                # we don't initiate a new selection based on bounding boxes here, we just iterate through the list generated last time a selection was made
#                # this is based on how far the distance of the object is from the camera (as returned by ogre)
#                self.moduleManager.iterateEntityUnderMouse() # don't select something new, switch through the currently selected models
#                return

        self.lastSelectionClick = relMousePos
        screenX = relMousePos.x()/float(self.ogreWidget.viewport.getActualWidth())
        screenY = relMousePos.y()/float(self.ogreWidget.viewport.getActualHeight())

        mouseRay = self.ogreWidget.getCamera().getCameraToViewportRay(screenX, screenY)

        if event.modifiers() == Qt.ControlModifier:
            self.moduleManager.selectionClick(relMousePos.x(), relMousePos.y(), mouseRay,  True,  False)
        elif event.modifiers() == Qt.ShiftModifier:
            self.moduleManager.selectionClick(relMousePos.x(), relMousePos.y(), mouseRay,  False,  True)
        else:
            self.moduleManager.selectionClick(relMousePos.x(), relMousePos.y(), mouseRay)

    def getWidth():
        return self.ogreWidget.getWidth()
        
    def getHeight():
        return self.ogreWidget.getHeight()

    def updateCamera(self):        
        if self.moveCamForward:
            self.ogreWidget.dollyCamera(og.Vector3( 0, 0,-0.2))
        if self.moveCamBackward:
            self.ogreWidget.dollyCamera(og.Vector3( 0, 0, 0.2))
        if self.strafeCamLeft:
            self.ogreWidget.dollyCamera(og.Vector3(-0.2, 0 , 0))
        if self.strafeCamRight:
            self.ogreWidget.dollyCamera(og.Vector3( 0.2, 0, 0))
        
        if self.moduleManager.pivot:
            self.moduleManager.pivot.update()
            
    def updateRenderWindow(self):
        self.ogreWidget.update()
        
    def onContextMenuTimer(self):
        if not self.rightMouseDown:
            relMousePos = self.ogreWidget.mapFromGlobal(QCursor.pos()) # get the mose position relative to the ogre window
            self.moduleManager.onContextMenu(relMousePos.x(), relMousePos.y())
    
    def toggleViewportGrid(self):
        if self.viewportGrid.isEnabled():
            self.viewportGrid.disable()
        else:
            self.viewportGrid.enable()
        
