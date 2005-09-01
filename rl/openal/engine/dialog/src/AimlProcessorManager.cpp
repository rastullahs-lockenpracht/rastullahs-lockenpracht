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

#include "AimlProcessorManager.h"
#include "processors/BrProcessor.h"
#include "processors/ConditionProcessor.h"
#include "processors/SystemProcessor.h"
#include "processors/SraiProcessor.h"

namespace rl
{
	map<string, AimlProcessor *> AimlProcessorManager::mProcessors;

	AimlProcessorManager::~AimlProcessorManager()
	{
		map<string,AimlProcessor*>::const_iterator itr = mProcessors.begin();
		for(itr++;itr!=mProcessors.end();itr++)
		{
			delete itr->second;
		}
		mProcessors.clear();
	}

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

	void AimlProcessorManager::addProcessor(const string &name,AimlProcessor* proc)
	{
		mProcessors[name]=proc;
	}

	void AimlProcessorManager::addStandardProcessors()
	{
		ConditionProcessor* cp=new ConditionProcessor();
		mProcessors["br"]=new BrProcessor();
		mProcessors["selection"]=cp;
		mProcessors["condition"]=cp;
		mProcessors["if"]=cp;
		mProcessors["srai"]=new SraiProcessor();
		mProcessors["system"]=new SystemProcessor();
	}

	AimlProcessor* AimlProcessorManager::getProcessor(const string &name)
	{
		map<string, AimlProcessor *>::const_iterator itr=mProcessors.find(name);
		if(itr!= mProcessors.end())
			return itr->second;
		else return NULL;
	}

	bool AimlProcessorManager::hasProcessor(const string &name) {
		return mProcessors.find(name) != mProcessors.end();
	}
}
