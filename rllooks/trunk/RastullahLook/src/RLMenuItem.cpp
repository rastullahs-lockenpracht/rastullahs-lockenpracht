/************************************************************************
    filename:   RLMenuItem.cpp
    created:    2/4/2005
    author:     Tomas Lindquist Olsen (based on code by Paul D Turner)
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
#include "RLMenuItem.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIWindowManager.h"
//#include "CEGUIFont.h"


// Start of CEGUI namespace section
namespace CEGUI
{

const utf8 RLMenubarItem::WidgetTypeName[] = "RastullahLook/MenubarItem";

const utf8	RLMenubarItem::ImagesetName[]				= "RastullahLook";
const utf8	RLMenubarItem::TopLeftFrameImageName[]		= "MenuTopLeft";
const utf8	RLMenubarItem::TopRightFrameImageName[]		= "MenuTopRight";
const utf8	RLMenubarItem::BottomLeftFrameImageName[]	= "MenuBottomLeft";
const utf8	RLMenubarItem::BottomRightFrameImageName[]	= "MenuBottomRight";
const utf8	RLMenubarItem::LeftFrameImageName[]			= "MenuLeft";
const utf8	RLMenubarItem::RightFrameImageName[]		= "MenuRight";
const utf8	RLMenubarItem::TopFrameImageName[]			= "MenuTop";
const utf8	RLMenubarItem::BottomFrameImageName[]		= "MenuBottom";
const utf8	RLMenubarItem::BackgroundImageName[]		= "Background";


/*************************************************************************
	Constructor for RLMenubarItem widgets.	
*************************************************************************/
RLMenubarItem::RLMenubarItem(const String& type, const String& name) :
	MenuItem(type, name)
{
	setNormalTextColour(0xFF000000);
	setHoverColour(0xFFA7C7FF);
	setPushedColour(0xFF87A7DF);

	// get images
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	// store framesizes
	storeFrameSizes();

	d_frame.setImages(
		&iset->getImage(TopLeftFrameImageName),
		&iset->getImage(TopRightFrameImageName),
		&iset->getImage(BottomLeftFrameImageName),
		&iset->getImage(BottomRightFrameImageName),
		&iset->getImage(LeftFrameImageName),
		&iset->getImage(TopFrameImageName),
		&iset->getImage(RightFrameImageName),
		&iset->getImage(BottomFrameImageName)
	);

	d_fill.setImage(&iset->getImage(BackgroundImageName));
	d_fill.setHorzFormatting(RenderableImage::HorzStretched);
	d_fill.setVertFormatting(RenderableImage::VertStretched);
	
	// centre text in item
	d_textFormatting = Centred;
}


/*************************************************************************
	Destructor for RLMenubar widgets.
*************************************************************************/
RLMenubarItem::~RLMenubarItem(void)
{
}


/*************************************************************************
	Perform the actual rendering for this Window.
*************************************************************************/
void RLMenubarItem::drawSelf(float z)
{
	drawBackground(z);

	if (isDisabled())
		d_textColour = d_disabledTextColour;
	else
		d_textColour = d_normalTextColour;

	TextItem::drawSelf(z);
}


/*************************************************************************
	Draw the background.
*************************************************************************/
void RLMenubarItem::drawBackground(float z)
{
	Rect clipper(getPixelRect());

	// do nothing if the widget is totally clipped.
	if (clipper.getWidth() == 0 || isDisabled())
	{
		return;
	}

	// get the destination screen rect for this window
	Rect absrect(getUnclippedPixelRect());

	colour col;

	if (d_opened)
		col = d_openedColour;
	else if (d_pushed)
		col = d_pushedColour;
	else if (d_hovering)
		col = d_hoverColour;
	else
		return;

	ColourRect colours(col);
	colours.modulateAlpha(getEffectiveAlpha());

	//
	// draw the frame
	//
	Vector3 pos(absrect.d_left, absrect.d_top, z);
	d_frame.setSize(absrect.getSize());
	d_frame.setColours(colours);
	d_frame.draw(pos, clipper);

	// calculate size for middle title bar segment
	float midWidth		= absrect.getWidth() - d_frameLeftSize - d_frameRightSize;
	float midHeight		= absrect.getHeight() - d_frameTopSize - d_frameBottomSize;

	//
	// draw the centre fill
	//
	d_fill.setSize(Size(midWidth, midHeight));
	d_fill.setColours(colours);
	pos.d_x += d_frameLeftSize;
	pos.d_y += d_frameTopSize;
	d_fill.draw(pos, clipper);
}


