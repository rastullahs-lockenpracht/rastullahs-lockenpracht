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


from PyQt4.QtCore import *
from PyQt4.QtGui import *

from GameObjectClass import *

# The drag events are processed in ObgreMainWindow.py

class GoTreeWidget(QTreeWidget):
    def __init__(self, parent = None):
        super(GoTreeWidget, self).__init__(parent)
        self.setDragEnabled(True)
        self.setAnimated(True)
        self.setAlternatingRowColors(True)
        
    def startDrag(self,  dropActions):
        data = QByteArray()
        stream = QDataStream(data,  QIODevice.WriteOnly)
        stream << self.currentItem().text(0)
        mimeData = QMimeData()
        mimeData.setData("application/x-game_object", data)
        drag = QDrag(self)
        drag.setMimeData(mimeData)
        drag.start(Qt.CopyAction)

class GameObjectClassView(QWidget):
    def __init__(self, gocManager, parent = None):
        super(GameObjectClassView, self).__init__(parent)
        self.gocManager = gocManager
        self.gocManager.setGameObjectsViewUpdateCallback(self.updateObjectList)

        self.gameObjectDict = {}

        self.setupUi()

    def addItemToDict(self, property, item):
        self.gameObjectDict[item] = property

    def setupUi(self):
        self.setObjectName("gameObjectClassView")
        self.resize(QSize(QRect(0,0,272,744).size()).expandedTo(self.minimumSizeHint()))

        self.gridlayout = QGridLayout(self)
        self.gridlayout.setObjectName("gridlayout")

        self.treeWidget = GoTreeWidget()
        self.treeWidget.setColumnCount(3)
        self.treeWidget.setObjectName("ObjectTreeView")

        self.gridlayout.addWidget(self.treeWidget , 1, 0, 1, 1)

        self.setWindowTitle(QApplication.translate("objectPreviewDialog", "Dialog", None, QApplication.UnicodeUTF8))
        self.connect(self.treeWidget, SIGNAL("itemDoubleClicked (QTreeWidgetItem *,int)"), self.onDoubleClick)
        QMetaObject.connectSlotsByName(self)

    def onDoubleClick(self, item,  row):
        if self.gameObjectDict[item].getType() == "MAP" or self.gameObjectDict[item].getType() == "ARRAY"or self.gameObjectDict[item].getType() == "GAME_OBJECT":
            print "NOT YET :)"
        else:
            if self.gameObjectDict[item].openEditor(row, self):
                item.setText(0, str(self.gameObjectDict[item].name))
                item.setText(2, str(self.gameObjectDict[item].data))



    def createPropertyMapItem(self, property, parentItem):
        for subProperty in property.childProperties:
            if subProperty is not None:
                if subProperty.getType() == "MAP":
                    item = QTreeWidgetItem(parentItem)
                    self.addItemToDict(subProperty, item)
                    item.setText(0, subProperty.name)
                    item.setText(1, subProperty.getType())
                    self.createPropertyMapItem(property, item)
                else:
                    item = QTreeWidgetItem(parentItem)
                    self.addItemToDict(subProperty, item)
                    item.setText(0, subProperty.name)
                    item.setText(1, subProperty.getType())
                    if not subProperty.getType() == "ARRAY":
                        item.setText(2, subProperty.data)


    def updateObjectList(self,  objectsDict):
        for key in objectsDict:
            file = objectsDict[key]
            fileItem = QTreeWidgetItem(self.treeWidget)
            fileItem.setText(0, str(key))
            fileItem.setIcon(0, QIcon("media/icons/filenew.png"))

            for go in file:
                goItem = QTreeWidgetItem(fileItem)
                self.addItemToDict(go, goItem)
                goItem.setIcon(0, QIcon("media/icons/agt_games.png"))
                goItem.setText(0, str(go.classid))
                goItem.setText(1, str(go.baseclass))

                for prop in go.properties:
                    item1 = QTreeWidgetItem(goItem) #the property item, needs to be created regardless the type
                    self.addItemToDict(prop, item1)
                    item1.setIcon(0, QIcon("media/icons/agt_multimedia.png"))
                    item1.setText(0, str(prop.name))
                    item1.setText(1, prop.getType())
                    if prop.getType() is "MAP":
                        self.createPropertyMapItem(prop, item1)
                    elif prop.getType() is "ARRAY":
                        pass
                    else:
                        item1.setText(2, unicode(prop.data))


