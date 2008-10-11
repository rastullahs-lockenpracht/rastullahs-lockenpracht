import sys
import ogre.renderer.OGRE as og


class Pivot():
    def __init__(self,  sceneManager):
        self.sceneManager = sceneManager

        self.mode = None

        self.meshManager = og.MeshManager.getSingleton ()

        self.pivotNode = sceneManager.getRootSceneNode().createChildSceneNode("pivotNode")
        self.__createMovePivot()
        self.__createRotatePivot()
        self.__createScalePivot()
        self.hide()
        self.setMoveMode()

        self.moveDirection = None
        self.isTransforming = False
        self.selectionList = None

    def __createMovePivot(self):
        self.xMoveEntity = self.sceneManager.createEntity("EditorXArrow",  "Pivot_Arrow.mesh")
        self.xMoveEntity.setMaterialName("Lockenwickler_Pivot_X")
        self.xMoveNode = self.pivotNode.createChildSceneNode()
        self.xMoveNode.attachObject(self.xMoveEntity)
        self.xMoveNode.translate(og.Vector3(2, 0, 0))
        self.xMoveNode.rotate(og.Vector3.UNIT_Y,  og.Degree(90))

#        plane = og.Plane (og.Vector3.UNIT_X, 0)
#        self.meshManager.createPlane ('EditorXArrowSelectionPlane', 'General', plane, 5, 14, 1, 1, False, 1, 5, 5, (0, 0, 1))
#        self.xMoveEntitySelectionPlane = self.sceneManager.createEntity ('EditorXArrowSelectionPlane', 'EditorXArrowSelectionPlane')
#        self.xMoveNodeSelectionPlane = self.xMoveNode.createChildSceneNode()
#        self.xMoveNodeSelectionPlane.attachObject (self.xMoveEntitySelectionPlane)
#        self.xMoveNodeSelectionPlane.translate(og.Vector3(0, 0, 5))
#        self.xMoveEntitySelectionPlane.setMaterialName("Lockenwickler_Pivot_X")



        self.yMoveEntity = self.sceneManager.createEntity("EditorYArrow",  "Pivot_Arrow.mesh")
        self.yMoveEntity.setMaterialName("Lockenwickler_Pivot_Y")
        self.yMoveNode = self.pivotNode.createChildSceneNode()
        self.yMoveNode.attachObject(self.yMoveEntity)
        self.yMoveNode.translate(og.Vector3(0, 2, 0))
        self.yMoveNode.rotate(og.Vector3.UNIT_X,  og.Degree(-90))
        #self.yNode.showBoundingBox(True)

