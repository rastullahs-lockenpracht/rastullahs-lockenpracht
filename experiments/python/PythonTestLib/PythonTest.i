%module(package="pythontest", directors="1") pythontest
%include "std_string.i"

%{
extern "C" { 
#include <Python.h>
}

#include <vector>

#include "TestClass.h"
#include "TestRegistry.h"

%}

%feature("director") TestClass; 
class TestClass {
public:
	TestClass();
	virtual ~TestClass();

	virtual int getInteger() const;
	void setInteger(int integer);

	double getDouble() const;
	void setDouble(double num);

	TestClass* getPointer();
	void setPointer(TestClass* pointer);

	std::string getString() const;
	void setString(const std::string& str);

	void test1(TestClass* arg);
	static void test2(TestClass* arg);
};

class TestRegistry {
public:
	TestRegistry();
	static TestRegistry* getInstance();

	TestClass* get(int num) const;
	void add(TestClass* obj);
};
