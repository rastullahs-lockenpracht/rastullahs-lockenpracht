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

#include "stdinc.h"

#include "FetchItemJob.h"

#include "Container.h"
#include "Creature.h"
#include "CreatureController.h"
#include "CreatureControllerManager.h"
#include "Inventory.h"
#include "Item.h"
#include "MathUtil.h"

using namespace Ogre;

namespace rl
{
	FetchItemJob::FetchItemJob(Creature* actor, Item* item, const Ogre::String& targetSlot, Ogre::Real duration)
		: Job(false, true, TimeSource::REALTIME_INTERRUPTABLE),
		  mActor(CreatureControllerManager::getSingleton().getCreatureController(actor)),
		  mItem(item),
		  mTargetSlot(targetSlot),
		  mTimeLeft(duration)
	{
	}


    FetchItemJob::~FetchItemJob()
	{
	}

    bool FetchItemJob::execute(Ogre::Real time)
	{
		if (mTimeLeft < 0 
			|| !mItem 
			|| mItem->getState() == GOS_UNDEFINED
			|| mItem->getState() == GOS_UNLOADED
			|| mItem->getState() == GOS_LOADED)
		{
			// Stay put where ever we are.
			mActor->setMovement(CreatureController::MT_STEHEN, Vector3::ZERO, Vector3::ZERO);
			return true;
		}

		Vector3 targetPos = mItem->getPosition();

		// Are we there now?
		Ogre::Real distance = MathUtil::distance(mItem->getWorldBoundingBox(),
			mActor->getCreature()->getWorldBoundingBox());
		if (distance < 1.0f)
		{
			mActor->setMovement(CreatureController::MT_STEHEN, Vector3::ZERO, Vector3::ZERO);
            
            if (mItem->getState() == GOS_IN_SCENE)
            {
                    // @todo play pickup animation
                    mActor->getCreature()->getInventory()->hold(mItem, mTargetSlot);
                    return true;
			}
            else if (mItem->getState() == GOS_IN_POSSESSION)
			{
				Container* container = mItem->getParentContainer();
				if (container && container->hasAction(CeGuiString("open"), mActor->getCreature()))
				{
					container->doAction(CeGuiString("open"), mActor->getCreature(), mItem);
					container->removeItem(mItem);
					mActor->getCreature()->getInventory()->hold(mItem, mTargetSlot);
					return true;
				}
			}
            else if (mItem->getState() == GOS_READY
					 || mItem->getState() == GOS_HELD)
			{
                    LOG_WARNING("FetchItemJob", "Target item is held by someone");
                    return true;
            }

            return false;
		}
        else 
        {
            // Hard set orientation.
            /// @todo Use CreatureController properly to turn smoothly
            Vector3 pos = mActor->getCreature()->getPosition();
            pos.y = 0;
            targetPos.y = 0;
            mActor->getCreature()->setOrientation(Vector3::NEGATIVE_UNIT_Z.getRotationTo(targetPos - pos));
            mActor->setMovement(CreatureController::MT_LAUFEN, Vector3::NEGATIVE_UNIT_Z, Vector3::ZERO);
            
            mTimeLeft  -= time;
            return false;            
        }

	}
}
