#
# a single config file that is imported 'first' by the Python-Ogre demos
# this is primarly used to manage Linux demos when we have binaries in non standard (safe) locations

import os,sys

#########################################################################
### Override this setting if you are using non standard locations

libpath = "/home/stefan/development/root/usr/lib/"

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
    pythonpath = os.path.abspath(os.path.join(libpath, 'python2.5/site-packages') )
    ## and insert it into the system path
    sys.path.insert(0,pythonpath)

    ## now we need to set the library path so the shared libraries can be found
    try:
        ldpath = os.environ['LD_LIBRARY_PATH']
    except:
        ldpath=""
    newldpath = libpath + ":" + ldpath
    os.environ['LD_LIBRARY_PATH'] = newldpath

