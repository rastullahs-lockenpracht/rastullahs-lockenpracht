#!BPY

""" Registration info for Blender menus:
Name: 'DotScene (.scene)...'
Blender: 241
Group: 'Import'
Tip: 'Import an DotScene (.scene) file.'
"""

__author__ = "Jochen Schäfer"
__version__ = "0.2 06/11/06"

__bpydoc__ = """\
This script imports DotScenes into Blender.
"""

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
# 0.1: 06/01/06
# -------------
# * Initial version.
#
# 0.2: 06/11/06
# -------------
# * Changed XML parser from SAX to MiniDOM.
# * Meshes are now parsed before constructing the scene.
# * Meshes are only converted to XML if necessary.
# * Meshes are only parsed if not parsed before.
#

import Blender
import os
import glob
import re
import math
import xml.sax
#import xml.sax.handler
import xml.dom
#from xml.dom import *
import xml.dom.minidom
from xml.dom.minidom import *
import ogre_import
import Blender.Mathutils
from Blender.Mathutils import *

IMPORT_LOG_LEVEL = 3
materials = []

def log(msg):
    if IMPORT_LOG_LEVEL >= 1: print msg

def vlog(msg):
    if IMPORT_LOG_LEVEL >= 2: print msg

def dlog(msg):
    if IMPORT_LOG_LEVEL >= 3: print msg

class Scene:  
    def __init__(self, dirname, meshes):
        self.meshes = meshes
        self.nodes = []
        self.externals = []
        self.dirname = dirname
        self.environment = None
        self.counter = 0
        self.meshparser = xml.sax.make_parser()   
        self.meshhandler = ogre_import.OgreMeshSaxHandler()
        self.meshparser.setContentHandler(self.meshhandler)
        
    def handleScene(self, root):
        if root.hasChildNodes():
            children = root.childNodes
            for child in children:
                if child.nodeType == xml.dom.Node.ELEMENT_NODE:
                    if child.tagName == "nodes":
                        self.nodes = Nodes().handleNodes(self, child)
                    elif child.tagName == "externals":
                        self.externals = Externals().handleExternals(self, child)
                    elif child.tagName == "environment":
                        self.environment = Environment().handleEnvironment(self, child)
                    else:
                        vlog ("Encountered unknown tag %s" % child.tagName)

class Nodes(Node):
    def __init__(self):
        self.nodes = []
        
    def handleNodes(self, scene, nodes):
        dlog("Handling Nodes")
        children = nodes.childNodes
        for child in children:
            if child.nodeType == xml.dom.Node.ELEMENT_NODE:
                if child.tagName == "node":
                    snode = SceneNode()
                    snode.handleNode(scene, child)
                    self.nodes.append(snode)
                else:
                    vlog("Encountered unknown tag %s" % child.tagName)

        return self.nodes

class Externals(Node):
    def __init__(self):
        self.externals = []
           
    def handleExternals(self, scene, externals):
        dlog("Handling Externals. Not implemented yet.")

class Environment(Node):
    def __init__(self):
        self.environment = None
            
    def handleEnvironment(self, scene, environment):
        dlog("Handling Environment. Not implemented yet.")
        
