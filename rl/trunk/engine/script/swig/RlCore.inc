/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
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

%apply SWIGTYPE *DYNAMIC { Actor * };

class ActorManager {
private:
	ActorManager();
public:
	static ActorManager& getSingleton(void);

	rl::MeshActor* createMeshActor(const String& name,const String& mesh);
	rl::GameActor* createGameActor(const String& name,const String& meshname);
	rl::LightActor* createLightActor(const String& name, int type);
	rl::ParticleSystemActor* createParticleSystemActor(const String& name,const String& partname);
	
	rl::Actor* getActor(const String& name);
	void deleteActor(const String& name);
};

class World {
public:	
	enum FogMode { FOG_NONE, FOG_EXP, FOG_EXP2, FOG_LINEAR  };

	rl::CameraActor* getActiveCamera(void) = 0;
	
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

class BSPWorld : public World {
private:
	BSPWorld();
};

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
private:
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
	const String& getTypeName();
	const String& getName();
	
	const Vector3& getPosition(void);
	
	void setPosition(Real x, Real y, Real z);
	void translate(Real x, Real y, Real z);
	void roll(Real angleunits);
	void pitch(Real angleunits);
	void yaw(Real angleunits);
	void rotate(Real x,Real y,Real z, Real angleunits);
	
	void attachActorToSceneNode(Actor* actor);
    void detachActorFromSceneNode(Actor* actor);
    
	rl::PhysicalThing* getPhysicalThing();
};


class MeshActor : virtual public Actor{
public:
	void setScale( Real x, Real y, Real z );
	
	RlAnimation* startAnimation(const char* anim, Real speed=1.0, unsigned int timesToPlay=0);
	void stopAnimation(const char* anim);
	RlAnimation* getAnimation(const char* anim) const;
	
	%rename(attachActorToNamedBone) attachActorToBone(Actor* actor, String name);
	void attachActorToBone( Actor* actor, String name );
    void attachActorToBone( Actor* actor,  unsigned short id );
    void detachActorFromBone( Actor* actor );
};


class AudibleActor : virtual public Actor {
public:
    void addSound(const String &name);
    void deleteSound(const String &name);
    void playSound(const String &name);
    void stopSound(const String &name);
    const SoundResource* getSound(const String &name) const;
    
    virtual const Vector3& getVelocity() const;
    virtual void setVelocity(const Vector3& velocity);
};

class CameraActor : virtual public Actor {
public:
};

class ParticleSystemActor : virtual public Actor {
public:
	void setScale( Real x, Real y, Real z );
};

class LightActor : virtual public Actor {
public:
	void setAttenuation( Real range, Real constant,Real linear, Real quadric );
    void setDiffuseColour( Real red, Real green, Real blue );
    void setDirection( Real x, Real y, Real z );
    void setSpecularColour( Real red, Real green, Real blue );
    void setSpotlightRange( Real innerangle, Real outerangle, Real falloff = 1.0 );
    void setVisible( bool visible );

	static LightActor* castDown( Actor* pActor );
};

class ListenerActor : public Actor {
public:
	static ListenerActor& getSingleton(void);
	static ListenerActor* getSingletonPtr(void);

	ListenerActor(const String& name);
    ListenerActor(const String& name, SceneNode *pParentSceneNode);
	~ListenerActor();
    const Vector3& getVelocity() const;
    void setVelocity(const Vector3& velocity);
    virtual void setPosition(Real x, Real y, Real z);
    virtual void setOrientation(const Quaternion& orientation);
};

class GameActor : public MeshActor, public AudibleActor, virtual public Actor  {
public:
};

class PhysicalThing {
private:
	PhysicalThing();
public:
	Actor* getActor( void );
	
    bool isDynamic( void );
    void setDynamic( bool dynamix );
	void stopDynamics();

    void addForce(Real dir_x, Real dir_y, Real dir_z);
    void addForceWorldSpace(Real dir_x, Real dir_y, Real dir_z);
    //void addForceWorldSpace(Real dir_x, Real dir_y, Real dir_z,Real pos_x, Real pos_y, Real pos_z);
    void addTorque(Real x, Real y, Real z);
    void addTorqueWorldSpace(Real x, Real y, Real z);
    
