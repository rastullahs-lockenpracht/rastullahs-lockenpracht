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

#ifndef _RenderSystemConfigComponent_H_
#define _RenderSystemConfigComponent_H_

#include "UiPrerequisites.h"
#include "ConfigComponent.h"

namespace rl
{
    class Ogre::RenderSystem;

    class _RlUiExport RenderSystemConfigComponent : public ConfigComponent
    {
    public:
        RenderSystemConfigComponent(const CEGUI::String& xmlfile, const CEGUI::String& drivername);
        virtual ~RenderSystemConfigComponent();

        virtual void apply() = 0;
        virtual void resetToDefaults() = 0;
        virtual void readDriverData() = 0;

    protected:
        virtual Ogre::RenderSystem* getDriver() const = 0;
        virtual void initialize() = 0;
    };
}

#endif // _RenderSystemConfigComponent_H_
