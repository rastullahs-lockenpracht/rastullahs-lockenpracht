// ----------------------------------------------------------------------------
//
//
// OpenSteer -- Steering Behaviors for Autonomous Characters
//
// Copyright (c) 2002-2005, Sony Computer Entertainment America
// Original author: Craig Reynolds <craig_reynolds@playstation.sony.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
//
// ----------------------------------------------------------------------------
//
//
// Capture the Flag   (a portion of the traditional game)
//
// The "Capture the Flag" sample steering problem, proposed by Marcin
// Chady of the Working Group on Steering of the IGDA's AI Interface
// Standards Committee (http://www.igda.org/Committees/ai.htm) in this
// message (http://sourceforge.net/forum/message.php?msg_id=1642243):
//
//     "An agent is trying to reach a physical location while trying
//     to stay clear of a group of enemies who are actively seeking
//     him. The environment is littered with obstacles, so collision
//     avoidance is also necessary."
//
// Note that the enemies do not make use of their knowledge of the 
// seeker's goal by "guarding" it.  
//
// XXX hmm, rename them "attacker" and "defender"?
//
// 08-12-02 cwr: created 
//
//
// ----------------------------------------------------------------------------


#include <iomanip>
#include <sstream>
#include "OpenSteer/SimpleVehicle.h"
#include "OpenSteer/OpenSteerDemo.h"
#include "OpenSteer/Color.h"

namespace {

    using namespace OpenSteer;


    // ----------------------------------------------------------------------------
    // short names for STL vectors (iterators) of SphereObstacle pointers
    // (obsolete? replace with ObstacleGroup/ObstacleIterator ?)


    typedef std::vector<SphereObstacle*> SOG;  // SphereObstacle group
    typedef SOG::const_iterator SOI;           // SphereObstacle iterator


    // ----------------------------------------------------------------------------
    // This PlugIn uses two vehicle types: CtfSeeker and CtfEnemy.  They have a
    // common base class: CtfBase which is a specialization of SimpleVehicle.


    class CtfBase : public SimpleVehicle
    {
    public:
        // constructor
        CtfBase () {reset ();}

        // reset state
        void reset (void);

        // draw this character/vehicle into the scene
        void draw (void);

        // annotate when actively avoiding obstacles
        void annotateAvoidObstacle (const float minDistanceToCollision);

        void drawHomeBase (void);

        void randomizeStartingPositionAndHeading (void);
        enum seekerState {running, tagged, atGoal};

        // for draw method
        Color bodyColor;

        // xxx store steer sub-state for anotation
        bool avoiding;

        // dynamic obstacle registry
        static void initializeObstacles (void);
        static void addOneObstacle (void);
        static void removeOneObstacle (void);
        float minDistanceToObstacle (const Vector3 point);
        static int obstacleCount;
        static const int maxObstacleCount;
        static SOG allObstacles;
    };


    class CtfSeeker : public CtfBase
    {
    public:

        // constructor
        CtfSeeker () {reset ();}

        // reset state
        void reset (void);

        // per frame simulation update
        void update (const float currentTime, const float elapsedTime);

        // is there a clear path to the goal?
        bool clearPathToGoal (void);

        Vector3 steeringForSeeker (void);
        void updateState (const float currentTime);
        void draw (void);
        Vector3 steerToEvadeAllDefenders (void);
        Vector3 XXXsteerToEvadeAllDefenders (void);
        void adjustObstacleAvoidanceLookAhead (const bool clearPath);
        void clearPathAnnotation (const float threshold,
                                  const float behindcThreshold,
                                  const Vector3& goalDirection);

        seekerState state;
        bool evading; // xxx store steer sub-state for anotation
        float lastRunningTime; // for auto-reset
    };


    class CtfEnemy : public CtfBase
    {
    public:

        // constructor
        CtfEnemy () {reset ();}

        // reset state
        void reset (void);

        // per frame simulation update
        void update (const float currentTime, const float elapsedTime);
    };


