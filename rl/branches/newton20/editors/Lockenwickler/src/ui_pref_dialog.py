# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'preferences_dialog.ui'
#
# Created: Mon Jun 23 19:28:46 2008
#      by: PyQt4 UI code generator 4.3.3
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_PreferencesDialog(object):
    def setupUi(self, PreferencesDialog):
        PreferencesDialog.setObjectName("PreferencesDialog")
        PreferencesDialog.resize(QtCore.QSize(QtCore.QRect(0,0,528,626).size()).expandedTo(PreferencesDialog.minimumSizeHint()))

        self.gridlayout = QtGui.QGridLayout(PreferencesDialog)
        self.gridlayout.setObjectName("gridlayout")

        self.toolBox = QtGui.QToolBox(PreferencesDialog)
        self.toolBox.setObjectName("toolBox")

        self.ResourcePaths = QtGui.QWidget()
        self.ResourcePaths.setGeometry(QtCore.QRect(0,0,510,505))
        self.ResourcePaths.setObjectName("ResourcePaths")

        self.gridlayout1 = QtGui.QGridLayout(self.ResourcePaths)
        self.gridlayout1.setObjectName("gridlayout1")

        self.listWidget = QtGui.QListWidget(self.ResourcePaths)
        self.listWidget.setObjectName("listWidget")
        self.gridlayout1.addWidget(self.listWidget,0,0,1,1)

        self.groupBox = QtGui.QGroupBox(self.ResourcePaths)
        self.groupBox.setMinimumSize(QtCore.QSize(0,95))
        self.groupBox.setObjectName("groupBox")

        self.layoutWidget = QtGui.QWidget(self.groupBox)
        self.layoutWidget.setGeometry(QtCore.QRect(10,20,471,67))
        self.layoutWidget.setObjectName("layoutWidget")

        self.gridlayout2 = QtGui.QGridLayout(self.layoutWidget)
        self.gridlayout2.setObjectName("gridlayout2")

        self.lineEdit = QtGui.QLineEdit(self.layoutWidget)
        self.lineEdit.setObjectName("lineEdit")
        self.gridlayout2.addWidget(self.lineEdit,0,0,1,1)

        self.loadModuleBtn = QtGui.QPushButton(self.layoutWidget)
        self.loadModuleBtn.setObjectName("loadModuleBtn")
        self.gridlayout2.addWidget(self.loadModuleBtn,1,0,1,2)

        self.modulConfigSelector = QtGui.QPushButton(self.layoutWidget)
        self.modulConfigSelector.setObjectName("modulConfigSelector")
        self.gridlayout2.addWidget(self.modulConfigSelector,0,1,1,1)
        self.gridlayout1.addWidget(self.groupBox,1,0,1,1)
        self.toolBox.addItem(self.ResourcePaths,"")

        self.OgreSettings = QtGui.QWidget()
        self.OgreSettings.setGeometry(QtCore.QRect(0,0,96,26))
        self.OgreSettings.setObjectName("OgreSettings")
        self.toolBox.addItem(self.OgreSettings,"")
        self.gridlayout.addWidget(self.toolBox,0,0,1,1)

        self.buttonBox = QtGui.QDialogButtonBox(PreferencesDialog)
        self.buttonBox.setOrientation(QtCore.Qt.Horizontal)
        self.buttonBox.setStandardButtons(QtGui.QDialogButtonBox.Cancel|QtGui.QDialogButtonBox.NoButton|QtGui.QDialogButtonBox.Ok)
        self.buttonBox.setObjectName("buttonBox")
        self.gridlayout.addWidget(self.buttonBox,1,0,1,1)

        self.retranslateUi(PreferencesDialog)
        self.toolBox.setCurrentIndex(0)
        QtCore.QObject.connect(self.buttonBox,QtCore.SIGNAL("accepted()"),PreferencesDialog.accept)
        QtCore.QObject.connect(self.buttonBox,QtCore.SIGNAL("rejected()"),PreferencesDialog.reject)
        QtCore.QObject.connect(self.buttonBox,QtCore.SIGNAL("clicked(QAbstractButton*)"),PreferencesDialog.close)
        QtCore.QMetaObject.connectSlotsByName(PreferencesDialog)

    def retranslateUi(self, PreferencesDialog):
        PreferencesDialog.setWindowTitle(QtGui.QApplication.translate("PreferencesDialog", "Dialog", None, QtGui.QApplication.UnicodeUTF8))
        self.groupBox.setTitle(QtGui.QApplication.translate("PreferencesDialog", "GroupBox", None, QtGui.QApplication.UnicodeUTF8))
        self.loadModuleBtn.setText(QtGui.QApplication.translate("PreferencesDialog", "Load Selected Module", None, QtGui.QApplication.UnicodeUTF8))
        self.modulConfigSelector.setText(QtGui.QApplication.translate("PreferencesDialog", "...", None, QtGui.QApplication.UnicodeUTF8))
        self.toolBox.setItemText(self.toolBox.indexOf(self.ResourcePaths), QtGui.QApplication.translate("PreferencesDialog", "Resource Paths", None, QtGui.QApplication.UnicodeUTF8))
        self.toolBox.setItemText(self.toolBox.indexOf(self.OgreSettings), QtGui.QApplication.translate("PreferencesDialog", "Ogre Settings", None, QtGui.QApplication.UnicodeUTF8))

