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


import OgreMainWindow
import ogre.renderer.OGRE as og

class Lockenwickler(QtGui.QMainWindow):
    def __init__(self, parent=None):
        QtGui.QWidget.__init__(self, parent)

        self.setupUi()

        self.consoleWindow = ConsoleWindow(False,  self)
        QtCore.QObject.connect(self.actionConsole_Window, QtCore.SIGNAL("triggered()"),
                                    self.toggleConsoleWindow)

        self.setupOgre()

        self.prefDialog = PreferencesDialog(self.loadModule, self)
        QtCore.QObject.connect(self.actionPreferences, QtCore.SIGNAL("triggered()"),
                                    self.togglePreferencesWindow)

        self.objectPropertyWin = ObjectPropertyWin(self)
        QtCore.QObject.connect(self.actionProperty_Window, QtCore.SIGNAL("triggered()"),
                                    self.togglePropertyWindow)

        self.modelSelectionDialog = ModelSelectionDialog(self.ogreRoot, self)
        QtCore.QObject.connect(self.actionObject_Selection, QtCore.SIGNAL("triggered()"),
                                    self.toggleModelPreviewWindow)

        QtCore.QObject.connect(self.actionClose, QtCore.SIGNAL("triggered()"),
                                    self.update)

        self.createDockWindows()

        self.mainTimer = QtCore.QTimer(self)
        self.mainTimer.connect(self.mainTimer, QtCore.SIGNAL("timeout()"), self.update)
        self.mainTimer.start(33)

        QtGui.QApplication.setKeyboardInputInterval(5000)

    def setupUi(self):
        self.setObjectName("MainWindow")
        self.resize(QtCore.QSize(QtCore.QRect(0,0,660,657).size()).expandedTo(self.minimumSizeHint()))

        self.centralwidget = QtGui.QWidget(self)
        self.centralwidget.setObjectName("centralwidget")

        self.hboxlayout = QtGui.QHBoxLayout(self.centralwidget)
        self.hboxlayout.setObjectName("hboxlayout")

        self.gridlayout = QtGui.QGridLayout()
        self.gridlayout.setObjectName("gridlayout")

        self.menubar = QtGui.QMenuBar(self)
        self.menubar.setGeometry(QtCore.QRect(0,0,660,22))
        self.menubar.setObjectName("menubar")

        self.menuFile = QtGui.QMenu(self.menubar)
        self.menuFile.setObjectName("menuFile")

        self.menuView = QtGui.QMenu(self.menubar)
        self.menuView.setObjectName("menuView")
        self.setMenuBar(self.menubar)

        self.statusbar = QtGui.QStatusBar(self)
        self.statusbar.setObjectName("statusbar")
        self.setStatusBar(self.statusbar)

        self.actionNeu = QtGui.QAction(self)
        self.actionNeu.setObjectName("actionNeu")

        self.actionSceneGraph = QtGui.QAction(self)
        self.actionSceneGraph.setCheckable(False)
        self.actionSceneGraph.setObjectName("actionSceneGraph")

        self.actionPreferences = QtGui.QAction(self)
        self.actionPreferences.setCheckable(False)
        self.actionPreferences.setObjectName("actionPreferences")

        self.actionProperty_Window = QtGui.QAction(self)
        self.actionProperty_Window.setObjectName("actionProperty_Window")

        self.actionObject_Selection = QtGui.QAction(self)
        self.actionObject_Selection.setObjectName("actionObject_Selection")

        self.actionClose = QtGui.QAction(self)
        self.actionClose.setObjectName("actionClose")

        self.actionConsole_Window = QtGui.QAction(self)
        self.actionConsole_Window.setObjectName("actionConsole_Window")
        self.menuFile.addAction(self.actionNeu)
        self.menuFile.addAction(self.actionClose)
        self.menuView.addAction(self.actionSceneGraph)
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
        self.actionNeu.setText(QtGui.QApplication.translate("MainWindow", "Neu", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSceneGraph.setText(QtGui.QApplication.translate("MainWindow", "SceneGraph", None, QtGui.QApplication.UnicodeUTF8))
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

        # setup resources
        config = og.ConfigFile()

        carryOn = root.showConfigDialog()
        if not carryOn:
            sys.exit('Quit from Config Dialog')

        root.initialise(False)

        self.OgreMainWinSceneMgr = self.ogreRoot.createSceneManager(og.ST_GENERIC, "OgreMainWinSceneMgr")
        self.OgreMainWinSceneMgr.ambientLight = og.ColourValue(4, 4, 4)

        self.moduleName = ""
        self.moduleManager = ModuleManager(self.ogreRoot,  self.OgreMainWinSceneMgr)

        self.ogreRenderWindow = OgreMainWindow.OgreMainWindow(self.moduleManager,  root,  self.OgreMainWinSceneMgr, self)
        self.gridlayout.addWidget(self.ogreRenderWindow,0,0,1,1)
        self.hboxlayout.addLayout(self.gridlayout)
        self.setCentralWidget(self.centralwidget)

        og.ResourceGroupManager.getSingleton().addResourceLocation("./media", "FileSystem", "General", False)
        og.ResourceGroupManager.getSingleton().initialiseAllResourceGroups()

        oglog = og.LogManager.getSingleton().getDefaultLog()
        oglog.addListener(self.consoleWindow.lockenLog)

    def update(self):
        self.ogreRoot.renderOneFrame()

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

    def togglePropertyWindow(self):
        if self.propertyDock.isHidden():
            self.propertyDock.show()
        else:
            self.propertyDock.hide()

    def toggleConsoleWindow(self):
        if self.consoleWindow.isHidden():
            self.consoleWindow.show()
        else:
            self.consoleWindow.hide()

    # parses the moduleconfig.rb and searches for all loader.loadmap(" statements in the ruby script
    # and parses them too
    def loadModule(self, modulePath, moduleName):
        if self.moduleName == moduleName:
            return

        self.moduleName = moduleName
        self.workingDir = modulePath + moduleName # the module path
        self.workingDirCommon = modulePath + "common" # the module path of the common module

        import codecs

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
        self.propertyDock.setAllowedAreas(QtCore.Qt.LeftDockWidgetArea | QtCore.Qt.RightDockWidgetArea)
        self.propertyDock.setWidget(self.objectPropertyWin)
        self.addDockWidget(QtCore.Qt.LeftDockWidgetArea, self.propertyDock)

        self.modelSelectionDock = QtGui.QDockWidget(self.tr("Models"), self)
        self.modelSelectionDock.setAllowedAreas(QtCore.Qt.LeftDockWidgetArea | QtCore.Qt.RightDockWidgetArea)
        self.modelSelectionDock.setWidget(self.modelSelectionDialog)
        self.addDockWidget(QtCore.Qt.RightDockWidgetArea, self.modelSelectionDock)

        self.consoleDock = QtGui.QDockWidget(self.tr("Console"), self)
        self.consoleDock.setAllowedAreas(QtCore.Qt.BottomDockWidgetArea | QtCore.Qt.TopDockWidgetArea)
        self.consoleDock.setWidget(self.consoleWindow)
        self.addDockWidget(QtCore.Qt.BottomDockWidgetArea, self.consoleDock)

        self.fileToolBar = self.addToolBar("File Toolbar")
        self.fileToolBar.setAllowedAreas(QtCore.Qt.TopToolBarArea | QtCore.Qt.BottomToolBarArea)
        self.fileToolBar.addAction(self.actionNeu)
        self.fileToolBar.addAction(self.actionClose)
        self.fileToolBar.hide()
        self.addToolBar(QtCore.Qt.TopToolBarArea, self.fileToolBar)

    def keyPressEvent(self,  event):
        if not event.isAutoRepeat():
            self.ogreRenderWindow.keyPressEvent(event)

    def keyReleaseEvent(self,  event):
        if not event.isAutoRepeat():
            self.ogreRenderWindow.keyReleaseEvent(event)
        pass

if __name__ == "__main__":
     app = QtGui.QApplication(sys.argv)
     myapp = Lockenwickler()
     myapp.show()

     sys.exit(app.exec_())

