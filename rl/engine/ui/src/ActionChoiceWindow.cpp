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
#include "stdinc.h" //precompiled header

#include "ActionChoiceWindow.h"
#include <CEGUI/WindowManager.h>
#include <boost/bind.hpp>

#include <algorithm>
#include <set>

#include "Action.h"
#include "ActionManager.h"
#include "Creature.h"
#include "Exception.h"
#include "GameObject.h"
#include "UiSubsystem.h"

using namespace CEGUI;
using namespace std;
using namespace Ogre;

namespace rl
{

    const int MAX_NUM_ACTIONS = 4;
    const int MAX_NUM_SUBACTIONS = 7;

    ActionChoiceWindow::ActionChoiceWindow(Creature* actor)
        : AbstractWindow("actionchoicewindow.xml", WIT_MOUSE_INPUT)
        , mActor(actor)
    {
        mHint = getWindow("ActionChoiceWindow/Hint");
        getWindow("ActionChoiceWindow")
            ->subscribeEvent(Window::EventMouseClick, &ActionChoiceWindow::handleClickNotOnButtons, this);
    }

    ActionChoiceWindow::~ActionChoiceWindow()
    {
        /*for (unsigned int i = 0; i<mButtons.size(); i++)
        {
            mWindow->removeChild(mButtons[i]);
            CEGUI::WindowManager::getSingleton().destroyWindow(mButtons[i]);
        }
        mButtons.clear();*/
    }

    bool ActionChoiceWindow::handleClickNotOnButtons(const EventArgs& evt)
    {
        MouseEventArgs mevt = static_cast<const MouseEventArgs&>(evt);
        if (mevt.clickCount == 1) // Doppelklicks von der Truhe rausfiltern
        {
            destroyWindow();
            return true;
        }

        return false;
    }

    int ActionChoiceWindow::showActionsOfObject(GameObject* object)
    {
        LOG_DEBUG2(Logger::UI, "Start", "ActionChoiceWindow::showActionsOfObject");
        mObject = object;

        for (unsigned int i = 0; i < mButtons.size(); i++)
        {
            mWindow->removeChild(mButtons[i]);
            CEGUI::WindowManager::getSingleton().destroyWindow(mButtons[i]);
        }
        mButtons.clear();
        LOG_DEBUG2(Logger::UI, "Buttons geloescht", "ActionChoiceWindow::showActionsOfObject");

        CEGUI::UVector2 center(cegui_reldim(0.5), cegui_reldim(0.5));
        static float RADIUS = 0.10;

        ActionVector actions = object->getValidActions(mActor);
        if (actions.size() != 0)
        {
            LOG_DEBUG2(Logger::UI, "Aktionen ermittelt", "ActionChoiceWindow::showActionsOfObject");

            ActionNode* actionTree = ActionNode::createActionTree(actions);
            LOG_DEBUG2(Logger::UI, "Baum erzeugt", "ActionChoiceWindow::showActionsOfObject");
            createButtons(actionTree, center, RADIUS, 0, 360);

            mButtonCancel = createButton("cancelbutton", center);
            bindDestroyWindowToClick(mButtonCancel);
            mWindow->addChild(mButtonCancel);

            LOG_DEBUG2(Logger::UI, "Buttons erzeugt", "ActionChoiceWindow::showActionsOfObject");
            setButtonActions(actionTree, actionTree);
            LOG_DEBUG2(Logger::UI, "Ende", "ActionChoiceWindow::showActionsOfObject");
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
                Window::EventMouseClick, boost::bind(&ActionChoiceWindow::activateAction, this, action));
            button->subscribeEvent(Window::EventMouseEntersArea,
                boost::bind(&ActionChoiceWindow::showHint, this, action->getDescription()));
            button->subscribeEvent(Window::EventMouseLeavesArea, boost::bind(&ActionChoiceWindow::showHint, this, ""));
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
                const NodeSet nodesToHide = ActionNode::getAllNodesNotBelow(treeRoot, actions);
                LOG_DEBUG2(Logger::UI, StringConverter::toString(nodesToHide.size()) + " nodes to hide",
                    "ActionChoiceWindow::setButtonActions");

                for (NodeSet::const_iterator iter = nodesToHide.begin(); iter != nodesToHide.end(); iter++)
                {
                    button->subscribeEvent(Window::EventMouseEntersArea,
                        boost::bind(&ActionChoiceWindow::setButtonVisible, this, (*iter)->getButton(), false));
                }
            }

