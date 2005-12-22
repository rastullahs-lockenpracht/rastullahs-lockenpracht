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

#ifndef __Rl_AimlNodeImplXerces_H__
#define __Rl_AimlNodeImplXerces_H__

#include "DialogPrerequisites.h"
#include "AimlNode.h"

namespace rl
{
	XERCES_CPP_NAMESPACE_USE
	class XERCES_CPP_NAMESPACE::DOMNode;

	class _RlDialogExport AimlNodeImplXerces :
		public AimlNode
	{
	public:
		AimlNodeImplXerces(AimlNode* parent, DOMNode* node);
		~AimlNodeImplXerces(void);

		AimlNode* getFirstChild();
		AimlNode* getNextSibling();

	private:
		DOMNode* mNode;
	};
}

#endif
