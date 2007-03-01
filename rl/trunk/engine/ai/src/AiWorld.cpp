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
#include "AiWorld.h"
#include "PhysicsManager.h"
#include <OgreNewt.h>
#include "SteeringVehicle.h"
#include "Actor.h"
#include "PhysicalThing.h"
#include <Ogre.h>
#include "PhysicsMaterialRaycast.h"

using namespace rl;
using namespace OpenSteer;

AiWorld::AiWorld(void) : mObstacles()
{
//  create an obstacle as bounding box of the walkarea for npcs
//  this should be accessable through scripting, the Obstacles should have names
//  for easier access
/*	BoxObstacle* o = new BoxObstacle(25,50,25);
	o->setSeenFrom(AbstractObstacle::inside);
	o->setPosition(Vec3(-40.0f,-10.0f, 0.0f));
	o->setForward(0,0,-1);
	addObstacle(o);

	o = new BoxObstacle(2,50,2);
	o->setSeenFrom(AbstractObstacle::outside);
	o->setPosition(Vec3(-31.5f,-10.0f, -3.5f));
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

ObstacleGroup AiWorld::getSteeringObstacles()
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



void NewtonWorldAsObstacle::findIntersectionWithVehiclePath (
    const AbstractVehicle& vehicle,
    PathIntersection& pi) const
{
    static PhysicsMaterialRaycast materialRaycast; // static, damit das nicht jedesmal neu erstellt werden muss

    pi.intersect = false;
    RaycastInfo info;
    // versuche bis zu 5 raycasts mit der Mitte und den "Ecken:" oben, unten, links, rechts!
    // Dadurch ist ungef�hr der Umriss gesichert!
    // Habe bisher keine elegantere L�sung gefunden
    RaycastType raycastType;
    Vec3 _pos = vehicle.position();
    Vec3 _futPos = vehicle.forward() * 2 + _pos;


    if( _pos == _futPos )
        return;

    Ogre::Vector3 pos (_pos.x, _pos.y, _pos.z);
    Ogre::Vector3 futPos (_futPos.x, _futPos.y, _futPos.z);

    // zur Hilfe bei der Berechnung
    Ogre::Vector3 velocityNorm(futPos - pos);
    velocityNorm.normalise(); // normalisierte geschwindigkeit (Richtung!)


    // Versuchen wir mal das Casten, um das urspr�ngliche SteelingVehicle zu kriegen
    const SteeringVehicle *steerVec = dynamic_cast<const SteeringVehicle *> (&vehicle);
    // falls nich geklappt, ist das Ergebnis NULL, das wird sp�ter abgefragt!


    for( int i = 0; i < 5; i++ )
    {
        Ogre::Vector3 castPos, castFutPos;  // die Koordinaten, die zum Casten verwendet werden

        raycastType = static_cast<RaycastType>(i);
        AxisAlignedBox aab;


        if( raycastType == MIDDLE ) // immer Ausf�hren
        {
            castPos = pos;
            castFutPos = futPos;
        }
        else if( raycastType == RIGHT )
        {
            // so wir ham ne normale und machen uns nen Y
            if( vehicle.radius() == 0 )
                continue;
            Ogre::Vector3 offset(velocityNorm.crossProduct(Ogre::Vector3::UNIT_Y));
            offset = offset.normalise() * vehicle.radius();
            castPos = pos + offset;
            castFutPos = futPos + offset;
        }
        else if( raycastType == LEFT )
        {
            // wie bei right ...
            if( vehicle.radius() == 0 )
                continue;
            Ogre::Vector3 offset(velocityNorm.crossProduct(Ogre::Vector3::UNIT_Y));
            offset = offset.normalise() * vehicle.radius();
            castPos = pos - offset;
            castFutPos = futPos - offset;
        }
        else
        {
            if( steerVec == NULL ) // hier abbrechen
                break;
            float height = steerVec->height();
            if( height == 0 )
                break;
            if( raycastType == TOP )
            {
                castPos = pos;
                pos.y += height;
                castFutPos = futPos;
                futPos.y += height;
            }
            else if( raycastType == BOTTOM )
            {
                castPos = pos;
                pos.y -= height;
                castFutPos = futPos;
                futPos.y -= height;
            }
        }

        // so alles richtig gesetzt!
        info = materialRaycast.execute(
            mNewtonWorld,
            mLevelMaterial,
            castPos,
            castFutPos);


        if( info.mBody )
            break;
    }


    switch(raycastType)
    {
    case NONE:
        return;
    case MIDDLE:
    default:
        pi.intersect = true;
        pi.obstacle = this;
        pi.distance = info.mDistance * (futPos - pos).length();
        pi.surfacePoint = _pos + (vehicle.forward() * pi.distance);

        // Die normale muss zu uns zeigen! k�nnte aber auch in die entgegengesetzte Richtung sein!
        pi.surfaceNormal = Vec3(info.mNormal.x, info.mNormal.y, info.mNormal.z);
        pi.surfaceNormal.normalize();
        // jedenfalls gilt daf�r, dass der abstand zwischen surfacePoint+surfaceNormal und _pos minimal sein muss
        if( ((pi.surfacePoint + pi.surfaceNormal) - _pos).length() > 
            ((pi.surfacePoint - pi.surfaceNormal) - _pos).length() )
            pi.surfaceNormal = -pi.surfaceNormal;
        pi.vehicleOutside = true; // egal?
        pi.steerHint = pi.surfaceNormal;
        //pi.steerHint.normalize();
        return;
    }
}
