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

namespace rl {
%{
static swig_type_info *GameAreaType_dynamic_cast(void **ptr) {
    rl::GameAreaType **ppGameAreaType = reinterpret_cast<rl::GameAreaType **>(ptr);
    
    rl::GameSphereAreaType *pGameSphereAreaType = dynamic_cast<rl::GameSphereAreaType*>(*ppGameAreaType);
    if(pGameSphereAreaType){
      *ptr=reinterpret_cast<void*>(pGameSphereAreaType);
      return SWIGTYPE_p_rl__GameSphereAreaType;
    }    
    
    return 0;
}
%}
DYNAMIC_CAST(SWIGTYPE_p_rl__GameAreaType, GameAreaType_dynamic_cast);



%{
static swig_type_info *Animation_dynamic_cast(void **ptr) {
    rl::Animation **ppAnimation = reinterpret_cast<rl::Animation **>(ptr);
    
    rl::TrackAnimation *pTrackAnimation = dynamic_cast<rl::TrackAnimation*>(*ppAnimation);
    if(pTrackAnimation){
      *ptr=reinterpret_cast<void*>(pTrackAnimation);
      return SWIGTYPE_p_rl__TrackAnimation;
    }    
    
    return 0;
}
%}
DYNAMIC_CAST(SWIGTYPE_p_rl__Animation, Animation_dynamic_cast);


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
    rl::SoundObject *pSoundObject=dynamic_cast<rl::SoundObject*>(*ppActorControlledObject);
    if (pSoundObject) {
        *ptr=reinterpret_cast<void*>(pSoundObject);
        return SWIGTYPE_p_rl__SoundObject;
    }
    return 0;
}
%}
DYNAMIC_CAST(SWIGTYPE_p_rl__ActorControlledObject, ActorControlledObject_dynamic_cast);


class ActorManager {
private:
	ActorManager();
public:
	static ActorManager& getSingleton(void);


	rl::Actor* createParticleSystemActor(const Ogre::String& name, const Ogre::String& soundfile);
    rl::Actor* createListenerActor(const Ogre::String& name);
    rl::Actor* createSoundStreamActor(const Ogre::String& name, const Ogre::String& soundfile);
    rl::Actor* createSoundSampleActor(const Ogre::String& name, const Ogre::String& soundfile);
    rl::Actor* createEmptyActor(const Ogre::String& name);
    rl::Actor* createLightActor(const Ogre::String& name, rl::LightObject::LightTypes type );
	rl::Actor* createMeshActor(const Ogre::String& name, const Ogre::String& meshname,
		    rl::PhysicsManager::GeometryTypes geom = rl::PhysicsManager::GT_NONE,
		    Ogre::Real density=1.0, rl::PhysicsManager::OffsetMode offsetMode = rl::PhysicsManager::OM_BOTTOMCENTERED);
		    
	//void createDebugActors();
	    
	rl::Actor* getActor(const Ogre::String& name);
	void destroyActor(rl::Actor* actor);
    void destroyAllActors();
};

class World {
public:	
	enum FogMode { FOG_NONE, FOG_EXP, FOG_EXP2, FOG_LINEAR  };

	void loadScene(const Ogre::String& levelName, const Ogre::String& resourceGroup ) = 0;

    virtual void setSkyBox (bool enable, const Ogre::String& materialName, Ogre::Real distance=2500, bool drawFirst=true );
    virtual void setSkyDome (bool enable, const Ogre::String& materialName, Ogre::Real curvature=10, Ogre::Real tiling=8, Ogre::Real distance=4000, bool drawFirst=true );
    virtual void setFog ( FogMode mode=FOG_NONE, const Ogre::ColourValue &colour=Ogre::ColourValue::White, Ogre::Real expDensity=0.001, Ogre::Real linearStart=0.0, Ogre::Real linearEnd=1.0);
    virtual FogMode getFogMode (void) const;
    virtual const Ogre::ColourValue & getFogColour (void) const;
    virtual Ogre::Real getFogStart (void) const;
    virtual Ogre::Real getFogEnd (void) const;	
    virtual void setAmbientLight(Ogre::Real r, Ogre::Real g, Ogre::Real b);
    virtual Ogre::Vector3 getStartPoint() const;
    virtual void setCastShadows(bool);

    void setShowBoundingBoxes( bool dis );
};

/// Alles was mit AreaKrams zu tun hat
%apply SWIGTYPE *DYNAMIC { GameAreaType * };

