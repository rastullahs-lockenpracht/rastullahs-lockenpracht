#include "AimlParser.h"
#include "processors/ConditionProcessor.h"
#include "Utils.h"
#include "ScriptObject.h"

#include <string>

using namespace std;

namespace rl
{
	string ConditionProcessor::process(DOMNode* node,Match* m, const char* str, NaturalLanguageProcessor* nlp)
	{		
		string buffer="";
		DOMNamedNodeMap* attrbs=node->getAttributes();
		DOMNode* scriptNode=attrbs->getNamedItem(XMLString::transcode("script"));
		DOMNode* classNode=attrbs->getNamedItem(XMLString::transcode("class"));
		DOMNode* nameNode=attrbs->getNamedItem(XMLString::transcode("name"));

		if(nameNode==NULL)	// If function name is missing return
		{
			return "";
		}
		// Look if condition has a value (standard: no value)
		DOMNode* valueNode=attrbs->getNamedItem(XMLString::transcode("value"));

		if(scriptNode!=NULL && classNode!=NULL)
		{
			string scriptStr=AimlParser::transcodeXmlCharToString(scriptNode->getNodeValue());
			string classStr =AimlParser::transcodeXmlCharToString(classNode->getNodeValue());

			ScriptObject *so=new ScriptObject("conditionTest");
			Ogre::String bla[]={"Popel"};
			so->setScript(scriptStr,classStr,1,bla);
			so->callFunction("OnPlaySound",0,0);

			int iReturn=so->callIntegerFunction("TestReturner",0,0);
			scriptStr+=classStr;
			if(iReturn==4){scriptStr="Hatgefunzt";}

			for (DOMNode* childNode=node->getFirstChild(); childNode != node->getLastChild(); childNode = childNode->getNextSibling() )
			{	// Search through all li-elements
				if ( childNode->getNodeType() == DOMNode::ELEMENT_NODE ) 
				{
					string nodeName=AimlParser::transcodeXmlCharToString(childNode->getNodeName());
					if(!nodeName.compare("li"))
					{
						DOMNamedNodeMap* attrbs=childNode->getAttributes();
						int value=AimlParser::getAttributeValueAsInteger(attrbs,"value");
						string idString= AimlParser::transcodeXmlCharToString(attrbs->getNamedItem(XMLString::transcode("id"))->getNodeValue());
						if(value==iReturn)
						{
							buffer+="<li id=\""+idString+"\" />";
							buffer+=nlp->process(childNode,m,str);
						}
					}
				}
			}	
		}
		return buffer;
	}
}// Namespace rl end
