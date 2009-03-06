# -*- coding: utf-8 -*-

"""
Module implementing NewModuleWizard.
"""

import os
import glob

from PyQt4.QtGui import QDialog
from PyQt4.QtCore import pyqtSignature

from Ui_NewModuleWizard import Ui_Dialog

class NewModuleWizard(QDialog, Ui_Dialog):
    """
    Class documentation goes here.
    """
    def __init__(self, moduleManager, parent = None):
       
        QDialog.__init__(self, parent)
        self.setupUi(self)

        self.moduleManager = moduleManager
        self.moduleManager.parseModuleConfig()
        
        for m in self.moduleManager.moduleList:
            self.moduleDependenciesList.addItem(m.name)

    @pyqtSignature("QString")
    def on_moduleNameLineEdit_textChanged(self, p0):
        if not self.moduleManager.moduleExists(str(self.moduleNameLineEdit.text())):
            self.sceneNameLineEdit.setEnabled(True)
            self.mapNameLineEdit.setEnabled(True)
            self.moduleDependenciesList.setEnabled(True)
            self.scriptDependenciesList.setEnabled(True)
            self.okButton.setEnabled(True)
        else:
            self.sceneNameLineEdit.setEnabled(False)
            self.mapNameLineEdit.setEnabled(False)
            self.moduleDependenciesList.setEnabled(False)
            self.scriptDependenciesList.setEnabled(False)
            self.okButton.setEnabled(False)
    
    def scanDirForScripts(self, dir):
        for file in os.listdir(dir):
            curFile = dir + "/" + file

            if file.startswith('.'): #ignore dot files (hidden)
                continue
            if os.path.isdir(curFile):
                self.scanDirForScripts(curFile)
                continue
            if os.path.isfile(curFile):
                if file.endswith(".rb"):
                    self.scriptDependenciesList.addItem(dir + "/" + file)
    
    @pyqtSignature("")
    def on_moduleDependenciesList_itemSelectionChanged(self):
        self.scriptDependenciesList.clear()
        modulePath = self.moduleManager.moduleCfgPath.replace("modules.cfg",  "")
        
        for m in self.moduleDependenciesList.selectedItems():
            path = os.path.join(modulePath, str(m.text()))
            cmd = os.path.join(path + "/scripts")
            self.scanDirForScripts(cmd)
        
    @pyqtSignature("")
    def on_moduleNameLineEdit_returnPressed(self):
        return
            
    @pyqtSignature("")
    def on_mapNameLineEdit_returnPressed(self):
        return
        
    @pyqtSignature("")
    def on_okButton_clicked(self):
        f = open(self.moduleManager.moduleCfgPath, "a")
        f.write("module=" + str(self.moduleNameLineEdit.text()) + "\n")
        f.close()
        
        modulePath = self.moduleManager.moduleCfgPath.replace("modules.cfg",  "")
        modulePath = str(modulePath + self.moduleNameLineEdit.text())
        os.mkdir(modulePath)
        os.mkdir(os.path.join(modulePath, "dialogs"))
        os.mkdir(os.path.join(modulePath, "dsa"))
        os.mkdir(os.path.join(modulePath, "gui"))
        os.mkdir(os.path.join(modulePath, "maps"))
        os.mkdir(os.path.join(modulePath, "materials"))
        os.mkdir(os.path.join(modulePath, "materials/textures"))
        os.mkdir(os.path.join(modulePath, "models"))
        os.mkdir(os.path.join(modulePath, "quests"))
        os.mkdir(os.path.join(modulePath, "scripts"))
        os.mkdir(os.path.join(modulePath, "scripts/maps"))
        os.mkdir(os.path.join(modulePath, "sound"))
        
        mname = str(self.moduleNameLineEdit.text())
        mdeps = ""
        for m in self.moduleDependenciesList.selectedItems():
            mdeps = mdeps + str("\"" + m.text() + "\", ")
        mdeps = mdeps.strip(", ")
        
        sdeps = ""
        for s in self.scriptDependenciesList.selectedItems():
            script = str(str(s.text()).replace("\\", "/"))
            script = script.split("/")
            script = script[len(script) - 1]
            sdeps = sdeps + str("       require " + script + "\n")
        
        p = os.path.join(modulePath, "scripts")
        p = os.path.join(p, "moduleconfig.rb")
        f = open(p, "w")
        f.write("include RlScript\n")
        f.write("\n\n")
        f.write("class " + mname.capitalize() + "Module < ContentModule\n")
        f.write("    def initialize()\n")
        f.write("        super(\"" + mname + "\"" + ", \"" + mname + "\"" + ", false, 200608012)\n")
        f.write("    end\n\n")
        f.write("    def getDependencies()\n")
        f.write("       return [" + str(mdeps) + "];\n")
        f.write("    end\n\n")
        f.write("    def getTextureLocations()\n")
        f.write("       return [\"textures\"];\n")
        f.write("    end\n\n")
        f.write("    def getModelLocations()\n")
        f.write("       return [\"models\"];\n")
        f.write("    end\n\n")
        f.write("    def getSoundLocations()\n")
        f.write("       return [\"sound\"];\n")
        f.write("    end\n\n")
        f.write("    def start()\n")
        f.write(sdeps + "\n")
        f.write("       require 'player.rb'\n")
        f.write("       require 'mckhero.rb'\n")
        f.write("\n\n       SceneManager.getSingleton().loadScene(\"" + self.sceneNameLineEdit.text() + "\");\n\n")
        f.write("       hero = $GOM.getGameObject(XXXXXX);\n")
        f.write("       PlayerSettings.preparePlayer(hero)\n")
        f.write("       PartyManager.getSingleton().setActiveCharacter(hero)\n\n")
        f.write("    end\n")
        f.write("end\n\n")

        f.write("CoreSubsystem.getSingleton().registerModule(" + mname.capitalize() + "Module.new());\n")
        f.close()
        
        
        
        p = os.path.join(modulePath, "maps")
        p = os.path.join(p, str(self.sceneNameLineEdit.text()) + ".rlscene")
        
        f = open(p, "w")
        f.write("<scene name=\"" + str(self.sceneNameLineEdit.text()) + "\">\n")
        f.write("    <map file=\"" + str(self.mapNameLineEdit.text()) + ".rlmap.xml\"" +  "/>\n")
        f.write("</scene>\n")
        f.close()
        
        p = os.path.join(modulePath, "maps")
        p = os.path.join(p, str(self.mapNameLineEdit.text()) + ".rlmap.xml")
        
        f = open(p, "w")
        f.write("<rastullahmap formatVersion=\"0.4.0\">\n")
        f.write("    <nodes>\n")
        f.write("    </nodes>\n")
        f.write("</rastullahmap>\n")
        f.close()
        
        self.moduleManager.resetParsedModuleConfig()
        self.moduleManager.openLoadModuleDialog()
        
    @pyqtSignature("")
    def on_cancelButton_pressed(self):
        self.close()


