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

/************************************************************************
         filename:       ListboxImageItem.h
         created:        2/6/2004
         author:         Blakharaz
         
         purpose:        Interface for list box text items with line wrapping
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
#ifndef _ListboxImageItem_h_
#define _ListboxImageItem_h_

#include "UiPrerequisites.h"

#include "elements/CEGUIListboxTextItem.h"
#include "CEGUIFont.h"

namespace CEGUI {

class RenderableImage;

class _RlUiExport ListboxImageItem : public ListboxItem
{
public:
	ListboxImageItem(
		const Image* const image, 
		uint item_id = 0, 
		void* item_data = NULL, 
		bool disabled = false, 
		bool auto_delete = true);

	virtual ~ListboxImageItem();

	virtual void draw(
		const Vector3& position, 
		float alpha, 
		const Rect& clipper) const;

	virtual CEGUI::Size getPixelSize() const;

private:
	RenderableImage* d_image;
};

}

#endif
