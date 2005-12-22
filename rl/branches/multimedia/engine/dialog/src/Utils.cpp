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

#include "Utils.h"

//--	Utils.cpp
namespace rl
{
	CeGuiString toUpper(const CeGuiString &str) 
	{
		CeGuiString result = str;
		for ( register unsigned int ix = 0; ix < result.length(); ++ix )		
			result[ix] = toupper(result[ix]);
		return result;
	}

	CeGuiString toLower(const CeGuiString &str) 
	{
		CeGuiString result = str;
		for ( register unsigned int ix = 0; ix < result.length(); ++ix ) 
			result[ix] = toupper(result[ix]);
		return result;
	}

	//--	possibly a bug in here
	CeGuiString trim(const CeGuiString &str) 
	{
		CeGuiString result = str;
		CeGuiString::size_type index = result.find_last_not_of(" \t\n\r");
		if ( index != CeGuiString::npos ) 
		{
			result.erase(index + 1, CeGuiString::npos);
		} else {
			return "";
		}
		index = result.find_first_not_of(" \t\n\r");
		if ( index != CeGuiString::npos ) 
			result.erase(0, index);
		return result;
	}
	
	static const CeGuiString WS = " \n\t\r";

	bool checkHeadWS(const CeGuiString &data) 
	{
		CeGuiString::size_type index = data.find_first_not_of(WS);
		return index != 0 && index != CeGuiString::npos;
	}

	bool checkTailWS(const CeGuiString &data) 
	{
		CeGuiString::size_type index = data.find_last_not_of(WS);
		return index != CeGuiString::npos && index != data.length() - 1;
	}

	CeGuiString normalise(const CeGuiString &data) 
	{
		CeGuiString buffer = data;
		CeGuiString::size_type index = buffer.find_first_not_of(WS);
		if ( index != CeGuiString::npos ) 
			buffer.erase(0, index);
		index = buffer.find_last_not_of(WS);
		if ( index != CeGuiString::npos && index < buffer.length() - 1 ) 
		{
			buffer.erase(index + 1, CeGuiString::npos);
		} else if ( index == CeGuiString::npos ) {
			return "";
		}
		CeGuiString result;
		bool addedSpace = false;
		for ( index = 0; index < buffer.length(); ++index ) 
		{
			if ( WS.find(buffer[index]) != CeGuiString::npos ) 
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
