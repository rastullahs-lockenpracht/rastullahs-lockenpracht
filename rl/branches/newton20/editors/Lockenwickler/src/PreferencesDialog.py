#################################################
 # Copyright (C) 2008  Stefan Stammberger
 #
 # This library is free software; you can redistribute it and/or
 # modify it under the terms of the GNU Lesser General Public
 # License as published by the Free Software Foundation; either
 # version 2.1 of the License, or (at your option) any later version.
 #
 # This library is distributed in the hope that it will be useful,
 # but WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 # Lesser General Public License for more details.
 #
 # You should have received a copy of the GNU Lesser General Public
 # License along with this library; if not, write to the Free Software
 # Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 #################################################


import sys
import platform
import string
from ui_pref_dialog import *

class PreferencesDialog(QtGui.QDialog):
    def __init__(self, loadModuleCallback, parent=None):
        QtGui.QDialog.__init__(self, parent)

        self.loadModuleCallback = loadModuleCallback

        self.prefDialog = Ui_PreferencesDialog()
        self.prefDialog.setupUi(self)

        QtCore.QObject.connect(self.prefDialog.modulConfigSelector, QtCore.SIGNAL("clicked()"),
                               self.openModulConfigSelector)

        QtCore.QObject.connect(self.prefDialog.loadModuleBtn, QtCore.SIGNAL("clicked()"),
                               self.onLoadSelectedModule)

        QtCore.QObject.connect(self.prefDialog.listWidget, QtCore.SIGNAL("itemDoubleClicked(QListWidgetItem *)"),
                               self.onLoadSelectedModule)

        if platform.system() == "Windows" or platform.system() == "MAC":
            pass
            # TODO: delete this when ready
#            self.prefDialog.lineEdit.setText("C:\Users\Stefan\Desktop\Lockenwickler\modules\modules.cfg")
#            self.moduleCfgPath = "C:\Users\Stefan\Desktop\Lockenwickler\modules\modules.cfg"
#            self.moduleCfgIsSelected = True
#            self.modulePath = self.moduleCfgPath.replace("modules.cfg", "")
#            self.readInModules()
            # end delete
        else:
            # TODO: delete this when ready
            self.prefDialog.lineEdit.setText("/home/stefan/blubb/rl_modules/modules/modules.cfg")
            self.moduleCfgPath = "/home/stefan/blubb/rl_modules/modules/modules.cfg"
            self.moduleCfgIsSelected = True
            self.modulePath = self.moduleCfgPath.replace("modules.cfg", "")
            self.readInModules()
            # end delete

        self.moduleCfgIsSelected = False

        self.setModal(True)

    def openModulConfigSelector(self):
        dialog = QtGui.QFileDialog(self)
        self.moduleCfgPath = str(dialog.getOpenFileName(self, "Select modules.cfg", "/home/stefan/blubb/rl_modules/modules/", "modules.cfg (*.cfg)"))
        self.modulePath = self.moduleCfgPath.replace("modules.cfg", "")

        from os.path import isfile

        if isfile(self.moduleCfgPath):
            self.prefDialog.lineEdit.setText(self.moduleCfgPath)
            self.moduleCfgIsSelected = True
            self.readInModules()

    def readInModules(self):
        if self.moduleCfgIsSelected:
            import codecs
            f = codecs.open(self.moduleCfgPath, 'r', 'utf-8')

            for line in f:
                if line.startswith('#'):
                    continue

                if line.startswith('module='):
                    splines = line.split('=')
                    str = splines[1].rstrip().rstrip()
                    if str != "common":
                        self.prefDialog.listWidget.addItem(str)

    def onLoadSelectedModule(self):
        item = self.prefDialog.listWidget.currentItem()

        if item == None:
            mbox = QtGui.QMessageBox(self)
            mbox.setModal(True)
            mbox.setText("Please select a module.")
            mbox.show()
        else:
            tempo = str(item.text())
            self.loadModuleCallback(self.modulePath, tempo)
            self.hide()



