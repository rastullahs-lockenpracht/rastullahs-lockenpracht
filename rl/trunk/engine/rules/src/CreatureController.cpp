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
#include "stdinc.h" //precompiled header

#include "Actor.h"
#include "Creature.h"
#include "DsaManager.h"
#include "MeshAnimation.h"
#include "MeshObject.h"
#include "CreatureController.h"
#include "CreatureControllerManager.h"
#include "PhysicsManager.h"
#include "PhysicalThing.h"




using namespace Ogre;
using namespace std;

namespace rl
{

    class Stehen : public AbstractMovement
    {
    public:
        Stehen(CreatureController *creature) : AbstractMovement(creature), mVelocity(Vector3::ZERO), mRotationMovement(NULL) {}
        virtual CreatureController::MovementType getId() const {return CreatureController::MT_STEHEN;}
        virtual CreatureController::MovementType getFallBackMovement() const {return CreatureController::MT_NONE;}
        virtual void activate()
        {
            AbstractMovement::activate();
            getRotationMovement()->activate();
        }
        virtual void deactivate()
        {
            AbstractMovement::deactivate();
            getRotationMovement()->deactivate();
        }
        virtual bool calculateBaseVelocity(Real &velocity)
        {
            velocity = 0.0f;
            return isPossible();
        }
        virtual bool isPossible() const
        {
            return
                mMovingCreature->getAbstractLocation() == CreatureController::AL_FLOOR &&
                mMovingCreature->getCreature()->getAu() > 0 &&
                !(mMovingCreature->getCreature()->getStatus() & (Effect::STATUS_DEAD | Effect::STATUS_UNCONSCIOUS | Effect::STATUS_SLEEPING));
        }
        virtual void calculateForceAndTorque(Vector3 &force, Vector3 &torque, Real timestep)
        {
            getRotationMovement()->calculateForceAndTorque(force, torque, timestep);

            Real mass;
            Vector3 inertia;
            OgreNewt::Body *body = mMovingCreature->getCreature()->getActor()->getPhysicalThing()->_getBody();
            body->getMassMatrix(mass, inertia);

            Vector3 vel = mMovingCreature->getVelocity();
            Real delay = 0.05;//(2 * PhysicsManager::getSingleton().getMaxTimestep());
            if(vel.squaredLength() > mVelocity.squaredLength())
                delay *= 1.5;
            force = mass * (mVelocity - vel) / delay;
            force.y = 0;
        }
        virtual bool run(Ogre::Real elapsedTime,  Ogre::Vector3 direction, Ogre::Vector3 rotation)
        {
            direction.normalise();
            Real velocity;
            calculateBaseVelocity(velocity);
            mVelocity = direction * velocity;
            applyAuChanges(elapsedTime);
            setAnimation(elapsedTime);
            if( getRotationMovement()->isPossible() )
                return getRotationMovement()->run(elapsedTime, direction, rotation);
            return false;
        }
        virtual void setAnimation(Ogre::Real elapsedTime)
        {
            Real omegaY = mMovingCreature->getCreature()->getActor()->getPhysicalThing()->_getBody()->getOmega().y;
            if( omegaY > Degree(20).valueRadians() )
                mMovingCreature->setAnimation("drehen_links");
            else
            {
                if( omegaY < Degree(-20).valueRadians() )
                    mMovingCreature->setAnimation("drehen_rechts");
                else
                    mMovingCreature->setAnimation("Idle");
            }
        }
        virtual void applyAuChanges(Ogre::Real elapsedTime)
        {
            mMovingCreature->getCreature()->regenerateAu(2, 0.5, elapsedTime);
        }
        virtual bool isDirectionPossible(Ogre::Vector3 &direction) const
        {
            Vector3 oldDirection(direction);
            direction = Vector3::ZERO;
            return oldDirection == Vector3::ZERO;
        }
        virtual bool isRotationPossible(Ogre::Vector3 &rotation) const
        {
            return getRotationMovement()->isRotationPossible(rotation);
        }
    protected:
        Ogre::Vector3 mVelocity;
        mutable AbstractMovement *mRotationMovement;
        virtual AbstractMovement* getRotationMovement() const
        {
            if( mRotationMovement == NULL)
            {
                mRotationMovement = mMovingCreature->getMovementFromId(CreatureController::MT_DREHEN);
            }
            if( mRotationMovement == NULL )
            {
                Throw(NullPointerException, "Konnte Movement mit der Id MT_STEHEN_DREHEN nicht finden.");
            }
            return mRotationMovement;
        }
    };

    class Drehen : public AbstractMovement
    {
    public:
        Drehen(CreatureController *creature) : AbstractMovement(creature), mYaw(0)
        {
            mRotLinearSpringK = 600.0f;
            Real relationCoefficient = 1.0f;
            mRotLinearDampingK = relationCoefficient * 2.0f * Math::Sqrt(mRotLinearSpringK);
        }
        virtual CreatureController::MovementType getId() const {return CreatureController::MT_DREHEN;}
        virtual CreatureController::MovementType getFallBackMovement() const {return CreatureController::MT_NONE;}
        virtual bool calculateBaseVelocity(Real &velocity)
        {
            velocity = 0.3f;
            return isPossible();
        }
        virtual bool isPossible() const
        {
            return
                mMovingCreature->getAbstractLocation() == CreatureController::AL_FLOOR;// &&
                mMovingCreature->getCreature()->getAu() > 1 &&
                !(mMovingCreature->getCreature()->getStatus() & (Effect::STATUS_IMMOBILE));
        }
        virtual void activate()
        {
            AbstractMovement::activate();
            mYaw = mMovingCreature->getCreature()->getActor()->getWorldOrientation().getYaw();
        }
        virtual void deactivate()
        {
            AbstractMovement::deactivate();
        }
        virtual void calculateForceAndTorque(Vector3 &force, Vector3 &torque, Real timestep)
        {
            Real mass;
            Vector3 inertia;
            OgreNewt::Body *body = mMovingCreature->getCreature()->getActor()->getPhysicalThing()->_getBody();
            body->getMassMatrix(mass, inertia);

            Quaternion orientation;
            Vector3 position;
            body->getPositionOrientation(position, orientation);
            // Calculate angular velocity
            // We first need the yaw rotation from actual yaw to desired yaw
            Vector3 src = orientation*Vector3::UNIT_Z;
            src.y = 0;
            Vector3 dst = Quaternion(mYaw, Vector3::UNIT_Y)*Vector3::UNIT_Z;
            dst.y = 0;
            Radian yaw = src.getRotationTo(dst, Vector3::UNIT_Y).getYaw();

            // using a spring system to apply the rotation
            Vector3 diff = Vector3(0, yaw.valueRadians(), 0);
            Vector3 omega = body->getOmega();
            omega.x = omega.z = 0;
            Vector3 springAcc = mRotLinearSpringK*diff - mRotLinearDampingK * omega;

            /// @ todo change this
            torque = springAcc * inertia; // this would be correct
            //torque = springAcc * mass * 40;
        }
        virtual bool run(Ogre::Real elapsedTime,  Ogre::Vector3 direction, Ogre::Vector3 rotation)
        {
            mYaw += Radian(rotation.y);
            while (mYaw.valueDegrees() > 360.0f) mYaw -= Degree(360.0f);
            while (mYaw.valueDegrees() < -360.0f) mYaw += Degree(360.0f);

            OgreNewt::Body *body = mMovingCreature->getCreature()->getActor()->getPhysicalThing()->_getBody();
            Quaternion orientation;
            Vector3 position;
            body->getPositionOrientation(position, orientation);

            return orientation.getYaw() != mYaw;
        }
        virtual bool isDirectionPossible(Ogre::Vector3 &direction) const
        {
            Vector3 oldDirection(direction);
            direction = Vector3::ZERO;
            return oldDirection == Vector3::ZERO;
        }
        virtual bool isRotationPossible(Ogre::Vector3 &rotation) const
        {
            Vector3 oldRotation(rotation);
            rotation.x = rotation.z = 0;
            return oldRotation.x == 0 && oldRotation.z == 0;
        }
    protected:
        Ogre::Radian mYaw;
        Ogre::Real mRotLinearDampingK, mRotLinearSpringK;
        virtual Real getMovementDefinedValue() {return mYaw.valueRadians();}
    };


