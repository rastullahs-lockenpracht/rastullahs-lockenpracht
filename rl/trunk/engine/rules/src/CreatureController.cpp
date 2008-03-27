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
#include "TimeSource.h"
#include "GameObjectManager.h"
#include "PhysicsMaterialRaycast.h"
#include "RulesMessages.h"



using namespace Ogre;
using namespace std;

namespace rl
{
    const Ogre::String CreatureController::PROPERTY_CREATUREID = "creature_id";

    const Real minSquaredSpeed = 0.6;

    // TODO: wenn Fallen nicht möglich ist (auf dem Boden und bewusstlos / Tod)

    class Fallen : public AbstractMovement
    {
    public:
        Fallen(CreatureController *creature) :
          AbstractMovement(creature)
          {
              mAnim = mMovingCreature->getCreature()->getAnimation("fallen");
              //mAnim = mMovingCreature->getCreature()->getAnimation("fallen_anfang");
              //mAnim = mMovingCreature->getCreature()->getAnimation("fallen_landung");
              // fallen_landung_verletzt ?
          }
        virtual CreatureController::MovementType getId() const {return CreatureController::MT_FALLEN;}
        virtual CreatureController::MovementType getFallBackMovement() const {return CreatureController::MT_NONE;}
        virtual void activate()
        {
	    mVel = 0;
        }
        virtual void deactivate()
        {
            std::ostringstream oss;
            oss << "Fallen v: " << mVel << "   ermittelte Hoehe: " << mVel*mVel/(2* fabs(PhysicsManager::getSingleton().getGravity().y));
            int h = int(mVel*mVel/(2* fabs(PhysicsManager::getSingleton().getGravity().y)));
	    oss << "    verwendete Hoehe: " << h << "m";

	    if( h < 4 && h > 0 ) // nicht in den Regeln, aber angenehmer, bei gelunger GE-Probe noch aufgefangen
	    {
		int probe = mMovingCreature->getCreature()->doEigenschaftsprobe("GE", h-6);
		if( probe == RESULT_PATZER )
		{
		    h++;
		    oss << "    GE-Patzer!";
		}
		else if( probe >= 0 )
		{
		    h = 0;
		    oss << "    Nochmal geschickt gelandet!";
		}
	    }


            if( h > 0 )
            {

                std::multiset<int> wuerfel;
                for(int i = 0; i < h; i++)
                    wuerfel.insert( DsaManager::getSingleton().rollD6() );
                int probenErschwernis = h;
                if( probenErschwernis > 10 )
                    probenErschwernis = 10;

                int taw = 0;
                if( mMovingCreature->getCreature()->hasTalent("Körperbeherrschung") )
                {
                    taw = mMovingCreature->getCreature()->doTalentprobe("Körperbeherrschung", probenErschwernis);
                }

                for( int i = 0; i < taw; i++)
                {
                    if( wuerfel.size() <= 0 )
                        break;
                    wuerfel.erase(--(wuerfel.end()));
                }

                int sum = 0;
                for( std::multiset<int>::iterator it = wuerfel.begin(); it != wuerfel.end(); it++)
                    sum += *it;

                mMovingCreature->getCreature()->damageLe(sum);

                oss << "    Schaden: " << sum;
            }
            LOG_MESSAGE(Logger::RULES, oss.str());

        }
        virtual bool calculateBaseVelocity(Real &velocity)
        {
            velocity = 0.0f;
            return isPossible();
        }
        virtual bool isPossible() const
        {
            return
                mMovingCreature->getAbstractLocation() == CreatureController::AL_AIRBORNE;
        }
        virtual void calculateForceAndTorque(Vector3 &force, Vector3 &torque, Real timestep)
        {
        }
        virtual bool run(Ogre::Real elapsedTime,  Ogre::Vector3 direction, Ogre::Vector3 rotation)
        {
            mVel = max(-mMovingCreature->getVelocity().y,mVel);
            mMovingCreature->setAnimation(mAnim.first, mAnim.second, 0);
            return true;
        }
        virtual void setAnimation(Ogre::Real elapsedTime)
        {
        }
        virtual void applyAuChanges(Ogre::Real elapsedTime)
        {
        }
        virtual bool isDirectionPossible(Ogre::Vector3 &direction) const
        {
            direction = Vector3::ZERO;
            return false;
        }
        virtual bool isRotationPossible(Ogre::Vector3 &rotation) const
        {
            rotation = Vector3::ZERO;
            return false;
        }
    protected:
        Creature::AnimationSpeedPair mAnim;
        Real mVel;
    };