    // ----------------------------------------------------------------------------
    // globals
    // (perhaps these should be member variables of a Vehicle or PlugIn class)


    const int CtfBase::maxObstacleCount = 100;

    const Vector3 gHomeBaseCenter (0, 0, 0);
    const float gHomeBaseRadius = 1.5;

    const float gMinStartRadius = 30;
    const float gMaxStartRadius = 40;

    const float gBrakingRate = 0.75;

    const Color evadeColor     (0.6f, 0.6f, 0.3f); // annotation
    const Color seekColor      (0.3f, 0.6f, 0.6f); // annotation
    const Color clearPathColor (0.3f, 0.6f, 0.3f); // annotation

    const float gAvoidancePredictTimeMin  = 0.9f;
    const float gAvoidancePredictTimeMax  = 2;
    float gAvoidancePredictTime = gAvoidancePredictTimeMin;

    bool enableAttackSeek  = true; // for testing (perhaps retain for UI control?)
    bool enableAttackEvade = true; // for testing (perhaps retain for UI control?)

    CtfSeeker* gSeeker = NULL;


    // count the number of times the simulation has reset (e.g. for overnight runs)
    int resetCount = 0;


    // ----------------------------------------------------------------------------
    // state for OpenSteerDemo PlugIn
    //
    // XXX consider moving this inside CtfPlugIn
    // XXX consider using STL (any advantage? consistency?)


    CtfSeeker* ctfSeeker;
    const int ctfEnemyCount = 4;
    CtfEnemy* ctfEnemies [ctfEnemyCount];


    // ----------------------------------------------------------------------------
    // reset state


    void CtfBase::reset (void)
    {
        SimpleVehicle::reset ();  // reset the vehicle 

        setSpeed (3);             // speed along Forward direction.
        setMaxForce (3.0);        // steering force is clipped to this magnitude
        setMaxSpeed (3.0);        // velocity is clipped to this magnitude

        avoiding = false;         // not actively avoiding

        randomizeStartingPositionAndHeading ();  // new starting position

        clearTrailHistory ();     // prevent long streaks due to teleportation
    }


    void CtfSeeker::reset (void)
    {
        CtfBase::reset ();
        bodyColor = Color(Vector3(0.4f, 0.4f, 0.6f)); // blueish
        gSeeker = this;
        state = running;
        evading = false;
    }


    void CtfEnemy::reset (void)
    {
        CtfBase::reset ();
        bodyColor = Color(Vector3(0.6f, 0.4f, 0.4f)); // redish
    }


    // ----------------------------------------------------------------------------
    // draw this character/vehicle into the scene


    void CtfBase::draw (void)
    {
        drawBasic2dCircularVehicle (*this, bodyColor);
        drawTrail ();
    }


    // ----------------------------------------------------------------------------


    void CtfBase::randomizeStartingPositionAndHeading (void)
    {
        // randomize position on a ring between inner and outer radii
        // centered around the home base
        const float rRadius = frandom2 (gMinStartRadius, gMaxStartRadius);
        const Vector3 randomOnRing = RandomUnitVectorOnXZPlane () * rRadius;
        setPosition (gHomeBaseCenter + randomOnRing);

        // are we are too close to an obstacle?
        if (minDistanceToObstacle (getPosition()) < radius()*5)
        {
            // if so, retry the randomization (this recursive call may not return
            // if there is too little free space)
            randomizeStartingPositionAndHeading ();
        }
        else
        {
            // otherwise, if the position is OK, randomize 2D heading
            randomizeHeadingOnXZPlane ();
        }
    }


    // ----------------------------------------------------------------------------


