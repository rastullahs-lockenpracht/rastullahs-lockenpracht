/************************************************************************
    filename:   RLTitlebar.cpp
    created:    9/5/2004
    author:     Paul D Turner
    
    purpose:    Implementation of Rastullah Look Titlebar widget
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://crayzedsgui.sourceforge.net)
    Copyright (C)2004 Paul D Turner (crayzed@users.sourceforge.net)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#include "RLTitlebar.h"
#include "CEGUIImageset.h"
#include "CEGUIFont.h"
#include "elements/CEGUIFrameWindow.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
    Constants
*************************************************************************/
// type name for this widget
const utf8  RLTitlebar::WidgetTypeName[]    = "RastullahLook/Titlebar";

const utf8  RLTitlebar::ImagesetName[]              = "RastullahLook";
const utf8  RLTitlebar::LeftEndSectionImageName[]   = "NewTitlebarLeft";
const utf8  RLTitlebar::MiddleSectionImageName[]    = "NewTitlebarMiddle";
const utf8  RLTitlebar::RightEndSectionImageName[]  = "NewTitlebarRight";
const utf8  RLTitlebar::SysAreaMiddleImageName[]    = "SysAreaMiddle";
const utf8  RLTitlebar::SysAreaRightImageName[]     = "SysAreaRight";
const utf8  RLTitlebar::NormalCursorImageName[]     = "MouseMoveCursor";
const utf8  RLTitlebar::NoDragCursorImageName[]     = "MouseTarget";

const colour RLTitlebar::CaptionColour      = 0xFFFFFF;


/*************************************************************************
    Constructor
*************************************************************************/
RLTitlebar::RLTitlebar(const String& type, const String& name) :
    Titlebar(type, name)
{
    // get images
    Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

    d_leftImage     = &iset->getImage(LeftEndSectionImageName);
    d_middleImage   = &iset->getImage(MiddleSectionImageName);
    d_rightImage    = &iset->getImage(RightEndSectionImageName);
    d_sysMidImage   = &iset->getImage(SysAreaMiddleImageName);
    d_sysRightImage = &iset->getImage(SysAreaRightImageName);

    // default text colour
    d_captionColour = CaptionColour;

    // set cursor
    setMouseCursor(&iset->getImage(NormalCursorImageName));

    setAlwaysOnTop(false);
}


/*************************************************************************
    Destructor
*************************************************************************/
RLTitlebar::~RLTitlebar(void)
{
}


/*************************************************************************
    return a Rect object describing the appropriately clipped Window
    area in screen space.
*************************************************************************/
Rect RLTitlebar::getPixelRect(void) const
{
    // clip to screen if we have no grand-parent
    if ((d_parent == NULL) || (d_parent->getParent() == NULL))
    {
        return System::getSingleton().getRenderer()->getRect().getIntersection(getUnclippedPixelRect());
    }
    // else clip to grand-parent
    else 
    {
        return d_parent->getParent()->getInnerRect().getIntersection(getUnclippedPixelRect());
    }

}


/*************************************************************************
    Perform the actual rendering for this widget.   
*************************************************************************/
void RLTitlebar::drawSelf(float z)
{
    Rect clipper(getPixelRect());

    // do nothing if the widget is totally clipped.
    if (clipper.getWidth() == 0)
    {
        return;
    }

    // get the destination screen rect for this window
    Rect absrect(getUnclippedPixelRect());

    // calculate colours to use.
    float alpha = getEffectiveAlpha();
    ColourRect colours(colour(1, 1, 1, alpha));

    // calculate widths for the title bar segments
    float leftWidth     = d_leftImage->getWidth();
    float rightWidth    = d_rightImage->getWidth();

    float sysRightWidth = d_sysRightImage->getWidth();
    float sysMidWidth;

    FrameWindow* parent = (FrameWindow*)getParent();

    sysMidWidth = ((parent != NULL) && parent->isCloseButtonEnabled()) ? d_sysMidImage->getWidth() : 0.0f;

    float midWidth      = absrect.getWidth() - leftWidth - rightWidth - sysRightWidth - sysMidWidth;

    //
    // draw the title bar images
    //
    Vector3 pos(absrect.d_left, absrect.d_top, z);
    Size    sz(leftWidth, absrect.getHeight());
    d_leftImage->draw(pos, sz, clipper, colours);
    pos.d_x += sz.d_width;

    sz.d_width = midWidth;
    d_middleImage->draw(pos, sz, clipper, colours);
    pos.d_x += sz.d_width;

    sz.d_width = rightWidth;
    d_rightImage->draw(pos, sz, clipper, colours);
    pos.d_x += sz.d_width;

    sz.d_width = sysMidWidth;
    d_sysMidImage->draw(pos, sz, clipper, colours);
    pos.d_x += sz.d_width;

    sz.d_width = sysRightWidth;
    d_sysRightImage->draw(pos, sz, clipper, colours);


    //
    // Draw the title text
    //
    // calculate colours to use for caption text
    colours.setColours(d_captionColour);
    colours.setAlpha(alpha);

    Rect textClipper(clipper);
    textClipper.setWidth(midWidth);
    textClipper = clipper.getIntersection(textClipper);
	pos.d_x = absrect.d_left + leftWidth * 0.5f;
	pos.d_y = absrect.d_top + ((absrect.getHeight() - getFont()->getLineSpacing()) * 0.25f);
    pos.d_z = System::getSingleton().getRenderer()->getZLayer(1);
    getFont()->drawText(d_parent->getText(), pos, textClipper, colours);
}


/*************************************************************************
    Handler for when drag mode changes
*************************************************************************/
void RLTitlebar::onDraggingModeChanged(WindowEventArgs& e)
{
    // base class processing
    Titlebar::onDraggingModeChanged(e);

    // change cursor depending on new mode.
    if (d_dragEnabled)
    {
        setMouseCursor(&ImagesetManager::getSingleton().getImageset(ImagesetName)->getImage(NormalCursorImageName));
    }
    else
    {
        setMouseCursor(&ImagesetManager::getSingleton().getImageset(ImagesetName)->getImage(NoDragCursorImageName));
    }

    requestRedraw();

    e.handled = true;
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

    Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
    Create, initialise and return a RLTitlebar
*************************************************************************/
Window* RLTitlebarFactory::createWindow(const String& name)
{
    RLTitlebar* wnd = new RLTitlebar(d_type, name);
    wnd->initialise();

    return wnd;
}

} // End of  CEGUI namespace section
