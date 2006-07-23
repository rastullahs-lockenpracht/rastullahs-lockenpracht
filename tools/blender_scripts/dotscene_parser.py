#!python
# -*- coding: iso-8859-1 -*-

__author__ = "Jochen Schäfer"
__version__ = "0.1 07/12/06"


# Copyright (c) 2006 Jochen Schäfer
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
# IN THE SOFTWARE.

# HISTORY:
# 0.1: 07/12/06
# -------------
# * Initial version.

import string
from string import lower
import xml.dom
import xml.dom.minidom
from xml.dom.minidom import *

PARSE_LOG_LEVEL = 3

def log(msg):
    if PARSE_LOG_LEVEL >= 1: print msg

def vlog(msg):
    if PARSE_LOG_LEVEL >= 2: print msg

def dlog(msg):
    if PARSE_LOG_LEVEL >= 3: print msg


######################################
# Data structures                    #
######################################


class Scene(object):
    def __init__(self):
        self.formatVersion = 0.0
        self.id = None
        self.sceneManager = None
        self.minOgreVersion = 0.0
        self.author = None
        self.nodes = None
        self.externals = None
        self.environment = None
        self.terrain = None
        self.userDataReference = None
        self.userData = {}
        
    def __str__(self):
        str = "Scene Object:\n"
        for attr in self.__dict__:
            str += attr + " => " + self.__dict__[attr].__str__() + "\n"
        return str
        
class Nodes(object):
    def __init__(self):
        self.position = None
        self.rotation = None
        self.scale = None
        self.node = []
        self.userData = {}
        
    def __str__(self):
        str = "Nodes:\n"
        for attr in self.__dict__:
            str += attr + " => " + self.__dict__[attr].__str__() + "\n"
        return str
        
class Node(object):
    def __init__(self):
        self.name = None
        self.id = None
        self.isTarget = True
        self.position = None
        self.rotation = None
        self.scale = None
        self.lookTarget = None
        self.trackTarget = None
        self.node = []
        self.entity = []
        self.light = []
        self.camera = []
        self.particleSystem = []
        self.billboardSet = []
        self.plane = []
        self.userDataReference = None
        self.userData = {}

    def __str__(self):
        str = "Node:\n"
        for attr in self.__dict__:
            str += attr + " => " + self.__dict__[attr].__str__() + "\n"
        return str
        
class Terrain(object):
    def __init__(self):
        self.dataFile = None
        self.userData = {}

    def __str__(self):
        str = "Terrain:\n"
        for attr in self.__dict__:
            str += attr + " => " + self.__dict__[attr].__str__() + "\n"
        return str
       
class UserDataReference(object):
    def __init__(self):
        self.id = None
        self.userData = {}
 
    def __str__(self):
        str = "UserDataReference:\n"
        for attr in self.__dict__:
            str += attr + " => " + self.__dict__[attr].__str__() + "\n"
        return str
       
class Octree(object):
    def __init__(self):
        self.binfile = None        
        self.octnode = OctNode()
        self.userData = {}

    def __str__(self):
        str = "Octree:\n"
        for attr in self.__dict__:
            str += attr + " => " + self.__dict__[attr].__str__() + "\n"
        return str
        
class OctNode(object):
    def __init__(self):
        self.px = 0.0
        self.py = 0.0
        self.pz = 0.0
        self.width = 0.0
        self.height = 0.0
        self.depth = 0.0
        self.octNode = []
        self.octMesh = []
        self.userData = {}

    def __str__(self):
        str = "OctNode:\n"
        for attr in self.__dict__:
            str += attr + " => " + self.__dict__[attr].__str__() + "\n"
        return str

class OctMesh(object):
    def __init__(self):
        self.octGeometry = OctGeometry()
        self.octMaterial = OctMaterial()
        self.userData = {}

    def __str__(self):
        str = "OctMesh:\n"
        for attr in self.__dict__:
            str += attr + " => " + self.__dict__[attr].__str__() + "\n"
        return str
        
class OctGeometry(object):
    def __init__(self):
        self.binaryDataOffset = None
        self.vertTotal = None
        self.triTotal = None
        self.normalTotal = None
        self.colorTotal = None
        self.texSets = None
        self.texTotal = None
        self.userData = {}

    def __str__(self):
        str = "OctGeomtry:\n"
        for attr in self.__dict__:
            str += attr + " => " + self.__dict__[attr].__str__() + "\n"
        return str
        
class OctMaterial(object):
    def __init__(self):
        self.name = None
        self.texture = None
        self.userData = {}

    def __str__(self):
        str = "OctMaterial:\n"
        for attr in self.__dict__:
            str += attr + " => " + self.__dict__[attr].__str__() + "\n"
        return str
        
class Rotation(object):
    def __init__(self):
        self.qx = 0.0
        self.qy = 0.0
        self.qz = 0.0
        self.qw = 0.0
        self.axisX = 0.0
        self.axisY = 0.0
        self.axisZ = 0.0
        self.angle = 0.0
        self.angleX = 0.0
        self.angleY = 0.0
        self.angleZ = 0.0
        self.userData = {}

    def __str__(self):
        str = "Rotation:\n"
        for attr in self.__dict__:
            str += attr + " => " + self.__dict__[attr].__str__() + "\n"
        return str
        
