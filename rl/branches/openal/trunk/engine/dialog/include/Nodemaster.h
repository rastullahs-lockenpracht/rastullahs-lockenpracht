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

#ifndef __Rl_DialogNodemaster_H__
#define __Rl_DialogNodemaster_H__

//--	Nodemaster.h
#include <xercesc/dom/DOM.hpp>
#include "DialogPrerequisites.h"

#include <map>
#include <string>


XERCES_CPP_NAMESPACE_USE
using namespace std;

namespace rl
{
	class _RlDialogExport Nodemaster
	{
	public:
		Nodemaster();
		Nodemaster(const string &templateValue);
		virtual ~Nodemaster();

		void deleteAllNodes();
		DOMNode* getTemplateNode() const;
		string getTemplate();
		void setTemplate(const string& data);

		Nodemaster* getChild(const string& key);
		void addChild(const string& key, Nodemaster *child);

	private:
		map<string, Nodemaster *> mChildren;
		static Nodemaster* cached;
		string mTemplate;
	};
}
#endif
