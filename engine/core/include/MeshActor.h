#ifndef __MeshActor_H__
#define __MeshActor_H__

#include "CorePrerequisites.h"
#include "Actor.h"

namespace rl {

class _RlCoreExport MeshActor : public virtual Actor
{
public:
	MeshActor(const String& name, Entity* pEntity);
	MeshActor(const String& name, SceneNode* pParentSceneNode, Entity* pEntity);

	virtual ~MeshActor();
	virtual const String& getTypeName();

	Entity* getEntity(void);

    void setScale( Real x, Real y, Real z );

    void startAnimation(const String&);
	void stopAnimation(const String&);

    void setCastShadows (bool enabled);
    bool getCastShadows (void) const;

    void attachActorToBone(Actor* actor, String name);
    void attachActorToBone(Actor* actor,  unsigned short id);
    void detachActorFromBone(Actor* actor);
protected:
	static const String TYPENAME;
	Entity* mEntity;
};

}
#endif