class LookTarget(object):
    def __init__(self):
        self.nodeName = None
        self.relativeTo = "parent"
        self.position = None
        self.localDirection = None
        self.userData = {}

    def __str__(self):
        str = "LookTarget:\n"
        for attr in self.__dict__:
            str += attr + " => " + self.__dict__[attr].__str__() + "\n"
        return str
        
class TrackTarget(object):
    def __init__(self):
        self.nodeName = None
        self.offset = None
        self.localDirection = None
        self.userData = {}

    def __str__(self):
        str = "TrackTarget:\n"
        for attr in self.__dict__:
            str += attr + " => " + self.__dict__[attr].__str__() + "\n"
        return str
        
class Entity(object):
    def __init__(self):
        self.vertexBuffer = None
        self.indexBuffer = None
        self.userDataReference = None
        self.name = None
        self.id = None
        self.meshFile = None
        self.materialFile = None
        self.static = False
        self.castShadows = True
        self.userData = {}
        
    def __str__(self):
        str = "Entity:\n"
        for attr in self.__dict__:
            str += attr + " => " + self.__dict__[attr].__str__() + "\n"
        return str

class Buffer(object):
    def __init__(self):
        self.usage = "staticWriteOnly"
        self.useShadow = True
        self.userData = {}
        
    def __str__(self):
        str = "Buffer:\n"
        for attr in self.__dict__:
            str += attr + " => " + self.__dict__[attr].__str__() + "\n"
        return str
        
class ParticleSystem(object):
    def __init__(self):
        self.name = None
        self.id = None
        self.file = None
        self.userDataReference = None
        self.userData = {}

    def __str__(self):
        str = "ParticleSystem:\n"
        for attr in self.__dict__:
            str += attr + " => " + self.__dict__[attr].__str__() + "\n"
        return str
        
class BillboardSet(object):
    def __init__(self):
        self.name = None
        self.material = None
        self.id = None
        self.width = 10.0
        self.height = 10.0
        self.type = "point"
        self.origin = "center"
        self.billboard = []
        self.userData = {}
        
    def __str__(self):
        str = "BillboardSet:\n"
        for attr in self.__dict__:
            str += attr + " => " + self.__dict__[attr].__str__() + "\n"
        return str

class Billboard(object):
    def __init__(self):
        self.id = None
        self.width = 0.0
        self.height = 0.0
        self.position = None
        self.rotation = Rotation
        self.colourDiffuse = None
        self.userData = {}

    def __str__(self):
        str = "Billboard:\n"
        for attr in self.__dict__:
            str += attr + " => " + self.__dict__[attr].__str__() + "\n"
        return str
        
class Plane(object):
    def __init__(self):
        self.name = None
        self.id = None
        self.distance = 0.0
        self.width = 0.0
        self.height = 0.0
        self.xSegments = 1
        self.ySegments = 1
        self.numTexCoordSets =  1
        self.uTile = 1.0
        self.vTile = 1.0
        self.material = None
        self.normals = True
        self.normal = None
        self.upVector = None
        self.vertexBuffer = None
        self.indexBuffer = None
        self.userData = {}

    def __str__(self):
        str = "Plane:\n"
        for attr in self.__dict__:
            str += attr + " => " + self.__dict__[attr].__str__() + "\n"
        return str
    
class Externals(object):
    def __init__(self):
        self.item = []
        self.userData = {}

    def __str__(self):
        str = "Externals:\n"
        for attr in self.__dict__:
            str += attr + " => " + self.__dict__[attr].__str__() + "\n"
        return str

class Item(object):
    def __init__(self):
        self.type = None
        self.file = File()
        self.userData = {}
        
    def __str__(self):
        str = "Item:\n"
        for attr in self.__dict__:
            str += attr + " => " + self.__dict__[attr].__str__() + "\n"
        return str

class File(object):
    def __init__(self):
        self.name = None    
        self.userData = {}

    def __str__(self):
        str = "File:\n"
        for attr in self.__dict__:
            str += attr + " => " + self.__dict__[attr].__str__() + "\n"
        return str

class Environment(object):
    def __init__(self):
        self.fog = None
        self.skyBox = None
        self.skyDome = None
        self.skyPlane = None
        self.clipping = None
        self.colourAmbient = None
        self.colourBackground = None
        self.userDataReference = None
        self.userData = {}
        
    def __str__(self):
        str = "Environment:\n"
        for attr in self.__dict__:
            str += attr + " => " + self.__dict__[attr].__str__() + "\n"
        return str

class Clipping(object):
    def __init__(self):
        self.near = None
        self.far = None
        self.userData = {}
        
    def __str__(self):
        str = "Clipping:\n"
        for attr in self.__dict__:
            str += attr + " => " + self.__dict__[attr].__str__() + "\n"
        return str

class Fog(object):
    def __init__(self):
        self.expDensity = 0.001
        self.linearStart = 0.0
        self.linearEnd = 1.0
        self.mode = "none"
        self.userData = {}
        
    def __str__(self):
        str = "Fog:\n"
        for attr in self.__dict__:
            str += attr + " => " + self.__dict__[attr].__str__() + "\n"
        return str

