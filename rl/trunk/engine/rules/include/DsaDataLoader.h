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

#ifndef __DsaDataLoader_H__
#define __DsaDataLoader_H__

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>

#include <string>

namespace rl {

	class Talent;
	class Person;

	class DsaDataLoader
	{
	public:
		static void loadData(std::string filename);

	private:
		static XERCES_CPP_NAMESPACE::DOMDocument* loadDataFile(std::string filename);
		
		static int getEBeFromString(const std::string& eBeString);

		static void initializeTalente(XERCES_CPP_NAMESPACE::DOMElement*);
		static Talent* processTalent(int id, int gruppe, XERCES_CPP_NAMESPACE::DOMElement* talentXml);

		static void initializePersonen(XERCES_CPP_NAMESPACE::DOMElement*);
		static Person* processPerson(int id, XERCES_CPP_NAMESPACE::DOMElement* talentXml);
			
		static void initializeKampftechniken(XERCES_CPP_NAMESPACE::DOMElement*);

		DsaDataLoader();
	};

}

#endif
