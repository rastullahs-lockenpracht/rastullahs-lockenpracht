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

#from elementtree.ElementTree import *
from xml.etree.cElementTree import *

from PyQt4.QtCore import *
from PyQt4.QtGui import *

import ogre.renderer.OGRE as og

from GOStringEditor import *
from GOIntEditor import *
from GOGenericEditor import *

class GOCStringProperty():
    def __init__(self, name, data):
        self.name = name
        self.data = data

    def getType(self):
        return "STRING"

    def openEditor(self, row, parent = None):
        dlg = GOStringEditor(parent)
        dlg.nameEdit.setText(self.name)
        dlg.dataEdit.setText(self.data)
        result = dlg.exec_()
        if result:
            self.name = dlg.nameEdit.text()
            self.data = dlg.dataEdit.toPlainText()

        return result

class GOCRealProperty():
    def __init__(self, name, data):
        self.name = name
        self.data = data

    def getType(self):
        return "REAL"

    def openEditor(self, row, parent = None):
        dlg = GOStringEditor(parent)
        dlg.nameEdit.setText(self.name)
        dlg.dataEdit.setText(self.data)
        result = dlg.exec_()
        if result:
            self.name = dlg.nameEdit.text()
            self.data = dlg.dataEdit.toPlainText()

        return result

class GOCBoolProperty():
    def __init__(self, name, data):
        self.name = name
        self.data = data

    def getType(self):
        return "BOOL"

class GOCIntProperty():
    def __init__(self, name, data):
        self.name = name
        self.data = data

    def getType(self):
        return "INT"

    def openEditor(self, row, parent = None):
        dlg = GOIntEditor(parent)
        dlg.nameEdit.setText(self.name)
        dlg.dataEdit.setText(self.data)
        result = dlg.exec_()
        if result:
            self.name = dlg.nameEdit.text()
            self.data = dlg.dataEdit.toPlainText()

        return result

class GOCIntPairProperty():
    def __init__(self, name, data):
        self.name = name
        self.data = data

    def getType(self):
        return "INTPAIR"

class GOCIntTripleProperty():
    def __init__(self, name, data):
        self.name = name
        self.data = data

    def getType(self):
        return "INTTRIPPLE"

class GOCMapProperty():
    def __init__(self, name):
        self.name = name
        self.childProperties = []

    def getType(self):
        return "MAP"



# gameObjectClass is the id of the class itself
# since a game object can be ingame more than once there is also a id for those
class GameObjectRepresentation(og.UserDefinedObject):     
    class PropertieRepresentation():
        def __init__(self, name = "", type = "STRING", data = ""):
            self.name = name
            self.type = type
            self.data = data

    def __init__(self, inWorldId, gameObjectClass, node, meshFile = None):
        og.UserDefinedObject.__init__(self)
        self.inWorldId = inWorldId
        self.gocName = str(gameObjectClass)
        self.node = node
        self.meshFile = meshFile
        self.state = "IN_SCENE"
        self.propertieDict = {}
    
    def addProperty(self, repeat = False):
        if not repeat:
            self.editor = GOGenericEditor("", "STRING", "", QApplication.focusWidget())
            
        if self.editor.exec_():
            rep = GameObjectRepresentation.PropertieRepresentation()
            rep.name = str(self.editor.nameEditBox.text())
            rep.type = str(self.editor.typeDropBox.currentText())
            
            if rep.type == "STRING":
                rep.data = str(self.editor.dataEditBox.text())
            elif rep.type == "BOOL":
                text = str(self.editor.dataEditBox.text())
                if text == "True" or text == "1" or text == "true":
                    rep.data = "True"
                elif text == "False" or text == "0" or text == "false":
                    rep.data = "False"
                else:
                    reply = QMessageBox.question(QApplication.focusWidget(), "Try again?", "Data is not a BOOL" , QMessageBox.Yes|QMessageBox.No)
                    if reply == QMessageBox.Yes:
                        self.addProperty(True)
                        return
                    elif reply == QMessageBox.No:
                        return
            elif rep.type == "REAL":
                try:
                     rep.data = str(float(self.editor.dataEditBox.text()))
                except ValueError, e:
                    reply = QMessageBox.question(QApplication.focusWidget(), "Try again?", "Data is not a REAL" , QMessageBox.Yes|QMessageBox.No)
                    if reply == QMessageBox.Yes:
                        self.addProperty(True)
                        print "ValueError: " + str(e)
                        return
                    elif reply == QMessageBox.No:
                        return
            elif rep.type == "INT":
                try:
                     rep.data = str(int(self.editor.dataEditBox.text()))
                except ValueError, e:
                    reply = QMessageBox.question(QApplication.focusWidget(), "Try again?", "Data is not a INT" , QMessageBox.Yes|QMessageBox.No)
                    if reply == QMessageBox.Yes:
                        self.addProperty(True)
                        print "ValueError: " + str(e)
                        return
                    elif reply == QMessageBox.No:
                        return
                
            if rep.name in self.propertieDict:
                reply = QMessageBox.question(QApplication.focusWidget(), "Warning...", "Replace the existing property?" , QMessageBox.Yes|QMessageBox.No|QMessageBox.Cancel)
                if reply == QMessageBox.Cancel:
                    return
                elif reply == QMessageBox.Yes:
                    self.propertieDict[rep.name] = rep
                elif reply == QMessageBox.No:
                    self.addProperty(True)
            else:
                self.propertieDict[rep.name] = rep
                
    def editProperty(self, description):
        return
    
    def getType(self):
        return "GAME_OBJECT_REPRESENTATION"

    def setPosition(self, pos):
        self.node.setPosition(pos)


class GameObjectClass():
    def __init__(self,  parentElement):
        self.classid = parentElement.get("classid")
        self.baseclass = parentElement.get("baseclass")

        self.meshFile = None

        self.properties = []

        for property in parentElement:#.getiterator("property"):
            if property.get("type") == "ARRAY":
                pass
            elif property.get("type") == "MAP":
                self.properties.append(self.createPropertyMap(property))
            else:
                self.properties.append(self.createProperty(property))

    def getType(self):
        return "GAME_OBJECT"

    def createPropertyMap(self, property):
        propMap = GOCMapProperty(property.get("name"))
        for subProperty in property:
            if subProperty.get("type") == "MAP":
                propMap.childProperties.append(self.createPropertyMap(property))
            else:
                for subProperty1 in property:
                    propMap.childProperties.append(self.createProperty(subProperty1))

        return propMap

    def getMeshFileName(self):
        if self.meshFile is None:
            for prop in self.properties:
                if prop.getType() == "STRING":
                    if prop.name == "meshfile":
                        self.meshFile = prop.data
                        return self.meshFile
            return None
        else:
            return self.meshFile

    def createProperty(self, property):
        if property.get("type") == "STRING":
            name = property.get("name")
            data = property.get("data")
            return GOCStringProperty(name, data)
        elif property.get("type") == "REAL":
            name = property.get("name")
            data = property.get("data")
            return GOCRealProperty(name, data)
        elif property.get("type") == "BOOL":
            name = property.get("name")
            data = property.get("data")
            return GOCBoolProperty(name, data)
        elif property.get("type") == "INT":
            name = property.get("name")
            data = property.get("data")
            return GOCIntProperty(name, data)
        elif property.get("type") == "INTPAIR":
            name = property.get("name")
            data = property.get("data")
            return GOCIntPairProperty(name, data)
        elif property.get("type") == "INTTRIPLE":
            name = property.get("name")
            data = property.get("data")
            return GOCIntTripleProperty(name, data)
        else:
            print property.get("type")



