/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
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

#include "Utils.h"

//--	Utils.cpp
namespace rl
{
	string toUpper(const string &str) 
	{
		string result = str;
		for ( register unsigned int ix = 0; ix < result.length(); ++ix )		
			result[ix] = toupper(result[ix]);
		return result;
	}

	string toLower(const string &str) 
	{
		string result = str;
		for ( register unsigned int ix = 0; ix < result.length(); ++ix ) 
			result[ix] = toupper(result[ix]);
		return result;
	}

	//--	possibly a bug in here
	string trim(const string &str) 
	{
		string result = str;
		string::size_type index = result.find_last_not_of(" \t\n\r");
		if ( index != string::npos ) 
		{
			result.erase(index + 1);
		} else {
			return "";
		}
		index = result.find_first_not_of(" \t\n\r");
		if ( index != string::npos ) 
			result.erase(0, index);
		return result;
	}
	
	static const string WS = " \n\t\r";

	bool checkHeadWS(const string &data) 
	{
		string::size_type index = data.find_first_not_of(WS);
		return index != 0 && index != string::npos;
	}

	bool checkTailWS(const string &data) 
	{
		string::size_type index = data.find_last_not_of(WS);
		return index != string::npos && index != data.length() - 1;
	}

	string normalise(const string &data) 
	{
		string buffer = data;
		string::size_type index = buffer.find_first_not_of(WS);
		if ( index != string::npos ) 
			buffer.erase(0, index);
		index = buffer.find_last_not_of(WS);
		if ( index != string::npos && index < buffer.length() - 1 ) 
		{
			buffer.erase(index + 1);
		} else if ( index == string::npos ) {
			return "";
		}
		string result;
		bool addedSpace = false;
		for ( index = 0; index < buffer.length(); ++index ) 
		{
			if ( WS.find(buffer[index]) != string::npos ) 
			{
				if ( addedSpace ) continue;
				else 
				{
					result += ' ';
					addedSpace = true;
				}
			} else {
				result += buffer[index];
				addedSpace = false;
			}
		}
		return result;
	}
}
//--	end-of-file