    void CtfEnemy::update (const float currentTime, const float elapsedTime)
    {
        // determine upper bound for pursuit prediction time
        const float seekerToGoalDist = Vector3::distance (gHomeBaseCenter,
                                                       gSeeker->getPosition());
        const float adjustedDistance = seekerToGoalDist - radius()-gHomeBaseRadius;
        const float seekerToGoalTime = ((adjustedDistance < 0 ) ?
                                        0 :
                                        (adjustedDistance/gSeeker->getSpeed()));
        const float maxPredictionTime = seekerToGoalTime * 0.9f;

        // determine steering (pursuit, obstacle avoidance, or braking)
        Vector3 steer (0, 0, 0);
        if (gSeeker->state == running)
        {
            const Vector3 avoidance =
                steerToAvoidObstacles (gAvoidancePredictTimeMin,
                                       (ObstacleGroup&) allObstacles);

            // saved for annotation
            avoiding = (avoidance == Vector3::ZERO);

            if (avoiding)
                steer = steerForPursuit (*gSeeker, maxPredictionTime);
            else
                steer = avoidance;
        }
        else
        {
            applyBrakingForce (gBrakingRate, elapsedTime);
        }
        applySteeringForce (steer, elapsedTime);

        // annotation
        annotationVelocityAcceleration ();
        recordTrailVertex (currentTime, getPosition());


        // detect and record interceptions ("tags") of seeker
        const float seekerToMeDist = Vector3::distance (getPosition(), 
                                                     gSeeker->getPosition());
        const float sumOfRadii = radius() + gSeeker->radius();
        if (seekerToMeDist < sumOfRadii)
        {
            if (gSeeker->state == running) gSeeker->state = tagged;

            // annotation:
            if (gSeeker->state == tagged)
            {
                const Color color (0.8f, 0.5f, 0.5f);
                annotationXZDisk (sumOfRadii,
                            (getPosition() + gSeeker->getPosition()) / 2,
                            color,
                            20);
            }
        }
    }


    // ----------------------------------------------------------------------------
    // are there any enemies along the corridor between us and the goal?


    bool CtfSeeker::clearPathToGoal (void)
    {
        const float sideThreshold = radius() * 8.0f;
        const float behindThreshold = radius() * 2.0f;

        const Vector3 goalOffset = gHomeBaseCenter - getPosition();
        const float goalDistance = goalOffset.length ();
        const Vector3 goalDirection = goalOffset / goalDistance;

        const bool goalIsAside = isAside (gHomeBaseCenter, 0.5);

        // for annotation: loop over all and save result, instead of early return 
        bool xxxReturn = true;

        // loop over enemies
        for (int i = 0; i < ctfEnemyCount; i++)
        {
            // short name for this enemy
            const CtfEnemy& e = *ctfEnemies[i];
            const float eDistance = Vector3::distance (getPosition(), e.getPosition());
            const float timeEstimate = 0.3f * eDistance / e.getSpeed(); //xxx
            const Vector3 eFuture = e.predictFuturePosition (timeEstimate);
            const Vector3 eOffset = eFuture - getPosition();
            const float alongCorridor = goalDirection.dotProduct(eOffset);
            const bool inCorridor = ((alongCorridor > -behindThreshold) && 
                                     (alongCorridor < goalDistance));
            const float eForwardDistance = getForward().dotProduct(eOffset);

            // xxx temp move this up before the conditionals
            annotationXZCircle (e.getRadius(), eFuture, clearPathColor, 20); //xxx

            // consider as potential blocker if within the corridor
            if (inCorridor)
            {
                const Vector3 perp = eOffset - (goalDirection * alongCorridor);
                const float acrossCorridor = perp.length();
                if (acrossCorridor < sideThreshold)
                {
                    // not a blocker if behind us and we are perp to corridor
                    const float eFront = eForwardDistance + e.getRadius();

                    //annotationLine (position, forward*eFront, gGreen); // xxx
                    //annotationLine (e.position, forward*eFront, gGreen); // xxx

                    // xxx
                    // std::ostringstream message;
                    // message << "eFront = " << std::setprecision(2)
                    //         << std::setiosflags(std::ios::fixed) << eFront << std::ends;
                    // draw2dTextAt3dLocation (*message.str(), eFuture, gWhite);

                    const bool eIsBehind = eFront < -behindThreshold;
                    const bool eIsWayBehind = eFront < (-2 * behindThreshold);
                    const bool safeToTurnTowardsGoal =
                        ((eIsBehind && goalIsAside) || eIsWayBehind);

                    if (! safeToTurnTowardsGoal)
                    {
                        // this enemy blocks the path to the goal, so return false
                        annotationLine (getPosition(), e.getPosition(), clearPathColor);
                        // return false;
                        xxxReturn = false;
                    }
                }
            }
        }

        // no enemies found along path, return true to indicate path is clear
        // clearPathAnnotation (sideThreshold, behindThreshold, goalDirection);
        // return true;
        //if (xxxReturn)
            clearPathAnnotation (sideThreshold, behindThreshold, goalDirection);
        return xxxReturn;
    }


