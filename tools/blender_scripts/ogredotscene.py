#!BPY

"""
Name: 'Ogre3D Scene'
Blender: 237
Group: 'Export'
Tooltip: 'Exports the current scene to Ogre3D'
"""

__author__ = ['Michael Reimpell']
__version__ = '0.0.0'
__url__ = ['OGRE website, http://www.ogre3d.org',
	'OGRE forum, http://www.ogre3d.org/phpBB2/']
__bpydoc__ = """\
Exports the current scene to Ogre3D.
"""

# Copyright (C) 2005  Michel Reimpell
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
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

# epydoc doc format
__docformat__ = "javadoc en"

######
# imports
######
import sys

try:
	import Blender
except ImportError:
	sys.exit("Please run this script from within Blender!\nSee the script's manual for more information.")

import os, math

######
# namespaces
######
from Blender import Draw
from Blender import Mathutils
from Blender import Registry
from Blender.BGL import *

######
# Classes
######
class Action:
	"""Action interface.
	
	   Actions encapsulate user requests.
	"""
	def __init__(self):
		"""Constructor.
		"""
		return
	def execute(self):
		"""Executes the action.
		"""
		return
	
class QuitAction(Action):
	"""Quits the windowing system.
	"""
	def execute(self):
		Blender.Draw.Exit()
		return

class Model:
	"""Model interface.
	"""
	def __init__(self):
		self.viewList = []
		return
	def addView(self, view):
		self.viewList.append(view)
		return
	def removeView(self, view):
		if view in self.viewList:
			self.viewList.remove(view)
		return
	# protected
	def _notify(self):
		"""Notify views.
		"""
		for view in self.viewList:
			view.update()
		return

class View:
	"""View interface.
	"""
	def __init__(self):
		return
	def update(self):
		return

class Widget:
	"""Widget interface.
	
	   @cvar INFINITY Infinity value for size hints.
	"""
	INFINITY = 2147483647
	def __init__(self):
		"""Constructor.
		"""
		return
	def draw(self, screenRectangle):
		"""Draws the widget into an area of the screen.
		
		   @param screenRectangle Area of the screen to draw into.
		          The screenRectangle is a list of the integers
		          <code>[xl, yl, xu, yu]</code>, where (xl,yl) is
		          the lower left corner of the area and (xu, yu) is
		          the upper right corner of the area.
		"""
		return
	def eventFilter(self, event, value):
		"""Called from event callback function.
		
		   @see Blender.Draw.Register
		"""
		return
	def addButtons(self, parent):
		"""Registers widget buttons at the parent.
		
		   @param parent Parent widget or screen.
		"""
		return
	def removeButtons(self, parent):
		"""Removes widget buttons from parent.
		
		   @param parent Parent widget or screen.
		"""
		return
	def getMaximumSize(self):
		"""Returns the maximum size of this widget.
		
		   @return List of integers <code>[width, height]</code>.
		"""
		return [Widget.INFINITY, Widget.INFINITY]
	def getMinimumSize(self):
		"""Returns the minimum size of this widget.
		
		   @return List of integers <code>[width, height]</code>.
		"""
		return [0, 0]
	def getPreferredSize(self):
		"""Returns the preferred size of this widget.
		
		   @return List of integers <code>[width, height]</code>.
		"""
		return [0, 0]

class VerticalLayout(Widget):
	"""Widget that manages vertically stacked child widgets.
	"""
	def __init__(self):
		"""Constructor.
		"""
		# widgetDict key: name, value: Widget
		self.widgetDict = {}
		# displayList list of widget names in the order to draw them
		self.displayList = []
		# parent to register buttons to.
		self.parent = None
		return
	def addWidget(self, widget, name):
		"""Adds a child widget to this layout.
		
		   The child widget is appended below all other widgets.
		
		   @param widget Widget to add.
		   @param name Unique name for the widget.
		"""
		self.widgetDict[name] = widget
		if name not in self.displayList:
			self.displayList.append(name)
		# register to known parent
		if self.parent:
			widget.addButtons(self.parent)
		return
	def removeWidget(self, name):
		"""Removes a child widget from this layout.
		
		   @param name Name of the widget to remove.
		   @see addWidget
		"""
		if self.widgetDict.has_key(name):
			if self.parent:
				self.widgetDict[name].removeButtons(self.parent)
			del self.widgetDict[name]
			del self.displayList[self.displayList.index(name)]
		return
	def addButtons(self, parent):
		self.parent = parent
		for widget in self.widgetDict.values():
			widget.addButtons(self.parent)
		return
	def removeButtons(self, parent):
		for widget in self.widgetDict.values():
			widget.removeButtons(self.parent)
		return
	def draw(self, screenRectangle):
		# split height for the child widgets
		minimumSize = self.getMinimumSize()
		height = screenRectangle[3]- screenRectangle[1]
		additionalHeight = height - minimumSize[1]
		stretchWidgetList = []
		extraHeight = 0
		# get widgets with unlimited preferred height
		if (additionalHeight > 0):
			stretchWidgetList = [name for (name, w) in self.widgetDict.items() if w.getPreferredSize()[1] >= Widget.INFINITY]
			if (len(stretchWidgetList) > 0):
				# give equal extra height to widgets with unlimited preferred height
				extraHeight = additionalHeight / len(stretchWidgetList)
		# draw widgets with minimum or minimum plus extra size
		y = screenRectangle[3]
		dy = 0
		for name in self.displayList:
			widget = self.widgetDict[name]
			dy = widget.getMinimumSize()[1]
			if (name in stretchWidgetList):
				dy += extraHeight
			widget.draw([screenRectangle[0], y-dy, screenRectangle[2], y])
			y -= dy
		return
	def eventFilter(self, event, value):
		for widget in self.widgetDict.values():
			widget.eventFilter(event, value)
		return
	def getMaximumSize(self):
		size = [0, 0]
		for widget in self.widgetDict.values():
			if (size[0] < widget.getMaximumSize()[0]):
				size[0] = widget.getMaximumSize()[0]
			size[1] += widget.getMaximumSize()[1]
		return size
	def getMinimumSize(self):
		size = [0, 0]
		for widget in self.widgetDict.values():
			if (size[0] < widget.getMinimumSize()[0]):
				size[0] = widget.getMinimumSize()[0]
			size[1] += widget.getMinimumSize()[1]
		return size
	def getPreferredSize(self):
		size = [0, 0]
		for widget in self.widgetDict.values():
			if (size[0] < widget.getPreferredSize()[0]):
				size[0] = widget.getPreferredSize()[0]
			size[1] += widget.getPreferredSize()[1]
		return size

class HorizontalLayout(Widget):
	"""Widget that manages horizontally stacked child widgets.
	"""
	def __init__(self):
		"""Constructor.
		"""
		# widgetDict key: name, value: Widget
		self.widgetDict = {}
		# displayList list of widget names in the order to draw them
		self.displayList = []
		# parent to register buttons to.
		self.parent = None
		return
	def addWidget(self, widget, name):
		"""Adds a child widget to this layout.
		
		   The child widget is appended below all other widgets.
		
		   @param widget Widget to add.
		   @param name Unique name for the widget.
		"""
		self.widgetDict[name] = widget
		if name not in self.displayList:
			self.displayList.append(name)
		if self.parent:
			widget.addButtons(self.parent)
		return
	def removeWidget(self, name):
		"""Removes a child widget from this layout.
		
		   @param name Name of the widget to remove.
		   @see addWidget
		"""
		if self.widgetDict.has_key(name):
			if self.parent:
				self.widgetDict[name].removeButtons(self.parent)
			del self.widgetDict[name]
			del self.displayList[self.displayList.index(name)]
		return
	def addButtons(self, parent):
		self.parent = parent
		for widget in self.widgetDict.values():
			widget.addButtons(self.parent)
		return
	def removeButtons(self, parent):
		for widget in self.widgetDict.values():
			widget.removeButtons(self.parent)
		return
	def draw(self, screenRectangle):
		# split height for the child widgets
		minimumSize = self.getMinimumSize()
		width = screenRectangle[2]- screenRectangle[0]
		additionalWidth = width - minimumSize[0]
		stretchWidgetList = []
		extraWidth = 0
		# get widgets with unlimited preferred height
		if (additionalWidth > 0):
			stretchWidgetList = [name for (name, w) in self.widgetDict.items() if w.getPreferredSize()[0] >= Widget.INFINITY]
			if (len(stretchWidgetList) > 0):
				# give equal extra height to widgets with unlimited preferred height
				extraWidth = additionalWidth / len(stretchWidgetList)
		# draw widgets with minimum or minimum plus extra size
		x = screenRectangle[0]
		dx = 0
		for name in self.displayList:
			widget = self.widgetDict[name]
			dx = widget.getMinimumSize()[0]
			if (name in stretchWidgetList):
				dx += extraWidth
			widget.draw([x, screenRectangle[1], x+dx, screenRectangle[3]])
			x += dx
		return
	def eventFilter(self, event, value):
		for widget in self.widgetDict.values():
			widget.eventFilter(event, value)
		return
	def getMaximumSize(self):
		size = [0, 0]
		for widget in self.widgetDict.values():
			size[0] += widget.getMaximumSize()[0]
			if (size[1] < widget.getMaximumSize()[1]):
				size[1] = widget.getMaximumSize()[1]
		return size
	def getMinimumSize(self):
		size = [0, 0]
		for widget in self.widgetDict.values():
			size[0] += widget.getMinimumSize()[0]
			if (size[1] < widget.getMinimumSize()[1]):
				size[1] = widget.getMinimumSize()[1]
		return size
	def getPreferredSize(self):
		size = [0, 0]
		for widget in self.widgetDict.values():
			size[0] += widget.getPreferredSize()[0]
			if (size[1] < widget.getPreferredSize()[1]):
				size[1] = widget.getPreferredSize()[1]
		return size

class HorizontalRatioLayout(Widget):
	"""Widget that manages horizontally stacked child widgets.
	
	   The drawing area is distributed among the childs with a given ratio.
	"""
	def __init__(self):
		# widgetDict key: name, value: [Widget, ratio]
		self.widgetDict = {}
		# displayList list of widget names in the order to draw them
		self.displayList = []
		# parent to register buttons to.
		self.parent = None
		return
	def addWidget(self, ratio, widget, name):
		"""Adds a child widget to this layout.
		
		   The child widget is appended right to all other widgets.
		   The ratios must sum up to one.
		
		   @param ratio Ratio of screen width to occupy with this widget. 
		   @param widget Widget to add.
		   @param name Unique name for the widget.
		"""
		self.widgetDict[name] = [widget, ratio]
		if name not in self.displayList:
			self.displayList.append(name)
		if self.parent:
			widget.addButtons(self.parent)
		return
	def removeWidget(self, name):
		"""Removes a child widget from this layout.
		
		   @param name Name of the widget to remove.
		   @see addWidget
		"""
		if self.widgetDict.has_key(name):
			if self.parent:
				self.widgetDict[name].removeButtons(self.parent)
			del self.widgetDict[name]
			del self.displayList[self.displayList.index(name)]
		return
	def addButtons(self, parent):
		self.parent = parent
		for widget in self.widgetDict.values():
			widget[0].addButtons(self.parent)
		return
	def removeButtons(self, parent):
		for widget in self.widgetDict.values():
			widget[0].removeButtons(self.parent)
		return
	def draw(self, screenRectangle):
		# split height for the child widgets
		minimumSize = self.getMinimumSize()
		width = screenRectangle[2] - screenRectangle[0]
		ratioSum = 0
		x = screenRectangle[0]
		dx = 0
		for name in self.displayList:
			[widget, ratio] = self.widgetDict[name]
			ratioSum += ratio
			if ratio >= 1:
				dx = screenRectangle[2] - x + 1
			else:
				dx = int(ratio*width)
			widget.draw([x, screenRectangle[1], x+dx-1, screenRectangle[3]])
			x += dx
		return
	def eventFilter(self, event, value):
		for [widget, ratio] in self.widgetDict.values():
			widget.eventFilter(event, value)
		return		
	def getMaximumSize(self):
		size = [0, 0]
		for [widget, ratio] in self.widgetDict.values():
			size[0] += widget.getMaximumSize()[0]
			if (size[1] < widget.getMaximumSize()[1]):
				size[1] = widget.getMaximumSize()[1]
		return size
	def getMinimumSize(self):
		size = [0, 0]
		for [widget, ratio] in self.widgetDict.values():
			size[0] += widget.getMinimumSize()[0]
			if (size[1] < widget.getMinimumSize()[1]):
				size[1] = widget.getMinimumSize()[1]
		return size
	def getPreferredSize(self):
		size = [Widget.INFINITY, 0]
		for [widget, ratio] in self.widgetDict.values():
			if (size[1] < widget.getPreferredSize()[1]):
				size[1] = widget.getPreferredSize()[1]
		return size

class Spacer(Widget):
	"""Occupies blank space on the screen.
	"""
	def __init__(self, minimumSize, maximumSize=None, preferredSize=None):
		"""Constructor.
		
		   If maximum or preferred size is not given, the minimum size will be used instead.
		"""
		self.minimumSize = minimumSize
		if maximumSize:
			self.maximumSize = maximumSize
		else:
			self.maximumSize = minimumSize
		if preferredSize:
			self.preferredSize = preferredSize
		else:
			self.preferredSize = minimumSize
	def getMaximumSize(self):
		return self.maximumSize
	def getMinimumSize(self):
		return self.minimumSize
	def getPreferredSize(self):
		return self.preferredSize

class Decorator(Widget):
	"""Decorates a child widget.
	"""
	def __init__(self, childWidget):
		self.childWidget = childWidget
		return
	def draw(self, screenRectangle):
		self.childWidget.draw(screenRectangle)
		return
	def eventFilter(self, event, value):
		self.childWidget.eventFilter(event, value)
		return
	def addButtons(self, parent):
		self.childWidget.addButtons(parent)
		return
	def removeButtons(self, parent):
		self.childWidget.removeButtons(parent)
		return

class Border(Decorator):
	"""Border around widgets.
	"""
	def __init__(self, childWidget, borderSize=10):
		"""Constructor.
		
		   @param childWidget Widget to decorate.
		   @param borderSize Size of the border.
		"""
		Decorator.__init__(self, childWidget)
		self.borderSize = borderSize
		return
	def draw(self, screenRectangle):
		rect = screenRectangle[:]
		rect[0] += self.borderSize
		rect[1] += self.borderSize
		rect[2] -= self.borderSize
		rect[3] -= self.borderSize
		self.childWidget.draw(rect)
		return
	def getMaximumSize(self):
		size = self.childWidget.getMaximumSize()[:]
		size[0] += 2*self.borderSize
		size[1] += 2*self.borderSize
		return size
	def getMinimumSize(self):
		size = self.childWidget.getMinimumSize()[:]
		size[0] += 2*self.borderSize
		size[1] += 2*self.borderSize
		return size
	def getPreferredSize(self):
		size = self.childWidget.getPreferredSize()[:]
		size[0] += 2*self.borderSize
		size[1] += 2*self.borderSize
		return size
		
