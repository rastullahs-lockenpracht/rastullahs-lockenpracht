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

#ifndef _SoundDriverConfigComponent_H_
#define _SoundDriverConfigComponent_H_

#include "ConfigComponent.h"
#include "UiPrerequisites.h"

namespace rl
{

    class SoundDriver;

    class _RlUiExport SoundDriverConfigComponent : public ConfigComponent
    {
    public:
        SoundDriverConfigComponent(const CEGUI::String& xmlfile, const CEGUI::String& drivername);
        virtual ~SoundDriverConfigComponent();

        virtual void apply() = 0;
        virtual void resetToDefaults() = 0;
        virtual void readDriverData() = 0;
    };
}

#endif // _SoundDriverConfigWindow_H_
