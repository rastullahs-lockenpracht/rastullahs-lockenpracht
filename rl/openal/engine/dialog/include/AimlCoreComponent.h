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

    /**
	 * This class loads and manages AIML files
	 * The AIML files are parsed with an AimlParser, the relevant Nodes get stored
	 * in a Graph. Every AIML file has its own node 
	 */
	class _RlDialogExport AimlCoreComponent
	{
	public:
		/// Constructors and Destructors
		AimlCoreComponent(void);
		virtual ~AimlCoreComponent(void);

		/**
		 * Getter for a GraphMaster with the given name
		 * @param name name of the GraphMaster, equivalent to the name of an AIML file
		 * @return The Graphmaster with the given name, if it exits (else NULL)
		 */
		virtual Graphmaster* getGraphMaster(const std::string& name);
		
	   /**
	    * Loads an AIML file with the given name
		* An AimlParser must be set
		* @param fileName Filename of the AIML file 
		* @return true if loading succeded
		* @see setAimlParser(AimlParser* parser)
		*/
		virtual bool loadAimlFile(const std::string& fileName);

		/**
		 * Sets the AimlParser necessary for parsing an AIML file
		 * @param Pointer to the AimlParser
		 */
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
