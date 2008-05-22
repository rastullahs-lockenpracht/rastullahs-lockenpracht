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
	printf("%d Elemente\n", mRegistry.size());
	for (std::vector<TestClass*>::const_iterator iter = mRegistry.begin(); iter != mRegistry.end(); iter++) 
	{
		TestClass* a = *iter;
		printf("%d  ", reinterpret_cast<const int>(a));
		TestClass::test2(a);
	}
	printf("\n");
	return mRegistry[num];
}

void TestRegistry::add(TestClass* obj)
{
	mRegistry.push_back(obj);
}