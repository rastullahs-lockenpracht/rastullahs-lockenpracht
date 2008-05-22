#!BPY

""" Registration info for Blender menus:
Name: 'DotScene (.scene)...'
Blender: 241
Group: 'Import'
Tip: 'Import an DotScene (.scene) file.'
"""

__author__ = "Jochen Schäfer"
__version__ = "0.3 07/18/06"

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
# 0.1: 06/24/06
# -------------
# * Initial version.
#
# 0.2: 07/11/06
# -------------
# * Changed XML parser from SAX to MiniDOM.
# * Meshes are now parsed before constructing the scene.
# * Meshes are only converted to XML if necessary.
# * Meshes are only parsed if not parsed before.
#
# 0.3: 07/18/06
# -------------
# * Now using DotsceneParser to analyze the scene
#
#
import Blender
import os
import glob
import re
import math
import xml.dom
import xml.dom.minidom
from xml.dom.minidom import *
import ogre_import
import Blender.Mathutils
from Blender.Mathutils import *
import dotscene_parser

IMPORT_LOG_LEVEL = 3
materials = []

def log(msg):
    if IMPORT_LOG_LEVEL >= 1: print msg

def vlog(msg):
    if IMPORT_LOG_LEVEL >= 2: print msg

def dlog(msg):
    if IMPORT_LOG_LEVEL >= 3: print msg

def translate(m, v):
    if v != None:
        x = v[0] * ogre_import.IMPORT_SCALE_FACTOR
        y = v[1] * ogre_import.IMPORT_SCALE_FACTOR
        z = v[2] * ogre_import.IMPORT_SCALE_FACTOR 
        m1 = TranslationMatrix(Vector([x, y, z]))
        m *= m1
    return m

def scale(m, v):
    if v != None:
        x = v[0] * ogre_import.IMPORT_SCALE_FACTOR
        y = v[1] * ogre_import.IMPORT_SCALE_FACTOR
        z = v[2] * ogre_import.IMPORT_SCALE_FACTOR 
        m1 = ScaleMatrix(x, 4, Vector([1.0, 0.0, 0.0]))
        m2 = ScaleMatrix(y, 4, Vector([0.0, 1.0, 0.0]))
        m3 = ScaleMatrix(z, 4, Vector([0.0, 0.0, 1.0]))
        m *= m1 * m2 * m3
    return m

def rotate(m, rotation):
    if rotation != None:
        qx = rotation.qx
        qy = rotation.qy
        qz = rotation.qz
        qw = rotation.qw
        q = Quaternion([qw, qx, qy, qz])
        qm = Blender.Mathutils.RotationMatrix( \
           q.angle, 4, "r", q.axis)
        m *= qm
    return m

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

def createEntity(dirname, scene, entity, node, meshes, matrix, materials):
    if entity.name == None or entity.name == "":
        entity.name = node.name
    if entity.name == None or entity.name == "":
		entity.name = entity.meshFile
		
    log ("Creating Blender Mesh %s " % entity.name)
    bmesh = Blender.NMesh.GetRaw(entity.name)
    if bmesh != None:
        log("Mesh is already registered")
    else:
        log("Importing mesh %s into scene" % entity.meshFile)
        
        meshfilename = entity.meshFile
        # is this a mesh file instead of an xml file?
        meshfile = Blender.sys.join(dirname, meshfilename)
        if ( meshfilename.lower().find( '.xml' ) == -1 ):
            meshfilename = meshfilename[0 : meshfilename.lower().find( '.mesh' )]
        else:
            meshfilename  = meshfilename[0 : meshfilename.lower().find( '.mesh.xml' )]

        dlog("Meshfilename %s" % meshfilename)
        if (meshes.has_key(meshfilename) == False):
            dlog("Meshname %s not found. Ignoring mesh." % entity.name)
            return
        else:
            mesh = meshes[meshfilename]       

        bmesh = ogre_import.CreateBlenderNMesh(str(entity.name), mesh, materials)
     
    bobject = Blender.Object.New('Mesh', entity.name)
    bobject.link(bmesh)
    createUserData(entity.userData, bobject)
      
    # apply transformation matrix 
    bobject.setMatrix(matrix)
    scene.link(bobject)
    bobject.select(True)
    return bobject

