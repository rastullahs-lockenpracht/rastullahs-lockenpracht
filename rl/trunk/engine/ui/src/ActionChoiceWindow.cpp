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
#include <boost/bind.hpp>
#include "UiPrerequisites.h"

#include <set>

#include "Action.h"
#include "ActionManager.h"
#include "GameObject.h"
#include "Person.h"
#include "UiSubsystem.h"
#include "Exception.h"

#include "ActionChoiceWindow.h"

using namespace CEGUI;
using namespace std;
using namespace Ogre;

namespace rl {

	const int MAX_NUM_ACTIONS = 4;
	const int MAX_NUM_SUBACTIONS = 7;

	ActionChoiceWindow::ActionChoiceWindow(Person* actor)
		:	CeGuiWindow("actionchoicewindow.xml", WND_MOUSE_INPUT),
			mActor(actor)
	{
		mHint = getStaticText("ActionChoiceWindow/Hint");
		bindClickToCloseWindow(getWindow("ActionChoiceWindow"));
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
	
	int ActionChoiceWindow::showActionsOfObject(GameObject* object)
	{
		UiSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, 
			"Start", "ActionChoiceWindow::showActionsOfObject");
		mObject = object;
		
		for (unsigned int i = 0; i<mButtons.size(); i++)
		{
			mWindow->removeChildWindow(mButtons[i]);
			CEGUI::WindowManager::getSingleton().destroyWindow(mButtons[i]);
		}
		mButtons.clear();
		UiSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, 
			"Buttons gelöscht", "ActionChoiceWindow::showActionsOfObject");

		CEGUI::Point center = mWindow->relativeToAbsolute(CEGUI::Point(0.5, 0.5));
		static int RADIUS = 80;

