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

from PyQt4.QtCore import *
from PyQt4.QtGui import *

from Property import *

class ObjectPropertyWin(QDialog):
    def __init__(self, parent=None):
        super(QDialog, self).__init__(parent)
        self.setupUi()

        self.connect(self.treeWidget, SIGNAL("itemDoubleClicked (QTreeWidgetItem *,int)"),
                               self.onItemClicked)

    def setupUi(self):
        self.setObjectName("ObjectPropertys")
        self.resize(QSize(QRect(0,0,761,724).size()).expandedTo(self.minimumSizeHint()))

        self.treeWidget = QTreeWidget(self)
        self.treeWidget.setGeometry(QRect(0,0,901,721))
        self.treeWidget.setMinimumSize(QSize(400,400))
        self.treeWidget.setObjectName("treeWidget")

        self.retranslateUi()
        QMetaObject.connectSlotsByName(self)

    def retranslateUi(self):
        self.treeWidget.headerItem().setText(0,QApplication.translate("ObjectPropertys", "Property", None, QApplication.UnicodeUTF8))
        self.treeWidget.headerItem().setText(1,QApplication.translate("ObjectPropertys", "Value", None, QApplication.UnicodeUTF8))
        self.treeWidget.clear()


    def onItemClicked(self, item, column):
        pass