    class Stehen : public AbstractMovement
    {
    public:
        Stehen(CreatureController *creature) : 
            AbstractMovement(creature), 
            mVelocity(Vector3::ZERO), 
            mRotationMovement(NULL), 
            mStepRecognitionMovement(NULL)
            {
                mAnimStehenLinks = creature->getCreature()->getAnimation("stehen_links");
                mAnimStehenRechts = creature->getCreature()->getAnimation("stehen_rechts");
                mAnimStehen = creature->getCreature()->getAnimation("stehen");
            }
        virtual CreatureController::MovementType getId() const {return CreatureController::MT_STEHEN;}
        virtual CreatureController::MovementType getFallBackMovement() const {return CreatureController::MT_FALLEN;}
        virtual void activate()
        {
            AbstractMovement::activate();
            getRotationMovement()->activate();
            getStepRecognitionMovement()->activate();
        }
        virtual void deactivate()
        {
            AbstractMovement::deactivate();
            getRotationMovement()->deactivate();
            getStepRecognitionMovement()->deactivate();
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
                !(mMovingCreature->getCreature()->getLifeState() & (Effect::LS_IMMOBILE));
        }
        virtual void calculateForceAndTorque(Vector3 &force, Vector3 &torque, Real timestep)
        {
            getRotationMovement()->calculateForceAndTorque(force, torque, timestep);

            Real mass;
            Vector3 inertia;
            OgreNewt::Body *body = mMovingCreature->getCreature()->getActor()->getPhysicalThing()->_getBody();
            body->getMassMatrix(mass, inertia);

            Vector3 vel = mMovingCreature->getVelocity();
            vel.y = 0;
            Real delay = 0.05;//(2 * PhysicsManager::getSingleton().getMaxTimestep());
            //if(vel.squaredLength() > mVelocity.squaredLength())
            //    delay *= 1.5;
            Vector3 diff = (mVelocity - vel);
            force = mass * diff / delay;
            // additional remove movement in wrong direction
            force += mass * (mVelocity.normalisedCopy() * vel.length() - vel) / delay; // do we need this, does this improve the movement behaviour
            // (changing direction during slow movement makes char slide sideways)
            force.y = 0;

            getStepRecognitionMovement()->calculateForceAndTorque(force, torque, timestep);
        }
        virtual bool run(Ogre::Real elapsedTime,  Ogre::Vector3 direction, Ogre::Vector3 rotation)
        {
            direction.normalise();
            Real velocity;
            calculateBaseVelocity(velocity);
            mVelocity = direction * velocity;
            applyAuChanges(elapsedTime);
            setAnimation(elapsedTime);
            bool ret = false;
            if( getRotationMovement()->isPossible() )
                if( getRotationMovement()->run(elapsedTime, direction, rotation) )
                    ret = true;
            if( getStepRecognitionMovement()->isPossible() )
                if (getStepRecognitionMovement()->run(elapsedTime, direction, rotation) )
                    ret = true;
            return ret;
        }
        virtual void setAnimation(Ogre::Real elapsedTime)
        {
            Real omegaY = mMovingCreature->getCreature()->getActor()->getPhysicalThing()->_getBody()->getOmega().y;
            if( omegaY > Degree(20).valueRadians() )
                mMovingCreature->setAnimation(mAnimStehenRechts.first, mAnimStehenRechts.second);
            else
            {
                if( omegaY < Degree(-20).valueRadians() )
                    mMovingCreature->setAnimation(mAnimStehenLinks.first, mAnimStehenLinks.second);
                else
                    mMovingCreature->setAnimation(mAnimStehen.first, mAnimStehen.second);
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
        Creature::AnimationSpeedPair mAnimStehenRechts;
        Creature::AnimationSpeedPair mAnimStehenLinks;
        Creature::AnimationSpeedPair mAnimStehen;
        mutable AbstractMovement *mRotationMovement;
        mutable AbstractMovement *mStepRecognitionMovement;
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
        virtual AbstractMovement* getStepRecognitionMovement() const
        {
            if( mStepRecognitionMovement == NULL )
            {
                mStepRecognitionMovement = mMovingCreature->getMovementFromId(CreatureController::MT_STUFENERKENNUNG);
            }
            if( mStepRecognitionMovement == NULL )
            {
                Throw(NullPointerException, "Konnte Movement mit der Id MT_STUFENERKENNUNG nicht finden.");
            }
            return mStepRecognitionMovement;
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
                mMovingCreature->getAbstractLocation() == CreatureController::AL_FLOOR &&
                mMovingCreature->getCreature()->getAu() > 1 &&
                !(mMovingCreature->getCreature()->getLifeState() & (Effect::LS_IMMOBILE));
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

            torque = springAcc * inertia;
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

            Real diff = (orientation.getYaw() - mYaw).valueDegrees();
            return diff <= 0.5f && diff >= -0.5f;
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
        Gehen(CreatureController *creature) : Stehen(creature)
        {
            mAnim = creature->getCreature()->getAnimation("gehen");
        }
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
                !(mMovingCreature->getCreature()->getLifeState() & Effect::LS_IMMOBILE);
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
            Real speed =  -mMovingCreature->getVelocity().z;
            if( fabs(speed) < 0.1 )
            {
                Stehen::setAnimation(elapsedTime);
                return;
            }

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

            MeshAnimation *meshAnim = mMovingCreature->setAnimation(mAnim.first, speed * mAnim.second, 0, mAnim.first, weight );
            if( meshAnim != NULL && relTimeOffset != 0 )
                meshAnim->doAddTime(relTimeOffset*meshAnim->getLength());
        }
    protected:
        Creature::AnimationSpeedPair mAnim;
    };

    class Joggen : public Gehen
    {
    public:
        Joggen(CreatureController *creature) : Gehen(creature)
        {
            mAnim = creature->getCreature()->getAnimation("joggen");
        }
        virtual CreatureController::MovementType getId() const {return CreatureController::MT_JOGGEN;}
        virtual CreatureController::MovementType getFallBackMovement() const {return CreatureController::MT_GEHEN;}
        virtual bool calculateBaseVelocity(Real &velocity)
        {
            velocity = mMovingCreature->getCurrentGS() / 2.0f;
            return isPossible();
        }
        virtual bool isPossible() const
        {
            return Gehen::isPossible() &&
                mMovingCreature->getCreature()->getAu() > 6;
        }
        virtual void applyAuChanges(Ogre::Real elapsedTime)
        {
            if( mMovingCreature->getVelocity().squaredLength() < minSquaredSpeed )
                Stehen::applyAuChanges(elapsedTime);
        }
        virtual void setAnimation(Ogre::Real elapsedTime)
        {
            if( mMovingCreature->getVelocity().squaredLength() < minSquaredSpeed)
            {
                Stehen::setAnimation(elapsedTime);
                return;
            }

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

            MeshAnimation *meshAnim = mMovingCreature->setAnimation(mAnim.first, -mMovingCreature->getVelocity().z * mAnim.second, 0, mAnim.first, weight );
            if( meshAnim != NULL && relTimeOffset != 0)
                meshAnim->doAddTime(relTimeOffset*meshAnim->getLength());
        }
    };

    class Laufen : public Gehen
    {
    public:
        Laufen(CreatureController *creature) : Gehen(creature), mTimePerAu(1L), mLastProbe(0)
        {
            mAnim = creature->getCreature()->getAnimation("laufen");
        }
        virtual CreatureController::MovementType getId() const {return CreatureController::MT_LAUFEN;}
        virtual CreatureController::MovementType getFallBackMovement() const {return CreatureController::MT_JOGGEN;}
        virtual bool calculateBaseVelocity(Real &velocity)
        {
            velocity = mMovingCreature->getCurrentGS() / 1.6f; // was 2.0
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
            if( mMovingCreature->getVelocity().squaredLength()< minSquaredSpeed )
                Stehen::applyAuChanges(elapsedTime);
            else
                mMovingCreature->getCreature()->damageAu(elapsedTime/mTimePerAu);
        }
        virtual void setAnimation(Ogre::Real elapsedTime)
        {
            if( mMovingCreature->getVelocity().squaredLength() < minSquaredSpeed )
            {
                Stehen::setAnimation(elapsedTime);
                return;
            }

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

            MeshAnimation *meshAnim = mMovingCreature->setAnimation(mAnim.first, -mMovingCreature->getVelocity().z * mAnim.second, 0, mAnim.first, weight );
            if( meshAnim != NULL && relTimeOffset != 0)
                meshAnim->doAddTime(relTimeOffset*meshAnim->getLength());
        }
        virtual void activate()
        {
            Gehen::activate();
            doTalentProbeIfNecessary();
        }
        virtual void doTalentProbeIfNecessary()
        {
            Date now = DsaManager::getSingleton().getCurrentDate();
            if( mLastProbe + Date::ONE_SPIELRUNDE >= now || mLastProbe == 0 )
            {
                mTimePerAu = 120; // was 180
                if( mMovingCreature->getCreature()->hasTalent("Athletik") )
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
                else
                {
                    mTimePerAu = 120; // was 180
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
        Rennen(CreatureController *creature) : Gehen(creature), mVelocityImprovement(0), mLastProbe(0)
        {
            mAnim = creature->getCreature()->getAnimation("rennen");
        }
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
            if( mMovingCreature->getVelocity().squaredLength() < minSquaredSpeed )
                Stehen::applyAuChanges(elapsedTime);
            else
                mMovingCreature->getCreature()->damageAu(elapsedTime/1.5);
        }
        virtual void setAnimation(Ogre::Real elapsedTime)
        {
            if( mMovingCreature->getVelocity().squaredLength() < minSquaredSpeed )
            {
                Stehen::setAnimation(elapsedTime);
                return;
            }

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

            MeshAnimation *meshAnim = mMovingCreature->setAnimation(mAnim.first, fabs(mMovingCreature->getVelocity().z) *mAnim.second, 0, mAnim.first, weight );
            if( meshAnim != NULL && relTimeOffset )
                meshAnim->doAddTime(relTimeOffset*meshAnim->getLength());
        }
        virtual void activate()
        {
            Gehen::activate();
            doTalentProbeIfNecessary();
        }
        virtual void doTalentProbeIfNecessary()
        {
            Date now = DsaManager::getSingleton().getCurrentDate();
            if( mLastProbe + mMovingCreature->getCreature()->getAuMax() * Date::ONE_SECOND >= now || mLastProbe == 0)
            {
                mVelocityImprovement = 0;
                if( mMovingCreature->getCreature()->hasTalent("Athletik") )
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
                else
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
        RueckwaertsGehen(CreatureController *creature) : Gehen(creature)
        {
            mAnim = creature->getCreature()->getAnimation("gehen_rueckwaerts");
        }
        virtual CreatureController::MovementType getId() const {return CreatureController::MT_RUECKWAERTS_GEHEN;}
        virtual CreatureController::MovementType getFallBackMovement() const {return CreatureController::MT_STEHEN;}
        virtual bool calculateBaseVelocity(Real &velocity)
        {
            velocity = mMovingCreature->getCurrentGS() / 4.0;
            return isPossible();
        }
    };

    class RueckwaertsJoggen : public Joggen
    {
    public:
        RueckwaertsJoggen(CreatureController *creature) : Joggen(creature)
        {
            mAnim = creature->getCreature()->getAnimation("joggen_rueckwaerts");
        }
        virtual CreatureController::MovementType getId() const {return CreatureController::MT_RUECKWAERTS_JOGGEN;}
        virtual CreatureController::MovementType getFallBackMovement() const {return CreatureController::MT_RUECKWAERTS_GEHEN;}
        virtual bool calculateBaseVelocity(Real &velocity)
        {
            velocity = mMovingCreature->getCurrentGS() / 3.2;
            return isPossible();
        }
    };


    class SeitwaertsGehen : public Gehen
    {
    public:
        SeitwaertsGehen(CreatureController *creature) : Gehen(creature), mLeft(true)
        {
            mAnim1 = creature->getCreature()->getAnimation("seitwaerts_rechts");
            mAnim = creature->getCreature()->getAnimation("seitwaerts_links");
        }
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
                mMovingCreature->setAnimation(mAnim1.first, mAnim1.second * -mMovingCreature->getVelocity().x);
            else
                mMovingCreature->setAnimation(mAnim.first, mAnim.second * -mMovingCreature->getVelocity().x);
        }
    protected:
        bool mLeft;
        Creature::AnimationSpeedPair mAnim1;
    };

    class Schleichen : public Gehen
    {
    public:
        Schleichen(CreatureController *creature) : Gehen(creature), mState(UP), mTimer(0)
        {
            mAnim = creature->getCreature()->getAnimation("schleichen");
            mAnim1= creature->getCreature()->getAnimation("schleiche_vorwaerts");
            mAnim2 = creature->getCreature()->getAnimation("stehen_zu_schleichen");
            mAnim3= creature->getCreature()->getAnimation("schleichen_zu_stehen");
            mAnimStehen= creature->getCreature()->getAnimation("stehen");
        }
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
                //setAnimation(elapsedTime);
                direction.normalise();
                Real velocity;
                calculateBaseVelocity(velocity);
                mVelocity = direction * velocity;
                if( direction == Vector3::ZERO || (-mMovingCreature->getVelocity().z) < 0.1)
                    mMovingCreature->setAnimation(mAnimStehen.first, mAnimStehen.second);
                else
                    mMovingCreature->setAnimation(mAnim.first, mAnim.second * -mMovingCreature->getVelocity().z);
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
            Gehen::activate();
            mState = UPTODOWN;
            mMovingCreature->setAnimation(mAnim2.first,mAnim3.second,1,mAnimStehen.first);
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
                mMovingCreature->setAnimation(mAnim3.first,mAnim3.second,1,mAnimStehen.first);
                mTimer = 0;
            }
        }
    protected:
        enum {UP, DOWN, UPTODOWN, DOWNTOUP} mState;
        Real mTimer;
        Creature::AnimationSpeedPair mAnim1;
        Creature::AnimationSpeedPair mAnim2;
        Creature::AnimationSpeedPair mAnim3;
    };


    class Hochsprung : public AbstractMovement
    {
    public:
        Hochsprung(CreatureController *creature) : AbstractMovement(creature), mState(DOWN), mHeight(0), mJumpNow(false), mTimer(0), mMoveForward(0)
        {
            mAnimation = creature->getCreature()->getAnimation("hochsprung");
            mAnimationAbsprung = creature->getCreature()->getAnimation("hochsprung_absprung");
            mAnimationLandung = creature->getCreature()->getAnimation("hochsprung_landung");
            mAnimationForCollision = creature->getCreature()->getAnimation("stehen");  // wird das hier gebraucht?
        }
        virtual CreatureController::MovementType getId() const {return CreatureController::MT_HOCHSPRUNG;}
        virtual CreatureController::MovementType getFallBackMovement() const {return CreatureController::MT_STEHEN;}
        virtual void activate()
        {
            AbstractMovement::activate();
            mState = DOWNTOUP;
            mMovingCreature->setAnimation(mAnimationAbsprung.first,mAnimationAbsprung.second,1,mAnimationForCollision.first);
            mTimer = 0;
            calculateBaseVelocity(mHeight);

            if( mMovingCreature->getCreature()->hasTalent("Athletik") )
            {
                int tap = mMovingCreature->getCreature()->doTalentprobe("Athletik", 0);
                if( tap > 0 )
                {
                    if( tap == RESULT_SPEKT_AUTOERFOLG )
                    {
                        mHeight += mMovingCreature->getCreature()->getTalent("Athletik") / 12.5;
                    }
                    else if( tap == RESULT_AUTOERFOLG )
                    {
                        mHeight += mMovingCreature->getCreature()->getTalent("Athletik") / 25.0;
                    }
                    else
                    {
                        mHeight += tap / 25.0;
                    }
                }
            }
            else
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
                    !(mMovingCreature->getCreature()->getLifeState() & (Effect::LS_IMMOBILE));
            else
                return
                mMovingCreature->getAbstractLocation() == CreatureController::AL_FLOOR &&
                mMovingCreature->getCreature()->getAu() > 8 &&
                mMovingCreature->getCreature()->getAu() > mMovingCreature->getCreature()->getAuMax()/3.0 &&
                !(mMovingCreature->getCreature()->getLifeState() & (Effect::LS_IMMOBILE));
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
            else if (mState == UP && mMoveForward != 0) // try to move forward, if wanted
            {
                // low velocity towards this direction
                if(mMoveForward > 0)
                    mMoveForward = 1.0;
                else
                    mMoveForward = -1.0;



                Real mass;
                Vector3 inertia;
                OgreNewt::Body *body = mMovingCreature->getCreature()->getActor()->getPhysicalThing()->_getBody();
                body->getMassMatrix(mass, inertia);

                Vector3 vel = mMovingCreature->getVelocity();
                Real delay = 0.05;//(2 * PhysicsManager::getSingleton().getMaxTimestep());
                Real diff = (mMoveForward - vel.z);
                force.z += mass * diff / delay;
            }

            Vector3 omega = mMovingCreature->getCreature()->getActor()->getPhysicalThing()->_getBody()->getOmega();
            torque = -omega / PhysicsManager::getSingleton().getMaxTimestep() * 2 * mass;
        }
        virtual bool run(Ogre::Real elapsedTime,  Ogre::Vector3 direction, Ogre::Vector3 rotation)
        {
            mMoveForward = direction.z;

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
                    mMovingCreature->setAnimation(mAnimationLandung.first, mAnimationLandung.second, 1, mAnimationForCollision.first);
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
                    mMovingCreature->setAnimation(mAnimation.first, mAnimation.second);
                    mJumpNow = true;
                    mTimer = 0;
                }
            }
            return false;
        }
        virtual bool isDirectionPossible(Ogre::Vector3 &direction) const
        {
            Vector3 oldDirection(direction);
            direction.x = 0;
            if(direction.y < 0)
                direction.y = 0;
            return oldDirection.x == 0 && oldDirection.y > 0;
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
        Ogre::Real mMoveForward;
        Creature::AnimationSpeedPair mAnimation;
        Creature::AnimationSpeedPair mAnimationAbsprung;
        Creature::AnimationSpeedPair mAnimationLandung;
        Creature::AnimationSpeedPair mAnimationForCollision;
    };


    class Weitsprung : public AbstractMovement
    {
    public:
        Weitsprung(CreatureController *creature) :
          AbstractMovement(creature), mState(DOWN), mWidth(0),
              mJumpNow(false), mTimer(0), mApplyForceTime(0.12),
              mApplyForceTimer(0), mLastForce(Vector3::ZERO),
              mVelocityBeforeJump(0), mTanJumpAngle(Math::Tan(Degree(17)))
        {
            mAnimation = creature->getCreature()->getAnimation("weitsprung");
            mAnimationAbsprung = creature->getCreature()->getAnimation("weitsprung_absprung");
            mAnimationLandung = creature->getCreature()->getAnimation("weitsprung_landung");
            mAnimationForCollision = creature->getCreature()->getAnimation("rennen"); // wird das hier gebraucht?
        }
        virtual CreatureController::MovementType getId() const {return CreatureController::MT_WEITSPRUNG;}
        virtual CreatureController::MovementType getFallBackMovement() const {return CreatureController::MT_STEHEN;}
        virtual void activate()
        {
            AbstractMovement::activate();
            mState = DOWNTOUP;
            mMovingCreature->setAnimation(mAnimationAbsprung.first,mAnimationAbsprung.second,1,mAnimationForCollision.first);
            mTimer = 0;
            calculateBaseVelocity(mWidth);


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

            if( mMovingCreature->getCreature()->hasTalent("Athletik") )
            {
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
                    !(mMovingCreature->getCreature()->getLifeState() & (Effect::LS_IMMOBILE));
            else
                return
                mMovingCreature->getAbstractLocation() == CreatureController::AL_FLOOR &&
                mMovingCreature->getCreature()->getAu() > 8 &&
                mMovingCreature->getCreature()->getAu() > mMovingCreature->getCreature()->getAuMax()/3.0 &&
                !(mMovingCreature->getCreature()->getLifeState() & (Effect::LS_IMMOBILE));
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
                if( mTimer < 0.1f )
                {
                    mMovingCreature->setAbstractLocation( CreatureController::AL_AIRBORNE );
                }
                else if( mMovingCreature->getAbstractLocation() != CreatureController::AL_AIRBORNE )
                {
                    mState = UPTODOWN;
                    mMovingCreature->setAnimation(mAnimationLandung.first, mAnimationLandung.second, 1, mAnimationForCollision.first);
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
                    mMovingCreature->setAnimation(mAnimation.first, mAnimation.second);
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
        Creature::AnimationSpeedPair mAnimation;
        Creature::AnimationSpeedPair mAnimationAbsprung;
        Creature::AnimationSpeedPair mAnimationLandung;
        Creature::AnimationSpeedPair mAnimationForCollision;
    };


    class StepRecognition : public AbstractMovement
    {
    public:
        StepRecognition(CreatureController *creature) : 
            AbstractMovement(creature), 
            mMoveToNextTarget(false)
        {
            mLinearSpringK = 600.0f;
            Real relationCoefficient = 1.0f;
            mLinearDampingK = relationCoefficient * 2.0f * Math::Sqrt(mLinearSpringK);
        }
        virtual CreatureController::MovementType getId() const {return CreatureController::MT_STUFENERKENNUNG;}
        virtual CreatureController::MovementType getFallBackMovement() const {return CreatureController::MT_NONE;}
        virtual void activate()
        {
            AbstractMovement::activate();
            mMoveToNextTarget = false;
        }
        virtual void deactivate()
        {
            AbstractMovement::deactivate();
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
                !(mMovingCreature->getCreature()->getLifeState() & (Effect::LS_DEAD | Effect::LS_UNCONSCIOUS | Effect::LS_SLEEPING));
        }
        virtual void calculateForceAndTorque(Vector3 &force, Vector3 &torque, Real timestep)
        {
            // move to nextTarget
            if( mMoveToNextTarget )
            {
                Real mass;
                Vector3 inertia;
                OgreNewt::Body *body = mMovingCreature->getCreature()->getActor()->getPhysicalThing()->_getBody();
                body->getMassMatrix(mass, inertia);

                Vector3 pos = mMovingCreature->getCreature()->getPosition();
                Vector3 diff = mNextTarget - pos;

                Vector3 vel = body->getVelocity();

                force.y = mass*( mLinearSpringK*diff.y - mLinearDampingK*vel.y );
                std::ostringstream oss;
                oss << "Testing Step-Recognition: Step force: " << force.y;
                LOG_MESSAGE(Logger::RULES, oss.str());
            }
        }
        virtual bool run(Ogre::Real elapsedTime,  Ogre::Vector3 direction, Ogre::Vector3 rotation)
        {
            if( !mMoveToNextTarget ) // check if we need to move up for a step
            {
                // raycast in the direction we should move to
                Vector3 globalDir = mMovingCreature->getCreature()->getOrientation() * direction; // the direction in global space
                if( globalDir == Vector3::ZERO )
                    return true;

                Vector3 vel = mMovingCreature->getCreature()->getActor()->getPhysicalThing()->getVelocity();
                vel.y = 0;
                Real raylen = vel.length() / 3;  // use longer ray, if higher velocity


                // raycasts
                PhysicsMaterialRaycast::MaterialVector materialVector;
                materialVector.push_back(PhysicsManager::getSingleton().getMaterialID("default")); // should we perhaps only use level here?
                materialVector.push_back(PhysicsManager::getSingleton().getMaterialID("level"));

                Vector3 start = mMovingCreature->getCreature()->getPosition() + Vector3::UNIT_Y * 0.1f;
                globalDir.y = 0;
                globalDir.normalise();
                Vector3 end = start + globalDir*raylen;

                bool foundbody = false;
                Real foundDistance = 0;

                RaycastInfo info;
                do
                {
                    info = 
                        mRaycast.execute(
                            PhysicsManager::getSingleton()._getNewtonWorld(),
                            &materialVector,
                            start, end);

                    // do we need to check bodies left and right of this ray? (step width?)


                    // already found nearer body
                    if( foundbody )
                    {
                        if( info.mBody && (info.mDistance >= foundDistance + 0.19) || // step deep enough
                            !info.mBody )
                        {
                            // found a step
                            mMoveToNextTarget = true;
                            mNextTarget = start + globalDir*raylen*foundDistance + 0.1 * globalDir;
                            std::ostringstream oss;
                            oss << "Testing Step-Recognition: Next Step: " << mNextTarget;
                            LOG_MESSAGE(Logger::RULES, oss.str());
                            break;
                        }
                    }

                    if( info.mBody )
                    {
                        foundbody = true;
                        foundDistance = info.mDistance;
                    }


                    start += Vector3::UNIT_Y * 0.05f;
                    end += Vector3::UNIT_Y * 0.05f;
                }
                while( info.mBody && (start - mMovingCreature->getCreature()->getPosition()).y <= 0.5 );
            }


            // check if the target is still needed
            // perform check also to verify found step
            if( mMoveToNextTarget )
            {
                Vector3 diffToTarget = mNextTarget - mMovingCreature->getCreature()->getPosition();
                diffToTarget.y = 0;
                
                // different direction
                Vector3 globalDir = mMovingCreature->getCreature()->getOrientation() * direction; // the direction in global space
                if( globalDir == Vector3::ZERO )
                {
                    mMoveToNextTarget = false;
                    LOG_MESSAGE(Logger::RULES, "Testing Step-Recognition: Step direction null");
                    return false;
                }

                // target reached
                if( diffToTarget.squaredLength() < 0.01 )
                {
                    mMoveToNextTarget = false;
                    LOG_MESSAGE(Logger::RULES, "Testing Step-Recognition: Step reached");
                    return false;
                }

                // different direction
                if( !diffToTarget.directionEquals(globalDir, Degree(5)) )
                {
                    mMoveToNextTarget = false;
                    LOG_MESSAGE(Logger::RULES, "Testing Step-Recognition: Step direction wrong");
                    return false;
                }
            }

            return mMoveToNextTarget;
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
            rotation = Vector3::ZERO;
            return oldRotation == Vector3::ZERO;
        }
    protected:
        bool mMoveToNextTarget;
        Vector3 mNextTarget;
        Real mLastSquaredDistance;
        Real mLinearSpringK, mLinearDampingK;
        PhysicsMaterialRaycast mRaycast;
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
        mLastFloorContact(TimeSourceManager::getSingleton().getTimeSource(TimeSource::REALTIME_INTERRUPTABLE)->getClock()),
        mLastFloorContact2(mLastFloorContact)
    {
        mGameObjectId = mCreature->getId();

        mOldMaterialId = mCreature->getActor()->getPhysicalThing()->_getBody()->getMaterialGroupID();
        const OgreNewt::MaterialID *material = PhysicsManager::getSingleton().getMaterialID("character");
        mCreature->getActor()->getPhysicalThing()->setMaterialID(material);

        mCreature->getActor()->getPhysicalThing()->setPhysicsController(this);


        std::pair<MovementType, AbstractMovement*> movementPair;
        movementPair.first = MT_NONE;
        movementPair.second = NULL;
        mMovementMap.insert(movementPair);
        movementPair.first = MT_FALLEN;
        movementPair.second = new Fallen (this);
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
        movementPair.first = MT_STUFENERKENNUNG;
        movementPair.second = new StepRecognition (this);
        mMovementMap.insert(movementPair);

        mMessageType_GameObjectsLoaded_Handler = MessagePump::getSingleton().addMessageHandler<MessageType_GameObjectsLoaded>(
                boost::bind(&CreatureController::refetchCreature, this));
    }

    CreatureController::~CreatureController()
    {
        MovementMap::iterator iter;
        for(iter = mMovementMap.begin(); iter != mMovementMap.end(); iter++)
            delete iter->second;
        mMovementMap.erase(mMovementMap.begin(), mMovementMap.end());

        if( mCreature->getActor() != NULL )
        {
            if( mCreature->getActor()->getPhysicalThing() != NULL )
            {
                mCreature->getActor()->getPhysicalThing()->setPhysicsController(NULL);

                mCreature->getActor()->getPhysicalThing()->setMaterialID(mOldMaterialId);
            }
        }
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

    Ogre::Real CreatureController::getMaximumSpeed() const
    {
        return getCurrentGS() * 1.2; // GS = m/KR
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
                meshAnim->setPaused(false);
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
                meshAnim->setPaused(false);
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
        Time timeSinceLastFloorContact = 
            TimeSourceManager::getSingleton().getTimeSource(TimeSource::REALTIME_INTERRUPTABLE)->getClock()
            - mLastFloorContact;
        Time timeSinceLastFloorContact2 = 
            TimeSourceManager::getSingleton().getTimeSource(TimeSource::REALTIME_INTERRUPTABLE)->getClock()
            - mLastFloorContact2;
        Vector3 speed = getVelocity();
        if( getAbstractLocation() == AL_AIRBORNE )
        {
            // find a reason why we now are AL_FLOOR
            if( timeSinceLastFloorContact < Time(Date::ONE_SECOND)*0.09 &&
                -speed.y < 0.4 )
            {
                setAbstractLocation(AL_FLOOR);
            }
        }
        else
        {
            // find a reason why we now are AL_AIRBORNE
            if( timeSinceLastFloorContact >= Time(Date::ONE_SECOND*0.1) && // 2 seconds?
                -speed.y > 0.4 )
            {
                setAbstractLocation(AL_AIRBORNE);
            }
        }

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

        AxisAlignedBox CharAab = mCreature->getActor()->getPhysicalThing()->_getBody()->getCollision()->getAABB();
        Real charHeight = CharAab.getMaximum().y - CharAab.getMinimum().y;
        Real stepHeight = point.y - charPos.y;

        if( stepHeight < charHeight/2 )
            isFloorCollision = true;

        if ( isFloorCollision )
        {
            Time time = TimeSourceManager::getSingleton().getTimeSource(TimeSource::REALTIME_INTERRUPTABLE)->getClock();
            if( time != mLastFloorContact )
            {
                mLastFloorContact2 = mLastFloorContact;
                mLastFloorContact = time;
            }
        }

        if( stepHeight < 0.4 )
        {
            if(stepHeight > 0.05f) // experimantal value, 
                                   // too low means the creature glides upwards on inclined planes, 
                                   // too high means the creature stops if moving slowly onto a step because of the friction
            {
                //setContactNormalAcceleration(5);
                rotateTangentDirections(charOri*mDirection + Vector3::UNIT_Y);
                setContactTangentAcceleration(5,0);
                setContactFrictionState(1,0);
                setContactFrictionState(1,1);
            }
            else
            {
                setContactFrictionState(1,0);
                setContactFrictionState(1,1);
            }
            //setContactTangentAcceleration(5);
            //setContactElasticity(0.0f);
        }
        else
        {
            Vector3 vel; // prevent going upwards if running against small things
            vel = mCreature->getActor()->getPhysicalThing()->getVelocity();
            if( vel.y >= 0 )
            {
                setContactFrictionState(1,0);
                setContactFrictionState(1,1);    
            }
            else
            {
                setContactFrictionState(0,0);
                setContactFrictionState(0,1);
            }
        }

        if(mMovement != NULL)
        {
            // @XXX Evil code!
            // Protected members from type OgreNewt::ContactCallback have to be overridden in order
            // for the movements to work. This is because these members are used by OgreNewt functions
            // for processing this contact. Should probably be solved in OgreNewt directly.
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
                if( movement != mMovement )
                {
                    // runs the old movement if idle!
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
                    mLastMovementChange = 0;
                }
                mDirection = direction;
                mRotation = rotation;
                
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

    bool CreatureController::refetchCreature()
    {
        mCreature = static_cast<Creature*>(GameObjectManager::getSingleton().getGameObject(mGameObjectId));

        //mOldMaterialId = mCreature->getActor()->getPhysicalThing()->_getBody()->getMaterialGroupID();

        const OgreNewt::MaterialID *material = PhysicsManager::getSingleton().getMaterialID("character");
        mCreature->getActor()->getPhysicalThing()->setMaterialID(material);

        mCreature->getActor()->getPhysicalThing()->setPhysicsController(this);

        return false;
    }
}
