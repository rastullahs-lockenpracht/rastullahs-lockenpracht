import os
import sys
import platform

sys.path.insert(0,'..')
import PythonOgreConfig

from random import randint
from os.path import isfile

from PyQt4 import QtGui, QtCore
from PreferencesDialog import *
from ObjectPropertyWin import *
from ModelSelectionDialog import *
from ConsoleWindow import *
from ModuleManager import *
from SceneExplorer import *

import OgreMainWindow
import ogre.renderer.OGRE as og

class Lockenwickler(QtGui.QMainWindow):
    def __init__(self, parent=None):
        QtGui.QWidget.__init__(self, parent)

        self.setupUi()

        self.consoleWindow = ConsoleWindow(True,  self)

        self.setupOgre()

        self.prefDialog = PreferencesDialog(self.loadModule, self)
        self.objectPropertyWin = ObjectPropertyWin(self)
        self.sceneExplorerWin = SceneExplorer(self)
        self.modelSelectionDialog = ModelSelectionDialog(self.ogreRoot, self)

        self.createDockWindows()

        self.mainTimer = QtCore.QTimer(self)
        self.mainTimer.connect(self.mainTimer, QtCore.SIGNAL("timeout()"), self.update)
        self.mainTimer.start(5)

        settings = QtCore.QSettings()
        self.restoreGeometry(settings.value("MainWindow/Geometry").toByteArray())
        self.restoreState(settings.value("MainWindow/DockWindows").toByteArray())

        self.setWindowTitle("Rastullahs Lockenwickler")
