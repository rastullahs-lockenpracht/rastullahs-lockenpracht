/**
 *
 */
#ifndef XERCES_STRING_H
#define XERCES_STRING_H

#include <xercesc/util/XMLString.hpp>
#include <string>
XERCES_CPP_NAMESPACE_USE

//typedef std::wstring XercesString;
//typedef std::basic_string<XMLCh> XercesString;


//XercesString& operator=(XercesString& v1, const char* value);


class XercesString : public std::basic_string< XMLCh >
{
public:
	typedef std::basic_string<XMLCh>::iterator IteratorType;
	virtual ~XercesString(void);
	XercesString();
	XercesString(IteratorType pBegin, IteratorType pEnd);
	XercesString(const XMLCh* value);
//	XercesString(const XercesString& value);
	XercesString(const char* value);
//	XercesString(const std::string& value);

	XercesString  operator+(const XercesString& pData) const;
	bool operator==(const char* pData) const;
	

	//std::string toString();
	
private:
	XercesString transcode(const char* const value);
};


#endif