		ActionVector actions = object->getValidActions(mActor);
		if (actions.size() != 0)
		{
			UiSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, 
				"Aktionen ermittelt", "ActionChoiceWindow::showActionsOfObject");
			ActionNode* actionTree = ActionNode::createActionTree(actions);
			UiSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, 
				"Baum erzeugt", "ActionChoiceWindow::showActionsOfObject");
			createButtons(actionTree, center, RADIUS, 0, 360);

			mButtonCancel = createButton("cancelbutton", center);
			bindClickToCloseWindow(mButtonCancel);
			mWindow->addChildWindow(mButtonCancel);
			
			UiSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, 
				"Buttons erzeugt", "ActionChoiceWindow::showActionsOfObject");
			setButtonActions(actionTree, actionTree);
			UiSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, 
				"Ende", "ActionChoiceWindow::showActionsOfObject");
		}
		return actions.size();
	}
	
	void ActionChoiceWindow::setButtonActions(ActionNode* actions, ActionNode* treeRoot)
	{
		PushButton* button = actions->getButton();
			
		if (actions->isLeaf())
		{
			Action* action = actions->getAction();
			
			if (actions->getGroup() != NULL)
				button->setVisible(false);
			else
				button->setVisible(true);

			button->subscribeEvent(
				Window::EventMouseClick,
				boost::bind(
					&ActionChoiceWindow::activateAction, this, action));
			button->subscribeEvent(
				Window::EventMouseEnters,
				boost::bind(
					&ActionChoiceWindow::showHint, this, action->getDescription()));
			button->subscribeEvent(
				Window::EventMouseLeaves,
				boost::bind(
					&ActionChoiceWindow::showHint, this, ""));
		}
		else
		{
			ActionGroup* gr = actions->getGroup();
			
			if (gr != NULL && gr->getParent() != NULL)
			{
				button->setVisible(false);					
			}
			else if (button != NULL)
				button->setVisible(true);
			
			if (button != NULL)
			{
				const NodeSet nodesToHide = 
					ActionNode::getAllNodesNotBelow(treeRoot, actions);
				UiSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, 
					StringConverter::toString(nodesToHide.size())+" nodes to hide",
					"ActionChoiceWindow::setButtonActions");
					
				for (NodeSet::const_iterator iter = nodesToHide.begin(); iter != nodesToHide.end(); iter++)
				{			
					button->subscribeEvent(
						Window::EventMouseEnters,
						boost::bind(
							&ActionChoiceWindow::setButtonVisible,
							this,
							(*iter)->getButton(),
							false));
				}
			}

			const NodeSet children = actions->getChildren();
			for (NodeSet::const_iterator iter = children.begin(); iter != children.end(); iter++)
			{
				if (button != NULL)
				{					
					button->subscribeEvent(
						Window::EventMouseEnters,
						boost::bind(
							&ActionChoiceWindow::setButtonVisible,
							this,
							(*iter)->getButton(),
							true));
				}
				
				setButtonActions(*iter, treeRoot);
			}
		}		
	}
	
	bool ActionChoiceWindow::activateAction(Action* action)
	{
		UiSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, 
			"Start", "ActionChoiceWindow::activateAction");
		UiSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, 
			action->getName().c_str(), "ActionChoiceWindow::activateAction");
			
		
        try
        {
            //TODO: Ask for target
            action->doAction(mObject, mActor, NULL); 
        }
        catch( ScriptInvocationFailedException& sife )
        {
            UiSubsystem::getSingleton().log(Ogre::LML_CRITICAL,  sife.toString() );
        }
		
		UiSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, 
			"Ende", "ActionChoiceWindow::activateAction");

		destroyWindow();
		return true;
	}

	void ActionChoiceWindow::createButtons(
		ActionNode* actions, const CEGUI::Point& center, 
		float radius, float angle, float angleWidth)
	{
		PushButton* button = NULL;

		if (actions->isLeaf())
		{
			button = createButton(actions->getAction()->getName(), center);
		}
		else
		{
			if (actions->getGroup() != NULL)
			{
				button = createButton(actions->getGroup()->getName(), center);
			}
			
			const set<ActionNode*> children = actions->getChildren();
			float angleStep = angleWidth / (float)children.size();
			float ang = children.size()>1 ? angle - angleWidth : angle;
			for (NodeSet::const_iterator iter = children.begin(); 
				iter != children.end(); iter++)
			{
				CEGUI::Point centerChild = getPositionOnCircle(center, radius, ang);
				createButtons(*iter, centerChild, radius, ang, 60);
				ang += angleStep;
			}
		}

		actions->setButton(button);
		if (button != NULL)
			mWindow->addChildWindow(button);		
	}

	PushButton* ActionChoiceWindow::createButton(const CeGuiString& name, const CEGUI::Point& pos)
	{
		Window* button = CeGuiWindow::loadWindow("buttons/"+name+".xml");
		if (button == NULL)
		{
			button = 
				CeGuiWindow::loadWindow(
					"buttons/defaultbutton.xml");
		}

		CEGUI::Size size = button->getAbsoluteSize();
		button->setPosition(
			Absolute, pos - CEGUI::Point(size.d_width/2, size.d_height/2));
		UiSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, 
			(button->getText()+" "+
			StringConverter::toString(button->getAbsoluteXPosition()) + ", " + 
			StringConverter::toString(button->getAbsoluteYPosition())).c_str(), 
			"createButton");
		
		return static_cast<PushButton*>(button);
	}
	
	bool ActionChoiceWindow::setButtonVisible(PushButton* button, bool visible)
	{
		Ogre::String showHide;
		
		if (visible)
			showHide = "Show ";
		else
			showHide = "Hide ";
		
		if (button == NULL)
		{
			UiSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, 
				showHide + "NULL", "ActionChoiceWindow::setButtonVisible");
			return true;
		}
		
		UiSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, (showHide+button->getName()).c_str());
		CEGUI::Point p = button->getRelativePosition();
		UiSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, 
			"("+StringConverter::toString(p.d_x)+", "+StringConverter::toString(p.d_y)+")");
			
		if (visible)
			button->show();
		else
			button->hide();
		return true;
	}

	bool ActionChoiceWindow::showHint(const CeGuiString& hint)
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
	
	CEGUI::Point ActionChoiceWindow::getPositionOnCircle(
		const CEGUI::Point& center, float radius, float angle)
	{
		UiSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, 
			"center="+StringConverter::toString(center.d_x)+","+StringConverter::toString(center.d_y)+
			" radius="+StringConverter::toString(radius)+
			" angle="+StringConverter::toString(angle)
			);
		static const float PI = 3.1415926;
		
		float relX = radius * sin(PI * angle/180);
		float relY = radius * cos(PI * angle/180);

		UiSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, 
			"diff="+StringConverter::toString(relX)+","+StringConverter::toString(relY));
			

		return center + CEGUI::Point(relX, relY);
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
	
	void ActionChoiceWindow::ActionNode::getAllNodes(ActionNode* treeRoot, std::set<ActionNode*>& nodes)
	{
		nodes.insert(treeRoot);
		const set<ActionNode*> children = treeRoot->getChildren();
		
		for (NodeSet::const_iterator iter = children.begin(); iter != children.end(); iter++)
			getAllNodes(*iter, nodes);
	}
	
	ActionChoiceWindow::NodeSet ActionChoiceWindow::ActionNode::getAllNodesNotBelow(
		ActionNode* treeRoot, ActionChoiceWindow::ActionNode* targetNode)
	{
		NodeSet allNodes;
		getAllNodes(treeRoot, allNodes);
		
		NodeSet nodes;
		for (NodeSet::iterator iter = allNodes.begin(); iter != allNodes.end(); iter++)
		{
			bool leaveOut = false;
			
			if ((*iter)->getParent() == treeRoot ||
				*iter == targetNode ||
				(*iter)->getButton() == NULL)
			{
				leaveOut = true;
				continue;
			}
				
			ActionNode* node = *iter;
			while(node->getParent() != NULL)
			{
				node = node->getParent();
				if (node == targetNode)
				{
					leaveOut = true;
					continue;
				}
			}

			if (!leaveOut)
				nodes.insert(*iter);
		}
		
		return nodes;
	}
	
}
