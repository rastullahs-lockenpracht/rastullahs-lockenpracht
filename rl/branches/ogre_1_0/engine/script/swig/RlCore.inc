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
	
	void addResourceLocation(const std::string& name, const std::string& locType, 
			const std::string& groupName = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
			bool recursive = false);
};
}

namespace rl {


class ActorManager {
private:
	ActorManager();
public:
	static ActorManager& getSingleton(void);

    rl::Actor* createLightActor(const std::string& name, rl::LightObject::LightTypes type );
	rl::Actor* createMeshActor(const std::string& name, const std::string& meshname,
		    int geom = rl::PhysicsManager::PT_NONE, Real density=1.0);
	    
	rl::Actor* getActor(const std::string& name);
	void destroyActor(rl::Actor* actor);
    void destroyAllActors();
};

class World {
public:	
	enum FogMode { FOG_NONE, FOG_EXP, FOG_EXP2, FOG_LINEAR  };

	void loadScene(const std::string& levelName ) = 0;

    virtual void setSkyPlane (bool enable, const Plane &plane, const std::string& materialName, Real scale=1000, Real tiling=10, bool drawFirst=true, Real bow=0 );
    virtual void setSkyBox (bool enable, const std::string& materialName, Real distance=5000, bool drawFirst=true );
    virtual void setSkyDome (bool enable, const std::string& materialName, Real curvature=10, Real tiling=8, Real distance=4000, bool drawFirst=true );
    virtual void setFog ( FogMode mode=FOG_NONE, const ColourValue &colour=ColourValue::White, Real expDensity=0.001, Real linearStart=0.0, Real linearEnd=1.0);
    virtual FogMode getFogMode (void) const;
    virtual const ColourValue & getFogColour (void) const;
    virtual Real getFogStart (void) const;
    virtual Real getFogEnd (void) const;	
    virtual void setAmbientLight(Real r, Real g, Real b);
    virtual Vector3 getStartPoint() const;
    virtual void setCastShadows(bool);
};

%apply SWIGTYPE *DYNAMIC { rl::Animation * };

%feature("director") AnimationListener; 
class AnimationListener
{
public:
	virtual ~AnimationListener();
	virtual void animationFinished(rl::AnimationEvent *anEvent) const = 0;
	virtual void animationPaused(rl::AnimationEvent *anEvent) const = 0;
	virtual void animationUnpaused(rl::AnimationEvent *anEvent) const = 0;
};

%feature("director") AnimationFrameListener; 
class AnimationFrameListener
{
public:
	virtual ~AnimationFrameListener();
	virtual void animationFrameReached( rl::AnimationFrameEvent *anEvent ) const = 0;
};

class AnimationEvent
{
public:
	rl::Animation* getAnimation() const;
	AnimationEvent( rl::Animation* anim,  const unsigned int reason );
};

class AnimationFrameEvent
{
public:
	rl::Animation* getAnimation() const;
	Ogre::Real getFrameNumber() const;
	AnimationFrameEvent( rl::Animation* anim,  const unsigned int reason, Ogre::Real frameNumber );
};

class Animation {
public:
	bool isPaused() const;
    void setPaused( bool isPaused );

	bool isIgnoringGlobalSpeed() const;
	void setIgnoringGlobalSpeed( bool isIgnoringGlobalSpeed );

	Real getSpeed() const;
	void setSpeed( Real speed );
	void reverseAnimation();

	void setTimesToPlay(unsigned int timesToPlay);
	unsigned int getTimesPlayed() const;
	void resetTimesPlayed();
	unsigned int getTimesToPlay() const;
	unsigned int getTimesToPlayLeft() const;
	Real getTimePlayed() const;

	Real getWeight(void) const;
    void setWeight(Real weight);
    
    void addAnimationListener( rl::AnimationListener *listener);
	void removeAnimationListener( rl::AnimationListener *listener);
	
	void addAnimationFrameListener( rl::AnimationFrameListener *listener, 
									float frameNumber );
private:
	Animation(  );
};

class TrackAnimation : public Animation {
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
	TrackAnimation(  ); 
};

class AnimationManager {
private:
	AnimationManager();
public:
	static AnimationManager& getSingleton(void);
	
	enum InterpolationMode { IM_LINEAR, IM_SPLINE };
	enum RotationInterpolationMode { RIM_LINEAR, RIM_SPHERICAL };
	
