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

#include <string>

#include "XmlHelper.h"
#include "CoreSubsystem.h"
#include "Interpreter.h"

#include "Creature.h"

#include "Utils.h"
#include "DialogSubsystem.h"
#include "DialogCharacter.h"
#include "DialogScriptObject.h"

#include "processors/ConditionProcessor.h"

using namespace std;

namespace rl
{
	CeGuiString ConditionProcessor::process(DOMNode* node,Match* m, const CeGuiString& str, NaturalLanguageProcessor* nlp)
	{		
		CeGuiString buffer;
		CeGuiString typeInfo;

		//  get attributes
		CeGuiString conditionContext = XmlHelper::getAttributeValueAsString(
				static_cast<DOMElement*>(node), "context" );

		CeGuiString conditionType = XmlHelper::getAttributeValueAsString( 
				static_cast<DOMElement*>(node), "type" );
		
		CeGuiString conditionName = XmlHelper::getAttributeValueAsString( 
				static_cast<DOMElement*>(node), "name" );

		int rVal = 0; 
		int tmpVal = 0;//XmlHelper::getAttributeValueAsInteger( (DOMElement*)node,XMLString::transcode("value") );
		int modifier = 0;

		if(static_cast<DOMElement*>(node)->hasAttribute((XMLCh*)"mod"))
		{
			modifier = 
				XmlHelper::getAttributeValueAsInteger( 
										static_cast<DOMElement*>(node), "mod" );
		}

		DialogScriptObject* scriptObject = 
			(static_cast<DialogCharacter*>(nlp))->getScriptObject();

		if(conditionType == "Talent")
		{
			typeInfo = " (T) ";
			rVal = scriptObject->getCharacter()->doTalentprobe(conditionName, modifier);
		}
		else if (conditionType == "Eigenschaft")
		{
			typeInfo = " (E) ";
			rVal = scriptObject->getCharacter()->doEigenschaftsprobe(conditionName, modifier);
		}
		else if(conditionType == "Basiswert")
		{
		}
		else
		{
			if(conditionContext == "response")
			{
				rVal = scriptObject->calcResponseValue(conditionName);
			}
			else if(conditionContext == "option")
			{
				rVal = scriptObject->calcOptionValue(conditionName);
			}
		}
		bool conditionChild = false;

		if(XmlHelper::transcodeToString(
			node->getParentNode()->getNodeName()).compare("li") == 0)
		{
			conditionChild = true;
			buffer += typeInfo;
			buffer += "<option type=\"" + conditionType + "\" ";
			buffer += "name=\"" + conditionName + "\" ";
			buffer += ">";
		}
		//  Search through all li-elements
		for (DOMNode* childNode = node->getFirstChild(); childNode != NULL; childNode = childNode->getNextSibling() )
		{	
			if ( childNode->getNodeType() == DOMNode::ELEMENT_NODE ) 
			{
				CeGuiString nodeName = XmlHelper::transcodeToString(
											childNode->getNodeName());
				if(nodeName.compare("li") == 0)
				{
					tmpVal = XmlHelper::getAttributeValueAsInteger( 
						static_cast<DOMElement*>(childNode), "value" );
					std::string sValue = XmlHelper::getAttributeValueAsStdString(
						static_cast<DOMElement*>(childNode), "value" );
					std::string id = XmlHelper::getAttributeValueAsStdString( 
						static_cast<DOMElement*>(childNode), "id" );

					bool conditionFulfilled = false;
			
					//  Add elements in <li> Tag only if <li>-value = return value of the named function
					if(conditionContext == "selection")
					{
						conditionFulfilled = true;
					}
					else if ( (conditionType == "Talent" 
						|| conditionType == "Eigenschaft")
						)
					{
						if( (rVal > 0 && tmpVal > 0)
							|| (rVal <= 0 && tmpVal == 0))
						{
							conditionFulfilled = true;
						}
					}
					else
					{
						conditionFulfilled = (tmpVal == rVal);
					}

					
					if(conditionFulfilled)
					{
						CeGuiString temp = nlp->process(childNode, m, str);
						if(conditionContext.empty())
						{
							// used for postprocessing of the option-tag when
							// checking for skills(talents)
							
							buffer += id + " " + temp;
						}
						else if(conditionContext == "selection")
						{
							buffer += "<li id=\"" + id + "\" ";
							buffer += "value=\"" + sValue + "\" ";
							buffer += ">";
							buffer += temp;
							buffer += "</li>";
						}
						else
						{
							buffer += "<li id=\"" + id + "\" ";
							buffer += "value=\"" + sValue + "\" ";
							buffer += "/>";
							buffer += temp;
						}
					} // end if conditionFulfilled						
				} // end compare nodeName
			} // end compare nodeType
			else if( childNode->getNodeType() == DOMNode::TEXT_NODE ) 
			{
				CeGuiString text = XmlHelper::transcodeToString(
						static_cast<DOMText*>(childNode)->getData());
				
				if( !normalise(text).empty())
				{
					buffer += nlp->process(node, m, str);
					return buffer;
				}
			}
		} // end for loop
		if(conditionChild)
		{
			buffer += "</option>";
		}
		return buffer;
	}
}// Namespace rl end
