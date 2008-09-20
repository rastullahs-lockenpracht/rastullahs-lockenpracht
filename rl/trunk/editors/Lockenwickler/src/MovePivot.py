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
        self.moveDirection = dirEntity.getName()
        self.selectionList = soList
        self.isTransforming = True
        pass

    def stopTransforming(self):
        self.isTransforming = False
        pass

    def hide(self):
        self.pivotNode.setVisible(False)

    def show(self):
        if self.mode == 1:
            self.xMoveNode.setVisible(True)
            self.yMoveNode.setVisible(True)
            self.zMoveNode.setVisible(True)
            self.freeMoveNode.setVisible(True)
        elif self.mode == 2:
            self.xRotateNode.setVisible(True)
            self.yRotateNode.setVisible(True)
            self.zRotateNode.setVisible(True)
        elif self.mode == 3:
            pass

    def setMoveMode(self):
        self.hide()
        self.mode = 1
        self.xMoveNode.setVisible(True)
        self.yMoveNode.setVisible(True)
        self.zMoveNode.setVisible(True)
        self.freeMoveNode.setVisible(False)

        pass

    def setRotateMode(self):
        self.hide()
        self.mode = 2
        self.xRotateNode.setVisible(True)
        self.yRotateNode.setVisible(True)
        self.zRotateNode.setVisible(True)
        pass

    def __setScaleMode(self):
        pass

    def onMouseMoved(self, globalX, globalY, incX, incY):
        print self.moveDirection
        pass
