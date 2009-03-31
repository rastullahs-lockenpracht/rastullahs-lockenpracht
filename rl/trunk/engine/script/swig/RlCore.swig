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


namespace rl {
/*
%{
static swig_type_info *GameAreaType_dynamic_cast(void **ptr) {
    rl::GameAreaType *pGameAreaType = static_cast<rl::GameAreaType *>(*ptr);
    
    rl::GameSphereAreaType *pGameSphereAreaType = dynamic_cast<rl::GameSphereAreaType*>(pGameAreaType);
    if (pGameSphereAreaType){
      *ptr=pGameSphereAreaType;
      return SWIGTYPE_p_rl__GameSphereAreaType;
    }    
    
    return 0;
}
%}
DYNAMIC_CAST(SWIGTYPE_p_rl__GameAreaType, GameAreaType_dynamic_cast);
*/


%{
static swig_type_info *BaseAnimation_dynamic_cast(void **ptr) {
    rl::BaseAnimation *pBaseAnimation = static_cast<rl::BaseAnimation *>(*ptr);
    
    rl::FadeAnimation *pFadeAnimation = dynamic_cast<rl::FadeAnimation*>(pBaseAnimation);
    if (pFadeAnimation){
      *ptr=pFadeAnimation;
      return SWIGTYPE_p_rl__FadeAnimation;
    }    
    rl::MeshAnimation *pMeshAnimation = dynamic_cast<rl::MeshAnimation*>(pBaseAnimation);
    if (pMeshAnimation){
      *ptr=pMeshAnimation;
      return SWIGTYPE_p_rl__MeshAnimation;
    }    
    rl::TrackAnimation *pTrackAnimation = dynamic_cast<rl::TrackAnimation*>(pBaseAnimation);
    if (pTrackAnimation){
      *ptr=pTrackAnimation;
      return SWIGTYPE_p_rl__TrackAnimation;
    }  
    
    return 0;
}
%}
DYNAMIC_CAST(SWIGTYPE_p_rl__BaseAnimation, BaseAnimation_dynamic_cast);


%{
static swig_type_info *ActorControlledObject_dynamic_cast(void **ptr)
{
    rl::ActorControlledObject *pActorControlledObject =
        static_cast<rl::ActorControlledObject*>(*ptr);

    rl::MergeableMeshObject *pMergeableMeshObject=dynamic_cast<rl::MergeableMeshObject*>(pActorControlledObject);
    if (pMergeableMeshObject) {
        *ptr=pMergeableMeshObject;
        return SWIGTYPE_p_rl__MergeableMeshObject;
    }
    rl::MeshObject *pMeshObject=dynamic_cast<rl::MeshObject*>(pActorControlledObject);
    if (pMeshObject) {
        *ptr=pMeshObject;
        return SWIGTYPE_p_rl__MeshObject;
    }
    rl::CameraObject *pCameraObject=dynamic_cast<rl::CameraObject*>(pActorControlledObject);
    if (pCameraObject) {
        *ptr=pCameraObject;
        return SWIGTYPE_p_rl__CameraObject;
    }
    rl::LightObject *pLightObject=dynamic_cast<rl::LightObject*>(pActorControlledObject);
    if (pLightObject) {
        *ptr=pLightObject;
        return SWIGTYPE_p_rl__LightObject;
    }
    rl::ParticleSystemObject *pParticleSystemObject=dynamic_cast<rl::ParticleSystemObject*>(pActorControlledObject);
    if (pParticleSystemObject) {
        *ptr=pParticleSystemObject;
        return SWIGTYPE_p_rl__ParticleSystemObject;
    }
    rl::SoundObject *pSoundObject=dynamic_cast<rl::SoundObject*>(pActorControlledObject);
    if (pSoundObject) {
        *ptr=pSoundObject;
        return SWIGTYPE_p_rl__SoundObject;
    }
    return 0;
}
%}
DYNAMIC_CAST(SWIGTYPE_p_rl__ActorControlledObject, ActorControlledObject_dynamic_cast);

%{
static swig_type_info *Job_dynamic_cast(void **ptr) {
    rl::Job *ppJob = static_cast<rl::Job*>(*ptr);
      
    
    rl::PlayAnimationJob *pPlayAnimationJob = dynamic_cast<rl::PlayAnimationJob*>(ppJob);
    if (pPlayAnimationJob){
      *ptr=pPlayAnimationJob;
      return SWIGTYPE_p_rl__PlayAnimationJob;
    }    
    rl::StartAnimationJob *pStartAnimationJob = dynamic_cast<rl::StartAnimationJob*>(ppJob);
    if (pStartAnimationJob){
      *ptr=pPlayAnimationJob;
      return SWIGTYPE_p_rl__StartAnimationJob;
    }    
    rl::PlaySound3dJob *pPlaySound3dJob = dynamic_cast<rl::PlaySound3dJob*>(ppJob);
    if (pPlaySound3dJob){
      *ptr=pPlaySound3dJob;
      return SWIGTYPE_p_rl__PlaySound3dJob;
    }    
    rl::PlaySound2dJob *pPlaySound2dJob = dynamic_cast<rl::PlaySound2dJob*>(ppJob);
    if (pPlaySound2dJob){
      *ptr=pPlaySound2dJob;
      return SWIGTYPE_p_rl__PlaySound2dJob;
    }
    rl::JobQueue *pJobQueue = dynamic_cast<rl::JobQueue*>(ppJob);
    if (pJobQueue) {
      *ptr=pJobQueue;
      return SWIGTYPE_p_rl__JobQueue;
    }
    rl::JobSet *pJobSet = dynamic_cast<rl::JobSet*>(ppJob);
    if (pJobSet) {
      *ptr=pJobSet;
      return SWIGTYPE_p_rl__JobSet;
    }
    
    return 0;
}
%}
DYNAMIC_CAST(SWIGTYPE_p_rl__Job, Job_dynamic_cast)

class ActorManager {
private:
	ActorManager();
public:
	static ActorManager& getSingleton(void);

