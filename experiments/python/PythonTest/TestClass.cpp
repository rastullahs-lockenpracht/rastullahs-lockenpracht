//#include "stdafx.h"
#include "TestClass.h"
#include <iostream>

TestClass::TestClass()
{
	mIntegerAttribute = 0;
	mDoubleAttribut = 0.0;
	mPointerAttribute = NULL;
	mStringAttribute = "";
}

TestClass::~TestClass()
{
}

int TestClass::getInteger()
{
	return mIntegerAttribute;
}

void TestClass::setInteger(int integer)
{
	mIntegerAttribute = integer;
}


double TestClass::getDouble()
{
	return mDoubleAttribut;
}

void TestClass::setDouble(double num)
{
	mDoubleAttribut = num;
}


TestClass* TestClass::getPointer()
{
	return mPointerAttribute;
}

void TestClass::setPointer(TestClass* pointer)
{
	mPointerAttribute = pointer;
}


std::string TestClass::getString()
{
	return mStringAttribute;
}

void TestClass::setString(const std::string& str)
{
	mStringAttribute = str;
}


void TestClass::test1(TestClass* args)
{
	printf("test1\nthis-Pointer:%d\nFelder %d %f '%s' %d\n", (int)this, getInteger(), getDouble(),		getString(), (int)getPointer());
}

void TestClass::test2(TestClass* args)
{
	int intgr = args->getInteger();
	double dbl = args->getDouble();
	std::string str = args->getString();
	int ptr = (int)args->getPointer();
	printf("Felder %d %f '%s' %d\n", intgr, dbl, str.c_str(), ptr);
}
