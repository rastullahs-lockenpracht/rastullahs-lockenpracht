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

import ctypes
import ogre.renderer.OGRE as og

class MyRaySceneQueryListener ( og.RaySceneQueryListener ):
    def __init__( self ):
        super ( MyRaySceneQueryListener, self ).__init__()
        self.dist = 100000
        
    # sort algorithm for the selection list
    def sortCompareImp(self,  x,  y):
        if x.distance > y.distance:
            return 1
        elif x.distance == y.distance:
            return 0
        else: # x<y
            return -1

    def queryResult ( self, entity, distance ):
#        if distance == 0.0: #camera is in the bounding box, ignore this selection
#            return True
        
        if self.dist > distance:
            self.dist = distance

        return True
