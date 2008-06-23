# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'lockewickler.ui'
#
# Created: Mon May 12 15:09:23 2008
#      by: PyQt4 UI code generator 4.3.3
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(QtCore.QSize(QtCore.QRect(0,0,660,657).size()).expandedTo(MainWindow.minimumSizeHint()))

        self.centralwidget = QtGui.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")

        self.hboxlayout = QtGui.QHBoxLayout(self.centralwidget)
        self.hboxlayout.setObjectName("hboxlayout")

        self.gridlayout = QtGui.QGridLayout()
        self.gridlayout.setObjectName("gridlayout")

        self.ogreRenderWindow = QtGui.QWidget(self.centralwidget)
        self.ogreRenderWindow.setMinimumSize(QtCore.QSize(250,250))
        self.ogreRenderWindow.setObjectName("ogreRenderWindow")
        self.gridlayout.addWidget(self.ogreRenderWindow,0,0,1,1)
        self.hboxlayout.addLayout(self.gridlayout)
        MainWindow.setCentralWidget(self.centralwidget)

        self.menubar = QtGui.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0,0,660,22))
        self.menubar.setObjectName("menubar")

        self.menuFile = QtGui.QMenu(self.menubar)
        self.menuFile.setObjectName("menuFile")

        self.menuView = QtGui.QMenu(self.menubar)
        self.menuView.setObjectName("menuView")
        MainWindow.setMenuBar(self.menubar)

        self.statusbar = QtGui.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.actionNeu = QtGui.QAction(MainWindow)
        self.actionNeu.setObjectName("actionNeu")

        self.actionSceneGraph = QtGui.QAction(MainWindow)
        self.actionSceneGraph.setCheckable(False)
        self.actionSceneGraph.setObjectName("actionSceneGraph")

        self.actionPreferences = QtGui.QAction(MainWindow)
        self.actionPreferences.setCheckable(False)
        self.actionPreferences.setObjectName("actionPreferences")

        self.actionProperty_Window = QtGui.QAction(MainWindow)
        self.actionProperty_Window.setObjectName("actionProperty_Window")

        self.actionObject_Selection = QtGui.QAction(MainWindow)
        self.actionObject_Selection.setObjectName("actionObject_Selection")

        self.actionClose = QtGui.QAction(MainWindow)
        self.actionClose.setObjectName("actionClose")

        self.actionConsole_Window = QtGui.QAction(MainWindow)
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

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QtGui.QApplication.translate("MainWindow", "MainWindow", None, QtGui.QApplication.UnicodeUTF8))
        self.menuFile.setTitle(QtGui.QApplication.translate("MainWindow", "File", None, QtGui.QApplication.UnicodeUTF8))
        self.menuView.setTitle(QtGui.QApplication.translate("MainWindow", "View", None, QtGui.QApplication.UnicodeUTF8))
        self.actionNeu.setText(QtGui.QApplication.translate("MainWindow", "Neu", None, QtGui.QApplication.UnicodeUTF8))
        self.actionSceneGraph.setText(QtGui.QApplication.translate("MainWindow", "SceneGraph", None, QtGui.QApplication.UnicodeUTF8))
        self.actionPreferences.setText(QtGui.QApplication.translate("MainWindow", "Preferences", None, QtGui.QApplication.UnicodeUTF8))
        self.actionProperty_Window.setText(QtGui.QApplication.translate("MainWindow", "Property Window", None, QtGui.QApplication.UnicodeUTF8))
        self.actionObject_Selection.setText(QtGui.QApplication.translate("MainWindow", "Object Selection", None, QtGui.QApplication.UnicodeUTF8))
        self.actionClose.setText(QtGui.QApplication.translate("MainWindow", "Quit", None, QtGui.QApplication.UnicodeUTF8))
        self.actionConsole_Window.setText(QtGui.QApplication.translate("MainWindow", "Console Window", None, QtGui.QApplication.UnicodeUTF8))

