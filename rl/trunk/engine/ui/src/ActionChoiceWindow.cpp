#include <boost/bind.hpp>

#include "Action.h"
#include "ActionChoiceWindow.h"
#include "ActionManager.h"
#include "GameObject.h"
#include "Person.h"

using namespace CEGUI;
using namespace std;

namespace rl {

	ActionChoiceWindow::ActionChoiceWindow(Person* actor)
		: CeGuiWindow("actionchoicewindow.xml", true)
	{
		mHint = getStaticText("ActionChoiceWindow/Hint");
		mState = IDLE;
		mActor = actor;
		addToRoot(mWindow);
	}
	
	ActionChoiceWindow::~ActionChoiceWindow()
	{
		for (unsigned int i = 0; i<mButtons.size(); i++)
		{
			mWindow->removeChildWindow(mButtons[i]);
			WindowManager::getSingleton().destroyWindow(mButtons[i]);
		}
		mButtons.clear();
	}
	
	void ActionChoiceWindow::showActionsOfObject(GameObject* object)
	{
		mObject = object;
		
		for (unsigned int i = 0; i<mButtons.size(); i++)
		{
			mWindow->removeChildWindow(mButtons[i]);
			WindowManager::getSingleton().destroyWindow(mButtons[i]);
		}
		mButtons.clear();
	
		ActionVector actions = object->getValidActions();
		Point center(300, 200);
		static int RADIUS = 40;
	
		for (	ActionVector::iterator iter = actions.begin(); 
				iter != actions.end(); iter++)
		{
			Action* action = *iter;
			CeGuiString actionName = action->getName();
			PushButton* actionButton = reinterpret_cast<PushButton*>(
				//WindowManager::getSingleton().loadWindowLayout(
				//	"buttons/"+actionName+".xml"));
				WindowManager::getSingleton().createWindow(
					(utf8*)"TaharezLook/Button", 
					getName()+"/Buttons/"+actionName));
			actionButton->setUserData(action);
			actionButton->setText(action->getName());
			Point pos = getPositionOnRadialMenu(
					center, 0, 0, RADIUS, mButtons.size(), actions.size());
			actionButton->setPosition(Absolute, pos);
			actionButton->setSize(Absolute, Size(140, 40));
			switch (action->getTargetClass())
			{
				case TC_NO_TARGET:
					actionButton->subscribeEvent(
						PushButton::EventClicked, 
						boost::bind(
							&GameObject::activateAction, 
							mObject, action, mActor, mObject));
					/*actionButton->subscribeEvent(
						PushButton::EventClicked, 
						boost::bind(
							&WindowManager::destroyWindow, 
							WindowManager::getSingletonPtr, this));*/
					break;
			}
			actionButton->subscribeEvent(
				PushButton::EventMouseEnters,
				boost::bind(&ActionChoiceWindow::handleShowHint, this, action->getDescription()));
			actionButton->subscribeEvent(
				PushButton::EventMouseLeaves,
				boost::bind(&ActionChoiceWindow::handleShowHint, this, (utf8*)""));
			mButtons.push_back(actionButton);
			mWindow->addChildWindow(actionButton);
		}
		mState = CHOOSE_OBJECT_ACTION;
	}
	
	bool ActionChoiceWindow::handleActionChosen(Action* action)
	{
		//TODO: Auswahl des Ziels/der Ziele
		//TODO: Ausführung der Action, hier im Dialog oder doch woanders?
		if (mState == CHOOSE_OBJECT_ACTION)
		{
			if (action->getTargetClass() == TC_NO_TARGET)
				activateAction(action, mObject, mActor, mObject);
			else
				/*selectTarget(action->getTargetClass())*/;
		}

		return true;
	}
	
	bool ActionChoiceWindow::handleShowHint(const CeGuiString& hint)
	{
		mHint->setText(hint);
		return true;
	}
	
	void ActionChoiceWindow::showTalentsOfActor()
	{
		
	}
	
	Point ActionChoiceWindow::getPositionOnRadialMenu(
		Point center, float minAngle, float maxAngle,
		float radius, int elemNum, int elemCount)
	{
		static const float PI = 3.1415926;
		
		if (maxAngle <= minAngle)
			maxAngle += 360;
			
		float angle = 
			minAngle + 
			(float)elemNum/(float)elemCount * (maxAngle - minAngle);
		if (angle > 360)
			angle -= 360;
		
		float relX = radius * sin(PI * angle/180);
		float relY = radius * cos(PI * angle/180);
		
		return center + Point(relX, relY);
	}
	
	void ActionChoiceWindow::activateAction(
		Action* action, GameObject* object, Person* actor, GameObject* target)
	{
		setVisible(false);
		action->doAction(object, actor, target);
		delete this;
	}
}
