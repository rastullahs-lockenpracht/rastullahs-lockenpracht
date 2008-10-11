import sys

from PyQt4.QtCore import *
from PyQt4.QtGui import *

class ObjectPropertyWin(QDialog):
    def __init__(self, parent=None):
        super(QDialog, self).__init__(parent)
        self.setupUi()

        self.connect(self.treeWidget, SIGNAL("itemDoubleClicked (QTreeWidgetItem *,int)"),
                               self.onItemClicked)


    def setupUi(self):
        self.setObjectName("ObjectPropertys")
        self.resize(QSize(QRect(0,0,761,724).size()).expandedTo(self.minimumSizeHint()))

        self.treeWidget = QTreeWidget(self)
        self.treeWidget.setGeometry(QRect(0,0,901,721))
        self.treeWidget.setMinimumSize(QSize(400,400))
        self.treeWidget.setObjectName("treeWidget")

        self.retranslateUi()
        QMetaObject.connectSlotsByName(self)

    def retranslateUi(self):
        self.setWindowTitle(QApplication.translate("ObjectPropertys", "Object Properties", None, QApplication.UnicodeUTF8))
        self.treeWidget.headerItem().setText(0,QApplication.translate("ObjectPropertys", "Property", None, QApplication.UnicodeUTF8))
        self.treeWidget.headerItem().setText(1,QApplication.translate("ObjectPropertys", "Value", None, QApplication.UnicodeUTF8))
        self.treeWidget.clear()

        item = QTreeWidgetItem(self.treeWidget)
        item.setText(0,QApplication.translate("ObjectPropertys", "Position", None, QApplication.UnicodeUTF8))
        item.setText(1,"")

        item1 = QTreeWidgetItem(item)
        item1.setText(0,QApplication.translate("ObjectPropertys", "X", None, QApplication.UnicodeUTF8))
        item1.setText(1,"")

        item2 = QTreeWidgetItem(item)
        item2.setText(0,QApplication.translate("ObjectPropertys", "Y", None, QApplication.UnicodeUTF8))
        item2.setText(1,"")

        item3 = QTreeWidgetItem(item)
        item3.setText(0,QApplication.translate("ObjectPropertys", "Z", None, QApplication.UnicodeUTF8))
        item3.setText(1,"")

    def onItemClicked(self, item, column):
        print item.text(0)
        print column
