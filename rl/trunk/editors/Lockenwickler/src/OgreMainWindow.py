import os
import sys
import platform

from PyQt4 import QtGui, QtCore

import OgreWidget
import ogre.renderer.OGRE as og

# this class is the heart of the 3d part
# it manages the two ogre render windows and recieves events from the windows through the event filter
class OgreMainWindow(QtGui.QWidget):
    def __init__(self, moduleManager,  ogreRoot, OgreMainWinSceneMgr,  parent):
        QtGui.QWidget.__init__(self, parent)
        self.moduleManager = moduleManager
        self.ogreRoot = ogreRoot
        self.OgreMainWinSceneMgr = OgreMainWinSceneMgr

        self.ogreRenderWindow = None

        self.leftMouseDown = False
        self.middleMouseDown = False
        self.rightMouseDown = False

        self.mDollyCamera = False

        self.moveCamForward = False
        self.moveCamBackward = False
        self.strafeCamLeft = False
        self.strafeCamRight = False

        self.camUpdateTimer = QtCore.QTimer(self)
        self.camUpdateTimer.connect(self.camUpdateTimer, QtCore.SIGNAL("timeout()"), self.updateCamera)

        self.lastSelectionClick = None # Qpoint wich saves the last selection click mouse position

        self.setupUi(self)

    def setupUi(self, Form):
        Form.setObjectName("Form")
        Form.resize(QtCore.QSize(QtCore.QRect(0,0,935,843).size()).expandedTo(Form.minimumSizeHint()))

        self.gridlayout = QtGui.QGridLayout(Form)
        self.gridlayout.setObjectName("gridlayout")

        # create the vertical splitter ( contains the preferences buttons and the horizontal splitter with the two render windows )
        self.splitterV = QtGui.QSplitter(Form)

        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.MinimumExpanding,QtGui.QSizePolicy.MinimumExpanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.splitterV.sizePolicy().hasHeightForWidth())
        self.splitterV.setSizePolicy(sizePolicy)
        self.splitterV.setOrientation(QtCore.Qt.Vertical)
        self.splitterV.setObjectName("splitter")

        # create the preferences buttons and connect the signals
        self.ogreWindowOptions = QtGui.QToolButton(self)
        QtCore.QObject.connect(self.ogreWindowOptions, QtCore.SIGNAL("clicked()"),
                                    self.onPreferencesButton)
        self.ogreWindowOptions.hide()

        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Maximum,QtGui.QSizePolicy.Minimum)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.ogreWindowOptions.sizePolicy().hasHeightForWidth())
        self.ogreWindowOptions.setSizePolicy(sizePolicy)
        self.ogreWindowOptions.setObjectName("ogreWindowPreferences")
        self.splitterV.addWidget(self.ogreWindowOptions)

        # create the horizontal splitter wich contains the two ogre render windows and add it to the vertical splitter

        ##################################
        self.ogreRenderWindow = OgreWidget.OgreWidget("OgreMainWin", self.ogreRoot, self.OgreMainWinSceneMgr, "MainCam", self.splitterV,  0)
        self.ogreRenderWindow.setMinimumSize(QtCore.QSize(250,250))

        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Maximum,QtGui.QSizePolicy.Maximum)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.ogreRenderWindow.sizePolicy().hasHeightForWidth())
        self.ogreRenderWindow.setSizePolicy(sizePolicy)
        self.ogreRenderWindow.setObjectName("ogreWidget")
        self.splitterV.addWidget(self.ogreRenderWindow)
        self.ogreRenderWindow.setBackgroundColor(og.ColourValue(0, 1, 1))
        ####################################

        self.gridlayout.addWidget(self.splitterV,0,0,1,1)

        # register the eventfilters for the render windows
        # this is needed to catch mouse enter and mouse leave events for these windows
        self.ogreRenderWindow.installEventFilter(self)

        self.lastMousePosX = 0
        self.lastMousePosY = 0

        self.retranslateUi(Form)
        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        Form.setWindowTitle(QtGui.QApplication.translate("Form", "Form", None, QtGui.QApplication.UnicodeUTF8))
        self.ogreWindowOptions.setText(QtGui.QApplication.translate("Form", "...", None, QtGui.QApplication.UnicodeUTF8))

    def onPreferencesButton(self):
        self.splitterH.setOrientation(QtCore.Qt.Vertical)

    def keyPressEvent(self,  event):
        if event.key() == QtCore.Qt.Key_W:
            self.moveCamForward = True
        elif event.key() == QtCore.Qt.Key_S:
            self.moveCamBackward = True
        elif event.key() == QtCore.Qt.Key_A:
            self.strafeCamLeft = True
        elif event.key() == QtCore.Qt.Key_D:
            self.strafeCamRight= True

    def keyReleaseEvent(self,  event):
        if event.key() == QtCore.Qt.Key_W:
            self.moveCamForward = False
        elif event.key() == QtCore.Qt.Key_S:
            self.moveCamBackward = False
        elif event.key() == QtCore.Qt.Key_A:
            self.strafeCamLeft = False
        elif event.key() == QtCore.Qt.Key_D:
            self.strafeCamRight= False

    def eventFilter(self, obj, event):
        if event.type() == 2:
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

        return False

    #calculates the the selection ray and notifies the ModuleManager that something is about to be selected
    def calculateSelectionRay(self,  event):
        relMousePos = self.ogreRenderWindow.mapFromGlobal(QtCore.QPoint(event.globalX(),  event.globalY())) # get the mose position relative to the ogre window

        if self.lastSelectionClick != None:
            if self.lastSelectionClick.x() == relMousePos.x() and self.lastSelectionClick.y() == relMousePos.y(): # mouse didn't move
                # we don't initiate a new selection based on bounding boxes here, we just iterate through the list generated last time a selection was made
                # this is based on how far the distance of the object is from the camera (as returned by ogre)
                self.moduleManager.iterateEntityUnderMouse() # don't select something new, switch through the currently selected models
                return

        self.lastSelectionClick = relMousePos
        screenX = relMousePos.x()/float(self.ogreRenderWindow.viewport.getActualWidth())
        screenY = relMousePos.y()/float(self.ogreRenderWindow.viewport.getActualHeight())

        mouseRay = self.ogreRenderWindow.getCamera().getCameraToViewportRay(screenX, screenY)

        if event.modifiers() == QtCore.Qt.ControlModifier:
            self.moduleManager.selectionClick(mouseRay,  True,  False)
        elif event.modifiers() == QtCore.Qt.ShiftModifier:
            self.moduleManager.selectionClick(mouseRay,  False,  True)
        else:
            self.moduleManager.selectionClick(mouseRay)

    def updateCamera(self):
        if self.moveCamForward:
            self.ogreRenderWindow.dollyCamera(og.Vector3( 0, 0,-0.2))
        if self.moveCamBackward:
            self.ogreRenderWindow.dollyCamera(og.Vector3( 0, 0, 0.2))
        if self.strafeCamLeft:
            self.ogreRenderWindow.dollyCamera(og.Vector3(-0.2, 0 , 0))
        if self.strafeCamRight:
            self.ogreRenderWindow.dollyCamera(og.Vector3( 0.2, 0, 0))
