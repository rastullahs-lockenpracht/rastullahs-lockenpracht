import sys
import os
import platform

copyCommand = "cp "
cpDestination =  " ../src/"

if platform.system() == "Windows":
    copyCommand = "copy "
    cpDestination = " ..\src\\"

###########################################################################
output = os.popen("pyuic4 lockewickler.ui > ui_lw.py")
for line in output.readlines():
    print line

output = os.popen(copyCommand + "ui_lw.py" + cpDestination)
for line in output.readlines():
    print "pyuic4 lockewickler.ui > ui_lw.py " + line
###########################################################################

###########################################################################
output = os.popen("pyuic4 preferences_dialog.ui > ui_pref_dialog.py")
for line in output.readlines():
    print line

output = os.popen(copyCommand + "ui_pref_dialog.py" + cpDestination)
for line in output.readlines():
    print "pyuic4 preferences_dialog.ui > ui_pref_dialog.py " + line
###########################################################################

###########################################################################
output = os.popen("pyuic4 object_property.ui > ui_obj_prop.py")
for line in output.readlines():
    print line

output = os.popen(copyCommand + "ui_obj_prop.py" + cpDestination)
for line in output.readlines():
    print "pyuic4 object_property.ui > ui_obj_prop.py " + line
###########################################################################

###########################################################################
output = os.popen("pyuic4 model_preview.ui > ui_model_prev.py")
for line in output.readlines():
    print line

output = os.popen(copyCommand + "ui_model_prev.py" + cpDestination)
for line in output.readlines():
    print "pyuic4 model_preview.ui > ui_model_prev.py " + line
###########################################################################

###########################################################################
output = os.popen("pyuic4 FourPanelView.ui > ui_FourPanelView.py")
for line in output.readlines():
        print line

output = os.popen(copyCommand + "ui_FourPanelView.py" + cpDestination)
for line in output.readlines():
    print "pyuic4 FourPanelView.ui > ui_FourPanelView.py " + line
###########################################################################

###########################################################################
output = os.popen("pyuic4 console_window.ui > ui_ConsoleWindow.py")
for line in output.readlines():
        print line

output = os.popen(copyCommand + "ui_ConsoleWindow.py" + cpDestination)
for line in output.readlines():
    print "pyuic4 console_window.ui > ui_ConsoleWindow.py " + line
###########################################################################

print "Fertig!"
