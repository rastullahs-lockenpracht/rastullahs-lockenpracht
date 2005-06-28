#ifndef TestRegistry_H
#define TestRegistry_H

class TestClass;

#include <vector>

class TestRegistry {
public:
	TestRegistry();
	static TestRegistry* getInstance();

	TestClass* get(int num);
	void add(TestClass* obj);

private:
	std::vector<TestClass*> mRegistry;
};

#endif