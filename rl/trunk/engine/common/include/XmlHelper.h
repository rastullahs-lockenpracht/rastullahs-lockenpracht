#ifndef __XmlHelper_h__
#define __XmlHelper_h__

#include "CommonPrerequisites.h"

#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/util/TransService.hpp>

#include <CEGUIString.h>

using XERCES_CPP_NAMESPACE::XMLTranscoder;
using XERCES_CPP_NAMESPACE::XMLTransService;
using CEGUI::utf8;

namespace rl {

class _RlCommonExport XmlHelper
{
public:
	static XMLTranscoder* sTranscoder;
	static XMLTransService::Codes sFailCode;

	static XERCES_CPP_NAMESPACE::DOMElement* getChildNamed(XERCES_CPP_NAMESPACE::DOMElement* parent, const char* name);
	static char* getValueAsString(XERCES_CPP_NAMESPACE::DOMElement* element);
	static utf8* getValueAsUtf(XERCES_CPP_NAMESPACE::DOMElement* element);
	static int getValueAsInteger(XERCES_CPP_NAMESPACE::DOMElement* element);
	static void initializeTranscoder();
};

}

#endif
