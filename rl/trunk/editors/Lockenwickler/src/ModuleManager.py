import sys
import xml.dom.minidom as xml

import ctypes
import ogre.renderer.OGRE as og

# a class to store information about a object that got selected
class SelectionObject():
    def __init__(self,  entity,  distance):
        self.entity = entity #the selected entity
        self.distance = distance # the distance from camera at the time of selection

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
        if distance == 0.0: #camera is in the bounding box, ignore this selection
            return True
        elif entity.getName() == "rayLine" :
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
            test=storageclass(1.1)
#         mVertexList = new Point[numVertices];
#         mIndexList = new unsigned int[numIndices];

        self.mNormalList = []
        self.mVertexList = []
        self.mIndexList = []

        ## Count the number of vertices and incides so we can Set them
        indexCount = 0
        vertListCount = 0

        if useSharedVertices:
            ## Real* pVertices (x, y, z order x numVertices)
            elem = pMesh.sharedVertexData.vertexDeclaration.findElementBySemantic(og.VES_POSITION)
            elemNormal = pMesh.sharedVertexData.vertexDeclaration.findElementBySemantic(og.VES_NORMAL)

            if not elem:
                ogre.Except(Exception.ERR_ITEM_NOT_FOUND, "Can't find position elements in the "
                    "mesh to be written!", "MeshSerializerImpl.writeGeometry")

            vbuf = pMesh.sharedVertexData.vertexBufferBinding.getBuffer(elem.getSource())
            vbufNormal = pMesh.sharedVertexData.vertexBufferBinding.getBuffer(elemNormal.getSource())

            ## need space for the 3 verticies
            storageclass = ctypes.c_float * 3
            test=storageclass(1.1)

            storageclass = ctypes.c_float * 3
            testNormal=storageclass(1.1)

            for j in range ( pMesh.sharedVertexData.vertexCount ):
                #vbufNormal.readData(j * vbufNormal.getVertexSize(), 3 * ctypes.sizeof(ctype.c_float), ctypes.addressof(testNormal))
                vbufNormal.readData(j * 3, 3 * ctypes.sizeof(ctype.c_float), ctypes.addressof(testNormal))
                self.mNormalList.append( og.Vector3(testNormal[0], testNormal[1],  testNormal[2]) )

                vbuf.readData(j * vbuf.getVertexSize(), 3 * ctypes.sizeof(ctype.c_float), ctypes.addressof(test))
                self.mVertexList.append( og.Vector3(test[0], test[1], test[2]) )
                vertListCount+=1

        for i in range ( pMesh.getNumSubMeshes() ):
            pSubMesh = pMesh.getSubMesh(i)
            if not pSubMesh.useSharedVertices:
                ## Real* pVertices (x, y, z order x numVertices)
                elem = pSubMesh.vertexData.vertexDeclaration.findElementBySemantic(og.VES_POSITION)
                elemNormal = pSubMesh.vertexData.vertexDeclaration.findElementBySemantic(og.VES_NORMAL)

                if not elem or not elemNormal:
                    og.Except(Exception.ERR_ITEM_NOT_FOUND, "Can't find position elements in the "
                        "mesh to be written!", "MeshSerializerImpl.writeGeometry")

                vbuf = pSubMesh.vertexData.vertexBufferBinding.getBuffer(elem.getSource())
                vbufNormal = pSubMesh.vertexData.vertexBufferBinding.getBuffer(elemNormal.getSource())

                ## need space for the verticies
                storageclass = ctypes.c_float * (pSubMesh.vertexData.vertexCount * 3)
                test=storageclass(1.1)

                storageclassNormal = ctypes.c_float * (pSubMesh.vertexData.vertexCount * 3)
                testNormal=storageclassNormal(1.1)

                vbuf.readData(0, pSubMesh.vertexData.vertexCount * 3 * ctypes.sizeof(ctypes.c_float),
                                                                                ctypes.addressof(test))
                vbufNormal.readData(0, pSubMesh.vertexData.vertexCount * 3 * ctypes.sizeof(ctypes.c_float),
                                                                                ctypes.addressof(testNormal))

                for j in range ( pSubMesh.vertexData.vertexCount ):
                    self.mVertexList.append( og.Vector3(test[j], test[j+1], test[j+2]) )
                    self.mNormalList.append( og.Vector3(testNormal[j], testNormal[j+1], testNormal[j+2]) )
                    vertListCount += 1

            ibuf = pSubMesh.indexData.indexBuffer
            ## need space for the verticies
            storageclass = ctypes.c_ushort * pSubMesh.indexData.indexCount
            test2=storageclass()

            ibuf.readData(0, ibuf.getSizeInBytes(), ctypes.addressof(test2))

            for j in range ( pSubMesh.indexData.indexCount ):
                self.mIndexList.append (test2[j])   # unsigned short
                indexCount += 1

    # used when a new selection is made, meaning when not iterationg through the selected objects
    def rayCastToPolygonLevel(self,  ray):
        self.lastRay = ray
        if len(self.selectionList) >= 1:
            for so in self.selectionList:
                self.getMeshInformation(so.entity)

                i = 0
                while i <= (len(self.mIndexList) - 3):
                    globalPosition = so.entity.getParentNode().getPosition()
                    result = og.Math.intersects(ray, globalPosition + self.mVertexList[self.mIndexList[i]], globalPosition + self.mVertexList[self.mIndexList[i+1]],
                                                                                                                                                              globalPosition + self.mVertexList[self.mIndexList[i+2]], True, True)

                    if result.first:
                        return so
#                        self.previousSelected = self.currentSelected
#
#                        if self.previousSelected is not -1:
#                            self.selectionList[self.previousSelected].setSelected(False)
#
#                        self.currentSelected = self.selectionList.index(so)
#                        so.setSelected(True)
#                        return
                    i += 3

#        print "fertsch #####################################################################################"

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

    def load(self,  moduleName,  mapFiles):
        self.moduleName = moduleName
        self.mapFiles = mapFiles

        for a in self.mapFiles:
            doc = xml.parse(a)
            node = doc.getElementsByTagName("entity")
            if node != None:
                self.parseSceneNodes(node)


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

            e = self.sceneManager.createEntity(entityName, meshFile)
            n = self.sceneManager.getRootSceneNode().createChild()
            n.attachObject(e)
            n.setPosition(nodePosition)
            n.setOrientation(nodeRotation)
            n.setScale(nodeScale)

        pass


    def selectionClick(self,  ray,  controlDown=False,  shiftDown=False):
        if not controlDown and not shiftDown:
            self.resetSelection()

        self.lastRay = ray
        self.raySceneQuery.Ray = ray
        self.raySceneQuery.execute(self.listenerDings)


        so = self.listenerDings.rayCastToPolygonLevel(ray)
        if so is not None and not controlDown and not shiftDown:
            so.setSelected(True)
            self.userSelectionList.append(so)
        elif so is not None and controlDown and not shiftDown:
            so.setSelected(True)
            self.userSelectionList.append(so)
        elif so is not None and not controlDown and shiftDown:
            for selo in self.userSelectionList:
                if so.entity.getName() == selo.entity.getName():
                    so.setSelected(False)
                    self.userSelectionList.remove(selo)

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

    def iterateEntityUnderMouse(self):
        self.listenerDings.iterateEntityUnderMouse()

        pass

    def resetSelection(self):
        for so in self.userSelectionList:
            so.setSelected(False)

        self.userSelectionList = []

        self.listenerDings.reset()
        pass

    def unload(self,  saveOnUnload=True):
        pass

    def save(self):
        pass



