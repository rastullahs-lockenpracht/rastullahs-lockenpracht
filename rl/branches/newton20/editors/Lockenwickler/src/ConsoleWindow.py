#################################################
 # Copyright (C) 2008  Stefan Stammberger
 #
 # This library is free software; you can redistribute it and/or
 # modify it under the terms of the GNU Lesser General Public
 # License as published by the Free Software Foundation; either
 # version 2.1 of the License, or (at your option) any later version.
 #
 # This library is distributed in the hope that it will be useful,
 # but WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 # Lesser General Public License for more details.
 #
 # You should have received a copy of the GNU Lesser General Public
 # License along with this library; if not, write to the Free Software
 # Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 #################################################


import sys
from PyQt4.QtCore import *
from PyQt4.QtGui import *

from ui_ConsoleWindow import *
import ogre.renderer.OGRE as og



class LockenLog(og.LogListener):
    def __init__(self, logFunction):
        og.LogListener.__init__(self)
        self.logFunction = logFunction

    def messageLogged(self, message, level, debug, logName):
        self.logFunction(str(message))

class ConsoleWindow(QtGui.QDialog):
    def __init__(self, filterLog, parent=None):
        QtGui.QDialog.__init__(self, parent)
        self.filterLog = filterLog

        self.consoleWindow = Ui_ConsoleWindow()
        self.consoleWindow.setupUi(self)
        self.consoleWindow.textEdit.setReadOnly(True)
        self.lockenLog = LockenLog(self.write)

        sys.stdout = self
        sys.stderr = self

        clearAction= QAction("Clear Window",  self)
        self.consoleWindow.textEdit.addAction(clearAction)
        self.consoleWindow.textEdit.setContextMenuPolicy(Qt.ActionsContextMenu)
        clearAction.setShortcut("Ctrl + R")
        self.connect(clearAction, SIGNAL("triggered()"), self.consoleWindow.textEdit.clear)


    def write(self, text):
        if self.filterLog:
            if text.isspace():
                return
            elif text.startswith("Error"):
                self.consoleWindow.textEdit.append(text)
                return
            elif text.startswith("OGRE EXCEPTION"):
                self.consoleWindow.textEdit.append(text)
                return
            elif text.startswith("dbg:"):
                self.consoleWindow.textEdit.append(text)
                return
        else:
            self.consoleWindow.textEdit.append(text)

    def clear(self):
        self.consoleWindow.textEdit.clear()
