/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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

namespace Ogre {
class Root {
private:
	Root();
public:
	static Ogre::Root& getSingleton(void);
	
	void addResourceLocation (const String &name, const String &locType, ResourceType resType=RESTYPE_ALL);
};
}

namespace rl {


class ActorManager {
private:
	ActorManager();
public:
	static ActorManager& getSingleton(void);

	rl::Actor* createMeshActor(const String& name, const String& meshname,
		    int geom = rl::PhysicsManager::PT_NONE, Real density=1.0);
	    
	rl::Actor* getActor(const String& name);
	void destroyActor(rl::Actor* actor);
    void destroyAllActors();
};

class World {
public:	
	enum FogMode { FOG_NONE, FOG_EXP, FOG_EXP2, FOG_LINEAR  };

	void loadScene(const String& levelName ) = 0;

    virtual void setSkyPlane (bool enable, const Plane &plane, const String &materialName, Real scale=1000, Real tiling=10, bool drawFirst=true, Real bow=0 );
    virtual void setSkyBox (bool enable, const String &materialName, Real distance=5000, bool drawFirst=true );
    virtual void setSkyDome (bool enable, const String &materialName, Real curvature=10, Real tiling=8, Real distance=4000, bool drawFirst=true );
    virtual void setFog ( FogMode mode=FOG_NONE, const ColourValue &colour=ColourValue::White, Real expDensity=0.001, Real linearStart=0.0, Real linearEnd=1.0);
    virtual FogMode getFogMode (void) const;
    virtual const ColourValue & getFogColour (void) const;
    virtual Real getFogStart (void) const;
    virtual Real getFogEnd (void) const;	
    virtual void setAmbientLight(Real r, Real g, Real b);
    virtual Vector3 getStartPoint() const;
    virtual void setCastShadows(bool);
};

%apply SWIGTYPE *DYNAMIC { rl::RlAnimation * };

class RlAnimation {
public:
	bool isPaused() const;
    void setPaused( bool isPaused );

	// Möglichkeit die globale SlowMotion zu umgehen
	// Nützlich für Statusanzeigen, oder ähnliche konstante Animationen
	bool isIgnoringGlobalSpeed() const;
	void setIgnoringGlobalSpeed( bool isIgnoringGlobalSpeed );

	// Regelbare Geschwindigkeit
	Real getSpeed() const;
	void setSpeed( Real speed );
	void reverseAnimation();

	// Regelbare Wiederholungsanzahl
	void setTimesToPlay(unsigned int timesToPlay);
	unsigned int getTimesPlayed() const;
	void resetTimesPlayed();
	unsigned int getTimesToPlay() const;
	unsigned int getTimesToPlayLeft() const;
	Real getTimePlayed() const;

	// Gewicht (Einfluss) der RlAnimation
	Real getWeight(void) const;
    void setWeight(Real weight);
    
    // Listener
    void addAnimationListener( rl::AnimationListener *listener);
	void removeAnimationListener( rl::AnimationListener *listener);
private:
	RlAnimation(  );
};

class RlTrackAnimation : public RlAnimation {
public:
	void addKeyFrame( Real timePos );
    void setKeyFrameTranslation( Real timePos, Real xPos, Real yPos, Real zPos );
    void setKeyFrameRotation( Real timePos, Real xRotAxis, Real yRotAxis, Real zRotAxis, Real angleUnits );
    void setKeyFrameScale( Real timePos, Real xScale, Real yScale, Real zScale );

	void setInterpolationMode( rl::AnimationManager::InterpolationMode im );
	rl::AnimationManager::InterpolationMode getInterpolationMode() const;

	void setRotationInterpolationMode( rl::AnimationManager::RotationInterpolationMode im );
	rl::AnimationManager::RotationInterpolationMode getRotationInterpolationMode() const;

	void setUseShortestRotationPath ( bool useShortestPath );
	bool getUseShortestRotationPath () const;
private:
	RlTrackAnimation(  ); 
};

class AnimationEvent
{
public:
	rl::RlAnimation* getRlAnimation() const;
	AnimationEvent( rl::RlAnimation* anim,  const unsigned int reason );
};

%feature("director") AnimationListener; 
class AnimationListener
{
public:
	virtual ~AnimationListener();
	virtual void animationFinished(rl::AnimationEvent *anEvent) const = 0;
	virtual void animationPaused(rl::AnimationEvent *anEvent) const = 0;
	virtual void animationUnpaused(rl::AnimationEvent *anEvent) const = 0;
};

class AnimationManager {
private:
	AnimationManager();
public:
	static AnimationManager& getSingleton(void);
	
