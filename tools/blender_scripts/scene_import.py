#!BPY

""" Registration info for Blender menus:
Name: 'DotScene (.scene)...'
Blender: 241
Group: 'Import'
Tip: 'Import an DotScene (.scene) file.'
"""

__author__ = "Jochen Schäfer"
__version__ = "0.1 06/06/18"

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
# 0.1: 06/06/18
# -------------
# * Initial version.
#
import Blender
import os
import glob
import re
import math
import xml.sax
import xml.sax.handler
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
    def __init__(self):
        self.meshes = {}
        self.nodes = []

class Node:
    def __init__(self):
        self.name = "NoName"
        self.matrix = Matrix()
        self.matrix.resize4x4()
        self.matrix.identity()
        self.id = -1
        self.mesh = 0
        
class NodeMesh(ogre_import.Mesh):
    def __init__(self):
        ogre_import.Mesh.__init__(self)
        self.materialfile = 0
        self.filename = 0
        self.meshid = -1

class DotSceneSaxHandler(xml.sax.handler.ContentHandler):
    global IMPORT_SCENE_SCALE_FACTOR
    
    def __init__(self, dirname):
        xml.sax.handler.ContentHandler.__init__(self)
        self.scene = Scene()
        self.node = Node()
        self.dirname = dirname
        self.meshparser = xml.sax.make_parser()   
        self.meshhandler = ogre_import.OgreMeshSaxHandler()
        self.meshparser.setContentHandler( self.meshhandler )
        self.counter = 1
        
    def startDocument(self):
        self.scene = Scene()
        self.mesh = NodeMesh()
        self.node = Node()
        self.counter = 1
            
    def translate(self, m, x, y, z):
        x *= ogre_import.IMPORT_SCALE_FACTOR
        y *= ogre_import.IMPORT_SCALE_FACTOR
        z *= ogre_import.IMPORT_SCALE_FACTOR
        m1 = TranslationMatrix(Vector([x, y, z]))
        m *= m1
        return m

    def scale(self, m, x, y, z):
        x *= ogre_import.IMPORT_SCALE_FACTOR
        y *= ogre_import.IMPORT_SCALE_FACTOR
        z *= ogre_import.IMPORT_SCALE_FACTOR
        m1 = ScaleMatrix(x, 4, Vector([1.0, 0.0, 0.0]))
        m2 = ScaleMatrix(y, 4, Vector([0.0, 1.0, 0.0]))
        m3 = ScaleMatrix(z, 4, Vector([0.0, 0.0, 1.0]))
        m *= m1 * m2 * m3
        return m
    
    def rotate(self, m, qx, qy, qz, qw):
        q = Quaternion([qw, qx, qy, qz])
        qm = q.toMatrix()
        qm.resize4x4()
        m *= qm
        return m
    
    def handleEntity(self, attrs):
        self.mesh.materialFile = attrs.get('materialFile', "")

        meshfilename = attrs.get('meshFile', "")
        meshfile = Blender.sys.join(self.dirname, meshfilename)
        # is this a mesh file instead of an xml file?
        if ( meshfilename.lower().find( '.xml' ) == -1 ):
            meshfilename = meshfilename[0 : meshfilename.lower().find( '.mesh' )]
        else:
            meshfilename  = meshfilename[0 : meshfilename.lower().find( '.mesh.xml' )]

        if (self.scene.meshes.has_key(meshfilename) == False):
            if ( meshfile.lower().find( '.xml' ) == -1 ):
                xmlname =  meshfile + '.xml'
                xml_files = glob.glob(xmlname)
                if (len(xml_files) == 0):
                    # No. Use the xml converter to fix this
                    log( "No mesh.xml file. Trying to convert it from binary mesh format." )
                    ogre_import.convert_meshfile( meshfile )
                else:
                    log("%s already exists. No need for conversion." % xmlname)
                meshfile = xmlname
                
            try:
                dlog("Parsing mesh %s " % meshfile)
                self.meshparser.parse( meshfile )
            except Exception, e:
                log( "    error parsing mesh %s " % \
                             ( meshfile ) )
                log( "        exception: %s" % str( e ) )
            self.mesh = self.meshhandler.mesh
            self.mesh.name = attrs.get('name', meshfilename)
                
            dlog("Meshname %s" % self.mesh.name)
            self.mesh.filename = meshfilename
        else:
            self.mesh = self.scene.meshes[meshfilename]
                
        self.node.name = attrs.get('name', meshfilename)
        self.node.id = int(attrs.get("id", "-1"))
        if (self.node.id == -1):
            self.node.id = self.counter
            self.counter += 1   
        else:
            if self.counter < self.node.id:
                self.counter = self.node.id + 1
                         
            # TODO: Castshadow, userdata

    def startElement(self, name, attrs):
        if name == "node":
            self.mesh = NodeMesh()
            self.node = Node()
            
        if name == "position":
            x = float(attrs.get('x', "0.0")) 
            y = float(attrs.get('z', "0.0"))
            z = float(attrs.get('y', "0.0")) 
            self.node.matrix = \
                self.translate(self.node.matrix, x, y, z) 
            
        if name == "rotation":
            qx = float(attrs.get('qx', "0.0"))
            qy = float(attrs.get('qz', "0.0"))
            qz = float(attrs.get('qy', "0.0"))
            qw = float(attrs.get('qw', "0.0"))
            self.node.matrix = \
                self.rotate(self.node.matrix, \
                     qx, qy, qz, qw)
            
        if name == "scale":
            x = float(attrs.get('x', "0.0"))
            y = float(attrs.get('z', "0.0"))
            z = float(attrs.get('y', "0.0"))
            self.node.matrix = \
                self.scale(self.node.matrix, x, y, z)
            
        if name == "entity":
            self.handleEntity(attrs)
        
    def endElement(self, name):
        if name == "node":
            dlog("Saving Node ID %d" % self.node.id)
            self.node.mesh = self.mesh
            self.scene.nodes.append(self.node)
            if (self.mesh.name == None):
                self.mesh.name = self.node.name
            self.mesh.name = str(self.mesh.name)
            self.scene.meshes[self.mesh.name] = self.mesh
      
