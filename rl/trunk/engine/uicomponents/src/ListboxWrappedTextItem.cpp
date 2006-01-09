/************************************************************************
	filename: 	ListboxWrappedTextItem.cpp
	created:	01/8/2006
	author:		Paul D Turner, Philipp Walser
	
	purpose:	Implementation of List box text items with WordWrapping
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
#include "ListboxWrappedTextItem.h"
#include "CEGUIFontManager.h"
#include "CEGUIFont.h"
#include "CEGUIWindow.h"
#include "CEGUIImage.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
const colour	ListboxWrappedTextItem::DefaultTextColour		= 0xFFFFFFFF;


/*************************************************************************
	Constructor
*************************************************************************/
ListboxWrappedTextItem::ListboxWrappedTextItem(const String& text, uint item_id, void* item_data, bool disabled, bool auto_delete) :
	ListboxItem(text, item_id, item_data, disabled, auto_delete),
	d_textCols(DefaultTextColour, DefaultTextColour, DefaultTextColour, DefaultTextColour),
	d_font(0), d_textFormatting(WordWrapLeftAligned)
{
}

/*************************************************************************
	Destructor
*************************************************************************/
ListboxWrappedTextItem::~ListboxWrappedTextItem()
{
}


/*************************************************************************
	Return a pointer to the font being used by this ListboxWrappedTextItem
*************************************************************************/
const Font* ListboxWrappedTextItem::getFont(void) const
{
	// prefer out own font
	if (d_font)
	{
		return d_font;
	}
	// try our owner window's font setting (may be null if owner uses no existant default font)
	else if (d_owner)
	{
		return d_owner->getFont();
	}
	// no owner, just use the default (which may be NULL anyway)
	else
	{
		return System::getSingleton().getDefaultFont();
	}

}


/*************************************************************************
	Set the font to be used by this ListboxWrappedTextItem
*************************************************************************/
void ListboxWrappedTextItem::setFont(const String& font_name)
{
	setFont(FontManager::getSingleton().getFont(font_name));
}

/*************************************************************************
	Return the rendered pixel size of this list box item.
*************************************************************************/
Size ListboxWrappedTextItem::getPixelSize(void) const
{
	Size size(0,0);
	
	if (d_owner != NULL)
	{
         size.d_width = d_owner->getAbsoluteSize().d_width - 25;
	}

	const Font* fnt = getFont();

	if (fnt != NULL)
	{
		size.d_height	= PixelAligned(fnt->getLineSpacing());
		if(size.d_width == 0)
		{
			size.d_width	= PixelAligned(fnt->getTextExtent(d_itemText));
		}
				 
		Rect formatRect;
		
		formatRect.setPosition(CEGUI::Point(0,0));
		formatRect.setSize(size);
		uint lines = fnt->getFormattedLineCount(d_itemText, formatRect, d_textFormatting);
		size.d_height *= lines;
		
	}



	return size;
}


/*************************************************************************
	Draw the list box item in its current state.
*************************************************************************/
void ListboxWrappedTextItem::draw(const Vector3& position, float alpha, const Rect& clipper) const
{
	if (d_selected && (d_selectBrush != 0))
	{
		d_selectBrush->draw(clipper, position.d_z, clipper, getModulateAlphaColourRect(d_selectCols, alpha));
	}

	const Font* fnt = getFont();

	if (fnt)
	{
        Vector3 finalPos(position);
        finalPos.d_y -= PixelAligned((fnt->getLineSpacing() - fnt->getBaseline()) * 0.5f);
		Rect draw_area(finalPos.d_x, finalPos.d_y, clipper.d_right, finalPos.d_y);
		//fnt->drawText(d_itemText, finalPos, clipper, getModulateAlphaColourRect(d_textCols, alpha));
		fnt->drawText(d_itemText, draw_area, finalPos.d_z, clipper, d_textFormatting, 
			getModulateAlphaColourRect(d_textCols, alpha));  
	}

}

void ListboxWrappedTextItem::draw(RenderCache& cache,const Rect& targetRect, float zBase, float alpha, const Rect* clipper) const
{
    if (d_selected && d_selectBrush != 0)
    {
        cache.cacheImage(*d_selectBrush, targetRect, zBase, getModulateAlphaColourRect(d_selectCols, alpha), clipper);
    }

    const Font* font = getFont();

    if (font)
    {
        Rect finalPos(targetRect);
        finalPos.d_top -= (font->getLineSpacing() - font->getBaseline()) * 0.5f;
        cache.cacheText(d_itemText, font, d_textFormatting, finalPos, zBase, getModulateAlphaColourRect(d_textCols, alpha), clipper);
	}
}


/*************************************************************************
	Set the colours used for text rendering.	
*************************************************************************/
void ListboxWrappedTextItem::setTextColours(colour top_left_colour, colour top_right_colour, colour bottom_left_colour, colour bottom_right_colour)
{
	d_textCols.d_top_left		= top_left_colour;
	d_textCols.d_top_right		= top_right_colour;
	d_textCols.d_bottom_left	= bottom_left_colour;
	d_textCols.d_bottom_right	= bottom_right_colour;
}

void ListboxWrappedTextItem::setTextFormatting(TextFormatting fmt)
{
	d_textFormatting = fmt;
}

const TextFormatting& ListboxWrappedTextItem::getTextFormatting() const
{
	return d_textFormatting;
}


} // End of  CEGUI namespace section




/*
void ListboxWrappedTextItem::draw(const Vector3& position, float alpha, const Rect& clipper) const
{
	if (d_selected && (d_selectBrush != NULL))
    {
        d_selectBrush->draw(clipper, position.d_z, clipper, getModulateAlphaColourRect(d_selectCols, alpha));
    }
 
    const Font* fnt = getFont();

    if (fnt != NULL)
    {
        Vector3 finalPos(position);
        finalPos.d_y -= PixelAligned((fnt->getLineSpacing() - fnt->getBaseline()) * 0.5f);
		Rect draw_area(finalPos.d_x, finalPos.d_y, clipper.d_right, finalPos.d_y);
		//fnt->drawWrappedText(d_itemText, draw_area, finalPos.d_y, clipper, d_textFormatting,
		//	getModulateAlphaColourRect(d_textCols, alpha));
		//fnt->drawText(d_itemText, draw_area, finalPos.d_z, clipper, d_textFormatting, getModulateAlphaColourRect(d_textCols, alpha));  
		//fnt->drawText(d_itemText, finalPos, clipper, getModulateAlphaColourRect(d_textCols, alpha));
	}
	
}*/
/*
CEGUI::Size ListboxWrappedTextItem::getPixelSize() const
{
	 CEGUI::Size size = ListboxWrappedTextItem::getPixelSize();
	 if (d_owner != NULL)
	 {
         size.d_width = d_owner->getAbsoluteSize().d_width;
	 }

     const Font* fnt = getFont();
 
     if (fnt != NULL)
     {		 
		 Rect formatRect;

		 formatRect.setPosition(CEGUI::Point(0,0));
		 formatRect.setSize(size);
		 uint lines = fnt->getFormattedLineCount(d_itemText, formatRect, d_textFormatting);
         size.d_height *= lines;
     }

	 return size;
}
*/


