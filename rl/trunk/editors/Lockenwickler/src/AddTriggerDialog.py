# -*- coding: utf-8 -*-

"""
Module implementing AddTriggerDialog.
"""

from PyQt4.QtGui import QDialog
from PyQt4.QtCore import pyqtSignature


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
        if self.trigger.addProperty():
            self.updateProperties()
        
    def accept(self):
        self.trigger.name = str(self.lineEdit.text())
        self.trigger.className = str(self.comboBox.currentText())
        self.close()
        
    def updateProperties(self):
        self.listView.clear()
        for prop in self.trigger.properties:
            self.listView.addItem(prop.name + "  " + prop.type + "  " + prop.data)
