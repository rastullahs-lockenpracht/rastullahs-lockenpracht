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
    def __init__(self, sceneManager, gocManager, parent=None):
        super(QDialog, self).__init__(parent)
        self.setupUi()
        self.sceneManager = sceneManager
        self.valueBeforeEdit = None
        self.gocManager = gocManager
        
        self.connect(self.treeWidget, SIGNAL("itemDoubleClicked (QTreeWidgetItem *,int)"),
                               self.onItemClicked)
        self.connect(self.treeWidget, SIGNAL("itemChanged (QTreeWidgetItem *,int)"),
                               self.onItemChanged)

    def setupUi(self):
        self.setObjectName("ObjectPropertys")
        self.resize(QSize(QRect(0,0,761,724).size()).expandedTo(self.minimumSizeHint()))

        self.treeWidget = QTreeWidget(self)
        self.treeWidget.setGeometry(QRect(0,0,901,721))
        self.treeWidget.setMinimumSize(QSize(400,400))
        self.treeWidget.setObjectName("treeWidget")
        self.treeWidget.setAlternatingRowColors(True)
        self.treeWidget.setAnimated(True)
        
        self.retranslateUi()
        QMetaObject.connectSlotsByName(self)

    def retranslateUi(self):
        self.treeWidget.headerItem().setText(0,QApplication.translate("ObjectPropertys", "Property", None, QApplication.UnicodeUTF8))
        self.treeWidget.headerItem().setText(1,QApplication.translate("ObjectPropertys", "Value", None, QApplication.UnicodeUTF8))
        self.treeWidget.clear()


    def onItemClicked(self, item, column):
        if column == 1:
            self.valueBeforeEdit = item.text(column)
            
            if item.text(0) == "State":
                rect = self.treeWidget.visualItemRect(item)
                
                combo = QComboBox(self.treeWidget)
                combo.setGeometry(QCursor.pos().x(), QCursor.pos().y(), 100, 30)
                combo.addItem("IN_SCENE")
                combo.addItem("BLUBB")
                combo.show()
            else:
                self.treeWidget.editItem(item, column)

    def onItemChanged(self, item, column):
        if self.valueBeforeEdit is not None and self.valueBeforeEdit != item.text(column):
            parent = item.parent()
            if parent is None: # handle properties that don't expand here (pos, rot, scale etc)
                if item.text(0) == "Name":
                    if not self.sceneManager.hasEntity(str(item.text(column))):
                        newEnt = self.node.getAttachedObject(0).clone(str(item.text(column)))
                        oldEnt = self.node.getAttachedObject(0)
                        self.node.detachObject(oldEnt)
                        self.sceneManager.destroyEntity(oldEnt)
                        self.node.attachObject(newEnt)
                        
                        # update the selection object since it still has references to the deleted entity
                        self.so.entity = newEnt
                        self.so.entityName = newEnt.getName()
                    else:
                        item.setText(column, self.valueBeforeEdit)
                        print "Error: Name already exists!"
                elif item.text(0) == "GameObject Id":
                    if not self.gocManager.inWorldIdExists(str(item.text(column))):
                        val = None
                        try:
                            val = int(item.text(1))
                        except ValueError, e:
                            item.setText(column, self.valueBeforeEdit)
                            print "ValueError: " + str(e)
                            return
                            
                        self.node.getAttachedObject(0).getUserObject().inWorldId = str(val)
                    else:
                        item.setText(column, self.valueBeforeEdit)
                        print "Error: Id exists already!"
                elif item.text(0) == "State":
                    rect = self.treeWidget.visualItemRect()
                    combo = QComboBox(self.treeWidget)
                    combo.setGeometry(rect)
                    
            else:
                val = None
                try:
                    val = float(item.text(1))
                except ValueError, e:
                    item.setText(column, self.valueBeforeEdit)
                    print "ValueError: " + str(e)
                    return
                    
                if parent.text(0) == "Position":
                    if item.text(0) == "X":
                        self.node.setPosition(val, self.node.getPosition().y, self.node.getPosition().z)
                    elif item.text(0) == "Y":
                        self.node.setPosition(self.node.getPosition().x, val, self.node.getPosition().z)
                    elif item.text(0) == "Z":
                        self.node.setPosition(self.node.getPosition().x, self.node.getPosition().y, val)
                elif parent.text(0) == "Orientation":
                    if item.text(0) == "W":
                        self.node.setOrientation(val, self.node.getOrientation().x, self.node.getOrientation().y, self.node.getOrientation().z)
                    elif item.text(0) == "X":
                        self.node.setOrientation(self.node.getOrientation().w, val, self.node.getOrientation().y, self.node.getOrientation().z)
                    elif item.text(0) == "Y":
                        self.node.setOrientation(self.node.getOrientation().w, self.node.getOrientation().x, val, self.node.getOrientation().z)
                    elif item.text(0) == "Z":
                        self.node.setOrientation(self.node.getOrientation().w, self.node.getOrientation().x, self.node.getOrientation().y, val)
                elif parent.text(0) == "Scale":
                    if item.text(0) == "X":
                        self.node.setScale(val, self.node.getScale().y, self.node.getScale().z)
                    elif item.text(0) == "Y":
                        self.node.setScale(self.node.getScale().x, val, self.node.getScale().z)
                    elif item.text(0) == "Z":
                        self.node.setScale(self.node.getScale().x, self.node.getScale().y, val)
                        
    def showProperties(self, so):
        # onItemChanged should only be called when the user changes values not when they change by code
        self.disconnect(self.treeWidget, SIGNAL("itemChanged (QTreeWidgetItem *,int)"),
                       self.onItemChanged)
                       
        self.so=so
        self.treeWidget.clear()
        
        name = so.entity.getParentNode().getName()
        self.node = so.entity.getParentNode()
        
        if name.startswith("entity_"):
            item = QTreeWidgetItem(self.treeWidget)
            item.setText(0, "Name")
            n = self.node.getAttachedObject(0).getName()
            item.setText(1, n)
            item.setFlags(item.flags() | Qt.ItemIsEditable)
            
            itemMat = QTreeWidgetItem(self.treeWidget)
            itemMat.setText(0, "Materials")
            
            i = 0
            while i < self.node.getAttachedObject(0).getNumSubEntities():
                it = QTreeWidgetItem(itemMat)
                it.setFlags(item.flags() | Qt.ItemIsEditable)
                it.setText(0, "SubMesh" + str(i))
                it.setText(1, self.node.getAttachedObject(0).getSubEntity(i).getMaterialName())
                i += 1
                
            
            #item.setFlags(item.flags() | Qt.ItemIsEditable)
            
            self.parsePosition(self.node)
            self.parseOrientation(self.node)
            self.parseScale(self.node)
            
        elif name.startswith("gameobject_"):
            item = QTreeWidgetItem(self.treeWidget)
            item.setText(0, "ClassName")
            n = str(self.node.getAttachedObject(0).getUserObject().gocName)
            item.setText(1, n)
            item.setFlags(item.flags() | Qt.ItemIsEditable)
            
            item = QTreeWidgetItem(self.treeWidget)
            item.setText(0, "GameObject Id")
            n = str(self.node.getAttachedObject(0).getUserObject().inWorldId)
            item.setText(1, n)
            item.setFlags(item.flags() | Qt.ItemIsEditable)
            
            item = QTreeWidgetItem(self.treeWidget)
            item.setText(0, "State")
            n = str(self.node.getAttachedObject(0).getUserObject().state)
            item.setText(1, n)
            item.setFlags(item.flags() | Qt.ItemIsEditable)
            
            
            self.parsePosition(self.node)
            self.parseOrientation(self.node)
            self.parseScale(self.node)
            
        self.connect(self.treeWidget, SIGNAL("itemChanged (QTreeWidgetItem *,int)"),
                       self.onItemChanged)
                       
                       
    def parsePosition(self, node):
        item = QTreeWidgetItem(self.treeWidget)
        item.setText(0, "Position")
        item.setText(1, str(node.getPosition().x) + "; " + str(node.getPosition().y) + "; " + str(node.getPosition().z))
        
        itemX = QTreeWidgetItem(item)
        itemX.setText(0, "X")
        itemX.setText(1, str(node.getPosition().x))
        itemX.setFlags(itemX.flags() | Qt.ItemIsEditable)
        
        itemY = QTreeWidgetItem(item)
        itemY.setText(0, "Y")
        itemY.setText(1, str(node.getPosition().y))
        itemY.setFlags(itemY.flags() | Qt.ItemIsEditable)
        
        itemZ = QTreeWidgetItem(item)
        itemZ.setText(0, "Z")
        itemZ.setText(1, str(node.getPosition().z))
        itemZ.setFlags(itemZ.flags() | Qt.ItemIsEditable)
        
    def parseOrientation(self, node):
        item = QTreeWidgetItem(self.treeWidget)
        item.setText(0, "Orientation")
        item.setText(1, str(node.getOrientation().w) + "; " + str(node.getOrientation().x) + "; " + str(node.getOrientation().y) + "; " + str(node.getOrientation().z))
        
        itemW = QTreeWidgetItem(item)
        itemW.setText(0, "W")
        itemW.setText(1, str(node.getOrientation().w))
        itemW.setFlags(itemW.flags() | Qt.ItemIsEditable)
        
        itemX = QTreeWidgetItem(item)
        itemX.setText(0, "X")
        itemX.setText(1, str(node.getOrientation().x))
        itemX.setFlags(itemX.flags() | Qt.ItemIsEditable)
        
        itemY = QTreeWidgetItem(item)
        itemY.setText(0, "Y")
        itemY.setText(1, str(node.getOrientation().y))
        itemY.setFlags(itemY.flags() | Qt.ItemIsEditable)
        
        itemZ = QTreeWidgetItem(item)
        itemZ.setText(0, "Z")
        itemZ.setText(1, str(node.getOrientation().z))
        itemZ.setFlags(itemZ.flags() | Qt.ItemIsEditable)
        
    def parseScale(self, node):
        item = QTreeWidgetItem(self.treeWidget)
        item.setText(0, "Scale")
        item.setText(1, str(node.getScale().x) + "; " + str(node.getScale().y) + "; " + str(node.getScale().z))
        
        itemX = QTreeWidgetItem(item)
        itemX.setText(0, "X")
        itemX.setText(1, str(node.getScale().x))
        itemX.setFlags(itemX.flags() | Qt.ItemIsEditable)
        
        itemY = QTreeWidgetItem(item)
        itemY.setText(0, "Y")
        itemY.setText(1, str(node.getScale().y))
        itemY.setFlags(itemY.flags() | Qt.ItemIsEditable)
        
        itemZ = QTreeWidgetItem(item)
        itemZ.setText(0, "Z")
        itemZ.setText(1, str(node.getScale().z))
        itemZ.setFlags(itemZ.flags() | Qt.ItemIsEditable)
        
    def updateProperties(self):
        if self.so is not None:
            self.showProperties(self.so)
            