    // ----------------------------------------------------------------------------


    void CtfSeeker::clearPathAnnotation (const float sideThreshold,
                                         const float behindThreshold,
                                         const Vector3& goalDirection)
    {
        const Vector3 behindSide = getSide() * sideThreshold;
        const Vector3 behindBack = getForward() * -behindThreshold;
        const Vector3 pbb = getPosition() + behindBack;
        const Vector3 gun = localRotateForwardToSide (goalDirection);
        const Vector3 gn = gun * sideThreshold;
        const Vector3 hbc = gHomeBaseCenter;
        annotationLine (pbb + gn,         hbc + gn,         clearPathColor);
        annotationLine (pbb - gn,         hbc - gn,         clearPathColor);
        annotationLine (hbc - gn,         hbc + gn,         clearPathColor);
        annotationLine (pbb - behindSide, pbb + behindSide, clearPathColor);
    }


    // ----------------------------------------------------------------------------
    // xxx perhaps this should be a call to a general purpose annotation
    // xxx for "local xxx axis aligned box in XZ plane" -- same code in in
    // xxx Pedestrian.cpp


    void CtfBase::annotateAvoidObstacle (const float minDistanceToCollision)
    {
        const Vector3 boxSide = getSide() * radius();
        const Vector3 boxFront = getForward() * minDistanceToCollision;
        const Vector3 FR = getPosition() + boxFront - boxSide;
        const Vector3 FL = getPosition() + boxFront + boxSide;
        const Vector3 BR = getPosition()            - boxSide;
        const Vector3 BL = getPosition()            + boxSide;
        const Color white (1,1,1);
        annotationLine (FR, FL, white);
        annotationLine (FL, BL, white);
        annotationLine (BL, BR, white);
        annotationLine (BR, FR, white);
    }


    // ----------------------------------------------------------------------------


    Vector3 CtfSeeker::steerToEvadeAllDefenders (void)
    {
        Vector3 evade (0, 0, 0);
        const float goalDistance = Vector3::distance (gHomeBaseCenter, getPosition());

        // sum up weighted evasion
        for (int i = 0; i < ctfEnemyCount; i++)
        {
            const CtfEnemy& e = *ctfEnemies[i];
            const Vector3 eOffset = e.getPosition() - getPosition();
            const float eDistance = eOffset.length();

            const float eForwardDistance = getForward().dotProduct(eOffset);
            const float behindThreshold = radius() * 2;
            const bool behind = eForwardDistance < behindThreshold;
            if ((!behind) || (eDistance < 5))
            {
                if (eDistance < (goalDistance * 1.2)) //xxx
                {
                    // const float timeEstimate = 0.5f * eDistance / e.speed;//xxx
                    const float timeEstimate = 0.15f * eDistance / e.getSpeed();//xxx
                    const Vector3 future =
                        e.predictFuturePosition (timeEstimate);

                    annotationXZCircle (e.getRadius(), future, evadeColor, 20); // xxx

                    const Vector3 offset = future - getPosition();
                    const Vector3 lateral = offset.perpendicularComponent (getForward());
                    const float d = lateral.length();
                    const float weight = -1000 / (d * d);
                    evade += (lateral / d) * weight;
                }
            }
        }
        return evade;
    }


