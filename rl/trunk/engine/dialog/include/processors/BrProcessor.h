/**
 * BrProcessor - Breaks output into multiple lines
 *
 * @author	Jonathan Roewen
 */
#ifndef BR_PROCESSOR_H
#define BR_PROCESSOR_H

#include "DialogPrerequisites.h"
#include "../AimlProcessor.h"
#include "../NaturalLanguageProcessor.h"

#include <string>
#include "OgreLogManager.h"
using namespace std;
namespace rl
{
	class BrProcessor : public AimlProcessor
	{
	public:
		~BrProcessor() { }
	
		inline string process(DOMNode*,Match *, const char *, NaturalLanguageProcessor*) 
		{
			return "\n";
		}
	};
}
#endif

