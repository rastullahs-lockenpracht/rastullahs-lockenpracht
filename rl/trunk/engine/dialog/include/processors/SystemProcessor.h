#ifndef SYSTEM_PROCESSOR_H
#define SYSTEM_PROCESSOR_H

#include "DialogPrerequisites.h"
#include "../AimlProcessor.h"
#include "../NaturalLanguageProcessor.h"
#include "../AimlParser.h"

#include <string>
using namespace std;

namespace rl
{
	class SystemProcessor : public AimlProcessor
	{
	public:
		SystemProcessor(){};
		~SystemProcessor() { }
	
		string process(DOMNode* node,Match* m, const char *str, NaturalLanguageProcessor* nlp)
		{
				DOMNamedNodeMap* attrbs=node->getAttributes();
				DOMNode* commandNode=attrbs->getNamedItem(XMLString::transcode("command"));	
				string commandStr=AimlParser::transcodeXmlCharToString(commandNode->getNodeValue());
				if(!commandStr.compare("exit"))
					nlp->mExit=true;
				return "";
		};
	};
}
#endif