    Vector3 CtfSeeker::XXXsteerToEvadeAllDefenders (void)
    {
        // sum up weighted evasion
        Vector3 evade (0, 0, 0);
        for (int i = 0; i < ctfEnemyCount; i++)
        {
            const CtfEnemy& e = *ctfEnemies[i];
            const Vector3 eOffset = e.getPosition() - getPosition();
            const float eDistance = eOffset.length();

            // xxx maybe this should take into account e's heading? xxx
            const float timeEstimate = 0.5f * eDistance / e.getSpeed(); //xxx
            const Vector3 eFuture = e.predictFuturePosition (timeEstimate);

            // annotation
            annotationXZCircle (e.getRadius(), eFuture, evadeColor, 20);

            // steering to flee from eFuture (enemy's future position)
            const Vector3 flee = xxxsteerForFlee (eFuture);

            const float eForwardDistance = getForward().dotProduct(eOffset);
            const float behindThreshold = radius() * -2;

            const float distanceWeight = 4 / eDistance;
            const float forwardWeight = ((eForwardDistance > behindThreshold) ?
                                         1.0f : 0.5f);

            const Vector3 adjustedFlee = flee * distanceWeight * forwardWeight;

            evade += adjustedFlee;
        }
        return evade;
    }


    // ----------------------------------------------------------------------------


    Vector3 CtfSeeker::steeringForSeeker (void)
    {
        // determine if obstacle avodiance is needed
        const bool clearPath = clearPathToGoal ();
        adjustObstacleAvoidanceLookAhead (clearPath);
        const Vector3 obstacleAvoidance =
            steerToAvoidObstacles (gAvoidancePredictTime,
                                   (ObstacleGroup&) allObstacles);

        // saved for annotation
        avoiding = (obstacleAvoidance != Vector3::ZERO);

        if (avoiding)
        {
            // use pure obstacle avoidance if needed
            return obstacleAvoidance;
        }
        else
        {
            // otherwise seek home base and perhaps evade defenders
            const Vector3 seek = xxxsteerForSeek (gHomeBaseCenter);
            if (clearPath)
            {
                // we have a clear path (defender-free corridor), use pure seek

                // xxx experiment 9-16-02
                Vector3 s = limitMaxDeviationAngle (seek, 0.707f, getForward());

                annotationLine (getPosition(), getPosition() + (s * 0.2f), seekColor);
                return s;
            }
            else
            {
                if (0) // xxx testing new evade code xxx
                {
                    // combine seek and (forward facing portion of) evasion
                    const Vector3 evade = steerToEvadeAllDefenders ();
                    const Vector3 steer = 
                        seek + limitMaxDeviationAngle (evade, 0.5f, getForward());

                    // annotation: show evasion steering force
                    annotationLine (getPosition(),getPosition()+(steer*0.2f),evadeColor);
                    return steer;
                }
                else

                {
                    const Vector3 evade = XXXsteerToEvadeAllDefenders ();
                    const Vector3 steer = limitMaxDeviationAngle (seek + evade,
                                                               0.707f, getForward());

                    annotationLine (getPosition(),getPosition()+seek, gRed);
                    annotationLine (getPosition(),getPosition()+evade, gGreen);

                    // annotation: show evasion steering force
                    annotationLine (getPosition(),getPosition()+(steer*0.2f),evadeColor);
                    return steer;
                }
            }
        }
    }


    // ----------------------------------------------------------------------------
    // adjust obstacle avoidance look ahead time: make it large when we are far
    // from the goal and heading directly towards it, make it small otherwise.


