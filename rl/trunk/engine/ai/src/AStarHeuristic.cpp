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
#include "AStarHeuristic.h"
#include "AStarWayPointNode.h"
#include "WayPointNode.h"

namespace rl {

AStarHeuristic::AStarHeuristic()
{
}

AStarHeuristic::~AStarHeuristic()
{
}

ManhattanDistance::ManhattanDistance()
{
}
ManhattanDistance::~ManhattanDistance()
{
}
float ManhattanDistance::calcDistance(const Ogre::Vector3& vec1, const Ogre::Vector3& vec2) const
{
	return (abs(vec1.x - vec2.x) + abs(vec1.y - vec2.y) + abs(vec1.z - vec2.z));
}
DiagonalDistance::DiagonalDistance()
{
}
DiagonalDistance::~DiagonalDistance()
{
}
float DiagonalDistance::calcDistance(const Ogre::Vector3& vec1, const Ogre::Vector3& vec2) const
{
	return std::max( std::max( abs(vec1.x - vec2.x), abs(vec1.y - vec2.y) ), abs(vec1.z - vec2.z) );
}
EuclideanDistance::EuclideanDistance()
{
}
EuclideanDistance::~EuclideanDistance()
{
}
float EuclideanDistance::calcDistance(const Ogre::Vector3& vec1, const Ogre::Vector3& vec2) const
{
    return vec1.distance( vec2);
}
EuclideanDistanceSquared::EuclideanDistanceSquared()
{
}
EuclideanDistanceSquared::~EuclideanDistanceSquared()
{
}
float EuclideanDistanceSquared::calcDistance(const Ogre::Vector3& vec1, const Ogre::Vector3& vec2) const
{
	return vec1.squaredDistance( vec2);
}

};