class SceneNode:
    def __init__(self):
        self.name = "SC_NoName"
        self.id = -1
        self.matrix = Matrix()
        self.matrix.resize4x4()
        self.matrix.identity()
        self.object = 0
        
    def translate(self, m, node):
        x = float(node.getAttribute('x')) * ogre_import.IMPORT_SCALE_FACTOR
        y = float(node.getAttribute('y')) * ogre_import.IMPORT_SCALE_FACTOR
        z = float(node.getAttribute('z')) * ogre_import.IMPORT_SCALE_FACTOR 
        m1 = TranslationMatrix(Vector([x, y, z]))
        m *= m1
        return m

    def scale(self, m, node):
        x = float(node.getAttribute('x')) * ogre_import.IMPORT_SCALE_FACTOR
        y = float(node.getAttribute('y')) * ogre_import.IMPORT_SCALE_FACTOR
        z = float(node.getAttribute('z')) * ogre_import.IMPORT_SCALE_FACTOR 
        m1 = ScaleMatrix(x, 4, Vector([1.0, 0.0, 0.0]))
        m2 = ScaleMatrix(y, 4, Vector([0.0, 1.0, 0.0]))
        m3 = ScaleMatrix(z, 4, Vector([0.0, 0.0, 1.0]))
        m *= m1 * m2 * m3
        return m
    
    def rotate(self, m, node):
        qx = float(node.getAttribute('qx'))
        qy = float(node.getAttribute('qy'))
        qz = float(node.getAttribute('qz'))
        qw = float(node.getAttribute('qw'))
        q = Quaternion([qw, qx, qy, qz])
        qm = Blender.Mathutils.RotationMatrix( \
           q.angle, 4, "r", q.axis)
        m *= qm
        return m

    def handleNode(self, scene, node):
        dlog("Node name %s" % node.getAttribute("name"))
        
        # Handle own attributes
        self.name = node.getAttribute('name')
            
        if node.getAttribute('id') == "":
            self.id = -1
        else:
            self.id = int(node.getAttribute("id"))
            
        if (self.id == -1):
            self.id = scene.counter
            scene.counter += 1   
        else:
            if scene.counter < self.id:
                scene.counter = self.id + 1

        # Handle child nodes
        for child in node.childNodes:
            if child.nodeType == xml.dom.Node.ELEMENT_NODE:
                if child.tagName == "position":
                    dlog("position")
                    self.matrix = self.translate(self.matrix, child)
                    
                elif child.tagName == "rotation":
                    dlog("rotation")
                    self.matrix = self.rotate(self.matrix, child)
                    
                elif child.tagName == "scale":
                    dlog("scale")
                    self.matrix = self.scale(self.matrix, child)
                    
                elif child.tagName == "entity":
                    dlog("entity")
                    self.object = NodeMesh()
                    self.object.handleEntity(scene, child)
                    
                elif child.tagName == "light":
                    dlog("light")
                    self.object = NodeLight()
                    self.object.handleLight(scene, child)

                elif child.tagName == "camera":
                    dlog("camera")
                    self.object = NodeCamera()
                    self.object.handleCamera(scene, child)
                    
                elif child.tagName == "userdata":
                    dlog("userdata")
                else:
                    vlog("Encountered unknown tag %s" % child.tagName)
        
class NodeObject:
    def __init__(self):
        self.name = "NO_NoName"
        self.id = -1

class NodeLight(NodeObject):
    def __init__(self):
        NodeObject.__init__(self)

    def handleLight(self, scene, node):
        dlog("Handling light %s" % node.getAttribute('name'))


class NodeCamera(NodeObject):
    def __init__(self):
        NodeObject.__init__(self)

    def handleCamera(self, scene, node):
        dlog("Handling camera %s" % node.getAttribute('name'))


class NodeMesh(NodeObject):
    def __init__(self):
        NodeObject.__init__(self)
        self.mesh = ogre_import.Mesh()
        self.materialfile = 0
        self.filename = 0

    def handleEntity(self, scene, node):
        dlog("Handling entity %s" % node.getAttribute('name'))
        self.name = node.getAttribute('name')
        self.materialFile = node.getAttribute('materialFile')

        meshfilename = node.getAttribute('meshFile')
        # is this a mesh file instead of an xml file?
        meshfile = Blender.sys.join(scene.dirname, meshfilename)
        if ( meshfilename.lower().find( '.xml' ) == -1 ):
            meshfilename = meshfilename[0 : meshfilename.lower().find( '.mesh' )]
        else:
            meshfilename  = meshfilename[0 : meshfilename.lower().find( '.mesh.xml' )]

        dlog("Meshfilename %s" % meshfilename)
        if (scene.meshes.has_key(meshfilename) == False):
            self.mesh = parseMesh(meshfile, scene.meshparser, scene.meshhandler)
            self.name = node.getAttribute('name')
                
            dlog("Meshname %s" % self.name)
            scene.meshes[meshfilename] = self.mesh
        else:
            self.mesh = scene.meshes[meshfilename]       

        self.filename = meshfilename
        if self.name == "":
            self.filename = meshfilename