	rl::Actor* createParticleSystemActor(const Ogre::String& name, const Ogre::String& soundfile);
    rl::Actor* createSoundStreamActor(const Ogre::String& name, const Ogre::String& soundfile);
    rl::Actor* createSoundSampleActor(const Ogre::String& name, const Ogre::String& soundfile);
    rl::Actor* createEmptyActor(const Ogre::String& name);
    rl::Actor* createLightActor(const Ogre::String& name, rl::LightObject::LightTypes type );
	rl::Actor* createMeshActor(const Ogre::String& name, const Ogre::String& meshname,
		    rl::GeometryType geom = rl::GT_NONE,
		    Ogre::Real mass = 0.0,
		    bool mergeable = false);
	rl::Actor* createBoxPrimitiveActor(const Ogre::String& name,
            const Ogre::Vector3& minCorner, const Ogre::Vector3& maxCorner,
            const Ogre::String& materialName = "", Ogre::Real mass = 0.0);
		    
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
    virtual void areaLeft(rl::GameAreaEvent *anEvent) = 0;
    virtual void areaEntered(rl::GameAreaEvent *anEvent) = 0;
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

/*
class GameSphereAreaType : public GameAreaType
{
private:
	GameSphereAreaType( Ogre::Vector3, Ogre::Real, unsigned long mask = 0xFFFFFFFF );
public:
};
*/
class GameAreaEventSource
{
private: 
	GameAreaEventSource( rl::GameAreaType* areaType, rl::Actor* act );
public:   
    void addAreaListener( rl::GameAreaListener*  list );
    void removeAreaListener( rl::GameAreaListener* list );
    bool hasListeners( ) const;

    rl::GameAreaType* getGameAreaType();
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
	void addMeshAreaListener( rl::Actor* actor, Ogre::Entity* ent, rl::GeometryType geom, 
	    rl::GameAreaListener* list, unsigned long queryMask = 0xFFFFFFFF );
	void addMeshAreaListener( rl::Actor* actor, rl::GeometryType geom, 
	    rl::GameAreaListener* list, unsigned long queryMask = 0xFFFFFFFF );
	
    void removeAreaListener( rl::GameAreaListener* list );
    void removeAllAreas( rl::Actor* actor );

    static rl::GameEventManager & getSingleton(void);
};

/// Alles was mit Animationen zu tun hat
%apply SWIGTYPE *DYNAMIC { BaseAnimation * };

class BaseAnimation {
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

	Ogre::Real getDelay(void) const;
    void setDelay(Ogre::Real delay);
    
private:
	BaseAnimation(  );
};

class FadeAnimation : public BaseAnimation {
private:
	FadeAnimation(  ); 
public:
	bool isDeleteOnFinish() const;
	void setDeleteOnFinish( bool deleteOnFinish );
};

class MeshAnimation : public BaseAnimation {
public:
    rl::MeshObject* getMeshObject( );
    
	Ogre::Real getWeight(void) const;
    void setWeight(Ogre::Real weight);
private:
	MeshAnimation(  ); 
};

class TrackAnimation : public BaseAnimation {
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
	void removeAnimation( rl::MeshAnimation* anim );
    void removeAnimation( rl::TrackAnimation* anim );
    void removeAnimation( rl::FadeAnimation* anim );
	void removeTrackAnimation( rl::Actor* act, const Ogre::String& name );
	void removeAllTrackAnimations( rl::Actor* act );
	
