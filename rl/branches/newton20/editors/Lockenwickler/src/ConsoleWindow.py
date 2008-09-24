import sys
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
        else:
            self.consoleWindow.textEdit.append(text)