class Box(Decorator):
	"""Provides a border with an optional titel for a child widget.
	"""
	def __init__(self, childWidget, title=None):
		"""Constructor.
		
		   @param childWidget Widget to decorate.
		   @param title Optional frame title.
		"""
		Decorator.__init__(self, childWidget)
		self.title = title
		return
	def draw(self, screenRectangle):
		rect = screenRectangle[:]
		if self.title:
			# border and title
			rect[0] += 3
			rect[1] += 3
			rect[2] -= 3
			rect[3] -= 13
			# title
			titleWidth = Blender.Draw.GetStringWidth(self.title)
			theme = Blender.Window.Theme.Get()[0]
			glColor4ub(*theme.get('text').text)
			glRasterPos2i(rect[0]+4, rect[3])
			Blender.Draw.Text(self.title)
			# border
			glColor3f(0.0, 0.0, 0.0)
			glBegin(GL_LINE_STRIP)
			glVertex2i(rect[0]+6+titleWidth, rect[3]+5)
			glVertex2i(rect[2], rect[3]+5)
			glVertex2i(rect[2], rect[1])
			glVertex2i(rect[0], rect[1])
			glVertex2i(rect[0], rect[3]+5)
			glVertex2i(rect[0]+3, rect[3]+5)
			glEnd()
			rect[0] += 3
			rect[1] += 3
			rect[2] -= 3
			rect[3] -= 3
		else:
			# border only
			glColor3f(0.0, 0.0, 0.0)
			glBegin(GL_LINE_STRIP)
			rect[0] += 3
			rect[1] += 3
			rect[2] -= 3
			rect[3] -= 3
			glVertex2i(rect[0], rect[1])
			glVertex2i(rect[0], rect[3])
			glVertex2i(rect[2], rect[3])
			glVertex2i(rect[2], rect[1])
			glVertex2i(rect[0], rect[1])
			glEnd()
			rect[0] += 3
			rect[1] += 3
			rect[2] -= 3
			rect[3] -= 3
		self.childWidget.draw(rect)
		return
	def getMaximumSize(self):
		size = self.childWidget.getMaximumSize()[:]
		size[0] += 12
		size[1] += 12
		if size[0] < self.getMinimumSize()[0]:
			size[0] = self.getMinimumSize()[0]
		return size
	def getMinimumSize(self):
		size = self.childWidget.getMinimumSize()[:]
		# basic border size
		size[0] += 12
		size[1] += 12
		# add title size
		if self.title:
			size[1] += 11
			if (size[0] < (Blender.Draw.GetStringWidth(self.title)+16)):
				size[0] = (Blender.Draw.GetStringWidth(self.title)+16)
		return size
	def getPreferredSize(self):
		size = self.childWidget.getPreferredSize()[:]
		size[0] += 12
		size[1] += 12
		if size[0] < self.getMinimumSize()[0]:
			size[0] = self.getMinimumSize()[0]
		return size

class WidgetList(Widget):
	"""Displays a list of vertically stacked widgets using a scrollbar if necessary.
	"""
	def __init__(self):
		"""Constructor.
		"""
		# widgetDict key: name, value: Widget
		self.widgetDict = {}
		# displayList list of widget names in the order to draw them
		self.displayList = []
		# visible child widgets
		self.visibleList = []
		# parent to register buttons to.
		self.parent = None
		# scrollbar
		self.scrollbar = VerticalScrollbar(0, 0, 0)
		return
	def addWidget(self, widget, name):
		"""Adds a child widget to this layout.
		
		   The child widget is appended below all other widgets.
		
		   @param widget Widget to add.
		   @param name Unique name for the widget.
		"""
		self.widgetDict[name] = widget
		if name not in self.displayList:
			self.displayList.append(name)
		# register to known parent
		if self.parent:
			widget.addButtons(self.parent)
		return
	def removeWidget(self, name):
		"""Removes a child widget from this layout.
		
		   @param name Name of the widget to remove.
		   @see addWidget
		"""
		if self.widgetDict.has_key(name):
			if self.parent:
				self.widgetDict[name].removeButtons(self.parent)
			del self.widgetDict[name]
			del self.displayList[self.displayList.index(name)]
		return
	def addButtons(self, parent):
		self.parent = parent
		for widget in self.widgetDict.values():
			widget.addButtons(self.parent)
		self.scrollbar.addButtons(self.parent)
		return
	def removeButtons(self, parent):
		for widget in self.widgetDict.values():
			widget.removeButtons(self.parent)
		self.scrollbar.removeButtons(self.parent)
		return	
	def draw(self, screenRectangle):
		# draw a border
		rect = screenRectangle[:]
		glColor3f(0.0, 0.0, 0.0)
		glBegin(GL_LINE_STRIP)
		glVertex2i(rect[0], rect[3])
		glVertex2i(rect[2], rect[3])
		glVertex2i(rect[2], rect[1])
		glVertex2i(rect[0], rect[1])
		glVertex2i(rect[0], rect[3])
		glVertex2i(rect[0], rect[3])
		glEnd()
		rect[0] += 1
		rect[1] += 1
		rect[2] -= 1
		rect[3] -= 1
		# calculate nFirstLines form screenRectangle and set scrollbar accordingly
		firstLine = self.scrollbar.getCurrentValue()
		# get list backwards until height is fully used
		nLines = len(self.displayList)
		height = rect[3] - rect[1]
		iLine = -1
		while ((iLine >= -nLines) and (height > 0)):
			height -= self.widgetDict[self.displayList[iLine]].getMinimumSize()[1]
			if height >= 0:
				iLine -= 1
		# nLines + iLine + 1 = nFirstLines - 1
		if (firstLine > (nLines + iLine + 1)):
			firstLine = nLines + iLine + 1
		self.scrollbar.currentValue = firstLine
		self.scrollbar.maximumValue = nLines + iLine + 1
		self.scrollbar.draw([rect[2]-20, rect[1], rect[2], rect[3]])
		# widgets
		self.visibleList = []
		height = rect[3] - rect[1]
		iLine = firstLine
		y = rect[3]
		while ((iLine < nLines) and (height > 0)):
			widget = self.widgetDict[self.displayList[iLine]]
			dy = widget.getMinimumSize()[1]
			height -= dy
			if (height > 0):
				widget.draw([rect[0], y - dy, rect[2]-21, y])
				self.visibleList.append(widget)
				y -= dy
				iLine += 1
		return
	def eventFilter(self, event, value):
		for widget in self.visibleList:
			widget.eventFilter(event, value)	
		self.scrollbar.eventFilter(event, value)
		return
	def getMinimumSize(self):
		# max minimumSize()[0] + scrollbar and minimumSize[1] of childs
		size = [0, 0]
		for widget in self.widgetDict.values():
			widgetSize = widget.getMinimumSize() 
			if size[0] < widgetSize[0]:
				size[0] = widgetSize[0]
			if size[1] < widgetSize[1]:
				size[1] = widgetSize[1]
		# scrollbar
		scrollbarSize = self.scrollbar.getMinimumSize()
		size[0] += scrollbarSize[0]
		if size[1] < scrollbarSize[1]:
			size[1] = scrollbarSize[1]
		return size
	def getMaximumSize(self):
		# max maximumSize()[0] and sum maximumSize()[1] of childs
		return [0, 0]
	def getPreferredSize(self):
		# self.minimumSize()[0] and Widget.INFINITY for height
		minimumSize = self.getMinimumSize()
		return [minimumSize[0], Widget.INFINITY]
	
class Label(Widget):
	"""Displays a text string.
	"""
	def __init__(self, string, fontSize='normal', fontColor=None):
		"""Constructor.
		
		   @param string Text string to display.
		   @param fontSize Size of the font used for display:
		                  <code>'normal'</code>, <code>'small'</code> or <code>'tiny'</code>.
		   @param fontColor List of font color values.
		"""
		self.string = string
		self.fontSize = fontSize
		self.fontColor = fontColor
		return
	def draw(self, screenRectangle):
		if self.fontColor:
			if (len(self.fontColor) == 3):
				glColor3f(*self.fontColor)
			else:
				glColor4f(*self.fontColor)
		else:
			# theme font color
			theme = Blender.Window.Theme.Get()[0]
			glColor4ub(*theme.get('text').text)
		glRasterPos2i(screenRectangle[0], screenRectangle[1]+3)
		Blender.Draw.Text(self.string, self.fontSize)
		return
	def getMinimumSize(self):
		return [Blender.Draw.GetStringWidth(self.string, self.fontSize), 11]
	
class Button(Widget):
	"""Push button.
	"""
	def __init__(self, action, name, size, tooltip=None):
		"""Constructor.
		
		   @param action Action to execute when the button is pushed.
		   @param name Button name.
		   @param size Minimum size of the button, <code>[width, height]</code>.
		   @param tooltip Tooltip to display for the button.
		"""
		self.action = action
		# Button event number
		self.event = None
		self.name = name
		self.size = size
		self.preferredSize = size
		self.tooltip = tooltip
		return
	def draw(self, rect):
		if self.tooltip:
			Blender.Draw.PushButton(self.name, self.event, rect[0], rect[1], rect[2]-rect[0], rect[3]-rect[1]-1, self.tooltip)
		else:
			Blender.Draw.PushButton(self.name, self.event, rect[0], rect[1], rect[2]-rect[0], rect[3]-rect[1]-1)
		return
	def addButtons(self, parent):
		self.event = parent.addButtonAction(self.action)
		return
	def getMinimumSize(self):
		return self.size
	def getPreferredSize(self):
		return self.preferredSize

class QuitButton(Button):
	def eventFilter(self, event, value):
		if (value != 0):
			# pressed
			if (event == Draw.ESCKEY):
				self.action.execute()
			if (event == Draw.QKEY):
				self.action.execute()
		return

class CheckBox(Widget):
	"""Two state toggle button.
	
	   To provide two way communication between the CheckBox and its parent widget
	   you can use the following setup:
<pre>
Parent	CheckBox	OnChangeAction
  |--------------------->T            Action(self)
  |------->T             |            CheckBox(OnChangeAction, ...)
  |        |------------>|            OnChangeAction.execute()
  |<-------+-------------|            Parent.checkBoxChanged()
  |------->|             |            CheckBox.isChecked()
</pre>
	"""
	def __init__(self, onChangeAction, checked, name, size, tooltip=None):
		# check status
		self.checked = checked
		self.action = CheckBox.CheckAction(self, onChangeAction)
		self.name = name
		self.size = size
		self.tooltip = tooltip
		self.event = None
		return
	def toggle(self):
		self.checked = 1 - self.checked
		Blender.Draw.Redraw(1)
		return
	def isChecked(self):
		return self.checked
	def draw(self, rect):
		if self.tooltip:
			Blender.Draw.Toggle(self.name, self.event, rect[0], rect[1], rect[2]-rect[0]-1, rect[3]-rect[1]-1, self.isChecked(), self.tooltip)
		else:
			Blender.Draw.Toggle(self.name, self.event, rect[0], rect[1], rect[2]-rect[0]-1, rect[3]-rect[1]-1, self.isChecked())
		return
	def addButtons(self, parent):
		self.event = parent.addButtonAction(self.action)
		return
	def getMinimumSize(self):
		return self.size
	class CheckAction(Action):
		def __init__(self, checkBox, action):
			self.checkBox = checkBox
			self.action = action
			return
		def execute(self):
			self.checkBox.toggle()
			self.action.execute()
			return

class StringButton(Button):
	def __init__(self, string, action, name, size, tooltip=None):
		"""Constructor.
		
		   @param string Initial string
		   @param action Action to execute when the button is pushed.
		   @param name Button name.
		   @param size Minimum size of the button, <code>[width, height]</code>.
		   @param tooltip Tooltip to display for the button.
		"""
		self.string = Blender.Draw.Create(string)
		Button.__init__(self, action, name, size, tooltip)
		return
	def getString(self):
		"""Returns current string.
		"""
		return self.string.val
	def draw(self, rect):
		if self.tooltip:
			self.string = Blender.Draw.String(self.name, self.event, rect[0], rect[1], rect[2]-rect[0], rect[3]-rect[1]-1, self.string.val, 255, self.tooltip)
		else:
			self.string = Blender.Draw.String(self.name, self.event, rect[0], rect[1], rect[2]-rect[0], rect[3]-rect[1]-1, self.string.val, 255)
		return
	
class BoundedValueModel(Model):
	def __init__(self, minimum=0, maximum=0, initial=0):
		Model.__init__(self)
		self.minimum = minimum
		self.maximum = maximum
		self.value = None
		self.setValue(initial)
		return
	def getMinimum(self):
		return self.minimum
	def getMaximum(self):
		return self.maximum
	def getValue(self):
		return self.value
	def setValue(self, value):
		if (value != self.value):
			if value < self.minimum:
				self.value = self.minimum
			elif value > self.maximum:
				self.value = self.maximum
			else:
				self.value = value
			self._notify()
		return

class NumberView(Widget, View):
	def __init__(self, name, model, minSize, prefSize=None, maxSize=None, tooltip=None):
		"""Constructor.
		   @param model BoundedValueModel.
		"""
		Widget.__init__(self)
		View.__init__(self)
		self.model = model
		self.model.addView(self)
		self.numberButton = Blender.Draw.Create(self.model.getValue())
		self.title = name
		self.tooltip = tooltip
		self.minSize = minSize
		self.maxSize = maxSize or minSize
		self.prefSize = prefSize or minSize
		self.event = None
		return
	def addButtons(self, parent):
		self.event = parent.addButtonAction(NumberView.SetAction(self))
		return
	def draw(self, rect):
		if self.tooltip:
			self.numberButton = Blender.Draw.Number(self.title, self.event, \
				rect[0], rect[1], rect[2]-rect[0]-1, rect[3]-rect[1]-1, \
				self.model.getValue(), self.model.getMinimum(), self.model.getMaximum(), self.tooltip)
		else:
			self.numberButton = Blender.Draw.Number(self.title, self.event, \
				rect[0], rect[1], rect[2]-rect[0]-1, rect[3]-rect[1]-1, \
				self.model.getValue(), self.model.getMinimum(), self.model.getMaximum())
		return
	def update(self):
		Blender.Draw.Redraw(1)
		return
	def getMaximumSize(self):
		return self.maxSize
	def getMinimumSize(self):
		return self.minSize
	def getPreferredSize(self):
		return self.prefSize
	class SetAction(Action):
		def __init__(self, view):
			self.view = view
			return
		def execute(self):
			self.view.model.setValue(self.view.numberButton.val)
			return

