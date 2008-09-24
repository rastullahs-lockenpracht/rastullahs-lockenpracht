import sys
from PyQt4.QtCore import *
from PyQt4.QtGui import *
import ogre.renderer.OGRE as og

class SceneExplorer(QWidget):
    def __init__(self, parent=None):
        QWidget.__init__(self, parent)
        self.sceneTreeView = QTreeView()

        vBoxLayout = QVBoxLayout()
        vBoxLayout.addWidget(self.sceneTreeView)

        self.setLayout(vBoxLayout)

