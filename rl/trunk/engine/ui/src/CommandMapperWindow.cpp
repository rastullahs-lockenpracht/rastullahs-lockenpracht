#include <boost/bind.hpp>
#include "CommandMapperWindow.h"

using namespace CEGUI;

namespace rl {

CommandMapperWindow::CommandMapperWindow()
	: CeGuiWindow("commandmapper.xml", true)
{
	subscribeEventsToElements();
}

void CommandMapperWindow::subscribeEventsToElements()
{
	getWindow("CommandMapper/ChangeButton")->
		subscribeEvent(
			PushButton::EventClicked,
			boost::bind(&CommandMapperWindow::handleChangeButton, this));
	getWindow("CommandMapper/CloseButton")->
		subscribeEvent(
			PushButton::EventClicked,
			boost::bind(&CommandMapperWindow::handleCloseButton, this));
	getWindow("CommandMapper/TableMovement")->
		subscribeEvent(
			Window::EventMouseDoubleClick,
			boost::bind(&CommandMapperWindow::handleChangeMovement, this));
	getWindow("CommandMapper/TableInBattle")->
		subscribeEvent(
			Window::EventMouseDoubleClick,
			boost::bind(&CommandMapperWindow::handleChangeInBattle, this));
	getWindow("CommandMapper/TableOffBattle")->
		subscribeEvent(
			Window::EventMouseDoubleClick,
			boost::bind(&CommandMapperWindow::handleChangeOffBattle, this));

}

bool CommandMapperWindow::handleChangeButton()
{
	return true;
}

bool CommandMapperWindow::handleCloseButton()
{
	return true;
}

bool CommandMapperWindow::handleChangeInBattle()
{
	return true;
}

bool CommandMapperWindow::handleChangeMovement()
{
	return true;
}

bool CommandMapperWindow::handleChangeOffBattle()
{
	return true;
}

}
