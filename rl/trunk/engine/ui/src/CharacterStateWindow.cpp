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

#include "DsaManager.h"
#include "Person.h"
#include "InputManager.h"
#include "Talent.h"

#include "CharacterStateWindow.h"

using namespace CEGUI;
using namespace Ogre;


namespace rl {

CharacterStateWindow::CharacterStateWindow()
: CeGuiWindow("characterstatewindow.xml", WND_SHOW)
{
	mLP = getProgressBar("CharacterStateWindow/LP");
	mAP = getProgressBar("CharacterStateWindow/AP");
	mName = getStaticText("CharacterStateWindow/Name");
	
	addToRoot(mWindow);	
}

CharacterStateWindow::~CharacterStateWindow()
{
}

void CharacterStateWindow::setCharacter(Person* person)
{
	mCharacter = person;
	update();
}

void CharacterStateWindow::update()
{
	mName->setText(mCharacter->getName());

	float lep, asp;
	if (mCharacter->getLeMax() <= 0)
		lep = 0.0;
	else
		lep = (float)mCharacter->getLe() / (float)mCharacter->getLeMax();

	mLP->setProgress(lep);

	if (!mCharacter->isMagic())
	{
		if (mAP->isVisible())
			mAP->setVisible(false);
		return;
	}

	if (!mAP->isVisible())
		mAP->setVisible(true);

	if (mCharacter->getAeMax() <= 0)
		asp = 0.0;
	else
		asp = (float)mCharacter->getAe() / (float)mCharacter->getAeMax();

	mAP->setProgress(asp);
}


}
