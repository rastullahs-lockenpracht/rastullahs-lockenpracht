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

#include "Nodemaster.h"
#include "Match.h"
#include "StringTokenizer.h"
#include "Utils.h"
#include "OgreLogManager.h"


#include "Graphmaster.h"
#include "DialogSubsystem.h"

//--	Graphmaster.cpp



#include <iostream>

using namespace std;
namespace rl
{
	Graphmaster::Graphmaster() 
		: mRoot(new Nodemaster())
	{
	}

	Graphmaster::~Graphmaster() 
	{
//		DialogSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, "Delete all graphmaster nodes");
		mRoot->deleteAllNodes();
//		DialogSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, "Delete root");
		delete mRoot;
	}

	const static CeGuiString GM_PATTERN = " <pattern> ";
	const static CeGuiString GM_THAT = " <that> ";
	const static CeGuiString GM_TOPIC = " <topic> ";
	const static CeGuiString ASTERISK = "*";
	const static CeGuiString UNDERSCORE = "_";

	Nodemaster* Graphmaster::add(	const CeGuiString& context, 
									const CeGuiString& pattern, 
									const CeGuiString& that, 
									const CeGuiString& topic,
									const CeGuiString& templateValue)
	{
		CeGuiString path = (context + GM_PATTERN + pattern + GM_THAT + that + GM_TOPIC + topic); // toUpper
		Nodemaster *node = mRoot, *child = NULL;
		char* c_string = (char*)path.c_str();
		//path
		//--	this code needs to be updated to use the StringTokenizer (this ain't robust)
		
		for ( char *end = strchr(c_string, ' '); c_string != NULL; )
		{
			if ( end != NULL )
				*end = '\0';
		
			CeGuiString token(c_string);
			token.data();
			child = node->getChild(token.c_str());

			if ( child == NULL ) 
			{
				child = new Nodemaster(templateValue.c_str());			// eigentlich müsste man direkt hier schon
													// das templateValue übergeben können
				node->addChild(token.c_str(), child);
			}
			node = child;
		
			if ( end != NULL ) 
			{
				c_string = end + 1;
				end = strchr(c_string, ' ');
			} else
				break;
		}
		return node;
	}

	Match* Graphmaster::match (	const CeGuiString& context, const CeGuiString& pattern, 
								const CeGuiString& that, const CeGuiString& topic) 
	{
		CeGuiString path = context + GM_PATTERN + pattern + GM_THAT + that + GM_TOPIC + topic;
		return match(mRoot, mRoot, Context, path, "", "");
	}

	Match* Graphmaster::match (	Nodemaster *node, Nodemaster *parent, component which, 
								const CeGuiString& input, const CeGuiString& star, 
								const CeGuiString& path) 
	{
		std::string tmpInput = input.c_str();
		StringTokenizer st(tmpInput, " ");
		
		//cerr << "PATH:  " << path << endl << "STAR:  " << star << endl << "INPUT: " << input << endl << endl;
		
		if ( !st.hasMoreTokens() ) {
			if ( node->getTemplateNode() == NULL ) 
			{
				cout << "-- no template" << endl << endl;
				return NULL;
			}
			Match* match = new Match();
			match->setNode(node);
			match->addStar(star.c_str(), which);
			match->setPattern(path.c_str(), which);
			return match;
		}
		
		string word = st.nextToken();
		
		//cerr << "WORD:  " << word << endl;
		string tail = "";
		Match* m = NULL;
		Nodemaster* n = NULL;
		
		if ( st.hasMoreTokens() ) 
		{
			tail = st.getString();
		}
		//cerr << "TAIL:  " << tail << endl << endl;
		
		static const string states = " <pattern> <that> <topic> ";
		if ( states.find(' ' + word + ' ') != string::npos ) 
		{
			//--	we're moving to next component match .. must return!
			component c = Pattern;	//--	an unfortunate workaround due to use of an enum
			switch ( which ) 
			{
				case Context: c = Pattern; break;
				case Pattern: c = That; break;
				case That: c = Topic; break;
				case Topic: return NULL;			
			}
			if ( (n = node->getChild(toUpper(word))) ) 
			{
				m = match(n, node, c, tail, "", "");
				if ( m != NULL ) 
				{
					m->addStar(star.c_str(), which);
					m->setPattern(path.c_str(), which);
				}
			}
			return m;
		}
		if ( (n = node->getChild(UNDERSCORE.c_str())) ) 
		{
			m = match(n, node, which, tail, word, path + ' ' + UNDERSCORE);
			if ( m != NULL ) 
			{
				m->addStar(star.c_str(), which);
				return m;
			}
		}
		if ( (n = node->getChild(toUpper(word))) ) 
		{
			m = match(n, node, which, tail, star, path + ' ' + word);
			if ( m != NULL ) 
			{
				return m;
			}
		}
		if ( (n = node->getChild(ASTERISK.c_str())) ) 
		{
			m = match(n, node, which, tail, word, path + ' ' + ASTERISK);
			if ( m != NULL ) 
			{
				m->addStar(star.c_str(), which);
				return m;
			}
		}
		if ( node == parent->getChild(ASTERISK.c_str()) || node == parent->getChild(UNDERSCORE.c_str()) ) 
			return match(node, parent, which, tail, star + ' ' + word, path);

		return NULL;
	}
}
//--	end-of-file
