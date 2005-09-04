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

#ifndef __ActionChoiceWindow_H__
#define __ActionChoiceWindow_H__

#include "UiPrerequisites.h"

#include "CeGuiWindow.h"
#include <set>
#include <vector>

namespace rl {

	class Action;
	class GameObject;
	class Person;
	class ActionGroup;

	class _RlUiExport ActionChoiceWindow : public CeGuiWindow
	{
	public:
		ActionChoiceWindow(Person* actor);
		~ActionChoiceWindow();
		
		int showActionsOfObject(GameObject* object);
		bool showHint(const CeGuiString& evt);

	private:	
		class ActionNode;

        struct actnodecmp 
        {
            bool operator()(ActionNode* x, ActionNode* y) const
            {
                if( x->getAction() != NULL && y->getAction() != NULL )
                    return x->getAction()->getName().compare( 
                    y->getAction()->getName() ) < 0;
                else if( x->getGroup() != NULL && y->getGroup() != NULL )
                    return x->getGroup()->getName().compare( 
                    y->getGroup()->getName() )  < 0;
                else if( x->getGroup() != NULL )
                    return true;
                else
                    return false;
            }
        };

        typedef std::set<ActionNode*,actnodecmp> NodeSet;
	
		static CEGUI::Point 
			getPositionOnCircle(
				const CEGUI::Point& center, 
				float radius, 
				float angle);

		void createButtons(
			ActionNode* actions, 
			const CEGUI::Point& center, 
			float radius, 
			float minAngle, 
			float maxAngle);

		CEGUI::PushButton* createButton(
			const CeGuiString& name,
			const CEGUI::Point& pos);
			
		void setButtonActions(ActionNode* actions, ActionNode* treeRoot);

		bool setButtonVisible(CEGUI::PushButton* button, bool visible);
		bool activateAction(Action* action);
		static float normalizeAngle(float angle);
		
		GameObject* mObject;
		std::vector<CEGUI::PushButton*> mButtons;
		CEGUI::PushButton* mButtonCancel;
		CEGUI::StaticText* mHint;

		Person* mActor;		
		
		class ActionNode
		{
		public:
			ActionNode(bool isLeaf) : mAction(NULL), mGroup(NULL), mLeaf(isLeaf), mParent(NULL) {}
			~ActionNode();

			void setAction(Action* a) { mAction = a; }
			Action* getAction() { return mAction; }
			
			void setGroup(ActionGroup* ag) { mGroup = ag; }
			ActionGroup* getGroup() { return mGroup; }
			
			void setParent(ActionNode* n)  { mParent = n; }
			ActionNode* getParent() { return mParent; }
			
			void setButton(CEGUI::PushButton* b)  { mButton = b; }
			CEGUI::PushButton* getButton() { return mButton; }
	
			void addChild(ActionNode* child);
			void removeChild(ActionNode* child);
			const NodeSet& getChildren();
			
			bool isLeaf() { return mLeaf; }
			
			static ActionNode* createActionTree(const ActionVector& actions, ActionGroup* rootGroup = NULL);
			static NodeSet getAllNodesNotBelow(ActionNode* treeRoot, ActionNode* targetNode);
			static void getAllNodes(ActionNode* treeRoot, NodeSet& node);
	
		private:
			bool handleClickNotOnButtons(const CEGUI::EventArgs& evt);

			NodeSet mChildren;
			ActionNode* mParent;
			bool mLeaf;
			Action* mAction;
			ActionGroup* mGroup;
			CEGUI::PushButton* mButton;
		};


	};

}

#endif
