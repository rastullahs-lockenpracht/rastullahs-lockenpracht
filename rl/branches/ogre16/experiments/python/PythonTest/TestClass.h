#ifndef TestClass_H
#define TestClass_H

#include <vector>
#include "TestPre.h"

class _PyTestExport TestClass {
public:
	TestClass();
	virtual ~TestClass();

	virtual int getInteger() const;
	void setInteger(int integer);

	double getDouble() const;
	void setDouble(double num);

	TestClass* getPointer() const;
	void setPointer(TestClass* pointer);

	std::string getString() const;
	void setString(const std::string& str);

	void test1(TestClass* arg);
	static void test2(TestClass* arg);

private:
	int mIntegerAttribute;
	double mDoubleAttribut;
	TestClass* mPointerAttribute;
	std::string mStringAttribute;
};

#endif