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

class GOCStringProperty():
    def __init__(self, name, data):
        self.name = name
        self.data = data

    def getType(self):
        return "STRING"

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


class GameObjectClass():
    def __init__(self,  parentElement):
        self.classid = parentElement.get("classid")
        self.baseclass = parentElement.get("baseclass")

        self.properties = []

        for property in parentElement.getiterator("property"):
            if property.get("type") == "ARRAY":
                pass
            elif property.get("type") == "STRING":
                name = property.get("name")
                data = property.get("data")
                self.properties.append(GOCStringProperty(name, data))
            elif property.get("type") == "REAL":
                name = property.get("name")
                data = property.get("data")
                self.properties.append(GOCRealProperty(name, data))
            elif property.get("type") == "BOOL":
                name = property.get("name")
                data = property.get("data")
                self.properties.append(GOCBoolProperty(name, data))
            elif property.get("type") == "MAP":
                pass
            elif property.get("type") == "INT":
                name = property.get("name")
                data = property.get("data")
                self.properties.append(GOCIntProperty(name, data))
            elif property.get("type") == "INTPAIR":
                name = property.get("name")
                data = property.get("data")
                self.properties.append(GOCIntPairProperty(name, data))
            elif property.get("type") == "INTTRIPLE":
                name = property.get("name")
                data = property.get("data")
                self.properties.append(GOCIntTripleProperty(name, data))
            else:
                print property.get("type")
