# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'console_window.ui'
#
# Created: Mon May 12 15:09:24 2008
#      by: PyQt4 UI code generator 4.3.3
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_ConsoleWindow(object):
    def setupUi(self, ConsoleWindow):
        ConsoleWindow.setObjectName("ConsoleWindow")
        ConsoleWindow.resize(QtCore.QSize(QtCore.QRect(0,0,884,225).size()).expandedTo(ConsoleWindow.minimumSizeHint()))

        self.gridlayout = QtGui.QGridLayout(ConsoleWindow)
        self.gridlayout.setObjectName("gridlayout")

        self.textEdit = QtGui.QTextEdit(ConsoleWindow)
        self.textEdit.setObjectName("textEdit")
        self.gridlayout.addWidget(self.textEdit,0,0,1,1)

        self.retranslateUi(ConsoleWindow)
        QtCore.QMetaObject.connectSlotsByName(ConsoleWindow)

    def retranslateUi(self, ConsoleWindow):
        ConsoleWindow.setWindowTitle(QtGui.QApplication.translate("ConsoleWindow", "Form", None, QtGui.QApplication.UnicodeUTF8))

