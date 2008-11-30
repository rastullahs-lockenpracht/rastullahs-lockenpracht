# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/home/stefan/Lockenwickler/ui files/NewModuleWizard.ui'
#
# Created: Sun Nov  9 14:15:16 2008
#      by: PyQt4 UI code generator 4.4.3
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_Dialog(object):
    def setupUi(self, Dialog):
        Dialog.setObjectName("Dialog")
        Dialog.resize(675, 703)
        self.gridLayout_3 = QtGui.QGridLayout(Dialog)
        self.gridLayout_3.setObjectName("gridLayout_3")
        self.label = QtGui.QLabel(Dialog)
        self.label.setObjectName("label")
        self.gridLayout_3.addWidget(self.label, 0, 0, 1, 1)
        self.moduleNameLineEdit = QtGui.QLineEdit(Dialog)
        self.moduleNameLineEdit.setObjectName("moduleNameLineEdit")
        self.gridLayout_3.addWidget(self.moduleNameLineEdit, 0, 1, 1, 1)
        self.label_2 = QtGui.QLabel(Dialog)
        self.label_2.setObjectName("label_2")
        self.gridLayout_3.addWidget(self.label_2, 1, 0, 1, 1)
        self.sceneNameLineEdit = QtGui.QLineEdit(Dialog)
        self.sceneNameLineEdit.setEnabled(False)
        self.sceneNameLineEdit.setObjectName("sceneNameLineEdit")
        self.gridLayout_3.addWidget(self.sceneNameLineEdit, 1, 1, 1, 1)
        self.label_3 = QtGui.QLabel(Dialog)
        self.label_3.setObjectName("label_3")
        self.gridLayout_3.addWidget(self.label_3, 2, 0, 1, 1)
        self.mapNameLineEdit = QtGui.QLineEdit(Dialog)
        self.mapNameLineEdit.setEnabled(False)
        self.mapNameLineEdit.setObjectName("mapNameLineEdit")
        self.gridLayout_3.addWidget(self.mapNameLineEdit, 2, 1, 1, 1)
        self.groupBox = QtGui.QGroupBox(Dialog)
        self.groupBox.setAlignment(QtCore.Qt.AlignLeading|QtCore.Qt.AlignLeft|QtCore.Qt.AlignVCenter)
        self.groupBox.setObjectName("groupBox")
        self.gridLayout_2 = QtGui.QGridLayout(self.groupBox)
        self.gridLayout_2.setObjectName("gridLayout_2")
        self.moduleDependenciesList = QtGui.QListView(self.groupBox)
        self.moduleDependenciesList.setEnabled(False)
        self.moduleDependenciesList.setSelectionMode(QtGui.QAbstractItemView.MultiSelection)
        self.moduleDependenciesList.setObjectName("moduleDependenciesList")
        self.gridLayout_2.addWidget(self.moduleDependenciesList, 1, 0, 1, 1)
        self.gridLayout_3.addWidget(self.groupBox, 3, 0, 1, 2)
        self.groupBox_2 = QtGui.QGroupBox(Dialog)
        self.groupBox_2.setObjectName("groupBox_2")
        self.gridLayout = QtGui.QGridLayout(self.groupBox_2)
        self.gridLayout.setObjectName("gridLayout")
        self.scriptDependenciesList = QtGui.QListView(self.groupBox_2)
        self.scriptDependenciesList.setEnabled(False)
        self.scriptDependenciesList.setSelectionMode(QtGui.QAbstractItemView.MultiSelection)
        self.scriptDependenciesList.setObjectName("scriptDependenciesList")
        self.gridLayout.addWidget(self.scriptDependenciesList, 0, 0, 1, 1)
        self.gridLayout_3.addWidget(self.groupBox_2, 4, 0, 1, 2)
        self.hboxlayout = QtGui.QHBoxLayout()
        self.hboxlayout.setSpacing(6)
        self.hboxlayout.setMargin(0)
        self.hboxlayout.setObjectName("hboxlayout")
        spacerItem = QtGui.QSpacerItem(131, 31, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.hboxlayout.addItem(spacerItem)
        self.okButton = QtGui.QPushButton(Dialog)
        self.okButton.setEnabled(False)
        self.okButton.setObjectName("okButton")
        self.hboxlayout.addWidget(self.okButton)
        self.cancelButton = QtGui.QPushButton(Dialog)
        self.cancelButton.setObjectName("cancelButton")
        self.hboxlayout.addWidget(self.cancelButton)
        self.gridLayout_3.addLayout(self.hboxlayout, 5, 0, 1, 2)

        self.retranslateUi(Dialog)
        QtCore.QObject.connect(self.okButton, QtCore.SIGNAL("clicked()"), Dialog.accept)
        QtCore.QObject.connect(self.cancelButton, QtCore.SIGNAL("clicked()"), Dialog.reject)
        QtCore.QMetaObject.connectSlotsByName(Dialog)

    def retranslateUi(self, Dialog):
        Dialog.setWindowTitle(QtGui.QApplication.translate("Dialog", "Dialog", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("Dialog", "Step 1: Module Name", None, QtGui.QApplication.UnicodeUTF8))
        self.label_2.setText(QtGui.QApplication.translate("Dialog", "Step 2: Scene Name", None, QtGui.QApplication.UnicodeUTF8))
        self.label_3.setText(QtGui.QApplication.translate("Dialog", "Step 3: Map Name", None, QtGui.QApplication.UnicodeUTF8))
        self.groupBox.setTitle(QtGui.QApplication.translate("Dialog", "Step 4: Module Dependencies", None, QtGui.QApplication.UnicodeUTF8))
        self.moduleDependenciesList.setToolTip(QtGui.QApplication.translate("Dialog", "Modules the new module will depend on", None, QtGui.QApplication.UnicodeUTF8))
        self.groupBox_2.setTitle(QtGui.QApplication.translate("Dialog", "Step 5: Script Dependencies", None, QtGui.QApplication.UnicodeUTF8))
        self.scriptDependenciesList.setToolTip(QtGui.QApplication.translate("Dialog", "Modules the new module will depend on", None, QtGui.QApplication.UnicodeUTF8))
        self.okButton.setText(QtGui.QApplication.translate("Dialog", "&OK", None, QtGui.QApplication.UnicodeUTF8))
        self.cancelButton.setText(QtGui.QApplication.translate("Dialog", "&Cancel", None, QtGui.QApplication.UnicodeUTF8))


if __name__ == "__main__":
    import sys
    app = QtGui.QApplication(sys.argv)
    Dialog = QtGui.QDialog()
    ui = Ui_Dialog()
    ui.setupUi(Dialog)
    Dialog.show()
    sys.exit(app.exec_())

