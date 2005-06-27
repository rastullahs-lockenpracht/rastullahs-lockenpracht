%module(package="pythontest", directors="1") pythontest

%{
extern "C" { 
#include <Python.h> 
}

#include "TestClass.h"

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
