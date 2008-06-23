import sys
from ui_obj_prop import *

class ObjectPropertyWin(QtGui.QDialog):
    def __init__(self, parent=None):
        QtGui.QDialog.__init__(self, parent)
        
        self.propWin = Ui_ObjectPropertys()
        self.propWin.setupUi(self)
        
        QtCore.QObject.connect(self.propWin.treeWidget, QtCore.SIGNAL("itemDoubleClicked (QTreeWidgetItem *,int)"), 
                               self.onItemClicked)
        
        
    def onItemClicked(self, item, column):
        print item.text(0)
        print column