 #################################################
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
import subprocess

sys.path.insert(0,'..')
import PythonOgreConfig

from random import randint

from PyQt4 import QtGui, QtCore
from PreferencesDialog import *
from ObjectPropertyWin import *
from ModelSelectionDialog import *
from MaterialSelectionDialog import *
from GameObjectClassView import *
from ConsoleWindow import *
from ModuleManager import *
from ModuleExplorer import *
from NewModuleWizard import *
from PivotRenderQueueListener import *

import OgreMainWindow
import ogre.renderer.OGRE as og

class Lockenwickler(QtGui.QMainWindow):
    def __init__(self, parent=None):
        QtGui.QWidget.__init__(self, parent)

#        pixmap = QPixmap("media/icons/lockenwickler_provisorium.png")
#        splash = QSplashScreen(pixmap, Qt.WindowStaysOnTopHint)
#        splash.setMask(pixmap.mask())
#        splash.showMessage("Starting...")
#        splash.show() 

        self.setupUi()

        self.consoleWindow = ConsoleWindow(False,  self)

        self.setupOgre()

        self.prefDialog = PreferencesDialog(self)
        self.objectPropertyWin = ObjectPropertyWin(self.OgreMainWinSceneMgr, self.gocManager, self)
        self.moduleExplorerWin = ModuleExplorer(self)
        self.modelSelectionDialog = ModelSelectionDialog(self.ogreRoot, self)
        self.materialSelectionDialog = MaterialSelectionDialog(self.ogreRoot, self)
        self.moduleManager.modelSelectionDialog = self.modelSelectionDialog
        self.moduleManager.materialSelectionDialog = self.materialSelectionDialog

        self.gameObjectClassView = GameObjectClassView(self.moduleManager.gocManager)

        self.createDockWindows()

        self.mainTimer = QtCore.QTimer(self)
        self.mainTimer.connect(self.mainTimer, QtCore.SIGNAL("timeout()"), self.update)
        self.mainTimer.start(5)

        settings = QtCore.QSettings()
        self.restoreGeometry(settings.value("MainWindow/Geometry").toByteArray())
        self.restoreState(settings.value("MainWindow/DockWindows").toByteArray())
        if not self.prefDialog.setCfgPath(settings.value("Preferences/moduleCfgPath").toString()):
            self.prefDialog.show()
            self.moduleManager.moduleCfgPath = self.prefDialog.moduleCfgPath
        else:
            self.moduleManager.moduleCfgPath = self.prefDialog.moduleCfgPath
        
        self.moduleManager.setModuleExplorer(self.moduleExplorerWin)
        self.moduleManager.setPropertyWindow(self.objectPropertyWin)
        self.moduleManager.setContextMenuCallback(self.onContextMenuCallback)
        
        self.setWindowIcon(QIcon("media/icons/lockenwickler_provisorium_small.png"))
        self.setWindowTitle("Rastullahs Lockenwickler")
        
        self.editorSetupFinished = False
        
#        splash.finish(self)

    def createAction(self, text, slot=None, shortcut=None, icon=None, tip=None, checkable=False, signal="triggered()"):
        action = QtGui.QAction(text, self)
        if icon is not None:
            action.setIcon(QtGui.QIcon("media/icons/%s" % icon))
        if shortcut is not None:
            action.setShortcut(shortcut)
        if tip is not None:
            action.setToolTip(tip)
            action.setStatusTip(tip)
        if slot is not None:
            self.connect(action, QtCore.SIGNAL(signal), slot)

        action.setCheckable(checkable)

        return action

    def addActions(self, target, actions):
        for act in actions:
            if act is None:
               target.addSeparator()
            else:
                target.addAction(act)

    def setupUi(self):
        self.setObjectName("MainWindow")

        self.centralwidget = QtGui.QWidget(self)
        self.centralwidget.setObjectName("centralwidget")

        self.hboxlayout = QtGui.QHBoxLayout(self.centralwidget)
        self.hboxlayout.setContentsMargins(0, 0, 0, 0)
        self.hboxlayout.setObjectName("hboxlayout")

        self.gridlayout = QtGui.QGridLayout()
        self.gridlayout.setObjectName("gridlayout")
        self.gridlayout.setContentsMargins(0, 0, 0, 0)
        
        self.menubar = QtGui.QMenuBar(self)
        self.menubar.setObjectName("menubar")

        self.menuFile = QtGui.QMenu(self.menubar)
        self.menuFile.setObjectName("menuFile")

        self.menuEdit = QtGui.QMenu(self.menubar)
        self.menuEdit.setObjectName("menuEdit")

        self.menuView = QtGui.QMenu(self.menubar)
        self.menuView.setObjectName("menuView")
        self.setMenuBar(self.menubar)


        self.statusbar = QtGui.QStatusBar(self)
        self.statusbar.setObjectName("statusbar")
        self.setStatusBar(self.statusbar)