class VerticalScrollbar(Widget):
	"""Scrollbar.
	"""
	def __init__(self, initial, minimum, maximum):
		"""Constructor.
		
		   @param initial Initial value.
		   @param minimum Minimum value.
		   @param maximum Maximum value.
		"""
		self.currentValue = initial
		self.minimumValue = minimum
		self.maximumValue = maximum
		# private
		self.buttonUpEvent = None
		self.buttonDownEvent = None
		self.guiRect = [0,0,0,0]
		self.positionRect = [0,0,0,0]
		self.markerRect = [0,0,0,0]
		self.mousePressed = 0
		self.mouseFocusX = 0
		self.mouseFocusY = 0
		self.markerFocusY = 0
		self.mousePositionY = 0
		return
	def getCurrentValue(self):
		"""Returns current marker position.
		"""
		return self.currentValue
	def up(self, steps=1):
		"""Move scrollbar up.
		"""
		if (steps > 0):
			if ((self.currentValue - steps) > self.minimumValue):
				self.currentValue -= steps
			else:
				self.currentValue = self.minimumValue
		return
	def down(self, steps=1):
		"""Move scrollbar down.
		"""
		if (steps > 0):
			if ((self.currentValue + steps) < self.maximumValue): 
				self.currentValue += steps
			else:
				self.currentValue = self.maximumValue
		return
	def draw(self, screenRectangle):
		"""draw scrollbar
		"""
		# get size of the GUI window to translate MOUSEX and MOUSEY events
		guiRectBuffer = Blender.BGL.Buffer(GL_FLOAT, 4)
		Blender.BGL.glGetFloatv(Blender.BGL.GL_SCISSOR_BOX, guiRectBuffer)
		self.guiRect = [int(guiRectBuffer.list[0]), int(guiRectBuffer.list[1]), \
		                int(guiRectBuffer.list[2]), int(guiRectBuffer.list[3])]
		# relative position
		x = screenRectangle[0]
		y = screenRectangle[1]
		width = screenRectangle[2] - x
		height = screenRectangle[3] - y
		self.positionRect = screenRectangle
		# check minimal size:
		# 2 square buttons,4 pixel borders and 1 pixel inside for inner and marker rectangles
		#if ((height > (2*(width+5))) and (width > 2*5)):
		# keep track of remaining area
		remainRect = self.positionRect[:]
		# draw square buttons
		Blender.Draw.Button("/\\", self.buttonUpEvent, x, y + (height-width), width, width, "scroll up") 
		remainRect[3] -=  width + 2
		Blender.Draw.Button("\\/", self.buttonDownEvent, x, y, width, width, "scroll down") 
		remainRect[1] +=  width + 1
		# draw inner rectangle
		Blender.BGL.glColor3f(0.13,0.13,0.13) # dark grey
		Blender.BGL.glRectf(remainRect[0], remainRect[1], remainRect[2], remainRect[3])
		remainRect[0] += 1
		remainRect[3] -= 1
		Blender.BGL.glColor3f(0.78,0.78,0.78) # light grey
		Blender.BGL.glRectf(remainRect[0], remainRect[1], remainRect[2], remainRect[3])
		remainRect[1] += 1
		remainRect[2] -= 1
		Blender.BGL.glColor3f(0.48,0.48,0.48) # grey
		Blender.BGL.glRectf(remainRect[0], remainRect[1], remainRect[2], remainRect[3])
		# draw marker rectangle
		# calculate marker rectangle
		innerHeight = remainRect[3]-remainRect[1]
		markerHeight = innerHeight/(self.maximumValue-self.minimumValue+1.0)
		# markerRect 
		self.markerRect[0] = remainRect[0]
		self.markerRect[1] = remainRect[1] + (self.maximumValue - self.currentValue)*markerHeight
		self.markerRect[2] = remainRect[2]
		self.markerRect[3] = self.markerRect[1] + markerHeight
		# clip markerRect to innerRect (catch all missed by one errors)
		if self.markerRect[1] > remainRect[3]:
			self.markerRect[1] = remainRect[3]
		if self.markerRect[3] > remainRect[3]:
			self.markerRect[3] = remainRect[3]
		# draw markerRect
		remainRect = self.markerRect
		Blender.BGL.glColor3f(0.78,0.78,0.78) # light grey
		Blender.BGL.glRectf(remainRect[0], remainRect[1], remainRect[2], remainRect[3])
		remainRect[0] += 1
		remainRect[3] -= 1
		Blender.BGL.glColor3f(0.13,0.13,0.13) # dark grey
		Blender.BGL.glRectf(remainRect[0], remainRect[1], remainRect[2], remainRect[3])
		remainRect[1] += 1
		remainRect[2] -= 1
		# check if marker has foucs
		if (self.mouseFocusX and self.mouseFocusY and (self.mousePositionY > self.markerRect[1]) and (self.mousePositionY < self.markerRect[3])):
			Blender.BGL.glColor3f(0.64,0.64,0.64) # marker focus grey
		else:
			Blender.BGL.glColor3f(0.60,0.60,0.60) # marker grey
		Blender.BGL.glRectf(remainRect[0], remainRect[1], remainRect[2], remainRect[3])
		return
	def eventFilter(self, event, value):
		"""event filter for keyboard and mouse input events
		   call it inside the registered event function
		"""
		if (value != 0):
			# Buttons
			if (event == Blender.Draw.PAGEUPKEY):
				self.up(3)
				Blender.Draw.Redraw(1)
			elif (event == Blender.Draw.PAGEDOWNKEY):
				self.down(3)
				Blender.Draw.Redraw(1)
			elif (event == Blender.Draw.UPARROWKEY):
				self.up(1)
				Blender.Draw.Redraw(1)
			elif (event == Blender.Draw.DOWNARROWKEY):
				self.down(1)
				Blender.Draw.Redraw(1)
			# Mouse
			elif (event == Blender.Draw.MOUSEX):
				# check if mouse is inside positionRect
				if (value >= (self.guiRect[0] + self.positionRect[0])) and (value <= (self.guiRect[0] + self.positionRect[2])):
					# redraw if marker got focus
					if (not self.mouseFocusX) and self.mouseFocusY:
						Blender.Draw.Redraw(1)
					self.mouseFocusX = 1
				else:
					# redraw if marker lost focus
					if self.mouseFocusX and self.mouseFocusY:
						Blender.Draw.Redraw(1)
					self.mouseFocusX = 0
			elif (event == Blender.Draw.MOUSEY):
				# check if mouse is inside positionRect
				if (value >= (self.guiRect[1] + self.positionRect[1])) and (value <= (self.guiRect[1] + self.positionRect[3])):
					self.mouseFocusY = 1
					# relative mouse position
					self.mousePositionY = value - self.guiRect[1]
					if ((self.mousePositionY > self.markerRect[1]) and (self.mousePositionY < self.markerRect[3])):
						# redraw if marker got focus
						if self.mouseFocusX and (not self.markerFocusY):
							Blender.Draw.Redraw(1)
						self.markerFocusY = 1
					else:
						# redraw if marker lost focus
						if self.mouseFocusX and self.markerFocusY:
							Blender.Draw.Redraw(1)
						self.markerFocusY = 0
					# move marker
					if (self.mousePressed == 1):
						# calculate step from distance to marker
						if (self.mousePositionY > self.markerRect[3]):
							# up
							self.up(1)
							Blender.Draw.Draw()
						elif (self.mousePositionY < self.markerRect[1]):
							# down
							self.down(1)
							Blender.Draw.Draw()
						# redraw if marker lost focus
						if self.mouseFocusX and self.mouseFocusY:
							Blender.Draw.Redraw(1)
				else:
					# redraw if marker lost focus
					if self.mouseFocusX and self.markerFocusY:
						Blender.Draw.Redraw(1)
					self.markerFocusY = 0
					self.mouseFocusY = 0
			elif ((event == Blender.Draw.LEFTMOUSE) and (self.mouseFocusX == 1) and (self.mouseFocusY == 1)):
				self.mousePressed = 1
				# move marker
				if (self.mousePositionY > self.markerRect[3]):
					# up
					self.up(1)
					Blender.Draw.Redraw(1)
				elif (self.mousePositionY < self.markerRect[1]):
					# down
					self.down(1)
					Blender.Draw.Redraw(1)
			elif (Blender.Get("version") >= 234):
				if (event == Blender.Draw.WHEELUPMOUSE):
					self.up(1)
					Blender.Draw.Redraw(1)
				elif (event == Blender.Draw.WHEELDOWNMOUSE):
					self.down(1)
					Blender.Draw.Redraw(1)
		else: # released keys and buttons
			if (event == Blender.Draw.LEFTMOUSE):
				self.mousePressed = 0
		return
	def getMinimumSize(self):
		return [20, 60]
	def getPreferredSize(self):
		return [20, Widget.INFINITY]
	def addButtons(self, parent):
		self.buttonUpEvent = parent.addButtonAction(VerticalScrollbar.UpAction(self))
		self.buttonDownEvent = parent.addButtonAction(VerticalScrollbar.DownAction(self))
		return
	class UpAction(Action):
		def __init__(self, scrollbar):
			self.scrollbar = scrollbar
			return
		def execute(self):
			self.scrollbar.up()
			Blender.Draw.Redraw(1)
			return
	class DownAction(Action):
		def __init__(self, scrollbar):
			self.scrollbar = scrollbar
			return
		def execute(self):
			self.scrollbar.down()
			Blender.Draw.Redraw(1)
			return

class ProgressBar(Widget):
	def __init__(self):
		Widget.__init__(self)
		# percentage of attained progress
		self.percent = 0.0
		self.minimumSize = [104, 24]
		self.preferredSize = [Widget.INFINITY, 15]
		self.event = None
		return
	def progress(self, percent):
		self.percent = percent
		Blender.Draw.Redraw(1)
		return
	def draw(self, rect):
		# border
		glColor3f(0.0, 0.0, 0.0)
		glBegin(GL_LINE_STRIP)
		glVertex2i(rect[0], rect[1])
		glVertex2i(rect[0], rect[3])
		glVertex2i(rect[2], rect[3])
		glVertex2i(rect[2], rect[1])
		glVertex2i(rect[0], rect[1])
		glEnd()
		# percentage bar
		width = rect[2] - rect[0] - 4
		barWidth = int(width*self.percent)
		Blender.Draw.Toggle("%d%%" % (self.percent*100), self.event, rect[0]+2, rect[1]+2, barWidth, rect[3]-rect[1]-4, 0) 
		return
	def addButtons(self, parent):
		self.event = parent.addButtonAction(ProgressBar.NoToggle())
	def getMinimumSize(self):
		return self.minimumSize
	def getPreferredSize(self):
		return self.preferredSize
	class NoToggle(Action):
		def execute(self):
			Blender.Draw.Redraw(1)
			return

class Screen:
	"""Represents the complete script window.
	
	   A screen represents the complete script window. It handles
	   drawing and events and can consist of several user interface
	   components.
	   
	   @cvar current Current active screen.
	"""
	current = None
	def __init__(self):
		"""Constructor.
		"""
		# buttonHandler event number management
		self.nButtonEvent = 0
		# buttonEventDict key: iButtonEvent, value: Action
		self.buttonEventDict = {}
		# default widget layout
		self.layout = VerticalLayout()
		return
	def activate(self):
		"""Makes the current screen active.
		
		   This method calls Blender.Draw.Register to register itself to
		   be responsible for windowing.
		"""
		self.layout.addButtons(self)
		Screen.current = self
		Blender.Draw.Register(self._draw, self._eventHandler, self._buttonHandler)
		return
	def addWidget(self, widget, name):
		"""Adds a widget to this screen.
		
		   @param widget Widget to add to this screen.
		   @param name Unique name for the widget.
		"""
		self.layout.addWidget(widget, name)
		return
	def removeWidget(self, name):
		"""Removes widget from this screen.
		
		   @param name Name of the widget to remove.
		   @see addWidget
		"""
		self.layout.removeWidget(name)
		return
	def addButtonAction(self, action):
		"""Registers an action for a button event.
		
		   @param action Action to execute on receive of the returned button event number.
		   @return eventNumber Event number to use for the button that corresponds to that action.
		"""
		# workaround for Blender 2.37 event 8 bug:
		shiftEvents = 100
		# get a free event number
		if (len(self.buttonEventDict) == self.nButtonEvent):
			self.nButtonEvent += 1
			eventNumber = self.nButtonEvent + shiftEvents
		else:
			eventNumber = [(x+1+shiftEvents) for x in range(self.nButtonEvent) if (x+1+shiftEvents) not in self.buttonEventDict.keys()][0]
		# assign action to that event
		self.buttonEventDict[eventNumber] = action
		return eventNumber
	def removeButtonAction(self, eventNumber):
		"""Action for the given event number will no longer be called.
		
		   @param eventNumber Event number for the action.
		"""
		if self.buttonEventDict.has_key(eventNumber):
			del self.buttonEventDict[eventNumber]
		return
	# callbacks for Blender.Draw.Register
	def _draw(self):
		"""Draws the screen.
		
		   Callback function for Blender.Draw.Register
		"""
		# clear background
		theme = Blender.Window.Theme.Get()[0]
		bgColor = [color/255.0 for color in theme.get('buts').back]
		glClearColor(*bgColor)
		glClear(GL_COLOR_BUFFER_BIT)

		# size of the script window
		size = list(Blender.Window.GetAreaSize())
		minimumSize = self.layout.getMinimumSize()
		if size[0] < minimumSize[0]:
			size[0] = minimumSize[0]
		if size[1] < minimumSize[1]:
			size[1] = minimumSize[1]
		screenRect = [0, 0, size[0]-1, size[1]-1]
		
		# draw widgets
		self.layout.draw(screenRect)
		return
	def _eventHandler(self, event, value):
		"""Handles keyboard and mouse input events.
		
		   Callback function for Blender.Draw.Register
		"""
		self.layout.eventFilter(event, value)
		return
	def _buttonHandler(self, event):
		"""Handles draw button events.
		
		   Callback function for Blender.Draw.Register
		"""
		if self.buttonEventDict.has_key(event):
			self.buttonEventDict[event].execute()
		return
	