	void setGlobalAnimationSpeed( Ogre::Real speed );
	Ogre::Real getGlobalAnimationSpeed( ) const;

    rl::FadeAnimation* fadeAnimation( rl::MeshAnimation* from, rl::MeshAnimation* to, Ogre::Real time );
    rl::FadeAnimation* fadeAnimation( rl::MeshAnimation* fromLoop, rl::MeshAnimation* blendAnim, 
        rl::MeshAnimation* toLoop, Ogre::Real loopDuration = 0.0);
	
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
    virtual void setActive(bool active);
};

class PhysicalObject : public rl::ActorControlledObject
{
};

class MeshObject : public rl::PhysicalObject
{
private:
    MeshObject(const Ogre::String& name, const Ogre::String& meshname);
public:
    
    /// Groesse der Boundingbox
    Ogre::AxisAlignedBox getDefaultSize() const;
        
	rl::BaseAnimation* getAnimation(const Ogre::String& animName);
	rl::BaseAnimation* replaceAnimation(const Ogre::String& oldAnimName, 
	    const Ogre::String& newAnimName, float speed=1.0, unsigned int timesToPlay=0);
	rl::BaseAnimation* startAnimation(const Ogre::String& animName, 
		float speed=1.0, unsigned int timesToPlay=0);
    void stopAnimation(const Ogre::String&);
    void stopAllAnimations();
    
    void setCastShadows (bool enabled);
    bool getCastShadows (void) const;    
};

class MergeableMeshObject : public rl::MeshObject
{
private:
    MergeableMeshObject(const Ogre::String& name, const Ogre::String& meshname);
public:
	void addSubmesh(const Ogre::String& meshname, const Ogre::String& meshfile);
	void removeSubmesh(const Ogre::String& submeshToRemove);
	void replaceSubmesh(const Ogre::String& submeshToReplace, const Ogre::String& substituteMeshname);
	void setBaseMeshPart(const Ogre::String& partname);
};

class LightObject : public rl::ActorControlledObject
{
private:
    LightObject(const Ogre::String& name, rl::LightObject::LightTypes type);
public:
	enum LightTypes { LT_POINT, LT_DIRECTIONAL, LT_SPOTLIGHT };

    void setAttenuation(float range, float constant,
        float linear, float quadric);
    void setDiffuseColour(float red, float green, float blue);
    void setDiffuseColour(const Ogre::ColourValue& colour);
    void setDirection(float x, float y, float z);
    void setSpecularColour(float red, float green, float blue);
    void setSpecularColour(const Ogre::ColourValue& colour);
    void setSpotlightRange(float innerangle, float outerangle,
        float falloff);
    void setCastShadows( bool cast );
};


class SoundObject : public rl::ActorControlledObject
{
public:
    SoundObject(rl::Sound *sound, const Ogre::String &name);
    virtual ~SoundObject();
    
  	void play(bool destroyWhenDone=false);

	void load();
	void unload();
	bool is3d() const;
	void set3d( bool is3d );

    /// Gibt zurck ob der Sound geloopt werden soll 
    bool isLooping() const;
    /// Setzt ob der Sound geloopt werden soll 
    void setLooping( bool looping );
    /// Pausiert den Sound.
    void pause(bool pausing);
    /// Ist der Sound pausiert?
    bool isPaused();
    /// Stoppt den Sound.
    void stop();
	/// Setzt die Lautstaerke (1.0 = volle Lautstrke)
	void setVolume(float volume = 1.0);
    // Sets the priority of this sound
    void setPriority(const int priority);
    // Gets the priority of this sound
    const int getPriority() const;
	
	rl::Sound* getSound();
};

class SoundStitchingObject : public rl::ActorControlledObject
{
public:
    /// Konstruktor
    SoundStitchingObject(rl::SoundStitching *stitching, const Ogre::String &name);
    /// Destruktor
    virtual ~SoundStitchingObject();
      
    /**
     * Interne Methode. Wird vom Aktor aufgerufen, wenn sich dessen
     * Status gendert hat. (Position, Orientierung, etc)
     * Die Standardimplementierung macht nichts, kann aber
     * von abgeleiteten Klassen berschrieben werden.
     */
    virtual void _update();
    
    /// Wie ActorControlledObject::getMovableObject()
    /// Nur schon gebrauchsfertig gecastet.
    rl::SoundStitching* getSoundStitching() const;
    
    
    virtual Ogre::String getObjectType() const;

