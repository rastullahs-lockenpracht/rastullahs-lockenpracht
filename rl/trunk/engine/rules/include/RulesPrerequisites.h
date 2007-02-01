/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __RulesPrerequisites_H__
#define __RulesPrerequisites_H__

#include "CommonPrerequisites.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
// Export control
#   if defined( RLRULES_EXPORTS )
#       define _RlRulesExport __declspec( dllexport )
#   else
#       define _RlRulesExport __declspec( dllimport )
#   endif
#else // Linux / Mac OSX etc
#   define _RlRulesExport
#endif

#include <map>
#include <vector>

#define _t(T) CeGuiString(T)

#include <OgreLogManager.h>

#undef min

namespace rl
{
    /**
     * @defgroup RulesRubyExports Rules
     * @ingroup RubyExports
     **/

    /**
     * \page rulespage Bemerkungen zu RlRules
     * @section abbde Deutsche DSA Abkuerzungen
     * @anchor abbdea 
     * AE Astral Energie\n
     * AP Abenteuerpunkte\n
     * AU Ausdauer\n
     * AT Attacke\n
     * @anchor abbdeb
     * BE Behinderung\n
     * @anchor abbdec
     * CH Charisma\n
     * @anchor abbdef
     * FF Fingerfertigkeit\n
     * FK Ferkampf\n
     * @anchor abbdeg
     * GE Gewandheit\n
     * @anchor abbdei
     * INI Initiative\n
     * IN Intuition\n
     * @anchor abbdek
     * KL Klugheit\n
     * KK Koerperkraft\n
     * @anchor abbdel 
     * LE Lebensenergie\n
     * @anchor abbdem
     * MR Magieresistenz\n
     * MU Mut\n
     * @anchor abbdep
     * PA Parade\n
     * @anchor abbdes
     * SE Spezielle Erfahrung\n
     * SF Sonderfertigkeit\n
     * @anchor abbdet 
     * TaW Talentwert\n
     * TP Trefferpunkte\n
     *
     * @section abben English abbreviations
     * @anchor abbenv
     * VI Vitality - LE Lebensenergie
     **/

    class _RlRulesExport Action;
    class _RlRulesExport GameObject;

	typedef std::vector<Action*> ActionVector;
	
	ActionVector::iterator findAction(ActionVector::iterator& begin, ActionVector::iterator& end, const CeGuiString actionName);
}
#endif
