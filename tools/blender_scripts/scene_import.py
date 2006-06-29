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
        self.nodes = []
        self.matrices = []

class DotSceneSaxHandler(xml.sax.handler.ContentHandler):
    global IMPORT_SCENE_SCALE_FACTOR
    
    def __init__(self):
        self.scene = 0
        self.mesh = 0
        self.matrix = Blender.Mathutils.Matrix( \
             [1.0, 0.0, 0.0, 0.0], \
             [0.0, 1.0, 0.0, 0.0], \
             [0.0, 0.0, 1.0, 0.0], \
             [0.0, 0.0, 0.0, 1.0])
        
    def startDocument(self):
        self.scene = Scene()
            
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
# TODO Materialfile
#            mat = attrs.get('materialFile', "")
#            if mat != "":
#                self.mesh.setMaterial( mat )

            meshfile = attrs.get('meshFile', "")
            # is this a mesh file instead of an xml file?
            if ( meshfile.lower().find( '.xml' ) == -1 ):
                # No. Use the xml converter to fix this
                log( "No mesh.xml file. Trying to convert it from binary mesh format." )
                ogre_import.convert_meshfile( meshfile )
                meshfile += '.xml'
                
            meshparser = xml.sax.make_parser()   
            meshhandler = ogre_import.OgreMeshSaxHandler()
            meshparser.setContentHandler( meshhandler )
            try:
                meshparser.parse( open( meshfile ) )
            except Exception, e:
                log( "    error parsing mesh %s " % \
                         ( meshfile ) )
                log( "        exception: %s" % str( e ) )
            self.mesh = meshhandler.mesh
            self.mesh.name = meshfile[0 : meshfile.lower().find( '.mesh.xml' )]


            # TODO: Castshadow, ID, userdata
            
        
    def endElement(self, name):
        if name == "node":
            self.scene.nodes.append(self.mesh)
            self.scene.matrices.append(self.matrix)
            
      
def fileselection_callback(filename):
    log("Reading scene file %s..." % filename)

    dirname = Blender.sys.dirname(filename)
    basename = Blender.sys.basename(filename)

    # parse material files and make up a dictionary: {mat_name:material, ..}
    materials = ogre_import.collect_materials(dirname)
    
    # Init parser, then parse
    parser = xml.sax.make_parser()   
    handler = DotSceneSaxHandler()
    parser.setContentHandler(handler)
    parser.parse(open(filename))
 
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
        
    node_count = len(handler.scene.nodes) 
    for i in range(0, node_count):
        node = handler.scene.nodes[i]
        log("Importing mesh %s into scene" % node.name)
        mesh = ogre_import.CreateBlenderMesh(node.name, node, materials)
        # apply transformation matrix 
        matrix = mesh.matrix
        matrix.identity()
        mesh.setMatrix(matrix)
        mesh.getData().transform(handler.scene.matrices[i], 1)
        scene.link(mesh)
        mesh.select(True)

    log ("Scene import done.")
    Blender.Redraw() 


Blender.Window.FileSelector(fileselection_callback, "Import DotScene", "*.scene")
