/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __EIGENSCHAFTENSTATESET_H__
#define __EIGENSCHAFTENSTATESET_H__

#include "StateSet.h"

namespace rl
{

    class _RlRulesExport EigenschaftenStateSet : public StateSet
    {
    public:
        EigenschaftenStateSet();
        ~EigenschaftenStateSet();

        int getStartValue();
        void setStartValue(int newStartValue);
        int getPermanentModifier();
        void setPermanentModifier(int newPermanentModifier);
        int getModifierWithoutRecalculation();
        void setModifierWithoutRecalculation(int newModifierWithoutRecalculation);

        int getValue(bool getUnmodifiedValue = false);
        int getValueForBasiswertCalculation();

    protected:
        // Der Wert bei Charaktererschaffung ohne Rassen-, Kultur- und Vorteilsboni/ -mali
        int mStartValue;
        // Rassen-, Kultur- und Vor-/Nachteils modifikatoren
        int mPermanentModifier;
        // Modifikatoren durch Effekte. Bei einer Neubereuchnung der Grundwerte (AT-Basis, AE etc) wird dieser
        // Modifikator ignoriert
        // @todo Aussagekraeftigeren Namen finden.
        int mModifierWithoutRecalculation;
    };
}

#endif //__EIGENSCHAFTENSTATESET_H__