    class Gehen : public Stehen
    {
    public:
        Gehen(CreatureController *creature) : Stehen(creature) {}
        virtual CreatureController::MovementType getId() const {return CreatureController::MT_GEHEN;}
        virtual CreatureController::MovementType getFallBackMovement() const {return CreatureController::MT_STEHEN;}
        virtual bool calculateBaseVelocity(Real &velocity)
        {
            velocity = mMovingCreature->getCurrentGS() / 3.6f * 0.7f;
            return isPossible();
        }
        virtual bool isPossible() const
        {
            return
                mMovingCreature->getAbstractLocation() == CreatureController::AL_FLOOR &&
                mMovingCreature->getCreature()->getAu() > 1 &&
                !(mMovingCreature->getCreature()->getStatus() & Effect::STATUS_IMMOBILE);
        }
        virtual bool run(Ogre::Real elapsedTime, Ogre::Vector3 direction, Ogre::Vector3 rotation)
        {
            Stehen::run(elapsedTime, direction, rotation);
            return true;
        }
        virtual bool isDirectionPossible(Ogre::Vector3 &direction) const
        {
            Vector3 oldDirection(direction);
            direction.x = direction.y = 0;
            if( direction.z > 0 )
                direction.z = 0;
            return oldDirection.x == 0 && oldDirection.y == 0 && oldDirection.z > 0;
        }
        virtual void applyAuChanges(Ogre::Real elapsedTime)
        {
            mMovingCreature->getCreature()->regenerateAu(2, 0.5, elapsedTime);
        }
        virtual void setAnimation(Ogre::Real elapsedTime)
        {
            Real step = 1.49; // the width of a step
            //MeshObject* charMesh = dynamic_cast<MeshObject*>(mMovingCreature->getCreature()->getActor()->getControlledObject());
            //Real length = charMesh->getAnimation("Run")->getLength();
            Real length = 5./3.;
            Real weight = 1;
            Real relTimeOffset = 0;

            switch( mMovingCreature->getLastMovementType() )
            {
            case CreatureController::MT_NONE:
                break;
            case CreatureController::MT_STEHEN:
                relTimeOffset = 0.25;
                break;
            case CreatureController::MT_JOGGEN:
            case CreatureController::MT_LAUFEN:
            case CreatureController::MT_RENNEN:
                relTimeOffset = mMovingCreature->getAnimationTimePlayed();
                if( mMovingCreature->getLastMovementChange() < 1.0f )
                    weight = mMovingCreature->getLastMovementChange() / 1.0f;
                break;
            default:
                break;
            }


            // apply relTimeOffset only once
            if( mMovingCreature->getLastMovementChange() > elapsedTime )
                relTimeOffset = 0;

            MeshAnimation *meshAnim = mMovingCreature->setAnimation("Walk", -mMovingCreature->getVelocity().z / (step / length), 0, "Walk", weight );
            if( meshAnim != NULL )
                meshAnim->doAddTime(relTimeOffset*meshAnim->getLength());
        }
    };

    class Joggen : public Gehen
    {
    public:
        Joggen(CreatureController *creature) : Gehen(creature) {}
        virtual CreatureController::MovementType getId() const {return CreatureController::MT_JOGGEN;}
        virtual CreatureController::MovementType getFallBackMovement() const {return CreatureController::MT_GEHEN;}
        virtual bool calculateBaseVelocity(Real &velocity)
        {
            velocity = mMovingCreature->getCurrentGS() / 2.8f;
            return isPossible();
        }
        virtual bool isPossible() const
        {
            return Gehen::isPossible() &&
                mMovingCreature->getCreature()->getAu() > 6;
        }
        virtual void applyAuChanges(Ogre::Real elapsedTime) {} // empty
        virtual void setAnimation(Ogre::Real elapsedTime)
        {
            Real step = 2.835; // the width of a step
            // if the persons runs, the feet don't touch always the ground, so this value must be bigger
            // trynerror:
            step += 0.5;
            //MeshObject* charMesh = dynamic_cast<MeshObject*>(mMovingCreature->getCreature()->getActor()->getControlledObject());
            //Real length = charMesh->getAnimation("Run")->getLength();
            Real length = 5./3.;
            Real weight = 1;
            Real relTimeOffset = 0;


            switch( mMovingCreature->getLastMovementType() )
            {
            case CreatureController::MT_NONE:
                break;
            case CreatureController::MT_STEHEN:
                relTimeOffset = 0.25;
                break;
            case CreatureController::MT_GEHEN:
                relTimeOffset = mMovingCreature->getAnimationTimePlayed();
                if( mMovingCreature->getLastMovementChange() < 0.5f )
                    weight = mMovingCreature->getLastMovementChange() / 0.5f;
                break;
            default:
                break;
            }

            // apply relTimeOffset only once
            if( mMovingCreature->getLastMovementChange() > elapsedTime )
                relTimeOffset = 0;

            MeshAnimation *meshAnim = mMovingCreature->setAnimation("Run", -mMovingCreature->getVelocity().z / (step / length), 0, "Run", weight );
            if( meshAnim != NULL )
                meshAnim->doAddTime(relTimeOffset*meshAnim->getLength());
        }
    };

