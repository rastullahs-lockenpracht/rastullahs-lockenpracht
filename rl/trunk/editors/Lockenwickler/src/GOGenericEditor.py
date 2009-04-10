 #################################################
# This source file is part of Rastullahs Lockenwickler.
# Copyright (C) 2003-2009 Team Pantheon. http://www.team-pantheon.de
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  US
 #################################################


import sys
from PyQt4.QtCore import *
from PyQt4.QtGui import *

class GOGenericEditor(QDialog):
    def __init__(self, description = "", type = "STRING", data = "", parent=None):
        QDialog.__init__(self, parent)
        
        self.layout = QVBoxLayout()
        
        self.nameEditBox = QLineEdit(self)
        self.nameEditBox.setText(description)
        self.layout.addWidget(self.nameEditBox)
        
        self.createTypeDropBox(type)
        
        if type == "STRING":
            self.dataEditBox = QLineEdit(self)
            self.dataEditBox.setText(data)
            self.layout.addWidget(self.dataEditBox)
        elif type == "REAL":
            self.dataEditBox = QLineEdit(self)
            self.dataEditBox.setText(data)
            self.dataEditBox.inputMask("9.0")
            self.layout.addWidget(self.dataEditBox)
#        elif type == "BOOL":
#            self.typeDropBox.setCurrentIndex(2)
#        elif type == "INT":
#            self.typeDropBox.setCurrentIndex(3)
#        elif type == "INTPAIR":
#            self.typeDropBox.setCurrentIndex(4)
#        elif type == "INTTRIPLE":
#            self.typeDropBox.setCurrentIndex(5)
        
        btnBox = QDialogButtonBox(QDialogButtonBox.Ok | QDialogButtonBox.Cancel)
        self.connect(btnBox, SIGNAL("accepted()"), self.accept)
        self.connect(btnBox, SIGNAL("rejected()"), self.reject)
        self.layout.addWidget(btnBox)
        
        self.setLayout(self.layout)
        
        
    def createTypeDropBox(self, type):
        self.typeDropBox = QComboBox(self)
        self.typeDropBox.addItem("STRING")
        self.typeDropBox.addItem("REAL")
        self.typeDropBox.addItem("BOOL")
        self.typeDropBox.addItem("INT")
        self.typeDropBox.addItem("INTPAIR")
        self.typeDropBox.addItem("INTTRIPLE")
        
        if type == "STRING":
            self.typeDropBox.setCurrentIndex(0)
        elif type == "REAL":
            self.typeDropBox.setCurrentIndex(1)
        elif type == "BOOL":
            self.typeDropBox.setCurrentIndex(2)
        elif type == "INT":
            self.typeDropBox.setCurrentIndex(3)
        elif type == "INTPAIR":
            self.typeDropBox.setCurrentIndex(4)
        elif type == "INTTRIPLE":
            self.typeDropBox.setCurrentIndex(5)
            
        self.layout.addWidget(self.typeDropBox)
        
        self.connect(self, SIGNAL("currentIndexChanged ( const QString & text )"), self.onTypeChanged)

    def onTypeChanged(self, type):
        print str(type)
        return
        
        
        
        
        
        
        
        
        
        
        
        
        
