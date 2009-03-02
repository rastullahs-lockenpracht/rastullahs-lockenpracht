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
import ogre.renderer.OGRE as og

class Pivot():
    def __init__(self,  sceneManager):
        self.sceneManager = sceneManager
        self.camera = self.sceneManager.getCamera("MainCam")

        self.mode = None
        self.isHidden = True

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
        self.xMoveEntity.setRenderQueueGroup(og.RENDER_QUEUE_OVERLAY - 1)
        self.xMoveNode = self.pivotNode.createChildSceneNode()
        self.xMoveNode.attachObject(self.xMoveEntity)
        self.xMoveNode.translate(og.Vector3(2, 0, 0))
        self.xMoveNode.rotate(og.Vector3().UNIT_Y,  og.Degree(90))
        
        self.yMoveEntity = self.sceneManager.createEntity("EditorYArrow",  "Pivot_Arrow.mesh")
        self.yMoveEntity.setMaterialName("Lockenwickler_Pivot_Y")
        self.yMoveEntity.setRenderQueueGroup(og.RENDER_QUEUE_OVERLAY - 1)
        self.yMoveNode = self.pivotNode.createChildSceneNode()
        self.yMoveNode.attachObject(self.yMoveEntity)
        self.yMoveNode.translate(og.Vector3(0, 2, 0))
        self.yMoveNode.rotate(og.Vector3().UNIT_X,  og.Degree(-90))


        self.zMoveEntity = self.sceneManager.createEntity("EditorZArrow",  "Pivot_Arrow.mesh")
        self.zMoveEntity.setMaterialName("Lockenwickler_Pivot_Z")
        self.zMoveEntity.setRenderQueueGroup(og.RENDER_QUEUE_OVERLAY - 1)
        self.zMoveNode = self.pivotNode.createChildSceneNode()
        self.zMoveNode.attachObject(self.zMoveEntity)
        self.zMoveNode.translate(og.Vector3(0, 0, 2))


    def __createRotatePivot(self):
        self.xRotateEntity = self.sceneManager.createEntity("EditorXRotator",  "Rotate_Torus.mesh")
        self.xRotateEntity.setMaterialName("Lockenwickler_Pivot_X")
        self.xRotateEntity.setRenderQueueGroup(og.RENDER_QUEUE_OVERLAY - 1)
        self.xRotateNode = self.pivotNode.createChildSceneNode()
        self.xRotateNode.attachObject(self.xRotateEntity)
        #self.xRotateNode.translate(0, 0, -5)
        self.xRotateNode.rotate(og.Vector3().UNIT_Y,  og.Degree(90))

        self.yRotateEntity = self.sceneManager.createEntity("EditorYRotator",  "Rotate_Torus.mesh")
        self.yRotateEntity.setMaterialName("Lockenwickler_Pivot_Y")
        self.yRotateEntity.setRenderQueueGroup(og.RENDER_QUEUE_OVERLAY - 1)
        self.yRotateNode = self.pivotNode.createChildSceneNode()
        self.yRotateNode.attachObject(self.yRotateEntity)
        #self.yRotateNode.translate(0, 0, -10)
        self.yRotateNode.rotate(og.Vector3().UNIT_X,  og.Degree(90))

        self.zRotateEntity = self.sceneManager.createEntity("EditorZRotator",  "Rotate_Torus.mesh")
        self.zRotateEntity.setMaterialName("Lockenwickler_Pivot_Z")
        self.zRotateEntity.setRenderQueueGroup(og.RENDER_QUEUE_OVERLAY - 1)
        self.zRotateNode = self.pivotNode.createChildSceneNode()
        self.zRotateNode.attachObject(self.zRotateEntity)


    def __createScalePivot(self):
        self.xScaleEntity = self.sceneManager.createEntity("EditorXScaler",  "Pivot_Arrow.mesh")
        self.xScaleEntity.setMaterialName("Lockenwickler_Pivot_X")
        self.xScaleEntity.setRenderQueueGroup(og.RENDER_QUEUE_OVERLAY - 1)
        self.xScaleNode = self.pivotNode.createChildSceneNode()
        self.xScaleNode.attachObject(self.xScaleEntity)
        self.xScaleNode.translate(og.Vector3(2, 0, 0))
        self.xScaleNode.rotate(og.Vector3().UNIT_Y,  og.Degree(90))
        
        self.yScaleEntity = self.sceneManager.createEntity("EditorYScaler",  "Pivot_Arrow.mesh")
        self.yScaleEntity.setMaterialName("Lockenwickler_Pivot_Y")
        self.yScaleEntity.setRenderQueueGroup(og.RENDER_QUEUE_OVERLAY - 1)
        self.yScaleNode = self.pivotNode.createChildSceneNode()
        self.yScaleNode.attachObject(self.yScaleEntity)
        self.yScaleNode.translate(og.Vector3(0, 2, 0))
        self.yScaleNode.rotate(og.Vector3().UNIT_X,  og.Degree(-90))


        self.zScaleEntity = self.sceneManager.createEntity("EditorZScaler",  "Pivot_Arrow.mesh")
        self.zScaleEntity.setMaterialName("Lockenwickler_Pivot_Z")
        self.zScaleEntity.setRenderQueueGroup(og.RENDER_QUEUE_OVERLAY - 1)
        self.zScaleNode = self.pivotNode.createChildSceneNode()
        self.zScaleNode.attachObject(self.zScaleEntity)
        self.zScaleNode.translate(og.Vector3(0, 0, 2))

    def setPosition(self,  pos):
        self.pivotNode.setPosition(pos)

    def getPosition(self):
        return self.pivotNode.getPosition()

    def startTransforming(self, dirEntity, soList):
        self.moveDirection = dirEntity.getName()
        self.selectionList = soList
        self.isTransforming = True
        pass

    def stopTransforming(self):
        self.isTransforming = False
        pass

    def hide(self):
        self.pivotNode.removeAllChildren()
        self.isHidden = True

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
            return
        self.isHidden = False

    def setMoveMode(self):
        self.hide()
        self.mode = 1
        self.pivotNode.addChild(self.xMoveNode)
        self.pivotNode.addChild(self.yMoveNode)
        self.pivotNode.addChild(self.zMoveNode)
    
    def setRotateMode(self):
        self.hide()
        self.mode = 2
        self.pivotNode.addChild(self.xRotateNode)
        self.pivotNode.addChild(self.yRotateNode)
        self.pivotNode.addChild(self.zRotateNode)
    
    def setScaleMode(self):
        self.hide()
        self.mode = 3
        self.pivotNode.addChild(self.xScaleNode)
        self.pivotNode.addChild(self.yScaleNode)
        self.pivotNode.addChild(self.zScaleNode)
        
    def onMouseMoved(self, globalX, globalY, incX, incY):
        # move mode
        if self.isTransforming:
            if self.mode == 1:
                transFactor = 0.1
                transVec = og.Vector3()
                if self.moveDirection == "EditorXArrow":
                    transVec = og.Vector3(-incX, 0.0 , 0.0)
                elif self.moveDirection == "EditorYArrow":
                    transVec = og.Vector3(0.0, -incY, 0.0)
                elif self.moveDirection == "EditorZArrow":
                    transVec = og.Vector3(0.0, 0.0, incX)

                transVec = transVec * transFactor
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
            
            # scale mode
            elif self.mode == 3:
                scaleFactor = 0.3
                if self.moveDirection == "EditorXScaler":
                    for so in self.selectionList:
                        scale = so.entity.getParentNode().getScale() + og.Vector3(incX * scaleFactor, 0, 0)
                        so.entity.getParentNode().setScale(scale)
                if self.moveDirection == "EditorYScaler":
                    for so in self.selectionList:
                        scale = so.entity.getParentNode().getScale() + og.Vector3(0, incY * scaleFactor, 0)
                        so.entity.getParentNode().setScale(scale)
                if self.moveDirection == "EditorZScaler":
                    for so in self.selectionList:
                        scale = so.entity.getParentNode().getScale() + og.Vector3(0, 0, incX * scaleFactor)
                        so.entity.getParentNode().setScale(scale)
        
        self.update()

    def update(self):
        vSize = og.Vector3(1.0,1.0,1.0)
        vScale = og.Vector3(1.0,1.0,1.0)

        dist = (self.camera.getDerivedPosition() - self.pivotNode._getDerivedPosition()).length()
        vScale *= dist / 90.0

        self.pivotNode.setScale(vScale.x * vSize.x,vScale.y * vSize.y,vScale.z * vSize.z)
        #print vScale.x * vSize.x,vScale.y * vSize.y,vScale.z * vSize.z
#        if not self.isHidden:
#            dist = self.camera.getPosition().distance(self.pivotNode.getPosition())
#            self.pivotNode.setScale(og.Vector3(0.5,  0.5,  0.5) * (dist / 30))


