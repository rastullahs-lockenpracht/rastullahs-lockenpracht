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
#include "ContextConditionProcessor.h"
#include "Creature.h"

using namespace rl;

ContextConditionProcessor::ContextConditionProcessor(void)
	: XmlNodeProcessor<Response, AimlBot, CeGuiString, false>("contextCondition")
{
	initialize();
}

ContextConditionProcessor::~ContextConditionProcessor(void)
{
}

void ContextConditionProcessor::initialize()
{
	addAllowedSubProcessor("li");
	addAttribute("mod");
	addAttribute("type");
	addAttribute("name");
	addAttribute("value");
}

void ContextConditionProcessor::preprocessStep()
{
	mPredicateValue.clear();
	mNodeValue.clear();
	mCurrentReturnValue.clear();
	try
	{
		LOG_MESSAGE(Logger::DIALOG, "ContextCondition: getPredciates");
		if(mAttributes["type"].find("probe") != CeGuiString::npos)
		{
			mCurrentHelper->getPredicates(mAttributes["type"])->setPredicate("mod", mAttributes["mod"]);
		}
		mPredicateValue = mCurrentHelper
				->getPredicates(mAttributes["type"])
				->getPredicate(mAttributes["name"]);

		if(!mAttributes["value"].empty())
		{
			mNodeValue = mAttributes["value"];
		}
	}
	catch(...)
	{
	}
}

void ContextConditionProcessor::processChildStep(XmlNode<CeGuiString>* pChild)
{
	mNodeValue.c_str();
	mPredicateValue.c_str();
	CeGuiString value = mNodeValue;
	if(value.empty())
	{
		value = pChild->getAttribute("value");
	}
	value.c_str();
	CeGuiString logMessage = "evaluated <li> with id " + pChild->getAttribute("id") + ". result: ";
	CeGuiString result = "false";
	if(mPredicateValue == value)
	{
		mCurrentReturnValue += pChild;
		result = "true";
	}
	LOG_MESSAGE(Logger::DIALOG, logMessage + result);
}

void ContextConditionProcessor::postprocessStep()
{
}