    // Wollen wir 3D?
    bool is3d() const;
    // Setzen des 3D-Flags.
    void set3d(bool is3d);

    // Sollen der Sound wiederholt werden?
    bool isLooping() const;
    // Setzen des Loop-Flags.
    void setLooping(bool looping);
    float getLength() const;
    /// Pausiert den Sound.
    void pause();
    /// Pausiert den Sound.
    void pause(bool pausing);
    /// Ist der Sound pausiert?
    bool isPaused();
    /// Stoppt den Sound.
    virtual void stop();
    /// Gibt die eingestellte relative Lautstaerke der Soundquelle zurueck (0.0 ... 1.0)
    const Ogre::Real getVolume() const; 
    /// Setzt die relative Lautstaerke der Soundquelle (0.0 .. 1.0).
    void setVolume(const Ogre::Real gain);
    // Sets the priority of this sound
    void setPriority(const int priority);
    // Gets the priority of this sound
    const int getPriority() const;
	/// Lädt den Ton
	void load();
    /// Entlädt den Sound.
    void unload();
    /// Aktivieren
    virtual void setActive(bool active);

    /// Start sound sequence
    void start();
    /// Spielt den Sound ab.
    virtual void play(bool destroyWhenDone=false);
    /// Laeuft der Sound noch
    virtual const bool isPlaying() const = 0;
	virtual bool isMeshObject() const;

    /// Count of configured slots.
    unsigned int getNumSlots();
    /// Add a sound resource to the cache overwritting existing sound resource.
    virtual void addSoundToCache(rl::CeGuiString label, rl::SoundResourcePtr sound);
    /// Assign a sound recource to a slot.
    virtual void putSoundIntoSlot(unsigned int, rl::CeGuiString label) = 0;
    /// Get the slot of the plaing sound.
    virtual unsigned int getPlayingSlot() = 0;
}; 


class ParticleSystemObject : public rl::ActorControlledObject
{
public:
     void setActive(bool active);
private:
	 ParticleSystemObject(const Ogre::String& name, const Ogre::String& partSys );
};

class CameraObject : public rl::PhysicalObject
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

	const const Ogre::String& getName() const;
	const Ogre::Vector3& getPosition(void);
	const Ogre::Quaternion& getOrientation(void);
	const Ogre::Vector3& getWorldPosition(void);
	const Ogre::Quaternion& getWorldOrientation(void);

    void setPosition(Ogre::Real x, Ogre::Real y, Ogre::Real z);
    void setPosition( const Ogre::Vector3& );
    void setOrientation( const Ogre::Quaternion& );
    
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

    void merge(Actor* actor, const Ogre::String& slot);

    rl::Actor* getChildByName(const Ogre::String& name ) const;
    
    void setVisible( bool vis, bool cascade = true );
	bool isVisible() const;
       
	void roll(Ogre::Real angleunits);
	void pitch(Ogre::Real angleunits);
	void yaw(Ogre::Real angleunits);
	
	unsigned long getQueryFlags() const;
    void setQueryFlags( unsigned long mask = 0xFFFFFFFF );
    void addQueryFlag( unsigned long flag  );
    void removeQueryFlag( unsigned long flag );
    void setListenerOf (Ogre::SceneNode *node);
    Ogre::Bone *_getBone() const;
    Ogre::SceneNode* _getSceneNode() const;
    
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
    PhysicalThing(
			rl::GeometryType geomType,
			rl::PhysicalObject* po, 
			Ogre::Real mass, 
			bool hullModifier = false);

    void setPosition(Ogre::Real x, Ogre::Real y, Ogre::Real z);
    void setOrientation(Ogre::Real w, Ogre::Real x, Ogre::Real y, Ogre::Real z);
    
    void setVelocity(const Ogre::Vector3& vel);

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
    void setMass(Ogre::Real mass);
    
    void updateCollisionHull();

    void setContactListener(rl::PhysicsContactListener* listener);
    rl::PhysicsContactListener* getContactListener() const;

    void fitToPose(const Ogre::String& name);

};

enum GeometryType {
    GT_NONE = -1,
    GT_BOX = 0,
    GT_SPHERE = 1,
    GT_CAPSULE = 2,
    GT_MESH = 3,
    GT_ELLIPSOID = 4,
    GT_CONVEXHULL = 5
};
    


class PhysicsManager
{
public:

    PhysicsManager();
	static PhysicsManager& getSingleton(void);
    
    rl::PhysicalThing* createPhysicalThing(rl::GeometryType geomType, rl::PhysicalObject* po,
		Ogre::Real mass);

    void removeAndDestroyPhysicalThing(PhysicalThing* thing);

