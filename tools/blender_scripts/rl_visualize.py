#!BPY

""" Registration info for Blender menus:
Name: 'Rastullah Visualizer'
Blender: 241
Group: 'Misc'
Tooltip: 'Visualization of Rastullah Level Features, like PhysicProxies,...'
"""

# --------------------------------------------------------------------------
# ***** BEGIN GPL LICENSE BLOCK *****
#
# Copyright (C) 2006 Julius Schulz
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#
# ***** END GPL LICENCE BLOCK *****
# --------------------------------------------------------------------------

__author__ = ['Julius Schulz, Team Pantheon']
__version__ = '0.1'
__url__ = ['Rastullahs Lockenpracht website, http://www.rastullahs-lockenpracht.de/',
	'Team Pantehon website, http://www.team-pantheon.de']
__bpydoc__ = """\
Visualization of Rastullah Level Features, like PhysicProxies,...
"""

import Blender
from Blender import Text, Scene

script_text = """
import Blender
from Blender import *
from Blender.BGL import *

def drawPrimitiveCube(object):
	objectMatrix = object.getMatrix('worldspace')

	cube = [ Mathutils.Vector([-1,-1, 1]), Mathutils.Vector([1, -1, 1]), Mathutils.Vector([1, 1, 1]), Mathutils.Vector([-1, 1, 1]), Mathutils.Vector([-1,-1, -1]), Mathutils.Vector([1, -1, -1]), Mathutils.Vector([1, 1, -1]), Mathutils.Vector([-1, 1, -1]) ]
	cube1 = []
	for point in cube:
		cube1.append(point*objectMatrix.rotationPart()+objectMatrix.translationPart())
	cube = cube1

	glColor4f(0,0,0.5,0.3)
	glLineWidth(2)

	glBegin(GL_LINES)

 	#top square
	glVertex3f(cube[0].x,cube[0].y,cube[0].z)
	glVertex3f(cube[1].x,cube[1].y,cube[1].z)
	glVertex3f(cube[1].x,cube[1].y,cube[1].z)
	glVertex3f(cube[2].x,cube[2].y,cube[2].z)
	glVertex3f(cube[2].x,cube[2].y,cube[2].z)
	glVertex3f(cube[3].x,cube[3].y,cube[3].z)
	glVertex3f(cube[3].x,cube[3].y,cube[3].z)
	glVertex3f(cube[0].x,cube[0].y,cube[0].z)
	
 	#bottom square
	glVertex3f(cube[4].x,cube[4].y,cube[4].z)
	glVertex3f(cube[5].x,cube[5].y,cube[5].z)
	glVertex3f(cube[5].x,cube[5].y,cube[5].z)
	glVertex3f(cube[6].x,cube[6].y,cube[6].z)
	glVertex3f(cube[6].x,cube[6].y,cube[6].z)
	glVertex3f(cube[7].x,cube[7].y,cube[7].z)
	glVertex3f(cube[7].x,cube[7].y,cube[7].z)
	glVertex3f(cube[4].x,cube[4].y,cube[4].z)

	glVertex3f(cube[0].x,cube[0].y,cube[0].z)
	glVertex3f(cube[4].x,cube[4].y,cube[4].z)
	glVertex3f(cube[1].x,cube[1].y,cube[1].z)
	glVertex3f(cube[5].x,cube[5].y,cube[5].z)
	glVertex3f(cube[2].x,cube[2].y,cube[2].z)
	glVertex3f(cube[6].x,cube[6].y,cube[6].z)
	glVertex3f(cube[3].x,cube[3].y,cube[3].z)
	glVertex3f(cube[7].x,cube[7].y,cube[7].z)
	
	glEnd()
	
	glLineWidth(1)
	return

def drawMesh(object):
	##mesh = Blender.Mesh.Get(object.getParent().data.name)
	mesh = object.getParent().getData(mesh=1)
	objectMatrix = object.getMatrix('worldspace')
	
	glColor4f(0,0,0.5,0.3)
	glLineWidth(2)

	for e in mesh.edges:
		glBegin(GL_LINES)
		v1 = e.v1.co*objectMatrix.rotationPart()+objectMatrix.translationPart()
		v2 = e.v2.co*objectMatrix.rotationPart()+objectMatrix.translationPart()
		glVertex3f(v1.x,v1.y,v1.z)
		glVertex3f(v2.x,v2.y,v2.z)
		glEnd()
		
	glLineWidth(1)
	return

# Get the Window matrix and create a gl Buffer with it�s data.
viewMatrix = Window.GetPerspMatrix()
viewBuff= [viewMatrix[i][j] for i in xrange(4) for j in xrange(4)]
viewBuff=BGL.Buffer(GL_FLOAT, 16, viewBuff)

# Load the Window matrix for gl Drawing.
glLoadIdentity()
glMatrixMode(GL_PROJECTION)
glPushMatrix()
glLoadMatrixf(viewBuff)

# Enable Point antializing to get round points.
glEnable(GL_POINT_SMOOTH);

# Enable glBlend for alpha blending.
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

# Get all objects in current Scene.
scene = Scene.GetCurrent()
objects = scene.getChildren()

for object in objects:
	if object.getType() == 'Empty':
		show = 0
		for objectLayer in object.layers:
			for windowLayer in Window.ViewLayers():
				if objectLayer == windowLayer: show = 1
		if show:
			try:
				if object.getProperty('SpecialType').getData() == 'PhysicProxy':
					if object.getProperty('proxy_type').getData() == 'Box':
						drawPrimitiveCube(object)
					elif object.getProperty('proxy_type').getData() == 'Mesh':
						drawMesh(object)
			except:
				a=1 #for nothing,just a placeholder thingy

# Disable alpha blending and point smoothing.
glDisable(GL_BLEND)
glDisable(GL_POINT_SMOOTH)
"""

try:
	txt = Text.Get("Rastullah Visualizer")
	Text.unlink(txt)
	txt = Text.New("Rastullah Visualizer")
	txt.write(script_text)
except:
	txt = Text.New("Rastullah Visualizer")
	txt.write(script_text)

scene = Scene.GetCurrent()
scene.clearScriptLinks(['Rastullah Visualizer'])
scene.addScriptLink('Rastullah Visualizer', 'Redraw')