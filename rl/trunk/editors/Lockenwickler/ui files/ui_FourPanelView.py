# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'FourPanelView.ui'
#
# Created: Mon May 12 15:09:24 2008
#      by: PyQt4 UI code generator 4.3.3
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_FourPaneView(object):
    def setupUi(self, FourPaneView):
        FourPaneView.setObjectName("FourPaneView")
        FourPaneView.resize(QtCore.QSize(QtCore.QRect(0,0,827,856).size()).expandedTo(FourPaneView.minimumSizeHint()))

        self.gridlayout = QtGui.QGridLayout(FourPaneView)
        self.gridlayout.setObjectName("gridlayout")

        self.splitter_3 = QtGui.QSplitter(FourPaneView)
        self.splitter_3.setOrientation(QtCore.Qt.Horizontal)
        self.splitter_3.setObjectName("splitter_3")

        self.splitter = QtGui.QSplitter(self.splitter_3)
        self.splitter.setOrientation(QtCore.Qt.Vertical)
        self.splitter.setObjectName("splitter")

        self.widget = QtGui.QWidget(self.splitter)
        self.widget.setObjectName("widget")

        self.widget_4 = QtGui.QWidget(self.splitter)
        self.widget_4.setObjectName("widget_4")

        self.splitter_2 = QtGui.QSplitter(self.splitter_3)
        self.splitter_2.setOrientation(QtCore.Qt.Vertical)
        self.splitter_2.setObjectName("splitter_2")

        self.widget_2 = QtGui.QWidget(self.splitter_2)
        self.widget_2.setObjectName("widget_2")

        self.widget_3 = QtGui.QWidget(self.splitter_2)
        self.widget_3.setObjectName("widget_3")
        self.gridlayout.addWidget(self.splitter_3,0,0,1,1)

        self.retranslateUi(FourPaneView)
        QtCore.QMetaObject.connectSlotsByName(FourPaneView)

    def retranslateUi(self, FourPaneView):
        FourPaneView.setWindowTitle(QtGui.QApplication.translate("FourPaneView", "Form", None, QtGui.QApplication.UnicodeUTF8))

