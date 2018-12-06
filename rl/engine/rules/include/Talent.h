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

#ifndef __TALENT_H__
#define __TALENT_H__

#include "RulesPrerequisites.h"

#include "Eigenschaft.h"
#include "Tripel.h"

namespace rl
{
    class _RlRulesExport Talent
    {
    public:
        typedef std::map<const CeGuiString, int> AusweichTalente;
        Talent(const CeGuiString name, const CeGuiString description, const EigenschaftTripel& eigenschaften, int ebe,
            int gruppe, const CeGuiString art, const AusweichTalente& ausweichTalente);

        bool operator==(const Talent& rhs) const;
        bool operator<(const Talent& rhs) const;
        CeGuiString getName() const;
        CeGuiString getDescription() const;
        int getEbe() const;

        /// Berechnet effektive Behinderung bei gegebener Behinderung;
        int calculateEbe(int be) const;
        EigenschaftTripel getEigenschaften() const;
        CeGuiString getArt() const;
        /**
         *  @brief Gibt eine Liste der Talente zurueck, auf die ausgewichen
         *   werden kann zuzueglich der Schwierigeit darauf auszuweichen
         **/
        AusweichTalente getAusweichTalente() const;

    private:
        const CeGuiString mName;
        const CeGuiString mDescription;
        const EigenschaftTripel mEigenschaften;
        const int mEbe;
        const int mTalentGruppe;
        const CeGuiString mArt;
        AusweichTalente mAusweichTalente;
    };
}
#endif //__TALENT_H__
