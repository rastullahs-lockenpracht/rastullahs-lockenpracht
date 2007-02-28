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

#ifndef CONFIGCOMPONENT_H
#define CONFIGCOMPONENT_H

#include <CEGUIWindow.h>

#include "UiPrerequisites.h"

namespace rl
{
	/**
	 * Abstract class defining the shared functionality between all
	 * concrete ConfigComponent implementation classes.
	 */
	class _RlUiExport ConfigComponent
	{
	public:
		ConfigComponent(const CEGUI::String& xmlfile);
		virtual ~ConfigComponent();

		/**
		 * Adds this component's root window to a given parent window
		 *
		 * @param parent Pointer to parent CEGUI::Window
		 */
		void addTo(CEGUI::Window* parent);

		/**
		 * Sets the visibilty state of the window
		 *
		 * @param visible true sets window to visible, false the other way
		 */
		void setVisible(bool visible);

	protected:
		/**
		 * Returns the name for the given driver
		 */
		virtual const CEGUI::String getDriverName() const = 0;

		/**
		 * Initialises the driver of the concrete ConfigComponent
		 */
		virtual void initialize() = 0;

	private:
		CEGUI::Window* mWindow;   //!< CEGUI root window for this component
		CEGUI::String mXmlFile;   //!< CEGUI layout file
	};
}

#endif // CONFIGCOMPONENT_H
