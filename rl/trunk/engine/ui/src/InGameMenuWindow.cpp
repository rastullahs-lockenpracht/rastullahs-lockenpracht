/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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
#include <boost/bind.hpp>

#include "UiPrerequisites.h"

#include "UiSubsystem.h"
#include "CoreSubsystem.h"
#include "WindowManager.h"

#include "InGameMenuWindow.h"
#include "Action.h"
#include "ActionManager.h"

#include <map>

using namespace CEGUI;
using namespace Ogre;
using std::map;

namespace rl {

InGameMenuWindow::InGameMenuWindow()
: CeGuiWindow("ingamemenuwindow", WND_MOUSE_INPUT)
{
	createMenu(getMenu("InGameMenu/Menubar"));
	addToRoot(mWindow);
}

InGameMenuWindow::~InGameMenuWindow()
{
}

void InGameMenuWindow::createMenu(MenuBase* menu)
{
	const ActionVector actions = ActionManager::getSingleton().getInGameGlobalActions();
	map<CeGuiString, PopupMenu*> menuGroups;

	for (ActionVector::const_iterator actIter = actions.begin(); actIter != actions.end(); actIter++)
	{
        Action* action = *actIter;
		ActionGroup* group = action->getGroup();
		if (group != NULL)
		{
			PopupMenu* menuGrp;
			map<CeGuiString, PopupMenu*>::iterator grpIter = menuGroups.find(group->getName());
			if (grpIter != menuGroups.end())
			{
				menuGrp = (*grpIter).second;
			}
			else
			{
				WindowFactory* factory = WindowFactoryManager::getSingleton().getFactory("RastullahLook/PopupMenu");
				menuGrp = static_cast<PopupMenu*>(factory->createWindow("IngameMenu/Menu"+group->getName()));

				MenuItem* grpItem = new MenuItem("RastullahLook/MenubarItem", "IngameMenu/"+group->getName());
				grpItem->setPopupMenu(menuGrp);
				menu->addItem(grpItem);
			}

			MenuItem* item = new MenuItem("RastullahLook/MenubarItem", "IngameMenu/"+group->getName()+"/"+action->getName());
			item->setText(action->getDescription());

			setAction(item, action);
		}
	}
}

void InGameMenuWindow::setAction(MenuItem* item, Action* action)
{
}

}