#####################################
        self.actionNeu =self.createAction("&New Module",  self.actionNewSlot,  QKeySequence.New,  "filenew.png",  "New Module")
        self.actionNeu.setObjectName("actionNeu")

        self.actionOpen = self.createAction("&Open Module",  self.actionOpenSlot,  QKeySequence.Open,  "fileopen.png",  "Open Module")
        self.actionOpen.setObjectName("actionOpen")
        
        self.actionSave = self.createAction("&Save",  self.actionSaveSlot,  QKeySequence.Save,  "filesave.png",  "Save Module")
        self.actionSave.setObjectName("actionSave")
        
        self.actionRunModule = self.createAction("&Save and Run",  self.actionRunModuleSlot,  "Alt+R",  "fileexport.png",  "Save And Run Module")
        self.actionRunModule.setObjectName("actionRunModule")

        self.actionClose = self.createAction("Quit",  self.actionQuitSlot,  "Alt+Q",  "exit.png",  "Quit")
        self.actionClose.setObjectName("actionQuit")
#####################################


#####################################
        self.actionDelete = self.createAction("Delete",  self.actionDeleteSlot,  QKeySequence.Delete,  "editdelete.png",  "Delete")
        self.actionDelete.setObjectName("actionDelete")

        self.actionCopy = self.createAction("Copy",  self.actionCopySlot,  QKeySequence.Copy,  "editcopy.png",  "Copy")
        self.actionCopy.setObjectName("actionCopy")

        self.actionCut = self.createAction("Cut",  self.actionCutSlot,  QKeySequence.Cut,  "editcut.png",  "Cut")
        self.actionCut.setObjectName("actionCut")

        self.actionPaste = self.createAction("Paste",  self.actionPasteSlot,  QKeySequence.Paste,  "editpaste.png",  "Paste")
        self.actionPaste.setObjectName("actionPaste")

        self.actionSelect = self.createAction("&Select",  self.actionSelectSlot,  "Space",  "cursor.png",  "Move selected object")
        self.actionSelect.setObjectName("actionSelect")

        self.actionMove = self.createAction("&Move",  self.actionMoveSlot,  "g",  "move.png",  "Move selected object")
        self.actionMove.setObjectName("actionMove")

        self.actionRotate = self.createAction("&Rotate",  self.actionRotateSlot,  "r",  "rotate.png",  "Rotate selected object")
        self.actionRotate.setObjectName("actionRotate")

        self.actionScale = self.createAction("&Scale",  self.actionScaleSlot,  "x",  "resizecol.png",  "Scale selected object")
        self.actionScale.setObjectName("actionScale")

        self.actionOneClickEntityPlacement = self.createAction("&OneClickEntityPlacement",  self.actionOneClickEntityPlacementSlot,  "",  "resizecol.png",  "Add an Entity just by a click")
        self.actionOneClickEntityPlacement.setObjectName("actionOneClickEntityPlacement")
        self.actionOneClickEntityPlacement.setCheckable(True)

