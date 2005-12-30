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

#include "Predicates.h"

#include <fstream>
#include <iostream>

using namespace std;

//-----------------------------------------------------------//
//
//	Required Additions
//
//	- Provide mechanisms for default values
//	- Provide mechanism for a global default value
//	- Provide mechanism to allow certain predicates to
//	  return their name, rather than their value when they
//	  are set
//
//	Close, but no cigar! Turns out putting defaults handling
//	in here results in incorrect precedence order :(
//
//-----------------------------------------------------------//



namespace rl 
{
	const static CeGuiString emptyString = "";
	Predicates::Predicates()
	{
	}

	Predicates::~Predicates()
	{
	}

	void Predicates::addPredicate(const CeGuiString& name, const CeGuiString& type, const CeGuiString& value)
	{
		if(mPredicates.find(name) == mPredicates.end())
		{
			mPredicates[name] = value;
		}
	}


	void Predicates::clearPredicate(const CeGuiString& name, const CeGuiString& type)
	{
		std::map<CeGuiString, CeGuiString>::iterator itr
			= mPredicates.find(name);
		if(itr != mPredicates.end())
		{
			mPredicates.erase(itr);
		}
	}
	
	const CeGuiString& Predicates::getPredicate(const CeGuiString& name, const CeGuiString& type)
	{
		if(mPredicates.find(name) != mPredicates.end())
		{
			return mPredicates[name];
		}
		return emptyString;
	}


	void Predicates::setPredicate(const CeGuiString& name, const CeGuiString& type, const CeGuiString& value)
	{
	//	if(mPredicates.find(name) != mPredicates.end())
	//	{
			mPredicates[name] = value;
	//	}
	}




/*	string urldecode(const string &input) 
	{
		return input;
	}
	string urlencode(const string &input) 
	{
		return input;
	}

	HistoryMap							Predicates::history;
	ArrayMap							Predicates::predicates;
	map<string, string>					Predicates::properties;
	map<string, pair<string, bool> >	Predicates::specializedPredicates;
	string								Predicates::defaultPredicate;

	//Predicates::Predicates() {}
	void Predicates::loadPredicates() 
	{
		//--	only load predicates
		ifstream predicates("data/predicates.csv");
		if ( !predicates.is_open() ) return;
	
		string property, id, value, index;
		while ( !predicates.eof() ) {
			getline(predicates, property, ',');
			getline(predicates, id, ',');
			getline(predicates, value, ',');
			getline(predicates, index);
			if ( !property.empty() ) {
				setValue(property, id, urldecode(value), atoi(index.c_str()));
			}
		}
		predicates.close();
	}

	//Predicates::~Predicates() {}

	void Predicates::savePredicates() 
	{
		//--	only save predicates
		ofstream predicates("data/predicates.csv", ios::trunc);
		if ( !predicates.is_open() ) 
			return;
		
		ArrayMap::const_iterator oitr = Predicates::predicates.begin();
		while ( oitr != Predicates::predicates.end() ) 
		{
			const Array &a = oitr->second;
			const string &property = oitr->first.first;
			const string &id = oitr->first.second;
			map<int, string>::const_iterator iitr = a.elements.begin();
			while ( iitr != a.elements.end() ) 
			{
				predicates << property << ',' << id << ',' << urlencode(iitr->second) << ',' << iitr->first << endl;
				++iitr;
			}
			++oitr;
		}
		predicates.close();
	}

	void Predicates::addValue(const string &property, const string &id, const string &value) 
	{
		Array &array = predicates[pair<string, string>(property, id)];

		if ( array.size < 0 ) array.size = 0;
	
		++array.size; ++array.number;
		array.elements[array.size] = value;
	}

	string Predicates::setValue(const string &property, const string &id, const string &value, int index) 
	{
		Array &array = predicates[pair<string, string>(property, id)];
	
		if ( array.elements.find(index) == array.elements.end() ) 
		{
			++array.number;
		}
		if ( index > array.size ) 
		{
			array.size = index;
		}
		array.elements[index] = value;

		map<string, pair<string, bool> >::const_iterator citr = specializedPredicates.find(property);
		if ( citr != specializedPredicates.end() && citr->second.second ) 
		{
			return property;
		} else {
			return value;
		}
	}

	void Predicates::clearValue(const string &property, const string &id, int index) 
	{
		//--	not implemented
	}

	void Predicates::clearValues(const string &property, const string &id) 
	{
		//--	not implemented
	}

	string Predicates::getValue(const string &property, const string &id, int index) 
	{
		map<pair<string, string>, Array>::const_iterator itr = predicates.find(pair<string, string>(property, id));
		if ( itr != predicates.end() ) 
		{
			const Array &array = itr->second;
			map<int, string>::const_iterator itr = array.elements.find(index);
			if ( itr != array.elements.end() )
				return itr->second;
		}
		return "";
	}

	string Predicates::getValues(const string &property, const string &id, const string &conjunction) 
	{
		map<pair<string, string>, Array>::const_iterator itr = predicates.find(pair<string, string>(property, id));
		if ( itr != predicates.end()) 
		{
			const Array &array = itr->second;
			map<int, string>::const_iterator itr = array.elements.begin();
			string result = "";
			while ( itr != array.elements.end() ) 
			{
				result += itr->second;
				if ( ++itr == array.elements.end() ) 
				{
					break;
				} else {
					result += conjunction;
				}
			}
			return result;
		}
		return "";
	}

	void Predicates::addHistory(const string &property, const string &id, const string &value)
	{
		History &h = history[pair<string, string>(property, id)];
		if ( h.size != HISTORY_SIZE ) {
			h.size = HISTORY_SIZE;
			h.top = -1;
		}
		++h.top;
		if ( h.top == h.size ) 
			h.top = 0;
		h.elements[h.top] = value;
	}

	string Predicates::getHistory(const string &property, const string &id, int index) 
	{
		if ( index > HISTORY_SIZE || index < 1 ) 
		{
			return "";
		}
		map<pair<string, string>, History>::iterator itr = history.find(pair<string, string>(property, id));
		if ( itr != history.end() ) 
		{
			int ix = index - 1;
			ix = itr->second.top - ix;
			if ( ix < 0 )
				ix += HISTORY_SIZE;
	
			map<int, string>::const_iterator citr = itr->second.elements.find(ix);
			if ( citr != itr->second.elements.end() )
				return citr->second;
		}
		return "";
	}

	void Predicates::setProperty(const string &property, const string &value) 
	{
		properties[property] = value;
	}

	string Predicates::getProperty(const string &property) 
	{
		return properties[property];
	}

//--	additions

	void Predicates::specializePredicate(const string &property, const string &value, bool returnName) 
	{
		specializedPredicates[property] = pair<string, bool>(value, returnName);
	}

	void Predicates::setGlobalDefault(const string &value) 
	{
		defaultPredicate = value;
	}

	string Predicates::lookupPredicate(const string &property, const string &defaultValue) 
	{
		if ( !defaultValue.empty() )
			return defaultValue;

		map<string, pair<string, bool> >::const_iterator itr = specializedPredicates.find(property);
		if ( itr != specializedPredicates.end() ) 
		{
			if ( !itr->second.first.empty() )
				return itr->second.first;
		}
		return defaultPredicate;
	}
	*/
}// Namespace rl end
//--	end-of-file
