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

from os.path import isfile,  join
from GlobDirectoryWalker import GlobDirectoryWalker

class TriggerProperty():
    def __init__(self, name, type, data):
        self.name = name
        self.type = type
        self.data = data

class Trigger():
    def __init__(self, className, name):
        self.className = className
        self.name = name
        self.properties = []
        
    def addProperty(self, name, type, data):
        prop = TriggerProperty(name,  type, data)
        self.properties.append(prop)
        
    def removeProperty(self, name):
        for prop in self.properties:
            if prop.name == name:
                self.properties.remove(prop)
                return
        
class TriggerManager():
    instance = None
    availableTriggers = []
    
    def __init__(self):
        self.triggerInstances = ()
        self.directories = []
        
        if TriggerManager.instance == None:
            TriggerManager.instance = self
        
    def addDirectory(self, dir):
        self.directories.append(dir)
        
        self.update()
    
    def createTrigger(self, className, name):
        trigger = Trigger(className, name)
        self.triggerInstances.append(trigger)
        return trigger
    
    def update(self):
        TriggerManager.availableTriggers = []
        
        for dir in self.directories:
            for file in GlobDirectoryWalker(dir, "*.rb"):
                f = open(file, "r")
                for line in f:
                    if line.startswith("class ") and "< Trigger" in line:
                        split = line.split(" ")
                        TriggerManager.availableTriggers.append(split[1])

    
