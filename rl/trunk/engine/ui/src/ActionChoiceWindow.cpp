/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
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
#include "UiPrerequisites.h"

#include <boost/bind.hpp>
#include <set>

#include "Action.h"
#include "ActionManager.h"
#include "GameObject.h"
#include "Person.h"
#include "WindowManager.h"
#include "CoreSubsystem.h"

#include "ActionChoiceWindow.h"

using namespace CEGUI;
using namespace std;

namespace rl {

	const int MAX_NUM_ACTIONS = 4;
	const int MAX_NUM_SUBACTIONS = 7;

	ActionChoiceWindow::ActionChoiceWindow(Person* actor)
		:	CeGuiWindow("actionchoicewindow.xml", WND_MOUSE_INPUT),
			mActor(actor)
	{
		mHint = getStaticText("ActionChoiceWindow/Hint");
		addToRoot(mWindow);
	}
	
	ActionChoiceWindow::~ActionChoiceWindow()
	{
		/*for (unsigned int i = 0; i<mButtons.size(); i++)
		{
			mWindow->removeChildWindow(mButtons[i]);
			CEGUI::WindowManager::getSingleton().destroyWindow(mButtons[i]);
		}
		mButtons.clear();*/
	}
	
	void ActionChoiceWindow::showActionsOfObject(GameObject* object)
	{
		mObject = object;
		
		mGroupedButtons.clear();
		for (unsigned int i = 0; i<mButtons.size(); i++)
		{
			mWindow->removeChildWindow(mButtons[i]);
			CEGUI::WindowManager::getSingleton().destroyWindow(mButtons[i]);
		}
		mButtons.clear();

		Point center = mWindow->relativeToAbsolute(Point(0.5, 0.5));
		static int RADIUS = 80;

		ActionVector actions = object->getValidActions();
        ActionNode* actionTree = ActionNode::createActionTree(actions);
		createButtons(actionTree, center, RADIUS, 0, 360);
//		setButtonActions(actionTree);
	}

	void ActionChoiceWindow::createButtons(ActionNode* actions, const Point& center, float radius, float angle, float angleWidth)
	{
		PushButton* button = NULL;

		if (actions->isLeaf())
			button = createButton(actions->getAction()->getName(), center);
		else
		{
			if (actions->getGroup() != NULL)
				button = createButton(actions->getGroup()->getName(), center);
			
			set<ActionNode*> children = actions->getChildren();
			float angleStep = angleWidth * 2.0 / (float)children.size();
			float ang = children.size()>1 ? angle - angleWidth : angle;
			for (set<ActionNode*>::iterator iter = children.begin(); iter != children.end(); iter++)
			{
				Point centerChild = getPositionOnCircle(center, radius, ang);
				createButtons(*iter, centerChild, radius, ang, 60);
				ang += angleStep;
			}
		}

		actions->setButton(button);
		if (button != NULL)
			addToRoot(button);		
	}

	PushButton* ActionChoiceWindow::createButton(const CeGuiString& name, const Point& pos)
	{
		PushButton* groupButton = reinterpret_cast<PushButton*>(
			//WindowManager::getSingleton().loadWindowLayout(
			//	"buttons/"+actionName+".xml"));
			CEGUI::WindowManager::getSingleton().createWindow(
				(utf8*)"TaharezLook/Button", 
				getName()+"/Buttons/"+name));

		groupButton->setText(name);
		groupButton->setSize(Absolute, Size(40, 40));
		groupButton->setPosition(Absolute, pos - Point(20, 20));
		

		return groupButton;
	}
	
	bool ActionChoiceWindow::showButton(PushButton* button)
	{
		CoreSubsystem::getSingleton().log(("Show "+button->getName()).c_str());
		Point p = button->getRelativePosition();
		CoreSubsystem::getSingleton().log(
			"("+StringConverter::toString(p.d_x)+", "+StringConverter::toString(p.d_y)+")");
		button->show();
		return true;
	}

	bool ActionChoiceWindow::hideAllGroupedButtons()
	{
		for (set<PushButton*>::iterator iter = mGroupedButtons.begin();
			iter != mGroupedButtons.end(); iter++)
			(*iter)->hide();

		return true;
	}
	
	bool ActionChoiceWindow::handleShowHint(const CeGuiString& hint)
	{
		mHint->setText(hint);
		return true;
	}

	float ActionChoiceWindow::normalizeAngle(float angle)
	{
		float ang = angle;

		while (ang < 0)
			ang += 360;
		while (ang > 360)
			ang -= 360;

		return ang;
	}
	
	Point ActionChoiceWindow::getPositionOnCircle(
		const Point& center, float radius, float angle)
	{
		static const float PI = 3.1415926;
		
		float relX = radius * sin(PI * angle/180);
		float relY = radius * cos(PI * angle/180);

		return center + Point(relX, relY);
	}
	
	ActionChoiceWindow::ActionNode* ActionChoiceWindow::ActionNode::createActionTree(const ActionVector& actions, ActionGroup* rootGroup)
	{
		ActionNode* root = new ActionNode(false);
		root->setGroup(rootGroup);

		set<ActionGroup*> groups;
		ActionVector rest;
		for (ActionVector::const_iterator iter = actions.begin(); iter != actions.end(); iter++)
		{
			Action* action = *iter;

			ActionGroup* group = action->getGroup();
			if (group == NULL || group == rootGroup )
				rest.push_back(action);
			else if (groups.find(group) == groups.end())
				groups.insert(group);
		}

		if (actions.size() / 1.2 <= rest.size() + groups.size())
		{
			groups.clear();
			rest = actions;
		}

		for (set<ActionGroup*>::iterator groupIter = groups.begin();
			 groupIter != groups.end(); groupIter++)
		{
			ActionVector actionsThisGroup;
			ActionGroup* thisGroup = *groupIter;

			for (ActionVector::const_iterator actionIter = actions.begin(); 
				 actionIter != actions.end(); actionIter++)
			{
				Action* action = *actionIter;
				if (action->getGroup() == thisGroup)
					actionsThisGroup.push_back(action);
			}

			if (actionsThisGroup.size() > 0)
			{
				ActionNode* actionNodeGroup = createActionTree(actionsThisGroup, thisGroup);
				root->addChild(actionNodeGroup);
			}
		}

		for (ActionVector::iterator iter = rest.begin(); iter != rest.end(); iter++)
		{
			ActionNode* node = new ActionNode(true);
			node->setAction(*iter);
			root->addChild(node);
		}

		return root;
	}

	void ActionChoiceWindow::ActionNode::addChild(ActionChoiceWindow::ActionNode* child) 
	{ 
		child->setParent(this); 
		mChildren.insert(child); 
	}
	
	const std::set<ActionChoiceWindow::ActionNode*>& ActionChoiceWindow::ActionNode::getChildren() 
	{ 
		return mChildren; 
	}

	
}
