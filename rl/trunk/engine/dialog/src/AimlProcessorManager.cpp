#include "AimlProcessorManager.h"
#include "processors/BrProcessor.h"
#include "processors/ConditionProcessor.h"
#include "processors/SystemProcessor.h"

namespace rl
{
	map<string, AimlProcessor *> AimlProcessorManager::mProcessors;
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
		mProcessors["br"]=new BrProcessor();
		mProcessors["condition"]=new ConditionProcessor();
		mProcessors["system"]=new SystemProcessor();
	}

	AimlProcessor* AimlProcessorManager::getProcessor(const string &name)
	{
		map<string, AimlProcessor *>::const_iterator itr=mProcessors.find(name);
		if(itr != mProcessors.end())
			return itr->second;
		else return NULL;
	}

	bool AimlProcessorManager::hasProcessor(const string &name) {
		return mProcessors.find(name) != mProcessors.end();
	}
}