def createLight(scene, light, node, matrix):
    log ("Creating Blender Light %s " % light.name)
    
    try:
        bLight = Blender.Lamp.Get(light.name)
    except:
        bLight = None

    if bLight != None:
        log("Light is already registered")
    else:
        log("Importing light into scene")
        type = "Photon"
        if light.type == "point":
            type = "Photon"
        elif light.type == "directional":
            type = "Sun"
        elif light.type == "spot":
            type = "Spot"
        elif light.type == "radPoint":
            type = "Hemi"
        else:
            vlog("Unknown light type. Setting Photon lamp")
        bLight = Blender.Lamp.New(type, light.name)
        if light.castShadows:
            bLight.mode |= Blender.Lamp.Modes["Shadows"]
        else:
            bLight.mode &= ~Blender.Lamp.Modes["Shadows"]
        bLight.col = light.colourSpecular

    bobject = Blender.Object.New('Lamp', light.name)
    bobject.link(bLight)
    createUserData(light.userData, bobject)
      
    # apply transformation matrix 
    lmatrix = translate(matrix, light.position)
    bobject.setMatrix(lmatrix)
    scene.link(bobject)
    bobject.select(True)
    return bobject

def createCamera(scene, camera, node, matrix):
    log ("Creating Blender Camera %s " % camera.name)
    
    try:
        bCamera = Blender.Camera.Get(light.name)
    except:
        bCamera = None

    if bCamera != None:
        log("Camera is already registered")
    else:
        log("Importing camera into scene")
        type = "persp"
        if camera.projectionType == "perspective":
            type = "persp"
        elif camera.projectionType == "ortographic" \
                or camera.projectionType == "orthographic":
            type = "ortho"
        else:
            vlog("Unknown camera type. Setting perspective camera")
        bCamera = Blender.Camera.New(type, camera.name)

    bobject = Blender.Object.New('Camera', camera.name)
    bobject.link(bCamera)
    createUserData(camera.userData, bobject)
 
    # apply transformation matrix 
    cmatrix = translate(matrix, camera.position)
    cmatrix = rotate(cmatrix, camera.rotation)
    bobject.setMatrix(cmatrix)
    scene.link(bobject)
    bobject.select(True)
    return bobject

def createUserData(userData, bobject):
    vlog("Creating user data %s" % userData)
    for property in userData:
        dlog("Property %s" % property)
        try:
            bproperty = bobject.getProperty(property)
        except:
            bobject.addProperty(property, userData[property])
        else:
            bproperty.setData(userData[property])


def createScene(filename, dotscene, meshes):
    dirname = Blender.sys.dirname(filename)
    basename = Blender.sys.basename(filename)

    # parse material files and make up a dictionary: {mat_name:material, ..}
    materials = ogre_import.collect_materials(dirname)

    # Now we create a new scene
    name = basename[0:basename.lower().find('.scene')]
    log("Creating scene %s..." % name)
    scene = Blender.Scene.New(name)
    scene.makeCurrent() 
        
    for node in dotscene.nodes.node:
        if node != None:
            matrix = Matrix()
            matrix.resize4x4()
            matrix.identity()
            matrix = translate(matrix, node.position)
            matrix = rotate(matrix, node.rotation)
            matrix = scale(matrix, node.scale)

            bobject = None
            for entity in node.entity:
                bobject = createEntity(dirname, scene, entity, node, meshes, matrix, materials)
                
            for light in node.light:
            	bobject = createLight(scene, light, node, matrix)
            
            for camera in node.camera:
            	bobject = createCamera(scene, camera, node, matrix)
            createUserData(node.userData, bobject)

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
                    dlog("Not cached yet")
                    mesh = parseMesh(meshfile, meshparser, meshhandler)
                    if mesh != None:
                        mesh.filename = meshfile
                        mesh.name = meshnode.getAttribute("name")
                        meshes[meshfilename] = mesh
                    
                else:
                    dlog("Cached")
            
    log ("Mesh import done")
    return meshes
          
def fileselection_callback(filename):
    log("Reading scene file %s..." % filename)

    dirname = Blender.sys.dirname(filename)
    basename = Blender.sys.basename(filename)

    sceneDoc = parse(filename)
    log("%s was parsed. Now handling it" % filename)
    meshes = collect_meshes(sceneDoc, dirname)
    scene = dotscene_parser.createScene(sceneDoc.documentElement)
    log("%s was handled." % filename)
    sceneDoc.unlink()
 
    createScene(filename, scene, meshes)
    log("Scene creation done")


Blender.Window.FileSelector(fileselection_callback, "Import DotScene", "*.scene")