%feature("director") GameAreaListener; 
class GameAreaListener
{
public:
	virtual ~GameAreaListener();
    virtual void areaLeft(rl::GameAreaEvent *anEvent) const = 0;
    virtual void areaEntered(rl::GameAreaEvent *anEvent) const = 0;
};

class GameAreaType
{
private:
	GameAreaType();
public:
	unsigned long getQueryMask() const;
    void setQueryMask( unsigned long mask );
    void addQueryFlag( unsigned long flag  );
    void removeQueryFlag( unsigned long flag );
    const Ogre::Vector3& getQueryPosition() const;
};

class GameSphereAreaType : public GameAreaType
{
private:
	GameSphereAreaType( Ogre::Vector3, Ogre::Real, unsigned long mask = 0xFFFFFFFF );
public:
};

class GameAreaEventSource
{
private: 
	GameAreaEventSource( rl::GameAreaType* areaType, rl::Actor* act );
public:   
    void addAreaListener( rl::GameAreaListener*  list );
    void removeAreaListener( rl::GameAreaListener* list );
    bool hasListeners( ) const;

    rl::GameAreaType* getGameAreaType() const;
    // const ActorMap& getInsideAreaList() const;
    rl::Actor* getActor() const;
};

class GameAreaEvent
{
private:
	GameAreaEvent( rl::GameAreaEventSource* src, const unsigned int reason );
public:
    rl::Actor* getProvokingActor() const;
    rl::GameAreaEventSource* getSource() const;
};

class GameEventManager
{
private:
    GameEventManager( );
public:
    void addSphereAreaListener( rl::Actor* actor, Ogre::Real radius, 
		rl::GameAreaListener* list, unsigned long queryMask = 0xFFFFFFFF );
    void removeAreaListener( rl::GameAreaListener* list );
    void removeAllAreas( rl::Actor* actor );

    static rl::GameEventManager & getSingleton(void);
};

/// Alles was mit Animationen zu tun hat
%apply SWIGTYPE *DYNAMIC { Animation * };

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
private:
	AnimationEvent( rl::Animation* anim,  const unsigned int reason );
};

class AnimationFrameEvent
{
public:
	rl::Animation* getAnimation() const;
	Ogre::Real getFrameNumber() const;
private:
	AnimationFrameEvent( rl::Animation* anim,  const unsigned int reason, Ogre::Real frameNumber );
};


class Animation {
public:
	bool isPaused() const;
    void setPaused( bool isPaused );

	bool isIgnoringGlobalSpeed() const;
	void setIgnoringGlobalSpeed( bool isIgnoringGlobalSpeed );

	Ogre::Real getSpeed() const;
	void setSpeed( Ogre::Real speed );
	void reverseAnimation();

	void setTimesToPlay(unsigned int timesToPlay);
	unsigned int getTimesPlayed() const;
	void resetTimesPlayed();
	unsigned int getTimesToPlay() const;
	unsigned int getTimesToPlayLeft() const;
	Ogre::Real getTimePlayed() const;

	Ogre::Real getWeight(void) const;
    void setWeight(Ogre::Real weight);
    
    rl::MeshObject* getMeshObject( );
    
    void addAnimationListener( rl::AnimationListener *listener);
	void removeAnimationListener( rl::AnimationListener *listener);
	
	void addAnimationFrameListener( rl::AnimationFrameListener *listener, 
									float frameNumber );
	void removeAnimationFrameListener( rl::AnimationFrameListener *listener );
	void removeAnimationFrameListener( rl::AnimationFrameListener *listener, Ogre::Real frameNumber );
private:
	Animation(  );
};

class TrackAnimation : public Animation {
public:
	void addKeyFrame( Ogre::Real timePos );
    void setKeyFrameTranslation( Ogre::Real timePos, Ogre::Real xPos, Ogre::Real yPos, Ogre::Real zPos );
    void setKeyFrameRotation( Ogre::Real timePos, Ogre::Real xRotAxis, Ogre::Real yRotAxis, Ogre::Real zRotAxis, Ogre::Real angleUnits );
    void setKeyFrameRotationQuaternion( Ogre::Real timePos, Ogre::Real w, Ogre::Real x, Ogre::Real y, Ogre::Real z );
    void setKeyFrameScale( Ogre::Real timePos, Ogre::Real xScale, Ogre::Real yScale, Ogre::Real zScale );

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
	