    class Laufen : public Gehen
    {
    public:
        Laufen(CreatureController *creature) : Gehen(creature), mTimePerAu(1), mLastProbe(0) {}
        virtual CreatureController::MovementType getId() const {return CreatureController::MT_LAUFEN;}
        virtual CreatureController::MovementType getFallBackMovement() const {return CreatureController::MT_JOGGEN;}
        virtual bool calculateBaseVelocity(Real &velocity)
        {
            velocity = mMovingCreature->getCurrentGS() / 2.0f;
            return isPossible();
        }
        virtual bool isPossible() const
        {
            if( mMovingCreature->getMovement() == this )
                return Gehen::isPossible() &&
                    mMovingCreature->getCreature()->getAu() > 6;
            else
                return Gehen::isPossible() &&
                    mMovingCreature->getCreature()->getAu() > 8;
        }
        virtual bool run(Ogre::Real elapsedTime, Ogre::Vector3 direction, Ogre::Vector3 rotation)
        {
            doTalentProbeIfNecessary();
            return Gehen::run(elapsedTime, direction, rotation);
        }
        virtual void applyAuChanges(Ogre::Real elapsedTime)
        {
            mMovingCreature->getCreature()->damageAu(elapsedTime/mTimePerAu);
        }
        virtual void setAnimation(Ogre::Real elapsedTime)
        {
            Real step = 2.835; // the width of a step
            // if the persons runs, the feet don't touch always the ground, so this value must be bigger
            // trynerror:
            step += 0.5;
            //MeshObject* charMesh = dynamic_cast<MeshObject*>(mMovingCreature->getCreature()->getActor()->getControlledObject());
            //Real length = charMesh->getAnimation("Run")->getLength();
            Real length = 5./3.;
            Real weight = 1;
            Real relTimeOffset = 0;


            switch( mMovingCreature->getLastMovementType() )
            {
            case CreatureController::MT_NONE:
                break;
            case CreatureController::MT_STEHEN:
                relTimeOffset = 0.25;
                break;
            case CreatureController::MT_GEHEN:
                relTimeOffset = mMovingCreature->getAnimationTimePlayed();
                if( mMovingCreature->getLastMovementChange() < 0.5f )
                    weight = mMovingCreature->getLastMovementChange() / 0.5f;
                break;
            default:
                break;
            }

            // apply relTimeOffset only once
            if( mMovingCreature->getLastMovementChange() > elapsedTime )
                relTimeOffset = 0;

            MeshAnimation *meshAnim = mMovingCreature->setAnimation("Run", -mMovingCreature->getVelocity().z / (step / length), 0, "Run", weight );
            if( meshAnim != NULL )
                meshAnim->doAddTime(relTimeOffset*meshAnim->getLength());
        }
        virtual void activate()
        {
            AbstractMovement::activate();
            doTalentProbeIfNecessary();
        }
        virtual void doTalentProbeIfNecessary()
        {
            Date now = DsaManager::getSingleton().getCurrentDate();
            if( mLastProbe + Date::ONE_SPIELRUNDE >= now || mLastProbe == 0 )
            {
                mTimePerAu = 180;
                try
                {
                    mLastProbe = now;
                    int taw = mMovingCreature->getCreature()->doTalentprobe("Athletik",0);
                    if( taw == RESULT_AUTOERFOLG )
                    {
                        mTimePerAu += mMovingCreature->getCreature()->getTalent("Athletik") * 5;
                    }
                    else if( taw == RESULT_SPEKT_AUTOERFOLG )
                    {
                        mTimePerAu += mMovingCreature->getCreature()->getTalent("Athletik") * 10;
                    }
                    else
                    {
                        mTimePerAu += taw * 5;
                    }
                }
                catch(OutOfRangeException)
                {
                    mTimePerAu = 180;
                }
            }
        }
    protected:
        Real mTimePerAu;
        Date mLastProbe;
    };


    class Rennen : public Gehen
    {
    public:
        Rennen(CreatureController *creature) : Gehen(creature), mVelocityImprovement(0), mLastProbe(0) {}
        virtual CreatureController::MovementType getId() const {return CreatureController::MT_RENNEN;}
        virtual CreatureController::MovementType getFallBackMovement() const {return CreatureController::MT_LAUFEN;}
        virtual bool calculateBaseVelocity(Real &velocity)
        {
            velocity = mMovingCreature->getCurrentGS();
            return isPossible();
        }
        virtual bool isPossible() const
        {
            if( mMovingCreature->getMovement() == this )
                return Gehen::isPossible() &&
                    mMovingCreature->getCreature()->getAu() > mMovingCreature->getCreature()->getAuMax() * 0.3 &&
                    mMovingCreature->getCreature()->getAu() > 6;
            else
                return Gehen::isPossible() &&
                    mMovingCreature->getCreature()->getAu() > mMovingCreature->getCreature()->getAuMax() * 0.5 &&
                    mMovingCreature->getCreature()->getAu() > 8;
        }
        virtual bool run(Ogre::Real elapsedTime, Ogre::Vector3 direction, Ogre::Vector3 rotation)
        {
            doTalentProbeIfNecessary();
            bool ret = Gehen::run(elapsedTime, direction, rotation);
            mVelocity += mVelocityImprovement;
            return ret;
        }
        virtual void applyAuChanges(Ogre::Real elapsedTime)
        {
            mMovingCreature->getCreature()->damageAu(elapsedTime/1.5);
        }
        virtual void setAnimation(Ogre::Real elapsedTime)
        {
            Real step = 2.835; // the width of a step
            // if the persons runs, the feet don't touch always the ground, so this value must be bigger
            // trynerror:
            step += 1.5;
            //MeshObject* charMesh = dynamic_cast<MeshObject*>(mMovingCreature->getCreature()->getActor()->getControlledObject());
            //Real length = charMesh->getAnimation("Run")->getLength();
            Real length = 5./3.;
            Real weight = 1;
            Real relTimeOffset = 0;


            switch( mMovingCreature->getLastMovementType() )
            {
            case CreatureController::MT_NONE:
                break;
            case CreatureController::MT_STEHEN:
                relTimeOffset = 0.25;
                break;
            case CreatureController::MT_GEHEN:
                relTimeOffset = mMovingCreature->getAnimationTimePlayed();
                if( mMovingCreature->getLastMovementChange() < 0.5f )
                    weight = mMovingCreature->getLastMovementChange() / 0.5f;
                break;
            default:
                break;
            }

            // apply relTimeOffset only once
            if( mMovingCreature->getLastMovementChange() > elapsedTime )
                relTimeOffset = 0;

            MeshAnimation *meshAnim = mMovingCreature->setAnimation("Run", -mMovingCreature->getVelocity().z / (step / length), 0, "Run", weight );
            if( meshAnim != NULL )
                meshAnim->doAddTime(relTimeOffset*meshAnim->getLength());
        }
        virtual void activate()
        {
            AbstractMovement::activate();
            doTalentProbeIfNecessary();
        }
        virtual void doTalentProbeIfNecessary()
        {
            Date now = DsaManager::getSingleton().getCurrentDate();
            if( mLastProbe + mMovingCreature->getCreature()->getAuMax() * Date::ONE_SECOND >= now || mLastProbe == 0)
            {
                mVelocityImprovement = 0;
                try
                {
                    mLastProbe = now;
                    int taw = mMovingCreature->getCreature()->doTalentprobe("Athletik",0);
                    if( taw == RESULT_AUTOERFOLG )
                    {
                        mVelocityImprovement += mMovingCreature->getCreature()->getTalent("Athletik") * 0.2;
                    }
                    else if( taw == RESULT_SPEKT_AUTOERFOLG )
                    {
                        mVelocityImprovement += mMovingCreature->getCreature()->getTalent("Athletik") * 0.3;
                    }
                    else
                    {
                        mVelocityImprovement += taw * 0.1;
                    }
                }
                catch(OutOfRangeException)
                {
                    mVelocityImprovement = 0;
                }
            }
        }
    protected:
        Real mVelocityImprovement;
        Date mLastProbe;
    };