def fileselection_callback(filename):
    log("Reading scene file %s..." % filename)

    dirname = Blender.sys.dirname(filename)
    basename = Blender.sys.basename(filename)

    # parse material files and make up a dictionary: {mat_name:material, ..}
    materials = ogre_import.collect_materials(dirname)
    
    # Init parser, then parse
    parser = xml.sax.make_parser()   
    handler = DotSceneSaxHandler(dirname)
    parser.setContentHandler(handler)
    parser.parse(filename)
 
    for k, v in handler.scene.meshes.iteritems():
        dlog("Key %s (%s)" % (k, type(k)))
 
    # Now we create a new scene
    name = basename[0:basename.lower().find('.scene')]
    log("Creating scene %s..." % name)
#    camdata = Blender.Camera.New('ortho')           # create new camera data
#    camdata.setName('newCam')
#    camdata.setLens(16.0)
    scene = Blender.Scene.New(name)
#    camobj = Blender.Object.New('Camera')
#    camobj.link(camdata)         
#    #scene.link(camobj)           
#    # Let there be light
#    l = Blender.Lamp.New('Spot')            # create new 'Spot' lamp data
#    l.setMode('Square', 'Shadows')   # set these two lamp mode flags
#    ob = Blender.Object.New('Lamp')         # create new lamp object
#    ob.link(l)   
#    #scene.link(ob)   
    scene.makeCurrent() 
        
    nodescount = len(handler.scene.nodes)
    for i in range(0, nodescount):
        node = handler.scene.nodes[i]
        mesh = node.mesh
        if mesh != None:
            dlog("Mesh name %s" % mesh.name)
            bmesh = Blender.NMesh.GetRaw(mesh.name)
            print(bmesh)
            if bmesh != None:
                log("Mesh is already registered")
            else:
                log("Importing mesh %s into scene" % (mesh.name))
                bmesh = ogre_import.CreateBlenderNMesh(str(mesh.name), mesh, materials)
                        
            object = Blender.Object.New( 'Mesh', node.name)
            object.link(bmesh)
                  
            # apply transformation matrix 
            object.setMatrix(node.matrix)
            scene.link(object)
            object.select(True)

    log ("Scene import done.")
    Blender.Redraw() 


Blender.Window.FileSelector(fileselection_callback, "Import DotScene", "*.scene")
