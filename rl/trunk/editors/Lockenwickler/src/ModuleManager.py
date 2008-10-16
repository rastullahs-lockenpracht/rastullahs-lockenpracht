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
import xml.dom.minidom as xml

import ctypes
import ogre.renderer.OGRE as og

from MovePivot import *

# a class to store information about a object that got selected
class SelectionObject():
    def __init__(self,  entity,  distance):
        self.entityName = entity.getName()
        self.entity = entity #the selected entity
        self.distance = distance # the distance from camera at the time of selection
        self.isPivot = False

    #if True this instance will show its bounding box else it will hide it
    def setSelected(self,  selected):
        if selected == True:
            self.entity.getParentNode().showBoundingBox(True)
        else:
            self.entity.getParentNode().showBoundingBox(False)

    def __eq__(self, other):
        return self.entity.getName() == other.entity.getName()

    def __ne__(self, other):
        return self.entity.getName() != other.entity.getName()

class MyRaySceneQueryListener ( og.RaySceneQueryListener ):
    def __init__( self ):
        super ( MyRaySceneQueryListener, self ).__init__()
        self.previouseSelection = None
        self.selectionList = []
        self.previousSelected = -1
        self.currentSelected = -1

        self.mNormalList = []
        self.mVertexList = []
        self.mIndexList = []

        self.currentRay = None
        self.lastRay = None

    # sort algorithm for the selection list
    def sortCompareImp(self,  x,  y):
        if x.distance > y.distance:
            return 1
        elif x.distance == y.distance:
            return 0
        else: # x<y
            return -1

    def queryResult ( self, entity, distance ):
        #print "dbg: " + entity.getName()
        if distance == 0.0: #camera is in the bounding box, ignore this selection
            return True
#        elif entity.getName() == "rayLine" :
#            return True
        elif entity.getName() == "OgreMainWin::0::ViewportGrid":
            return True
        elif entity.isVisible() and entity.getName() == "EditorXArrow":
            so = SelectionObject(entity,  distance)
            so.isPivot = True
            self.selectionList.append(so)
            return False
        elif entity.isVisible() and entity.getName() == "EditorYArrow":
            so = SelectionObject(entity,  distance)
            so.isPivot = True
            self.selectionList.append(so)
            return False
        elif entity.isVisible() and entity.getName() == "EditorZArrow":
            so = SelectionObject(entity,  distance)
            so.isPivot = True
            self.selectionList.append(so)
            return False
        elif entity.isVisible() and entity.getName() == "EditorFreeMover":
            return True
        elif entity.getName() == "EditorXRotator" or entity.getName() == "EditorYRotator" or entity.getName() == "EditorZRotator":
            if entity.isVisible() and self.rayCastToPolygonLevelOnSingleMesh(self.currentRay,  entity):
                so = SelectionObject(entity,  distance)
                so.isPivot = True
                self.selectionList.append(so)
                return False
            else:
                return True
        else:
            so = SelectionObject(entity,  distance)
            self.selectionList.append(so)

        self.selectionList.sort(self.sortCompareImp)

        return True

    def reset(self):
        self.previousSelected = -1
        self.currentSelected = -1
        del self.selectionList[:]
        #self.selectionList = []

#    def iterateEntityUnderMouse(self):
#        self.previousSelected = self.currentSelected
#        if len(self.selectionList) >= self.currentSelected: # would mean we are out of bounds
#            self.selectionList[self.currentSelected].setSelected(False)
#
#        self.currentSelected += 1
#
#        if len(self.selectionList) == self.currentSelected: # means we are out of bounds and reached the end of the list, reset it to zero
#            self.currentSelected = 0
#
#        if len(self.selectionList) >= self.currentSelected: # would mean we are out of bounds
#            #print str(self.selectionList[self.currentSelected].distance) + " "  + self.selectionList[self.currentSelected].entity.getName()
#            if self.rayCastToPolygonLevelOnCurrentSelection():
#                self.selectionList[self.currentSelected].setSelected(True)
#                return self.selectionList[self.currentSelected]


    def getMeshInformation(self,  entity):
        numVertices = 0
        numIndices = 0
        useSharedVertices = False

        if not entity:
            return False

        pMesh = entity.getMesh()

        pos =    entity.getParentNode().getWorldPosition()
        orient = entity.getParentNode().getWorldOrientation()
        scale =  entity.getParentNode().getScale()

        for i in range ( pMesh.getNumSubMeshes() ):
            pSubMesh = pMesh.getSubMesh(i)
            if pSubMesh.useSharedVertices:
                useSharedVertices = True
            else:
                numVertices += pSubMesh.vertexData.vertexCount
            numIndices += pSubMesh.indexData.indexCount

        if useSharedVertices:
            numVertices += pMesh.sharedVertexData.vertexCount

            storageclass = ctypes.c_float * 3
            test=storageclass(0.0,  0.0,  0.0)