    class RueckwaertsGehen : public Gehen
    {
    public:
        RueckwaertsGehen(CreatureController *creature) : Gehen(creature) {}
        virtual CreatureController::MovementType getId() const {return CreatureController::MT_RUECKWAERTS_GEHEN;}
        virtual CreatureController::MovementType getFallBackMovement() const {return CreatureController::MT_STEHEN;}
        virtual bool calculateBaseVelocity(Real &velocity)
        {
            velocity = mMovingCreature->getCurrentGS() / 4.0;
            return isPossible();
        }
        virtual void setAnimation(Ogre::Real elapsedTime)
        {
            mMovingCreature->setAnimation("gehen_rueckwaerts");
        }
    };

    class RueckwaertsJoggen : public Joggen
    {
    public:
        RueckwaertsJoggen(CreatureController *creature) : Joggen(creature) {}
        virtual CreatureController::MovementType getId() const {return CreatureController::MT_RUECKWAERTS_JOGGEN;}
        virtual CreatureController::MovementType getFallBackMovement() const {return CreatureController::MT_RUECKWAERTS_GEHEN;}
        virtual bool calculateBaseVelocity(Real &velocity)
        {
            velocity = mMovingCreature->getCurrentGS() / 3.2;
            return isPossible();
        }
        virtual void setAnimation(Ogre::Real elapsedTime)
        {
            mMovingCreature->setAnimation("Run");
        }
    };


    class SeitwaertsGehen : public Gehen
    {
    public:
        SeitwaertsGehen(CreatureController *creature) : Gehen(creature), mLeft(true) {}
        virtual CreatureController::MovementType getId() const {return CreatureController::MT_SEITWAERTS_GEHEN;}
        virtual CreatureController::MovementType getFallBackMovement() const {return CreatureController::MT_STEHEN;}
        virtual bool calculateBaseVelocity(Real &velocity)
        {
            velocity = mMovingCreature->getCurrentGS() / 4.0;
            return isPossible();
        }
        virtual bool isDirectionPossible(Ogre::Vector3 &direction) const
        {
            Vector3 oldDirection(direction);
            direction.z = direction.y = 0;
            return oldDirection.z == 0 && oldDirection.y == 0;
        }
        virtual bool run(Ogre::Real elapsedTime, Ogre::Vector3 direction, Ogre::Vector3 rotation)
        {
            mLeft = direction.x < 0;
            return Gehen::run(elapsedTime, direction, rotation);
        }
        virtual void setAnimation(Ogre::Real elapsedTime)
        {
            if( mLeft )
                mMovingCreature->setAnimation("seitwaerts_links");
            else
                mMovingCreature->setAnimation("seitwaerts_rechts");
        }
    protected:
        bool mLeft;
    };

    class Schleichen : public Gehen
    {
    public:
        Schleichen(CreatureController *creature) : Gehen(creature), mState(UP), mTimer(0) {}
        virtual CreatureController::MovementType getId() const {return CreatureController::MT_SCHLEICHEN;}
        virtual CreatureController::MovementType getFallBackMovement() const {return CreatureController::MT_STEHEN;}
        virtual bool calculateBaseVelocity(Real &velocity)
        {
            velocity = mMovingCreature->getCurrentGS() / 6.0;
            return isPossible();
        }
        virtual bool run(Ogre::Real elapsedTime, Ogre::Vector3 direction, Ogre::Vector3 rotation)
        {
            if( mState == UPTODOWN )
            {
                mTimer += elapsedTime;
                if( mTimer > 0.5f )
                    mState = DOWN;
            }
            if( mState == UP )
            {
                mMovingCreature->setMovement(CreatureController::MT_STEHEN, direction, rotation);
            }
            if( mState == DOWNTOUP )
            {
                mTimer += elapsedTime;
                if( mTimer > 0.5f )
                    mState = UP;
            }
            if( mState == DOWN )
            {
                setAnimation(elapsedTime);
                direction.normalise();
                Real velocity;
                calculateBaseVelocity(velocity);
                mVelocity = direction * velocity;
                if( direction == Vector3::ZERO )
                    mMovingCreature->setAnimation("hocke_idle");
                else
                    mMovingCreature->setAnimation("hocke_gehen");
                applyAuChanges(elapsedTime);
                if( getRotationMovement()->isPossible() )
                    getRotationMovement()->run(elapsedTime, direction, rotation);
            }
            else
                mVelocity = Vector3::ZERO;
            return true;
        }
        void applyAuChanges(Ogre::Real elapsedTime) {}
        virtual void setAnimation(Ogre::Real elapsedTime) {} // is not used
        virtual void activate()
        {
            AbstractMovement::activate();
            mState = UPTODOWN;
            mMovingCreature->setAnimation("idle_zu_hocke",1,1,"Idle");
            mTimer = 0;
        }
        virtual bool canChangeToMovement(CreatureController::MovementType id)
        {
            return mState == UP;
        }
        virtual void requestChangeToMovement(CreatureController::MovementType id)
        {
            if( mState == DOWN )
            {
                mState = DOWNTOUP;
                mMovingCreature->setAnimation("hocke_zu_stehen",1,1,"Idle");
                mTimer = 0;
            }
        }
    protected:
        enum {UP, DOWN, UPTODOWN, DOWNTOUP} mState;
        Real mTimer;
    };