class SkyBox(object):
    def __init__(self):
        self.rotation = Rotation()
        self.material = None
        self.distance = 5000.0
        self.drawFirst = True
        self.userData = {}
        
    def __str__(self):
        str = "SkyBox:\n"
        for attr in self.__dict__:
            str += attr + " => " + self.__dict__[attr].__str__() + "\n"
        return str

class SkyDome(object):
    def __init__(self):
        self.rotation = Rotation()
        self.material = None
        self.curvature = 10.0
        self.tiling = 8.0
        self.distance = 4000.0
        self.drawFirst = True
        self.userData = {}
        
    def __str__(self):
        str = "SkyDome:\n"
        for attr in self.__dict__:
            str += attr + " => " + self.__dict__[attr].__str__() + "\n"
        return str

class SkyPlane(object):
    def __init__(self):
        self.material = None
        self.planeX = 0.0
        self.planeY = -1.0
        self.planeZ = 0.0
        self.planeD = 5000.0
        self.scale = 1000.0
        self.bow = 0.0
        self.tiling = 10.0
        self.drawFirst = True
        self.userData = {}
        
    def __str__(self):
        str = "SkyPlane:\n"
        for attr in self.__dict__:
            str += attr + " => " + self.__dict__[attr].__str__() + "\n"
        return str

class Light(object):
    def __init__(self):
        self.name = None
        self.id = None
        self.type = "point"
        self.visible = True
        self.castShadows = True
        self.position = None
        self.normal = None
        self.colourDiffuse = None
        self.colourSpecular = None
        self.lightRange = None
        self.lightAttenuation = None
        self.userDataReference = None
        self.userData = {}

    def __str__(self):
        str = "Light:\n"
        for attr in self.__dict__:
            str += attr + " => " + self.__dict__[attr].__str__() + "\n"
        return str

class LightRange(object):
    def __init__(self):
        self.inner = None
        self.outer = None
        self.falloff = None
        self.userData = {}

    def __str__(self):
        str = "LightRange:\n"
        for attr in self.__dict__:
            str += attr + " => " + self.__dict__[attr].__str__() + "\n"
        return str
        
class LightAttenuation(object):
    def __init__(self):
        self.range = None
        self.constant = None
        self.linear = None
        self.quadratric = None
        self.userData = {}

    def __str__(self):
        str = "LightAttenuation:\n"
        for attr in self.__dict__:
            str += attr + " => " + self.__dict__[attr].__str__() + "\n"
        return str
        
class Camera(object):
    def __init__(self):
        self.name = None
        self.id = None
        self.fov = None
        self.aspectRatio = None
        self.projectionType = "perspective"
        self.clipping = None
        self.position = None
        self.rotation = None
        self.normal = None
        self.lookTarget = None
        self.trackTarget = None
        self.userDataReference = None
        self.userData = {}
        
    def __str__(self):
        str = "Camera:\n"
        for attr in self.__dict__:
            str += attr + " => " + self.__dict__[attr].__str__() + "\n"
        return str
        
####################################
# Handling procedures              #
####################################

        
def getAttribute(element, attrName, default = None):
    if element.nodeType == xml.dom.Node.ELEMENT_NODE:
        value = str(element.getAttribute(attrName))
        if value == "":
            value = default
        return value
    else:
        return default;

def createScene(root):
    vlog("Creating Scene")
    scene = Scene()
    scene.formatVersion = getAttribute(root, "formatVersion", "0.0")
    scene.id = getAttribute(root, "id")
    scene.sceneManager = getAttribute(root, "sceneManager")
    scene.minOgreVersion = getAttribute(root, "minOgreVersion", "0.0")
    scene.author = getAttribute(root, "author")

    for child in root.childNodes:
        if child.nodeType == xml.dom.Node.ELEMENT_NODE:
            if child.tagName == "nodes":
                scene.nodes = handleNodes(child)
            elif child.tagName == "externals":
                scene.externals = handleExternals(child)
            elif child.tagName == "environment":
                scene.environment = handleEnvironment(child)
            elif child.tagName == "terrain":
                scene.terrain = handleTerrain(child)
            elif child.tagName == "userDataReference":
                scene.userDataReference = handleUserDataReference(child)
            elif child.tagName == "octree":
                scene.octree = handleOctree(child)
            elif child.tagName == "light":
                scene.light = handleLight(child)
            elif child.tagName == "camera":
                scene.camera = handleCamera(child)
            elif lower(child.tagName) == "userdata":
                scene.userData = handleUserData(child)
            else:
                vlog ("Scene: Encountered unknown tag %s" % child.tagName)

    return scene
    
    
def handleNodes(node):
    dlog("Handling Nodes")
    nodes = Nodes()
    for child in node.childNodes:
        if child.nodeType == xml.dom.Node.ELEMENT_NODE:
            if child.tagName == "node":
                nodes.node.append(handleNode(child))
            elif child.tagName == "position":
                nodes.position = handleVector(child)
            elif child.tagName == "scale":
                nodes.scale = handleVector(child)
            elif child.tagName == "rotation":
                nodes.rotation = handleRotation(child)
            elif lower(child.tagName) == "userdata":
                scene.userData = handleUserData(child)
            else:
                vlog("Nodes: Encountered unknown tag %s" % child.tagName)

    return nodes
       
    
