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
#ifndef __RL_NLP_H__
#define __RL_NLP_H__

#include <xercesc/dom/DOM.hpp>
#include "DialogPrerequisites.h"

#include "Graphmaster.h"

//#include "Substituter.h"
//#include "Predicates.h"

#include <string>


XERCES_CPP_NAMESPACE_USE

namespace rl
{
	using XERCES_CPP_NAMESPACE::DOMDocument; //XXX: Warum brauche ich das unter VS 2003?

	/** Main class for starting a dialog with a NPC
	 *  @author Philipp Walser
	*/
	class _RlDialogExport NaturalLanguageProcessor
	{
	public:
		NaturalLanguageProcessor(std::string dialogFile);
		~NaturalLanguageProcessor();

		Graphmaster *getGM() const { return mGm; }
		bool loadAiml(const std::string &filename);
		map<int,std::string> respond(const std::string &input);
		string process(DOMNode* node, Match *match, const string &id);
		void processOption(const std::string &name, const std::string &value);
		bool mExit;
	private:
		Graphmaster *mGm;
		std::map<int,std::string> mResponses;
		

		//string buffer;
		//Predicates *pr;
		//Substituter *sb;
	};
}


/*		//Not yet implemented and maybe never will be...
		Predicates *getPr() const { return pr; }
		Substituter *getSb() const { return sb; }
	
		#if 0
		inline string getHistory(const string &property, const string &id, int history, int sentence) {
			return pr->getHistory(property, id, history);
		}
		#else
		string getHistory(const string &property, const string &id, int history, int sentence);
		#endif
		inline string setValue(const string &property, const string &id, const string &value, int index = 1) {
			return Predicates::setValue(property, id, value, index);
		}
		inline void addValue(const string &property, const string &id, const string &value) {
			Predicates::addValue(property, id, value);
		}
		inline string getValue(const string &property, const string &id, int index = 1) {
			return Predicates::getValue(property, id, index);
		}
		inline string getValues(const string &property, const string &id, const string &conjunction = " and ") {
			return Predicates::getValues(property, id, conjunction);
		}
		inline string getProperty(const string &property) {
			return Predicates::getProperty(property);
		}
		inline string substitute(const string &input, const string &type) {
			return sb->substitute(input, type);
		}
		inline void loadFiles(const string &files) {
			AimlParser::loadFileExpr(files, this);
		}
		inline string lookupPredicate(const string &property, const string &defaultValue) {
			return Predicates::lookupPredicate(property, defaultValue);
		}
		string result_buffer;
*/	
#endif
