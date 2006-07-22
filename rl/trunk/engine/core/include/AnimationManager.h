/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __AnimationManager_H__
#define __AnimationManager_H__

#include <map>
#include <set>
#include <OgreSingleton.h>

#include "GameTask.h"
#include "CorePrerequisites.h"

namespace rl {

class Actor;
class BaseAnimation;
class MeshAnimation;
class TrackAnimation;
class FadeAnimation;
class MeshObject;


/** 
	Diese Klasse verwaltet sämtliche Animationen und kümmert sich um das Starten und Stoppen dieser
	@see Animation, TrackAnimation
*/
class _RlCoreExport AnimationManager
  : public GameTask,
    protected Ogre::Singleton<AnimationManager>
{
public:
	/// Interpolations-Modus, Spline ist besser, braucht auch mehr Perfomance
	enum InterpolationMode { IM_LINEAR, IM_SPLINE };
	/// RotationsInterpolations-Modus, Spherical ist genauer, aber aufwändiger
	enum RotationInterpolationMode { RIM_LINEAR, RIM_SPHERICAL };

    /** Default Constructor */
	AnimationManager( );
	/** Default Deconstructor */
    virtual ~AnimationManager();

	/** Erzeugt eine Animation, trägt diese ein und beginnt
		sofort mit dem Abspielen, wenn gewünscht.
		@param	animState Der AnimationState
		@param	speed	Die Geschwindigkeit, 1.0 Originalgeschw.
		@param	timesToPlay	Die Wiederholungen

		@see Animation
	*/
    MeshAnimation* addMeshAnimation(Ogre::AnimationState* animState, MeshObject* mesh,
        Ogre::Real speed=1.0, unsigned int timesToPlay=0, bool paused=false ); 
	/** Gibt die Animation zurück, die zum AnimationState gehört 
     *  @returns NULL wenn es die Animation nicht gibt
     */
	BaseAnimation* getAnimation(Ogre::AnimationState* animState) const;
	/// Entfernt eine Animation, und stoppt das Abspielen dieser
    void removeAnimation(Ogre::AnimationState* animState);

    /// Checks if an Animation is still used in another Animation for Example in a FadeAnimation
    bool isStillInUse( BaseAnimation* anim ) const;

	/** Erzeugt eine neue, leere TrackAnimation, der der SceneNode
		des Actors zugeordnet wird.
		@param	actor	Der zugeordnete Actor
		@param	name	Der einzigartige Name der Animation
		@param	length	Die gewünschte Länge des Tracks
	*/
	TrackAnimation* createTrackAnimation(Actor* actor,
	    const Ogre::String& name, Ogre::Real length );
    /// Entfernt alle Animationen
    void removeAllAnimations();
	/// Entfernt eine Animation
	void removeAnimation(TrackAnimation* anim);
    void removeAnimation(MeshAnimation* anim);
    void removeAnimation(FadeAnimation* anim);
    /// Ersetzt eine alte Animation durch eine Neue
    MeshAnimation* replaceAnimation(MeshAnimation* oldAnim,  
	Ogre::AnimationState* newAnimState, Ogre::Real speed=1.0, unsigned int timesToPlay=0  );
	/// Entfernt eine TrackAnimation dieses Actors
	void removeTrackAnimation( Actor* act, const Ogre::String& name );
	/// Entfernt alle TrackAnimations dieses Actors
	void removeAllTrackAnimations( Actor* act );
	

    /// Blendet von Animation 'from' zu 'to' über
    FadeAnimation* fadeAnimation( MeshAnimation* from, 
        MeshAnimation* to, Ogre::Real time );
    /** Blendet Animationen von nach über, wenn loopDuration nicht
      * größer 0 ist, wird hier die Dauer der Animation genommen 
      *  @todo Rückwärts anschauen, Geschwindigkeit */
    FadeAnimation* fadeAnimation( MeshAnimation* fromLoop, 
        MeshAnimation* blendAnim, MeshAnimation* toLoop, Ogre::Real loopDuration = 0.0 );


	/** Globale Beschleunigung, für SlowMotion oder andere sinnige Effekte
		@param speed Der Beschleunigungsfaktor. 
		@remarks Negative Werte lassen das Spiel nicht rückwärts laufen, nur
			die Animationen.
	*/
	void setGlobalAnimationSpeed( Ogre::Real speed );
	/// Gibt die globale Beschleunigung zurück
	Ogre::Real getGlobalAnimationSpeed( ) const;
	
	/// Setzt den StandardInterpolationsModus für neue Animationen
	void setDefaultInterpolationMode( AnimationManager::InterpolationMode im );
	/// Gibt den StandardInterpolationsModus für neue Animationen zurück
	AnimationManager::InterpolationMode getDefaultInterpolationMode() const;

	/// Setzt den StandardRotationsInterpolationsModus für neue Animationen
	void setDefaultRotationInterpolationMode( AnimationManager::RotationInterpolationMode rim );
	/// Gibt den Standard RotationsInterpolationsModus für neue Animationen zurück
	AnimationManager::RotationInterpolationMode getDefaultRotationInterpolationMode() const;

	/// Geerbt von GameTask, wird in jedem Frame mit der vergangenen Zeit aufgerufen
    virtual void run(Ogre::Real timePassed);

	/// Singleton
    static AnimationManager & getSingleton(void);
	/// Singleton
	static AnimationManager * getSingletonPtr(void);
private:
    static void stopAnimation( BaseAnimation* anim );

    typedef std::set<FadeAnimation*> FadeAnimSet;
    FadeAnimSet mFadeAnimSet;

    typedef std::map<Ogre::AnimationState*,BaseAnimation*> StateAnimMap;
    StateAnimMap mStateAnimationMap;

	/// Die globale Beschleunigung
	Ogre::Real mGlobalAnimationSpeed;
};
}
#endif
