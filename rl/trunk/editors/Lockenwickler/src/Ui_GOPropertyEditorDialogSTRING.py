# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/home/stefan/Lockenwickler/ui files/GOPropertyEditorDialogSTRING.ui'
#
# Created: Mon Oct 27 20:13:11 2008
#      by: PyQt4 UI code generator 4.4.3
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_PropertyEditDialog(object):
    def setupUi(self, PropertyEditDialog):
        PropertyEditDialog.setObjectName("PropertyEditDialog")
        PropertyEditDialog.setWindowModality(QtCore.Qt.ApplicationModal)
        PropertyEditDialog.resize(419, 124)
        self.gridLayout = QtGui.QGridLayout(PropertyEditDialog)
        self.gridLayout.setObjectName("gridLayout")
        self.label = QtGui.QLabel(PropertyEditDialog)
        self.label.setObjectName("label")
        self.gridLayout.addWidget(self.label, 0, 0, 1, 1)
        self.label_2 = QtGui.QLabel(PropertyEditDialog)
        self.label_2.setObjectName("label_2")
        self.gridLayout.addWidget(self.label_2, 0, 1, 1, 1)
        self.label_3 = QtGui.QLabel(PropertyEditDialog)
        self.label_3.setObjectName("label_3")
        self.gridLayout.addWidget(self.label_3, 0, 2, 1, 1)
        self.nameEdit = QtGui.QLineEdit(PropertyEditDialog)
        self.nameEdit.setMinimumSize(QtCore.QSize(120, 0))
        self.nameEdit.setMaximumSize(QtCore.QSize(250, 16777215))
        self.nameEdit.setObjectName("nameEdit")
        self.gridLayout.addWidget(self.nameEdit, 1, 0, 1, 1)
        self.comboBox = QtGui.QComboBox(PropertyEditDialog)
        self.comboBox.setObjectName("comboBox")
        self.comboBox.addItem(QtCore.QString())
        self.comboBox.addItem(QtCore.QString())
        self.comboBox.addItem(QtCore.QString())
        self.comboBox.addItem(QtCore.QString())
        self.comboBox.addItem(QtCore.QString())
        self.comboBox.addItem(QtCore.QString())
        self.comboBox.addItem(QtCore.QString())
        self.comboBox.addItem(QtCore.QString())
        self.gridLayout.addWidget(self.comboBox, 1, 1, 1, 1)
        self.dataEdit = QtGui.QTextEdit(PropertyEditDialog)
        self.dataEdit.setObjectName("dataEdit")
        self.gridLayout.addWidget(self.dataEdit, 1, 2, 1, 1)
        self.buttonBox = QtGui.QDialogButtonBox(PropertyEditDialog)
        self.buttonBox.setOrientation(QtCore.Qt.Horizontal)
        self.buttonBox.setStandardButtons(QtGui.QDialogButtonBox.Cancel|QtGui.QDialogButtonBox.Ok)
        self.buttonBox.setObjectName("buttonBox")
        self.gridLayout.addWidget(self.buttonBox, 2, 0, 1, 3)
        self.label.setBuddy(self.nameEdit)
        self.label_2.setBuddy(self.comboBox)
        self.label_3.setBuddy(self.dataEdit)

        self.retranslateUi(PropertyEditDialog)
        QtCore.QObject.connect(self.buttonBox, QtCore.SIGNAL("accepted()"), PropertyEditDialog.accept)
        QtCore.QObject.connect(self.buttonBox, QtCore.SIGNAL("rejected()"), PropertyEditDialog.reject)
        QtCore.QMetaObject.connectSlotsByName(PropertyEditDialog)

    def retranslateUi(self, PropertyEditDialog):
        PropertyEditDialog.setWindowTitle(QtGui.QApplication.translate("PropertyEditDialog", "Dialog", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("PropertyEditDialog", "&Name", None, QtGui.QApplication.UnicodeUTF8))
        self.label_2.setText(QtGui.QApplication.translate("PropertyEditDialog", "&Type", None, QtGui.QApplication.UnicodeUTF8))
        self.label_3.setText(QtGui.QApplication.translate("PropertyEditDialog", "&Data", None, QtGui.QApplication.UnicodeUTF8))
        self.comboBox.setItemText(0, QtGui.QApplication.translate("PropertyEditDialog", "STRING", None, QtGui.QApplication.UnicodeUTF8))
        self.comboBox.setItemText(1, QtGui.QApplication.translate("PropertyEditDialog", "BOOL", None, QtGui.QApplication.UnicodeUTF8))
        self.comboBox.setItemText(2, QtGui.QApplication.translate("PropertyEditDialog", "INT", None, QtGui.QApplication.UnicodeUTF8))
        self.comboBox.setItemText(3, QtGui.QApplication.translate("PropertyEditDialog", "INTPAIR", None, QtGui.QApplication.UnicodeUTF8))
        self.comboBox.setItemText(4, QtGui.QApplication.translate("PropertyEditDialog", "INTTRIPPLE", None, QtGui.QApplication.UnicodeUTF8))
        self.comboBox.setItemText(5, QtGui.QApplication.translate("PropertyEditDialog", "REAL", None, QtGui.QApplication.UnicodeUTF8))
        self.comboBox.setItemText(6, QtGui.QApplication.translate("PropertyEditDialog", "MAP", None, QtGui.QApplication.UnicodeUTF8))
        self.comboBox.setItemText(7, QtGui.QApplication.translate("PropertyEditDialog", "ARRAY", None, QtGui.QApplication.UnicodeUTF8))


if __name__ == "__main__":
    import sys
    app = QtGui.QApplication(sys.argv)
    PropertyEditDialog = QtGui.QDialog()
    ui = Ui_PropertyEditDialog()
    ui.setupUi(PropertyEditDialog)
    PropertyEditDialog.show()
    sys.exit(app.exec_())

