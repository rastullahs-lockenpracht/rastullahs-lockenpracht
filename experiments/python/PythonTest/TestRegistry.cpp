//#include "stdafx.h"
#include "TestClass.h"
#include "TestRegistry.h"

TestRegistry::TestRegistry()
{
	mRegistry.clear();
}

TestRegistry* TestRegistry::getInstance()
{
	static TestRegistry instance = TestRegistry();
	return &instance;
}

TestClass* TestRegistry::get(int num)
{
	return mRegistry[num];
}

void TestRegistry::add(TestClass* obj)
{
	mRegistry.push_back(obj);
}