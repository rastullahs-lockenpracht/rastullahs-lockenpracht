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
#include "AimlParser.h"
//#include "NaturalLanguageProcessor.h"
//#include "Predicates.h"
//#include "Nodemaster.h"
//#include "ScriptObject.h"
//#include "CoreSubsystem.h"
//#include "Interpreter.h"
//#include "XmlHelper.h"
//#include "DialogSubsystem.h"
/*
#include <string>


XERCES_CPP_NAMESPACE_USE
using namespace std;
*/
namespace rl
{
	AimlParser::AimlParser(NaturalLanguageProcessor* nlp) 
		: mNlp(nlp), mAimlCore(NULL) 
	{ 
	}
	
	AimlParser::AimlParser(AimlCoreComponent* aimlCore) 
		:  mNlp(NULL), mAimlCore(aimlCore) 
	{ 
	}

	bool AimlParser::parse(const std::string& fileName)
	{
		mFileName = fileName;
		return true;
	}

} // Namespace rl end