###
# specific classes
class OgreFrame(Decorator):
	"""Ogre Logo, Title and border.
	"""
	OGRE_LOGO = Buffer(GL_BYTE, [48,122*4],[[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,64,0,0,0,95,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,95,0,0,0,127,0,0,0,127,0,1,0,127,0,2,0,127,2,5,2,127,2,5,2,127,4,6,4,127,5,8,5,127,8,11,8,127,8,11,8,127,3,5,3,127,2,3,2,127,0,1,0,127,0,1,0,127,0,1,0,127,0,1,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,64,0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,95,0,0,0,127,1,2,1,127,4,6,4,127,10,13,10,127,18,22,18,127,23,28,23,127,24,30,24,127,25,31,25,127,25,31,25,127,26,32,26,127,26,32,26,127,26,32,26,127,25,31,25,127,24,30,24,127,18,23,18,127,3,5,3,127,4,6,4,127,8,11,8,127,9,12,9,127,13,17,13,127,17,22,17,127,15,19,15,127,7,9,7,127,1,2,1,127,0,0,0,127,0,0,0,95,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,127,2,4,2,127,4,6,4,127,18,22,18,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,18,22,18,127,15,19,15,127,20,26,20,127,25,31,25,127,26,32,26,127,26,32,26,127,25,31,25,127,25,31,25,127,25,31,25,127,26,32,26,127,24,30,24,127,16,20,16,127,4,5,4,127,0,0,0,95,0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,95,1,1,1,127,13,15,13,127,12,15,12,127,24,29,24,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,23,29,23,127,24,30,24,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,23,28,23,127,3,5,3,127,0,0,0,127,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,95,1,1,1,127,19,24,19,127,11,15,11,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,23,28,23,127,17,21,17,127,22,28,22,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,23,28,23,127,3,5,3,127,0,0,0,127,0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,127,20,24,20,127,16,20,16,127,20,25,20,127,24,30,24,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,22,28,22,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,23,28,23,127,3,5,3,127,0,0,0,127,0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,64,5,7,5,127,26,32,26,127,15,19,15,127,41,48,41,127,38,45,38,127,24,30,24,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,23,28,23,127,3,4,3,127,0,0,0,127,58,66,58,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,127,20,24,20,127,27,34,27,127,26,32,26,127,47,55,47,127,47,55,47,127,39,46,39,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,11,16,11,127,0,1,0,127,3,3,3,127,94,106,94,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,127,33,39,33,127,45,52,45,127,28,32,28,127,47,55,47,127,44,51,44,127,39,46,39,127,27,33,27,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,21,26,21,127,0,2,0,127,0,0,0,127,23,26,23,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,127,24,28,24,127,33,40,33,127,18,22,18,127,29,35,29,127,25,31,25,127,24,30,24,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,5,8,5,127,1,2,1,127,0,0,0,127,70,79,70,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,94,105,94,127,70,79,70,127,76,86,76,127,90,101,90,127,103,116,103,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,64,0,0,0,127,4,6,4,127,12,16,12,127,22,27,22,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,23,29,23,127,28,34,28,127,35,42,35,127,28,35,28,127,25,31,25,127,23,29,23,127,23,29,23,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,17,21,17,127,0,2,0,127,0,0,0,127,31,36,31,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,100,112,100,127,92,103,92,127,103,116,103,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,100,112,100,127,81,92,81,127,68,77,68,127,65,73,65,127,65,73,65,127,76,86,76,127,78,88,78,127,83,94,83,127,92,103,92,127,85,95,85,127,31,35,31,127,6,7,6,127,6,7,6,127,13,14,13,127,13,14,13,127,19,21,19,127,26,29,26,127,26,29,26,127,48,54,48,127,96,108,96,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,70,78,70,127,3,3,3,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,12,13,11,127,23,26,23,127,36,40,36,127,49,55,49,127,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,64,0,0,0,127,2,4,2,127,16,20,16,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,24,30,24,127,26,33,26,127,59,68,59,127,81,91,81,127,87,98,87,127,86,96,86,127,80,90,80,127,71,79,71,127,59,66,59,127,36,41,35,127,23,29,23,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,24,31,24,127,26,32,26,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,5,8,5,127,0,1,0,127,18,20,18,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,91,103,91,127,58,65,58,127,29,33,29,127,6,7,6,127,0,0,0,127,0,0,0,127,1,2,1,127,22,24,22,127,54,61,54,127,94,106,94,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,88,99,88,127,51,58,51,127,18,21,18,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,17,19,17,127,48,54,48,127,80,91,80,127,102,115,102,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,29,33,29,127,0,0,0,127,41,31,14,127,33,25,11,127,18,14,6,127,2,2,1,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,127,2,3,2,127,24,29,24,127,26,32,26,127,24,30,24,127,25,31,25,127,24,30,24,127,24,30,24,127,24,30,24,127,23,29,23,127,34,41,34,127,78,88,78,127,87,98,87,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,87,97,87,127,87,97,87,127,84,93,84,127,62,69,62,127,34,40,34,127,24,30,24,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,23,28,23,127,26,30,26,127,36,38,36,127,47,50,46,127,39,42,37,127,34,40,34,127,30,37,30,127,24,30,24,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,15,19,15,127,0,1,0,127,0,0,0,127,102,115,102,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,94,106,94,127,43,48,43,127,4,5,4,127,0,0,0,127,0,0,0,127,0,0,0,127,6,5,2,127,16,12,5,127,2,2,1,127,0,0,0,127,0,0,0,127,7,8,7,127,58,65,58,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,96,108,96,127,41,47,41,127,1,1,1,127,0,0,0,127,0,0,0,127,6,5,2,127,27,21,9,127,42,33,14,127,46,36,16,127,46,36,16,127,33,25,11,127,31,24,11,127,25,19,9,127,16,12,5,127,12,9,4,127,0,0,0,127,107,82,36,127,115,88,38,127,107,82,36,127,107,82,36,127,100,76,33,127,92,71,31,127,88,68,30,127,0,0,0,127,4,3,2,127,0,0,0,127,0,0,0,127,0,0,0,127,13,15,13,127,65,73,65,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,13,14,13,127,0,0,0,127,107,82,36,127,122,94,41,127,122,94,41,127,122,94,41,127,109,84,36,127,96,73,32,127,80,62,27,127,65,50,22,127,52,40,17,127,37,28,12,127,21,16,7,127,2,2,1,127,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,127,9,11,9,127,48,56,48,127,45,53,45,127,41,48,41,127,33,40,33,127,34,41,34,127,37,44,37,127,54,62,54,127,77,87,77,127,87,97,87,127,87,97,87,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,79,88,79,127,61,69,61,127,25,31,25,127,25,31,25,127,23,28,23,127,19,23,19,127,42,43,41,127,60,60,59,127,61,61,59,127,61,61,59,127,63,63,61,127,35,37,34,127,38,45,38,127,33,39,33,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,15,19,15,127,0,1,0,127,0,0,0,127,102,115,102,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,81,91,81,127,9,11,9,127,0,0,0,127,2,2,1,127,44,34,15,127,86,66,29,127,115,88,38,127,122,94,41,127,122,94,41,127,121,92,40,127,94,72,31,127,39,30,13,127,0,0,0,127,0,0,0,127,40,45,40,127,101,114,101,127,105,118,105,127,105,118,105,127,105,118,105,127,85,95,85,127,11,13,11,127,0,0,0,127,4,3,2,127,50,38,17,127,94,72,31,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,92,71,31,127,0,0,0,127,107,82,36,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,100,76,33,127,2,2,1,127,105,81,35,127,98,75,33,127,60,46,20,127,23,18,8,127,0,0,0,127,1,1,1,127,90,102,90,127,105,118,105,127,105,118,105,127,105,118,105,127,6,7,6,127,0,0,0,127,115,88,38,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,8,6,3,127,0,0,0,95,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,127,3,5,3,127,45,53,45,127,46,54,46,127,46,54,46,127,47,55,47,127,46,54,46,127,68,78,68,127,87,98,87,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,87,98,87,127,67,76,67,127,38,46,38,127,21,26,21,127,50,52,50,127,60,60,59,127,61,61,59,127,60,60,58,127,60,60,58,127,60,60,58,127,61,61,59,127,39,41,38,127,52,59,52,127,67,76,67,127,23,29,23,127,25,31,25,127,25,31,25,127,25,31,25,127,15,19,15,127,0,1,0,127,0,0,0,127,102,115,102,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,59,67,59,127,1,1,1,127,0,0,0,127,35,27,12,127,105,81,35,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,86,66,29,127,8,6,3,127,0,0,0,127,36,40,36,127,105,118,105,127,105,118,105,127,82,92,82,127,7,7,7,127,0,0,0,127,31,24,10,127,107,82,36,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,80,62,27,127,0,0,0,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,82,63,28,127,46,36,16,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,27,21,9,127,0,0,0,127,78,88,78,127,105,118,105,127,105,118,105,127,105,118,105,127,0,0,0,127,0,0,0,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,100,76,33,127,0,0,0,127,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,127,0,2,0,127,41,49,41,127,46,54,46,127,46,54,46,127,49,56,49,127,77,87,77,127,87,98,87,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,85,96,85,127,55,64,55,127,44,52,44,127,23,28,23,127,17,22,17,127,90,92,90,127,84,84,82,127,60,60,58,127,60,60,58,127,60,60,58,127,60,60,58,127,61,61,59,127,39,41,38,127,54,62,54,127,62,71,62,127,23,29,23,127,25,31,25,127,25,31,25,127,25,31,25,127,15,20,15,127,0,1,0,127,0,0,0,127,102,115,102,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,81,90,81,127,1,1,1,127,0,0,0,127,61,47,21,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,103,79,34,127,12,9,4,127,0,0,0,127,47,52,47,127,93,104,93,127,8,9,8,127,0,0,0,127,52,40,17,127,121,92,40,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,77,59,26,127,0,0,0,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,63,49,21,127,105,81,35,127,122,94,41,127,122,94,41,127,122,94,41,127,100,76,33,127,0,0,0,127,9,11,9,127,101,113,101,127,105,118,105,127,105,118,105,127,105,118,105,127,0,0,0,127,0,0,0,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,69,53,23,127,0,0,0,127,0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,127,0,1,0,127,37,44,37,127,46,54,46,127,49,57,49,127,79,89,79,127,87,97,87,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,56,64,56,127,46,53,46,127,25,31,25,127,22,27,22,127,25,31,25,127,44,47,44,127,116,116,115,127,59,59,57,127,60,60,58,127,60,60,58,127,60,60,58,127,61,61,59,127,38,41,37,127,69,78,69,127,45,53,45,127,24,30,24,127,25,31,25,127,25,31,25,127,25,31,25,127,15,20,15,127,0,0,0,127,5,6,5,127,104,117,104,127,105,118,105,127,105,118,105,127,105,118,105,127,93,104,93,127,8,9,8,127,0,0,0,127,61,47,21,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,96,73,32,127,2,2,1,127,0,0,0,127,24,28,24,127,0,0,0,127,37,28,12,127,121,92,40,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,77,59,26,127,10,8,3,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,88,68,30,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,39,30,13,127,0,0,0,127,43,49,43,127,105,118,105,127,105,118,105,127,105,118,105,127,93,105,93,127,0,0,0,127,14,11,5,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,39,30,13,127,0,0,0,127,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,64,0,1,0,127,21,25,21,127,48,57,49,127,82,92,82,127,87,97,87,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,87,97,87,127,87,98,87,127,60,69,60,127,43,50,43,127,29,36,29,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,116,116,116,127,71,71,70,127,60,60,58,127,60,60,58,127,60,60,58,127,62,62,60,127,30,32,29,127,75,85,75,127,29,36,29,127,25,31,25,127,24,30,24,127,24,30,24,127,23,28,23,127,10,14,10,127,0,0,0,127,40,45,40,127,105,118,105,127,105,118,105,127,105,118,105,127,105,118,105,127,33,38,33,127,0,0,0,127,39,30,13,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,67,52,23,127,0,0,0,127,0,0,0,127,10,8,3,127,113,87,38,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,107,82,36,127,84,65,28,127,71,54,24,127,115,88,38,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,67,51,22,127,16,12,5,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,121,92,40,127,122,94,41,127,122,94,41,127,122,94,41,127,105,81,35,127,2,2,1,127,0,0,0,127,0,0,0,127,18,21,18,127,61,69,61,127,102,115,102,127,92,103,92,127,0,0,0,127,16,12,5,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,60,46,20,127,52,40,17,127,69,53,23,127,86,66,29,127,10,8,3,127,0,0,0,127,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,127,2,5,2,127,49,57,49,127,87,98,87,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,87,98,87,127,86,97,86,127,75,84,75,127,53,61,53,127,34,41,34,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,22,28,22,127,96,97,96,127,93,93,92,127,59,59,58,127,60,60,58,127,60,60,58,127,61,61,59,127,34,39,34,127,74,84,74,127,23,29,23,127,25,31,25,127,37,39,34,127,47,47,41,127,44,45,39,127,17,18,16,127,0,0,0,127,52,59,52,127,105,118,105,127,105,118,105,127,105,118,105,127,81,92,81,127,0,0,0,127,8,6,3,127,111,85,37,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,121,92,40,127,50,38,17,127,16,12,5,127,33,25,11,127,103,79,34,127,122,94,41,127,122,94,41,127,122,94,41,127,121,92,40,127,23,18,8,127,0,0,0,127,69,53,23,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,77,59,26,127,27,21,9,127,0,0,0,127,0,0,0,127,0,0,0,127,92,71,31,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,61,47,21,127,18,14,6,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,117,90,39,127,88,68,29,127,54,41,18,127,14,11,5,127,0,0,0,127,0,0,0,127,17,18,17,127,68,76,68,127,0,0,0,127,21,16,7,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,31,24,11,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,95,0,0,0,127,37,43,37,127,89,100,89,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,87,97,87,127,88,99,88,127,82,92,82,127,61,69,61,127,36,42,36,127,27,32,27,127,23,29,23,127,23,29,23,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,23,29,23,127,78,80,76,127,102,102,102,127,58,58,57,127,60,60,58,127,60,60,58,127,58,58,56,127,40,47,40,127,56,64,56,127,24,29,23,127,44,45,40,127,49,49,43,127,49,49,43,127,46,46,41,127,41,42,37,127,0,0,0,127,38,43,38,127,105,118,105,127,105,118,105,127,105,118,105,127,33,37,33,127,0,0,0,127,61,47,21,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,77,59,26,127,0,0,0,127,0,0,0,127,0,0,0,127,12,9,4,127,113,87,38,127,122,94,41,127,122,94,41,127,122,94,41,127,84,65,28,127,4,3,2,127,115,88,38,127,122,94,41,127,122,94,41,127,122,94,41,127,121,92,40,127,42,33,14,127,0,0,0,127,119,91,40,127,102,78,34,127,75,57,25,127,52,40,17,127,88,68,29,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,61,47,21,127,31,24,11,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,121,92,40,127,84,65,28,127,19,15,7,127,0,0,0,127,4,5,4,127,0,0,0,127,31,24,11,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,111,85,37,127,115,88,38,127,122,94,41,127,122,94,41,127,48,37,16,127,0,0,0,127,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,32,0,0,0,127,6,7,5,127,67,75,67,127,89,100,89,127,87,97,87,127,87,97,87,127,87,98,87,127,88,99,88,127,88,98,88,127,80,90,80,127,62,71,62,127,45,52,45,127,39,46,39,127,57,65,57,127,65,74,65,127,59,67,59,127,54,61,54,127,55,61,55,127,28,34,28,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,24,30,24,127,64,67,64,127,109,109,108,127,58,58,57,127,60,60,58,127,61,60,59,127,50,50,47,127,47,55,47,127,33,39,33,127,44,44,39,127,48,48,42,127,48,48,42,127,28,30,25,127,36,37,31,127,48,48,42,127,1,2,1,127,36,41,36,127,105,118,105,127,105,118,105,127,99,111,99,127,4,5,4,127,2,2,1,127,113,87,38,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,65,50,22,127,0,0,0,127,30,34,30,127,27,30,27,127,0,0,0,127,67,51,22,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,58,44,19,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,71,54,24,127,0,0,0,127,18,14,6,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,54,41,18,127,31,24,11,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,121,92,40,127,56,43,19,127,0,0,0,127,0,0,0,127,31,24,11,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,37,28,12,127,0,0,0,127,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,95,0,0,0,127,2,3,2,127,28,32,28,127,58,65,58,127,56,64,56,127,50,57,50,127,46,54,46,127,42,49,42,127,43,50,43,127,62,71,62,127,80,90,80,127,87,98,87,127,87,98,87,127,87,97,87,127,87,98,87,127,86,97,87,127,78,85,78,127,46,52,46,127,24,30,24,127,25,31,25,127,25,31,25,127,25,31,25,127,24,30,24,127,64,67,64,127,104,104,104,127,58,58,57,127,60,60,58,127,62,61,60,127,34,38,33,127,37,43,37,127,50,51,44,127,48,48,42,127,48,48,42,127,23,27,22,127,32,36,30,127,95,95,82,127,43,45,39,127,0,0,0,127,45,51,45,127,105,118,105,127,105,118,105,127,71,80,71,127,0,0,0,127,35,27,12,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,103,79,35,127,2,2,1,127,0,0,0,127,11,13,11,127,0,0,0,127,65,50,22,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,121,92,40,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,23,18,8,127,0,0,0,127,35,27,12,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,46,36,16,127,41,31,14,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,119,91,40,127,37,28,12,127,50,38,17,127,73,56,24,127,107,82,36,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,69,53,23,127,0,0,0,127,44,34,15,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,27,21,9,127,0,0,0,127,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,8,10,8,127,51,59,51,127,84,95,84,127,87,98,87,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,87,127,63,71,63,127,23,29,23,127,25,31,25,127,25,31,25,127,25,31,25,127,23,29,23,127,76,78,75,127,100,100,99,127,58,58,57,127,61,60,59,127,53,54,51,127,24,30,24,127,29,33,28,127,77,76,63,127,47,48,42,127,29,32,27,127,24,30,24,127,30,35,29,127,90,91,84,127,28,29,25,127,0,0,0,127,77,86,76,127,105,118,105,127,105,118,105,127,44,50,44,127,0,0,0,127,69,53,23,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,81,62,27,127,4,3,2,127,0,0,0,127,12,9,4,127,107,82,36,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,4,3,2,127,0,0,0,127,54,41,18,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,46,36,16,127,46,36,16,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,100,76,33,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,48,37,16,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,42,33,14,127,46,36,16,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,16,12,5,127,0,0,0,127,0,0,0,127,4,3,2,127,6,5,2,127,0,0,0,95,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,64,0,0,0,95,1,1,1,127,60,68,60,127,87,98,87,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,87,97,87,127,73,82,73,127,24,30,24,127,25,31,25,127,25,31,25,127,25,31,25,127,22,28,22,127,89,92,89,127,87,87,86,127,59,59,58,127,60,59,58,127,31,35,31,127,25,31,25,127,43,45,38,127,74,74,62,127,43,43,38,127,22,28,22,127,25,31,25,127,24,30,24,127,26,32,26,127,13,14,12,127,0,0,0,127,100,113,100,127,105,118,105,127,105,118,105,127,21,24,21,127,0,0,0,127,98,75,33,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,113,87,38,127,92,71,31,127,117,90,39,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,19,15,7,127,0,0,0,127,71,54,24,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,39,30,13,127,50,38,17,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,82,63,28,127,0,0,0,127,23,26,23,127,38,42,38,127,5,7,5,127,0,0,0,127,96,73,32,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,111,85,37,127,54,41,18,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,82,63,28,127,16,12,5,127,16,12,5,127,16,12,5,127,12,9,4,127,46,35,16,127,82,63,28,127,117,90,39,127,46,36,16,127,0,0,0,127,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,127,33,38,33,127,89,99,89,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,84,94,84,127,28,35,28,127,25,31,25,127,25,31,25,127,25,31,25,127,22,28,22,127,100,101,100,127,73,73,71,127,61,60,59,127,35,38,35,127,24,30,24,127,24,30,24,127,48,51,41,127,69,69,57,127,36,37,32,127,24,30,24,127,28,34,28,127,25,31,25,127,25,31,25,127,17,21,17,127,0,0,0,127,80,90,80,127,105,118,105,127,105,118,105,127,6,7,6,127,0,0,0,127,115,88,38,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,56,43,19,127,0,0,0,127,88,68,29,127,117,90,39,127,107,82,36,127,92,71,31,127,80,62,27,127,69,53,23,127,60,46,20,127,46,36,16,127,33,25,11,127,23,18,8,127,4,3,2,127,61,47,21,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,65,50,22,127,0,0,0,127,20,22,20,127,26,30,26,127,0,0,0,127,2,2,1,127,109,84,36,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,100,76,33,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,77,59,26,127,21,16,7,127,60,46,20,127,94,72,31,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,54,41,18,127,0,0,0,127,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,95,6,7,6,127,81,91,81,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,88,98,88,127,60,68,60,127,27,33,27,127,24,30,24,127,25,31,25,127,22,28,22,127,91,91,91,127,57,58,56,127,31,36,31,127,24,30,24,127,25,31,25,127,25,31,25,127,27,31,26,127,70,71,58,127,41,42,36,127,37,43,37,127,66,74,66,127,23,29,23,127,25,31,25,127,19,22,19,127,0,0,0,127,75,84,75,127,105,118,105,127,102,114,102,127,0,0,0,127,4,3,2,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,117,90,39,127,31,24,10,127,2,2,1,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,4,3,2,127,61,47,21,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,46,36,16,127,0,0,0,127,0,0,0,127,0,0,0,127,8,6,3,127,73,56,24,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,121,92,40,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,61,47,21,127,0,0,0,127,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,127,45,52,45,127,87,98,88,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,82,92,82,127,46,54,46,127,34,41,34,127,25,31,25,127,25,31,25,127,26,30,26,127,24,30,24,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,24,30,24,127,33,37,31,127,48,48,42,127,43,43,38,127,66,74,65,127,23,29,23,127,25,31,25,127,20,25,20,127,0,0,0,127,70,78,70,127,105,118,105,127,92,103,92,127,0,0,0,127,16,12,5,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,86,66,29,127,48,37,16,127,31,24,11,127,16,12,5,127,23,18,8,127,33,25,11,127,52,40,17,127,71,54,24,127,96,73,32,127,117,90,39,127,63,49,21,127,73,56,24,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,88,68,29,127,77,59,26,127,77,59,26,127,90,69,30,127,117,90,39,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,73,56,24,127,0,0,0,127,0,0,0,32],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,95,25,28,25,127,88,99,88,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,86,97,86,127,87,98,87,127,70,79,70,127,46,54,46,127,47,55,47,127,45,52,45,127,30,37,30,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,24,30,24,127,44,52,44,127,72,81,72,127,70,79,70,127,23,29,23,127,25,31,25,127,21,25,21,127,0,0,0,127,66,73,65,127,105,118,105,127,92,103,92,127,0,0,0,127,16,12,5,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,80,62,27,127,77,59,26,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,77,59,26,127,0,0,0,127,0,0,0,64],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,1,0,127,64,72,64,127,87,97,87,127,86,97,86,127,86,97,86,127,87,97,87,127,86,97,86,127,86,96,86,127,85,95,85,127,71,80,71,127,47,55,47,127,46,54,46,127,46,54,46,127,46,54,46,127,47,55,47,127,31,38,31,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,23,29,23,127,59,67,59,127,77,87,77,127,58,66,58,127,25,31,25,127,25,31,25,127,22,27,22,127,0,0,0,127,48,54,48,127,105,118,105,127,92,103,92,127,0,0,0,127,16,12,5,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,98,75,33,127,80,62,27,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,92,71,31,127,0,0,0,127,0,0,0,64],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,127,14,16,14,127,88,99,88,127,88,98,88,127,88,98,88,127,72,82,72,127,51,59,51,127,52,61,52,127,55,63,55,127,47,55,47,127,45,53,45,127,45,53,45,127,46,54,46,127,46,54,46,127,46,54,46,127,45,53,45,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,37,44,37,127,76,86,76,127,73,82,73,127,32,39,32,127,23,29,23,127,2,2,2,127,30,34,30,95,105,118,105,64,98,111,98,64,0,0,0,95,4,3,2,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,115,88,38,127,92,71,31,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,119,91,40,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,98,75,33,127,0,0,0,127,0,0,0,64],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,127,21,24,21,127,55,62,55,127,51,57,50,127,64,72,64,127,86,96,86,127,85,95,85,127,84,94,84,127,86,96,86,127,84,95,84,127,82,92,82,127,75,85,75,127,52,60,52,127,46,54,46,127,46,54,46,127,45,53,45,127,26,32,26,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,29,36,29,127,28,34,28,127,24,30,24,127,62,71,62,127,88,99,88,127,66,75,66,127,24,30,24,127,8,11,8,127,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,127,105,81,35,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,98,75,33,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,121,92,40,127,100,76,33,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,107,82,36,127,0,0,0,127,0,0,0,64],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,127,31,36,31,127,35,40,35,127,33,36,32,127,31,34,31,127,47,55,47,127,51,59,51,127,47,55,47,127,39,46,39,127,29,36,29,127,37,43,37,127,52,60,52,127,77,87,77,127,49,58,49,127,46,54,46,127,40,48,40,127,24,30,24,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,29,35,29,127,80,90,80,127,59,67,59,127,24,30,24,127,24,30,24,127,76,86,76,127,87,98,87,127,39,46,39,127,17,22,17,127,0,0,0,127,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,127,75,57,25,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,79,60,26,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,113,87,38,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,71,55,24,127,103,79,35,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,117,90,39,127,0,0,0,127,0,0,0,64],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,127,41,48,41,127,69,79,69,127,39,45,39,127,47,54,47,127,77,87,77,127,86,97,86,127,88,97,87,127,87,97,86,127,82,93,83,127,57,65,57,127,25,31,25,127,24,30,24,127,26,32,26,127,26,32,26,127,26,32,26,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,75,85,75,127,87,98,87,127,67,75,67,127,23,29,23,127,23,29,23,127,56,64,56,127,85,95,85,127,75,84,75,127,24,30,24,127,3,3,3,127,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,127,29,22,10,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,119,91,40,127,8,6,3,127,109,84,36,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,119,91,40,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,98,75,33,127,6,5,2,127,107,82,36,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,0,0,0,127,0,0,0,64],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,127,12,15,12,127,45,53,46,127,48,56,48,127,65,72,63,127,98,81,79,127,123,119,119,127,117,108,108,127,94,79,76,127,88,88,80,127,64,73,64,127,24,30,24,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,35,41,35,127,86,96,86,127,87,98,87,127,61,69,61,127,23,29,23,127,24,30,24,127,46,53,46,127,84,94,84,127,87,98,87,127,55,63,55,127,10,12,10,127,0,0,0,95,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,127,92,71,31,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,75,57,25,127,0,0,0,127,52,40,17,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,98,75,33,127,12,9,4,127,0,0,0,127,109,84,36,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,14,11,5,127,0,0,0,95],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,127,22,26,22,127,30,37,30,127,23,29,23,127,41,40,35,127,91,73,72,127,113,103,103,127,100,75,75,127,87,58,58,127,83,72,66,127,54,63,55,127,23,29,23,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,30,25,127,34,41,34,127,69,78,69,127,81,91,81,127,34,41,34,127,25,31,25,127,23,29,23,127,61,69,61,127,82,92,82,127,75,85,75,127,82,92,82,127,24,29,24,127,1,1,1,127,0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,127,23,18,8,127,119,91,40,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,121,92,40,127,17,14,6,127,0,0,0,127,2,2,1,127,96,73,32,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,75,58,25,127,6,5,2,127,0,0,0,127,0,0,0,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,18,14,6,127,0,0,0,127],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,127,24,29,24,127,48,56,48,127,28,34,28,127,24,30,24,127,25,31,25,127,36,37,32,127,68,55,52,127,82,63,62,127,80,52,52,127,81,82,74,127,28,34,28,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,24,30,24,127,23,29,23,127,25,31,25,127,24,30,24,127,25,31,25,127,24,29,24,127,56,64,56,127,87,97,87,127,70,79,70,127,88,99,88,127,49,57,49,127,10,12,10,127,0,0,0,95,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,127,44,34,15,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,67,52,23,127,0,0,0,127,0,0,0,95,0,0,0,127,12,9,4,127,109,84,36,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,100,76,33,127,33,25,11,127,0,0,0,127,0,0,0,127,0,0,0,95,0,0,0,127,107,82,36,127,117,90,39,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,31,24,11,127,0,0,0,127],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,95,14,16,14,127,81,91,81,127,72,81,72,127,43,51,43,127,23,29,23,127,24,30,24,127,23,30,24,127,23,30,23,127,25,31,25,127,26,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,26,32,26,127,32,39,32,127,30,37,30,127,24,30,24,127,25,31,25,127,25,31,25,127,25,32,25,127,83,93,83,127,77,86,77,127,87,97,87,127,80,90,80,127,22,27,22,127,1,1,1,127,0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,127,46,35,15,127,121,92,40,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,96,73,32,127,4,3,2,127,0,0,0,95,0,0,0,0,0,0,0,32,0,0,0,127,12,9,4,127,98,75,33,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,96,73,32,127,40,31,14,127,2,2,1,127,0,0,0,127,0,0,0,64,0,0,0,0,0,0,0,32,0,0,0,127,0,0,0,127,0,0,0,127,2,2,1,127,16,12,5,127,25,19,9,127,33,25,11,127,46,36,16,127,56,43,19,127,61,47,21,127,77,59,26,127,84,65,28,127,92,71,31,127,107,82,36,127,115,88,38,127,122,94,41,127,122,94,41,127,39,30,13,127,0,0,0,127],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,127,18,21,18,127,83,93,83,127,89,100,89,127,71,81,71,127,54,61,54,127,37,44,37,127,24,30,24,127,23,29,23,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,24,30,24,127,42,50,42,127,70,79,70,127,87,98,87,127,74,83,74,127,28,35,28,127,25,31,25,127,24,30,24,127,42,49,42,127,76,86,76,127,86,97,86,127,88,99,88,127,41,49,41,127,11,14,11,127,0,0,0,95,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,127,27,21,9,127,105,81,35,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,98,75,33,127,12,9,4,127,0,0,0,95,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,127,2,2,1,127,58,44,19,127,113,87,38,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,105,81,35,127,63,49,21,127,21,16,7,127,0,0,0,127,0,0,0,127,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,16,12,5,127,6,5,2,127,0,0,0,95],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,127,13,17,13,127,61,70,61,127,85,96,85,127,89,100,89,127,88,98,88,127,77,87,77,127,60,67,60,127,26,32,26,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,25,31,25,127,12,16,12,127,12,15,12,127,40,46,40,127,80,90,80,127,80,89,80,127,34,40,34,127,24,30,24,127,23,29,23,127,51,59,51,127,88,99,88,127,86,97,86,127,76,85,76,127,22,27,22,127,1,2,1,127,0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,127,4,3,2,127,59,46,20,127,111,85,37,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,119,91,40,127,65,50,22,127,4,3,2,127,0,0,0,127,0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,95,0,0,0,127,4,3,2,127,44,34,15,127,80,62,27,127,111,85,37,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,122,94,41,127,121,92,40,127,100,76,33,127,75,57,25,127,48,37,16,127,18,13,6,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,127,0,0,0,64,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,127,19,23,19,127,46,53,46,127,64,72,64,127,80,90,80,127,85,96,85,127,74,84,74,127,28,34,28,127,25,31,25,127,25,31,25,127,25,30,25,127,25,31,25,127,25,31,25,127,25,31,25,127,17,21,17,127,1,3,1,127,0,1,0,127,0,0,0,127,9,11,9,127,51,59,52,127,82,93,83,127,45,52,45,127,23,29,23,127,24,30,24,127,59,67,59,127,88,99,88,127,85,96,85,127,30,37,30,127,12,15,12,127,0,0,0,95,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,95,0,0,0,127,4,3,2,127,42,33,14,127,82,63,28,127,107,82,36,127,103,79,35,127,84,65,28,127,54,41,18,127,12,9,4,127,0,0,0,127,0,0,0,95,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,95,0,0,0,127,0,0,0,127,0,0,0,127,10,8,3,127,25,19,9,127,31,24,11,127,31,24,11,127,31,24,11,127,31,24,11,127,18,14,6,127,35,27,12,127,105,81,35,127,80,62,27,127,54,41,18,127,29,22,10,127,6,5,2,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,95,0,0,0,64,0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,127,8,10,8,127,33,39,33,127,44,51,44,127,46,53,46,127,44,52,44,127,39,46,39,127,25,30,25,127,25,31,25,127,25,31,25,127,24,30,24,127,15,19,15,127,5,7,5,127,0,1,0,127,0,0,0,127,0,0,0,95,0,0,0,64,0,0,0,64,0,1,0,127,21,24,21,127,66,74,66,127,57,66,57,127,24,30,24,127,23,29,23,127,52,60,52,127,40,47,40,127,24,30,24,127,23,28,23,127,1,2,1,127,0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,95,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,95,0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,64,0,0,0,95,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,64,0,0,0,64,0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,95,0,0,0,127,11,13,11,127,23,28,23,127,33,39,33,127,36,43,36,127,23,29,23,127,20,26,20,127,11,15,11,127,3,4,3,127,0,1,0,127,0,0,0,127,0,0,0,95,0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,95,3,5,3,127,37,41,37,127,58,66,58,127,27,33,27,127,24,30,24,127,26,32,26,127,25,31,25,127,25,31,25,127,8,9,8,127,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,64,0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,95,0,0,0,127,0,0,0,127,0,0,0,127,0,1,0,127,0,0,0,127,0,0,0,127,0,0,0,127,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,127,12,15,12,127,42,49,42,127,32,39,32,127,24,30,24,127,25,31,25,127,25,31,25,127,18,22,18,127,0,0,0,127,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,64,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,2,2,2,127,23,27,23,127,37,43,37,127,26,33,26,127,25,31,25,127,24,30,24,127,4,4,4,127,0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,95,4,5,4,127,24,28,23,127,29,35,29,127,25,31,25,127,12,16,12,127,0,0,0,95,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,95,4,4,4,127,11,14,11,127,16,20,16,127,0,0,0,127,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32,0,0,0,64,0,0,0,127,0,0,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]])
	def __init__(self, childWidget, title):
		"""Constructor.
		
		   @param title Header title.
		"""
		Decorator.__init__(self, childWidget)
		self.title = title
		self.border = 10
		return
	def draw(self, screenRectangle):
		rect = screenRectangle[:]
		rect[0] += self.border
		rect[1] += self.border
		rect[2] -= self.border
		rect[3] -= self.border
		# title
		glColor3ub(210, 236, 210)
		glRecti(rect[0],rect[3]-41,rect[2],rect[3]-17)
		glColor3ub(50, 62, 50)
		glRasterPos2i(rect[0]+126, rect[3]-34)
		Blender.Draw.Text(self.title)
		glRasterPos2i(rect[0]+127, rect[3]-34)
		Blender.Draw.Text(self.title)
		# logo
		glRasterPos2i(rect[0]+1, rect[3]-48)	
		glEnable(GL_BLEND)
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
		glDrawPixels(122, 48, GL_RGBA, GL_BYTE, OgreFrame.OGRE_LOGO)
		rect[3] -= 60
		glDisable(GL_BLEND)
		# child
		self.childWidget.draw(rect)
		return
	def getMaximumSize(self):
		size = self.childWidget.getMaximumSize()[:]
		size[0] += 2*self.border
		size[1] += 2*self.border + 60
		width = 2*self.border + Blender.Draw.GetStringWidth(self.title) + 85
		if size[0] < width:
			size[0] = width
		height = 60 + 2*self.border
		if size[1] < height:
			size[1] = height
		return size
	def getMinimumSize(self):
		size = self.childWidget.getMinimumSize()[:]
		size[0] += 2*self.border
		size[1] += 2*self.border + 60
		width = 2*self.border + Blender.Draw.GetStringWidth(self.title) + 85
		if size[0] < width:
			size[0] = width
		height = 60 + 2*self.border
		if size[1] < height:
			size[1] = height
		return size
	def getPreferredSize(self):
		size = self.childWidget.getPreferredSize()[:]
		size[0] += 2*self.border
		size[1] += 2*self.border + 60
		width = 2*self.border + Blender.Draw.GetStringWidth(self.title) + 85
		if size[0] < width:
			size[0] = width
		height = 60 + 2*self.border
		if size[1] < height:
			size[1] = height
		return size