def handleTerrain(node):
    dlog("Handling Terrain.")
    terrain = Terrain()
    terrain.dataFile = getAttribute(node, 'dataFile')
    for child in node.childNodes:
        if child.nodeType == xml.dom.Node.ELEMENT_NODE:
            if lower(child.tagName) == "userdata":
                terrain.userData = handleUserData(child)
            else:
                vlog("Terrain: Encountered unknown tag %s" % child.tagName)
    return terrain
    
def handleUserDataReference(node):
    dlog("Handling UserDataReference")
    userDataReference = UserDataReference()
    userDataReference.id = getAttribute(node, 'id')
    return userDataReference
    
def handleOctree(node):
    dlog("Handling Octree.")
    octree = Octree()
    octree.binFile = getAttribute(node, 'binFile')
    for child in node.childNodes:
        if child.nodeType == xml.dom.Node.ELEMENT_NODE:
            if child.tagName == "octNode":
                octree.octnode = handleOctNode(child)
            elif lower(child.tagName) == "userdata":
                scene.userData = handleUserData(child)
            else:
                vlog("Octree: Encountered unknown tag %s" % child.tagName)
    return octree

def handleOctNode(node):
    dlog("Handling OctNode.")
    octNode = OctNode()
    octNode.px = float(getAttribute(node, 'px', 0.0))
    octNode.py = float(getAttribute(node, 'py', 0.0))
    octNode.pz = float(getAttribute(node, 'pz', 0.0))
    octNode.width = float(getAttribute(node, 'width', 0.0))
    octNode.height = float(getAttribute(node, 'height', 0.0))
    octNode.depth = float(getAttribute(node, 'depth', 0.0))
    for child in node.childNodes:
        if child.nodeType == xml.dom.Node.ELEMENT_NODE:
            if child.tagName == "octNode":
                octNode.octNode.append(handleOctNode(child))
            elif child.tagName == "octMesh":
                octNode.octMesh.append(handleOctMesh(child))
            elif lower(child.tagName) == "userdata":
                octNode.userData = handleUserData(child)
            else:
                vlog("OctNode: Encountered unknown tag %s" % child.tagName)
    return octNode

def handleOctMesh(node):                
    dlog("Handling OctMesh.")
    octMesh = OctMesh()
    for child in node.childNodes:
        if child.nodeType == xml.dom.Node.ELEMENT_NODE:
            if child.tagName == "octGeometry":
                octMesh.octGeometry = handleOctGeometry(child)
            elif child.tagName == "octMaterial":
                octMesh.octMaterial = handleOctMaterial(child)
            elif lower(child.tagName) == "userdata":
                octMesh.userData = handleUserData(child)
            else:
                vlog("OctMesh: Encountered unknown tag %s" % child.tagName)
    return octMesh
    
def handleOctGeometry(node):                
    dlog("Handling OctGeometry.")
    octGeometry = OctGeometry()
    octGeometry.binaryDataOffset = getAttribute(node, 'binaryDataOffset')
    octGeometry.colorTotal = getAttribute(node, 'colorTotal')
    octGeometry.normalTotal = getAttribute(node, 'normalTotal')
    octGeometry.texSets = getAttribute(node, 'texSets')
    octGeometry.texTotal = getAttribute(node, 'texTotal')
    octGeometry.triTotal = getAttribute(node, 'triTotal')
    octGeometry.vertTotal = getAttribute(node, 'vertTotal')
    for child in node.childNodes:
        if child.nodeType == xml.dom.Node.ELEMENT_NODE:
            if lower(child.tagName) == "userdata":
                octGeometry.userData = handleUserData(child)
            else:
                vlog("OctGeometry: Encountered unknown tag %s" % child.tagName)
    return octGeometry

def handleOctMaterial(node):                
    dlog("Handling OctMaterial.")
    octMaterial = OctMaterial()
    octMaterial.name = getAttribute(node, 'name')
    octMaterial.texture = getAttribute(node, 'texture')
    for child in node.childNodes:
        if child.nodeType == xml.dom.Node.ELEMENT_NODE:
            if lower(child.tagName) == "userdata":
                octMaterial.userData = handleUserData(child)
            else:
                vlog("OctMaterial: Encountered unknown tag %s" % child.tagName)
    return octMaterial

