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
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/dom/DOM.hpp>

#include <CEGUIPropertyHelper.h>

#include "XmlHelper.h"
#include "AimlProcessorManager.h"
#include "AimlProcessor.h"
#include "DialogResponse.h"

namespace rl
{
	DialogResponse::DialogResponse( const CeGuiString& input, 
									const CeGuiString& response,
									const Options& currentOptions,
									const Options& selectableOptions,
									NaturalLanguageProcessor* nlp)
		:	mInput(input), 
			mResponse(response), 
			mCurrentOptions(currentOptions),
			mSelectableOptions(selectableOptions), 
			mNlp(nlp)
	{
	}

	DialogResponse::~DialogResponse(void)
	{
	}

	std::pair<int, CeGuiString> DialogResponse::getSelectedOption(int id)
	{
		XERCES_CPP_NAMESPACE_USE

		if(mSelectableOptions.find(id) != mSelectableOptions.end())
		{
			XercesDOMParser* parser = new XercesDOMParser();
		// copy the response data into a memory buffer for postprocessing
			MemBufInputSource memBuff((const XMLByte*)mSelectableOptions[id].data(),
									mSelectableOptions[id].size(),"response",false);
			parser->parse(memBuff);
		//  get doc from parser and get first node
			DOMDocument* doc = parser->getDocument();
			DOMNode* node = doc->getDocumentElement();

			CeGuiString result;
			AimlProcessor* pt = AimlProcessorManager::getProcessor("condition");
			if(pt)
			{
				result = pt->process(node, NULL, "0", mNlp);
			}
			if(parser) delete parser;
			int endPos = result.find_first_of(" ");
			
			int rVal = CEGUI::PropertyHelper::stringToInt(result.substr(0, endPos));
			result = result.substr(endPos + 1) + "\n";
			
			return std::pair<int, CeGuiString>(rVal,result);

		}
		else
		{
			return std::pair<int, CeGuiString>(id, mCurrentOptions[id]);
		}
	}
}
