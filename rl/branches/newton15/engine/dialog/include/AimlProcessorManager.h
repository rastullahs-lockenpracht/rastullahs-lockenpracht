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

#ifndef __Rl_DialogProcessorManager_H__
#define __Rl_DialogProcessorManager_H__

//--	AimlProcessorManager.h
#include "AimlProcessor.h"
#include "DialogPrerequisites.h"


#include <map>
//#include <string>

using namespace std;

namespace rl
{
	class _RlDialogExport AimlProcessorManager //: public Singleton<AimlProcessorManager>
	{
	public:
		AimlProcessorManager(){}
		virtual ~AimlProcessorManager();
		static void init();
		static void addProcessor(const CeGuiString &name,AimlProcessor* proc);
		static void addStandardProcessors();
		static AimlProcessor* getProcessor(const CeGuiString &name);
		static bool hasProcessor(const CeGuiString &name);
		
		static map<CeGuiString, AimlProcessor *> mProcessors;
		static map<CeGuiString, Graphmaster*> mGraphMasters;
	};
}
#endif
