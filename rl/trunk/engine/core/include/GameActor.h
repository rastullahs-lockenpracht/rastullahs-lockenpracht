#ifndef __GameActor_H__
#define __GameActor_H__

#include "CorePrerequisites.h"

#include "MeshActor.h"
#include "AudibleActor.h"
//#include "ScriptObject.h"
#include "ListenerActor.h"

namespace rl {

class _RlCoreExport GameActor 
	:	public MeshActor,
		public AudibleActor
		//public ScriptObject,
{
public:
    
	/** Constructs a GameActor 
		@param name The Actors unique name
	*/
	GameActor( const String& name, Entity* pEntity);
	GameActor( const String& name, SceneNode* pParentSceneNode, Entity* pEntity);

	virtual ~GameActor();

	void playSound(const String &name);

	/** Returns the TypeName */
	virtual const String& getTypeName();

	static GameActor* castDown( Actor* pActor )
	{	return dynamic_cast<GameActor*>(pActor);  }

    /// Sets the position of this object. 
    virtual void setPosition(Real x, Real y, Real z);
    /// Sets the orientation of this object.
    virtual void setOrientation(const Quaternion& orientation);

    virtual bool isSimpleOgreActor();
protected:
	/** The TypeName */
	static const String TYPENAME;
};

}
#endif
