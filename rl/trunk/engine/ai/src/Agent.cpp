/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2006 Team Pantheon. http://www.team-pantheon.de
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
#include "Agent.h"
#include "Exception.h"
#include "Creature.h"
#include "DialogCharacter.h"
#include "SteeringVehicle.h"
#include "SteeringMachine.h"
//#include "PerceptionPool.h"

using namespace rl;

Agent::Agent(Creature* character)
	: mBehaviour(NULL), mVehicle(NULL), mDialogBot(NULL), 
	  mCreature(character)
//	  mPerceptionPool(new PerceptionPool())
{
	initialize();
}

Agent::Agent(DialogCharacter* character)
	: mBehaviour(NULL), mVehicle(NULL), mDialogBot(character), 
	  mCreature(character->getNonPlayerCharacter())
//	  mPerceptionPool(new PerceptionPool())
{
	initialize();
}

Agent::~Agent(void)
{
    delete mVehicle;
    delete mBehaviour;
//	delete mPerceptionPool;
}

void Agent::initialize()
{
	if(mCreature == NULL)
	{
		Throw(NullPointerException, "Agent has no creature");
	}
    mVehicle = new SteeringVehicle(this, mCreature->getActor());
    Logger::getSingleton().log(
        Logger::AI, 
        Logger::LL_NORMAL, 
        "created SteeringVehicle for Agent");
    
	mBehaviour = new SteeringMachine(NULL, mVehicle);
    Logger::getSingleton().log(
        Logger::AI, 
        Logger::LL_NORMAL, 
        "created SteeringMachine for Agent");
//  a perceptron should be the controller, and the perceptron calculates
//  the steering force with the help of different steering behaviours
    PhysicsManager::getSingleton().
        setPhysicsController(mCreature->getActor()->getPhysicalThing(), this);
    Logger::getSingleton().log(
        Logger::AI, 
        Logger::LL_NORMAL, 
        "added Agent to PhysicsManager as PhysicsController");
//  TODO: if creature == NULL throw exception
}

void Agent::addSteeringBehaviour(SteeringBehaviour* behaviour)
{
    behaviour->setParent(mBehaviour);
    behaviour->setController(mVehicle);
    mBehaviour->addState(behaviour);
    Logger::getSingleton().log(
        Logger::AI, 
        Logger::LL_MESSAGE, 
        "added SteeringBehaviour for Agent");
}

void Agent::update(const float elapsedTime)
{
    //	mBehaviour->update(elapsedTime);
}

void Agent::OnApplyForceAndTorque(PhysicalThing* thing)
{
    OgreNewt::World* world = PhysicsManager::getSingleton()._getNewtonWorld();
	Real elapsedTime = world->getTimeStep();
    mBehaviour->update(elapsedTime);
	// currentTime not needed yet, only elapsedTime
    mVehicle->update(0.0f, elapsedTime);
}

bool Agent::isDialogActive()
{
	return mDialogBot->isActive();
}