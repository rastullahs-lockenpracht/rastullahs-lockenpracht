//#include "stdafx.h"
#include "TestClass.h"
#include <iostream>

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


char* TestClass::getString()
{
	return mStringAttribute;
}

void TestClass::setString(char* str)
{
	mStringAttribute = str;
}


void TestClass::test1(TestClass* args)
{
	printf("test1\nthis-Pointer:%d\nFelder%d %f '%s' %d\n", (int)this, getInteger(), getDouble(),		getString(), (int)getPointer());
}

void TestClass::test2(TestClass* args)
{
	printf("test2\nthis-Pointer:%d\nFelder%d %f '%s' %d\n", (int)args, args->getInteger(), args->getDouble(), args->getString(), (int)args->getPointer());
}
