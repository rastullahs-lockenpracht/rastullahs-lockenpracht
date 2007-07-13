/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2006 Team Pantheon. http://www.team-pantheon.de
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */
#include "stdinc.h" //precompiled header

#include "ContextInterpreter.h"
#include "ContextConditionProcessor.h"
#include "ScriptProcessor.h"
#include "DialogResponse.h"
#include "DialogOption.h"
#include "AimlBot.h"

using namespace std;

namespace rl {

CeGuiString ContextInterpreter::INTERPRETER_NAME = "contextInterpreter";
CeGuiString ContextInterpreter::RL_TYPE = "rldialog:type";
CeGuiString ContextInterpreter::RL_LI_ID = "rldialog:id";
CeGuiString ContextInterpreter::RL_PATTERN_REF = "rldialog:patternRef";
CeGuiString ContextInterpreter::RL_E_PROBE = "eigenschaftsprobe";
CeGuiString ContextInterpreter::RL_E_MODIFIER = " (E) ";
CeGuiString ContextInterpreter::RL_T_PROBE = "talentprobe";
CeGuiString ContextInterpreter::RL_T_MODIFIER = " (T) ";
CeGuiString ContextInterpreter::RL_RESPONSE = "response";
CeGuiString ContextInterpreter::RL_OPTION = "option";
CeGuiString ContextInterpreter::RL_SELECTION = "selection";
CeGuiString ContextInterpreter::AIML_CONDITION = "condition";
CeGuiString ContextInterpreter::AIML_TYPE = "type";
CeGuiString ContextInterpreter::AIML_GOSSIP = "gossip";
CeGuiString ContextInterpreter::AIML_LI = "li";

ContextInterpreter::ContextInterpreter(void)
	: AimlInterpreter<CeGuiString>(INTERPRETER_NAME), mBot(NULL)
{
	initialize();
}

ContextInterpreter::~ContextInterpreter(void)
{
}

void ContextInterpreter::processResponse()
{
//  now we have one response with several <li>s. those <li>s should be processed
	Response<CeGuiString>::GossipData gossip = mReturnValue.getGossip();
	Response<CeGuiString>::GossipData::const_iterator itr = gossip.begin();
	for(; itr != gossip.end(); ++itr)
	{
		if((*itr)->getNodeName() == AIML_LI)
		{
			CeGuiString id = (*itr)->getAttribute(RL_LI_ID);
			id.c_str();
			CeGuiString text = getProcessor(AIML_LI)->process(*itr, mBot).getResponse();
			text.c_str();
			mResponses.push_back(make_pair<CeGuiString, CeGuiString>(id, text));
		}
	}
}

void ContextInterpreter::processOption()
{
	Response<CeGuiString>::GossipData gossip = mReturnValue.getGossip();
	Response<CeGuiString>::GossipData::const_iterator itr = gossip.begin();
	for(; itr != gossip.end(); ++itr)
	{
		if((*itr)->getNodeName() == AIML_LI)
		{
			Response<CeGuiString> response = getProcessor(AIML_LI)->process(*itr, mBot);
		//  check if this option is influenced by a condition check
			XmlNode<CeGuiString>* pChild = (*itr)->getFirstChild();
			for(; pChild != NULL; pChild = pChild->getNextSibling())
			{
				if(pChild->getNodeName() == AIML_CONDITION)
				{
					CeGuiString type = pChild->getAttribute(AIML_TYPE);
					if(type == RL_E_PROBE)
					{
						response += RL_E_MODIFIER;
					}
					else if(type == RL_T_PROBE)
					{
						response += RL_T_MODIFIER;
					}
				}
			}
			DialogOption* option = new DialogOption(response, mBot);
			option->setId((*itr)->getAttribute(RL_LI_ID));
			option->setPattern((*itr)->getAttribute(RL_PATTERN_REF));
			mOptions.push_back(option);
		}
		else
		{
			// exception or logging...
		}
	}
}

void ContextInterpreter::processSelection()
{
	Response<CeGuiString>::GossipData gossip = mReturnValue.getGossip();
	Response<CeGuiString>::GossipData::const_iterator itr = gossip.begin();
	for(; itr != gossip.end(); ++itr)
	{
		if((*itr)->getNodeName() == AIML_LI)
		{
			DialogOption* option = new DialogOption(getProcessor(AIML_LI)->process(*itr));
			option->setId((*itr)->getAttribute(RL_LI_ID));
			option->setPattern((*itr)->getAttribute(RL_PATTERN_REF));
			mOptions.push_back(option);
		}
	}
}

DialogResponse* ContextInterpreter::interpret(const Response<CeGuiString>::GossipData& pData, AimlBot<CeGuiString>* pProcessHelper)
{
	mOptions.clear();
	mResponses.clear();
	mBot = pProcessHelper;
	CeGuiString type;
	Response<CeGuiString>::GossipData::const_iterator itr = pData.begin();
	for(; itr != pData.end(); ++itr)
	{
		mCurrentType = (*itr)->getAttribute(RL_TYPE);
		mCurrentType.c_str();
		pProcessHelper->getPredicates("default")->setPredicate("contextType", mCurrentType);
		process( (*itr), pProcessHelper);
	}
	pProcessHelper->getPredicates("default")->setPredicate("contextType", "");
	return new DialogResponse(mResponses, mOptions, pProcessHelper);
}

Response<CeGuiString> ContextInterpreter
			::process(XmlNode<CeGuiString>* pNode, AimlBot<CeGuiString>* pProcessHelper)
{
//  pNode == gossip
	Response<CeGuiString> response;
	XmlNode<CeGuiString>* child = pNode->getFirstChild();
	for(; child != NULL; child = child->getNextSibling(), mReturnValue.clear())
	{
		if(child->getNodeName() == AIML_LI)
		{
		//	add the li-node to the responses gossip data
			mReturnValue += child;
		}
		else if(child->getNodeName() == AIML_CONDITION)
		{
		//	add a list of valid li-nodes to the gossip data of the repsonse
			LOG_MESSAGE(Logger::DIALOG, "process condition in context" + mCurrentType);
			mReturnValue += getProcessor("contextCondition")
					->process(child, pProcessHelper);
		}
	//	process the returnValue for the specific context
		if(mCurrentType == RL_RESPONSE && pNode->getNodeName() == AIML_GOSSIP)
		{
			LOG_MESSAGE(Logger::DIALOG, "process response");
			processResponse();
		}
		else if(mCurrentType == RL_OPTION && pNode->getNodeName() == AIML_GOSSIP)
		{
			LOG_MESSAGE(Logger::DIALOG, "process option");
			processOption();
		}
		else if(mCurrentType == RL_SELECTION && pNode->getNodeName() == AIML_GOSSIP)
		{
			LOG_MESSAGE(Logger::DIALOG, "process selection");
			processSelection();
		}
	}
	return response;
}

void ContextInterpreter::initialize()
{
	addProcessor(new ContextConditionProcessor());
	addProcessor(new ScriptProcessor());
}

}
