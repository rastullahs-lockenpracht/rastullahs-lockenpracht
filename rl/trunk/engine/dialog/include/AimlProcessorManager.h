#ifndef __Rl_DialogProcessorManager_H__
#define __Rl_DialogProcessorManager_H__

//--	AimlProcessorManager.h
#include "AimlProcessor.h"
#include "DialogPrerequisites.h"


#include <map>
#include <string>

using namespace std;

namespace rl
{
	class _RlDialogExport AimlProcessorManager //: public Singleton<AimlProcessorManager>
	{
	public:
		static void init();
		static void addProcessor(const string &name,AimlProcessor* proc);
		static void addStandardProcessors();
		static AimlProcessor* getProcessor(const string &name);
		static bool hasProcessor(const string &name);
		
		static map<string, AimlProcessor *> mProcessors;
	};
}
#endif
