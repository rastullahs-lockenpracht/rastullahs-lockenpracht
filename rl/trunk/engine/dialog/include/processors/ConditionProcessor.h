#ifndef CONDITION_PROCESSOR_H
#define CONDITION_PROCESSOR_H

#include "DialogPrerequisites.h"
#include "../AimlProcessor.h"
#include "../NaturalLanguageProcessor.h"

#include <string>
using namespace std;

namespace rl
{
	class ConditionProcessor : public AimlProcessor
	{
	public:
		~ConditionProcessor() { }
	
		string process(DOMNode* node,Match* m, const char *str, NaturalLanguageProcessor* nlp);
	};
}
#endif
