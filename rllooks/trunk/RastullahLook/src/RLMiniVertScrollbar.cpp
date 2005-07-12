/************************************************************************
	filename: 	RLMiniVertScrollbar.cpp
	created:	2/6/2004
	author:		Paul D Turner
	
	purpose:	Implementation of Rastullah mini vertical scroll bar widget.
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

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
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIWindowManager.h"
#include "RLMiniVertScrollbar.h"
#include "RLMiniVertScrollbarThumb.h"
#include "RLButton.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
// type name for this widget
const utf8	RLMiniVertScrollbar::WidgetTypeName[]	= "RastullahLook/VerticalScrollbar";

// Progress bar image names
const utf8	RLMiniVertScrollbar::ImagesetName[]					= "RastullahLook";
const utf8	RLMiniVertScrollbar::ScrollbarBodyImageName[]		= "MiniVertScrollBarSegment";
const utf8	RLMiniVertScrollbar::UpButtonNormalImageName[]		= "MiniVertScrollUpNormal";
const utf8	RLMiniVertScrollbar::UpButtonHighlightImageName[]	= "MiniVertScrollUpHover";
const utf8	RLMiniVertScrollbar::DownButtonNormalImageName[]	= "MiniVertScrollDownNormal";
const utf8	RLMiniVertScrollbar::DownButtonHighlightImageName[]	= "MiniVertScrollDownHover";

// some layout stuff
const float	RLMiniVertScrollbar::ThumbPositionX	= 0.15f;
const float	RLMiniVertScrollbar::ThumbWidth		= 0.7f;				
const float	RLMiniVertScrollbar::BodyPositionX	= 0.3f;			
const float	RLMiniVertScrollbar::BodyWidth		= 0.4f;

// type names for the component widgets
const utf8*	RLMiniVertScrollbar::ThumbWidgetType			= RLMiniVertScrollbarThumb::WidgetTypeName;
const utf8*	RLMiniVertScrollbar::IncreaseButtonWidgetType	= RLButton::WidgetTypeName;
const utf8*	RLMiniVertScrollbar::DecreaseButtonWidgetType	= RLButton::WidgetTypeName;


/*************************************************************************
	Constructor for Rastullah mini vertical scroll bar widgets
*************************************************************************/
RLMiniVertScrollbar::RLMiniVertScrollbar(const String& type, const String& name) :
	Scrollbar(type, name)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	// setup cache of image pointers
	d_body = &iset->getImage(ScrollbarBodyImageName);
}


/*************************************************************************
	Destructor for Rastullah mini vertical scroll bar widgets
*************************************************************************/
RLMiniVertScrollbar::~RLMiniVertScrollbar(void)
{
}


/*************************************************************************
	create a PushButton based widget to use as the increase button for
	this scroll bar.
*************************************************************************/
PushButton* RLMiniVertScrollbar::createIncreaseButton(void) const
{
	// create the widget
	RLButton* btn = (RLButton*)WindowManager::getSingleton().createWindow(IncreaseButtonWidgetType, getName() + "__auto_incbtn__");

	// perform some initialisation
	btn->setStandardImageryEnabled(false);
	btn->setCustomImageryAutoSized(true);
	btn->setAlwaysOnTop(true);

	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);
	RenderableImage img;
	img.setHorzFormatting(RenderableImage::HorzStretched);
	img.setVertFormatting(RenderableImage::VertStretched);
	img.setImage(&iset->getImage(DownButtonNormalImageName));
	btn->setNormalImage(&img);
	btn->setDisabledImage(&img);

	img.setImage(&iset->getImage(DownButtonHighlightImageName));
	btn->setHoverImage(&img);
	btn->setPushedImage(&img);

	return btn;
}


/*************************************************************************
	create a PushButton based widget to use as the decrease button for
	this scroll bar.
*************************************************************************/
PushButton* RLMiniVertScrollbar::createDecreaseButton(void) const
{
	// create the widget
	RLButton* btn = (RLButton*)WindowManager::getSingleton().createWindow(DecreaseButtonWidgetType, getName() + "__auto_decbtn__");

	// perform some initialisation
	btn->setStandardImageryEnabled(false);
	btn->setCustomImageryAutoSized(true);
	btn->setAlwaysOnTop(true);

	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);
	RenderableImage img;
	img.setHorzFormatting(RenderableImage::HorzStretched);
	img.setVertFormatting(RenderableImage::VertStretched);
	img.setImage(&iset->getImage(UpButtonNormalImageName));
	btn->setNormalImage(&img);
	btn->setDisabledImage(&img);

	img.setImage(&iset->getImage(UpButtonHighlightImageName));
	btn->setHoverImage(&img);
	btn->setPushedImage(&img);

	return btn;
}


