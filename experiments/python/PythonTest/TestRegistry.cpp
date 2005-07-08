//#include "stdafx.h"
#include "TestClass.h"
#include "TestRegistry.h"

static TestRegistry* sInstance = 0;

TestRegistry::TestRegistry()
: mRegistry()
{
	mRegistry.clear();
}

TestRegistry* TestRegistry::getInstance()
{
	if( sInstance == 0 )
        sInstance = new TestRegistry();

	return sInstance;
}

TestClass* TestRegistry::get(int num) const
{
	return mRegistry[num];
}

void TestRegistry::add(TestClass* obj)
{
	mRegistry.push_back(obj);
}