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
#ifndef __Rl_AimlParser_H__
#define __Rl_AimlParser_H__

#include "DialogPrerequisites.h"

#include <string>

namespace rl
{
	class NaturalLanguageProcessor;
	class AimlCoreComponent;

	/** Handler class used to parse the startup xml file and aiml files using SAX2
	 *  @author Philipp Walser
     */
	class _RlDialogExport AimlParser
	{
	public:
		//! Constructor
		AimlParser(NaturalLanguageProcessor* nlp);
		AimlParser(AimlCoreComponent* aimlCore);
		//! Destructor
		virtual ~AimlParser(void){ }

		virtual bool parse(const std::string& fileName);

	protected:
		std::string mFileName;
		NaturalLanguageProcessor* mNlp;
		AimlCoreComponent* mAimlCore;
	};

} // End of  rl namespace section

#endif
