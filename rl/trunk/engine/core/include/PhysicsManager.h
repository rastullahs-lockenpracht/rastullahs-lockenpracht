#ifndef __PhysicsManager_H__
#define __PhysicsManager_H__

#include <map>
#include <list>
#include <OgreSingleton.h>

#include "SynchronizedTask.h"

#include "CorePrerequisites.h"

struct dxWorld;		/* dynamics world */
struct dxSpace;		/* collision space */
struct dxBody;		/* rigid body (dynamics object) */
struct dxGeom;		/* geometry (collision object) */
struct dxJoint;
struct dxJointNode;
struct dxJointGroup;

typedef struct dxWorld *dWorldID;
typedef struct dxSpace *dSpaceID;
typedef struct dxBody *dBodyID;
typedef struct dxGeom *dGeomID;
typedef struct dxJoint *dJointID;
typedef struct dxJointGroup *dJointGroupID;

class dSpace;
class dBody;
class dWorld;
class dSimpleSpace;
class dJoint;
class dHashSpace;
class dJointGroup;


namespace rl {

    class PhysicalThing;
    class Actor;
    class World;

    typedef std::map<Actor*,PhysicalThing*> PhysicalThingActorMap;
    typedef std::pair<Actor*,PhysicalThing*> PhysicalThingActorPair;

    class _RlCoreExport PhysicsManager : public SynchronizedTask, protected Singleton<PhysicsManager>
    {
    public:
        PhysicsManager();
        virtual ~PhysicsManager();

        virtual void run( Real elapsedTime );

        // Creation of PhysicalThings
        PhysicalThing* createPhysicalThing(Actor* actor);
        PhysicalThing* createSpherePhysicalThing(Actor* actor, Real density, bool noDynamics = false);
        PhysicalThing* createBoxPhysicalThing(Actor* actor, Real density, bool noDynamics = false);
        PhysicalThing* createCappedCylinderPhysicalThing(Actor* actor, Real density, bool noDynamics = false);
        // TODO TriMesh
        void removePhysicalThing(Actor* actor);

        // TODO Joints

        // Spaces for combining non-colliding objects 
        // ( for example the parts of a car... )
        void activateGlobalSpace();
        void activatePhysicalThingSpace( PhysicalThing* thing );
        void removePhysicalThingSpace( PhysicalThing* thing );
        void moveToCurrentSpace( PhysicalThing* thing );
        void createSimpleSpace();

        // Global Settings
        void setGravity( Real x, Real y, Real z );
        Vector3 getGravity();
        void setCFM(Real cfm);
        Real getCFM();
        void setERP(Real erp);
        Real getERP();

        dSpace* getCurrSpace();
        dWorld* getWorld();
        dJointGroup* getContactJointGroup();

        void setWorldScene( World* world );
        void setEnabled(bool enabled);

        // Singleton Stuff
        static PhysicsManager & getSingleton(void);
        static PhysicsManager * getSingletonPtr(void);

        // Collision Callback
        static void collisionCallback(void* data, dGeomID o1, dGeomID o2);
    private:
        bool m_Enabled;

        PhysicalThingActorMap mPhysicalThings;
        IntersectionSceneQuery* mQuery;

        std::list<dSimpleSpace*> mSimpleSpaces;
        std::list<dJoint*> mJoints;

        dHashSpace* mGlobalSpace;
        dWorld* mWorld;
        dSpace* mCurrSpace;
        dJointGroup* mContactJointGroup;

        Real mStepSize;
        Real mLeftOverTime;
    };

}

#endif
