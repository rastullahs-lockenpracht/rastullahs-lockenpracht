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
#include "ContextInterpreter.h"
#include "DialogResponse.h"
#include "DialogOption.h"

using namespace rl;
using namespace std;

ContextInterpreter::ContextInterpreter(void)
	: AimlInterpreter("contextInterpreter")
{
}

ContextInterpreter::~ContextInterpreter(void)
{
}

void ContextInterpreter::processResponse()
{
	// now we have one response with several <li>s. those <li>s should be processed
	Response<CeGuiString>::GossipData gossip = mReturnValue.getGossip();
	Response<CeGuiString>::GossipData::const_iterator itr = gossip.begin();
	for(; itr != gossip.end(); ++itr)
	{
		CeGuiString id = (*itr)->getAttribute("id");
		CeGuiString text = process(*itr).getResponse();
		mResponses.push_back(make_pair<CeGuiString, CeGuiString>(id, text));
	}
}

void ContextInterpreter::processOption()
{
	Response<CeGuiString>::GossipData gossip = mReturnValue.getGossip();
	Response<CeGuiString>::GossipData::const_iterator itr = gossip.begin();
	for(; itr != gossip.end(); ++itr)
	{
		if((*itr)->getNodeName() == "li")
		{
			DialogOption* option = new DialogOption();
			option->setId((*itr)->getAttribute("id"));
			option->setPattern((*itr)->getAttribute("patternRef"));
			option->setData(getProcessor("li")->process(*itr));
			mOptions.push_back(option);
		}
		else
		{
			// exception or logging...
		}
	}
}

DialogResponse* ContextInterpreter::interpret(const Response<CeGuiString>::GossipData& pData, AimlBot<CeGuiString>* pProcessHelper)
{
//  we don't use mReturnValue aus return value in this method!
	mReturnValue.clear();
//	Response dialogOption;
	CeGuiString type;
	Response<CeGuiString>::GossipData::const_iterator itr = pData.begin();
	for(; itr != pData.end(); ++itr)
	{
		mCurrentType = (*itr)->getAttribute("rl:type");
		process( (*itr), pProcessHelper);
	}
	return new DialogResponse(mResponses, mOptions, pProcessHelper);
}

Response<CeGuiString> ContextInterpreter
			::process(XmlNode<CeGuiString>* pNode, AimlBot<CeGuiString>* pProcessHelper)
{
//  pNode == gossip
	XmlNode<CeGuiString>* child = pNode->getFirstChild();
	for(; child != NULL; child = child->getNextSibling())
	{
		if(child->getNodeName() == "li")
		{
			// add the li-node to the responses gossip data
			mReturnValue += child;
		}
		else if(pNode->getNodeName() == "condition")
		{
		// add a list of valid li-nodes to the gossip data of the repsonse
		/*  TODO: implement conditionprocessor
			mReturnValue += getProcessor("gossipCondition")
					->process(pNode, NULL);
		*/
		}
		// maybe allow some other processors, too
		if(mCurrentType == "response")
		{
			processResponse();
		}
		else if(mCurrentType == "option")
		{
			processOption();
		}
	}
	return Response<CeGuiString>();
}

void ContextInterpreter::initialize()
{
	AimlInterpreter::initialize();
	//addProcessor(new ConditionContextProcessor());
}