	rl::TrackAnimation* createTrackAnimation(rl::Actor* actor, const Ogre::String& name, Ogre::Real length );
	void removeAnimation( rl::Animation* anim );
	void removeTrackAnimation( rl::Actor* act, const Ogre::String& name );
	void removeAllTrackAnimations( rl::Actor* act );
	
	
	void setGlobalAnimationSpeed( Ogre::Real speed );
	Ogre::Real getGlobalAnimationSpeed( ) const;
	
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

class PhysicalObject : public ActorControlledObject
{
};

class MeshObject : public PhysicalObject
{
private:
    MeshObject(const Ogre::String& name, const Ogre::String& meshname);
public:
    
    /// Groesse der Boundingbox
    Ogre::Vector3 getSize();
    Ogre::Vector3 getCenter();
    Ogre::Real getRadius();
    Ogre::Real getHeight();
    
	rl::Animation* getAnimation(const Ogre::String& animName) const;
	rl::Animation* replaceAnimation(const Ogre::String& oldAnimName, 
	    const Ogre::String& newAnimName, float speed=1.0, unsigned int timesToPlay=0);
	rl::Animation* startAnimation(const Ogre::String& animName, 
		float speed=1.0, unsigned int timesToPlay=0);
    void stopAnimation(const Ogre::String&);
    void stopAllAnimations();
    
    void setCastShadows (bool enabled);
    bool getCastShadows (void) const;    
};

class LightObject : public ActorControlledObject
{
private:
    LightObject(const Ogre::String& name, rl::LightObject::LightTypes type);
public:
	enum LightTypes { LT_POINT, LT_DIRECTIONAL, LT_SPOTLIGHT };

    void setAttenuation(float range, float constant,
        float linear, float quadric);
    void setDiffuseColour(float red, float green, float blue);
    void setDirection(float x, float y, float z);
    void setSpecularColour(float red, float green, float blue);
    void setSpotlightRange(float innerangle, float outerangle,
        float falloff);
    void setCastShadows( bool cast );
};

class SoundObject : public ActorControlledObject
{
public:
	 void play( );
	 bool isLooping() const;
	 void setLooping( bool looping );
     void pause(bool pausing);
     bool isPaused();
     void stop();
};

class ParticleSystemObject : public ActorControlledObject
{
public:
     void setActive(bool active);
private:
	 ParticleSystemObject(const Ogre::String& name, const Ogre::String& partSys );
};

class CameraObject : public PhysicalObject
{
private:
    CameraObject(const Ogre::String& name);
public:
};

class Actor {
private:
	Actor(const Ogre::String& name);
public:	
	static const Ogre::String DEFAULT_SLOT_NAME;

    enum QueryGroupFlags
    {
        QGF_NONE = 0,
        QGF_DEFAULT = 1,
        QGF_PLAYER = 2,
        QGF_ALL = 0xFFFFFFFF
    };

	const const Ogre::String& getName() const;
	const Ogre::Vector3& getPosition(void);
	const Ogre::Quaternion& getOrientation(void);
	const Ogre::Vector3& getWorldPosition(void);
	const Ogre::Quaternion& getWorldOrientation(void);
    void setPosition(Ogre::Real x, Ogre::Real y, Ogre::Real z);
    
    rl::ActorControlledObject* getControlledObject();
    Ogre::UserDefinedObject* getGameObject() const;
    rl::PhysicalThing* getPhysicalThing() const;

    void setScale( Ogre::Real sx, Ogre::Real sy, Ogre::Real sz );
    void placeIntoScene(
            const Ogre::Vector3& position = Ogre::Vector3::ZERO,
            const Ogre::Quaternion& orientation = Ogre::Quaternion::IDENTITY,
            const Ogre::String& physicsBone = "");  
    void placeIntoScene(
        Ogre::Real px, Ogre::Real py, Ogre::Real pz,
        Ogre::Real ow, Ogre::Real ox, Ogre::Real oy, Ogre::Real oz, 
        const Ogre::String& physicsBone = "");
    void removeFromScene();
           
