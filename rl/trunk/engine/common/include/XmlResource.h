#ifndef __XmlResource_H__
#define __XmlResource_H__

#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include "xercesc/sax2/SAX2XMLReader.hpp"

#include "CommonPrerequisites.h"
#include "Resource.h"

namespace rl {

	class _RlCommonExport XmlResource : 
		public Ogre::Resource
	{
	public:
		XmlResource(const Ogre::String& name);
		~XmlResource();

		void load();
		void unload();

		void parseBy(XERCES_CPP_NAMESPACE::XercesDOMParser* parser);
		void parseBy(XERCES_CPP_NAMESPACE::SAX2XMLReader* parser);
		

	private:
		XERCES_CPP_NAMESPACE::MemBufInputSource* mXmlBuffer;
	};

}

#endif