class RoundedPanel(Widget):
	def __init__(self, radius, color):
		self.radius = radius
		self.color = color
		return
	def draw(self, screenRect):
		glEnable(GL_BLEND)
		glColor4ub(*self.color)
		if (((screenRect[2]-screenRect[0]) < 2*self.radius)
			or ((screenRect[3]-screenRect[1]) < 2*self.radius)):
			# drawing area to small for round edges
			glRecti(*screenRect)
		else:
			glBegin(GL_POLYGON)
			# upper left
			glVertex2i(screenRect[0], screenRect[3] - self.radius)
			glVertex2i(screenRect[0] + self.radius, screenRect[3])
			# upper right
			glVertex2i(screenRect[2] - self.radius, screenRect[3])
			glVertex2i(screenRect[2], screenRect[3] - self.radius)
			# lower right
			glVertex2i(screenRect[2], screenRect[1] + self.radius)
			glVertex2i(screenRect[2] - self.radius, screenRect[1])
			# lower left
			glVertex2i(screenRect[0] + self.radius, screenRect[1])
			glVertex2i(screenRect[0], screenRect[1] + self.radius)
			# close polygon
			glVertex2i(screenRect[0], screenRect[3] - self.radius)
			glEnd()
		glDisable(GL_BLEND)
		return