def handleNode(node):
    dlog("Handling Node name %s" % getAttribute(node, "name"))
    
    snode = Node()
    snode.name = getAttribute(node, 'name')
    snode.id = getAttribute(node, 'id')
    snode.isTarget = bool(getAttribute(node, 'isTarget', True))

    for child in node.childNodes:
        if child.nodeType == xml.dom.Node.ELEMENT_NODE:
            if child.tagName == "position":
                snode.position = handleVector(child)
            elif child.tagName == "rotation":
                snode.rotation = handleRotation(child)
            elif child.tagName == "scale":
                snode.scale = handleVector(child)
            elif child.tagName == "entity":
                snode.entity.append(handleEntity(child))
            elif child.tagName == "light":
                snode.light.append(handleLight(child))
            elif child.tagName == "camera":
                snode.camera.append(handleCamera(child))
            elif child.tagName == "userDataReference":
                snode.userDataReference = handleUserDataReference(child)
            elif child.tagName == "lookTarget":
                snode.lookTarget = handleLookTarget(child)
            elif child.tagName == "trackTarget":
                snode.trackTarget = handleTrackTarget(child)
            elif child.tagName == "node":
                snode.node.append(handleNode(child))
            elif child.tagName == "particleSystem":
                snode.particleSystem.append(handleParticleSystem(child))
            elif child.tagName == "billboardSet":
                snode.billboardSet.append(handleBillboardSet(child))
            elif child.tagName == "plane":
                snode.plane.append(handlePlane(child))
            elif lower(child.tagName) == "userdata":
                snode.userData = handleUserData(child)
            else:
                vlog("Node: Encountered unknown tag %s" % child.tagName)
    return snode
        
def handleEntity(node):
    dlog("Handling entity %s" % getAttribute(node, 'name'))
    entity = Entity()
    entity.name = getAttribute(node, 'name')
    entity.id = getAttribute(node, 'id')
    entity.meshFile = getAttribute(node, 'meshFile')
    entity.materialFile = getAttribute(node, 'materialFile')
    entity.static = bool(getAttribute(node, 'static', False))
    entity.castShadows = bool(getAttribute(node, 'castShadows', True))
    for child in node.childNodes:
        if child.nodeType == xml.dom.Node.ELEMENT_NODE:
            if child.tagName == "vertexBuffer":
                entity.vertexBuffer = handleBuffer(child)
            elif child.tagName == "indexBuffer":
                entity.indexBuffer = handleBuffer(child)
            elif child.tagName == "userDataReference":
                entity.userDataReference = handleUserDataReference(child)
            elif lower(child.tagName) == "userdata":
                entity.userData = handleUserData(child)
            else:
                vlog("Entity: Encountered unknown tag %s" % child.tagName)
    return entity
                
def handleVector(node):
    dlog("Handling a vector for tag %s" % node.tagName)
    v = [ \
        float(getAttribute(node, 'x', 0.0)), \
        float(getAttribute(node, 'y', 0.0)), \
        float(getAttribute(node, 'z', 0.0)) \
    ]
    return v

def handleColour(node):
    dlog("Handling colour for tag %s" % node.tagName)
    v = [ \
        float(getAttribute(node, 'r', 0.0)), \
        float(getAttribute(node, 'g', 0.0)), \
        float(getAttribute(node, 'b', 0.0)) \
    ]
    return v

def handleRotation(node):
    dlog("Handling rotation")
    rotation = Rotation()
    rotation.qx = float(getAttribute(node, 'qx', 0.0))
    rotation.qy = float(getAttribute(node, 'qy', 0.0))
    rotation.qz = float(getAttribute(node, 'qz', 0.0))
    rotation.qw = float(getAttribute(node, 'qw', 0.0))
    rotation.axisX = float(getAttribute(node, 'axisX', 0.0))
    rotation.axisY = float(getAttribute(node, 'axisY', 0.0))
    rotation.axisZ = float(getAttribute(node, 'axisZ', 0.0))
    rotation.angle = float(getAttribute(node, 'angle', 0.0))
    rotation.angleX = float(getAttribute(node, 'angleX', 0.0))
    rotation.angleY = float(getAttribute(node, 'angleY', 0.0))
    rotation.angleZ = float(getAttribute(node, 'angleZ', 0.0))
    return rotation
 
def handleLookTarget(node):
    dlog("Handling LookTarget")
    lookTarget = LookTarget()
    lookTarget.nodeName = getAttribute(node, 'nodeName')
    lookTarget.relativeTo = getAttribute(node, 'relativeto', "parent")
    for child in node.childNodes:
        if child.nodeType == xml.dom.Node.ELEMENT_NODE:
            if child.tagName == "position":
                lookTarget.position = handleVector(child)
            elif child.tagName == "localDirection":
                lookTarget.localDirection = handleVector(child)
            elif lower(child.tagName) == "userdata":
                lookTarget.userData = handleUserData(child)
            else:
                vlog("LookTarget: Encountered unknown tag %s" % child.tagName)
    
    return lookTarget
    
def handleTrackTarget(node):
    dlog("Handling TrackTarget")
    trackTarget = TrackTarget()
    trackTarget.nodeName = getAttribute(node, 'nodeName')
    for child in node.childNodes:
        if child.nodeType == xml.dom.Node.ELEMENT_NODE:
            if child.tagName == "offset":
                trackTarget.offset = handleVector(child)
            elif child.tagName == "localDirection":
                trackTarget.localDirection = handleVector(child)
            elif lower(child.tagName) == "userdata":
                trackTarget.userData = handleUserData(child)
            else:
                vlog("TrackTarget: Encountered unknown tag %s" % child.tagName)
    
    return trackTarget

