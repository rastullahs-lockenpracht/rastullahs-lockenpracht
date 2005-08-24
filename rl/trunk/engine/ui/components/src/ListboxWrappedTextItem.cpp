/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Clarified Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Clarified Artistic License for more details.
 *
 *  You should have received a copy of the Clarified Artistic License
 *  along with this program; if not you can get it here
 *  http://www.jpaulmorrison.com/fbp/artistic2.htm.
 */

//#include <elements/CEGUIListbox.h>
#include "ListboxWrappedTextItem.h"
#include <CEGUIWindow.h>
#include <CEGUIImage.h>


namespace CEGUI {

ListboxWrappedTextItem::ListboxWrappedTextItem(const String& text, uint item_id, void* item_data, bool disabled, bool auto_delete)
: ListboxTextItem(text, item_id, item_data, disabled, auto_delete)
{
	d_textFormatting = LeftAligned;
}

ListboxWrappedTextItem::~ListboxWrappedTextItem()
{
}

void ListboxWrappedTextItem::setTextFormatting(TextFormatting fmt)
{
	d_textFormatting = fmt;
}

const TextFormatting& ListboxWrappedTextItem::getTextFormatting() const
{
	return d_textFormatting;
}

void ListboxWrappedTextItem::draw(const Vector3& position, float alpha, const Rect& clipper) const
{
	if (d_selected && (d_selectBrush != NULL))
    {
        d_selectBrush->draw(clipper, position.d_z, clipper, getModulateAlphaColourRect(d_selectCols, alpha));
    }
 
    const Font* fnt = getFont();

    if (fnt != NULL)
    {
         fnt->drawText(d_itemText, clipper, position.d_z, clipper, d_textFormatting, getModulateAlphaColourRect(d_textCols, alpha));
    }
}

CEGUI::Size ListboxWrappedTextItem::getPixelSize() const
{
	 CEGUI::Size size = ListboxTextItem::getPixelSize();
	 if (d_owner != NULL)
	 {
         size.d_width = d_owner->getSize().d_width;
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


}