    void CtfSeeker::adjustObstacleAvoidanceLookAhead (const bool clearPath)
    {
        if (clearPath)
        {
            evading = false;
            const float goalDistance = Vector3::distance (gHomeBaseCenter,getPosition());
            const bool headingTowardGoal = isAhead (gHomeBaseCenter, 0.98f);
            const bool isNear = (goalDistance/getSpeed()) < gAvoidancePredictTimeMax;
            const bool useMax = headingTowardGoal && !isNear;
            gAvoidancePredictTime =
                (useMax ? gAvoidancePredictTimeMax : gAvoidancePredictTimeMin);
        }
        else
        {
            evading = true;
            gAvoidancePredictTime = gAvoidancePredictTimeMin;
        }
    }


    // ----------------------------------------------------------------------------


    void CtfSeeker::updateState (const float currentTime)
    {
        // if we reach the goal before being tagged, switch to atGoal state
        if (state == running)
        {
            const float baseDistance = Vector3::distance (getPosition(),gHomeBaseCenter);
            if (baseDistance < (radius() + gHomeBaseRadius)) state = atGoal;
        }

        // update lastRunningTime (holds off reset time)
        if (state == running)
        {
            lastRunningTime = currentTime;
        }
        else
        {
            const float resetDelay = 4;
            const float resetTime = lastRunningTime + resetDelay;
            if (currentTime > resetTime) 
            {
                // xxx a royal hack (should do this internal to CTF):
                OpenSteerDemo::queueDelayedResetPlugInXXX ();
            }
        }
    }


    // ----------------------------------------------------------------------------


    void CtfSeeker::draw (void)
    {
        // first call the draw method in the base class
        CtfBase::draw();

        // select string describing current seeker state
        char* seekerStateString = "";
        switch (state)
        {
        case running:
            if (avoiding)
                seekerStateString = "avoid obstacle";
            else if (evading)
                seekerStateString = "seek and evade";
            else
                seekerStateString = "seek goal";
            break;
        case tagged: seekerStateString = "tagged"; break;
        case atGoal: seekerStateString = "reached goal"; break;
        }

        // annote seeker with its state as text
        const Vector3 textOrigin = getPosition() + Vector3 (0, 0.25, 0);
        std::ostringstream annote;
        annote << seekerStateString << std::endl;
        annote << std::setprecision(2) << std::setiosflags(std::ios::fixed)
               << getSpeed() << std::ends;
        draw2dTextAt3dLocation (annote, textOrigin, gWhite, drawGetWindowWidth(), drawGetWindowHeight());

        // display status in the upper left corner of the window
        std::ostringstream status;
        status << seekerStateString << std::endl;
        status << obstacleCount << " obstacles [F1/F2]" << std::endl;
        status << resetCount << " restarts" << std::ends;
        const float h = drawGetWindowHeight ();
        const Vector3 screenLocation (10, h-50, 0);
        draw2dTextAt2dLocation (status, screenLocation, gGray80, drawGetWindowWidth(), drawGetWindowHeight());
    }


    // ----------------------------------------------------------------------------
    // update method for goal seeker


    void CtfSeeker::update (const float currentTime, const float elapsedTime)
    {
        // do behavioral state transitions, as needed
        updateState (currentTime);

        // determine and apply steering/braking forces
        Vector3 steer (0, 0, 0);
        if (state == running)
        {
            steer = steeringForSeeker ();
        }
        else
        {
            applyBrakingForce (gBrakingRate, elapsedTime);
        }
        applySteeringForce (steer, elapsedTime);

        // annotation
        annotationVelocityAcceleration ();
        recordTrailVertex (currentTime, getPosition());
    }


    // ----------------------------------------------------------------------------
    // dynamic obstacle registry
    //
    // xxx need to combine guts of addOneObstacle and minDistanceToObstacle,
    // xxx perhaps by having the former call the latter, or change the latter to
    // xxx be "nearestObstacle": give it a position, it finds the nearest obstacle
    // xxx (but remember: obstacles a not necessarilty spheres!)


