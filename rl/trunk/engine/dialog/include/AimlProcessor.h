#ifndef __Rl_DialogAimlProcessor_H__
#define __Rl_DialogAimlProcessor_H__
#include <xercesc/dom/DOM.hpp>
#include "DialogPrerequisites.h"
#include <string>

#include "NaturalLanguageProcessor.h"
#include "Match.h"
using namespace std;
XERCES_CPP_NAMESPACE_USE
namespace rl
{
	class _RlDialogExport AimlProcessor
	{
	public:
		virtual ~AimlProcessor() { };
		virtual string process(DOMNode *, Match *, const char *, NaturalLanguageProcessor *)=0;
	};
}
#endif