#         mVertexList = new Point[numVertices];
#         mIndexList = new unsigned int[numIndices];

        self.mVertexList = []
        self.mIndexList = []

        ## Count the number of vertices and incides so we can Set them
        indexCount = 0
        vertListCount = 0

        if useSharedVertices:
            ## Real* pVertices (x, y, z order x numVertices)
            elem = pMesh.sharedVertexData.vertexDeclaration.findElementBySemantic(og.VES_POSITION)

            if not elem:
                ogre.Except(Exception.ERR_ITEM_NOT_FOUND, "Can't find position elements in the "
                    "mesh to be written!", "MeshSerializerImpl.writeGeometry")

            vbuf = pMesh.sharedVertexData.vertexBufferBinding.getBuffer(elem.getSource())

            ## need space for the 3 verticies
            storageclass = ctypes.c_float * 3
            test=storageclass(0.0,  0.0,  0.0)

            for j in range ( pMesh.sharedVertexData.vertexCount ):
                vbuf.readData(j * vbuf.getVertexSize(), 3 * ctype.sizeof(ctype.c_float), ctype.addressof(test))
                self.mVertexList.append( og.Vector3(test[0], test[1], test[2]) )
                vertListCount+=1

        for i in range ( pMesh.getNumSubMeshes() ):
            pSubMesh = pMesh.getSubMesh(i)
            if not pSubMesh.useSharedVertices:
                ## Real* pVertices (x, y, z order x numVertices)
                elem = pSubMesh.vertexData.vertexDeclaration.findElementBySemantic(og.VES_POSITION)

                if not elem:
                    og.Except(Exception.ERR_ITEM_NOT_FOUND, "Can't find position elements in the "
                        "mesh to be written!", "MeshSerializerImpl.writeGeometry")

                vbuf = pSubMesh.vertexData.vertexBufferBinding.getBuffer(elem.getSource())

                ## need space for the verticies
                storageclass = ctypes.c_float * (pSubMesh.vertexData.vertexCount * 6)
                test=storageclass(0.0)

                vbuf.readData(0, pSubMesh.vertexData.vertexCount * 6 * ctypes.sizeof(ctypes.c_float), ctypes.addressof(test))

                for j in range ( 0,  pSubMesh.vertexData.vertexCount * 6,  6):
                    #print j
                    p = og.Vector3(test[j], test[j+1], test[j+2])
                    p /= scale
                    p += pos
                    p = orient * p
                    self.mVertexList.append( p )
                    vertListCount += 1

            ibuf = pSubMesh.indexData.indexBuffer
            ## need space for the verticies
            storageclass = ctypes.c_ushort * pSubMesh.indexData.indexCount
            test2=storageclass()


            ibuf.readData(0, ibuf.getSizeInBytes(), ctypes.addressof(test2))
            for j in range ( pSubMesh.indexData.indexCount ):
                self.mIndexList.append (test2[j])   # unsigned short
                indexCount += 1

            ih = 0
            for blah in self.mVertexList:
                #print str(ih) + ": "  +  str(blah)
                ih += 1

#            for blah in self.mIndexList:
#                print blah


    # used when a new selection is made, meaning when not iterationg through the selected objects
    def rayCastToPolygonLevel(self,  ray):
        self.lastRay = ray

        for so in self.selectionList:
            if so.isPivot:
                return so

        if len(self.selectionList) >= 1:
            for so in self.selectionList:
                if self.rayCastToPolygonLevelOnSingleMesh(ray,  so.entity):
                    return so


    def rayCastToPolygonLevelOnSingleMesh(self,  ray,  entity):
        return True
        self.getMeshInformation(entity)
        name = entity.getName()
        print "dbg: " + name

        temp = []
        for vec in self.mVertexList:
            temp.append(vec.x)
            temp.append(vec.y)
            temp.append(vec.z)

        globalPosition = entity.getParentNode().getWorldPosition()
        globalOrientation = entity.getParentNode().getWorldOrientation()


        i = 0
        while i <= (len(self.mIndexList) - 3):
            verta = globalPosition + self.mVertexList[self.mIndexList[i]]
            vertb = globalPosition + self.mVertexList[self.mIndexList[i+1]]
            vertc = globalPosition + self.mVertexList[self.mIndexList[i+2]]

            verta = globalOrientation * verta
            vertb = globalOrientation * vertb
            vertc = globalOrientation * vertc
            normal = og.Math.calculateBasicFaceNormal(verta, vertb, vertc)

            result = og.Math.intersects(ray, verta, vertb, vertc, True, True)
            #result = og.Math.intersects(ray, globalPosition + self.mVertexList[self.mIndexList[i]], globalPosition + self.mVertexList[self.mIndexList[i+1]],
            #                                                                                                                                              globalPosition + self.mVertexList[self.mIndexList[i+2]], normal,  True, True)

            if result.first:
                print "dbg: Treffer!!!!!!!!!"
                return True

            i += 3

        return False

    def rayCastToPolygonLevelOnCurrentSelection(self):
        if len(self.selectionList) >= 1:
            self.getMeshInformation(self.selectionList[self.currentSelected].entity)

            i = 0
            while i <= (len(self.mIndexList) - 3):
                globalPosition = self.selectionList[self.currentSelected].entity.getParentNode().getPosition()
                result = og.Math.intersects(self.lastRay, globalPosition + self.mVertexList[self.mIndexList[i]], globalPosition + self.mVertexList[self.mIndexList[i+1]],
                                                                                                                                                          globalPosition + self.mVertexList[self.mIndexList[i+2]], True, True)

                if result.first:
                   return True

                i += 3
            return False