    int CtfBase::obstacleCount = -1; // this value means "uninitialized"
    SOG CtfBase::allObstacles;


    #define testOneObstacleOverlap(radius, center)               \
    {                                                            \
        float d = Vector3::distance (c, center);                    \
        float clearance = d - (r + (radius));                    \
        if (minClearance > clearance) minClearance = clearance;  \
    }


    void CtfBase::initializeObstacles (void)
    {
        // start with 40% of possible obstacles
        if (obstacleCount == -1)
        {
            obstacleCount = 0;
            for (int i = 0; i < (maxObstacleCount * 0.4); i++) addOneObstacle ();
        }
    }


    void CtfBase::addOneObstacle (void)
    {
        if (obstacleCount < maxObstacleCount)
        {
            // pick a random center and radius,
            // loop until no overlap with other obstacles and the home base
            float r;
            Vector3 c;
            float minClearance;
            const float requiredClearance = gSeeker->radius() * 4; // 2 x diameter
            do
            {
                r = frandom2 (1.5, 4);
                c = randomVectorOnUnitRadiusXZDisk () * gMaxStartRadius * 1.1f;
                minClearance = FLT_MAX;

                for (SOI so = allObstacles.begin(); so != allObstacles.end(); so++)
                {
                    testOneObstacleOverlap ((**so).radius, (**so).center);
                }

                testOneObstacleOverlap (gHomeBaseRadius - requiredClearance,
                                        gHomeBaseCenter);
            }
            while (minClearance < requiredClearance);

            // add new non-overlapping obstacle to registry
            allObstacles.push_back (new SphereObstacle (r, c));
            obstacleCount++;
        }
    }


    float CtfBase::minDistanceToObstacle (const Vector3 point)
    {
        float r = 0;
        Vector3 c = point;
        float minClearance = FLT_MAX;
        for (SOI so = allObstacles.begin(); so != allObstacles.end(); so++)
        {
            testOneObstacleOverlap ((**so).radius, (**so).center);
        }
        return minClearance;
    }


    void CtfBase::removeOneObstacle (void)
    {
        if (obstacleCount > 0)
        {
            obstacleCount--;
            allObstacles.pop_back();
        }
    }


    // ----------------------------------------------------------------------------
    // PlugIn for OpenSteerDemo


    class CtfPlugIn : public PlugIn
    {
    public:

        const char* name (void) {return "Capture the Flag";}

        float selectionOrderSortKey (void) {return 0.01f;}

        virtual ~CtfPlugIn() {} // be more "nice" to avoid a compiler warning

        void open (void)
        {
            // create the seeker ("hero"/"attacker")
            ctfSeeker = new CtfSeeker;
            all.push_back (ctfSeeker);

            // create the specified number of enemies, 
            // storing pointers to them in an array.
            for (int i = 0; i<ctfEnemyCount; i++)
            {
                ctfEnemies[i] = new CtfEnemy;
                all.push_back (ctfEnemies[i]);
            }

            // initialize camera
            OpenSteerDemo::init2dCamera (*ctfSeeker);
            OpenSteerDemo::camera.mode = Camera::cmFixedDistanceOffset;
            OpenSteerDemo::camera.fixedTarget = Vector3(15, 0, 0);
            OpenSteerDemo::camera.fixedPosition = Vector3(80, 60, 0);

            CtfBase::initializeObstacles ();
        }

        void update (const float currentTime, const float elapsedTime)
        {
            // update the seeker
            ctfSeeker->update (currentTime, elapsedTime);
          
            // update each enemy
            for (int i = 0; i < ctfEnemyCount; i++)
            {
                ctfEnemies[i]->update (currentTime, elapsedTime);
            }
        }