    void attach( 
            Actor* actor,
            const Ogre::String& childSlot = "SLOT_DEFAULT",
            const Ogre::Vector3& offsetPosition=Ogre::Vector3::ZERO,
            const Ogre::Quaternion& offsetOrientation=Ogre::Quaternion::IDENTITY
            );
   	void attachAxisRot( 
            rl::Actor* actor,
            const Ogre::String& childSlot = "SLOT_DEFAULT",            
            const Ogre::Vector3& offsetPosition=Ogre::Vector3::ZERO,
            const Ogre::Vector3& offsetAxis=Ogre::Vector3::UNIT_X,
            const Ogre::Radian& offsetRotation=Ogre::Radian(0) );
    void attachToSlot( 
            Actor* actor,
            const Ogre::String& slot,
            const Ogre::String& childSlot = "SLOT_DEFAULT",
            const Ogre::Vector3& offsetPosition=Ogre::Vector3::ZERO,
			const Ogre::Quaternion& offsetOrientation=Ogre::Quaternion::IDENTITY
			);
    void attachToSlotAxisRot( 
            rl::Actor* actor,
            const Ogre::String& slot,
            const Ogre::String& childSlot = "SLOT_DEFAULT",            
			const Ogre::Vector3& offsetPosition=Ogre::Vector3::ZERO,
            const Ogre::Vector3& offsetAxis=Ogre::Vector3::UNIT_X,
            const Ogre::Radian& offsetRotation=Ogre::Radian(0) );
                    
    void detach(rl::Actor* actor);
    
    rl::Actor* getChildByName(const Ogre::String& name ) const;
       
	void roll(Ogre::Real angleunits);
	void pitch(Ogre::Real angleunits);
	void yaw(Ogre::Real angleunits);
	
	unsigned long getQueryMask() const;
    void setQueryMask( unsigned long mask = 0xFFFFFFFF );
    void addQueryFlag( unsigned long flag  );
    void removeQueryFlag( unsigned long flag );
};

%feature("director") PhysicsContactListener; 
class PhysicsContactListener
{
public:
    PhysicsContactListener();
    virtual ~PhysicsContactListener();
    
    virtual void contactOccured(rl::Actor* actor1, rl::Actor* actor2) = 0;
};

class PhysicalThing
{
public:
    PhysicalThing(OgreNewt::Body* body, const Ogre::Vector3& offset = Ogre::Vector3::ZERO,
        const Ogre::Quaternion& orientationBias = Ogre::Quaternion::IDENTITY);

    void setPosition(Ogre::Real x, Ogre::Real y, Ogre::Real z);
    void setOrientation(Ogre::Real w, Ogre::Real x, Ogre::Real y, Ogre::Real z);

    // Sets the vector, that will always point up.
    void setUpConstraint(const Ogre::Vector3& upVector = Ogre::Vector3::UNIT_Y);
    Ogre::Vector3 getUpConstraint() const;
    void clearUpConstraint();

    rl::Actor* getActor() const;

    void addForce(const Ogre::Vector3& force);

    void freeze();
    void unfreeze();
        
    void setGravityOverride(bool override,
        Ogre::Real x = 0.0f, Ogre::Real y = 0.0f, Ogre::Real z = 0.0f);
        
    Ogre::Real getMass() const;
    
    void updateCollisionHull();

    void setContactListener(rl::PhysicsContactListener* listener);
    rl::PhysicsContactListener* getContactListener() const;
};


class PhysicsManager
{
public:

    enum GeometryTypes {
        GT_NONE = -1,
        GT_BOX = 0,
        GT_SPHERE = 1,
        GT_CAPSULE = 2,
        GT_MESH = 3,
        GT_ELLIPSOID = 4,
        GT_CONVEXHULL = 5
    };
    
    /// Typ bestimmt, wo der Usrprung (0/0/0) des Objektes liegt.
    enum OffsetMode {
        /// Ursprung in Objektmitte
        OM_CENTERED = 0,
        /// Ursprung Unten-Mitte
        OM_BOTTOMCENTERED
    };
    
    PhysicsManager();
	static PhysicsManager& getSingleton(void);
    
    rl::PhysicalThing* createPhysicalThing(rl::PhysicsManager::GeometryTypes geomType, rl::PhysicalObject* po,
		Ogre::Real mass, rl::PhysicsManager::OffsetMode offsetMode = rl::PhysicsManager::OM_BOTTOMCENTERED );

    void removeAndDestroyPhysicalThing(PhysicalThing* thing);

    // Global Settings
    void setGravity(Ogre::Real x, Ogre::Real y, Ogre::Real z);
    Ogre::Vector3 getGravity() const;

    bool isEnabled() const;
    void setEnabled(bool enabled);
	
	void toggleDebugMode();
    bool isDebugMode() const;
};


class CoreSubsystem {
private:
	CoreSubsystem();
public:
    static CoreSubsystem& getSingleton(void);
    
    void loadMap(const Ogre::String& type, const Ogre::String& filename,
		const Ogre::String& resourceGroup, const Ogre::String& startupScript = "");
    World* getWorld();
    void makeScreenshot(const Ogre::String& sName);
    RL_LONGLONG getClock();
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



