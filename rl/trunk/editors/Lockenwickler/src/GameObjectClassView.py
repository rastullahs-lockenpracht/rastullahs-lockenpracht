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

from GameObjectClass import *

class GameObjectClassView(QWidget):
    def __init__(self, gocManager, parent = None):
        super(GameObjectClassView, self).__init__(parent)
        self.gocManager = gocManager
        self.gocManager.setGameObjectsViewUpdateCallback(self.updateObjectList)

        self.setupUi()

    def setupUi(self):
        self.setObjectName("gameObjectClassView")
        self.resize(QSize(QRect(0,0,272,744).size()).expandedTo(self.minimumSizeHint()))

        self.gridlayout = QGridLayout(self)
        self.gridlayout.setObjectName("gridlayout")

        self.treeWidget = QTreeWidget()
        self.treeWidget.setColumnCount(3)
        self.treeWidget.setObjectName("ObjectTreeView")

        self.gridlayout.addWidget(self.treeWidget , 1, 0, 1, 1)

        self.setWindowTitle(QApplication.translate("objectPreviewDialog", "Dialog", None, QApplication.UnicodeUTF8))

        QMetaObject.connectSlotsByName(self)

    def updateObjectList(self,  objectsDict):
        changeColor = True
        for key in objectsDict:
            file = objectsDict[key]
            fileItem = QTreeWidgetItem(self.treeWidget)
            fileItem.setText(0, str(key))
            fileItem.setIcon(0, QIcon("media/icons/filenew.png"))

            for go in file:
                goItem = QTreeWidgetItem(fileItem)
                goItem.setIcon(0, QIcon("media/icons/agt_games.png"))
                if changeColor:
                    goItem.setBackgroundColor(0, QColor("lightGray"))
                    goItem.setBackgroundColor(1, QColor("lightGray"))
                    goItem.setBackgroundColor(2, QColor("lightGray"))
                goItem.setText(0, str(go.classid))
                goItem.setText(1, str(go.baseclass))
                changeColor = not changeColor

                for prop in go.properties:
                    item1 = QTreeWidgetItem(goItem)
                    if changeColor:
                        item1.setBackgroundColor(0, QColor("lightGray"))
                        item1.setBackgroundColor(1, QColor("lightGray"))
                        item1.setBackgroundColor(2, QColor("lightGray"))
                    item1.setIcon(0, QIcon("media/icons/agt_multimedia.png"))
                    item1.setText(0, str(prop.name))
                    item1.setText(1, prop.getType())
                    if prop.getType() is "MAP":
                        pass
                    elif prop.getType() is "ARRAY":
                        pass
                    else:
                        item1.setText(2, unicode(prop.data))

                    changeColor = not changeColor

