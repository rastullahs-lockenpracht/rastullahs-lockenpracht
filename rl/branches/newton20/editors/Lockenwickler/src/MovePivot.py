import sys

class MovePivot():
    def __init__(self,  sceneManager):
        self.sceneManager = sceneManager
        self.moveX = False
        self.moveY = False
        self.moveZ = False

    # set true if the mouse input should be processed or not
    def activated(self,  moveX,  moveY,  moveZ):
        self.moveX = moveX
        self.moveY = moveY
        self.moveZ = moveZ

    def move(self):
        pass