            const NodeSet children = actions->getChildren();
            for (NodeSet::const_iterator iter = children.begin(); iter != children.end(); iter++)
            {
                if (button != NULL)
                {
                    button->subscribeEvent(Window::EventMouseEntersArea,
                        boost::bind(&ActionChoiceWindow::setButtonVisible, this, (*iter)->getButton(), true));
                }

                setButtonActions(*iter, treeRoot);
            }
        }
    }

    bool ActionChoiceWindow::activateAction(Action* action)
    {
        LOG_DEBUG2(Logger::UI, "Start", "ActionChoiceWindow::activateAction");
        LOG_DEBUG2(Logger::UI, action->getName().c_str(), "ActionChoiceWindow::activateAction");

        // TODO: Ask for target
        action->doAction(mObject, mActor, NULL);

        LOG_DEBUG2(Logger::UI, "Ende", "ActionChoiceWindow::activateAction");

        destroyWindow();
        return true;
    }

    void ActionChoiceWindow::createButtons(
        ActionNode* actions, const CEGUI::UVector2& center, float radius, float angle, float angleWidth)
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

            const NodeSet children = actions->getChildren();
            float angleStep = angleWidth / (float)children.size();
            float ang = children.size() > 1 ? angle - angleWidth : angle - 180;
            for (NodeSet::const_iterator iter = children.begin(); iter != children.end(); iter++)
            {
                CEGUI::UVector2 centerChild = getPositionOnCircle(center, radius, ang);
                createButtons(*iter, centerChild, radius, ang, 60);
                ang += angleStep;
            }
        }

        actions->setButton(button);
        if (button != NULL)
            mWindow->addChild(button);
    }

    PushButton* ActionChoiceWindow::createButton(const CeGuiString& name, const CEGUI::UVector2& pos)
    {
        Window* button = AbstractWindow::loadWindow("buttons/" + name + ".xml");
        if (button == NULL)
        {
            button = AbstractWindow::loadWindow("buttons/defaultbutton.xml");
        }

        auto size = button->getSize();
        button->setPosition(CEGUI::UVector2(pos.d_x - size.d_width * 0.5, pos.d_y - size.d_height * 0.5));
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
            LOG_DEBUG2(Logger::UI, showHide + "NULL", "ActionChoiceWindow::setButtonVisible");
            return true;
        }

        LOG_DEBUG(Logger::UI, showHide + button->getName());
        // CEGUI::Point p = button->getRelativePosition();
        // LOG_DEBUG(Logger::UI,
        //	"("+StringConverter::toString(p.d_x)+", "+StringConverter::toString(p.d_y)+")");

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

    CEGUI::UVector2 ActionChoiceWindow::getPositionOnCircle(const CEGUI::UVector2& center, float radius, float angle)
    {
        // LOG_DEBUG(Logger::UI,
        //	"center="+StringConverter::toString(center.d_x)+","+StringConverter::toString(center.d_y)+
        //	" radius="+StringConverter::toString(radius)+
        //	" angle="+StringConverter::toString(angle)
        //	);
        static const float PI = 3.1415926;

        float relX = radius * sin(PI * angle / 180);
        float relY = radius * cos(PI * angle / 180);

        LOG_DEBUG(Logger::UI, "diff=" + StringConverter::toString(relX) + "," + StringConverter::toString(relY));

        return center + CEGUI::UVector2(cegui_reldim(relX), cegui_reldim(relY));
    }

    ActionChoiceWindow::ActionNode* ActionChoiceWindow::ActionNode::createActionTree(
        const ActionVector& actions, ActionGroup* rootGroup)
    {
        ActionNode* root = new ActionNode(false);
        root->setGroup(rootGroup);

        std::set<ActionGroup*> groups;
        ActionVector rest;

        for (ActionVector::const_iterator iter = actions.begin(); iter != actions.end(); iter++)
        {
            Action* action = *iter;

            ActionGroup* group = action->getGroup();
            if (group == NULL || group == rootGroup)
            {
                rest.push_back(action);
            }
            else if (groups.find(group) == groups.end())
            {
                groups.insert(group);
            }
        }

        if (actions.size() / 1.2 <= rest.size() + groups.size())
        {
            groups.clear();
            rest = actions;
        }

        for (std::set<ActionGroup*>::iterator groupIter = groups.begin(); groupIter != groups.end(); groupIter++)
        {
            ActionVector actionsThisGroup;
            ActionGroup* thisGroup = *groupIter;

            for (ActionVector::const_iterator actionIter = actions.begin(); actionIter != actions.end(); actionIter++)
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

    const ActionChoiceWindow::NodeSet& ActionChoiceWindow::ActionNode::getChildren()
    {
        return mChildren;
    }

    void ActionChoiceWindow::ActionNode::getAllNodes(ActionNode* treeRoot, NodeSet& nodes)
    {
        nodes.insert(treeRoot);
        const NodeSet children = treeRoot->getChildren();

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

            if ((*iter)->getParent() == treeRoot || *iter == targetNode || (*iter)->getButton() == NULL)
            {
                leaveOut = true;
                continue;
            }

            ActionNode* node = *iter;
            while (node->getParent() != NULL)
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