    void createPhysicsProxy(PhysicalThing* pt);

    void createPhysicsProxy_RagDoll(PhysicalThing* pt);

    void destroyPhysicsProxy(PhysicalThing* pt);

    // Global Settings
    void setGravity(Ogre::Real x, Ogre::Real y, Ogre::Real z);
    Ogre::Vector3 getGravity() const;

    bool isEnabled() const;
    void setEnabled(bool enabled);
	
	void toggleDebugMode();
    bool isDebugMode() const;
};

class DebugVisualsManager
{
public:
    DebugVisualsManager();
    static DebugVisualsManager& getSingleton(void);

    void show(unsigned long flags);
    void hide(unsigned long flags);
    void showAll();
    void hideAll();
    void toggleAll();
};
  
class ContentLoader;
    
%feature("director") ContentModule; 
class ContentModule
{
public:
	ContentModule(
		const Ogre::String& id, 
		const rl::CeGuiString& name, 
		bool common, 
		long minimumEngineVersion);
		
	virtual ~ContentModule();
		
	virtual const Ogre::StringVector& getDependencies() const = 0;
	virtual const Ogre::StringVector& getTextureLocations() const = 0;
	virtual const Ogre::StringVector& getSoundLocations() const = 0;
	virtual const Ogre::StringVector& getModelLocations() const = 0;

	virtual void start() = 0;
	
	void loadContent();
	
	void registerContentLoader(rl::ContentLoader* loader);
};

%feature("director") ContentLoader;
class ContentLoader
{
public:
    ContentLoader(const Ogre::String& resourceGroup);
    virtual ~ContentLoader();

    virtual void loadContent() = 0;
    virtual void unloadContent() = 0;

    virtual const CEGUI::String getClassName() const = 0;
};

class CoreSubsystem {
private:
	CoreSubsystem();
public:
    static CoreSubsystem& getSingleton(void);
    
    void loadMap(const Ogre::String& type, const Ogre::String& filename,
		const Ogre::String& resourceGroup);

	void registerModule(rl::ContentModule* module);

	rl::ContentModule* getActiveAdventureModule() const;
    rl::ContentModule* getModule(const Ogre::String& moduleId) const;

    rl::World* getWorld();
    void makeScreenshot(const Ogre::String& sName);
    bool isInitialized() const;
    void setScheme(const Ogre::String& schemeName);
    bool getDeveloperMode() const;
    void setTimeFactor(const Ogre::Real& factor = 1.0F);
    void setPaused(bool paused);
};

class Sound 
{
private:
	Sound(const SoundResourcePtr& res);
public:
    // Einige Standardwerte fr Soundprioritten.
    enum SOUND_PRIORITY {
            PRIO_SPEECH = 30,
            PRIO_MUSIC = 75,
            PRIO_SFX_ENVIRONMENT = 128,
            PRIO_SFX_OTHER = 175,
            PRIO_OTHER = 220
    };

	virtual ~Sound();
    
    /// Name zurueckgeben
    const Ogre::String& getName() const;

    /// Gibt die eingestellte Position der Soundquelle zurueck
    const Ogre::Vector3 getPosition() const;
    /// Setzt die Position der Soundquelle.
    void setPosition(const Ogre::Vector3& direction);
    /// Gibt die eingestellte relative Lautstaerke der Soundquelle zurueck
    const Ogre::Real getVolume() const; 
    /// Setzt die relative Lautstaerke der Soundquelle.
    void setVolume(const Ogre::Real gain);
    /// Gibt die Richtung der Soundquelle zurueck.
    const Ogre::Quaternion getDirection() const;
    /// Gibt die Geschwindigkeit der Soundquelle zurueck.
    const Ogre::Vector3 getVelocity() const;
    /// Setzt die Richtung der Soundquelle.
    void setDirection(const Ogre::Quaternion&);
    /// Setzt die Geschwindigkeit der Soundquelle.
    void setVelocity(const Ogre::Vector3&);
    /// Setzt die Entfernung, ab der ein 3D-Sound leiser wird
	void setRolloffStartDistance(const Ogre::Real dist);
	const Ogre::Real getRolloffStartDistance() const;
	/// Setzt die Entfernung, ab der ein 3D-Sound nicht mehr leiser wird
	void setRolloffEndDistance(const Ogre::Real dist);
	const Ogre::Real getRolloffEndDistance() const;

    /// Spielt den Sound ab.
    void play() ;
    /// Pausiert den Sound.
    void pause(bool pausing) ;
    /// Ist der Sound pausiert?
    bool isPaused() ;
    /// Stoppt den Sound.
    void stop() ;
    /// Laeuft der Sound noch
    const bool isPlaying() const ;

