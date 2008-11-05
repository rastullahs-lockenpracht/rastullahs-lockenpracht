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

from elementtree.ElementTree import *

from PyQt4.QtCore import *
from PyQt4.QtGui import *

import ogre.renderer.OGRE as og

from GOStringEditor import *

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
    def __init__(self, inWorldId, gameObjectClass, node, meshFile = None):
        og.UserDefinedObject.__init__(self)
        self.inWorldId = inWorldId
        self.gocName = gameObjectClass
        self.node = node
        self.meshFile = meshFile

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