class ModuleManager(object):
    def __init__(self,  ogreRoot,  sceneManager):
        self.sceneManager = sceneManager
        self.ogreRoot = ogreRoot
        self.raySceneQuery = self.sceneManager.createRayQuery(og.Ray())

        self.userSelectionList = []

        self.listenerDings = MyRaySceneQueryListener()

        self.lastRay = None
#        self.rayLine = None

        self.pivot = None
        self.movingPivot = False

        self.leftMouseDown = False
        self.middleMouseDown = False
        self.rightMouseDown = False

        self.dropCount = 0
        self.dropNode = None
        self.dropEntity = None
        self.dropCollisionPlane = og.Plane(og.Vector3.UNIT_Y, og.Vector3.ZERO)

        self.numerOfCopys = 0 #everytime a copy is made this numer is increased to generate unique node and mesh names

    def load(self,  moduleName,  mapFiles):
        self.moduleName = moduleName
        self.mapFiles = mapFiles

        for a in self.mapFiles:
            doc = xml.parse(a)
            node = doc.getElementsByTagName("entity")
            if node != None:
                self.parseSceneNodes(node)

        self.pivot = Pivot(self.sceneManager)
        self.pivot.hide()

    def parseSceneNodes(self,  nodeList):
        for ent in nodeList:
            entityName = ent.attributes["name"].nodeValue # get the name of the ent
            meshFile = ent.attributes["meshfile"].nodeValue # meshfile
            nodePosition = None
            nodeRotation = None
            nodeScale = None

            for cn in ent.childNodes:
                if cn.nodeType == cn.ELEMENT_NODE:
                    if cn.localName == "position":
                        px = float(cn.attributes["x"].nodeValue)
                        py = float(cn.attributes["y"].nodeValue)
                        pz = float(cn.attributes["z"].nodeValue)
                        nodePosition = og.Vector3(px, py, pz)
                        continue

                    if cn.localName == "rotation":
                        qw = float(cn.attributes["qw"].nodeValue)
                        qx = float(cn.attributes["qx"].nodeValue)
                        qy = float(cn.attributes["qy"].nodeValue)
                        qz = float(cn.attributes["qz"].nodeValue)
                        nodeRotation = og.Quaternion(qw,  qx, qy, qz)
                        continue

                    if cn.localName == "scale":
                        px = float(cn.attributes["x"].nodeValue)
                        py = float(cn.attributes["y"].nodeValue)
                        pz = float(cn.attributes["z"].nodeValue)
                        nodeScale = og.Vector3(px, py, pz)
                        continue

            try:
                e = self.sceneManager.createEntity(entityName, meshFile)
            except:
                print "Warning: Meshfile " + meshFile + " could not be found."
                return

            n = self.sceneManager.getRootSceneNode().createChild(entityName + "_node")
            n.attachObject(e)
            n.setPosition(nodePosition)
            #n.setOrientation(nodeRotation)
            n.setScale(nodeScale)

        pass

    # called when a click into Main Ogre Window occurs
    def selectionClick(self,  ray,  controlDown=False,  shiftDown=False):
        self.listenerDings.reset()
        self.lastRay = ray
        self.listenerDings.currentRay = ray
        self.raySceneQuery.Ray = ray
        self.raySceneQuery.execute(self.listenerDings)

        so = self.listenerDings.rayCastToPolygonLevel(ray)
        if so is not None:
            if not so.isPivot:
                if self.pivot is not None:
                    self.pivot.show()
                if not controlDown and not shiftDown:
                    self.resetSelection()
                    so.setSelected(True)
                    self.userSelectionList.append(so)
                    self.updatePivots()
                elif controlDown and not shiftDown:
                    so.setSelected(True)

                    for soFromList in self.userSelectionList:
                        if soFromList == so:
                            return # object already selected

                    self.userSelectionList.append(so)
                    self.updatePivots()


                elif not controlDown and shiftDown:
                    for selo in self.userSelectionList:
                        if so == selo:
                            so.setSelected(False)
                            self.userSelectionList.remove(selo)
                    self.updatePivots()
            else:
                #so.entity is the pivot direction that was clicked
                self.pivot.startTransforming(so.entity,  self.userSelectionList)
        else:
            self.resetSelection() # click in empty space, deselect everything
            if self.pivot is not None:
                self.pivot.hide()