#        plane = og.Plane (og.Vector3.UNIT_X, 0)
#        self.meshManager.createPlane ('EditorYArrowSelectionPlane', 'General', plane, 14, 0, 1, 1, False, 1, 5, 5, (0, 0, 1))
#        self.yMoveEntitySelectionPlane = self.sceneManager.createEntity ('EditorYArrowSelectionPlane', 'EditorYArrowSelectionPlane')
#        self.yMoveNodeSelectionPlane = self.xMoveNode.createChildSceneNode()
#        self.yMoveNodeSelectionPlane.attachObject (self.yMoveEntitySelectionPlane)
#        self.yMoveNodeSelectionPlane.translate(og.Vector3(0, 5, 0))
#        self.yMoveEntitySelectionPlane.setMaterialName("Lockenwickler_Pivot_Y")


        self.zMoveEntity = self.sceneManager.createEntity("EditorZArrow",  "Pivot_Arrow.mesh")
        self.zMoveEntity.setMaterialName("Lockenwickler_Pivot_Z")
        self.zMoveNode = self.pivotNode.createChildSceneNode()
        self.zMoveNode.attachObject(self.zMoveEntity)
        self.zMoveNode.translate(og.Vector3(0, 0, 2))
        #self.zNode.showBoundingBox(True)

        self.freeMoveEntity = self.sceneManager.createEntity("EditorFreeMoveArrow",  "Pivot_FreeMover.mesh")
        self.freeMoveEntity.setMaterialName("Lockenwickler_FreeMover")
        self.freeMoveNode = self.pivotNode.createChildSceneNode()
        self.freeMoveNode.attachObject(self.freeMoveEntity)


    def __createRotatePivot(self):
        self.xRotateEntity = self.sceneManager.createEntity("EditorXRotator",  "Rotate_Torus.mesh")
        self.xRotateEntity.setMaterialName("Lockenwickler_Pivot_X")
        self.xRotateNode = self.pivotNode.createChildSceneNode()
        self.xRotateNode.attachObject(self.xRotateEntity)
        #self.xRotateNode.translate(0, 0, -5)
        self.xRotateNode.rotate(og.Vector3.UNIT_Y,  og.Degree(90))

        self.yRotateEntity = self.sceneManager.createEntity("EditorYRotator",  "Rotate_Torus.mesh")
        self.yRotateEntity.setMaterialName("Lockenwickler_Pivot_Y")
        self.yRotateNode = self.pivotNode.createChildSceneNode()
        self.yRotateNode.attachObject(self.yRotateEntity)
        #self.yRotateNode.translate(0, 0, -10)
        self.yRotateNode.rotate(og.Vector3.UNIT_X,  og.Degree(90))

        self.zRotateEntity = self.sceneManager.createEntity("EditorZRotator",  "Rotate_Torus.mesh")
        self.zRotateEntity.setMaterialName("Lockenwickler_Pivot_Z")
        self.zRotateNode = self.pivotNode.createChildSceneNode()
        self.zRotateNode.attachObject(self.zRotateEntity)

        pass

    def __createScalePivot(self):
        pass

    def setPosition(self,  pos):
        self.pivotNode.setPosition(pos)

    def startTransforming(self, dirEntity, soList):
        print "dbg: transforming....."
        self.moveDirection = dirEntity.getName()
        self.selectionList = soList
        self.isTransforming = True
        pass

    def stopTransforming(self):
        self.isTransforming = False
        pass

    def hide(self):
        self.pivotNode.removeAllChildren()

    def show(self):
        self.hide()
        if self.mode == 1:
            self.pivotNode.addChild(self.xMoveNode)
            self.pivotNode.addChild(self.yMoveNode)
            self.pivotNode.addChild(self.zMoveNode)
        elif self.mode == 2:
            self.pivotNode.addChild(self.xRotateNode)
            self.pivotNode.addChild(self.yRotateNode)
            self.pivotNode.addChild(self.zRotateNode)
        elif self.mode == 3:
            pass

    def setMoveMode(self):
        self.hide()
        self.mode = 1
        self.pivotNode.addChild(self.xMoveNode)
        self.pivotNode.addChild(self.yMoveNode)
        self.pivotNode.addChild(self.zMoveNode)
        pass

    def setRotateMode(self):
        self.hide()
        self.mode = 2
        self.pivotNode.addChild(self.xRotateNode)
        self.pivotNode.addChild(self.yRotateNode)
        self.pivotNode.addChild(self.zRotateNode)
        pass

    def settScaleMode(self):
        pass

    def onMouseMoved(self, globalX, globalY, incX, incY):
        # move mode
        if self.mode == 1:
            transVec = None
            if self.moveDirection == "EditorXArrow":
                transVec = og.Vector3(-incX, 0.0 , 0.0)
            elif self.moveDirection == "EditorYArrow":
                transVec = og.Vector3(0.0, -incY, 0.0)
            elif self.moveDirection == "EditorZArrow":
                transVec = og.Vector3(0.0, 0.0, -incX)

            for so in self.selectionList:
                so.entity.getParentNode().translate(transVec)

            self.pivotNode.translate(transVec)

        # rotate mode
        elif self.mode == 2:
            rotValue = (incX + incY) * 0.05

            if self.moveDirection == "EditorXRotator":
                for so in self.selectionList:
                    so.entity.getParentNode().pitch(rotValue)
            if self.moveDirection == "EditorYRotator":
                for so in self.selectionList:
                    so.entity.getParentNode().yaw(rotValue)
            if self.moveDirection == "EditorZRotator":
                for so in self.selectionList:
                    so.entity.getParentNode().roll(rotValue)

        pass