    void createCappedCylinderGeometry( Real radius, Real length, const Vector3& position = Vector3::ZERO, const Quaternion& orientation = Quaternion::IDENTITY );
    void createBoxGeometry( const Vector3& length, const Vector3& position = Vector3::ZERO, const Quaternion& orientation = Quaternion::IDENTITY );
    void createSphereGeometry( Real radius, const Vector3& position = Vector3::ZERO, const Quaternion& orientation = Quaternion::IDENTITY );

    void createCappedCylinderMass( Real density, Real radius, Real length, const Vector3& position = Vector3::ZERO, const Quaternion& orientation = Quaternion::IDENTITY );
    void createBoxMass( Real density, const Vector3& length, const Vector3& position = Vector3::ZERO, const Quaternion& orientation = Quaternion::IDENTITY );
    void createSphereMass( Real density, Real radius, const Vector3& position = Vector3::ZERO, const Quaternion& orientation = Quaternion::IDENTITY );


	void setBounceParameters(Real restitutionValue,	Real velocityThreshold);
	Real getBounceRestitutionValue(void);
	Real getBounceVelocityThreshold(void);
	void setSoftness(Real softness);
	Real getSoftness(void);
    void setFriction(Real friction);
    Real getFriction(void);
    void setSoftErp(Real erp);
    Real getSoftErp(void);
	
    //void setLinearVelocity(const Vector3& vel);
    void setLinearVelocity(Real x, Real y, Real z);
    const Vector3& getLinearVelocity(void);
    const Vector3& getAngularVelocity(void);
    //void setAngularVelocity(const Vector3& vel);
    void setAngularVelocity(Real x, Real y, Real z);
};

class PhysicsManager {
private:
	PhysicsManager();
public:  
    PhysicalThing* createPhysicalThing(Actor* actor);
    PhysicalThing* createSpherePhysicalThing(Actor* actor, Real density, bool noDynamics = false);
    PhysicalThing* createBoxPhysicalThing(Actor* actor, Real density, bool noDynamics = false);
    PhysicalThing* createCappedCylinderPhysicalThing(Actor* actor, Real density, bool noDynamics = false);
    void removeAndDestroyPhysicalThing(Actor* actor);

    void activateGlobalSpace();
    void activatePhysicalThingSpace( PhysicalThing* thing );
    void removePhysicalThingSpace( PhysicalThing* thing );
    void moveToCurrentSpace( PhysicalThing* thing );
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
    void  makeScreenshot(const String& sName);
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

%{
static swig_type_info *Actor_dynamic_cast(void **ptr) {
    rl::Actor **ppActor = reinterpret_cast<rl::Actor **>(ptr);
    rl::GameActor *pGameActor = dynamic_cast<rl::GameActor*>(*ppActor);
    if(pGameActor){
      *ptr=reinterpret_cast<void*>(pGameActor);
      return SWIGTYPE_p_rl__GameActor;
    }
    rl::CameraActor *pCameraActor = dynamic_cast<rl::CameraActor*>(*ppActor);
    if(pCameraActor){
      *ptr=reinterpret_cast<void*>(pCameraActor);
      return SWIGTYPE_p_rl__CameraActor;
    }
    rl::LightActor *pLightActor = dynamic_cast<rl::LightActor*>(*ppActor);
    if(pLightActor){
      *ptr=reinterpret_cast<void*>(pLightActor);
      return SWIGTYPE_p_rl__LightActor;
    }
    rl::ParticleSystemActor *pParticleSystemActor =
       dynamic_cast<rl::ParticleSystemActor*>(*ppActor);
    if(pParticleSystemActor){
      *ptr=reinterpret_cast<void*>(pParticleSystemActor);
      return SWIGTYPE_p_rl__ParticleSystemActor;
    }
    rl::MeshActor *pMeshActor =
       dynamic_cast<rl::MeshActor*>(*ppActor);
    if(pMeshActor){
      *ptr=reinterpret_cast<void*>(pMeshActor);
      return SWIGTYPE_p_rl__MeshActor;
    }
    rl::ListenerActor *pListenerActor =
       dynamic_cast<rl::ListenerActor*>(*ppActor);
    if(pListenerActor){
      *ptr=reinterpret_cast<void*>(pListenerActor);
      return SWIGTYPE_p_rl__ListenerActor;
    }
    return 0;
}
%}
DYNAMIC_CAST(SWIGTYPE_p_rl__Actor, Actor_dynamic_cast);