class Selectable(Model):
	"""Keeps track of selection status.
	"""
	def __init__(self, selected):
		Model.__init__(self)
		self.selected = selected
		return
	def isSelected(self):
		return self.selected
	def setSelected(self, selected):
		self.selected = selected
		self._notify()
		return
	def select(self):
		self.selected = 1
		self._notify()
		return
	def deselect(self):
		self.selected = 0
		self._notify()
		return
	
class SelectableLabel(Widget, View):
	def __init__(self, selectable, string):
		"""Constructor.
		
		   @param selectable Model.
		   @param string String to display.
		"""
		#Widget.__init__(self)
		View.__init__(self)
		self.string = string
		self.selectable = selectable
		# mouse focus indicator
		self.mouseFocusX = 0
		self.mouseFocusY = 0
		# last drawing location
		self.guiRect = [0, 0, 0, 0]
		self.positionRect = [0, 0, 0, 0]
		return
	def draw(self, screenRectangle):
		# get size of the GUI window to translate MOUSEX and MOUSEY events
		guiRectBuffer = Blender.BGL.Buffer(GL_FLOAT, 4)
		Blender.BGL.glGetFloatv(Blender.BGL.GL_SCISSOR_BOX, guiRectBuffer)
		# replace element wise to avoid IndexError in eventFilter
		self.guiRect[0] = int(guiRectBuffer.list[0])
		self.guiRect[1] = int(guiRectBuffer.list[1])
		self.guiRect[2] = int(guiRectBuffer.list[2])
		self.guiRect[3]	= int(guiRectBuffer.list[3])
		## differs by one:
		## self.guiRect = Blender.Window.GetScreenInfo(Blender.Window.Types['SCRIPT'])[0]['vertices']
		self.positionRect = screenRectangle
		# draw panel and label
		glEnable(GL_BLEND)
		color = Blender.Window.Theme.Get()[0].get('ui').menu_back
		glColor4ub(*color)
		glRecti(*screenRectangle)
		if (self.mouseFocusX and self.mouseFocusY):
			color = Blender.Window.Theme.Get()[0].get('ui').menu_hilite
		else: 
			color = Blender.Window.Theme.Get()[0].get('ui').menu_item
		glColor4ub(*color)
		glRecti(screenRectangle[0], screenRectangle[1]+2, screenRectangle[2], screenRectangle[3]-2)
		if self.selectable.isSelected():
			color = [59, 89, 59, 128]
			glColor4ub(*color)
			glRecti(screenRectangle[0], screenRectangle[1]+2, screenRectangle[2], screenRectangle[3]-2)
		color = Blender.Window.Theme.Get()[0].get('ui').menu_text
		glColor4ub(*color)
		glRasterPos2i(screenRectangle[0]+4, screenRectangle[1]+7)
		Blender.Draw.Text(self.string)
		glDisable(GL_BLEND)
		return
	def eventFilter(self, event, value):
		"""Left mouse click on the label toggles selection status.
		"""
		if (value != 0):
			# check focus
			if (event == Blender.Draw.MOUSEX):
				if ((value >= (self.guiRect[0] + self.positionRect[0]))
					and (value <= (self.guiRect[0]) + self.positionRect[2])):
					# redraw if widget got focus
					if (not self.mouseFocusX) and self.mouseFocusY:
						Blender.Draw.Redraw(1)
					self.mouseFocusX = 1
				else:
					# redraw if widget lost focus
					if self.mouseFocusX and self.mouseFocusY:
						Blender.Draw.Redraw(1)
					self.mouseFocusX = 0
			elif (event == Blender.Draw.MOUSEY):
				if ((value >= (self.guiRect[1] + self.positionRect[1]))
					and (value <= (self.guiRect[1] + self.positionRect[3]))):
					# redraw if marker got focus
					if (not self.mouseFocusY) and self.mouseFocusX:
						Blender.Draw.Redraw(1)
					self.mouseFocusY = 1
				else:
					# redraw if marker lost focus
					if self.mouseFocusX and self.mouseFocusY:
						Blender.Draw.Redraw(1)
					self.mouseFocusY = 0
			# check selection
			elif ((event == Blender.Draw.LEFTMOUSE)
				and self.mouseFocusX and self.mouseFocusY):
				self.selectable.setSelected(1 - self.selectable.isSelected())
		return
	def getMinimumSize(self):
		return [Blender.Draw.GetStringWidth(self.string), 11+12]	
	def update(self):
		Blender.Draw.Redraw(1)
		return

########################################################################################

class Logger:
	"""Logs messages and status.
	
	   Logs messages as a list of strings and keeps track of the status.
	   Possible status values are info, warning and error.
	   
	   @cvar INFO info status
	   @cvar WARNING warning status
	   @cvar ERROR error status
	"""
	INFO, WARNING, ERROR = range(3)
	def __init__(self):
		"""Constructor.
		"""
		self.messageList = []
		self.status = Logger.INFO
		return
	def logInfo(self, message):
		"""Logs an info message.
		
		   @param message message string
		"""
		self.messageList.append(message)
		return		
	def logWarning(self, message):
		"""Logs a warning message.
		
		   The status is set to <code>Logger.WARNING</code> if it is not already <code>Logger.ERROR</code>.
		   
		   @param message message string
		"""
		self.messageList.append("Warning: "+message)
		if not self.status == Logger.ERROR:
			self.status = Logger.WARNING
		return
	def logError(self, message):
		"""Logs an error message.
		
		   The status is set to <code>Logger.ERROR</code>.
		   
		   @param message message string
		"""
		self.messageList.append("Error: "+message)
		self.status = Logger.ERROR
		return
	def getStatus(self):
		"""Gets the current status.
		
		   The status can be
		   <ul>
		   <li><code>Logger.INFO</code>
		   <li><code>Logger.WARNING</code>
		   <li><code>Logger.ERROR</code>
		   </ul>
		   
		   @return status
		"""
		return self.status
	def getMessageList(self):
		"""Returns the list of log messages.
		
		   @return list of strings
		"""
		return self.messageList

# global logger instance
exportLogger = Logger()

class ExportSettings:
	"""Global export settings.
	"""
	def __init__(self):
		self.path = os.path.dirname(Blender.Get('filename'))
		self.doProperties = 0
		self.rotX = BoundedValueModel(-1000, 1000, 0)
		self.rotY = BoundedValueModel(-1000, 1000, 0)
		self.rotZ = BoundedValueModel(-1000, 1000, 0)
		self.load()
		return
	def load(self):
		"""Load settings from registry, if available.
		"""
		settingsDict = Registry.GetKey('ogredotscene', True)
		if settingsDict:
			if settingsDict.has_key('path'):
				self.path = settingsDict['path']
			if settingsDict.has_key('doProperties'):
				self.doProperties = settingsDict['doProperties']
			if settingsDict.has_key('rotX'):
				self.rotX.setValue(settingsDict['rotX'])
			if settingsDict.has_key('rotY'):
				self.rotY.setValue(settingsDict['rotY'])
			if settingsDict.has_key('rotZ'):
				self.rotZ.setValue(settingsDict['rotZ'])
		return
	def save(self):
		"""Save settings to registry.
		"""
		settingsDict = {}
		settingsDict['path'] = self.path
		settingsDict['doProperties'] = self.doProperties
		settingsDict['rotX'] = self.rotX.getValue()
		settingsDict['rotY'] = self.rotY.getValue()
		settingsDict['rotZ'] = self.rotZ.getValue()
		Registry.SetKey('ogredotscene', settingsDict, True) 
		return

# Blender.Window.FileSelector does not accept type 'instancemethod' callbacks
# set this to the instancemethod callback
fileSelectorCallbackMethod = None
def fileSelectorCallback(fileName):
	"""FileSelector needs callback of PyFunction_Type
	"""
	fileSelectorCallbackMethod(fileName)
	return

class ExportSettingsView(Box):
	def __init__(self, exportSettings):
		# reference to model
		self.exportSettings = exportSettings
		self.propertiesCheckBox = CheckBox(ExportSettingsView.PropertiesChangeAction(self), \
									self.exportSettings.doProperties, \
									'Logic Properties', \
									[Blender.Draw.GetStringWidth('Logic Properties')+10,20], \
									'Export object logic propterties as userData.')
		self.stringButton = StringButton(self.exportSettings.path, \
							ExportSettingsView.PathChangedAction(self), \
							'Path: ', [200, 20], 'Export directory')
		# expand horizontally
		self.stringButton.preferredSize = [Widget.INFINITY, 13]
		self.button = Button(ExportSettingsView.SelectAction(self), \
							'Select', [100, 20], 'Select the export directory')
		hLayout = HorizontalLayout()
		hLayout.addWidget(self.stringButton,'path')
		hLayout.addWidget(self.button,'select')
		vLayout = VerticalLayout()
		vLayout.addWidget(self.propertiesCheckBox, 'doProperties')
		hRotLayout = HorizontalLayout()
		hRotLayout.addWidget(NumberView('RotX', self.exportSettings.rotX, [100, 20], [Widget.INFINITY, 20], 
										tooltip='Additional rotation around x-axis.'), 'rotX')
		hRotLayout.addWidget(NumberView('RotY', self.exportSettings.rotY, [100, 20], [Widget.INFINITY, 20], 
		                                tooltip='Additional rotation around y-axis.'), 'rotY')
		hRotLayout.addWidget(NumberView('RotZ', self.exportSettings.rotZ, [100, 20], [Widget.INFINITY, 20], 
		                                tooltip='Additional rotation around z-axis.'), 'rotZ')
		vLayout.addWidget(hRotLayout, 'hRotLayout')
		vLayout.addWidget(hLayout, 'hLayout')
		Box.__init__(self, vLayout, 'Global Settings')
		return
	def _pathSelectCallback(self, fileName):
		self.exportSettings.path = os.path.dirname(fileName)
		self.stringButton.string = Blender.Draw.Create(os.path.dirname(fileName))
		Blender.Draw.Redraw(1)
		return
	def _propertiesChange(self):
		self.exportSettings.doProperties = self.propertiesCheckBox.isChecked()
		return
	class PropertiesChangeAction(Action):
		def __init__(self, exportSettingsView):
			self.view = exportSettingsView
			return
		def execute(self):
			self.view._propertiesChange()
			return
	class PathChangedAction(Action):
		def __init__(self, exportSettingsView):
			self.view = exportSettingsView
			return
		def execute(self):
			self.view.exportSettings.path = self.view.stringButton.getString()
			return
	class SelectAction(Action):
		def __init__(self, exportSettingsView):
			self.view = exportSettingsView
			return
		def execute(self):
			global fileSelectorCallbackMethod 
			fileSelectorCallbackMethod = self.view._pathSelectCallback
			Blender.Window.FileSelector(fileSelectorCallback, "Export Directory", self.view.exportSettings.path)
			#Blender.Window.FileSelector(self.view._pathSelectCallback, "Export Directory", self.view.exportSettings.path)
			return

