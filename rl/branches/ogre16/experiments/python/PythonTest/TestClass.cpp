//#include "stdafx.h"
#include "TestClass.h"
#include <iostream>

TestClass::TestClass()
{
	mIntegerAttribute = 0;
	mDoubleAttribut = 0.0;
	mPointerAttribute = NULL;
	mStringAttribute = "";
    printf("Erstelle TestClass Instanz ( %d ) \n", (int)this );
}

TestClass::~TestClass()
{
    mIntegerAttribute = 0;
    printf("Entferne TestClass Instanz ( %d ) \n", (int)this );
}

int TestClass::getInteger() const
{
	return mIntegerAttribute;
}

void TestClass::setInteger(int integer)
{
	mIntegerAttribute = integer;
}


double TestClass::getDouble() const
{
	return mDoubleAttribut;
}

void TestClass::setDouble(double num)
{
	mDoubleAttribut = num;
}


TestClass* TestClass::getPointer() const
{
	return mPointerAttribute;
}

void TestClass::setPointer(TestClass* pointer)
{
	mPointerAttribute = pointer;
}


std::string TestClass::getString() const
{
	return mStringAttribute;
}

void TestClass::setString(const std::string& str)
{
	mStringAttribute = str;
}


void TestClass::test1(TestClass* args)
{
    printf("test1\nthis-Pointer:%d\nFelder: %d %f '%s' %d\n", 
        (int)this, getInteger(), getDouble(), getString(), (int)getPointer());
}

void TestClass::test2(TestClass* args)
{
	int intgr = args->getInteger();
	double dbl = args->getDouble();
	std::string str = args->getString();
	int ptr = (int)args->getPointer();
    printf( "Felder: " );
    printf( "%d ", intgr );
    printf( "%f ", dbl);
    printf( "'%s' ", str.c_str() );
    printf( "%d \n", ptr );
}