def handleBuffer(node):
    dlog("Handling Buffer for tag %s" % node.tagName)
    buffer = Buffer()
    buffer.usage = getAttribute(node, 'usage', "staticWriteOnly")
    buffer.useShadow = bool(getAttribute(node, 'usage', True))
    for child in node.childNodes:
        if child.nodeType == xml.dom.Node.ELEMENT_NODE:
            if lower(child.tagName) == "userdata":
                buffer.userData = handleUserData(child)
            else:
                vlog("Buffer: Encountered unknown tag %s" % child.tagName)
    return buffer

def handleParticleSystem(node):
    dlog("Handling ParticleSystem")
    particleSystem = ParticleSystem()
    particleSystem.file = getAttribute(node, 'file')
    particleSystem.id = getAttribute(node, 'id')
    particleSystem.name = getAttribute(node, 'name')
    for child in node.childNodes:
        if child.nodeType == xml.dom.Node.ELEMENT_NODE:
            if child.tagName == "userDataReference":
                particleSystem.userDataReference = handleUserDataReference(child)
            elif lower(child.tagName) == "userdata":
                particleSystem.userData = handleUserData(child)
            else:
                vlog("ParticleSystem: Encountered unknown tag %s" % child.tagName)
    
    return particleSystem

def handleBillboardSet(node):
    dlog("Handling BillboardSet")
    billboardset = BillboardSet()
    billboardset.name = getAttribute(node, 'name')
    billboardset.material = getAttribute(node, 'material')
    billboardset.id = getAttribute(node, 'id')
    billboardset.width = float(getAttribute(node, 'width', 10.0))
    billboardset.height = float(getAttribute(node, 'height', 10.0))
    billboardset.type = getAttribute(node, 'type', "point")
    billboardset.origin = getAttribute(node, 'origin', "center")
    for child in node.childNodes:
        if child.nodeType == xml.dom.Node.ELEMENT_NODE:
            if child.tagName == "billboard":
                billboardset.billboard.append(handleBillboard(child))
            elif lower(child.tagName) == "userdata":
                billboardset.userData = handleUserData(child)
            else:
                vlog("BillboardSet: Encountered unknown tag %s" % child.tagName)
    return billboardset
    
def handleBillboard(node):
    dlog("Handling Billboard")
    billboard = Billboard()
    billboard.id = getAttribute(node, 'id')
    billboard.width = float(getAttribute(node, 'width', 0.0))
    billboard.height = float(getAttribute(node, 'height', 0.0))
    for child in node.childNodes:
        if child.nodeType == xml.dom.Node.ELEMENT_NODE:
            if child.tagName == "position":
                billboard.position = handleVector(child)
            elif child.tagName == "rotation":
                billboard.rotation = handleRotation(child)
            elif child.tagName == "colourDiffuse":
                billboard.colourDiffuse = handleColour(child)
            elif lower(child.tagName) == "userdata":
                billboard.userData = handleUserData(child)
            else:
                vlog("Billboard: Encountered unknown tag %s" % child.tagName)
    return billboard

def handlePlane(node):
    dlog("Handling Plane")
    plane = Plane()
    plane.name = getAttribute(node, 'name')
    plane.material = getAttribute(node, 'material')
    plane.id = getAttribute(node, 'id')
    plane.width = float(getAttribute(node, 'width', 10.0))
    plane.height = float(getAttribute(node, 'height', 10.0))
    plane.distance = float(getAttribute(node, 'distance', 0.0))
    plane.xSegments = int(getAttribute(node, 'xSegments', 1))
    plane.ySegments = int(getAttribute(node, 'ySegments', 1))
    plane.numTexcoordSets = int(getAttribute(node, 'numCoordSets', 1))
    plane.uTile = float(getAttribute(node, 'uTile', 1.0))
    plane.vTile = float(getAttribute(node, 'vTile', 1.0))
    plane.normals = bool(getAttribute(node, 'normals', True))
    for child in node.childNodes:
        if child.nodeType == xml.dom.Node.ELEMENT_NODE:
            if child.tagName == "normal":
                plane.normal = handleVector(child)
            elif child.tagName == "upVector":
                plane.upVector = handleVector(child)
            elif child.tagName == "vertexBuffer":
                plane.vertexBuffer = handleBuffer(child)
            elif child.tagName == "indexBuffer":
                plane.indexBuffer = handleBuffer(child)
            elif lower(child.tagName) == "userdata":
                plane.userData = handleUserData(child)
            else:
                vlog("Plane: Encountered unknown tag %s" % child.tagName)
    return plane

def handleExternals(node):
    dlog("Handling Externals.")
    externals = Externals()
    for child in node.childNodes:
        if child.nodeType == xml.dom.Node.ELEMENT_NODE:
            if child.tagName == "item":
                externals.item.append(handleItem(child))
            elif lower(child.tagName) == "userdata":
                externals.userData = handleUserData(child)
            else:
                vlog("Externals: Encountered unknown tag %s" % child.tagName)
    return externals

