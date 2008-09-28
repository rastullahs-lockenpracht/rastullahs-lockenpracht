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

#ifndef __CreatureWalkPathJob_H__
#define __CreatureWalkPathJob_H__

#include "AiPrerequisites.h"

#include "Landmark.h"
#include "LandmarkPath.h"

#include "AStar.h"
#include "CreatureController.h"
#include "Job.h"

namespace rl
{
    class WayPointGraph;

    class _RlAiExport CreatureWalkPathJob : public Job
    {
    public:
        static const CeGuiString PROPERTY_CURRENTLANDMARK;
        static const CeGuiString PROPERTY_NEXTLANDMARKS;
        static const CeGuiString PROPERTY_GAMEOBJECTID;
       /** Constructor
        * @param name the name of the Job
        * @param movingCreature the job has to know a moving creature
        * @param startLandmark first landmark, the moving creature has to go to
        */
        CreatureWalkPathJob(const Ogre::String& name, Creature* movingCreature, Landmark* startLandmark);
        //destructor
        virtual ~CreatureWalkPathJob();

        //adds an additional landmark the moving creature has to go to
        void addLandmark(const Ogre::String& name, const Ogre::Vector3& position);
        void addLandmark(Landmark* lm);
        void setWayPoints(const WayPointGraph* wps);

       /**
        * This function is not intented to be called directly
        * @retval false, if the job is not done
        */
        virtual bool execute(Ogre::Real time);

        // @retval the moving creature controlled by this object
        Creature* getCreature();

        /// derived from PropertyHolder
        virtual const Property getProperty(const CeGuiString& key) const;
        /// derived from PropertyHolder
        virtual void setProperty(const CeGuiString& key, const Property& value);
        /// derived from PropertyHolder
        virtual PropertyKeys getAllPropertyKeys() const;

        const CeGuiString getClassName() const {return "CreatureWalkPathJob";}
    private:

        LandmarkPath mLandmarkPath;
        Ogre::String mName;
        Creature* mMovingCreature;

        Landmark* mCurrentLandmark;
        Landmark* mNextLandmark;
        const WayPointGraph* mWayPoints;
        AStar::AStarPath mCurrentWayPath;
        Ogre::Real mTimeSinceLastRotation;
        bool mUpdatedDirection;
    };

    //CreatureWalkPathJob* createSavedCreateWalkPathJob();
}

#endif
