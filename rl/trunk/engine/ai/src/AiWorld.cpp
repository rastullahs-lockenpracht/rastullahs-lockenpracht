/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#include "AiWorld.h"
#include "PhysicsManager.h"
#include "LineSetPrimitive.h"
#include "CoreSubsystem.h"
#include <OgreNewt.h>
#include "SteeringVehicle.h"
#include "Actor.h"
#include "PhysicalThing.h"
#include "OgreVector3.h"
#include "PhysicsMaterialRaycast.h"
#include "World.h"

using namespace OpenSteer;
using namespace Ogre;

namespace rl {

AiWorld::AiWorld(void) : mObstacles()
{
//  create an obstacle as bounding box of the walkarea for npcs
//  this should be accessable through scripting, the Obstacles should have names
//  for easier access
/*	BoxObstacle* o = new BoxObstacle(25,50,25);
	o->setSeenFrom(AbstractObstacle::inside);
	o->setPosition(Vector3(-40.0f,-10.0f, 0.0f));
	o->setForward(0,0,-1);
	addObstacle(o);

	o = new BoxObstacle(2,50,2);
	o->setSeenFrom(AbstractObstacle::outside);
	o->setPosition(Vector3(-31.5f,-10.0f, -3.5f));
	o->setForward(0,0,-1);
	addObstacle(o); */

}

AiWorld::~AiWorld(void)
{
	removeAllObstacles();
}

void AiWorld::addObstacle(Obstacle* obstacle)
{
	mObstacles.push_back(obstacle);
}

const ObstacleGroup& AiWorld::getSteeringObstacles()
{
	return mObstacles;
}

void AiWorld::removeAllObstacles()
{
	ObstacleIterator itr = mObstacles.begin();
	for(; itr != mObstacles.end(); ++itr)
	{
		delete (*itr);
	}
	mObstacles.clear();
}

NewtonWorldAsObstacle::NewtonWorldAsObstacle(void)
{
    mMaterialsToConsider.push_back(PhysicsManager::getSingleton().getMaterialID("level"));
    mMaterialsToConsider.push_back(PhysicsManager::getSingleton().getMaterialID("default"));
    mNewtonWorld = PhysicsManager::getSingleton()._getNewtonWorld();
}

void NewtonWorldAsObstacle::findIntersectionWithVehiclePath (
    const AbstractVehicle& vehicle,
    PathIntersection& pi) const
{
    static PhysicsMaterialRaycast materialRaycast; // static, damit das nicht jedesmal neu erstellt werden muss

    pi.intersect = false;
    RaycastInfo info;
    RaycastInfo infoNearest;
    // versuche bis zu 5 raycasts mit der Mitte und den "Ecken:" oben, unten, links, rechts!
    // Dadurch ist ungefaehr der Umriss gesichert!
    // Habe bisher keine elegantere Loesung gefunden
    RaycastType raycastType;
    Vector3 pos = vehicle.getPosition();
    Vector3 futPos = vehicle.getForward() * vehicle.getSpeed() * 3 + pos;


    if (pos == futPos)
        return;

    // zur Hilfe bei der Berechnung
    Ogre::Vector3 velocityNorm(futPos - pos);
    velocityNorm.normalise(); // normalisierte geschwindigkeit (Richtung!)


    // Versuchen wir mal das Casten, um das urspruengliche SteelingVehicle zu kriegen
    const SteeringVehicle *steerVec = static_cast<const SteeringVehicle *> (&vehicle);
    // falls nich geklappt, ist das Ergebnis NULL, das wird spaeter abgefragt!

    if( steerVec != NULL )
    {
        pos = pos + Ogre::Vector3::UNIT_Y*steerVec->getHeight()/2.0;
        futPos = futPos + Ogre::Vector3::UNIT_Y*steerVec->getHeight()/2.0;
    }


    RaycastType nearestRayCast(NONE);
    infoNearest.mDistance = 2;
    for( int i = 0; i < 5; i++ )
    {
        Ogre::Vector3 castPos, castFutPos;  // die Koordinaten, die zum Casten verwendet werden

        raycastType = static_cast<RaycastType>(i);
        AxisAlignedBox aab;


        if( raycastType == MIDDLE ) // immer Ausfuehren
        {
            castPos = pos;
            castFutPos = futPos;
        }
        else if( raycastType == RIGHT )
        {
            // so wir ham ne normale und machen uns nen Y
            if( vehicle.getRadius() == 0 )
                continue;
            Vector3 offset = vehicle.getSide().normalisedCopy() * (vehicle.getRadius() - 0.1);
            castPos = pos + offset;
            castFutPos = futPos + offset;
        }
        else if( raycastType == LEFT )
        {
            // wie bei right ...
            if( vehicle.getRadius() == 0 )
                continue;
            Vector3 offset = vehicle.getSide().normalisedCopy() * (vehicle.getRadius() - 0.1);
            castPos = pos - offset;
            castFutPos = futPos - offset;
        }
        else
        {
            if( steerVec == NULL ) // hier abbrechen
                break;
            float height = steerVec->getHeight();
            if( height == 0 )
                break;
            if( raycastType == TOP )
            {
                castPos = pos;
                castPos.y += height*0.5;
                castFutPos = futPos;
                castFutPos.y += height*0.5;
            }
            else if( raycastType == BOTTOM )
            {
                castPos = pos;
                castPos.y -= height*0.5-0.4;
                castFutPos = futPos;
                castFutPos.y -= height*0.5-0.4;
            }
        }

        if( DebugVisualisable::isShown() )
            mDebugRaycasts.push_back(std::make_pair(castPos,castFutPos));


        // so alles richtig gesetzt!
        info = materialRaycast.execute(
            mNewtonWorld,
            &mMaterialsToConsider,
            castPos,
            castFutPos);


        if( info.mBody )
        {
            if( !info.mNormal.directionEquals(Ogre::Vector3::UNIT_Y, Degree(5)) )
            {
                nearestRayCast = raycastType;
                if( info.mDistance < infoNearest.mDistance )
                    infoNearest = info;
            }
        }
    }



    switch(nearestRayCast)
    {
    case NONE:
        return;
    default:
        pi.intersect = true;
        pi.obstacle = this;
        pi.distance = infoNearest.mDistance * (futPos - pos).length();
        pi.surfacePoint =
        vehicle.getPosition() + (vehicle.getForward() * pi.distance);

        // Die normale muss zu uns zeigen! koennte aber auch in die entgegengesetzte Richtung sein!
        pi.surfaceNormal = Vector3(infoNearest.mNormal.x, infoNearest.mNormal.y, infoNearest.mNormal.z);
/*        pi.surfaceNormal.normalisedCopy();
        // jedenfalls gilt dafuer, dass der abstand zwischen surfacePoint+surfaceNormal und _pos minimal sein muss
        if( ((pi.surfacePoint + pi.surfaceNormal) - _pos).length() >
            ((pi.surfacePoint - pi.surfaceNormal) - _pos).length() )
            pi.surfaceNormal = -pi.surfaceNormal;
*/        pi.vehicleOutside = true; // egal?

        // do we need different handling for right and left?
        pi.steerHint = pi.surfaceNormal;
        return;
    }
}



// methods from debugvisualisable
DebugVisualisableFlag NewtonWorldAsObstacle::getFlag() const
{
    return DVF_BOT;
}

void NewtonWorldAsObstacle::updatePrimitive()
{
    if( mSceneNode->getParent() == NULL )
    {
        SceneManager* mgr =
            CoreSubsystem::getSingletonPtr()->getWorld()->getSceneManager();
        mgr->getRootSceneNode()->addChild(mSceneNode);
    }

    LineSetPrimitive* lineSet = static_cast<LineSetPrimitive*>(mPrimitive);
    lineSet->clear();
    for( size_t i = 0; i < mDebugRaycasts.size(); i++)
        lineSet->addLine(mDebugRaycasts[i].first, mDebugRaycasts[i].second, Ogre::ColourValue::Blue);
    mDebugRaycasts.clear();
}

void NewtonWorldAsObstacle::doCreatePrimitive()
{
    mPrimitive = new LineSetPrimitive();
}


}
