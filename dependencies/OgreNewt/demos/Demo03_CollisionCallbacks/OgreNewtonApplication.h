#pragma once
#include <ExampleApplication.h>

#include <OgreNewt.h>


#include "ConveyorBelt.h"
#include "ConveyorMatCallback.h"


class OgreNewtonApplication :
	public ExampleApplication
{
public:
	enum BodType { BT_BASIC, BT_CONVEYOR };

	OgreNewtonApplication(void);
	~OgreNewtonApplication(void);

protected:
	void createFrameListener();
	void createScene();

	void createMaterials();

private:
	OgreNewt::World* m_World;
	const OgreNewt::MaterialID* mMatDefault;
	const OgreNewt::MaterialID* mMatConveyor;
	OgreNewt::MaterialPair* mMatPairDefaultConveyor;
	ConveyorMatCallback* mConveyorCallback;

	
	Ogre::FrameListener* mNewtonListener;

	std::vector<ConveyorBelt*> mBelts;

};