def parseMesh(meshfile, parser, handler):
    if ( meshfile.lower().find( '.xml' ) == -1 ):
        xmlname =  meshfile + '.xml'
        newermesh = False
        try:
            xml_files = glob.glob(xmlname)
            xmlstat = os.stat(xmlname)
            meshstat = os.stat(meshfile)
            newermesh = (xmlstat.st_mtime < meshstat.st_mtime)
        except Exception, e:
            vlog("Error while stat()ing %s" % str(e))
            
        if (len(xml_files) == 0) or newermesh:
            # No. Use the xml converter to fix this
            log( "No mesh.xml file. Trying to convert it from binary mesh format." )
            ogre_import.convert_meshfile( meshfile )
        else:
            log("%s already exists. No need for conversion." % xmlname)
        meshfile = xmlname
        
    try:
        dlog("Parsing mesh %s " % meshfile)
        parser.parse( meshfile )
    except Exception, e:
        log( "    error parsing mesh %s " % \
                     ( meshfile ) )
        log( "        exception: %s" % str( e ) )
        return None
    
    return handler.mesh

def createScene(basename, dotscene, materials):
    # Now we create a new scene
    name = basename[0:basename.lower().find('.scene')]
    log("Creating scene %s..." % name)
    scene = Blender.Scene.New(name)
    scene.makeCurrent() 
        
    for node in dotscene.nodes:
        object = node.object
        if object != None:
            if str(object.__class__) == "__main__.NodeMesh":
                if object.name == "":
                    object.name = object.filename
                if object.mesh == None:
                    log("Mesh %s obviously wasn't parsed" % object.name)
                    continue
                    
                log ("Creating Blender Mesh %s " % object.name)
                bmesh = Blender.NMesh.GetRaw(object.name)
                if bmesh != None:
                    log("Mesh is already registered")
                else:
                    log("Importing mesh %s into scene" % (object.name))
                    bmesh = ogre_import.CreateBlenderNMesh(str(object.name), object.mesh, materials)
                 
                if node.name == "":
                    node.name = object.name
                    
                bobject = Blender.Object.New('Mesh', node.name)
                bobject.link(bmesh)
                  
                # apply transformation matrix 
                bobject.setMatrix(node.matrix)
                scene.link(bobject)
                bobject.select(True)
                
            elif str(object.__class__) == "__main__.NodeLight":
                log ("Creating Blender Light %s " % object.name)
            
            elif str(object.__class__) == "__main__.NodeCamera":
                log ("Creating Blender Camera %s " % object.name)

            else:
                vlog("Unknown object type")

    log ("Scene import done.")
    Blender.Redraw() 

def collect_meshes(dom, dirname):
    meshes = {}
    meshparser = xml.sax.make_parser()   
    meshhandler = ogre_import.OgreMeshSaxHandler()
    meshparser.setContentHandler(meshhandler)
    
    meshlist = dom.getElementsByTagName("entity")
    for meshnode in meshlist:
        if meshnode.nodeType == xml.dom.Node.ELEMENT_NODE:
            meshfilename = meshnode.getAttribute("meshFile")
            if meshfilename != "":
                meshfile = Blender.sys.join(dirname, meshfilename)
                # is this a mesh file instead of an xml file?
                if ( meshfilename.lower().find( '.xml' ) == -1 ):
                    meshfilename = meshfilename[0 : meshfilename.lower().find( '.mesh' )]
                else:
                    meshfilename  = meshfilename[0 : meshfilename.lower().find( '.mesh.xml' )]

                dlog("Meshfilename %s" % meshfilename)
                if (meshes.has_key(meshfilename) == False):
                    dlog("Cached")
                    mesh = parseMesh(meshfile, meshparser, meshhandler)
                    if mesh != None:
                        mesh.filename = meshfile
                        mesh.name = meshnode.getAttribute("name")
                        meshes[meshfilename] = mesh
                    
                else:
                    dlog("Already there")
                
            
    log ("Mesh import done")
    return meshes
          
def fileselection_callback(filename):
    log("Reading scene file %s..." % filename)

    dirname = Blender.sys.dirname(filename)
    basename = Blender.sys.basename(filename)

    # parse material files and make up a dictionary: {mat_name:material, ..}
    materials = ogre_import.collect_materials(dirname)
    
    sceneDoc = parse(filename)
    log("%s was parsed. Now handling it" % filename)
    meshes = collect_meshes(sceneDoc, dirname)
    scene = Scene(dirname, meshes)
    dlog(meshes)
    scene.handleScene(sceneDoc.documentElement)
    log("%s was handled." % filename)
    sceneDoc.unlink()
 
    createScene(basename, scene, materials)
    log("Scene creation done")


Blender.Window.FileSelector(fileselection_callback, "Import DotScene", "*.scene")
