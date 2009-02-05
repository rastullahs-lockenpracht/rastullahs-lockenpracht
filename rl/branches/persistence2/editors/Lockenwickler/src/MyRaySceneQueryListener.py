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

import ctypes
import ogre.renderer.OGRE as og
import ogre.physics.OgreNewt as on

# a class to store information about a object that got selected
class SelectionObject():
    def __init__(self,  entity,  distance):
        self.entityName = entity.getName()
        self.entity = entity #the selected entity
        self.distance = distance # the distance from camera at the time of selection
        self.isPivot = False

#        if self.entity.getUserObject() is not None:
#            self.isGameObject = True
#        else:
#            self.isGameObject = False

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

        self.World = on.World()
        self.World.setWorldSize(og.Vector3(-1000000, -1000000, -1000000), og.Vector3(1000000, 1000000, 1000000))

    def __del__(self):
        del self.World

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
        elif entity.getName() == "OgreMainWin::0::ViewportGrid":
            return True
        elif entity.getName() == "rayLine":
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
        col = on.TreeCollision(self.World, entity.getParentNode(), False)
        bod = on.Body(self.World, col)


        start = ray.getOrigin()
        end = ray.getPoint(10000)

        retNorm = og.Vector3(0.0, 0.0, 0.0)

        val = on.CollisionRayCast(col, start, end, retNorm)
#        print val
#
#        print retNorm.x
#        print retNorm.y
#        print retNorm.z
        del col
        self.World.destroyAllBodies()
        
        if retNorm.x != 0.0 or retNorm.y != 0.0 or retNorm.z != 0.0:
#           print "yes"
           return True
        else:
#            print "no"
            return False
           
        
        

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


