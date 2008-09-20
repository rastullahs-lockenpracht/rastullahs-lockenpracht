import sys
import xml.dom.minidom as xml

import ctypes
import ogre.renderer.OGRE as og

from MovePivot import *

# a class to store information about a object that got selected
class SelectionObject():
    def __init__(self,  entity,  distance):
        self.entity = entity #the selected entity
        self.distance = distance # the distance from camera at the time of selection
        self.isPivot = False

    #if True this instance will show its bounding box else it will hide it
    def setSelected(self,  selected):
        if selected == True:
            self.entity.getParentNode().showBoundingBox(True)
        else:
            self.entity.getParentNode().showBoundingBox(False)

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
        print entity.getName()
        if distance == 0.0: #camera is in the bounding box, ignore this selection
            return True
        elif entity.getName() == "rayLine" :
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
        for so in self.selectionList:
            so.setSelected(False)

        self.previousSelected = -1
        self.currentSelected = -1

        self.selectionList = []

    def iterateEntityUnderMouse(self):
        self.previousSelected = self.currentSelected
        if len(self.selectionList) >= self.currentSelected: # would mean we are out of bounds
            self.selectionList[self.currentSelected].setSelected(False)

        self.currentSelected += 1

        if len(self.selectionList) == self.currentSelected: # means we are out of bounds and reached the end of the list, reset it to zero
            self.currentSelected = 0

        if len(self.selectionList) >= self.currentSelected: # would mean we are out of bounds
            #print str(self.selectionList[self.currentSelected].distance) + " "  + self.selectionList[self.currentSelected].entity.getName()
            if self.rayCastToPolygonLevelOnCurrentSelection():
                self.selectionList[self.currentSelected].setSelected(True)
                return self.selectionList[self.currentSelected]


    def getMeshInformation(self,  entity):
        numVertices = 0
        numIndices = 0
        useSharedVertices = False

        if not entity:
            return False

        pMesh = entity.getMesh()

        pos =    entity.getParentNode().getWorldPosition()
        scale =  entity.getParentNode().getScale()
        orient = entity.getParentNode().getWorldOrientation()



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
                print str(ih) + ": "  +  str(blah)
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
        self.getMeshInformation(entity)
        globalPosition = entity.getParentNode().getWorldPosition()

        i = 0
        while i <= (len(self.mIndexList) - 3):
            result = og.Math.intersects(ray, globalPosition + self.mVertexList[self.mIndexList[i]], globalPosition + self.mVertexList[self.mIndexList[i+1]],
                                                                                                                                                          globalPosition + self.mVertexList[self.mIndexList[i+2]], True, False)

            if result.first:
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
        self.rayLine = None

        self.pivot = None
        self.movingPivot = False

        self.leftMouseDown = False
        self.middleMouseDown = False
        self.rightMouseDown = False

    def load(self,  moduleName,  mapFiles):
        self.moduleName = moduleName
        self.mapFiles = mapFiles

        for a in self.mapFiles:
            doc = xml.parse(a)
            node = doc.getElementsByTagName("entity")
            if node != None:
                self.parseSceneNodes(node)

        self.pivot = Pivot(self.sceneManager)

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

            n = self.sceneManager.getRootSceneNode().createChild()
            n.attachObject(e)
            n.setPosition(nodePosition)
            n.setOrientation(nodeRotation)
            n.setScale(nodeScale)

        pass

    # called when a click into Main Ogre Window occurs
    def selectionClick(self,  ray,  controlDown=False,  shiftDown=False):
        self.lastRay = ray
        self.listenerDings.currentRay = ray
        self.raySceneQuery.Ray = ray
        self.raySceneQuery.execute(self.listenerDings)

        so = self.listenerDings.rayCastToPolygonLevel(ray)
        if so is not None:
            if not so.isPivot:
                self.pivot.show()
                if not controlDown and not shiftDown:
                    self.resetSelection()
                    so.setSelected(True)
                    self.userSelectionList.append(so)
                    self.updatePivots()
                elif controlDown and not shiftDown:
                    so.setSelected(True)
                    self.userSelectionList.append(so)
                    self.updatePivots()
                elif not controlDown and shiftDown:
                    for selo in self.userSelectionList:
                        if so.entity.getName() == selo.entity.getName():
                            so.setSelected(False)
                            self.userSelectionList.remove(selo)
                    self.updatePivots()
            else:
                print "isPivot: " + str(so.entity.getName())
                self.pivot.startTransforming(so.entity,  self.userSelectionList)
        else:
            print "noneee"
            self.resetSelection() # click in empty space, deselect everything
            self.pivot.hide()

        if self.rayLine == None:
            self.rayLine = self.sceneManager.createManualObject("rayLine")
            self.rayLine.setDynamic(True)
            self.sceneManager.getRootSceneNode().createChildSceneNode("raynode").attachObject(self.rayLine)

            self.rayLine.begin("BaseWhiteNoLighting", og.RenderOperation.OT_LINE_STRIP)

            self.rayLine.position(ray.getOrigin())
            self.rayLine.position( ray.getPoint(10000))

            self.rayLine.end()

        else:
            self.rayLine.beginUpdate(0)

            self.rayLine.position(ray.getOrigin())
            self.rayLine.position( ray.getPoint(10000))

            self.rayLine.end()

    def leftMouseUp(self):
        if self.pivot.isTransforming:
            self.pivot.stopTransforming()

    def iterateEntityUnderMouse(self):
        self.listenerDings.iterateEntityUnderMouse()

        pass

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

        self.pivot.setPosition(newPivotPosition / len(self.userSelectionList))

    def unload(self,  saveOnUnload=True):
        pass

    def save(self):
        pass



