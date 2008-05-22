/**
 * OpenSteer -- Steering Behaviors for Autonomous Characters
 *
 * Copyright (c) 2002-2005, Sony Computer Entertainment America
 * Original author: Craig Reynolds <craig_reynolds@playstation.sony.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 *
 * @file
 *
 * @author Bjoern Knafla <bknafla@uni-kassel.de>
 *
 * Segmented path build of polylines.
 */
#ifndef OPENSTEER_POLYLINESEGMENTEDPATH_H
#define OPENSTEER_POLYLINESEGMENTEDPATH_H

// Include std::vector
#include <vector>



// Include OpenSteer::SegmentedPath
#include "OpenSteer/SegmentedPath.h"

// Include OpenSteer::PointToPathAlikeBaseDataExtractionPolicy
#include "OpenSteer/QueryPathAlikeBaseDataExtractionPolicies.h"

// Include Vector3
#include "OpenSteer/Vec3.h"

// Include OpenSteer::distance
#include "OpenSteer/Vec3Utilities.h"



namespace OpenSteer {
    
    
    /**
     * Segmented path build by polylines. The last point of the path might be 
     * connected to the first point building a closed cycle.
     *
     * 
     */
    class PolylineSegmentedPath : public SegmentedPath {
    public:
        typedef std::vector< Vector3 > PathPoints;
        /**
         * Constructs an invalid path. Behavior of most member functions is
         * undefined if a path has less than two distinct points.
         */
        PolylineSegmentedPath();
        
        /**
         * Constructs a new path.
         *
         * @param numOfPoints Number of points. Must be at least two.
         * @param newPoints As many points as indicated by @a numOfPoints. Two
         *                  adjacent points mustn't be identical and the first
         *                  the last point mustn't be identical.
         * @param closedCycle If @c true the first point of @a newPoints is
         *                    copied to the end of the path to represent the 
         *                    cycle closing segment.
         */
        PolylineSegmentedPath( size_type numOfPoints,
                               Vector3 const newPoints[],
                               bool closedCycle );

        PolylineSegmentedPath( const PathPoints& newPoints,
                               bool closedCycle );
        
        PolylineSegmentedPath( PolylineSegmentedPath const& other );
        
        virtual ~PolylineSegmentedPath();
        
        PolylineSegmentedPath& operator=( PolylineSegmentedPath other );
        
        void swap( PolylineSegmentedPath& other );
        
        
        /**
         * Replaces all path information by the given ones.
         *
         * @param numOfPoints Number of points. Must be at least two.
         * @param newPoints As many points as indicated by @a numOfPoints. Two
         *                  adjacent points mustn't be identical and the first
         *                  the last point mustn't be identical.
         * @param closedCycle If @c true the first point of @a newPoints is
         *                    copied to the end of the path to represent the 
         *                    cycle closing segment.
         */
        void setPath( size_type numOfPoints,
                      Vector3 const newPoints[],
                      bool closedCycle );

        void setPath( const PathPoints& newPoints, bool closedCycle);
        /**
         * Replaces @a numOfPoints points starting at @a startIndex.
         *
         * In the resulting sequence of points there mustn't be two adjacent 
         * ones that are equal. The first and last point mustn't be identical,
         * too.
         *
         * If the first point is changed and the path is cyclic the duplication
         * of the first point at the end of the sequence representing the
         * path closing segment is updated automatically.
         *
         * @param startIndex First point to be moved or replaced.
         * @param numOfPoints Number of points to move or replace. 
         *                    <code> numOfPoints + startIndex </code> must be
         *                    lesser or equal to @c pointCount.
         * @param newPoints Moved points to replace the old ones.
         */
        void movePoints( size_type startIndex,
                         size_type numOfPoints,
                         Vector3 const newPoints[]);
        
        
        
        virtual bool isValid() const;
        virtual Vector3 mapPointToPath (const Vector3& point,
                                     Vector3& tangent,
                                     float& outside) const;
		virtual Vector3 mapPathDistanceToPoint (float pathDistance) const;
		virtual float mapPointToPathDistance (const Vector3& point) const;
        virtual bool isCyclic() const;
        virtual float length() const;
        
        
        virtual size_type pointCount() const;
        virtual Vector3 point( size_type pointIndex ) const;        
        
        
        virtual size_type segmentCount() const;
        virtual float segmentLength( size_type segmentIndex ) const;
        virtual Vector3 segmentStart( size_type segmentIndex ) const;
        virtual Vector3 segmentEnd( size_type segmentIndex ) const;
        virtual float mapPointToSegmentDistance( size_type segmentIndex, 
                                                 Vector3 const& point ) const;
        virtual Vector3 mapSegmentDistanceToPoint( size_type segmentIndex, 
                                                float segmentDistance ) const;
        virtual Vector3 mapSegmentDistanceToTangent( size_type segmentIndex, 
                                                  float segmentDistance ) const;
        
        virtual void mapDistanceToSegmentPointAndTangent( size_type segmentIndex,
                                                          float distance,
                                                          Vector3& pointOnPath,
                                                          Vector3& tangent ) const;
        
        virtual void mapPointToSegmentDistanceAndPointAndTangent( size_type segmentIndex,
                                                                  Vector3 const& point,
                                                                  float& distance,
                                                                  Vector3& pointOnPath,
                                                                  Vector3& tangent ) const;
        
    private:
        std::vector< Vector3 > points_;
        std::vector< Vector3 > segmentTangents_;
        std::vector< float > segmentLengths_;
        bool closedCycle_;
    }; // class PolylineSegmentedPath
    
    
    /**
     * Swaps the content of @a lhs and @a rhs.
     */
    inline void swap( PolylineSegmentedPath& lhs, PolylineSegmentedPath& rhs ) {
        lhs.swap( rhs );
    }
    
    
    /**
     * Extracts the base data of @c PolylineSegmentedPath.
     */
    template<>
    class PointToPathAlikeBaseDataExtractionPolicy< PolylineSegmentedPath > {
    public:
        
        static void extract( PolylineSegmentedPath const& pathAlike,
                             PolylineSegmentedPath::size_type segmentIndex,
                             Vector3 const& point, 
                             float& segmentDistance, 
                             float&, 
                             float& distancePointToPath, 
                             Vector3& pointOnPathCenterLine, 
                             Vector3& tangent ) {
            pathAlike.mapPointToSegmentDistanceAndPointAndTangent( segmentIndex, point, segmentDistance, pointOnPathCenterLine, tangent );
            distancePointToPath = distance( point, pointOnPathCenterLine );
        }
        
    }; // class PointToPathAlikeBaseDataExtractionPolicy
    
    /**
     * Extracts the base data of @c PolylineSegmentedPath.
     */
    template<>
    class DistanceToPathAlikeBaseDataExtractionPolicy< PolylineSegmentedPath > {
    public:
        static void extract( PolylineSegmentedPath const& pathAlike,
                             PolylineSegmentedPath::size_type segmentIndex,
                             float segmentDistance, 
                             Vector3& pointOnPathCenterLine, 
                             Vector3& tangent, 
                             float&  )  {
            pathAlike.mapDistanceToSegmentPointAndTangent( segmentIndex, segmentDistance, pointOnPathCenterLine, tangent );     
        }
        
        
    }; // DistanceToPathAlikeBaseDataExtractionPolicy
    
    
    
} // namespace OpenSteer


#endif // OPENSTEER_POLYLINESEGMENTEDPATH_H