#####################################
#####################################
        self.actionSceneExplorer = self.createAction("&Scene Exlporer",  self.toggleModuleExplorer,  "Alt+E",  "view_tree.png",  "Module Explorer",  False)
        self.actionSceneExplorer.setObjectName("actionSceneExplorer")
        
        self.actionPreferences = self.createAction("&Preferences",  self.togglePreferencesWindow,  "Alt+P",  "configure.png",  "Lockenwickler Preferences",  False)
        self.actionPreferences.setObjectName("actionPreferences")

        self.actionProperty_Window = self.createAction("Pr&operty Window",  self.togglePropertyWindow,  "Alt+P",  "unsortedlist1.png",  "Property Window")
        self.actionProperty_Window.setObjectName("actionProperty_Window")

        self.actionObject_Selection = self.createAction("&Model Preview Window",  self.toggleModelPreviewWindow,  "Alt+O",  "tux.png",  "Model Preview")
        self.actionObject_Selection.setObjectName("actionObject_Selection")
        
        self.actionMaterial_Selection = self.createAction("Material &Preview Window",  self.toggleMaterialPreviewWindow,  "Alt+M",  "colors.png",  "Material Preview")
        self.actionMaterial_Selection.setObjectName("actionMaterial_Selection")

        self.actionGameObjectClass_Selection = self.createAction("&Game Object Class Preview Window",  self.toggleGameObjectViewWindow,  "Alt+G",  "multirow.png",  "GameObjectClass Preview")
        self.actionGameObjectClass_Selection.setObjectName("actionObject_Selection")

        self.actionConsole_Window = self.createAction("&Console Window",  self.toggleConsoleWindow,  "Alt+C",  "console.png",  "Console Window")
        self.actionConsole_Window.setObjectName("actionConsole_Window")
        
        self.actionToggleViewportGrid = self.createAction("&Toggle Grid",  self.toggleViewportGrid,  "Alt+G",  "console.png",  "Toggle Viewport Grid")
        self.actionToggleViewportGrid.setObjectName("actionToggleViewportGrid")

