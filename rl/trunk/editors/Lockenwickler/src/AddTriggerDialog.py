# -*- coding: utf-8 -*-

"""
Module implementing AddTriggerDialog.
"""

from PyQt4.QtGui import QDialog
from PyQt4.QtCore import pyqtSignature

from Ui_AddTriggerDialog import Ui_Dialog

class AddTriggerDialog(QDialog, Ui_Dialog):
    def __init__(self, parent = None):

        QDialog.__init__(self, parent)
        self.setupUi(self)
    
    @pyqtSignature("QString")
    def on_lineEdit_textChanged(self, p0):
        """
        Slot documentation goes here.
        """
        # TODO: not implemented yet
        raise NotImplementedError
    
    @pyqtSignature("")
    def on_pushButton_clicked(self):
        """
        Slot documentation goes here.
        """
        # TODO: not implemented yet
        raise NotImplementedError
