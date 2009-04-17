 #################################################
# This source file is part of Rastullahs Lockenwickler.
# Copyright (C) 2003-2009 Team Pantheon. http://www.team-pantheon.de
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  US
 #################################################


import sys
import platform
import string

from PyQt4.QtCore import *
from PyQt4.QtGui import *

class PreferencesDialog(QDialog):
    pathToModuleConfig = None
    externalTextAppCmd = None
    
    def __init__(self, parent=None):
        QDialog.__init__(self, parent)

        self.label = QLabel()
        self.label.setText("Please select the modules.cfg file.")
        self.modulConfigSelector = QPushButton()
        self.modulConfigSelector.setText("...")
        self.lineEdit = QLineEdit()

        layout = QGridLayout()
        layout.addWidget(self.label, 0, 0, 1, 2)
        layout.addWidget(self.lineEdit, 1, 0)
        layout.addWidget(self.modulConfigSelector, 1, 1)
        QObject.connect(self.modulConfigSelector, SIGNAL("clicked()"),
                               self.openModulConfigSelector)

        self.externalTextApplabel = QLabel()
        self.externalTextApplabel.setText("Please select the Texteditor to use.")
        self.externalTextAppSelector = QPushButton()
        self.externalTextAppSelector.setText("...")
        self.externalTextAppLineEdit = QLineEdit()

        layout.addWidget(self.externalTextApplabel, 2, 0, 1, 2)
        layout.addWidget(self.externalTextAppLineEdit, 3, 0)
        layout.addWidget(self.externalTextAppSelector, 3, 1)
        QObject.connect(self.externalTextAppSelector, SIGNAL("clicked()"),
                               self.openExternalTextEditorSelector)

    

        self.setLayout(layout)
        self.setMinimumSize(600, 100)
        self.moduleCfgPath = None
        self.externalEditorPath = None


        self.setModal(True)

        buttonBox = QDialogButtonBox(QDialogButtonBox.Ok)
        layout.addWidget(buttonBox)
        self.connect(buttonBox, SIGNAL("accepted()"), self.onOk)       
        
    def onOk(self):
        PreferencesDialog.externalTextAppCmd = str(self.externalTextAppLineEdit.text())
        self.accept()
        
        
    def setCfgPath(self, inPath):
        path = str(inPath)

        from os.path import isfile

        if isfile(path):
            PreferencesDialog.pathToModuleConfig = path
            self.moduleCfgPath = path
            self.modulePath = self.moduleCfgPath.replace("modules.cfg", "")
            self.lineEdit.setText(self.moduleCfgPath)
            return True
        else:
            return False

    def openExternalTextEditorSelector(self):
        dialog = QFileDialog(self)
        self.externalEditorPath = str(dialog.getOpenFileName(self, "Select the executable", "/home/melven/rastullah/rl_dist/modules/"))

        from os.path import isfile

        if isfile(self.externalEditorPath):
            self.externalTextAppLineEdit.setText(self.externalEditorPath)
            PreferencesDialog.externalTextAppCmd = str(self.externalTextAppLineEdit.text())
            
    def openModulConfigSelector(self):
        dialog = QFileDialog(self)
        self.moduleCfgPath = str(dialog.getOpenFileName(self, "Select modules.cfg", "/home/melven/rastullah/rl_dist/modules/", "modules.cfg (*.cfg)"))
        self.modulePath = self.moduleCfgPath.replace("modules.cfg", "")
        PreferencesDialog.pathToModuleConfig = self.moduleCfgPath
        
        from os.path import isfile

        if isfile(self.moduleCfgPath):
            self.lineEdit.setText(self.moduleCfgPath)


    def onLoadSelectedModule(self):
        item = self.prefDialog.listWidget.currentItem()

        if item == None:
            mbox = QMessageBox(self)
            mbox.setModal(True)
            mbox.setText("Please select a module.")
            mbox.show()
        else:
            tempo = str(item.text())
            self.loadModuleCallback(self.modulePath, tempo)
            self.hide()

    def getModuleConfigPath(self):
        return self.lineEdit

