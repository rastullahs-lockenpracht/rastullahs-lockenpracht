#ifndef TestClass_H
#define TestClass_H

#include <vector>

class TestClass {
public:
	TestClass();
	virtual ~TestClass();

	virtual int getInteger();
	void setInteger(int integer);

	double getDouble();
	void setDouble(double num);

	TestClass* getPointer();
	void setPointer(TestClass* pointer);

	std::string getString();
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