    // Sets the priority of this sound
    virtual void setPriority(const int priority);
    // Gets the priority of this sound
    virtual const int getPriority() const;

    /// Gibt zurck ob der Sound geloopt werden soll 
    bool isLooping() const;
    /// Setzt ob der Sound geloopt werden soll 
    void setLooping( bool looping );
    
    // Sind wir gueltig
    bool isValid() const;
    
    bool is3d();
    void set3d(bool is3d);
};

typedef std::map<CeGuiString, SoundResourcePtr> SoundCache;

class SoundStitching
{
public:
    // Einige Standardwerte für Soundprioritäten.
    enum SOUND_PRIORITY {
        PRIO_SPEECH = 30,
        PRIO_MUSIC = 75,
        PRIO_SFX_ENVIRONMENT = 128,
        PRIO_SFX_OTHER = 175,
        PRIO_OTHER = 220
    };

    /// Konstruktor
    SoundStitching(unsigned int numSlots, rl::CeGuiString name, rl::SoundDriver* creator);
    /// Destruktor
    virtual ~SoundStitching();
      
    // SoundResource zurueckgeben.
    const rl::SoundResourcePtr& getSoundResource() const;
    // Wollen wir 3D?
    bool is3d() const;
    // Setzen des 3D-Flags.
    void set3d(bool is3d);
    // Sind wir gueltig
    virtual bool isValid() const;
    
    // Sollen der Sound wiederholt werden?
    bool isLooping() const;
    // Setzen des Loop-Flags.
    void setLooping(bool looping);

    virtual float getLength() const = 0;

    /// Unsere Bounding-Box
    virtual const Ogre::AxisAlignedBox& getBoundingBox(void) const;
    /// Bound-Radius
    virtual Ogre::Real getBoundingRadius() const;
    /// Rendern
    virtual void _updateRenderQueue(Ogre::RenderQueue *queue);

    /// Count of configured slots.
    unsigned int getNumSlots();
    /// Add a sound resource to the cache overwritting existing sound resource.
    virtual void addSoundToCache(rl::CeGuiString label, rl::SoundResourcePtr sound);
    /// Assign a sound recource to a slot.
    virtual void putSoundIntoSlot(unsigned int, rl::CeGuiString label) = 0;
    /// Get the slot of the plaing sound.
    virtual unsigned int getPlayingSlot() = 0;

    /// Gibt die eingestellte Position der Soundquelle zurueck
    virtual const Ogre::Vector3 getPosition() const = 0;
    /// Setzt die Position der Soundquelle.
    virtual void setPosition(const Ogre::Vector3& direction) = 0;
    /// Gibt die eingestellte relative Lautstaerke der Soundquelle zurueck (0.0 ... 1.0)
    virtual const Ogre::Real getVolume() const = 0; 
    /// Setzt die relative Lautstaerke der Soundquelle (0.0 .. 1.0).
    virtual void setVolume(const Ogre::Real gain) = 0;
    /// Gibt die Richtung der Soundquelle zurueck.
    virtual const Ogre::Quaternion getDirection() const = 0;
    /// Gibt die Geschwindigkeit der Soundquelle zurueck.
    virtual const Ogre::Vector3 getVelocity() const = 0;
    /// Setzt die Richtung der Soundquelle.
    virtual void setDirection(const Ogre::Quaternion&) = 0;
    /// Setzt die Geschwindigkeit der Soundquelle.
    virtual void setVelocity(const Ogre::Vector3&) = 0;
    /// Setzt die Entfernung, ab der ein 3D-Sound leiser wird
    virtual void setRolloffStartDistance(const Ogre::Real&);
    virtual const Ogre::Real getRolloffStartDistance() const;
    /// Setzt die Entfernung, ab der ein 3D-Sound nicht mehr leiser wird
    virtual void setRolloffEndDistance(const Ogre::Real&);
    virtual const Ogre::Real getRolloffEndDistance() const;
    // Sets the priority of this sound
    virtual void setPriority(const int priority) = 0;
    // Gets the priority of this sound
    virtual const int getPriority() const = 0;

    /// Spielt den Sound ab.
    virtual void play(bool destroyWhenDone=false) = 0;
    /// Pausiert den Sound.
    virtual void pause(bool pausing) = 0;
    /// Ist der Sound pausiert?
    virtual bool isPaused() = 0;
    /// Stoppt den Sound.
    virtual void stop() = 0;
    /// Laeuft der Sound noch
    virtual const bool isPlaying() const = 0;
   
}; 

class SoundManager
{
public:

    SoundManager();
    static SoundManager& getSingleton(void);
    Sound* createSound(const Ogre::String& res, rl::SoundType type=ST_SAMPLE);
    void destroySound(Sound*);
};

class GameLoop
{
private:
	GameLoop();
public:      
	static GameLoop& getSingleton(void);
	void quitGame();
};

%apply SWIGTYPE *DYNAMIC { Job * };

%feature("director") Job;
class Job
{
public:
    /**
     * Constructor.
     *
     *  @param isDiscardable Set this true, if the Job's goal can be reached in a single
     *         step. For instance if a Job is supposed to slowly close a window by fading
     *         its alpha to 0, then it is discardable, and Job#discard will just finish
     *         the process by closing the window.
     *
     *  @param destroyWhenDone Set this to true, when the JobScheduler shall delete the
     *         Job, after execution is finished. This should usually be the case, but
     *         sometimes it is sensible to pool a number of Jobs for reuse.
     */
	Job( bool isDiscardable, bool destroyWhenDone );
	virtual ~Job();
	
	/**
     * This is the function in which the job is supposed to do whatever it is supposed to do.
     * This function is called by the JobScheduler at most once per frame. The frequency depends
     * on the Job's priority given with JobScheduler#addJob.
     * @param time the frame-time time, since the last call of this function.
     * @return true, when the job is done, false else. If false is returned. The Job will be
     * rescheduled for another execution.
     */
	virtual bool execute(Ogre::Real time) = 0;
    private:
    bool execute(Ogre::Real time);
    /// Returns whether the job can be removed from the queue by the scheduler,
    virtual bool isDiscardable();

    /// Finish whatever the Job is doing. It won't get a chance to continue.
    /// Overloaded functions must *not* call this implementation.
    virtual void discard();

    // Returns true, if the Job shall be deleted, if the Job is finished. Returns false else.
    virtual bool destroyWhenDone();
};

class PlaySound2dJob : public rl::Job
{
public:
    PlaySound2dJob(const Ogre::String& sound, Ogre::Real volume=1.0f, int priority = Sound::PRIO_MUSIC);
    virtual ~PlaySound2dJob();

private:
    bool execute(Ogre::Real time);
};

class PlaySound3dJob : public rl::Job
{
public:
    PlaySound3dJob(const Ogre::String& sound, const Ogre::Vector3& pos, Ogre::Real volume=1.0f, int priority = Sound::PRIO_SFX_ENVIRONMENT);
    virtual ~PlaySound3dJob();

private:
    bool execute(Ogre::Real time);
};

class PlayAnimationJob : public rl::Job
{
public:
    PlayAnimationJob(Actor* actor, const Ogre::String& anim, Ogre::Real duration,
		bool replaceAllAnims=true);
    PlayAnimationJob(Actor* actor, const Ogre::String& anim, int loops = 1,
		bool replaceAllAnims=true);
    virtual ~PlayAnimationJob();
    
private:
    bool execute(Ogre::Real time);
};

class StartAnimationJob : public rl::Job
{
public:
    StartAnimationJob(Actor* actor, const Ogre::String& anim, bool doLoop=false,
		bool replaceAllAnims=true);
    virtual ~StartAnimationJob();
    
private:
    bool execute(Ogre::Real time);
};

//class LightFadeJob : public rl::Job
//{
//public:
//    LightFadeJob(LightObject* lightObject, const Ogre::ColourValue& targetDiffuse, 
//        const Ogre::ColourValue& targetSpecular, Ogre::Real duration = 1.0f);
//
//    virtual bool execute(Ogre::Real time);
//};

class JobQueue : public rl::Job
{
public:
    JobQueue();
    virtual ~JobQueue();
    void add(Job* job);
    
private:
    bool execute(Ogre::Real time);
};

class JobSet : public rl::Job
{
public:
    JobSet();
    virtual ~JobSet();
    void add(Job* job);
    
private:
    bool execute(Ogre::Real time);
};

class JobScheduler
{
public:
    typedef enum {JP_LOW = 10, JP_NORMAL = 20, JP_HIGH = 30} JobPriority;
	static JobScheduler& getSingleton(void);
    unsigned long addJob(rl::Job* job,
		rl::JobScheduler::JobPriority priority = JobScheduler::JP_NORMAL,
		Ogre::Real delay=0.0f);
};

class Zone
{
public:
	void addLight(Actor* light);
    void addSound(const Ogre::String& name);
	void addTrigger(rl::Trigger* trigger);
	std::list<Actor*> getLights() const;
    std::list<Ogre::String> getSounds() const;
	std::list<rl::Trigger*> getTriggers() const;
    void removeLight(Actor* light);
    void removeSound(const Ogre::String& name);
    void removeTrigger(rl::Trigger* trigger);
protected:
    Zone(long id);
private:
    Zone();
};


class ZoneManager
{
public:
	static const ZoneManager& getSingleton();
    rl::Zone* createZone(const Ogre::String& name, bool needsToBeSaved);
    void destroyZone(const Ogre::String& name);