/*************************************************************************
	create a Thumb based widget to use as the thumb for this scroll bar.
*************************************************************************/
Thumb* RLMiniVertScrollbar::createThumb(void) const
{
	// create the widget
	RLMiniVertScrollbarThumb* thumb = (RLMiniVertScrollbarThumb*)WindowManager::getSingleton().createWindow(ThumbWidgetType, getName() + "__auto_thumb__");

	// perform some initialisation
	thumb->setVertFree(true);
	thumb->setXPosition(ThumbPositionX);
	thumb->setWidth(ThumbWidth);

	return thumb;
}


/*************************************************************************
	layout the scroll bar component widgets
*************************************************************************/
void RLMiniVertScrollbar::layoutComponentWidgets(void)
{
	Size bsz;
	bsz.d_width = bsz.d_height = d_abs_area.getWidth();

	// install button sizes
	d_increase->setSize(absoluteToRelative(bsz));
	d_decrease->setSize(absoluteToRelative(bsz));

	// position buttons
	d_decrease->setPosition(Point(0.0f, 0.0f));
	d_increase->setPosition(Point(0.0f, absoluteToRelativeY(d_abs_area.getHeight() - bsz.d_height)));

	// this will configure thumb widget appropriately
	updateThumb();
}


/*************************************************************************
	update the size and location of the thumb to properly represent the
	current state of the scroll bar
*************************************************************************/
void RLMiniVertScrollbar::updateThumb(void)
{
	// calculate actual padding values to use.
	float slideTrackYPadding = d_decrease->getAbsoluteHeight();

	// calculate maximum extents for thumb positioning.
	float posExtent		= d_documentSize - d_pageSize;
	float slideExtent	= ceguimax(0.0f, d_abs_area.getHeight() - (2 * slideTrackYPadding) - d_thumb->getAbsoluteHeight());

	// Thumb does not change size with document length, we just need to update position and range
	d_thumb->setVertRange(absoluteToRelativeY(slideTrackYPadding), absoluteToRelativeY(slideTrackYPadding + slideExtent));
	d_thumb->setYPosition(absoluteToRelativeY(slideTrackYPadding + (d_position * (slideExtent / posExtent))));
}


/*************************************************************************
	return value that best represents current scroll bar position given
	the current location of the thumb.
*************************************************************************/
float RLMiniVertScrollbar::getValueFromThumb(void) const
{
	// calculate actual padding values to use.
	float slideTrackYPadding = d_decrease->getAbsoluteHeight();

	// calculate maximum extents for thumb positioning.
	float posExtent		= d_documentSize - d_pageSize;
	float slideExtent	= d_abs_area.getHeight() - (2 * slideTrackYPadding) - d_thumb->getAbsoluteHeight();

	return	(d_thumb->getAbsoluteYPosition() - slideTrackYPadding) / (slideExtent / posExtent);
}


/*************************************************************************
	Given window location 'pt', return a value indicating what change
	should be made to the scroll bar.
*************************************************************************/
float RLMiniVertScrollbar::getAdjustDirectionFromPoint(const Point& pt) const
{
	Rect absrect(d_thumb->getUnclippedPixelRect());

	if (pt.d_y < absrect.d_top)
	{
		return -1.0f;
	}
	else if (pt.d_y > absrect.d_bottom)
	{
		return 1.0f;
	}
	else
	{
		return 0.0f;
	}

}


/*************************************************************************
	Perform rendering for this widget
*************************************************************************/
void RLMiniVertScrollbar::drawSelf(float z)
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
	ColourRect colours(colour(1, 1, 1, getEffectiveAlpha()));

	//
	// Render bar body
	//
	float button_height = d_decrease->getAbsoluteHeight();

	Vector3 pos(absrect.d_left + PixelAligned(absrect.getWidth() * BodyPositionX), absrect.d_top + button_height, z);
	Size	sz(PixelAligned(absrect.getWidth() * BodyWidth), absrect.getHeight() - PixelAligned(button_height * 0.5f));

	d_body->draw(pos, sz, clipper, colours);
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a RLMiniVertScrollbar
*************************************************************************/
Window* RLMiniVertScrollbarFactory::createWindow(const String& name)
{
	RLMiniVertScrollbar* wnd = new RLMiniVertScrollbar(d_type, name);
	wnd->initialise();

	return wnd;
}

} // End of  CEGUI namespace section
