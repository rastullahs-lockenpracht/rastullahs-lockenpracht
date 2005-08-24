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

#include "Action.h"
#include "Exception.h"

namespace rl
{
    Action::Action(const CeGuiString& name, const CeGuiString& description) 
		: mName(name),
          mDescription(description),
		  mGroup(NULL)
    {
    }

    Action::~Action()
    {
    }

    const CeGuiString& Action::getName() const
    {
        return mName;
    }

	/*const CeGuiString& Action::getClassName() const
	{
		static CeGuiString className = (CEGUI::utf8*)"Action";
		return className;
	}*/

    const CeGuiString& Action::getDescription() const
    {
        return mDescription;
    }

    int Action::getTargetClass() const
    {
        return TC_NO_TARGET;
    }

	bool Action::canDo(GameObject* object, Creature* actor) const
    {
        // Im Zweifel klappts halt. :)
        return true;
    }
    
    void Action::doAction(GameObject* object,
                          Creature* actor,
                          GameObject* target)
    {
        // Muss ueberschrieben werden, sonst passiert nix.
    }
	
	void Action::setGroup(ActionGroup* group)
	{
		mGroup = group;
	}
	
	ActionGroup* Action::getGroup() const
	{
		return mGroup;
	}
	
	ActionGroup::ActionGroup(CeGuiString name, ActionGroup* parent)
	{
		mParent = parent;
		mName = name;
		
		if (parent != NULL)
			parent->addChild(this);
	}
	
	ActionGroup::~ActionGroup()
	{
		if (mParent != NULL)
			mParent->removeChild(this);
		
		for (ChildrenList::iterator it = mChildren.begin(); it != mChildren.end(); it++)
			(*it)->removeParent();
	}
	
	const CeGuiString& ActionGroup::getName() const
	{
		return mName;
	}
	
	const ActionGroup* const ActionGroup::getParent() const
	{
		return mParent;
	}
	
	void ActionGroup::addChild(ActionGroup* child)
	{
		mChildren.insert(child);
	}
	
	void ActionGroup::removeChild(ActionGroup* child)
	{
		ChildrenList::iterator it = mChildren.find(child);
		if (it != mChildren.end())
			mChildren.erase(it);
	}
	
	void ActionGroup::removeParent()
	{
		mParent = NULL;
	}
};
