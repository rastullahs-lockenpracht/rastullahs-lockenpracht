/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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

#include "stdinc.h"

#include "DialogElement.h"

#include "DialogParagraph.h"

namespace rl
{
    DialogElement::DialogElement(int id)
        : mId(id)
    {
    }

    DialogElement::~DialogElement()
    {
        for (std::list<DialogParagraph*>::iterator it = mParagraphs.begin();
            it != mParagraphs.end(); ++it)
        {
            delete *it;
        }
    }

    int DialogElement::getId() const
    {
        return mId;
    }

    void DialogElement::addParagraph(DialogParagraph* paragraph)
    {
        mParagraphs.push_back(paragraph);
    }

    std::list<DialogParagraph*> DialogElement::getParagraphs(Dialog* dialog)
    {
        return mParagraphs;
    }

	bool DialogElement::isSelection() const
	{
		return false;
	}

}
