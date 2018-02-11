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
import ogre.renderer.OGRE as og

# I have found this trick in nullsquares post here:
# http://www.ogre3d.org/forums/viewtopic.php?f=2&t=43440&p=299049&hilit=setRenderQueueGroup#p299049
class PivotRenderQueueListener(og.RenderQueueListener):
    def __init__(self):
        super (PivotRenderQueueListener, self).__init__()
        
    def renderQueueStarted(self, queueGroupId, invocation, skipThisInvocation):
        if queueGroupId == og.RENDER_QUEUE_OVERLAY - 1:
            og.Root.getSingleton().getRenderSystem().clearFrameBuffer(og.FBT_DEPTH)

    def renderQueueEnded(self, queueGroupId, invocation, skipThisInvocation):
        return