        void redraw (const float currentTime, const float elapsedTime)
        {
            // selected vehicle (user can mouse click to select another)
            AbstractVehicle& selected = *OpenSteerDemo::selectedVehicle;

            // vehicle nearest mouse (to be highlighted)
            AbstractVehicle& nearMouse = *OpenSteerDemo::vehicleNearestToMouse ();

            // update camera
            OpenSteerDemo::updateCamera (currentTime, elapsedTime, selected);

            // draw "ground plane" centered between base and selected vehicle
            const Vector3 goalOffset = gHomeBaseCenter-OpenSteerDemo::camera.getPosition();
            const Vector3 goalDirection = goalOffset.normalisedCopy();
            const Vector3 cameraForward = OpenSteerDemo::camera.xxxls().getForward();
            const float goalDot = cameraForward.dotProduct(goalDirection);
            const float blend = remapIntervalClip (goalDot, 1, 0, 0.5, 0);
            const Vector3 gridCenter = interpolate (blend,
                                                 selected.getPosition(),
                                                 gHomeBaseCenter);
            OpenSteerDemo::gridUtility (gridCenter);

            // draw the seeker, obstacles and home base
            ctfSeeker->draw();
            drawObstacles ();
            drawHomeBase();

            // draw each enemy
            for (int i = 0; i < ctfEnemyCount; i++) ctfEnemies[i]->draw ();

            // highlight vehicle nearest mouse
            OpenSteerDemo::highlightVehicleUtility (nearMouse);
        }

        void close (void)
        {
            // delete seeker
            delete (ctfSeeker);
            ctfSeeker = NULL;

            // delete each enemy
            for (int i = 0; i < ctfEnemyCount; i++)
            {
                delete (ctfEnemies[i]);
                ctfEnemies[i] = NULL;
            }

            // clear the group of all vehicles
            all.clear();
        }

        void reset (void)
        {
            // count resets
            resetCount++;

            // reset the seeker ("hero"/"attacker") and enemies
            ctfSeeker->reset ();
            for (int i = 0; i<ctfEnemyCount; i++) ctfEnemies[i]->reset ();

            // reset camera position
            OpenSteerDemo::position2dCamera (*ctfSeeker);

            // make camera jump immediately to new position
            OpenSteerDemo::camera.doNotSmoothNextMove ();
        }

        void handleFunctionKeys (int keyNumber)
        {
            switch (keyNumber)
            {
            case 1: CtfBase::addOneObstacle ();    break;
            case 2: CtfBase::removeOneObstacle (); break;
            }
        }

        void printMiniHelpForFunctionKeys (void)
        {
            std::ostringstream message;
            message << "Function keys handled by ";
            message << '"' << name() << '"' << ':' << std::ends;
            OpenSteerDemo::printMessage (message);
            OpenSteerDemo::printMessage ("  F1     add one obstacle.");
            OpenSteerDemo::printMessage ("  F2     remove one obstacle.");
            OpenSteerDemo::printMessage ("");
        }

        const AVGroup& allVehicles (void) {return (const AVGroup&) all;}

        void drawHomeBase (void)
        {
            const Vector3 up (0, 0.01f, 0);
            const Color atColor (0.3f, 0.3f, 0.5f);
            const Color noColor = gGray50;
            const bool reached = ctfSeeker->state == CtfSeeker::atGoal;
            const Color baseColor = (reached ? atColor : noColor);
            drawXZDisk (gHomeBaseRadius,    gHomeBaseCenter, baseColor, 40);
            drawXZDisk (gHomeBaseRadius/15, gHomeBaseCenter+up, gBlack, 20);
        }

        void drawObstacles (void)
        {
            const Color color (0.8f, 0.6f, 0.4f);
            const SOG& allSO = CtfBase::allObstacles;
            for (SOI so = allSO.begin(); so != allSO.end(); so++)
            {
                drawXZCircle ((**so).radius, (**so).center, color, 40);
            }
        }

        // a group (STL vector) of all vehicles in the PlugIn
        std::vector<CtfBase*> all;
    };


    CtfPlugIn gCtfPlugIn;


    // ----------------------------------------------------------------------------


} // anonymous namespace