    void addAreaToZone(const Ogre::String& name, 
        Ogre::Vector3 size, rl::GeometryType geom,
        Ogre::Vector3 position, Ogre::Vector3 offset, Ogre::Quaternion orientation,
        Ogre::Real transitionDistance,
        unsigned long queryflags);

    void subtractAreaFromZone(const Ogre::String& name, 
        Ogre::Vector3 size, rl::GeometryType geom,
        Ogre::Vector3 position, Ogre::Vector3 offset, Ogre::Quaternion orientation,
        Ogre::Real transitionDistance,
        unsigned long queryflags);

    void addMeshAreaToZone(const Ogre::String& name,
        const Ogre::String& meshname, rl::GeometryType geom,
        Ogre::Vector3 position,
        Ogre::Vector3 scale, Ogre::Vector3 offset, Ogre::Quaternion orientation,
        Ogre::Real transitionDistance,
        unsigned long queryflags);

    void subtractMeshAreaFromZone(const Ogre::String& name,
        const Ogre::String& meshname, rl::GeometryType geom,
        Ogre::Vector3 position,
        Ogre::Vector3 scale, Ogre::Vector3 offset, Ogre::Quaternion orientation,
        Ogre::Real transitionDistance,
        unsigned long queryflags);
        
	rl::Zone* getDefaultZone();
	rl::Zone* getZone(const Ogre::String& name);
    rl::Zone* getZone(long id);
    bool isZoneActive(const rl::Zone *zone) const;
    bool isInZone(Zone* zone, Actor *actor);
};

%feature("director") Trigger;
class  Trigger
{
public:
    Trigger(const Ogre::String &classname, const Ogre::String &name, bool needsToBeSaved);
    virtual ~Trigger();
	virtual bool activate() = 0;
	virtual bool deactivate() = 0;
	virtual bool deleteIfZoneDestroyed() const;
	const Ogre::String& getName() const;
    virtual const rl::Property getProperty(const rl::CeGuiString& key) const;
    virtual void setProperty(const rl::CeGuiString& key, const rl::Property& value);
    virtual rl::PropertyKeys getAllPropertyKeys() const;
};

class SoundFadeJob : public rl::Job
{
public:
    SoundFadeJob(SoundObject* soundObject, rl::SoundFadeFunctor* functor, bool destroyWhenDone);

    virtual bool execute(Ogre::Real time);
    virtual void discard();
};

%rename(getVolume) rl::SoundFadeFunctor::operator();
class SoundFadeFunctor
{
public:
    SoundFadeFunctor(Ogre::Real duration);

    virtual Ogre::Real operator()(Ogre::Real time) = 0;
};

%rename(getVolume) rl::LinearSoundFadeFunctor::operator();
class LinearSoundFadeFunctor : public rl::SoundFadeFunctor
{
public:
    LinearSoundFadeFunctor(Ogre::Real duration, bool up);

    virtual Ogre::Real operator()(Ogre::Real time);
};

%rename(getVolume) rl::PolynomicSoundFadeFunctor::operator();
typedef std::vector< std::pair<Ogre::Real, Ogre::Real> > InterpolationPoints;
typedef std::vector<Ogre::Real> Parameters;
class PolynomicSoundFadeFunctor : public rl::SoundFadeFunctor
{
public:
    PolynomicSoundFadeFunctor(Ogre::Real duration, InterpolationPoints *points);
	~PolynomicSoundFadeFunctor();

    virtual Ogre::Real operator()(Ogre::Real time);
};

typedef RL_LONGLONG rl::Time;

class TimeSource : public rl::GameTask
{
public:
    typedef enum
    {
        REALTIME_CONTINUOUS,
        REALTIME_INTERRUPTABLE,
        GAMETIME
    }  TimeSourceType;
    
    virtual rl::TimeSource::TimeSourceType getType() const = 0;

    virtual rl::Time getClock() const = 0;
    virtual void setClock(const rl::Time& time) = 0;
    virtual void setTimeFactor(Ogre::Real factor = 1.0) = 0;
};

class TimeSourceManager
{
public:
    static rl::TimeSourceManager& getSingleton();
    void registerTimeSource(rl::TimeSource* ts);
    rl::TimeSource* getTimeSource(const rl::TimeSource::TimeSourceType& type) const;
};


}
