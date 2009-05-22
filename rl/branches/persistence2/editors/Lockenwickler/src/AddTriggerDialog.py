# -*- coding: utf-8 -*-

"""
Module implementing AddTriggerDialog.
"""

from PyQt4.QtCore import *
from PyQt4.QtGui import *

from Ui_AddTriggerDialog import Ui_Dialog

class AddTriggerDialog(QDialog, Ui_Dialog):
    def __init__(self, trigger, triggerClassNames, triggerManager, parent = None):
        QDialog.__init__(self, parent)
        self.setupUi(self)
        self.triggerManager = triggerManager
        self.trigger = trigger

        self.buttonBox.setEnabled(False)
        
        for trigger in triggerClassNames:
            self.comboBox.addItem(trigger)
        
        self.connect(self, SIGNAL("accepted()"), self.onAccepted)
        
    @pyqtSignature("QString")
    def on_lineEdit_textChanged(self, p0):
        for trigger in self.triggerManager.triggerInstances:
            if trigger.name is p0:
                self.buttonBox.setEnabled(False)
            else:
                self.buttonBox.setEnabled(True)
                
        if len(self.triggerManager.triggerInstances) == 0:
            self.buttonBox.setEnabled(True)
            
    @pyqtSignature("")
    def on_pushButton_clicked(self):
        if self.trigger.manualAddProperty():
            self.updateProperties()
        
    def onAccepted(self):
        self.trigger.name = str(self.lineEdit.text())
        self.trigger.className = str(self.comboBox.currentText())
        
    def updateProperties(self):
        self.listView.clear()
        for prop in self.trigger.properties:
            self.listView.addItem(prop.name + "  " + prop.type + "  " + prop.data)