def handleItem(node):
    dlog("Handling Item.")
    item = Item()
    item.type = getAttribute(node, 'type')
    for child in node.childNodes:
        if child.nodeType == xml.dom.Node.ELEMENT_NODE:
            if child.tagName == "file":
                item.file = handleFile(child)
            elif lower(child.tagName) == "userdata":
                item.userData = handleUserData(child)
            else:
                vlog("Item: Encountered unknown tag %s" % child.tagName)
    return item

def handleFile(node):
    dlog("Handling File.")
    file = File()
    file.name = getAttribute(node, 'name')
    for child in node.childNodes:
        if child.nodeType == xml.dom.Node.ELEMENT_NODE:
            if lower(child.tagName) == "userdata":
                file.userData = handleUserData(child)
            else:
                vlog("File: Encountered unknown tag %s" % child.tagName)
    return file

def handleEnvironment(node):
    dlog("Handling Environment.")
    environment = Environment()
    for child in node.childNodes:
        if child.nodeType == xml.dom.Node.ELEMENT_NODE:
            if child.tagName == "fog":
                environment.fog = handleFog(child)
            elif child.tagName == "skyBox":
                environment.skyBox = handleSkyBox(child)
            elif child.tagName == "skyDome":
                environment.skyDome = handleSkyDome(child)
            elif child.tagName == "skyPlane":
                environment.skyDome = handleSkyPlane(child)
            elif child.tagName == "clipping":
                environment.clipping = handleClipping(child)
            elif child.tagName == "colourAmbient":
                environment.colourAmbient = handleColour(child)
            elif child.tagName == "colourBackground":
                environment.colourBackground = handleColour(child)
            elif child.tagName == "userDataReference":
                environment.userDataReference = handleUserDataReference(child)
            elif lower(child.tagName) == "userdata":
                environment.userData = handleUserData(child)
            else:
                vlog("Environment: Encountered unknown tag %s" % child.tagName)
    return environment

def handleFog(node):
    dlog("Handling Fog.")
    fog = Fog()
    fog.expDensity = float(getAttribute(node, 'expDensity', 0.001))
    fog.linearStart = float(getAttribute(node, 'linearStart', 0.0))
    fog.linearEnd = float(getAttribute(node, 'linearEnd', 1.0))
    fog.mode = getAttribute(node, 'mode', "none")
    for child in node.childNodes:
        if child.nodeType == xml.dom.Node.ELEMENT_NODE:
            if child.tagName == "colourDiffuse":
                fog.colourDiffuse = handleColour(child)
            elif lower(child.tagName) == "userdata":
                fog.userData = handleUserData(child)
            else:
                vlog("Fog: Encountered unknown tag %s" % child.tagName)
    return fog

def handleClipping(node):
    dlog("Handling Clipping.")
    clipping = Clipping()
    clipping.near = getAttribute(node, 'near')
    clipping.far = getAttribute(node, 'far')
    for child in node.childNodes:
        if child.nodeType == xml.dom.Node.ELEMENT_NODE:
            if lower(child.tagName) == "userdata":
                clipping.userData = handleUserData(child)
            else:
                vlog("Clipping: Encountered unknown tag %s" % child.tagName)
    return clipping

def handleSkyBox(node):
    dlog("Handling SkyBox.")
    skybox = SkyBox()
    skybox.material = getAttribute(node, 'material')
    skybox.distance = float(getAttribute(node, 'distance', 5000.0))
    skybox.drawFirst = bool(getAttribute(node, 'drawFirst', True))
    for child in node.childNodes:
        if child.nodeType == xml.dom.Node.ELEMENT_NODE:
            if child.tagName == "rotation":
                skybox.rotation = handleRotation(child)
            elif lower(child.tagName) == "userdata":
                skybox.userData = handleUserData(child)
            else:
                vlog("SkyBox: Encountered unknown tag %s" % child.tagName)
    return skybox

def handleSkyDome(node):
    dlog("Handling SkyDome.")
    skydome = SkyDome()
    skydome.material = getAttribute(node, 'material')
    skydome.curvature = float(getAttribute(node, 'curvature', 10.0))
    skydome.tiling = float(getAttribute(node, 'tiling', 8.0))
    skydome.distance = float(getAttribute(node, 'distance', 4000.0))
    skydome.srawFirst = bool(getAttribute(node, 'drawFirst', True))
    for child in node.childNodes:
        if child.nodeType == xml.dom.Node.ELEMENT_NODE:
            if child.tagName == "rotation":
                skydome.rotation = handleRotation(child)
            elif lower(child.tagName) == "userdata":
                skydome.userData = handleUserData(child)
            else:
                vlog("SkyDome: Encountered unknown tag %s" % child.tagName)
    return skydome

def handleSkyPlane(node):
    dlog("Handling SkyPlane.")
    skyplane = SkyPlane()
    skyplane.material = getAttribute(node, 'material')
    skyplane.planeX = float(getAttribute(node, 'planeX', 0.0))
    skyplane.planeY = float(getAttribute(node, 'planeY', -1.0))
    skyplane.planeZ = float(getAttribute(node, 'planeZ', 0.0))
    skyplane.planeD = float(getAttribute(node, 'planeD', 5000.0))
    skyplane.scale = float(getAttribute(node, 'scale', 1000.0))
    skyplane.bow = float(getAttribute(node, 'bow', 0.0))
    skyplane.tiling = float(getAttribute(node, 'tiling', 10.0))
    skyplane.drawFirst = bool(getAttribute(node, 'drawFirst', True))
    for child in node.childNodes:
        if child.nodeType == xml.dom.Node.ELEMENT_NODE:
            if lower(child.tagName) == "userdata":
                skyplane.userData = handleUserData(child)
            else:
                vlog("Skyplane: Encountered unknown tag %s" % child.tagName)
    return skyplane

