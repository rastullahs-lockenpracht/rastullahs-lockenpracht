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
#ifndef _FMOD4CONFIG_H_
#define _FMOD4CONFIG_H_

#include "SoundDriverConfigComponent.h"
#include "Fmod4Driver.h"

namespace rl
{
    class Fmod4ConfigComponent : public SoundDriverConfigComponent
    {
    public:
        Fmod4ConfigComponent();
        virtual ~Fmod4ConfigComponent();
        virtual void apply();
        virtual void resetToDefaults();
        virtual void readDriverData();

    private:
        bool handleDriverChanged();
        bool handleOutputChanged();
        bool handleSpeakerChanged();

        // Die Combobox, aus der gewählt wird.
        CEGUI::Combobox* mOutput;
        CEGUI::Combobox* mDriver;
        CEGUI::Combobox* mSpeaker;

        // War der Treiber vorher aktiv.
        bool mWasActive;

        Fmod4Driver* mFmod4Driver;
    };

}

#endif /* FMOD4CONFIG_H_*/
