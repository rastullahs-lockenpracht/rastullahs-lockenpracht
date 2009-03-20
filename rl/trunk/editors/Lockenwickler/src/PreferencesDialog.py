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

        self.setLayout(layout)
        
        self.moduleCfgPath = None
        
        QObject.connect(self.modulConfigSelector, SIGNAL("clicked()"),
                               self.openModulConfigSelector)

        self.setModal(True)

    def setCfgPath(self, inPath):
        path = str(inPath)

        from os.path import isfile

        if isfile(path):
            self.moduleCfgPath = path
            self.modulePath = self.moduleCfgPath.replace("modules.cfg", "")
            self.lineEdit.setText(self.moduleCfgPath)
            return True
        else:
            return False

    def openModulConfigSelector(self):
        dialog = QFileDialog(self)
        self.moduleCfgPath = str(dialog.getOpenFileName(self, "Select modules.cfg", "/home/melven/rastullah/rl_dist/modules/", "modules.cfg (*.cfg)"))
        self.modulePath = self.moduleCfgPath.replace("modules.cfg", "")

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