#        # Import Psyco if available
#        try:
#            import psyco
#            psyco.full()
#            #psyco.log()
#            #psyco.profile()
#        except ImportError:
#            pass

        #QtGui.QApplication.setKeyboardInputInterval(5000)

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
        self.hboxlayout.setObjectName("hboxlayout")

        self.gridlayout = QtGui.QGridLayout()
        self.gridlayout.setObjectName("gridlayout")

        self.menubar = QtGui.QMenuBar(self)
        self.menubar.setObjectName("menubar")

        self.menuFile = QtGui.QMenu(self.menubar)
        self.menuFile.setObjectName("menuFile")

        self.menuView = QtGui.QMenu(self.menubar)
        self.menuView.setObjectName("menuView")
        self.setMenuBar(self.menubar)


        self.statusbar = QtGui.QStatusBar(self)
        self.statusbar.setObjectName("statusbar")
        self.setStatusBar(self.statusbar)

        self.actionNeu =self.createAction("&New",  self.close,  "Ctrl + N",  "filenew.png",  "New")
        self.actionNeu.setObjectName("actionNeu")

        self.actionClose = self.createAction("Quit",  self.close,  "Alt + Q",  "exit.png",  "Quit")
        self.actionClose.setObjectName("actionClose")



        self.actionMove = self.createAction("&Move",  self.actionMoveSlot,  "g",  "move.png",  "Move selected object")
        self.actionMove.setObjectName("actionMove")

        self.actionRotate = self.createAction("&Rotate",  self.actionRotateSlot,  "r",  "rotate.png",  "Rotate selected object")
        self.actionRotate.setObjectName("actionRotate")

        self.actionScale = self.createAction("&Scale",  self.actionScaleSlot,  "x",  "resizecol.png",  "Scale selected object")
        self.actionRotate.setObjectName("actionRotate")


        self.actionSceneExplorer = self.createAction("&Scene Exlporer",  self.toggleSceneExplorer,  "Alt + E",  "view_tree.png",  "Scene Explorer",  False)
        self.actionSceneExplorer.setObjectName("actionSceneExplorer")

        self.actionPreferences = self.createAction("&Preferences",  self.togglePreferencesWindow,  "Alt + P",  "configure.png",  "Lockenwickler Preferences",  False)
        self.actionPreferences.setObjectName("actionPreferences")

        self.actionProperty_Window = self.createAction("Pr&operty Window",  self.togglePropertyWindow,  "Alt + P",  "unsortedlist1.png",  "Property Window")
        self.actionProperty_Window.setObjectName("actionProperty_Window")

        self.actionObject_Selection = self.createAction("&Model Preview Window",  self.toggleModelPreviewWindow,  "Alt + O",  "tux.png",  "Model Preview")
        self.actionObject_Selection.setObjectName("actionObject_Selection")

        self.actionConsole_Window = self.createAction("&Console Window",  self.toggleConsoleWindow,  "Alt + C",  "console.png",  "Console Window")
        self.actionConsole_Window.setObjectName("actionConsole_Window")


        self.menuFile.addAction(self.actionNeu)
        self.menuFile.addAction(self.actionClose)
        self.menuView.addAction(self.actionSceneExplorer)
        self.menuView.addAction(self.actionPreferences)
        self.menuView.addAction(self.actionProperty_Window)
        self.menuView.addAction(self.actionObject_Selection)
        self.menuView.addAction(self.actionConsole_Window)
        self.menubar.addAction(self.menuFile.menuAction())
        self.menubar.addAction(self.menuView.menuAction())

        self.retranslateUi()
        QtCore.QMetaObject.connectSlotsByName(self)

    def retranslateUi(self):
        self.setWindowTitle(QtGui.QApplication.translate("MainWindow", "MainWindow", None, QtGui.QApplication.UnicodeUTF8))
        self.menuFile.setTitle(QtGui.QApplication.translate("MainWindow", "File", None, QtGui.QApplication.UnicodeUTF8))
        self.menuView.setTitle(QtGui.QApplication.translate("MainWindow", "View", None, QtGui.QApplication.UnicodeUTF8))
        self.actionNeu.setText(QtGui.QApplication.translate("MainWindow", "New", None, QtGui.QApplication.UnicodeUTF8))
        self.actionMove.setText(QtGui.QApplication.translate("MainWindow", "Move", None, QtGui.QApplication.UnicodeUTF8))
        self.actionRotate.setText(QtGui.QApplication.translate("MainWindow", "Rotate", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSceneExplorer.setText(QtGui.QApplication.translate("MainWindow", "Scene Explorer", None, QtGui.QApplication.UnicodeUTF8))
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

        self.OgreMainWinSceneMgr = self.ogreRoot.createSceneManager(og.ST_GENERIC, "OgreMainWinSceneMgr")
        self.OgreMainWinSceneMgr.ambientLight = og.ColourValue(4, 4, 4)

        self.moduleName = ""
        self.moduleManager = ModuleManager(self.ogreRoot,  self.OgreMainWinSceneMgr)

        self.ogreRenderWindow = OgreMainWindow.OgreMainWindow(self.moduleManager,  root,  self.OgreMainWinSceneMgr,  self)
        self.gridlayout.addWidget(self.ogreRenderWindow,0,0,1,1)
        self.hboxlayout.addLayout(self.gridlayout)
        self.setCentralWidget(self.centralwidget)

        og.ResourceGroupManager.getSingleton().addResourceLocation("./media", "FileSystem", "General", False)
        og.ResourceGroupManager.getSingleton().initialiseAllResourceGroups()

        oglog = og.LogManager.getSingleton().getDefaultLog()
        oglog.addListener(self.consoleWindow.lockenLog)

    def update(self):
#        try:
#            self.OgreMainWinSceneMgr.getSceneNode("saeule_076_node").yaw(0.02)
#        except Exception,  e:
#            pass

        self.ogreRoot.renderOneFrame()

    def actionMoveSlot(self):
        self.moduleManager.pivot.setMoveMode()
        return

    def actionRotateSlot(self):
        self.moduleManager.pivot.setRotateMode()
        return

    def actionScaleSlot(self):
        self.moduleManager.pivot.setScaleMode()
        return

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

    def toggleSceneExplorer(self):
        if self.sceneExplorerDock.isHidden():
            self.sceneExplorerDock.show()
        else:
            self.sceneExplorerDock.hide()

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

    # parses the moduleconfig.rb and searches for all loader.loadmap(" statements in the ruby script
    # and parses them too
    def loadModule(self, modulePath, moduleName):
        if self.moduleName == moduleName:
            return

        self.moduleName = moduleName
        self.workingDir = modulePath + moduleName # the module path
        self.workingDirCommon = modulePath + "common" # the module path of the common module

        import codecs
        import glob

        modConfig = self.workingDir + "/scripts/moduleconfig.rb"
        if isfile(modConfig): # is the modconfig existing?
            f = codecs.open(modConfig, 'r', 'utf-8')
        else:
            print ("Error: couldn't find module config")
            pass

        self.mapFiles = [] # a list in case the module has more than one map file

        for line in f:
            lStripped = line.strip() #strip the whitespace away, not needed here

            # get the map file(s)
            if lStripped.startswith("loader.loadMap("): # all loadMap commands begin with loader.loadMap(
                spl = lStripped.split('"')
                for a in spl:
                    if a.endswith(".xml"):
                        pathToMapFile = self.workingDir + "/maps/" + a
                        self.mapFiles.append(pathToMapFile)

        self.setWindowTitle(moduleName)

        self.modelSelectionDialog.scanDirForModels(self.workingDir, moduleName)
        self.modelSelectionDialog.scanDirForModels(self.workingDirCommon, "common")

        self.setResourcePaths(self.workingDir, moduleName)
        self.setResourcePaths(self.workingDirCommon, "common")
        og.ResourceGroupManager.getSingleton().initialiseAllResourceGroups()

        command = (os.path.join(self.workingDir,  "maps") + "/*.xml")
        for mf in glob.glob(command): # search for all xml files in the maps directory and add them
            self.mapFiles.append(mf)

        command = (os.path.join(self.workingDir,  "maps") + "/*.scene")
        for mf in glob.glob(command): # search for all xml files in the maps directory and add them
            self.mapFiles.append(mf)

        self.moduleManager.load(moduleName,  self.mapFiles)

    def setResourcePaths(self, path, moduleName):
        for file in os.listdir(path):
            curFile = path + "/" + file

            if file.startswith('.'): #ignore dot files (hidden)
                continue
            if isdir(curFile):
                og.ResourceGroupManager.getSingleton().addResourceLocation(curFile, "FileSystem", moduleName, False)
                self.setResourcePaths(curFile, moduleName)
                continue
            if isfile(curFile):
                pass

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

        self.sceneExplorerDock = QtGui.QDockWidget(self.tr("Scene Explorer"), self)
        self.sceneExplorerDock.setObjectName("SceneExplorerDockWindow")
        self.sceneExplorerDock.setAllowedAreas(QtCore.Qt.LeftDockWidgetArea | QtCore.Qt.RightDockWidgetArea)
        self.sceneExplorerDock.setWidget(self.sceneExplorerWin)
        self.addDockWidget(QtCore.Qt.LeftDockWidgetArea, self.sceneExplorerDock)

        self.consoleDock = QtGui.QDockWidget(self.tr("Console"), self)
        self.consoleDock.setObjectName("ConsoleDockWindow")
        self.consoleDock.setAllowedAreas(QtCore.Qt.BottomDockWidgetArea | QtCore.Qt.TopDockWidgetArea)
        self.consoleDock.setWidget(self.consoleWindow)
        self.addDockWidget(QtCore.Qt.BottomDockWidgetArea, self.consoleDock)

        self.fileToolBar = self.addToolBar("File Toolbar")
        self.fileToolBar.setObjectName("FileToolBar")
        self.fileToolBar.setAllowedAreas(QtCore.Qt.TopToolBarArea | QtCore.Qt.BottomToolBarArea)
        self.fileToolBar.addAction(self.actionNeu)
        self.fileToolBar.addAction(self.actionClose)
        self.addToolBar(QtCore.Qt.TopToolBarArea, self.fileToolBar)

        self.moveToolBar = self.addToolBar("Transformation Bar")
        self.moveToolBar.setObjectName("TransformationBar")
        self.moveToolBar.setAllowedAreas(QtCore.Qt.TopToolBarArea | QtCore.Qt.BottomToolBarArea)
        self.moveToolBar.addAction(self.actionMove)
        self.moveToolBar.addAction(self.actionRotate)
        self.moveToolBar.addAction(self.actionScale)
        self.addToolBar(QtCore.Qt.TopToolBarArea, self.moveToolBar)

    def keyPressEvent(self,  event):
        if not event.isAutoRepeat():
            self.ogreRenderWindow.keyPressEvent(event)

    def keyReleaseEvent(self,  event):
        if not event.isAutoRepeat():
            self.ogreRenderWindow.keyReleaseEvent(event)
        pass

    def connectActionButtons(self):
        pass

    def saveOnClose(self):
        reply = QtGui.QMessageBox.question(self,  "Rastullahs Lockenwickler - Unsaved Chages",  "Save unsaved changes?",  QtGui.QMessageBox.Yes|QtGui.QMessageBox.No|QtGui.QMessageBox.Cancel)
        if reply == QtGui.QMessageBox.Cancel:
            return False
        if reply == QtGui.QMessageBox.Yes:
            print""
            #TODO: implement save here
        return True

    def closeEvent(self,  event):
        if self.saveOnClose():
            settings = QtCore.QSettings()
            settings.setValue("MainWindow/Geometry",  QtCore.QVariant(self.saveGeometry()))
            settings.setValue("MainWIndow/DockWindows",  QtCore.QVariant(self.saveState()))
        else:
            event.ignore()

if __name__ == "__main__":
    app = QtGui.QApplication(sys.argv)
    app.setOrganizationName("Team Pantheon")
    app.setOrganizationDomain("rastullahs-lockenpracht.de/team")
    app.setApplicationName("Lockenwickler")

    form = Lockenwickler()
    form.show()

    sys.exit(app.exec_())

