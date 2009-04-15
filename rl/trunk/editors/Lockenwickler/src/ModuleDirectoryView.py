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
from PyQt4.QtCore import *
from PyQt4.QtGui import *


class ModuleDirectoryView(QWidget):        
    def __init__(self, parent = None):
        super(ModuleDirectoryView, self).__init__(parent)
        
        self.model = QDirModel()
        self.model.setReadOnly(False)
        
        self.tree = QTreeView()
        self.tree.setModel(self.model)
        
        self.tree.setWindowTitle("Module Dir View")
        layout = QVBoxLayout()
        layout.setContentsMargins(0, 0, 0, 0)
        layout.addWidget(self.tree)
        self.setLayout(layout)
        
    def parseDirectory(self, dir):
        self.tree.setRootIndex(self.model.index(dir));

