#ifndef TestRegistry_H
#define TestRegistry_H

class TestClass;

#include <vector>
#include "TestPre.h"

class _PyTestExport TestRegistry {
public:
	TestRegistry();
	static TestRegistry* getInstance();

	TestClass* get(int num) const;
	void add(TestClass* obj);

private:

	std::vector<TestClass*> mRegistry;
};

#endif