#        if self.rayLine == None:
#            self.rayLine = self.sceneManager.createManualObject("rayLine")
#            self.rayLine.setDynamic(True)
#            self.sceneManager.getRootSceneNode().createChildSceneNode("raynode").attachObject(self.rayLine)
#
#            self.rayLine.begin("BaseWhiteNoLighting", og.RenderOperation.OT_LINE_STRIP)
#
#            self.rayLine.position(ray.getOrigin())
#            self.rayLine.position( ray.getPoint(10000))
#
#            self.rayLine.end()
#
#        else:
#            self.rayLine.beginUpdate(0)
#
#            self.rayLine.position(ray.getOrigin())
#            self.rayLine.position( ray.getPoint(10000))
#
#            self.rayLine.end()

    def deleteObjects(self):
        if len(self.userSelectionList) < 1:
            return

        self.pivot.hide()

        for so in self.userSelectionList:
            self.sceneManager.destroySceneNode(so.entity.getParentNode().getName())
            del so

        self.userSelectionList = []

    def incrementNameSuffixNumber(self, name):
        newName = ""
        split = name.split("_")
        lastPart = len(split)-1
        newName = name.rstrip(split[lastPart])
        newName = newName + str(self.numerOfCopys)

#        if split[lastPart].isdigit() and not split[lastPart].startswith("0"):
#            num = int(split[lastPart])
#            num = num + 1
#            newName = name.rstrip(split[lastPart])
#            newName = newName + str(num)
#        else:
#            newName = name + "_1"

        self.numerOfCopys = self.numerOfCopys + 1
        return newName

    def copyObjects(self):
        if len(self.userSelectionList) < 1:
            return

        newSelectionList = []
        print "dbg: "
        for so in self.userSelectionList:
            nodeName = self.incrementNameSuffixNumber(so.entity.getParentNode().getName())
            newNode = self.sceneManager.getRootSceneNode().createChild(nodeName)

            entityName = self.incrementNameSuffixNumber(so.entity.getName())
            newEntity = self.sceneManager.createEntity(entityName, so.entity.getMesh().getName())

            newNode.attachObject(newEntity)
            newNode.setPosition(so.entity.getParentNode().getPosition())
            newNode.setOrientation(so.entity.getParentNode().getOrientation())
            newNode.setScale(so.entity.getParentNode().getScale())

            newSO = SelectionObject(newEntity, so.distance)
            newSO.setSelected(True)
            newSelectionList.append(newSO)

        self.resetSelection()
        self.userSelectionList = newSelectionList

    def cutObjects(self):
        if len(self.userSelectionList) < 1:
            return
        print "dbg: cut"

    def pasteObjects(self):
        print "dbg: paste"

    def leftMouseUp(self):
        if self.pivot is not None and self.pivot.isTransforming:
            self.pivot.stopTransforming()

#    def iterateEntityUnderMouse(self):
#        self.listenerDings.iterateEntityUnderMouse()
#
#        pass

    def resetSelection(self):
        for so in self.userSelectionList:
            so.setSelected(False)

        self.userSelectionList = []

        self.listenerDings.reset()
        pass


    def updatePivots(self):
        newPivotPosition = og.Vector3(0, 0, 0)

        for so in self.userSelectionList:
            newPivotPosition += so.entity.getParentNode().getPosition()
        if self.pivot is not None:
            self.pivot.setPosition(newPivotPosition / len(self.userSelectionList))

    def unload(self,  saveOnUnload=True):
        pass

    def save(self):
        pass

    def startDropModelAction(self, meshFile, ray):
        self.dropEntity = self.sceneManager.createEntity("dropMesh" + str(self.dropCount), str(meshFile))
        self.dropNode = self.sceneManager.getRootSceneNode().createChild("dropNode" + str(self.dropCount))
        self.dropNode.attachObject(self.dropEntity)

        result = og.Math.intersects(ray, self.dropCollisionPlane)
        if result.first == True:
            self.dropNode.setPosition(ray.getPoint(result.second))
        else:
            self.dropNode.setPosition(ray.getPoint(50))

        self.dropCount += 1

    def moveDropModelAction(self, ray):
        result = og.Math.intersects(ray, self.dropCollisionPlane)
        if result.first == True:
            self.dropNode.setPosition(ray.getPoint(result.second))
        else:
            self.dropNode.setPosition(ray.getPoint(50))


    def stopDropModelAction(self, ray):
        pass

