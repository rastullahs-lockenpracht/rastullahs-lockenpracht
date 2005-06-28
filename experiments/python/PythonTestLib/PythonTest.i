%module(package="pythontest", directors="1") pythontest

%{
extern "C" { 
#include <Python.h> 
}

#include "TestClass.h"
#include "TestRegistry.h"

%}

class TestClass {
public:
	int getInteger();
	void setInteger(int integer);

	double getDouble();
	void setDouble(double num);

	TestClass* getPointer();
	void setPointer(TestClass* pointer);

	char* getString();
	void setString(char* str);

	void test1(TestClass* arg);
	static void test2(TestClass* arg);
};

class TestRegistry {
public:
	TestRegistry();
	static TestRegistry* getInstance();

	TestClass* get(int num);
	void add(TestClass* obj);
};
