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

from PyQt4 import QtCore, QtGui

class Ui_ConsoleWindow(object):
    def setupUi(self, ConsoleWindow):
        ConsoleWindow.setObjectName("ConsoleWindow")
        ConsoleWindow.resize(QtCore.QSize(QtCore.QRect(0,0,884,225).size()).expandedTo(ConsoleWindow.minimumSizeHint()))

        self.gridlayout = QtGui.QGridLayout(ConsoleWindow)
        self.gridlayout.setObjectName("gridlayout")
        self.gridlayout.setContentsMargins(0, 0, 0, 0)
        
        self.textEdit = QtGui.QTextEdit(ConsoleWindow)
        self.textEdit.setObjectName("textEdit")
        self.gridlayout.addWidget(self.textEdit,0,0,1,1)

        self.retranslateUi(ConsoleWindow)
        QtCore.QMetaObject.connectSlotsByName(ConsoleWindow)

    def retranslateUi(self, ConsoleWindow):
        ConsoleWindow.setWindowTitle(QtGui.QApplication.translate("ConsoleWindow", "Form", None, QtGui.QApplication.UnicodeUTF8))

