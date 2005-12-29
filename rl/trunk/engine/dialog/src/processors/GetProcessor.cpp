#include "processors/GetProcessor.h"
#include "RulesSubsystem.h"
#include "Questbook.h"
#include "Quest.h"

namespace rl
{
	GetProcessor::GetProcessor(void)
	{
	}

	GetProcessor::~GetProcessor(void)
	{
	}

	CeGuiString GetProcessor::process(DOMNode* node,Match* match, const CeGuiString& str, NaturalLanguageProcessor* nlp)
	{
		CeGuiString propertyType = XmlHelper::getAttributeValueAsString( 
				static_cast<DOMElement*>(node), "type" );
		
		CeGuiString propertyName = XmlHelper::getAttributeValueAsString( 
				static_cast<DOMElement*>(node), "name" );

		CeGuiString rVal;

		if(propertyType == "quest")
		{
			Quest* quest = RulesSubsystem::getSingletonPtr()->getQuestBook()
				->getQuest(propertyName);
			if(quest != NULL)
			{
				rVal = quest->getState();
			}
		}
		else
		{
			rVal = nlp->getPredicates().getPredicate(propertyName, "default");
		}

		return rVal;
	}
}
