/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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

#include "ListboxImageItem.h"
#include <CEGUIWindow.h>
#include <CEGUIImage.h>

namespace CEGUI {

ListboxImageItem::ListboxImageItem(const Image* image, uint item_id, void* item_data, bool disabled, bool auto_delete)
: ListboxItem("aaa", item_id, item_data, disabled, auto_delete)
{
	d_image = image;
}

ListboxImageItem::~ListboxImageItem()
{
}

void ListboxImageItem::draw(const Vector3& position, float alpha, const Rect& clipper) const
{
	if (d_selected && (d_selectBrush != NULL))
    {
        d_selectBrush->draw(clipper, position.d_z, clipper, getModulateAlphaColourRect(d_selectCols, alpha));
    }

	d_image->draw(position, clipper);
}

CEGUI::Size ListboxImageItem::getPixelSize() const
{
	return d_image->getSize();
}


}