#####################################
#####################################


        self.menuFile.addAction(self.actionNeu)
        self.menuFile.addAction(self.actionOpen)
        self.menuFile.addAction(self.actionSave)
        self.menuFile.addAction(self.actionRunModule)
        self.menuFile.addAction(self.actionClose)

        self.menuEdit.addAction(self.actionSelect)
        self.menuEdit.addAction(self.actionMove)
        self.menuEdit.addAction(self.actionRotate)
        self.menuEdit.addAction(self.actionScale)
        self.menuEdit.addSeparator()
        self.menuEdit.addAction(self.actionDelete)
        self.menuEdit.addAction(self.actionCopy)
        self.menuEdit.addAction(self.actionCut)
        self.menuEdit.addAction(self.actionPaste)
        self.menuEdit.addSeparator()
        self.menuEdit.addAction(self.actionOneClickEntityPlacement)
        

        self.menuView.addAction(self.actionSceneExplorer)
        self.menuView.addAction(self.actionPreferences)
        self.menuView.addAction(self.actionProperty_Window)
        self.menuView.addAction(self.actionObject_Selection)
        self.menuView.addAction(self.actionMaterial_Selection)
        self.menuView.addAction(self.actionGameObjectClass_Selection)
        self.menuView.addAction(self.actionConsole_Window)
        self.menuView.addAction(self.actionToggleViewportGrid)
        
        self.menubar.addAction(self.menuFile.menuAction())
        self.menubar.addAction(self.menuEdit.menuAction())
        self.menubar.addAction(self.menuView.menuAction())

        self.retranslateUi()
        QtCore.QMetaObject.connectSlotsByName(self)

    def retranslateUi(self):
        self.setWindowTitle(QtGui.QApplication.translate("MainWindow", "MainWindow", None, QtGui.QApplication.UnicodeUTF8))
        self.menuFile.setTitle(QtGui.QApplication.translate("MainWindow", "File", None, QtGui.QApplication.UnicodeUTF8))
        self.menuEdit.setTitle(QtGui.QApplication.translate("MainWindow", "Edit", None, QtGui.QApplication.UnicodeUTF8))
        self.menuView.setTitle(QtGui.QApplication.translate("MainWindow", "View", None, QtGui.QApplication.UnicodeUTF8))
        self.actionNeu.setText(QtGui.QApplication.translate("MainWindow", "New Module", None, QtGui.QApplication.UnicodeUTF8))
        self.actionMove.setText(QtGui.QApplication.translate("MainWindow", "Move", None, QtGui.QApplication.UnicodeUTF8))
        self.actionRotate.setText(QtGui.QApplication.translate("MainWindow", "Rotate", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSceneExplorer.setText(QtGui.QApplication.translate("MainWindow", "Module Explorer", None, QtGui.QApplication.UnicodeUTF8))
        self.actionPreferences.setText(QtGui.QApplication.translate("MainWindow", "Preferences", None, QtGui.QApplication.UnicodeUTF8))
        self.actionProperty_Window.setText(QtGui.QApplication.translate("MainWindow", "Property Window", None, QtGui.QApplication.UnicodeUTF8))
        self.actionObject_Selection.setText(QtGui.QApplication.translate("MainWindow", "Object Selection", None, QtGui.QApplication.UnicodeUTF8))
        self.actionClose.setText(QtGui.QApplication.translate("MainWindow", "Quit", None, QtGui.QApplication.UnicodeUTF8))
        self.actionConsole_Window.setText(QtGui.QApplication.translate("MainWindow", "Console Window", None, QtGui.QApplication.UnicodeUTF8))

    def setupOgre(self, pluginCfgPath="./Plugins.cfg", ogreCfgPath="./ogre.cfg", logPath="./ogre.log"):
        if platform.system() == "Windows":
            pluginCfgPath="./Plugins-windows.cfg"
        else:
            pluginCfgPath="./Plugins-linux.cfg"

        root = og.Root(pluginCfgPath, ogreCfgPath, logPath)
        self.ogreRoot = root

        if  not self.ogreRoot.restoreConfig() and not self.ogreRoot.showConfigDialog():
            sys.exit('Quit from Config Dialog')

        root.initialise(False)

        self.pivotRenderQueueListener = PivotRenderQueueListener()
        self.OgreMainWinSceneMgr = self.ogreRoot.createSceneManager(og.ST_GENERIC, "OgreMainWinSceneMgr")
        self.OgreMainWinSceneMgr.ambientLight = og.ColourValue(4, 4, 4)
        self.OgreMainWinSceneMgr.addRenderQueueListener(self.pivotRenderQueueListener)
        
        self.moduleName = ""
        self.moduleManager = ModuleManager(self.ogreRoot,  self.OgreMainWinSceneMgr)
        self.gocManager = self.moduleManager.gocManager
        
        self.ogreMainWindow = OgreMainWindow.OgreMainWindow(self.moduleManager,  root,  self.OgreMainWinSceneMgr,  self)
        self.gridlayout.addWidget(self.ogreMainWindow,0,0,1,1)
        self.hboxlayout.addLayout(self.gridlayout)
        self.setCentralWidget(self.centralwidget)
        
        oglog = og.LogManager.getSingleton().getDefaultLog()
        oglog.addListener(self.consoleWindow.lockenLog)

    def finishEditorSetup(self):
        if not self.editorSetupFinished:
            og.ResourceGroupManager.getSingleton().addResourceLocation("./media", "FileSystem", "General", False)
            og.ResourceGroupManager.getSingleton().initialiseAllResourceGroups()

            self.moduleManager.pivot = Pivot(self.OgreMainWinSceneMgr)
            self.moduleManager.pivot.hide()
            self.editorSetupFinished = True
        
    def update(self):
        self.ogreRoot.renderOneFrame()
        if platform.system() == "Linux":
            self.ogreMainWindow.updateRenderWindow()
            self.modelSelectionDialog.updateRenderWindow()
            self.materialSelectionDialog.updateRenderWindow()

    def actionOpenSlot(self):
        self.finishEditorSetup()
        self.moduleManager.openLoadModuleDialog()

    def actionNewSlot(self):
        newModuleWiz = NewModuleWizard(self.moduleManager, self)
        newModuleWiz.exec_()
        return
        
    def actionSaveSlot(self):
        self.moduleManager.save()
        
    def actionRunModuleSlot(self):
        self.moduleManager.save()
        if platform.system() == "Windows":
            workingDir = self.prefDialog.moduleCfgPath.replace("/modules/modules.cfg", "")
            executable = os.path.join(workingDir, "Rastullah.exe")
            executable = executable.replace("/",  "\\")
            if os.path.isfile(executable):
                subprocess.Popen([executable, "--module", self.moduleManager.mainModule.name], 0, None, None, None, None, None, False, False, workingDir)

        
    def actionQuitSlot(self):
        self.close()

    def actionDeleteSlot(self):
        self.moduleManager.deleteObjects()

    def actionCopySlot(self):
        self.moduleManager.copyObjects()

    def actionCutSlot(self):
        self.moduleManager.cutObjects()

    def actionPasteSlot(self):
        self.moduleManager.pasteObjects(self.ogreMainWindow.getCameraToViewportRay())

    def actionSelectSlot(self):
        self.moduleManager.pivot.hide()

    def actionMoveSlot(self):
        self.moduleManager.pivot.setMoveMode()

    def actionRotateSlot(self):
        self.moduleManager.pivot.setRotateMode()

    def actionScaleSlot(self):
        self.moduleManager.pivot.setScaleMode()

    def actionOneClickEntityPlacementSlot(self):
        self.moduleManager.setOneClickEntityPlacement(self.actionOneClickEntityPlacement.isChecked())

    def togglePreferencesWindow(self):
        if self.prefDialog.isHidden():
            self.prefDialog.show()
        else:
            self.prefDialog.hide()

    def toggleModelPreviewWindow(self):
        if self.modelSelectionDock.isHidden():
            self.modelSelectionDock.show()
        else:
            self.modelSelectionDock.hide()
    
    def toggleMaterialPreviewWindow(self):
        if self.materialSelectionDock.isHidden():
            self.materialSelectionDock.show()
        else:
            self.materialSelectionDock.hide()

    def toggleGameObjectViewWindow(self):
        if self.gameObjectClassViewDock.isHidden():
            self.gameObjectClassViewDock.show()
        else:
            self.gameObjectClassViewDock.hide()

    def toggleModuleExplorer(self):
        if self.moduleExplorerDock.isHidden():
            self.moduleExplorerDock.show()
        else:
            self.moduleExplorerDock.hide()

    def togglePropertyWindow(self):
        if self.propertyDock.isHidden():
            self.propertyDock.show()
        else:
            self.propertyDock.hide()

    def toggleConsoleWindow(self):
        if self.consoleDock.isHidden():
            self.consoleDock.show()
        else:
            self.consoleDock.hide()

    def toggleViewportGrid(self):
        self.ogreMainWindow.toggleViewportGrid()

    def createDockWindows(self):
        self.propertyDock = QtGui.QDockWidget(self.tr("Properties"), self)
        self.propertyDock.setObjectName("PropertyDockWindow")
        self.propertyDock.setAllowedAreas(QtCore.Qt.LeftDockWidgetArea | QtCore.Qt.RightDockWidgetArea)
        self.propertyDock.setWidget(self.objectPropertyWin)
        self.addDockWidget(QtCore.Qt.LeftDockWidgetArea, self.propertyDock)

        self.modelSelectionDock = QtGui.QDockWidget(self.tr("Models"), self)
        self.modelSelectionDock.setObjectName("ModelSelectionDockWindow")
        self.modelSelectionDock.setAllowedAreas(QtCore.Qt.LeftDockWidgetArea | QtCore.Qt.RightDockWidgetArea)
        self.modelSelectionDock.setWidget(self.modelSelectionDialog)
        self.addDockWidget(QtCore.Qt.RightDockWidgetArea, self.modelSelectionDock)
        
        self.materialSelectionDock = QtGui.QDockWidget(self.tr("Materials"), self)
        self.materialSelectionDock.setObjectName("MaterialSelectionDockWindow")
        self.materialSelectionDock.setAllowedAreas(QtCore.Qt.LeftDockWidgetArea | QtCore.Qt.RightDockWidgetArea)
        self.materialSelectionDock.setWidget(self.materialSelectionDialog)
        self.addDockWidget(QtCore.Qt.RightDockWidgetArea, self.materialSelectionDock)

        self.gameObjectClassViewDock = QtGui.QDockWidget(self.tr("GameObjectClasses"), self)
        self.gameObjectClassViewDock.setObjectName("GameObjectClassView")
        self.gameObjectClassViewDock.setAllowedAreas(QtCore.Qt.LeftDockWidgetArea | QtCore.Qt.RightDockWidgetArea)
        self.gameObjectClassViewDock.setWidget(self.gameObjectClassView)
        self.addDockWidget(QtCore.Qt.RightDockWidgetArea, self.gameObjectClassViewDock)

        self.moduleExplorerDock = QtGui.QDockWidget(self.tr("Module Explorer"), self)
        self.moduleExplorerDock.setObjectName("ModuleExplorerDockWindow")
        self.moduleExplorerDock.setAllowedAreas(QtCore.Qt.LeftDockWidgetArea | QtCore.Qt.RightDockWidgetArea)
        self.moduleExplorerDock.setWidget(self.moduleExplorerWin)
        self.addDockWidget(QtCore.Qt.LeftDockWidgetArea, self.moduleExplorerDock)

        self.consoleDock = QtGui.QDockWidget(self.tr("Console"), self)
        self.consoleDock.setObjectName("ConsoleDockWindow")
        self.consoleDock.setAllowedAreas(QtCore.Qt.BottomDockWidgetArea | QtCore.Qt.TopDockWidgetArea)
        self.consoleDock.setWidget(self.consoleWindow)
        self.addDockWidget(QtCore.Qt.BottomDockWidgetArea, self.consoleDock)

        self.fileToolBar = self.addToolBar("File Toolbar")
        self.fileToolBar.setObjectName("FileToolBar")
        self.fileToolBar.setAllowedAreas(QtCore.Qt.TopToolBarArea | QtCore.Qt.BottomToolBarArea)
        self.fileToolBar.addAction(self.actionNeu)
        self.fileToolBar.addAction(self.actionOpen)
        self.fileToolBar.addAction(self.actionSave)
        self.fileToolBar.addAction(self.actionRunModule)
        self.fileToolBar.addAction(self.actionClose)
        self.addToolBar(QtCore.Qt.TopToolBarArea, self.fileToolBar)

        self.moveToolBar = self.addToolBar("Transformation Bar")
        self.moveToolBar.setObjectName("TransformationBar")
        self.moveToolBar.setAllowedAreas(QtCore.Qt.TopToolBarArea | QtCore.Qt.BottomToolBarArea)
        self.moveToolBar.addAction(self.actionSelect)
        self.moveToolBar.addAction(self.actionMove)
        self.moveToolBar.addAction(self.actionRotate)
        self.moveToolBar.addAction(self.actionScale)
        self.addToolBar(QtCore.Qt.TopToolBarArea, self.moveToolBar)

    def keyPressEvent(self,  event):
        if not event.isAutoRepeat():
            self.ogreMainWindow.keyPressEvent(event)

    def keyReleaseEvent(self,  event):
        if not event.isAutoRepeat():
            self.ogreMainWindow.keyReleaseEvent(event)
        pass

    def onContextMenuCallback(self, actions, menus):
        menu = QMenu("My Menu!!")
        menu.addAction(self.actionDelete)
        menu.addAction(self.actionCopy)
        menu.addAction(self.actionCut)
        menu.addAction(self.actionPaste)
        menu.addSeparator()
        
        for m in menus:
            menu.addMenu(m)
        for a in actions:
            menu.addAction(a)

            
        menu.exec_(QCursor.pos())
        

    def connectActionButtons(self):
        pass

    def saveOnClose(self):
#        reply = QtGui.QMessageBox.question(self,  "Rastullahs Lockenwickler - Unsaved Chages",  "Save unsaved changes?",  QtGui.QMessageBox.Yes|QtGui.QMessageBox.No|QtGui.QMessageBox.Cancel)
#        if reply == QtGui.QMessageBox.Cancel:
#            return False
#        if reply == QtGui.QMessageBox.Yes:
#            print""
#            #TODO: implement save here
        return True

    def closeEvent(self,  event):
        if self.saveOnClose():
            settings = QtCore.QSettings()
            settings.setValue("Preferences/moduleCfgPath", QtCore.QVariant(self.prefDialog.lineEdit.text()))
            settings.setValue("MainWindow/Geometry",  QtCore.QVariant(self.saveGeometry()))
            settings.setValue("MainWIndow/DockWindows",  QtCore.QVariant(self.saveState()))
        else:
            event.ignore()

if __name__ == "__main__":
#    # Import Psyco if available
#    try:
#        import psyco
#        psyco.full(0.02)
#        #psyco.log()
#        #psyco.profile()
#    except ImportError:
#        pass
    
    app = QtGui.QApplication(sys.argv)
    app.setOrganizationName("Team Pantheon")
    app.setOrganizationDomain("rastullahs-lockenpracht.de/team")
    app.setApplicationName("Lockenwickler")

    form = Lockenwickler()
    form.show()

    sys.exit(app.exec_())