def handleLight(node):
    dlog("Handling Light.")
    light = Light()
    light.name = getAttribute(node, 'name')
    light.id = getAttribute(node, 'id')
    light.type = getAttribute(node, 'type', "point")
    light.visible = bool(getAttribute(node, 'visible', True))
    light.castShadows = bool(getAttribute(node, 'castShadows', True))
    for child in node.childNodes:
        if child.nodeType == xml.dom.Node.ELEMENT_NODE:
            if child.tagName == "position":
                light.position = handleVector(child)
            elif child.tagName == "normal":
                light.normal = handleVector(child)
            elif child.tagName == "colourDiffuse":
                light.colourDiffuse = handleColour(child)
            elif child.tagName == "colourSpecular":
                light.colourSpecular = handleColour(child)
            elif child.tagName == "lightRange":
                light.lightRange = handleLightRange(child)
            elif child.tagName == "lightAttenuation":
                light.lightAttenuation = handleLightAttenuation(child)
            elif child.tagName == "userDataReference":
                light.userDataReference = handleUserDataReference(child)
            elif lower(child.tagName) == "userdata":
                light.userData = handleUserData(child)
            else:
                vlog("Light: Encountered unknown tag %s" % child.tagName)
    return light

def handleLightRange(node):
    dlog("Handling LightRange.")
    lightrange = LightRange()
    lightrange.inner = getAttribute(node, 'inner')
    lightrange.outer = getAttribute(node, 'outer')
    lightrange.falloff = getAttribute(node, 'falloff')
    for child in node.childNodes:
        if child.nodeType == xml.dom.Node.ELEMENT_NODE:
            if lower(child.tagName) == "userdata":
                lightrange.userData = handleUserData(child)
            else:
                vlog("LightRange: Encountered unknown tag %s" % child.tagName)
    return lightrange

def handleLightAttenuation(node):
    dlog("Handling LightAttenuation.")
    lightattenuation = LightAttenuation()
    lightattenuation.range = getAttribute(node, 'range')
    lightattenuation.constant = getAttribute(node, 'costant')
    lightattenuation.linear = getAttribute(node, 'linear')
    lightattenuation.quadratic = getAttribute(node, 'quadratic')
    for child in node.childNodes:
        if child.nodeType == xml.dom.Node.ELEMENT_NODE:
            if lower(child.tagName) == "userdata":
                lightattenuation.userData = handleUserData(child)
            else:
                vlog("LightAttenuation: Encountered unknown tag %s" % child.tagName)
    return lightattenuation

def handleCamera(node):
    dlog("Handling Camera.")
    camera = Camera()
    camera.name = getAttribute(node, 'name')
    camera.id = getAttribute(node, 'id')
    camera.fov = getAttribute(node, 'fov')
    camera.aspectRatio = getAttribute(node, 'aspectRatio')
    camera.projectionType = getAttribute(node, 'projectionType', "perspective")
    for child in node.childNodes:
        if child.nodeType == xml.dom.Node.ELEMENT_NODE:
            if child.tagName == "clipping":
                camera.clipping = handleClipping(child)
            elif child.tagName == "position":
                camera.position = handleVector(child)
            elif child.tagName == "rotation":
                camera.rotation = handleRotation(child)
            elif child.tagName == "normal":
                camera.normal = handleVector(child)
            elif child.tagName == "lookTarget":
                camera.lookTarget = handleLookTarget(child)
            elif child.tagName == "trackTarget":
                camera.trackTarget = handleTrackTarget(child)
            elif child.tagName == "userDataReference":
                camera.clipping = handleUserDataReference(child)
            elif lower(child.tagName) == "userdata":
                camera.userData = handleUserData(child)
            else:
                vlog("Camera: Encountered unknown tag %s" % child.tagName)
    return camera
 
def handleUserData(node):
    dlog("Handling UserData")
    userdata = {}
    for child in node.childNodes:
        if child.nodeType == xml.dom.Node.ELEMENT_NODE:
            if child.tagName == "property":
                name = getAttribute(child, 'name')
                type = getAttribute(child, 'type')
                data = getAttribute(child, 'data')
                if lower(type) == "string":
                    userdata[name] = str(data)
                elif lower(type) == "int":
                    userdata[name] = int(data)
                elif lower(type) == "float":
                    userdata[name] = float(data)
                else:
                    vlog("User Data: Encountered unknown property type %s" % type)
    dlog(userdata)
    return userdata
 
 
    
#log("Starting Parser Test")
#sceneDoc = parse("test1.scene")
#scene = createScene(sceneDoc.documentElement)
#log(scene)
#log("Test done")

    