    class Hochsprung : public AbstractMovement
    {
    public:
        Hochsprung(CreatureController *creature) : AbstractMovement(creature), mState(DOWN), mHeight(0), mJumpNow(false), mTimer(0) {}
        virtual CreatureController::MovementType getId() const {return CreatureController::MT_HOCHSPRUNG;}
        virtual CreatureController::MovementType getFallBackMovement() const {return CreatureController::MT_STEHEN;}
        virtual void activate()
        {
            AbstractMovement::activate();
            mState = DOWNTOUP;
            mMovingCreature->setAnimation("idle_absprung",1,1,"Idle");
            mTimer = 0;
            calculateBaseVelocity(mHeight);

            try
            {
                int taw = mMovingCreature->getCreature()->doTalentprobe("Athletik", 0);
                if( taw > 0 )
                {
                    if( taw == RESULT_SPEKT_AUTOERFOLG )
                    {
                        mHeight += mMovingCreature->getCreature()->getTalent("Athletik") / 12.5;
                    }
                    else if( taw == RESULT_AUTOERFOLG )
                    {
                        mHeight += mMovingCreature->getCreature()->getTalent("Athletik") / 25.0;
                    }
                    else
                    {
                        mHeight += taw / 25.0;
                    }
                }
            }
            catch(OutOfRangeException err)
            {
            }
            mMovingCreature->getCreature()->damageAu(2./3);
        }
        virtual void deactivate()
        {
            AbstractMovement::deactivate();
        }
        virtual bool calculateBaseVelocity(Real &velocity)
        {
            velocity = (mMovingCreature->getCreature()->getEigenschaft("GE") +
                        mMovingCreature->getCreature()->getEigenschaft("KK")) / 4.0 / 5.0;

            // steht nicht in den Regeln aber finde ich sinnvoll
            // velocityBase *= (1 - getrageneLast/KK);
            // steht in den Regeln: pro Erschöpfung ein KK abziehen
            //if( mErschoepfung > getEigenschaft("KO") )
            //    velocity -= (mErschoepfung - getEigenschaft("KO")) / 4.0 / 5.0;
            // steht nicht in den Regeln, aber finde ich sinnvoll
            //if( getAu() < getAuBasis() / 3.0 )
            //    velocity -= (getAu() / getAuBasis() * 3.0) * getEigenschaft("GE") / 4.0 / 5.0;
            return isPossible();
        }
        virtual bool isPossible() const
        {
            if( mMovingCreature->getMovement() == this )
                return
                    (mMovingCreature->getAbstractLocation() == CreatureController::AL_FLOOR ||
                    mMovingCreature->getAbstractLocation() == CreatureController::AL_AIRBORNE) &&
                    mMovingCreature->getCreature()->getAu() > 6 &&
                    !(mMovingCreature->getCreature()->getStatus() & (Effect::STATUS_IMMOBILE));
            else
                return
                mMovingCreature->getAbstractLocation() == CreatureController::AL_FLOOR &&
                mMovingCreature->getCreature()->getAu() > 8 &&
                mMovingCreature->getCreature()->getAu() > mMovingCreature->getCreature()->getAuMax()/3.0 &&
                !(mMovingCreature->getCreature()->getStatus() & (Effect::STATUS_IMMOBILE));
        }
        virtual void calculateForceAndTorque(Vector3 &force, Vector3 &torque, Real timestep)
        {
            Real mass;
            Vector3 inertia;
            OgreNewt::Body *body = mMovingCreature->getCreature()->getActor()->getPhysicalThing()->_getBody();
            body->getMassMatrix(mass, inertia);

            if( mJumpNow )
            {
                mJumpNow = false;

                Real m = mass;
                Real g = PhysicsManager::getSingleton().getGravity().length();
                Real t = timestep;
                Real h = mHeight;
                Real jumpForce = 0.5f*g*m * (Math::Sqrt(1 + 8*h/(g * t * t)) - 1);
                force = Vector3(0,
                    jumpForce,
                    0);
                mMovingCreature->setAbstractLocation( CreatureController::AL_AIRBORNE );
            }

            Vector3 omega = mMovingCreature->getCreature()->getActor()->getPhysicalThing()->_getBody()->getOmega();
            torque = -omega / PhysicsManager::getSingleton().getMaxTimestep() * 2 * mass;
        }
        virtual bool run(Ogre::Real elapsedTime,  Ogre::Vector3 direction, Ogre::Vector3 rotation)
        {
            if( mState == DOWN )
            {
                mMovingCreature->setMovement(CreatureController::MT_STEHEN, direction, rotation);
            }
            if( mState == UPTODOWN )
            {
                mTimer += elapsedTime;
                if( mTimer >= 0.5f )
                {
                    mState = DOWN;
                }
            }
            if( mState == UP )
            {
                mTimer += elapsedTime;
                if( mTimer < 0.5f )
                {
                    mMovingCreature->setAbstractLocation( CreatureController::AL_AIRBORNE );
                }
                else if( mMovingCreature->getAbstractLocation() != CreatureController::AL_AIRBORNE )
                {
                    mState = UPTODOWN;
                    mMovingCreature->setAnimation("idle_sprung_landung", 1, 1, "Idle");
                    mTimer = 0;
                }
            }
            if( mState == DOWNTOUP )
            {
                mTimer += elapsedTime;
                if( mTimer >= 0.25f )
                {
                    mState = UP;
                    mMovingCreature->setAbstractLocation(CreatureController::AL_AIRBORNE);
                    //mMovingCreature->setAnimation("idle_sprung"); // we also don't need this animation!
                    mJumpNow = true;
                    mTimer = 0;
                }
            }
            return false;
        }
        virtual bool isDirectionPossible(Ogre::Vector3 &direction) const
        {
            Vector3 oldDirection(direction);
            direction.z = direction.x = 0;
            if(direction.y < 0)
                direction.y = 0;
            return oldDirection.x == 0 && oldDirection.z == 0 && oldDirection.y > 0;
        }
        virtual bool isRotationPossible(Ogre::Vector3 &rotation) const
        {
            rotation = Vector3::ZERO;
            return false;
        }
        virtual bool canChangeToMovement(CreatureController::MovementType id)
        {
            return mState == DOWN;
        }
    protected:
        enum {UP, DOWN, UPTODOWN, DOWNTOUP} mState;
        Ogre::Real mHeight;
        bool mJumpNow;
        Ogre::Real mTimer;
    };


