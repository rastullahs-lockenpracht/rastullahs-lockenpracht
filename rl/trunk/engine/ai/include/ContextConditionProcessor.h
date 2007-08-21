/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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
#ifndef __Rl_ContextConditionProcessor_H__
#define __Rl_ContextConditionProcessor_H__

#include "XmlMapper/XmlNodeProcessor.h"
#include "AiPrerequisites.h"
#include "AimlBot.h"

using namespace XmlMapper;
using namespace MadaBot;

namespace rl
{
    /**
     * Special ConditionProcessor for processing conditions that are a ChildNode
     * of a specific gossip context
     */
	class ContextConditionProcessor 
		: public XmlNodeProcessor<Response, AimlBot, CeGuiString, false>
	{
	public:
		ContextConditionProcessor(void); 
		virtual ~ContextConditionProcessor(void);
		
        /**
         * Get the needed Predicates of an AimlBot for preparing the processing of a 
         * condition
         */
		virtual void preprocessStep();

        /**
         * Evaluates every ChildNode of the condition if it is a ListItem (<li>)
         */
		virtual void processChildStep(XmlNode<CeGuiString>* pChild);

        /**
         * Not used in this processor
         */
		virtual void postprocessStep();
	protected:
		virtual void initialize();
		CeGuiString mPredicateValue;
		CeGuiString mNodeValue;
	};
}
#endif
