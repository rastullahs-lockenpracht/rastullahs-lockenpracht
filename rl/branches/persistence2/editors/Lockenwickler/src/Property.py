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

from PyQt4.QtCore import *
from PyQt4.QtGui import *

class Vector3Property():
    def __init__(self):
        pass

    def __parseValues(self):
        self.vector3 = self.so.entity.getParentNode().getPosition()
        self.__createTree()

    def __createTree(self):
        item = QTreeWidgetItem(self.treeWidget)
        item.setText(0,QApplication.translate("ObjectPropertys", "Position", None, QApplication.UnicodeUTF8))
        item.setText(1, str(self.vector3.x) + " , " + str(self.vector3.y) +" , " + str(self.vector3.z))

        item1 = QTreeWidgetItem(item)
        item1.setText(0,QApplication.translate("ObjectPropertys", "X", None, QApplication.UnicodeUTF8))
        item1.setText(1, str(self.vector3.x))

        item2 = QTreeWidgetItem(item)
        item2.setText(0,QApplication.translate("ObjectPropertys", "Y", None, QApplication.UnicodeUTF8))
        item2.setText(1, str(self.vector3.y))

        item3 = QTreeWidgetItem(item)
        item3.setText(0,QApplication.translate("ObjectPropertys", "Z", None, QApplication.UnicodeUTF8))
        item3.setText(1, str(self.vector3.z))