/*************************************************************************
	Store the sizes for the frame edges
*************************************************************************/
void RLMenubarItem::storeFrameSizes(void)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	const Image* img;
	img = &iset->getImage(LeftFrameImageName);
	d_frameLeftSize = img->getWidth();

	img = &iset->getImage(RightFrameImageName);
	d_frameRightSize = img->getWidth();

	img = &iset->getImage(TopFrameImageName);
	d_frameTopSize = img->getHeight();

	img = &iset->getImage(BottomFrameImageName);
	d_frameBottomSize = img->getHeight();
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Factory Methods
*************************************************************************/
//////////////////////////////////////////////////////////////////////////

Window* RLMenubarItemFactory::createWindow(const String& name)
{
    return new RLMenubarItem(d_type, name);
}


void RLMenubarItemFactory::destroyWindow(Window* window)
{
    if (window->getType() == d_type)
    {
		delete window;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


const utf8 RLPopupMenuItem::WidgetTypeName[] = "RastullahLook/PopupMenuItem";

const utf8	RLPopupMenuItem::ImagesetName[]					= "RastullahLook";
const utf8	RLPopupMenuItem::TopLeftFrameImageName[]		= "MenuTopLeft";
const utf8	RLPopupMenuItem::TopRightFrameImageName[]		= "MenuTopRight";
const utf8	RLPopupMenuItem::BottomLeftFrameImageName[]		= "MenuBottomLeft";
const utf8	RLPopupMenuItem::BottomRightFrameImageName[]	= "MenuBottomRight";
const utf8	RLPopupMenuItem::LeftFrameImageName[]			= "MenuLeft";
const utf8	RLPopupMenuItem::RightFrameImageName[]			= "MenuRight";
const utf8	RLPopupMenuItem::TopFrameImageName[]			= "MenuTop";
const utf8	RLPopupMenuItem::BottomFrameImageName[]			= "MenuBottom";
const utf8	RLPopupMenuItem::BackgroundImageName[]			= "Background";

const utf8	RLPopupMenuItem::ArrowLeftImageName[]			= "PopupMenuArrowLeft";
const utf8	RLPopupMenuItem::ArrowRightImageName[]			= "PopupMenuArrowRight";


/*************************************************************************
	Constructor for RLPopupMenuItem widgets.	
*************************************************************************/
RLPopupMenuItem::RLPopupMenuItem(const String& type, const String& name) :
	MenuItem(type, name)
{
	setNormalTextColour(0xFF000000);
	setHoverColour(0xFFA7C7FF);
	setPushedColour(0xFF87A7DF);
	
	d_textXOffset = 20;

	// get images
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	// store framesizes
	storeFrameSizes();

	d_frame.setImages(
		&iset->getImage(TopLeftFrameImageName),
		&iset->getImage(TopRightFrameImageName),
		&iset->getImage(BottomLeftFrameImageName),
		&iset->getImage(BottomRightFrameImageName),
		&iset->getImage(LeftFrameImageName),
		&iset->getImage(TopFrameImageName),
		&iset->getImage(RightFrameImageName),
		&iset->getImage(BottomFrameImageName)
	);

	d_fill.setImage(&iset->getImage(BackgroundImageName));
	d_fill.setHorzFormatting(RenderableImage::HorzStretched);
	d_fill.setVertFormatting(RenderableImage::VertStretched);
	
	d_arrowLeft.setImage(&iset->getImage(ArrowLeftImageName));
	d_arrowLeft.setHorzFormatting(RenderableImage::HorzCentred);
	d_arrowLeft.setVertFormatting(RenderableImage::VertCentred);
	
	d_arrowRight.setImage(&iset->getImage(ArrowRightImageName));
	d_arrowRight.setHorzFormatting(RenderableImage::HorzCentred);
	d_arrowRight.setVertFormatting(RenderableImage::VertCentred);
}


/*************************************************************************
	Destructor for RLPopupMenuItem widgets.
*************************************************************************/
RLPopupMenuItem::~RLPopupMenuItem(void)
{
}


/*************************************************************************
	Perform the actual rendering for this Window.
*************************************************************************/
void RLPopupMenuItem::drawSelf(float z)
{
	drawBackground(z);

	// text
	if (isDisabled())
		d_textColour = d_disabledTextColour;
	else
		d_textColour = d_normalTextColour;

	TextItem::drawSelf(z);
}


/*************************************************************************
	Draw the background.
*************************************************************************/
void RLPopupMenuItem::drawBackground(float z)
{
	Rect clipper(getPixelRect());

	// do nothing if the widget is totally clipped.
	if (clipper.getWidth() == 0 || isDisabled())
	{
		return;
	}

	// get the destination screen rect for this window
	Rect absrect(getUnclippedPixelRect());

	colour col;
	bool draw_bg = true;

	if (d_opened)
		col = d_openedColour;
	else if (d_pushed)
		col = d_pushedColour;
	else if (d_hovering)
		col = d_hoverColour;
	else
		draw_bg = false;

	col.setAlpha(getEffectiveAlpha());
	ColourRect colours(col);

	if (draw_bg)
	{
		//
		// draw the frame
		//
		Vector3 pos(absrect.d_left, absrect.d_top, z);
		d_frame.setSize(absrect.getSize());
		d_frame.setColours(colours);
		d_frame.draw(pos, clipper);

		// calculate size for middle title bar segment
		float midWidth		= absrect.getWidth() - d_frameLeftSize - d_frameRightSize;
		float midHeight		= absrect.getHeight() - d_frameTopSize - d_frameBottomSize;

		//
		// draw the centre fill
		//
		d_fill.setSize(Size(midWidth, midHeight));
		d_fill.setColours(colours);
		pos.d_x += d_frameLeftSize;
		pos.d_y += d_frameTopSize;
		d_fill.draw(pos, clipper);
	}

	// draw a popup indicator arrow for items with a popup menu
	if (d_popup!=NULL)
	{
		RenderableImage& arrow = d_opened?d_arrowLeft:d_arrowRight;

		arrow.setSize(Size(absrect.getHeight(),absrect.getHeight()));
		arrow.setColours(ColourRect(colour(0,0,0,col.getAlpha())));
		arrow.draw(Vector3(absrect.d_right-absrect.getHeight(),absrect.d_top,z),clipper);
	}

}


/*************************************************************************
	Store the sizes for the frame edges
*************************************************************************/
void RLPopupMenuItem::storeFrameSizes(void)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	const Image* img;
	img = &iset->getImage(LeftFrameImageName);
	d_frameLeftSize = img->getWidth();

	img = &iset->getImage(RightFrameImageName);
	d_frameRightSize = img->getWidth();

	img = &iset->getImage(TopFrameImageName);
	d_frameTopSize = img->getHeight();

	img = &iset->getImage(BottomFrameImageName);
	d_frameBottomSize = img->getHeight();
}


/*************************************************************************
	Get the optimal pixelsize for this PopupMenuItem
*************************************************************************/
Size RLPopupMenuItem::getItemPixelSize()
{
	Size sz = MenuItem::getItemPixelSize();
	sz.d_width += d_arrowRight.getImage()->getWidth();
	return sz;
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Factory Methods
*************************************************************************/
//////////////////////////////////////////////////////////////////////////

Window* RLPopupMenuItemFactory::createWindow(const String& name)
{
    return new RLPopupMenuItem(d_type, name);
}


void RLPopupMenuItemFactory::destroyWindow(Window* window)
{
    if (window->getType() == d_type)
    {
		delete window;
	}
}

} // End of  CEGUI namespace section
