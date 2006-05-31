#include "XercesString.h"
/*
XercesString& operator=(XercesString& v1, const char* value)
{
	XMLCh* tmp = XMLString::transcode(value);
	v1 = tmp;
	XMLString::release(&tmp);
	return v1;//returnValue;
}*/

XercesString::XercesString(void)
	: basic_string()
{
//	basic_string(
}

XercesString::XercesString(IteratorType pBegin, IteratorType pEnd)
	: basic_string(pBegin, pEnd)
{
}


XercesString::XercesString(const XMLCh* value)
{
	if(value != NULL)
	{
		basic_string::operator +=(value);
	}
}
/*
XercesString::XercesString(const XercesString& value)
	: basic_string(value)
{
}
*/
XercesString::XercesString(const char* value)
	: basic_string(transcode(value))
{
//	static_cast<basic_string>(this);
}


XercesString::~XercesString(void)
{
}


XercesString  XercesString::operator+(const XercesString& pData) const
{
//	basic_string(operator+=(*this, pData);
	XercesString returnValue = *this;
	returnValue += pData.c_str();
/*	XMLCh* tmp = XMLString::replicate(this->c_str());
	XMLString::catString(tmp, pData.c_str());
	XercesString returnValue(tmp);
	XMLString::release(&static_cast<XMLCh*>(tmp));*/
	return returnValue;
}
bool XercesString::operator==(const char* pData) const
{
	XMLCh* tmp = XMLString::transcode(pData);
	bool returnValue = XMLString::equals(this->c_str(), tmp);
	XMLString::release(&tmp);
	return returnValue;
}
/*

XercesString  XercesString::operator+ (const char* const pData) const
{
	basic_string::operator +=(XMLString::transcode(pData));
}
*/
/*
std::string XercesString::toString()
{
	char* tmp = XMLString::transcode(this->c_str());
	std::string temp(tmp);
	XMLString::release(&tmp);
	return temp;
}

*/
XercesString XercesString::transcode(const char* const value)
{
	XMLCh* tmp = XMLString::transcode(value);
	XercesString returnValue(tmp);
	XMLString::release(&tmp);
	return returnValue;
//	this->
	// something to set the data to the transcoded value
	//XMLString::relese(tmp);
}