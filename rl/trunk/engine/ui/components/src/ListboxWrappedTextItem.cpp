/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */

#include "ListboxWrappedTextItem.h"

#include "CEGUIImage.h"

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

Size ListboxWrappedTextItem::getPixelSize() const
{
	 Size size = ListboxTextItem::getPixelSize();

     const Font* fnt = getFont();
 
     if (fnt != NULL)
     {		 
		 Rect formatRect;

		 formatRect.setPosition(Point(0,0));
		 formatRect.setSize(size);
		 uint lines = fnt->getFormattedLineCount(d_itemText, formatRect, d_textFormatting);
         size.d_height *= lines;
     }

	 return size;
}


}