class ObjectExporter(Model):
	"""Interface. Exports a Blender object to Ogre.
	"""
	def __init__(self, object, enabled=1, children=None):
		"""Constructor.
		   
		   @param object Blender object to export.
		   @param enabled Only enabled objects are exported.
		   @param children List of child ObjectExporters
		"""
		Model.__init__(self)
		# blender object
		self.object = object
		# export status
		self.enabled = enabled
		# list of child ObjectExporters
		self.children = children or []
		return
	def isEnabled(self):
		return self.enabled
	def setEnabled(self, enabled):
		self.enabled = enabled
		self._notify()
		return
	def getBlenderObject(self):
		return self.object
	def addChild(self, objectExporter):
		self.children.append(objectExporter)
		self._notify()
		return
	def removeChild(self, objectExporter):
		if objectExporter in self.children:
			self.children.remove(objectExporter)
			self._notify()
		return
	def getChildren(self):
		return self.children
	def export(self, sceneExporter, fileObject, indent=0, parentTransform=Blender.Mathutils.Matrix([1,0,0,0],[0,1,0,0],[0,0,1,0],[0,0,0,1])):
		"""Append node element for this object and all enabled childrens into a given file.
		
		   @param sceneExporter Blender sceneExporter
		   @param fileObject already opened XML file.
		   @param indent indentation in the XML file.
		   @param parentTransform transformations are exported in this coordinate system.
		"""
		if self.isEnabled():
			fileObject.write(self._indent(indent) + "<node name=\"%s\">\n" % self.object.getName())
			# create transformation matrix relative to parent
			objectMatrix = self.object.getMatrix("worldspace")
			inverseParentMatrix = Blender.Mathutils.Matrix(*parentTransform)
			inverseParentMatrix.invert()
			matrix = objectMatrix*inverseParentMatrix
			# position
			fileObject.write(self._indent(indent+1) + \
							"<position x=\"%.6f\" y=\"%.6f\" z=\"%.6f\"/>\n" % tuple(matrix.translationPart()))
			# rotation
			rot = matrix.toQuat()
			fileObject.write(self._indent(indent+1) + \
							"<rotation qx=\"%.6f\" qy=\"%.6f\" qz=\"%.6f\" qw=\"%.6f\"/>\n" % (rot.x, rot.y, rot.z, rot.w))
			# scale
			rot.inverse()
			inverseRotationMatrix = rot.toMatrix()
			inverseRotationMatrix.resize4x4()
			scale = matrix*inverseRotationMatrix
			fileObject.write(self._indent(indent+1) + \
							"<scale x=\"%.6f\" y=\"%.6f\" z=\"%.6f\"/>\n" % (scale[0][0], scale[1][1], scale[2][2]))
			# specifics
			self.writeSpecificNodeElements(sceneExporter, fileObject, indent+1)
			# logic properties
			self.writeUserData(sceneExporter, fileObject, indent+1)
			for child in self.children:
				child.export(sceneExporter, fileObject, indent+1, self.object.getMatrix("worldspace"))
			fileObject.write(self._indent(indent)+"</node>\n")
		else:
			for child in self.children:
				child.export(sceneExporter, fileObject, indent, parentTransform)
		return
	def writeSpecificNodeElements(self, sceneExporter, fileObject, indent=0):
		"""Virtual method to write node elements that depend on the object type.
		
		   This method is called before the node element is closed. Overwrite
		   this method in derived classes for specific object types.
		"""
		return
	def propertyFilter(self, property):
		"""Filter properties that should not be exported as user data.
		
		   @param property the property to test.
		   @return <code>1</code> if filter applies, otherwise <code>0</code>.
		"""
		return 0
	def writeUserData(self, sceneExporter, fileObject, indent=0):
		"""Writes object propreties as userDataReference.
		"""
		if ((sceneExporter.settings.doProperties) and len(self.object.getAllProperties())):
			fileObject.write(self._indent(indent) + "<userData>\n")
			for property in self.object.getAllProperties():
				if (property and not(self.propertyFilter(property))):
					fileObject.write(self._indent(indent + 1) + 
						"<property type=\"%s\" " % property.getType() +
						"name=\"%s\" " % property.getName() +
						"data=\"%s\"/>\n" % str(property.getData()))
			fileObject.write(self._indent(indent) + "</userData>\n")
		return
	# protected
	def _indent(self, indent):
		return "  "*indent

class MeshExporter(ObjectExporter):
	def __init__(self, object, enabled=1, children=None):
		ObjectExporter.__init__(self, object, enabled, children)
		return
	def writeSpecificNodeElements(self, sceneExporter, fileObject, indent=0):
		fileObject.write(self._indent(indent) + "<entity name=\"%s\" meshFile=\"%s\"" \
			% (self.object.getName(), self.object.data.name + ".mesh"))
		# static property
		pList = [p for p in self.object.getAllProperties() if (p.getType() == "BOOL" and p.getName() == "static")]
		if len(pList):
			fileObject.write(" static=\"")
			if pList[0].getData():
				fileObject.write("true\"")
			else:
				fileObject.write("false\"")
		fileObject.write("/>\n")
		return
	def propertyFilter(self, property):
		filter = 0
		if ((property.getType() == "BOOL") and (property.getName() == "static")):
			filter = 1
		return filter
	
class LampExporter(ObjectExporter):
	def __init__(self, object, enabled=1, children=None):
		ObjectExporter.__init__(self, object, enabled, children)
		return
	def writeSpecificNodeElements(self, sceneExporter, fileObject, indent=0):
		lamp = self.object.data
		fileObject.write(self._indent(indent) + "<light name=\"%s\" " % lamp.getName()) 
		# type specific parameters
		if (lamp.getType() & lamp.Types['Sun']):
			self._writeSunNodeElements(sceneExporter, fileObject, indent)
		elif (lamp.getType() & lamp.Types['Spot']):
			self._writeSpotNodeElements(sceneExporter, fileObject, indent)
		else:
			self._writeLampNodeElements(sceneExporter, fileObject, indent)
		# close light
		fileObject.write(self._indent(indent) + "</light>\n")
		return
	def _attenuation(self, lamp):
		# attenuation calculation:
		#
		# OpenGL attenuation factor f = 1/(c+l*r+q*r^2),
		# where r is the distance between the light source and the vertex to be lit.
		#
		# Ignore Sphere option as it can't be mapped to Ogre settings.
		# Note: Multiply lamp colors with Blender's energy factor e.
		#       => here: e=1.0
		c = 1.0
		l = 0.0
		q = 0.0
		if (lamp.getMode() & lamp.Modes['Quad']):
				# Blender's intensity:
				# f = e*d(d+Q1*r)*d^2/(d^2+Q2*r^2)
				#   = e*d^3/(d^3 + Q1*d^2*r + d*Q2*r^2 + Q1*Q2*r^3)
				# => approximately
				# c = d^3/(e*d^3)  = 1/e
				# l = Q1/(e*d)
				# q = Q2/(e*d^2)
				l = lamp.getQuad1()/lamp.getDist()
				q = lamp.getQuad2()/(lamp.getDist()*lamp.getDist())
		else:
			# no quad, no sphere
			# Blender's intensity:
			# f = e*d/(d+r),
			# where e is energy and d is distance.
			# =>
			# c = d/(e*d) = 1/e, l = 1/(e*d), q = 0
			l = 1.0/lamp.getDist()
		return (c, l, q)
	def _colourDiffuse(self, lamp):
		if (lamp.getMode() & lamp.Modes['NoDiffuse']):
			color = (0, 0, 0)
		else:
			e = lamp.getEnergy()
			rgb = lamp.col
			color = (e*rgb[0], e*rgb[1], e*rgb[2])
		return color
	def _colourSpecular(self, lamp):
		if (lamp.getMode() & lamp.Modes['NoSpecular']):
			color = (0, 0, 0)
		else:
			e = lamp.getEnergy()
			rgb = lamp.col
			color = (e*rgb[0], e*rgb[1], e*rgb[2])
		return color
	def _writeLampNodeElements(self, sceneExporter, fileObject, indent=0):
		# LT_POINT
		fileObject.write("type=\"point\">\n")
		lamp = self.object.data
		fileObject.write(self._indent(indent+1) + \
			"<colourDiffuse r=\"%f\" g=\"%f\" b=\"%f\"/>\n" % self._colourDiffuse(lamp))
		fileObject.write(self._indent(indent+1) + \
			"<colourSpecular r=\"%f\" g=\"%f\" b=\"%f\"/>\n" % self._colourSpecular(lamp))
		fileObject.write(self._indent(indent+1) + \
			"<lightAttenuation range=\"5000.0\" constant=\"%f\" linear=\"%f\" quadratic=\"%f\"/>\n" \
			% self._attenuation(lamp))
		return
	def _writeSpotNodeElements(self, sceneExporter, fileObject, indent=0):
		# LT_SPOT
		fileObject.write("type=\"spot\" ")
		lamp = self.object.data
		# castShadows="true" only if Modes['Shadows'] is set
		if (lamp.getMode() & lamp.Modes['Shadows']):
			fileObject.write("castShadows=\"true\">\n")
			range = lamp.getClipEnd()
		else:
			fileObject.write("castShadows=\"false\">\n")
			range = 5000.0
		# points locally in -z direction
		fileObject.write(self._indent(indent+1) + "<normal x=\"0.0\" y=\"0.0\" z=\"-1.0\"/>\n")
		fileObject.write(self._indent(indent+1) + \
			"<colourDiffuse r=\"%f\" g=\"%f\" b=\"%f\"/>\n" % self._colourDiffuse(lamp))
		fileObject.write(self._indent(indent+1) + \
			"<colourSpecular r=\"%f\" g=\"%f\" b=\"%f\"/>\n" % self._colourSpecular(lamp))
		# OpenGL parameters:
		# GL_SPOT_CUTOFF = spotSize/2.0
		# GL_SPOT_EXPONENT = 128.0*spotBlend
		# but Ogre::GLRenderSystem Class only uses outer parameter
		#angle = lamp.getSpotSize()*math.pi/180.0
		# angle in degree
		angle = lamp.getSpotSize()
		fileObject.write(self._indent(indent+1) + \
			"<lightRange inner=\"%f\" outer=\"%f\" falloff=\"1.0\"/>\n" % (angle, angle))
		fileObject.write(self._indent(indent+1) +"<lightAttenuation range=\"%f\" " % range)
		fileObject.write("constant=\"%f\" linear=\"%f\" quadratic=\"%f\"/>\n" \
			% self._attenuation(lamp))
		return
	def _writeSunNodeElements(self, sceneExporter, fileObject, indent=0):
		# LT_DIRECTIONAL
		fileObject.write("type=\"directional\">\n")
		lamp = self.object.data
		# points locally in -z direction
		fileObject.write(self._indent(indent+1) + "<normal x=\"0.0\" y=\"0.0\" z=\"-1.0\"/>\n")
		fileObject.write(self._indent(indent+1) + \
			"<colourDiffuse r=\"%f\" g=\"%f\" b=\"%f\"/>\n" % self._colourDiffuse(lamp))
		fileObject.write(self._indent(indent+1) + \
			"<colourSpecular r=\"%f\" g=\"%f\" b=\"%f\"/>\n" % self._colourSpecular(lamp))
		return

class CameraExporter(ObjectExporter):
	def __init__(self, object, enabled=1, children=None):
		ObjectExporter.__init__(self, object, enabled, children)
		return
	def writeSpecificNodeElements(self, sceneExporter, fileObject, indent=0):
		# camera element
		camera = self.object.data
		fileObject.write(self._indent(indent) + "<camera name=\"%s\" " % camera.getName())
		# fov calculation
		render = sceneExporter.scene.getRenderingContext()
		aspX = render.aspectRatioX()
		aspY = render.aspectRatioY()
		sizeX = render.imageSizeX()
		sizeY = render.imageSizeY()
		if (camera.getType()==0):
			# perspective projection
			# FOV calculation
			#    .
			#   /|
			#  / |v = 16
			# /a |
			#.---'
			#  u = lens
			#
			# => FOV = 2*a = 2*arctan(v/u)
			#
			# Blender's v is in direction of the larger image dimension.
			# Ogre's FOV is definied in y direction. Therefore if 
			# SizeX*AspX > SizeY*AspY the corresponding v in y direction is
			# v_y = v*(SizeY*AspY)/(SizeX*AspX).
			fovY = 0.0
			if (sizeX*aspX > sizeY*aspY):
				fovY = 2*math.atan(sizeY*aspY*16.0/(camera.getLens()*sizeX*aspX))
			else:
				fovY = 2*math.atan(16.0/camera.getLens())
			# fov in degree
			fovY = fovY*180.0/math.pi
			# write fov
			fileObject.write("fov=\"%f\"" % fovY)
			fileObject.write(" projectionType=\"perspective\">\n")
		else:
			# orthographic projection
			# FOV calculation 
			# 
			# In Blender, the displayed area is scale wide in the larger image direction,
			# where scale is a ortho camera attribute.
			#
			# Ogre calculates the with and height of the displayed area from the
			# FOVy and the near clipping plane.
			#    .
			#   /|
			#  / |y
			# /a |
			#.---'
			#  n
			# Here n = near clipping plane and a = FOVy/2.
			#
			# Hence y = scale/2 and FOVy = 2*atan(scale/(2*n)).
			fovY = 0.0
			if (sizeX*aspX > sizeY*aspY):
				# tan(FOVx/2.0) = x/n = y*a/n,
				# where a is the aspect ratio.
				# It follows that s/(n*a) = y/n = tan(FOVy/2.0).
				fovY = 2.0*math.atan(sizeY*aspY*camera.getScale()/(2.0*camera.getClipStart()*sizeX*aspX))
			else:
				fovY = 2.0*math.atan(camera.getScale()/(2.0*camera.getClipStart()))
			# fov in degree
			fovY = fovY*180.0/math.pi
			# write fov
			fileObject.write("fov=\"%f\"" % fovY)
			fileObject.write(" projectionType=\"orthographic\">\n")
		# normal
		# Blender's camera normal always points in -z direction locally.
		fileObject.write(self._indent(indent + 1) + \
			"<normal x=\"0.0\" y=\"0.0\" z=\"-1.0\"/>\n")
		# default clipping
		fileObject.write(self._indent(indent + 1) + \
			"<clipping near=\"%f\" far=\"%f\"/>\n" % (camera.getClipStart(), camera.getClipEnd()))
		# close camera
		fileObject.write(self._indent(indent) + "</camera>\n")
		return
		
class ObjectExporterFactory:
	"""Creates ObjectExporter for different object types.
	"""
	def create(object, enabled):
		"""Creates ObjecteExporter for the given Blender object.
		
		   @param object Blender object to export
		   @return ObjectExporter on success, <code>None</code> on failure.
		"""
		exporter = None
		if (object.getType() == "Mesh"):
			exporter = MeshExporter(object, enabled)
		elif (object.getType() == "Lamp"):
			exporter = LampExporter(object, enabled)
		elif (object.getType() == "Camera"):
			exporter = CameraExporter(object, enabled)
		elif (object.getType() == "Empty"):
			exporter = ObjectExporter(object, enabled)
		elif (object.getType() == "Armature"):
			exporter = None
		return exporter
	create = staticmethod(create)

class ObjectExporterTree(Model):
	"""Creates tree of ObjectExporter for a Blender scene.
	"""
	ENABLEALL, ENABLENONE, ENABLESELECTION = range(3)
	def __init__(self, scene, enable=ENABLESELECTION):
		"""Constructor.
		
		   @param scene Blender.Scene.
		   @param enable Initial export status.
		"""
		Model.__init__(self)
		self.rootExporterList = []
		# create exporter tree
		# key: object name, value: ObjectExporter
		exporterDict = {}
		# 1.) create ObjectExporters for all exportable scene objects
		selectedList = []
		if Blender.Object.GetSelected():
			selectedList = [object.getName() for object in Blender.Object.GetSelected()]
		for object in scene.getChildren():
			enabled = 0
			if (enable == ObjectExporterTree.ENABLESELECTION):
				if object.getName() in selectedList:
					enabled = 1
			elif (enable == ObjectExporterTree.ENABLEALL):
				enabled = 1
			exporter = ObjectExporterFactory.create(object, enabled)
			if exporter:
				# register object
				exporterDict[object.getName()] = exporter
		#  2.) create tree structure
		for exporter in exporterDict.values():
			# search for an exportable parent
			object = exporter.getBlenderObject().getParent()
			searchCompleted = 0
			while not searchCompleted:
				# object a Blender.Object (possible parent) or none
				if object:
					parentName = object.getName()
					if exporterDict.has_key(parentName):
						# exportable parent found
						exporterDict[parentName].addChild(exporter)
						searchCompleted = 1
					else:
						# try next possible parent
						object = object.getParent()
				else:
					# exporter object has no exportable parent
					self.rootExporterList.append(exporter)
					searchCompleted = 1
		return
	def export(self, sceneExporter, fileObject, indent=0, parentTransform=Blender.Mathutils.Matrix([1,0,0,0],[0,1,0,0],[0,0,1,0],[0,0,0,1])):
		for exporter in self.rootExporterList:
			exporter.export(sceneExporter, fileObject, indent, parentTransform)
		return
	def enableAll(self):
		for exporter in self:
			exporter.setEnabled(1)
		self._notify()
		return
	def enableNone(self):
		for exporter in self:
			exporter.setEnabled(0)
		self._notify()
		return
	def invertEnabled(self):
		for exporter in self:
			exporter.setEnabled(1 - exporter.isEnabled())
		self._notify()
		return
	def enableLayer(self, layer):
		#TODO
		self._notify()
		return
	def disableLayer(self, layer):
		self._notify()
		#TODO
		return
	def __iter__(self):
		return ObjectExporterTree.ObjectExporterTreeIterator(self)
	class ObjectExporterTreeIterator:
		def __init__(self, objectExporterTree):
			self.stack = objectExporterTree.rootExporterList[:]
			return
		def next(self):
			exporter = None
			if len(self.stack):
				exporter = self.stack.pop()
				self.stack.extend(exporter.getChildren())
			else:
				raise StopIteration
			return exporter

