/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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
#ifndef __Rl_AimlCoreComponent_H__
#define __Rl_AimlCoreComponent_H__

#include <map> 

#include "DialogPrerequisites.h"

namespace rl
{
	class AimlParser;
	class Graphmaster;

	class _RlDialogExport AimlCoreComponent
	{
	public:
		AimlCoreComponent(void);
		virtual ~AimlCoreComponent(void);

		virtual bool loadAimlFile(const std::string& fileName);

		void setAimlParser(AimlParser* parser);
		
	private:
		typedef std::map<std::string,Graphmaster*> GraphMasterMap;
		GraphMasterMap mGraphMasters;

		AimlParser* mParser;
	};

	inline void AimlCoreComponent::setAimlParser(AimlParser* parser)
	{
		mParser = parser;
	}
}
#endif