    class Weitsprung : public AbstractMovement
    {
    public:
        Weitsprung(CreatureController *creature) :
          AbstractMovement(creature), mState(DOWN), mWidth(0),
              mJumpNow(false), mTimer(0), mApplyForceTime(0.12),
              mApplyForceTimer(0), mLastForce(Vector3::ZERO),
              mVelocityBeforeJump(0), mTanJumpAngle(Math::Tan(Degree(17))) {}
        virtual CreatureController::MovementType getId() const {return CreatureController::MT_WEITSPRUNG;}
        virtual CreatureController::MovementType getFallBackMovement() const {return CreatureController::MT_STEHEN;}
        virtual void activate()
        {
            AbstractMovement::activate();
            mState = DOWNTOUP;
            mMovingCreature->setAnimation("rennen_absprung",1,1,"Run");
            mTimer = 0;
            calculateBaseVelocity(mWidth);

            try
            {
                // the person will only achieve this width if it is running
                // retrieve run movement
                AbstractMovement *run = mMovingCreature->getMovementFromId(CreatureController::MT_RENNEN);
                if( run != NULL )
                {
                    Real vel(0);
                    run->calculateBaseVelocity(vel);
                    Real factor = -mMovingCreature->getVelocity().z / vel;
                    factor = std::max(Real(0),factor);
                    // without moving before, the width will be 1/3
                    mWidth = mWidth/3. + mWidth * 2./3. * factor;
                }


                int taw = mMovingCreature->getCreature()->doTalentprobe("Athletik", 0);
                if( taw > 0 )
                {
                    if( taw == RESULT_SPEKT_AUTOERFOLG )
                    {
                        mWidth += mMovingCreature->getCreature()->getTalent("Athletik") / 25.0;
                    }
                    else if( taw == RESULT_AUTOERFOLG )
                    {
                        mWidth += mMovingCreature->getCreature()->getTalent("Athletik") / 50.0;
                    }
                    else
                    {
                        mWidth += taw / 50.0;
                    }
                }
            }
            catch(OutOfRangeException err)
            {
            }
            mMovingCreature->getCreature()->damageAu(2./3);
        }
        virtual void deactivate()
        {
            AbstractMovement::deactivate();
        }
        virtual bool calculateBaseVelocity(Real &velocity)
        {
            velocity = (mMovingCreature->getCreature()->getEigenschaft("GE") +
                        mMovingCreature->getCreature()->getEigenschaft("KK")) / 5.0;

            // steht nicht in den Regeln aber finde ich sinnvoll
            // velocityBase *= (1 - getrageneLast/KK);
            // steht in den Regeln: pro Erschöpfung ein KK abziehen
            //if( mErschoepfung > getEigenschaft("KO") )
            //    velocity -= (mErschoepfung - getEigenschaft("KO")) / 5.0;
            // steht nicht in den Regeln, aber finde ich sinnvoll
            //if( getAu() < getAuBasis() / 3.0 )
            //    velocity -= (getAu() / getAuBasis() * 3.0) * getEigenschaft("GE") / 5.0;
            return isPossible();
        }
        virtual bool isPossible() const
        {
            if( mMovingCreature->getMovement() == this )
                return
                    (mMovingCreature->getAbstractLocation() == CreatureController::AL_FLOOR ||
                    mMovingCreature->getAbstractLocation() == CreatureController::AL_AIRBORNE) &&
                    mMovingCreature->getCreature()->getAu() > 6 &&
                    !(mMovingCreature->getCreature()->getStatus() & (Effect::STATUS_IMMOBILE));
            else
                return
                mMovingCreature->getAbstractLocation() == CreatureController::AL_FLOOR &&
                mMovingCreature->getCreature()->getAu() > 8 &&
                mMovingCreature->getCreature()->getAu() > mMovingCreature->getCreature()->getAuMax()/3.0 &&
                !(mMovingCreature->getCreature()->getStatus() & (Effect::STATUS_IMMOBILE));
        }
        virtual void calculateForceAndTorque(Vector3 &force, Vector3 &torque, Real timestep)
        {
            Real mass;
            Vector3 inertia;
            OgreNewt::Body *body = mMovingCreature->getCreature()->getActor()->getPhysicalThing()->_getBody();
            body->getMassMatrix(mass, inertia);


            if( mJumpNow && timestep != 0 )
            {
                mApplyForceTimer += timestep;

                if( mApplyForceTimer == timestep // first time
                    && mApplyForceTimer < mApplyForceTime )// this is handled below
                {
                    Real t1 = mApplyForceTime;
                    Real sx = mWidth;
                    Real g = -PhysicsManager::getSingleton().getGravity().y;
                    Real v0 = mVelocityBeforeJump;
                    Real ax = 0;
                    Real ay = 0;

                    if( t1 != 0 || 2 * sx >= v0 * t1 )
                    {
                        Real v1x = ( sx - v0 * t1 / 2 ) / ( Math::Sqrt( mTanJumpAngle * (2*sx /g - v0 * t1/g) ) + t1/2);
                        Real v1y = mTanJumpAngle * v1x;
                        ax =  (v1x - v0) / t1;
                        ay = v1y/t1 + g;
                    }

                    mLastForce.x = 0;
                    mLastForce.y = ay * mass;
                    mLastForce.z = -ax * mass;
                }

                if( mApplyForceTimer < mApplyForceTime )
                {
                    force = mLastForce;
                }
                else
                {
                    mJumpNow = false;
                    // adoption of the formula to our real jump time
                    Real t1 = mApplyForceTimer;
                    Real sx = mWidth;
                    Real g = -PhysicsManager::getSingleton().getGravity().y;
                    Real v0 = mVelocityBeforeJump;
                    Real ax = 0;
                    Real ay = 0;

                    if( t1 != 0 || 2 * sx >= v0 * t1 )
                    {
                        Real v1x = ( sx - v0 * t1 / 2 ) / ( Math::Sqrt( mTanJumpAngle * (2*sx /g - v0 * t1/g) ) + t1/2);
                        Real v1y = mTanJumpAngle * v1x;

std::ostringstream oss;
oss << "v0: " << v0 << "    v1x: " << v1x << "    timediff: " << mApplyForceTimer - mApplyForceTime;
LOG_MESSAGE(Logger::RULES, oss.str());

                        Vector3 v_now = mMovingCreature->getVelocity();
                        ax =  (v1x - -v_now.z) / timestep;
                        ay = (v1y- v_now.y)/timestep + g;
                    }

                    force.x = 0;
                    force.y = ay * mass;
                    force.z = -ax * mass;
                }
            }

            Vector3 omega = mMovingCreature->getCreature()->getActor()->getPhysicalThing()->_getBody()->getOmega();
            torque = -omega / PhysicsManager::getSingleton().getMaxTimestep() * 2 * mass;
        }
        virtual bool run(Ogre::Real elapsedTime,  Ogre::Vector3 direction, Ogre::Vector3 rotation)
        {
            if( mState == DOWN )
            {
                mMovingCreature->setMovement(CreatureController::MT_STEHEN, direction, rotation);
            }
            if( mState == UPTODOWN )
            {
                mTimer += elapsedTime;
                if( mTimer >= 0.2f )
                {
                    mState = DOWN;
                }
            }
            if( mState == UP )
            {
                mTimer += elapsedTime;
                if( mTimer < 0.5f )
                {
                    mMovingCreature->setAbstractLocation( CreatureController::AL_AIRBORNE );
                }
                else if( mMovingCreature->getAbstractLocation() != CreatureController::AL_AIRBORNE )
                {
                    mState = UPTODOWN;
                    mMovingCreature->setAnimation("rennen_sprung_landung", 1, 1, "Run");
                    mTimer = 0;
                }
            }
            if( mState == DOWNTOUP )
            {
                mTimer += elapsedTime;
                if( mTimer >= 0.3f )
                {
                    mState = UP;
                    mMovingCreature->setAbstractLocation(CreatureController::AL_AIRBORNE);
                    //mMovingCreature->setAnimation("rennen_sprung");
                    mJumpNow = true;
                    mApplyForceTimer = 0;
                    mVelocityBeforeJump = -mMovingCreature->getVelocity().z;
                    mTimer = 0;
                }
            }
            return false;
        }
        virtual bool isDirectionPossible(Ogre::Vector3 &direction) const
        {
            Vector3 oldDirection(direction);
            direction.x = 0;
            if(direction.y <= 0 || direction.z >= 0)
                direction.y = direction.z = 0;
            return oldDirection.x == 0 && oldDirection.z < 0 && oldDirection.y > 0;
        }
        virtual bool isRotationPossible(Ogre::Vector3 &rotation) const
        {
            rotation = Vector3::ZERO;
            return false;
        }
        virtual bool canChangeToMovement(CreatureController::MovementType id)
        {
            return mState == DOWN;
        }
    protected:
        enum {UP, DOWN, UPTODOWN, DOWNTOUP} mState;
        Ogre::Real mWidth;
        bool mJumpNow;
        Ogre::Real mTimer;
        Ogre::Real mApplyForceTime;
        Ogre::Real mApplyForceTimer;
        Ogre::Vector3 mLastForce;
        Ogre::Real mVelocityBeforeJump;
        Ogre::Real mTanJumpAngle;
    };


















