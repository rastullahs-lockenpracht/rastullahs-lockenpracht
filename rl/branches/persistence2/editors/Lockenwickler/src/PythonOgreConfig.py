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


#
# a single config file that is imported 'first' by the Python-Ogre demos
# this is primarly used to manage Linux demos when we have binaries in non standard (safe) locations

import os,sys

#########################################################################
### Override this setting if you are using non standard locations

libpath = "/home/fusion/development/root/usr/lib/"

#########################################################################


if os.name == 'nt':
    libpath=""
    pythonpath = ""
else:
    if libpath == "" :  # we need to find this ourselves
        localpath = os.path.abspath(os.path.dirname(__file__) ) # assume that this module is in the ./demos directory
        libpath = os.path.abspath(os.path.join(localpath,'../lib'))  # so in the binary release the lib is one path back up
        if not os.path.exists ( libpath ):   #we must be running from the development environment
            libpath = os.path.abspath(os.path.join(localpath,'../../root/usr/lib'))
            if not os.path.exists ( libpath ):
                ## should die here
                print "**Problem finding libraries"
                libpath=localpath

    ## path to the Python-Ogre modules
    pythonpath = os.path.abspath(os.path.join(libpath, 'python2.6/site-packages') )
    ## and insert it into the system path
    sys.path.insert(0,pythonpath)

    ## now we need to set the library path so the shared libraries can be found
    try:
        ldpath = os.environ['LD_LIBRARY_PATH']
    except:
        ldpath=""
    newldpath = libpath + ":" + ldpath
    os.environ['LD_LIBRARY_PATH'] = newldpath

