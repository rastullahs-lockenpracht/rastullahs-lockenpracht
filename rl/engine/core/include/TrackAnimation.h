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

#ifndef __TrackAnimation_H__
#define __TrackAnimation_H__

#include "CorePrerequisites.h"

#include "BaseAnimation.h"
#include "AnimationManager.h"

namespace rl {

class _RlCoreExport Actor;
/** Diese Klasse stellt eine Moeglichkeit zur Verfuegung, einfach Tracks zu erzeugen denen
	Aktoren folgen.
*/
class _RlCoreExport TrackAnimation : public BaseAnimation
{
public:
	/** Konstruktor, der einen neuen Track erstellt
		@param name		Der (einzigartige) Name der Animation
		@param node		Der Knoten der gesteuert werden soll
		@param length	Die gesamte Laenge der Animation
		@remarks Konstruktor sollte nicht direkt aufgerufen werden, 
		sondern vom AnimationManager.
	 */
	TrackAnimation(const Ogre::String& name, Actor *act,
	    Ogre::Real length);
	/// Virtueller Destruktor
	virtual ~TrackAnimation();

	/// ueberschriebe von BaseAnimation
	virtual void doAddTime( Ogre::Real timePassed );
	
	/** Setzt ein neues Keyframe an einer Zeitposition
		@param timePos	Die ZeitPosition
		@remarks Bei 0 existiert schon zu Beginn ein Keyframe
	*/
    void addKeyFrame( Ogre::Real timePos );
	/** Setzt die Verschiebung bei einem KeyFrame
		@param	timePos	Die ZeitPosition des KeyFrames
		@param	xPos	Die x-Position
		@param	yPos	Die y-Position
		@param	zPos	Die z-Position
		@remarks Sollte das KeyFrame nichts existieren, wird das 
			davorliegende KeyFrame ueberschrieben
	*/
    void setKeyFrameTranslation( Ogre::Real timePos,
        Ogre::Real xPos, Ogre::Real yPos, Ogre::Real zPos );
	/** Setzt die Verschiebung bei einem KeyFrame
		@param	timePos	Die ZeitPosition des KeyFrames
		@param	xRotAxis Die Achsendefinition
		@param	yRotAxis Die Achsendefinition
		@param	yRotAxis Die Achsendefinition
		@param	angleUnits Die Drehung ( in Grad )
		@remarks Sollte das KeyFrame nichts existieren, wird der
			davorliegende KeyFrame ueberschrieben
	*/
    void setKeyFrameRotation( Ogre::Real timePos,
        Ogre::Real xRotAxis, Ogre::Real yRotAxis, Ogre::Real zRotAxis,
        Ogre::Real angleUnits );
    void setKeyFrameRotationQuaternion( Ogre::Real timePos,
        Ogre::Real w, Ogre::Real x, Ogre::Real y, Ogre::Real z );

	/** Setzt die Skalierung bei einem KeyFrame
		@param	timePos	Die ZeitPosition des KeyFrames
		@param	xScale	Die x-Skalierung
		@param	yScale	Die y-Skalierung
		@param	zScale	Die z-Skalierung
		@remarks Sollte das KeyFrame nichts existieren, wird das 
			davorliegende KeyFrame ueberschrieben
	*/
    void setKeyFrameScale( Ogre::Real timePos,
        Ogre::Real xScale, Ogre::Real yScale, Ogre::Real zScale);

	/// Setzt die Art der Interpolation (InterpolationMode aus AnimationManager
	void setInterpolationMode( AnimationManager::InterpolationMode im );
	/// Gibt die Art der Interpolation zurueck
	AnimationManager::InterpolationMode getInterpolationMode() const;

	/// Setzt die Art der RotationsInterpolation (RotationInterpolationMode aus AnimationManager)
	void setRotationInterpolationMode(
	    AnimationManager::RotationInterpolationMode im );
	/// Gibt die Art der RotationsInterpolation zurueck 
	AnimationManager::RotationInterpolationMode getRotationInterpolationMode() const;

	/// Setzt ob bei Rotation der kuerzeste Weg benutzt werden soll
	void setUseShortestRotationPath ( bool useShortestPath );
	/// Gibt zurueck ob bei Rotation der kuerzeste Weg benutzt werden soll
	bool getUseShortestRotationPath () const;

	/// Teste ob der Track zu einem bestimmten Actor gehoert
	Actor* getActor( ) const ;
    Ogre::AnimationState* getAnimationState() const;

	const Ogre::String& getName() const;
private:
	/// Interne Methode ein KeyFrame zu einer ZeitPosition zu erlangen
	Ogre::TransformKeyFrame* getKeyFrameAtTimePos(Ogre::Real timePos);	
    
    /// Setzt den AnimationState
	void setAnimationState( Ogre::AnimationState* animState );

    /// Der AnimationState
    Ogre::AnimationState* mAnimState;
	/// Die erstellte Animation
	Ogre::Animation* mAnimation;
	/// Der erstellte Track
	Ogre::NodeAnimationTrack* mAnimationTrack;
	/// Der bewegte Actor
	Actor* mActor;
};

}
#endif
