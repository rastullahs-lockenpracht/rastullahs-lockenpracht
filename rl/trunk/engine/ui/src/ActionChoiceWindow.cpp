#include <boost/bind.hpp>

#include "Action.h"
#include "ActionChoiceWindow.h"
#include "ActionManager.h"
#include "GameObject.h"
#include "Creature.h"

using namespace CEGUI;
using namespace std;

namespace rl {

	ActionChoiceWindow::ActionChoiceWindow()
		: CeGuiWindow("actionchoicewindow.xml", true)
	{
		mHint = getStaticText("ActionChoiceWindow/Hint");
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
			actionButton->setText(CeGuiString(action->getDescription()));
			Point pos = getPositionOnRadialMenu(
					center, 0, 0, RADIUS, mButtons.size(), actions.size());
			actionButton->setPosition(Absolute, pos);
			actionButton->setSize(Absolute, Size(40, 40));
			actionButton->subscribeEvent(
				PushButton::EventClicked, 
				boost::bind(
					&ActionChoiceWindow::handleActionChosen, 
					this, actionName));
			actionButton->subscribeEvent(
				PushButton::EventMouseEnters,
				boost::bind(&ActionChoiceWindow::handleShowHint, this, _1));
			actionButton->subscribeEvent(
				PushButton::EventMouseLeaves,
				boost::bind(&ActionChoiceWindow::handleRemoveHint, this));
			mButtons.push_back(actionButton);
			mWindow->addChildWindow(actionButton);
		}
	}
	
	bool ActionChoiceWindow::handleActionChosen(const CeGuiString& action)
	{
		//TODO: Auswahl des Ziels/der Ziele
		//TODO: Ausführung der Action, hier im Dialog oder doch woanders?
		return true;
	}
	
	bool ActionChoiceWindow::handleShowHint(const EventArgs& evt)
	{
		mHint->setText((utf8*)"");
		return true;
	}
	
	bool ActionChoiceWindow::handleRemoveHint()
	{
		mHint->setText((utf8*)"");
		return true;
	}
	
	void ActionChoiceWindow::showTalentsOfPerson(Creature* creature)
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

}