class ObjectExporterTreeView(VerticalLayout, View):
	def __init__(self, objectExporterTree):
		# call super constructors
		VerticalLayout.__init__(self)
		View.__init__(self)
		# reference to model
		self.tree = objectExporterTree
		# fully expanded list of all NodeViews alphabetically sorted on each level
		self._createNodeList()
		# create WidgetList
		self.widgetList = WidgetList()
		i = 0
		for label in self.nodeList:
			self.widgetList.addWidget(label, 'label'+str(i))
			i +=1
		self.addWidget(self.widgetList, 'widgetList')
		# Selection Buttons: All, None, Invert
		buttonsPanel = HorizontalRatioLayout()
		buttonsPanel.addWidget(1.0/3, Button( \
			ObjectExporterTreeView.SelectAllAction(self.tree), \
			'All',[60,20],'Select all'),'all')
		buttonsPanel.addWidget(1.0/3, Button( \
			ObjectExporterTreeView.SelectNoneAction(self.tree), \
			'None',[60,20],'Select none'),'none')
		buttonsPanel.addWidget(1.0/3, Button( \
			ObjectExporterTreeView.InvertSelectionAction(self.tree), \
			'Invert',[60,20],'Invert selection'),'invert')
		self.addWidget(buttonsPanel, 'buttonsPanel')
		self.tree.addView(self)
		return
	def _createNodeList(self):
		self.nodeList = []
		rootDict = dict([(e.getBlenderObject().getName(), e) for e in self.tree.rootExporterList])
		rootDictKeys = rootDict.keys()
		rootDictKeys.sort()
		rootDictKeys.reverse()
		stack = []
		for name in rootDictKeys:
			stack.append((0, rootDict[name]))
		# stack created	
		childrenDict = {}
		while len(stack):
			depth, exporter = stack.pop()
			self.nodeList.append(ObjectExporterTreeView.NodeView(exporter, depth))
			childrenList = exporter.getChildren()
			if len(childrenList):
				# insert children
				childrenDict = dict([(e.getBlenderObject().getName(), e) for e in childrenList])
				childrenDictKeys = childrenDict.keys()
				childrenDictKeys.sort()
				childrenDictKeys.reverse()
				for name in childrenDictKeys:
					stack.append((depth + 1, childrenDict[name]))
		return
	def update(self):
		Blender.Draw.Redraw(1)
		return
	class NodeView(SelectableLabel, View):
		"""Keeps track of collapse status of the node.
		"""
		def __init__(self, objectExporter, depth):
			# call super constructors
			View.__init__(self)
			self.depth = depth
			self.isCollapsed = 0
			label = depth*"    " + objectExporter.getBlenderObject().getName()
			SelectableLabel.__init__(self,
				ObjectExporterTreeView.NodeView.ExporterWrapper(objectExporter), label)
			objectExporter.addView(self)
			return
		def collapse(self, state):
			"""@param state 0 to collapse, 1 to expand
			"""
			self.isCollapsed = state
			return
		def isCollapsed(self):
			"""@return <code>true</code> if collapsed, <code>false</code> if expanded.
			"""
			return self.isCollapsed
		class ExporterWrapper(Selectable, View):
			def __init__(self, objectExporter):
				self.exporter = objectExporter
				Selectable.__init__(self, self.exporter.isEnabled())
				self.exporter.addView(self)
				return
			def select(self):
				self.exporter.setEnabled(1)
				self._notify()
				return
			def deselect(self):
				self.exporter.setEnabled(0)
				self._notify()
				return
			def isSelected(self):
				return self.exporter.isEnabled()
			def setSelected(self, selected):
				self.exporter.setEnabled(selected)
				self._notify()
				return
			def update(self):
				# pass through
				self._notify()
				return
	class SelectAllAction(Action):
		def __init__(self, objectExporterTree):
			self.tree = objectExporterTree
			return
		def execute(self):
			self.tree.enableAll()
			return
	class SelectNoneAction(Action):
		def __init__(self, objectExporterTree):
			self.tree = objectExporterTree
			return
		def execute(self):
			self.tree.enableNone()
			return
	class InvertSelectionAction(Action):
		def __init__(self, objectExporterTree):
			self.tree = objectExporterTree
			return
		def execute(self):
			self.tree.invertEnabled()
			return

class ProgressListener:
	def __init__(self):
		self.percent = 0.0
		self.string = ''
		self.viewList = []
		return
	def addView(self, view):
		self.viewList.append(view)
		return
	def removeView(self, view):
		if view in self.viewList:
			self.viewList.remove(view)
		return
	def progress(self, percent, string):
		self.percent = percent
		self.string = string
		# update views
		self._notify()
		return
	# private
	def _notify(self):
		"""Notify views.
		"""
		for view in self.viewList:
			view.update()
		return

class ProgressListenerView(Widget):
	def __init__(self, progressListener):
		self.listener = progressListener
		self.listener.addView(self)
		return
	def update(self):
		Blender.Draw.Redraw(1)
		return

class ProgressListenerProgressBar(ProgressBar, ProgressListenerView):
	def __init__(self, progressListener):
		ProgressListenerView.__init__(self, progressListener)
		ProgressBar.__init__(self)
		return
	def update(self):
		self.progress(self.listener.percent, self.listener.string)
		ProgressListenerView.update(self)
		return

class ProgressListenerDoneButton(Button, ProgressListenerView):
	def __init__(self, progressListener, action, name, size, tooltip=None):
		ProgressListenerView.__init__(self, progressListener)
		Button(self, action, name, size, tooltip)
		if (self.listener.percent >= 100):
			self.done = 1
		else:
			self.done = 0
		return
	def update(self):
		if (self.listener.percent >= 100):
			self.done = 1
			ProgressListenerView.update(self)
		return
	def draw(self, rect):
		if self.done:
			Button.draw(self, rect)
		return

class SceneExporter:
	"""Exports a scene to a dotScene XML file.
	"""
	def __init__(self, scene, settings, enable=ObjectExporterTree.ENABLESELECTION):
		"""Constructor.
		
		   @param scene Blender.Scene.
		   @param settings ExportSettings.
		   @param enable Initial export status.
		"""
		self.scene = scene
		self.settings = settings
		self.tree = ObjectExporterTree(scene, enable)
		return
	def export(self):
		"""Export the scene to dotScene XML file.
		"""
		# open scene xml file
		fileName = self.scene.getName() + ".xml"
		fileObject = open(os.path.join(self.settings.path, fileName), "w")
		self._write(fileObject)
		fileObject.close()
		return
	def addProgressListener(self, listener):
		self.progressListenerList.append(listener)
		return
	def removeProgressListener(self, listener):
		if listener in self.progressListenerList:
			self.progressListenerList.remove(listener)
		return
	# private
	def _progress(self, percent, string):
		"""Report progress to the attached ProgressListeners.
		"""
		for listener in self.progressListenerList:
			listener.progress(percent, string)
		return
	def _write(self, fileObject):
		"""Write XML scene into the fileObject.
		"""
		# header
		fileObject.write("<scene formatVersion=\"0.2.0\">\n")
		# nodes
		self._writeNodes(fileObject, 1)
		# externals (material file)
		self._writeExternals(fileObject, 1)
		# environment
		self._writeEnvironment(fileObject, 1)
		# footer
		fileObject.write("</scene>\n")
		return
	def _writeNodes(self, fileObject, indent=0):
		fileObject.write(self._indent(indent) + "<nodes>\n")
		# default: set matrix to 90 degree rotation around x-axis
		# rotation order: x, y, z
		# WARNING: Blender uses left multiplication!
		rotationMatrix = Mathutils.RotationMatrix(self.settings.rotX.getValue(), 4, 'x')
		rotationMatrix *= Mathutils.RotationMatrix(self.settings.rotY.getValue(), 4, 'y')
		rotationMatrix *= Mathutils.RotationMatrix(self.settings.rotZ.getValue(), 4, 'z')
		self.tree.export(self, fileObject, indent+1, rotationMatrix)
		fileObject.write(self._indent(indent) + "</nodes>\n")
		return
	def _writeExternals(self, fileObject, indent=0):
		fileObject.write(self._indent(indent) + "<externals>\n")
		fileObject.write(self._indent(indent + 1) + "<item type=\"material\">\n")
		fileObject.write(self._indent(indent + 2) + "<file name=\"%s\"/>\n" % (self.scene.getName()+".material"))
		fileObject.write(self._indent(indent + 1) + "</item>\n")
		fileObject.write(self._indent(indent) + "</externals>\n")
		return
	def _writeEnvironment(self, fileObject, indent=0):
		self.scene.makeCurrent() 
		world = Blender.World.GetCurrent()
		if world:
			fileObject.write(self._indent(indent) + "<environment>\n")
			# can only get the active world of the current scene
			if (world.mode & 1):
				# Mist enabled
				# Color = f*pixelColor + (1-f)*fogColor
				#
				# Blender's f factor:
				# f = (1 - g((z - mistStart)/mistDepth))*(1-mistIntensity)
				# where
				# g(x)=x^2 for Qua,
				# g(x)=x for Lin,
				# g(x)=sqrt(x) for Sqr
				# and z is the distance of the pixel to the camera.
				#
				# OpenGL's f factor:
				# f = exp(-(density*z)) for GL_EXP,
				# f = exp(-(density*z)^2) for GL_EXP2,
				# f = (end - z)/(end - start) = (1 - (z - start)/(end - start)) for GL_LINEAR
				#
				# Assume depth and intensity is small and linear approximate
				# all Blender mist types, i.e. ignore intensity and type.
				mist = world.getMist() 
				fileObject.write(self._indent(indent + 1) + \
					"<fog linearStart=\"%f\" linearEnd=\"%f\" mode=\"linear\">\n" \
					% (mist[1], mist[1]+mist[2]))
				fileObject.write(self._indent(indent + 2) + \
					"<colourDiffuse r=\"%f\" g=\"%f\" b=\"%f\"/>\n" \
					% tuple(world.getHor()))
				fileObject.write(self._indent(indent + 1) + "</fog>\n")
			# ambient color
			fileObject.write(self._indent(indent + 1) + \
							"<colourAmbient r=\"%f\" g=\"%f\" b=\"%f\"/>\n" \
							% tuple(world.getAmb()))
			# horizon color
			fileObject.write(self._indent(indent + 1) + \
							"<colourBackground r=\"%f\" g=\"%f\" b=\"%f\"/>\n" \
							% tuple(world.getHor()))
			fileObject.write(self._indent(indent) + "</environment>\n")
		return
	def _indent(self, indent):
		return "  "*indent

class SceneExporterView(VerticalLayout):
	def __init__(self, sceneExporter):
		# call super constructor
		VerticalLayout.__init__(self)
		# reference to model
		self.sceneExporter = sceneExporter
		self.addWidget(ObjectExporterTreeView(self.sceneExporter.tree), 'treeView')
		self.addWidget(ExportSettingsView(self.sceneExporter.settings), 'settingsView')
		return

########################################################################################
#TestCases
class TestWidget(Widget):
	def __init__(self, color, preferredSize=[50, 50]):
		self.color = color
		self.preferredSize = preferredSize
		return
	def draw(self, rect):
		Blender.BGL.glColor3f(*self.color)
		Blender.BGL.glRectf(rect[0], rect[1], rect[2], rect[3])
		return
	def getMinimumSize(self):
		return [50, 50]
	def getPreferredSize(self):
		return self.preferredSize

class ExportAction(Action):
	def __init__(self, sceneExporter):
		self.sceneExporter = sceneExporter
		return
	def execute(self):
		tmp = Screen.current
		ExportAction.WaitScreen().activate()
		self.sceneExporter.export()
		ExportAction.DoneScreen(ExportAction.DoneAction(tmp)).activate()
		return
	class WaitScreen(Screen):
		def __init__(self):
			Screen.__init__(self)
			self.addWidget(Label("Exporting, please wait!"), 'label')
			return
	class DoneScreen(Screen):
		def __init__(self, doneAction):
			Screen.__init__(self)
			vLayout = VerticalLayout()
			vLayout.addWidget(Label("Scene Exported!"), 'label1')
			vLayout.addWidget(Label(" "), 'label2')
			vLayout.addWidget(Label("Note: Don't forget to export all mesh objects with the ogreexport.py script."), 'label3')
			vLayout.addWidget(Spacer([0,0], [Widget.INFINITY, Widget.INFINITY], [Widget.INFINITY, Widget.INFINITY]),'space')
			hLayout = HorizontalLayout()
			hLayout.addWidget(Button(doneAction, 'Ok', [100, 30]),'button')
			vLayout.addWidget(hLayout,'buttonPanel')
			self.addWidget(OgreFrame(vLayout, 'dotScene Exporter'),'ogreFrame')
			return
	class DoneAction(Action):
		def __init__(self, screen):
			self.screen = screen
			return
		def execute(self):
			self.screen.activate()
			return

class DotSceneExporterApplication:
	"""Controller for the application screen.
	"""
	def __init__(self):
		self.screen = Screen()
		self.exportSettings = ExportSettings()
		self.sceneExporter = SceneExporter(Blender.Scene.getCurrent(), self.exportSettings)
		self.sceneExporterView = SceneExporterView(self.sceneExporter)
		vLayout = VerticalLayout()
		vLayout.addWidget(self.sceneExporterView, 'sceneExporterView')
		buttonLayout = HorizontalLayout()
		buttonLayout.addWidget(Button(ExportAction(self.sceneExporter), 'Export', [100, 30], 'Export scene'), 'exportButton')
		buttonLayout.addWidget(Spacer([0, 0], [0, 0], [Widget.INFINITY, 0]), 'spacer')
		buttonLayout.addWidget(QuitButton(DotSceneExporterApplication.QuitAction(self.exportSettings), 'Quit', [100, 30], 'Quit'), 'quitButton')
		vLayout.addWidget(buttonLayout, 'buttonPanel')
		self.screen.addWidget(OgreFrame(vLayout, 'dotScene Exporter'),'ogreFrame')
		return
	def go(self):
		"""Start the Application.
		"""
		self.screen.activate()
		return
	class QuitAction(Action):
		def __init__(self, exportSettings):
			self.settings = exportSettings
			return
		def execute(self):
			self.settings.save()
			Blender.Draw.Exit()
			return

if (__name__ == "__main__"):
	application = DotSceneExporterApplication()
	application.go()