    CreatureController::CreatureController(Creature *creature) :
        mCreature(creature),
        mAbstractLocation(AL_AIRBORNE),
        mMovement(NULL),
        mLastMovementType(MT_NONE),
        mLastMovementChange(0),
        mDirection(Vector3::ZERO),
        mRotation(Vector3::ZERO),
        mLastAnimationName(""),
        mLastCollisionName(""),
        mLastAnimationSpeed(1),
        mStillWeightedAnimationName(""),
        mLastFloorContact(0)
    {
        CreatureControllerManager::getSingleton().add(this);

        const OgreNewt::MaterialID *material = PhysicsManager::getSingleton().getMaterialID("character");
        mCreature->getActor()->getPhysicalThing()->setMaterialID(material);

        mCreature->getActor()->getPhysicalThing()->setPhysicsController(this);


        std::pair<MovementType, AbstractMovement*> movementPair;
        movementPair.first = MT_NONE;
        movementPair.second = NULL;
        mMovementMap.insert(movementPair);
        movementPair.first = MT_DREHEN;
        movementPair.second = new Drehen (this);
        mMovementMap.insert(movementPair);
        movementPair.first = MT_STEHEN;
        movementPair.second = new Stehen (this);
        mMovementMap.insert(movementPair);
        movementPair.first = MT_GEHEN;
        movementPair.second = new Gehen (this);
        mMovementMap.insert(movementPair);
        movementPair.first = MT_JOGGEN;
        movementPair.second = new Joggen (this);
        mMovementMap.insert(movementPair);
        movementPair.first = MT_LAUFEN;
        movementPair.second = new Laufen (this);
        mMovementMap.insert(movementPair);
        movementPair.first = MT_RENNEN;
        movementPair.second = new Rennen (this);
        mMovementMap.insert(movementPair);
        movementPair.first = MT_RUECKWAERTS_GEHEN;
        movementPair.second = new RueckwaertsGehen (this);
        mMovementMap.insert(movementPair);
        movementPair.first = MT_RUECKWAERTS_JOGGEN;
        movementPair.second = new RueckwaertsJoggen (this);
        mMovementMap.insert(movementPair);
        movementPair.first = MT_SEITWAERTS_GEHEN;
        movementPair.second = new SeitwaertsGehen (this);
        mMovementMap.insert(movementPair);
        movementPair.first = MT_SCHLEICHEN;
        movementPair.second = new Schleichen (this);
        mMovementMap.insert(movementPair);
        movementPair.first = MT_HOCHSPRUNG;
        movementPair.second = new Hochsprung (this);
        mMovementMap.insert(movementPair);
        movementPair.first = MT_WEITSPRUNG;
        movementPair.second = new Weitsprung (this);
        mMovementMap.insert(movementPair);
    }

    CreatureController::~CreatureController()
    {
        MovementMap::iterator iter;
        for(iter = mMovementMap.begin(); iter != mMovementMap.end(); iter++)
            delete iter->second;
        mMovementMap.erase(mMovementMap.begin(), mMovementMap.end());

        mCreature->getActor()->getPhysicalThing()->setPhysicsController(NULL);

        CreatureControllerManager::getSingleton().remove(this);
    }

    CreatureController::MovementType CreatureController::getMovementId() const
    {
        if (mMovement == NULL)
            return MT_NONE;
        else
            return mMovement->getId();
    }

    int CreatureController::getCurrentGS() const
    {
        int act_gs = mCreature->getWert(Creature::WERT_GS);
        ///@todo wy does this not work
        //act_gs -= mCreature->getWert(Creature::WERT_BE);
        return max(act_gs,1);
    }

    MeshAnimation *CreatureController::setAnimation(const Ogre::String &name, Ogre::Real speed, unsigned int timesToPlay, const Ogre::String &collisionName, Real weight)
    {
        MeshObject* mesh = dynamic_cast<MeshObject*>(mCreature->getActor()->getControlledObject());
        PhysicalThing* pt = mCreature->getActor()->getPhysicalThing();
        MeshAnimation *meshAnim(NULL);

        if( mLastAnimationName != name)
        {
            const Ogre::String *pCollisionName = &collisionName;
            if( collisionName == "" )
                pCollisionName = &name;

            if( *pCollisionName != mLastCollisionName )
            {
                pt->fitToPose(*pCollisionName);
                mLastCollisionName = *pCollisionName;
            }


            if ( mStillWeightedAnimationName != "" )
            {
                mesh->stopAnimation(mStillWeightedAnimationName);
                mStillWeightedAnimationName = "";
            }


            if( mesh->hasAnimation(mLastAnimationName) )
            {
                if ( weight == 1 )
                    mesh->stopAnimation(mLastAnimationName);
                else
                {
                    MeshAnimation *lastAnimation = mesh->getAnimation(mLastAnimationName);
                    if( lastAnimation != NULL )
                    {
                        mStillWeightedAnimationName = mLastAnimationName;
                        lastAnimation->setWeight(1-weight);
                    }
                }
            }

            meshAnim = mesh->startAnimation(name, speed, timesToPlay);
            if( meshAnim )
            {
                meshAnim->setWeight(weight);
                mLastAnimationName = name;
                mLastAnimationSpeed = speed;
            }
        }
        else
        {
            meshAnim = mesh->getAnimation(name);
            if( mLastAnimationSpeed != speed )
            {
                meshAnim->setSpeed(speed);
                mLastAnimationSpeed = speed;
                meshAnim->setWeight(weight);
            }
            if( mStillWeightedAnimationName != "" )
            {
                MeshAnimation *lastAnimation = mesh->getAnimation(mStillWeightedAnimationName);
                if( lastAnimation !=  NULL )
                {
                    lastAnimation->setWeight(1-weight);
                    if( weight == 1 )
                    {
                        mesh->stopAnimation(mStillWeightedAnimationName);
                        mStillWeightedAnimationName = "";
                    }
                }
                else
                {
                    mStillWeightedAnimationName = "";
                }
            }
        }

        return meshAnim;
    }

