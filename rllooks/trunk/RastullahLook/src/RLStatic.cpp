/************************************************************************
	filename: 	RLStatic.cpp
	created:	5/6/2004
	author:		Paul D Turner
	
	purpose:	Implementation of Taharez Look static widgets & factories.
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
#include "RLStatic.h"
#include "elements/CEGUIStaticText.h"
#include "elements/CEGUIStaticImage.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIWindowManager.h"
#include "RLMiniVertScrollbar.h"
#include "RLMiniHorzScrollbar.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
const utf8	RLStaticText::WidgetTypeName[]		= "RastullahLook/StaticText";
const utf8	RLStaticImage::WidgetTypeName[]		= "RastullahLook/StaticImage";

// component widget type names
const utf8*	RLStaticText::HorzScrollbarTypeName	= RLMiniHorzScrollbar::WidgetTypeName;
const utf8*	RLStaticText::VertScrollbarTypeName	= RLMiniVertScrollbar::WidgetTypeName;


/*************************************************************************
	Routine to do some common initialisation of static widgets
*************************************************************************/
void initTaharezStatic(Static* s)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset((utf8*)"RastullahLook");
	
	s->setFrameImages(
		&iset->getImage((utf8*)"StaticTopLeft"),
		&iset->getImage((utf8*)"StaticTopRight"),
		&iset->getImage((utf8*)"StaticBottomLeft"),
		&iset->getImage((utf8*)"StaticBottomRight"),
		&iset->getImage((utf8*)"StaticLeft"),
		&iset->getImage((utf8*)"StaticTop"),
		&iset->getImage((utf8*)"StaticRight"),
		&iset->getImage((utf8*)"StaticBottom")
		);

	s->setBackgroundImage(&iset->getImage((utf8*)"StaticBackdrop"));

	s->setFrameEnabled(true);
	s->setBackgroundEnabled(true);
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	
	RLStaticText methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////

/*************************************************************************
	create and return a pointer to a Scrollbar widget for use as
	vertical scroll bar	
*************************************************************************/
Scrollbar* RLStaticText::createVertScrollbar(const String& name) const
{
	Scrollbar* sbar = (Scrollbar*)WindowManager::getSingleton().createWindow(VertScrollbarTypeName, name);

	// set min/max sizes
	sbar->setMinimumSize(Size(0.0125f, 0.0f));
	sbar->setMaximumSize(Size(0.0125f, 1.0f));

	return sbar;
}


/*************************************************************************
	create and return a pointer to a Scrollbar widget for use as
	horizontal scroll bar	
*************************************************************************/
Scrollbar* RLStaticText::createHorzScrollbar(const String& name) const
{
	Scrollbar* sbar = (Scrollbar*)WindowManager::getSingleton().createWindow(HorzScrollbarTypeName, name);

	// set min/max sizes
	sbar->setMinimumSize(Size(0.0f, 0.016667f));
	sbar->setMaximumSize(Size(1.0f, 0.016667f));

	return sbar;
}


/*************************************************************************
	Initialises the Window based object ready for use.
*************************************************************************/
void RLStaticText::initialise(void)
{
	StaticText::initialise();
	initTaharezStatic(this);
}


/*************************************************************************
	Setup size and position for the component widgets attached to this
	RLStaticText
*************************************************************************/
void RLStaticText::performChildWindowLayout()
{
    // base class layout
    Static::performChildWindowLayout();

	// set desired size for vertical scroll-bar
	Size v_sz(0.05f, 1.0f);
	d_vertScrollbar->setSize(v_sz);

	// get the actual size used for vertical scroll bar.
	v_sz = absoluteToRelative(d_vertScrollbar->getAbsoluteSize());


	// set desired size for horizontal scroll-bar
	Size h_sz(1.0f, 0.0f);

	if (getAbsoluteHeight() != 0.0f)
	{
		h_sz.d_height = (getAbsoluteWidth() * v_sz.d_width) / getAbsoluteHeight();
	}

	// adjust length to consider width of vertical scroll bar if that is visible
	if (d_vertScrollbar->isVisible())
	{
		h_sz.d_width -= v_sz.d_width;
	}

	d_horzScrollbar->setSize(h_sz);

	// get actual size used
	h_sz = absoluteToRelative(d_horzScrollbar->getAbsoluteSize());


	// position vertical scroll bar
	d_vertScrollbar->setPosition(Point(1.0f - v_sz.d_width, 0.0f));

	// position horizontal scroll bar
	d_horzScrollbar->setPosition(Point(0.0f, 1.0f - h_sz.d_height));
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	
	RLStaticImage methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////

/*************************************************************************
	Initialises the Window based object ready for use.
*************************************************************************/
void RLStaticImage::initialise(void)
{
	StaticImage::initialise();
	initTaharezStatic(this);
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a StaticText for the Taharez Scheme
*************************************************************************/
Window* RLStaticTextFactory::createWindow(const String& name)
{
	return new RLStaticText(d_type, name);
}


/*************************************************************************
	Create, initialise and return a StaticImage for the Taharez Scheme
*************************************************************************/
Window* RLStaticImageFactory::createWindow(const String& name)
{
	return new RLStaticImage(d_type, name);
}

} // End of  CEGUI namespace section
