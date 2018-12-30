/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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

/* Wrapping rl::CeGuiStrings to ruby and back 
 for rl::CeGuiString
*/
%typemap(typecheck) rl::CeGuiString, const rl::CeGuiString, rl::CeGuiString*, rl::CeGuiString&, const rl::CeGuiString*, const rl::CeGuiString& = char *;

%apply std::string { rl::CeGuiString };
%apply const std::string { const rl::CeGuiString };

%apply std::string* { rl::CeGuiString* };
// Not defined yet %apply const rl::CeGuiString* { const rl::CeGuiString* };

%apply std::string& { rl::CeGuiString& };
%apply const std::string& { const rl::CeGuiString& };
