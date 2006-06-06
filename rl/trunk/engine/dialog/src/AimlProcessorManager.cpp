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

#include <xercesc/util/XMemory.hpp>
#include "AimlProcessorManager.h"
#include "processors/BrProcessor.h"
#include "processors/ConditionProcessor.h"
#include "processors/SystemProcessor.h"
#include "processors/SraiProcessor.h"
#include "processors/SetProcessor.h"
#include "processors/GetProcessor.h"

namespace rl
{
	map<CeGuiString, AimlProcessor *> AimlProcessorManager::msProcessors;

	void AimlProcessorManager::init()
	{
/*		mmProcessors["br"] = new BrProcessor();
		mmProcessors["id"] = new IdProcessor();
		mmProcessors["sr"] = new SrProcessor();
		mProcessors["bot"] = new BotProcessor();
		mProcessors["get"] = new GetProcessor();
	// 	mProcessors["irc"] = new IrcProcessor();
		mProcessors["set"] = new SetProcessor();
		mProcessors["name"] = new NameProcessor();
		mProcessors["srai"] = new SraiProcessor();
		mProcessors["star"] = new StarProcessor();
		mProcessors["that"] = new ThatProcessor();
		mProcessors["input"] = new InputProcessor();
		mProcessors["learn"] = new LearnProcessor();
		mProcessors["think"] = new ThinkProcessor();
		mProcessors["formal"] = new FormalProcessor();
		mProcessors["person"] = new PersonProcessor();
		mProcessors["random"] = new RandomProcessor();
		mProcessors["sentence"] = new SentenceProcessor();
		mProcessors["thatstar"] = new ThatstarProcessor();
		mProcessors["condition"] = new ConditionProcessor();
		mProcessors["lowercase"] = new LowercaseProcessor();
		mProcessors["normalize"] = new NormalizeProcessor();
		mProcessors["topicstar"] = new TopicstarProcessor();
		mProcessors["uppercase"] = new UppercaseProcessor();
		mProcessors["template"] = new TemplateProcessor();
	//	mProcessors["http"] = new HTTPProcessor();
		mProcessors["secure"] = new SecureProcessor();
		mProcessors["system"] = new SystemProcessor();
	#if defined(ENABLE_JAVASCRIPT) 
		mProcessors["javascript"] = new JavaScriptProcessor();
	#endif
		mProcessors["authenticate"] = new AuthenticateProcessor();
	
		mProcessors["person2"] = new Person2Processor();
		mProcessors["gender"] = new GenderProcessor();
	//	mProcessors["xmlsocket"] = new XmlSocketProcessor();
	//	mProcessors["timer"] = new TimerProcessor();
		mProcessors["substitute"] = new SubstituteProcessor();
		mProcessors["version"] = new VersionProcessor();
		mProcessors["implode"] = new ImplodeProcessor();
		mProcessors["explode"] = new ExplodeProcessor();
	//	mProcessors["ruby"] = new RubyProcessor();				// Versuchen hiermit RubyCode zuzulassen
*/	}

	void AimlProcessorManager::addProcessor(const CeGuiString &name,AimlProcessor* proc)
	{
		msProcessors[name]=proc;
	}

    void AimlProcessorManager::shutdown()
    {
        set<void*> deleted;
		map<CeGuiString,AimlProcessor*>::const_iterator itr = msProcessors.begin();
		for(; itr!=msProcessors.end(); ++itr)
		{
            // Da gleiche Prozessoren mit verschiedenen Schlüsseln gespeichert sind,
            // prüfen, ob Prozessor nicht bereits gelöscht.
            if (deleted.find(itr->second) == deleted.end())
            {
			    delete itr->second;
                deleted.insert(itr->second);
            }
		}
		msProcessors.clear();
    }

	void AimlProcessorManager::addStandardProcessors()
	{
	//	ConditionProcessor* cp = new ConditionProcessor();
		msProcessors["br"] = new BrProcessor();
	//	msProcessors["selection"] = cp;
	//	msProcessors["condition"] = cp;
	//	msProcessors["option"] = cp;
	//	msProcessors["if"] = cp;
		msProcessors["srai"] = new SraiProcessor();
		msProcessors["system"] = new SystemProcessor();
		msProcessors["set"] = new SetProcessor();
		msProcessors["get"] = new GetProcessor();
	}

	AimlProcessor* AimlProcessorManager::getProcessor(const CeGuiString &name)
	{
		map<CeGuiString, AimlProcessor *>::const_iterator itr = msProcessors.find(name);
		if(itr != msProcessors.end())
			return itr->second;
		else return NULL;
	}

	bool AimlProcessorManager::hasProcessor(const CeGuiString &name) {
		return msProcessors.find(name) != msProcessors.end();
	}
}
