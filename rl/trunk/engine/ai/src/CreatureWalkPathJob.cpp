/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#include "CreatureWalkPathJob.h"

namespace rl
{
    CreatureWalkPathJob::CreatureWalkPathJob(const Ogre::String& name, MovingCreature* movingCreature, Landmark* startLandmark) : Job(true, true), 
        mLandmarkPath("LandmarkPath" + name)
    {
        //the moving creature moves from the current position to the landmark
        mMovingCreature = movingCreature;
        mCurrentLandmark = new Landmark(name + "_startup",mMovingCreature->getCreature()->getPosition());
        mNextLandmark = startLandmark;
    }

    CreatureWalkPathJob::~CreatureWalkPathJob()
    {
    }

    bool CreatureWalkPathJob::execute(Ogre::Real time)
    {
        if (mMovingCreature->getCreature()->getPosition() == mNextLandmark->getPosition())
        {
            if (mLandmarkPath.getPoints().size())
            {
                mCurrentLandmark = mNextLandmark;
                mNextLandmark = mLandmarkPath.getPoints().front();
                mLandmarkPath.getPoints().pop_front();
            }
            else
            {
                mCurrentLandmark = mNextLandmark;
                //@todo
            }
            
        }
        else if (mCurrentLandmark != mNextLandmark)
        {
            updateCreature(time);
        }

        return false;
    }

    void CreatureWalkPathJob::addLandmark(const Ogre::String& name, const Ogre::Vector3& position)
    {
        mLandmarkPath.addPoint(name, position);
    }

    void CreatureWalkPathJob::addLandmark(Landmark* lm)
    {
        mLandmarkPath.addPoint(lm);
    }

    MovingCreature* CreatureWalkPathJob::getMovingCreature()
    {
        return mMovingCreature;
    }

    void CreatureWalkPathJob::updateCreature(Ogre::Real time)
    {
        LOG_MESSAGE("JOB","CreatureWalkPathJob::updateCreature");
        //@todo: calculate translation and rotation
        AbstractMovement* rotate = mMovingCreature->getMovementFromId(MovingCreature::MT_DREHEN);
        Ogre::Real baseVelRot = 0;
        AbstractMovement* translate = mMovingCreature->getMovementFromId(MovingCreature::MT_RENNEN);
        Ogre::Real baseVelTrans = 0;
        Ogre::Radian rotation(time) ;
        Ogre::Vector3 direction = Ogre::Vector3(0,0,0);

        if(rotate->calculateBaseVelocity(baseVelRot))
        {
            Ogre::Vector3 creatureViewVector = mMovingCreature->getCreature()->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
            Ogre::Radian baseVelRotRad(baseVelRot * 2 * Ogre::Math::PI);
            //@todo
        }
        //if(translate->calculateBaseVelocity(baseVelTrans))
        //{
        direction = mNextLandmark->getPosition() - mCurrentLandmark->getPosition();
            //@todo
        //}

        LOG_MESSAGE("Movement", "Vec3 " + Ogre::StringConverter::toString(direction) 
            + " Set Movement " + Ogre::StringConverter::toString(
            mMovingCreature->setMovement(MovingCreature::MT_GEHEN, direction, Ogre::Vector3(0,rotation.valueRadians(),0))));
        //mMovingCreature->setAnimation("rennen");

        //make sure that the creature can't fail the landmark
        //Ogre::Vector3 diffTrack = mNextLandmark->getPosition() - mCurrentLandmark->getPosition();
        if(direction.dotProduct(mNextLandmark->getPosition()-mMovingCreature->getCreature()->getPosition()) < 0)
            mMovingCreature->getCreature()->setPosition(mNextLandmark->getPosition());
    }
}
