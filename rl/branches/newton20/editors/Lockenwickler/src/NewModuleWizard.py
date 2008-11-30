# -*- coding: utf-8 -*-

"""
Module implementing NewModuleWizard.
"""

from PyQt4.QtGui import QDialog
from PyQt4.QtCore import pyqtSignature

from Ui_NewModuleWizard import Ui_Dialog

class NewModuleWizard(QDialog, Ui_Dialog):
    """
    Class documentation goes here.
    """
    def __init__(self, moduleManager, parent = None):
        """
        Constructor
        """

        self.moduleManager = moduleManager

        QDialog.__init__(self, parent)
        self.setupUi(self)

    @pyqtSignature("QString")
    def on_moduleNameLineEdit_textChanged(self, p0):
        return
        # TODO: not implemented yet
        #raise NotImplementedError

    @pyqtSignature("")
    def on_moduleNameLineEdit_returnPressed(self):

        # TODO: not implemented yet
        raise NotImplementedError

    @pyqtSignature("")
    def on_okButton_clicked(self):

        # TODO: not implemented yet
        raise NotImplementedError

    @pyqtSignature("")
    def on_cancelButton_pressed(self):
        self.close()


