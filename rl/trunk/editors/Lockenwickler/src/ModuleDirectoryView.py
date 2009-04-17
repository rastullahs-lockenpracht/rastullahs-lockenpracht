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
import os
import subprocess

                
from PyQt4.QtCore import *
from PyQt4.QtGui import *

from PreferencesDialog import *

class ModuleDirectoryView(QWidget):        
    def __init__(self, parent = None):
        super(ModuleDirectoryView, self).__init__(parent)
        self.modulesPath = ""

        self.model = QDirModel()

        self.model.setFilter(QDir.Files | QDir.Dirs | QDir.System | QDir.NoDotAndDotDot | QDir.Drives)

        self.tree = QTreeView()
        
        self.tree.setContextMenuPolicy(Qt.CustomContextMenu)        
        self.connect(self.tree, SIGNAL("customContextMenuRequested(const QPoint &)"), self.doMenu)
        
        self.tree.setModel(self.model)
        self.tree.setWindowTitle("Module Dir View")
        layout = QVBoxLayout()
        layout.setContentsMargins(0, 0, 0, 0)
        layout.addWidget(self.tree)
        self.setLayout(layout)
        
    def doMenu(self, point):
        menu = QMenu(self)
        #item = self.tree.itemAt(point)
            
        action= self.createAction("Delete", self.onDelete, None, "editdelete.png")
        menu.addAction(action)
        
        menu.exec_(QCursor.pos())
        #subprocess.Popen(PreferencesDialog.externalTextAppCmd)

    def onDelete(self):
        print "delete"
        
    def getFilters(self, dir, list):
        for f in os.listdir(dir):
            if f.startswith("."):
                continue
            list.append(f)
            if os.path.isdir(os.path.join(dir, f)):
                self.getFilters(os.path.join(dir, f), list)
        
    def parseDirectory(self, dirs):
#        nameFilters = []
#        for dir in dirs:
#            self.getFilters(dir, nameFilters)

        self.tree.setRootIndex(self.model.index(self.modulesPath))

    

        
    def createAction(self, text, slot=None, shortcut=None, icon=None, tip=None, checkable=False, signal="triggered()"):
        action = QAction(text, self)
        if icon is not None:
            action.setIcon(QIcon("media/icons/%s" % icon))
        if shortcut is not None:
            action.setShortcut(shortcut)
        if tip is not None:
            action.setToolTip(tip)
            action.setStatusTip(tip)
        if slot is not None:
            self.connect(action, SIGNAL(signal), slot)

        action.setCheckable(checkable)

        return action
        

        
        
        
        
        
        
        
        
        
        
        
