# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'object_property.ui'
#
# Created: Mon May 12 15:09:23 2008
#      by: PyQt4 UI code generator 4.3.3
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_ObjectPropertys(object):
    def setupUi(self, ObjectPropertys):
        ObjectPropertys.setObjectName("ObjectPropertys")
        ObjectPropertys.resize(QtCore.QSize(QtCore.QRect(0,0,761,724).size()).expandedTo(ObjectPropertys.minimumSizeHint()))

        self.treeWidget = QtGui.QTreeWidget(ObjectPropertys)
        self.treeWidget.setGeometry(QtCore.QRect(0,0,901,721))
        self.treeWidget.setMinimumSize(QtCore.QSize(400,400))
        self.treeWidget.setObjectName("treeWidget")

        self.retranslateUi(ObjectPropertys)
        QtCore.QMetaObject.connectSlotsByName(ObjectPropertys)

    def retranslateUi(self, ObjectPropertys):
        ObjectPropertys.setWindowTitle(QtGui.QApplication.translate("ObjectPropertys", "Object Properties", None, QtGui.QApplication.UnicodeUTF8))
        self.treeWidget.headerItem().setText(0,QtGui.QApplication.translate("ObjectPropertys", "Property", None, QtGui.QApplication.UnicodeUTF8))
        self.treeWidget.headerItem().setText(1,QtGui.QApplication.translate("ObjectPropertys", "Value", None, QtGui.QApplication.UnicodeUTF8))
        self.treeWidget.clear()

        item = QtGui.QTreeWidgetItem(self.treeWidget)
        item.setText(0,QtGui.QApplication.translate("ObjectPropertys", "Position", None, QtGui.QApplication.UnicodeUTF8))
        item.setText(1,"")

        item1 = QtGui.QTreeWidgetItem(item)
        item1.setText(0,QtGui.QApplication.translate("ObjectPropertys", "X", None, QtGui.QApplication.UnicodeUTF8))
        item1.setText(1,"")

        item2 = QtGui.QTreeWidgetItem(item)
        item2.setText(0,QtGui.QApplication.translate("ObjectPropertys", "Y", None, QtGui.QApplication.UnicodeUTF8))
        item2.setText(1,"")

        item3 = QtGui.QTreeWidgetItem(item)
        item3.setText(0,QtGui.QApplication.translate("ObjectPropertys", "Z", None, QtGui.QApplication.UnicodeUTF8))
        item3.setText(1,"")