#    def getMeshInformation(self,  entity):
#        numVertices = 0
#        numIndices = 0
#        useSharedVertices = False
#
#        if not entity:
#            return False
#
#        pMesh = entity.getMesh()
#
#        position =    entity.getParentNode()._getDerivedPosition()
#        orient = entity.getParentNode()._getDerivedOrientation()
#        scale =  entity.getParentNode().getScale()
#
#        for i in range ( pMesh.getNumSubMeshes() ):
#            pSubMesh = pMesh.getSubMesh(i)
#            if pSubMesh.useSharedVertices:
#                useSharedVertices = True
#            else:
#                numVertices += pSubMesh.vertexData.vertexCount
#            numIndices += pSubMesh.indexData.indexCount
#
#        if useSharedVertices:
#            numVertices += pMesh.sharedVertexData.vertexCount
#
#            storageclass = ctypes.c_float * 3
#            test=storageclass(0.0,  0.0,  0.0)
##         mVertexList = new Point[numVertices];
##         mIndexList = new unsigned int[numIndices];
#
#        self.mVertexList = []
#        self.mIndexList = []
#
#        ## Count the number of vertices and incides so we can Set them
#        indexCount = 0
#        vertListCount = 0
#
#        if useSharedVertices:
#            ## Real* pVertices (x, y, z order x numVertices)
#            elem = pMesh.sharedVertexData.vertexDeclaration.findElementBySemantic(og.VES_POSITION)
#
#            if not elem:
#                ogre.Except(Exception.ERR_ITEM_NOT_FOUND, "Can't find position elements in the "
#                    "mesh to be written!", "MeshSerializerImpl.writeGeometry")
#
#            vbuf = pMesh.sharedVertexData.vertexBufferBinding.getBuffer(elem.getSource())
#
#            ## need space for the 3 verticies
#            storageclass = ctypes.c_float * 3
#            test=storageclass(0.0,  0.0,  0.0)
#
#            for j in range ( pMesh.sharedVertexData.vertexCount ):
#                vbuf.readData(j * vbuf.getVertexSize(), 3 * ctype.sizeof(ctype.c_float), ctype.addressof(test))
#                pt = og.Vector3(test[0], test[1], test[2])
#                self.mVertexList.append( (orient * (pt * scale)) + position )
#                vertListCount+=1
#
#        for i in range ( pMesh.getNumSubMeshes() ):
#            pSubMesh = pMesh.getSubMesh(i)
#            if not pSubMesh.useSharedVertices:
#                ## Real* pVertices (x, y, z order x numVertices)
#                elem = pSubMesh.vertexData.vertexDeclaration.findElementBySemantic(og.VES_POSITION)
#
#                if not elem:
#                    og.Except(Exception.ERR_ITEM_NOT_FOUND, "Can't find position elements in the "
#                        "mesh to be written!", "MeshSerializerImpl.writeGeometry")
#
#                vbuf = pSubMesh.vertexData.vertexBufferBinding.getBuffer(elem.getSource())
#
#                ## need space for the verticies
#                storageclass = ctypes.c_float * (pSubMesh.vertexData.vertexCount * 6)
#                test=storageclass(0.0)
#
#                vbuf.readData(0, pSubMesh.vertexData.vertexCount * 6 * ctypes.sizeof(ctypes.c_float), ctypes.addressof(test))
#
#                for j in range ( 0,  pSubMesh.vertexData.vertexCount * 6,  6):
#                    #print j
#                    pt = og.Vector3(test[j], test[j+1], test[j+2])
#                    self.mVertexList.append( (orient * (pt * scale)) + position )
#                    vertListCount += 1
#
#            ibuf = pSubMesh.indexData.indexBuffer
#            ## need space for the verticies
#            storageclass = ctypes.c_ushort * pSubMesh.indexData.indexCount
#            test2=storageclass()
#
#
#            ibuf.readData(0, ibuf.getSizeInBytes(), ctypes.addressof(test2))
#            for j in range ( pSubMesh.indexData.indexCount ):
#                self.mIndexList.append (test2[j])   # unsigned short
#                indexCount += 1
#
#            ih = 0
#            for blah in self.mVertexList:
#                #print str(ih) + ": "  +  str(blah)
#                ih += 1
#
#    def rayCastToPolygonLevel(self,  ray):
#        self.lastRay = ray
#
#        for so in self.selectionList:
#            if so.isPivot:
#                return so
#
#        if len(self.selectionList) >= 1:
#            for so in self.selectionList:
#                if self.rayCastToPolygonLevelOnSingleMesh(ray,  so.entity):
#                    return so
#
#
#    def rayCastToPolygonLevelOnSingleMesh(self,  ray,  entity):
#        self.getMeshInformation(entity)
#        name = entity.getName()
#        print "dbg: " + name
#
#        temp = []
#        for vec in self.mVertexList:
#            temp.append(vec.x)
#            temp.append(vec.y)
#            temp.append(vec.z)
#
#        globalPosition = entity.getParentNode()._getDerivedPosition()
#        globalOrientation = entity.getParentNode()._getDerivedOrientation()
#
#
#        i = 0
#        while i <= (len(self.mIndexList) - 3):
#            verta = globalPosition + self.mVertexList[self.mIndexList[i]]
#            vertb = globalPosition + self.mVertexList[self.mIndexList[i+1]]
#            vertc = globalPosition + self.mVertexList[self.mIndexList[i+2]]
#
#            verta = globalOrientation * verta
#            vertb = globalOrientation * vertb
#            vertc = globalOrientation * vertc
#            normal = og.Math.calculateBasicFaceNormal(verta, vertb, vertc)
#
#            result = og.Math.intersects(ray, verta, vertb, vertc, True, True)
#            #result = og.Math.intersects(ray, globalPosition + self.mVertexList[self.mIndexList[i]], globalPosition + self.mVertexList[self.mIndexList[i+1]],
#            #                                                                                                                                              globalPosition + self.mVertexList[self.mIndexList[i+2]], normal,  True, True)
#
#            if result.first:
#                #print "dbg: Treffer!!!!!!!!!"
#                return True
#
#            i += 3
#
#        return False
#
#    def rayCastToPolygonLevelOnCurrentSelection(self):
#        if len(self.selectionList) >= 1:
#            self.getMeshInformation(self.selectionList[self.currentSelected].entity)
#
#            i = 0
#            while i <= (len(self.mIndexList) - 3):
#                globalPosition = self.selectionList[self.currentSelected].entity.getParentNode().getPosition()
#                result = og.Math.intersects(self.lastRay, globalPosition + self.mVertexList[self.mIndexList[i]], globalPosition + self.mVertexList[self.mIndexList[i+1]],
#                                                                                                                                                          globalPosition + self.mVertexList[self.mIndexList[i+2]], True, True)
#
#                if result.first:
#                   return True
#
#                i += 3
#            return False
