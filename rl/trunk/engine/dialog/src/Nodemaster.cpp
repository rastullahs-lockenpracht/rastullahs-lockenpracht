#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/dom/DOM.hpp>

#include "Nodemaster.h"
#include "Utils.h"
//--	Nodemaster.cpp



XERCES_CPP_NAMESPACE_USE

namespace rl
{
	Nodemaster* Nodemaster::cached;
	Nodemaster::Nodemaster() { }
	Nodemaster::Nodemaster(const string &templateValue) : mTemplate(templateValue) { }

	Nodemaster::~Nodemaster() {}

	void Nodemaster::deleteAllNodes()
	{
		map<string, Nodemaster *>::const_iterator itr = mChildren.begin();
		for(itr++;itr!=mChildren.end();itr++)
		{
			delete itr->second; //child;
		}
		mChildren.clear();
	}

	DOMNode* Nodemaster::getTemplateNode() const 
	{
		XercesDOMParser* parser=new XercesDOMParser();
		MemBufInputSource mTest((const XMLByte *)mTemplate.data(),mTemplate.size(),"test",false);
		parser->parse(mTest);
		DOMDocument* doc=parser->adoptDocument();
		if ( doc == NULL ) 
			return NULL;
		delete parser;
		return doc;
	}

	string Nodemaster::getTemplate()
	{
		return mTemplate;
	}

	void Nodemaster::setTemplate(const string &data) 
	{
		mTemplate=data;
	}

	void Nodemaster::addChild(const string &key, Nodemaster *child) 
	{
		cached = child;
		mChildren[toUpper(key)] = cached;
	}

	Nodemaster *Nodemaster::getChild(const string &key) 
	{
		if ( key.empty() ) return cached;
		map<string, Nodemaster *>::iterator itr = mChildren.find(toUpper(key));
		if ( itr == mChildren.end() ) return NULL;
		cached = itr->second;
		return cached;
	}
}
