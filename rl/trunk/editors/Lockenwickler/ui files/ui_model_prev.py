# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'model_preview.ui'
#
# Created: Mon May 12 15:09:24 2008
#      by: PyQt4 UI code generator 4.3.3
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_modelPreviewDialog(object):
    def setupUi(self, modelPreviewDialog):
        modelPreviewDialog.setObjectName("modelPreviewDialog")
        modelPreviewDialog.resize(QtCore.QSize(QtCore.QRect(0,0,272,744).size()).expandedTo(modelPreviewDialog.minimumSizeHint()))

        self.gridlayout = QtGui.QGridLayout(modelPreviewDialog)
        self.gridlayout.setObjectName("gridlayout")

        self.modelSearchBox = QtGui.QLineEdit(modelPreviewDialog)
        self.modelSearchBox.setObjectName("modelSearchBox")
        self.gridlayout.addWidget(self.modelSearchBox,0,0,1,1)

        self.splitter = QtGui.QSplitter(modelPreviewDialog)
        self.splitter.setOrientation(QtCore.Qt.Vertical)
        self.splitter.setObjectName("splitter")

        self.listWidget = QtGui.QListWidget(self.splitter)
        self.listWidget.setObjectName("listWidget")

        self.modelPreviewWindow = QtGui.QWidget(self.splitter)
        self.modelPreviewWindow.setMinimumSize(QtCore.QSize(200,200))
        self.modelPreviewWindow.setObjectName("modelPreviewWindow")
        self.gridlayout.addWidget(self.splitter,1,0,1,1)

        self.retranslateUi(modelPreviewDialog)
        QtCore.QObject.connect(self.modelSearchBox,QtCore.SIGNAL("textChanged(QString)"),self.listWidget.clearSelection)
        QtCore.QMetaObject.connectSlotsByName(modelPreviewDialog)

    def retranslateUi(self, modelPreviewDialog):
        modelPreviewDialog.setWindowTitle(QtGui.QApplication.translate("modelPreviewDialog", "Dialog", None, QtGui.QApplication.UnicodeUTF8))

