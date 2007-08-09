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
#ifndef __RlAI_AStarHeuristic_H__
#define __RlAI_AStarHeuristic_H__

#include "AiPrerequisites.h"

namespace rl {

	

	/** Abstract heuristic class definining an interface for AStar to access.
	 * Contains the heuristic to calculate the distance cost to the goal
	 */
	class _RlAiExport AStarHeuristic
	{
	public:
		/** default constructor
		 */
		AStarHeuristic();
		/** explicit virtual destructor
		 */
		virtual ~AStarHeuristic();

		/** calculates the (remaining) distance between wp1 and wp2.
		 * @param vec1 start position in world space
		 * @param vec2 goal position in world space
		 * @returns a float giving the result of the heuristic distance calculation
		 */
		virtual float calcDistance(const Ogre::Vector3& vec1, const Ogre::Vector3& vec2) const = 0;
	};

	// a set of sample heuristics that can be used

	//! Manhatten distance heuristic
	class _RlAiExport ManhattanDistance
		: public AStarHeuristic
	{
	public:
		//! default constructor
		ManhattanDistance();
		//! explicit virtual destructor
		virtual ~ManhattanDistance();

		/** calculates the Manhatten distance of the given 2 points.
		 * @param vec1 start point in world space
		 * @param vec2 end point in world space
		 * @returns a float containing the Manhatten distance of the 2 points.
		 */
		virtual float calcDistance(const Ogre::Vector3& vec1, const Ogre::Vector3& vec2) const;
	};

	//! diagonal distance heuristic
	class _RlAiExport DiagonalDistance
		: public AStarHeuristic
	{
	public:
		//! default constructor
		DiagonalDistance();
		//! explicit virtual destructor
		virtual ~DiagonalDistance();

		/** calculates the of diagonal distance the given 2 points.
		 * @param vec1 start point in world space
		 * @param vec2 end point in world space
		 * @returns a float containing the diagonal distance of the 2 points.
		 */
		virtual float calcDistance(const Ogre::Vector3& vec1, const Ogre::Vector3& vec2) const;
	};

	//! euclidean distance heuristic
	class _RlAiExport EuclideanDistance
		: public AStarHeuristic
	{
	public:
		//! default constructor
		EuclideanDistance();
		//! explicit virtual destructor
		virtual ~EuclideanDistance();

		/** calculates the of the given 2 points.
		 * @param vec1 start point in world space
		 * @param vec2 end point in world space
		 * @returns a float containing the of the 2 points.
		 */
		virtual float calcDistance(const Ogre::Vector3& vec1, const Ogre::Vector3& vec2) const;
	};

	//! euclidian distance square heuristic
	class _RlAiExport EuclideanDistanceSquared
		: public AStarHeuristic
	{
	public:
		//! default constructor
		EuclideanDistanceSquared();
		//! explicit virtual destructor
		virtual ~EuclideanDistanceSquared();

		/** calculates the of the given 2 points.
		 * @param vec1 start point in world space
		 * @param vec2 end point in world space
		 * @returns a float containing the of the 2 points.
		 */
		virtual float calcDistance(const Ogre::Vector3& vec1, const Ogre::Vector3& vec2) const;
	};

}; // namespace

#endif