    Ogre::Real CreatureController::getAnimationTimePlayed() const
    {
        MeshObject* mesh = dynamic_cast<MeshObject*>(mCreature->getActor()->getControlledObject());
        MeshAnimation *meshAnim = mesh->getAnimation(mLastAnimationName);
        if( meshAnim != NULL )
            return meshAnim->getTimePlayed()/meshAnim->getLength();
        return 0;
    }

    Ogre::Vector3 CreatureController::getVelocity() const
    {
        return
            mCreature->getActor()->getPhysicalThing()->getOrientation().Inverse() *
            mCreature->getActor()->getPhysicalThing()->_getBody()->getVelocity();
    }

    Ogre::Vector3 CreatureController::getOmega() const
    {
        return mCreature->getActor()->getPhysicalThing()->_getBody()->getOmega();
    }

    bool CreatureController::run(Real elapsedTime)
    {
        if(mMovement != NULL)
        {
            mLastMovementChange += elapsedTime;
            if( !mMovement->isPossible() )
            {
                setMovement(mMovement->getFallBackMovement(), mDirection, mRotation);
                if( mMovement == NULL )
                    return false;
            }

            Vector3 rotation(mRotation);
            mRotation = Vector3::ZERO;
            return mMovement->run(elapsedTime, mDirection, rotation);
        }
        return false;
    }

    void CreatureController::OnApplyForceAndTorque(PhysicalThing* thing)
    {
        Vector3 force, torque;
        OgreNewt::Body *body = thing->_getBody();
        force = Vector3::ZERO;
        torque = Vector3::ZERO;
        OgreNewt::World *world = PhysicsManager::getSingleton()._getNewtonWorld();
        Real timestep = world->getTimeStep();
        Real mass;
        Vector3 inertia;
        body->getMassMatrix(mass, inertia);

        if(mMovement != NULL)
        {
            mMovement->calculateForceAndTorque(force, torque, timestep);
            force = thing->getOrientation() * force;
        }
        else
        {
            // don't move
            force = - mass * body->getVelocity() / 1 * timestep;
            force.y = 0;
        }
        force += mass * PhysicsManager::getSingleton().getGravity();
        body->setForce(force);
        body->setTorque(torque);
    }

    int CreatureController::userProcess()
    {
        // own collision handling (floor, in order to get information for mAbstractLocation)
        Vector3 point;
        Vector3 normal;
        getContactPositionAndNormal(point, normal);

        // determine if this contact is with the floor.
        // Meaning the contact normal has an angle to UNIT_Y of 20 or less.
        Degree angle = Math::ACos(normal.dotProduct(Vector3::UNIT_Y));

        Vector3 charPos;
        Quaternion charOri;
        mCreature->getActor()->getPhysicalThing()->_getBody()->getPositionOrientation(charPos, charOri);
        bool isFloorCollision(false);

        //AxisAlignedBox CharAab = mCharBody->getCollision()->getAABB();
        //Real charHeight = CharAab.getMaximum().y - CharAab.getMinimum().y;
        Real stepHeight = point.y - charPos.y;

        if( stepHeight < 0.5f )
            isFloorCollision = true;

        if ( isFloorCollision )
        {
            setAbstractLocation(AL_FLOOR);
            if(stepHeight > 0.1f)
                setContactNormalAcceleration(10);
            setContactElasticity(0.0f);
            mLastFloorContact = 0.0f;
        }
        else
        {
            mLastFloorContact += PhysicsManager::getSingleton()._getNewtonWorld()->getTimeStep();
            if(mLastFloorContact >= 2.2f || getAbstractLocation() == AL_AIRBORNE)
                setAbstractLocation(AL_AIRBORNE);
        }


        if(mMovement != NULL)
        {
            // i hope this will copy the protected members of the contact callback
            OgreNewt::ContactCallback *movement = mMovement;
            *movement = (OgreNewt::ContactCallback)(*this);
            return movement->userProcess();
        }

        // return one to tell Newton we want to accept this contact
        return 1;
    }

    AbstractMovement *CreatureController::getMovementFromId(CreatureController::MovementType id)
    {
        MovementMap::iterator iter = mMovementMap.find(id);
        if(iter == mMovementMap.end())
        {
            Throw(IllegalArgumentException, "Der angegebene Bewegungstyp wurde nicht gefunden.");
        }
        return iter->second;
    }


    bool CreatureController::setMovement(MovementType type, Vector3 direction, Vector3 rotation)
    {
        if( mMovement != NULL )
        {
            if( mMovement->getId() == type )
            {
                CreatureControllerManager::getSingleton().setActive(this);
                mDirection = direction;
                mRotation = rotation;
                return true;
            }
        }

        if( mMovement != NULL )
        {
            if( !mMovement->canChangeToMovement(type) )
            {
                mMovement->requestChangeToMovement(type);
                return false;
            }
        }

        AbstractMovement *movement = getMovementFromId(type);


        while(movement != NULL)
        {
            if(movement->isPossible())
            {
                CreatureControllerManager::getSingleton().setActive(this); // runs the old movement if idle!
                if(mMovement == NULL)
                {
                    mLastMovementType = MT_NONE;
                }
                else
                {
                    mLastMovementType = mMovement->getId();
                    mMovement->deactivate();
                }
                mMovement = movement;
                mMovement->activate();
                mDirection = direction;
                mRotation = rotation;
                mLastMovementChange = 0;
                return true;
            }

            movement = getMovementFromId(movement->getFallBackMovement());
        }

        return false;
    }



    Ogre::Radian CreatureController::getYaw()
    {
        Radian yaw = mCreature->getActor()->getWorldOrientation().getYaw();

        AbstractMovement *drehen = getMovementFromId(MT_DREHEN);

        if( drehen == NULL )
            return yaw;

        if( !drehen->isActive() )
            return yaw;

        return Radian(drehen->getMovementDefinedValue());
    }
}