	TrackAnimation* createTrackAnimation(rl::Actor* actor, const std::string& name, Real length );
	void removeAnimation( rl::Animation* anim );
	
	void setGlobalAnimationSpeed( Real speed );
	Real getGlobalAnimationSpeed( ) const;
	
	void setDefaultInterpolationMode( rl::AnimationManager::InterpolationMode im );
	rl::AnimationManager::InterpolationMode getDefaultInterpolationMode() const;

	void setDefaultRotationInterpolationMode( rl::AnimationManager::RotationInterpolationMode rim );
	rl::AnimationManager::RotationInterpolationMode getDefaultRotationInterpolationMode() const;
};

%apply SWIGTYPE *DYNAMIC { ActorControlledObject * };
class ActorControlledObject {
private:
    ActorControlledObject();
public:
    rl::Actor* getActor();
};

class MeshObject : public ActorControlledObject
{
private:
    MeshObject(const std::string& name, const std::string& meshname);
public:
    
    /// Groesse der Boundingbox
    Ogre::Vector3 getSize();
    Ogre::Vector3 getCenter();
    Ogre::Real getRadius();
    Ogre::Real getHeight();
    
	Animation* getAnimation(const std::string& animName) const;
	Animation* startAnimation(const std::string& animName, 
		Ogre::Real speed=1.0, unsigned int timesToPlay=0);
    void stopAnimation(const std::string&);
    
    void setCastShadows (bool enabled);
    bool getCastShadows (void) const;    
};

class LightObject : public ActorControlledObject
{
private:
    LightObject(const std::string& name, rl::LightObject::LightTypes type);
public:
	enum LightTypes { LT_POINT, LT_DIRECTIONAL, LT_SPOTLIGHT };

    void setAttenuation(float range, float constant,
        float linear, float quadric);
    void setDiffuseColour(float red, float green, float blue);
    void setDirection(float x, float y, float z);
    void setSpecularColour(float red, float green, float blue);
    void setSpotlightRange(float innerangle, float outerangle,
        float falloff);
};

class ParticleSystemObject : public ActorControlledObject
{
private:
	 ParticleSystemObject(const std::string& name, const std::string& partSys );
};

class CameraObject : public ActorControlledObject
{
private:
    CameraObject(const std::string& name);
public:
};

class Actor {
private:
	Actor(const std::string& name);
public:	
	const Vector3& getPosition(void);
    void setPosition(Real x, Real y, Real z);
    
    ActorControlledObject* getControlledObject();

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
    
    void initializeModule(const std::string& module);
    void loadMap(const std::string& type, const std::string& filename, const std::string& startupScript = "");
    World* getWorld();
    void makeScreenshot(const std::string& sName);
    void log(const std::string& msg);
};


class GameLoopManager {
private:
	GameLoopManager();
public:
    static GameLoopManager& getSingleton(void);
    
    bool isPaused();
    void setPaused(bool pause);
};

}

%{
static swig_type_info *ActorControlledObject_dynamic_cast(void **ptr)
{
    rl::ActorControlledObject **ppActorControlledObject =
        reinterpret_cast<rl::ActorControlledObject**>(ptr);

    rl::MeshObject *pMeshObject=dynamic_cast<rl::MeshObject*>(*ppActorControlledObject);
    if (pMeshObject) {
        *ptr=reinterpret_cast<void*>(pMeshObject);
        return SWIGTYPE_p_rl__MeshObject;
    }
    rl::CameraObject *pCameraObject=dynamic_cast<rl::CameraObject*>(*ppActorControlledObject);
    if (pCameraObject) {
        *ptr=reinterpret_cast<void*>(pCameraObject);
        return SWIGTYPE_p_rl__CameraObject;
    }
    rl::LightObject *pLightObject=dynamic_cast<rl::LightObject*>(*ppActorControlledObject);
    if (pLightObject) {
        *ptr=reinterpret_cast<void*>(pLightObject);
        return SWIGTYPE_p_rl__LightObject;
    }
    rl::ParticleSystemObject *pParticleSystemObject=dynamic_cast<rl::ParticleSystemObject*>(*ppActorControlledObject);
    if (pParticleSystemObject) {
        *ptr=reinterpret_cast<void*>(pParticleSystemObject);
        return SWIGTYPE_p_rl__ParticleSystemObject;
    }
    return 0;
}
%}
DYNAMIC_CAST(SWIGTYPE_p_rl__ActorControlledObject, ActorControlledObject_dynamic_cast);
