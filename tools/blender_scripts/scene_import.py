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
        self.nodes = {}
        self.matrices = {}
        self.nodenames = {}

class DotSceneSaxHandler(xml.sax.handler.ContentHandler):
    global IMPORT_SCENE_SCALE_FACTOR
    
    def __init__(self, dirname):
        self.scene = 0
        self.mesh = 0
        self.dirname = dirname
        self.matrix = Blender.Mathutils.Matrix( \
             [1.0, 0.0, 0.0, 0.0], \
             [0.0, 1.0, 0.0, 0.0], \
             [0.0, 0.0, 1.0, 0.0], \
             [0.0, 0.0, 0.0, 1.0])
        self.meshparser = xml.sax.make_parser()   
        self.meshhandler = ogre_import.OgreMeshSaxHandler()
        self.meshparser.setContentHandler( self.meshhandler )
        self.nodeid = -1
        self.nodename = ""
        self.materialFile = ""
        self.counter = 1
        
    def startDocument(self):
        self.scene = Scene()
        self.counter = 1
            
    def translate(self, m, x, y, z):
        m1 = TranslationMatrix(Vector([x, y, z]))
        m *= m1
        dlog(m)
        return m

    def scale(self, m, x, y, z):
        m1 = ScaleMatrix(x, 4, Vector([1.0, 0.0, 0.0]))
        m2 = ScaleMatrix(y, 4, Vector([0.0, 1.0, 0.0]))
        m3 = ScaleMatrix(z, 4, Vector([0.0, 0.0, 1.0]))
        m *= m1 * m2 * m3
        dlog(m)
        return m
    
    def rotate(self, m, qx, qy, qz, qw):
        q = Quaternion([qw, qx, qy, qz])
        dlog(m)
        qm = q.toMatrix()
        qm.resize4x4()
        dlog(qm)
        m *= qm
        return m
    
    def startElement(self, name, attrs):
        if name == "node":
            self.mesh = ogre_import.Mesh()
            self.matrix = Blender.Mathutils.Matrix( \
                 [1.0, 0.0, 0.0, 0.0], \
                 [0.0, 1.0, 0.0, 0.0], \
                 [0.0, 0.0, 1.0, 0.0], \
                 [0.0, 0.0, 0.0, 1.0])
            
        if name == "position":
            x = float(attrs.get('x', "0.0")) 
            y = -float(attrs.get('z', "0.0"))
            z = float(attrs.get('y', "0.0")) 
            dlog("Position:")
            self.matrix = \
                self.translate(self.matrix, x, y, z) 
            
        if name == "rotation":
            qx = float(attrs.get('qx', "0.0"))
            qy = -float(attrs.get('qz', "0.0"))
            qz = float(attrs.get('qy', "0.0"))
            qw = float(attrs.get('qw', "0.0"))
            dlog("Rotation:")
            self.matrix = \
                self.rotate(self.matrix, \
                     qx, qy, qz, qw)
            
        if name == "scale":
            x = float(attrs.get('x', "0.0"))
            y = float(attrs.get('z', "0.0"))
            z = float(attrs.get('y', "0.0"))
            dlog("Scale: ")
            self.matrix = \
                self.scale(self.matrix, x, y, z)
            
        if name == "entity":
            self.materialFile = attrs.get('materialFile', "")

            meshfile = Blender.sys.join(self.dirname, attrs.get('meshFile', ""))
            meshname = attrs.get('meshFile', "")
            # is this a mesh file instead of an xml file?
            if ( meshname.lower().find( '.xml' ) == -1 ):
                meshname = meshname[0 : meshname.lower().find( '.mesh' )]
            else:
                meshname  = meshname[0 : meshname.lower().find( '.mesh.xml' )]

            if (self.scene.meshes.has_key(meshname) == False):
                if ( meshfile.lower().find( '.xml' ) == -1 ):
                    # No. Use the xml converter to fix this
                    log( "No mesh.xml file. Trying to convert it from binary mesh format." )
                    ogre_import.convert_meshfile( meshfile )
                    meshfile += '.xml'
                
                try:
                    dlog("Parsing mesh %s " % meshfile)
                    self.meshparser.parse( meshfile )
                except Exception, e:
                    log( "    error parsing mesh %s " % \
                             ( meshfile ) )
                    log( "        exception: %s" % str( e ) )
                self.mesh = self.meshhandler.mesh
                self.mesh.name = meshname
            else:
                dlog(self.scene.meshes.has_key(meshname))
                dlog(self.scene.meshes)
                self.mesh = self.scene.meshes[meshname]
                
            self.nodeid = int(attrs.get("id", "-1"))
            if (self.nodeid == -1):
                self.nodeid = self.counter
                self.counter += 1
                
            self.nodename = attrs.get("name", "")

            # TODO: Castshadow, userdata
            
        
    def endElement(self, name):
        if name == "node":
            dlog("Save Node ID %d" % self.nodeid)
            self.scene.nodes[self.nodeid] = self.mesh
            self.scene.nodenames[self.nodeid] = self.nodename
            self.scene.matrices[self.nodeid] = self.matrix
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
        
    for id, node in handler.scene.nodes.iteritems():
        log("Importing mesh %s (NMesh: %s) into scene" % (handler.scene.nodenames[id], node.name))
        bmesh = ogre_import.CreateBlenderNMesh(str(node.name), node, materials)
        
        object = Blender.Object.New( 'Mesh', \
            handler.scene.nodenames[id] )
        object.link( bmesh )
        
        # apply transformation matrix 
        object.setMatrix(handler.scene.matrices[id])
        scene.link(object)
        object.select(True)

    log ("Scene import done.")
    Blender.Redraw() 


Blender.Window.FileSelector(fileselection_callback, "Import DotScene", "*.scene")
