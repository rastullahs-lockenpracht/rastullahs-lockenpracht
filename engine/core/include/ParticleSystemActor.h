#ifndef __ParticleSystemActor_H__
#define __ParticleSystemActor_H__

#include "CorePrerequisites.h"
#include "Actor.h"

namespace rl {

class _RlCoreExport ParticleSystemActor : public virtual Actor
{
public:
    /** Constructor
		@param name The Actors name
		@param pSceneNode The SceneNode of the ParticleSystem
		@param pParticleSystem The ParticleSystem
	*/
	ParticleSystemActor( const String& name, ParticleSystem* pParticleSystem );

	/** Constructor
		@param name The Actors name
		@param pParentSceneNode The Parent SceneNode for the ParticleSystem
		@param pParticleSystem The ParticleSystem
	*/
	ParticleSystemActor( const String& name, SceneNode* pParentSceneNode, ParticleSystem* pParticleSystem );
    
    /** Default Deconstructor */
	virtual ~ParticleSystemActor();

    void setScale( Real x, Real y, Real z );

	/** Returns the Typename */
	virtual const String& getTypeName();

	/** Gets the internal ParticleSystem object. */
	ParticleSystem* getOgreParticleSystem(void)
	{
		return mParticleSystem;
	}
private:
    static const String TYPENAME;
    ParticleSystem* mParticleSystem;

};

}

#endif
