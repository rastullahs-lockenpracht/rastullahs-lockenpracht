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

#include <CEGUIPropertyHelper.h>

#include "processors/SetProcessor.h"
#include "RulesSubsystem.h"
#include "QuestBook.h"
#include "Quest.h"

namespace rl
{
	SetProcessor::SetProcessor(void)
	{
	}

	SetProcessor::~SetProcessor(void)
	{
	}

	CeGuiString SetProcessor::process(DOMNode* node,Match* match, const CeGuiString& str, NaturalLanguageProcessor* nlp)
	{
		CeGuiString propertyType = XmlHelper::getAttributeValueAsString( 
				static_cast<DOMElement*>(node), "type" );
		
		CeGuiString propertyName = XmlHelper::getAttributeValueAsString( 
				static_cast<DOMElement*>(node), "name" );

		CeGuiString propertyValue;
		
		DOMNode* childNode = node->getFirstChild();
		if( childNode->getNodeType() == DOMNode::TEXT_NODE ) 
		{
			propertyValue = XmlHelper::transcodeToString(
						static_cast<DOMText*>(childNode)->getData());
		}

		if(propertyType == "quest")
		{
			Quest* quest = RulesSubsystem::getSingletonPtr()->getQuestBook()
				->getQuest(propertyName);
			if(quest != NULL)
			{
				Quest::State state = static_cast<Quest::State>(
					CEGUI::PropertyHelper::stringToInt(propertyValue));
				quest->setState(state);
			}
		}
		else
		{
			nlp->getPredicates().setPredicate(propertyName, "default", propertyValue);
		}

		return "";
	}
}