	enum InterpolationMode { IM_LINEAR, IM_SPLINE };
	enum RotationInterpolationMode { RIM_LINEAR, RIM_SPHERICAL };
	
	RlTrackAnimation* createTrackAnimation(rl::Actor* actor, const String& name, Real length );
	void removeAnimation(RlAnimation* anim);
	
	void setGlobalAnimationSpeed( Real speed );
	Real getGlobalAnimationSpeed( ) const;
	
	void setDefaultInterpolationMode( rl::AnimationManager::InterpolationMode im );
	rl::AnimationManager::InterpolationMode getDefaultInterpolationMode() const;

	void setDefaultRotationInterpolationMode( rl::AnimationManager::RotationInterpolationMode rim );
	rl::AnimationManager::RotationInterpolationMode getDefaultRotationInterpolationMode() const;
};

class Actor {
private:
	Actor(const String& name);
public:	
	const Vector3& getPosition(void);
    void setPosition(Real x, Real y, Real z);
    
    void placeIntoScene(Real px, Real py, Real pz,
        Real ow, Real ox, Real oy, Real oz);
       
	void roll(Real angleunits);
	void pitch(Real angleunits);
	void yaw(Real angleunits);
};


class PhysicalThing {
private:
	PhysicalThing();
public:
    void setPosition(Real x, Real y, Real z);
    void setOrientation(Real w, Real x, Real y, Real z);

    rl::Actor* getActor();

    bool isDynamic();
    void setDynamic(bool dynamic);

    void addForce(Real dir_x, Real dir_y, Real dir_z);
    void addForceWorldSpace(Real dir_x, Real dir_y, Real dir_z);
    void addForce(Real dir_x, Real dir_y, Real dir_z,
        Real pos_x, Real pos_y, Real pos_z);
    void addForceWorldSpace(Real dir_x, Real dir_y, Real dir_z,
        Real pos_x, Real pos_y, Real pos_z);
    void addTorque(Real x, Real y, Real z);
    void addTorqueWorldSpace(Real x, Real y, Real z);

    void setBounceParameters(Real restitutionValue,
        Real velocityThreshold);
    Real getBounceRestitutionValue(void);
    Real getBounceVelocityThreshold(void);

    void setSoftness(Real softness);
    Real getSoftness(void);

    void setFriction(Real friction);
    Real getFriction(void);
    void setSoftErp(Real erp);
    Real getSoftErp(void);

    void setLinearVelocity(Real x, Real y, Real z);
    const Vector3 getLinearVelocity(void);

    const Vector3 getAngularVelocity(void);
    void setAngularVelocity(Real x, Real y, Real z);
};

class PhysicsManager {
private:
	PhysicsManager();
public:  
	static const int PT_NONE = -1;
    static const int PT_BOX = 0;
    static const int PT_SPHERE = 1;
    static const int PT_CAPSULE = 2;
    static const int PT_MESH = 3;

    void removeAndDestroyPhysicalThing(rl::PhysicalThing* thing);

    void activateGlobalSpace();
    void activatePhysicalThingSpace( rl::PhysicalThing* thing );
    void removePhysicalThingSpace( rl::PhysicalThing* thing );
    void moveToCurrentSpace( rl::PhysicalThing* thing );
    void createSimpleSpace();
    
    void setGravity( Real x, Real y, Real z );
    Vector3 getGravity();
    
    void setEnabled(bool enabled);
    
    static PhysicsManager & getSingleton(void);
	static PhysicsManager * getSingletonPtr(void);
};

class CoreSubsystem {
private:
	CoreSubsystem();
public:
    static CoreSubsystem& getSingleton(void);
    
    void initializeModule(const String& module);
    void loadMap(const String type, const String filename, const String startupScript = "");
    World* getWorld();
    void makeScreenshot(const String& sName);
    void log(const String& msg);
};


class GameLoop {
private:
	GameLoop();
public:
    static GameLoop& getSingleton(void);
    
    bool isPaused();
    void setPaused(bool pause);
};

}

