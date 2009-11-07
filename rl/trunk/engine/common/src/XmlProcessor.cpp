/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Clarified Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Clarified Artistic License for more details.
 *
 *  You should have received a copy of the Clarified Artistic License
 *  along with this program; if not you can get it here
 *  http://www.jpaulmorrison.com/fbp/artistic2.htm.
 */

#include "stdinc.h" //precompiled header

#include <CEGUIPropertyHelper.h>

#include "XmlProcessor.h"

#include "Exception.h"
#include "Logger.h"
#include "XmlResource.h"
#include "XmlResourceManager.h"

using namespace Ogre;
using CEGUI::utf8;

namespace rl 
{

TiXmlElement* XmlProcessor::appendChildElement(TiXmlElement* parent, const char* const name) const
{
    RlAssert(parent != NULL, "XmlProcessor::appendChildElement: parent must not be NULL" );

    TiXmlElement* child = new TiXmlElement(name);
    parent->LinkEndChild(child);
    return child;
}

TiXmlElement* XmlProcessor::appendChildElement(TiXmlDocument* parent, const char* const name) const
{
    RlAssert(parent != NULL, "XmlProcessor::appendChildElement: parent must not be NULL" );

	return appendChildElement(parent->RootElement(), name);
}


const TiXmlElement* XmlProcessor::getChildNamed(const TiXmlElement* parent, const char* const name) const
{
    RlAssert(parent != NULL, "XmlProcessor::getChildNamed: parent must not be NULL" );

	for (const TiXmlNode* cur = parent->FirstChild(); cur; cur = cur->NextSibling())
	{
		if (cur->Type() == TiXmlNode::ELEMENT && cur->ValueTStr() == name)
		{
			return cur->ToElement();
		}
	}

	return NULL;
}

TiXmlElement* XmlProcessor::getChildNamed(TiXmlElement* parent, const char* const name) const
{
    RlAssert(parent != NULL, "XmlProcessor::getChildNamed: parent must not be NULL" );

	for (TiXmlNode* cur = parent->FirstChild(); cur; cur = cur->NextSibling())
	{
		if (cur->Type() == TiXmlNode::ELEMENT && cur->ValueTStr() == name)
		{
			return cur->ToElement();
		}
	}

	return NULL;
}

class XmlElementFinder : public TiXmlVisitor
{
public:
	XmlElementFinder(const char* const tagName)
		: mResult(),
		  mTagName(tagName)
	{
	}

	virtual bool VisitEnter(const TiXmlElement &element, const TiXmlAttribute *firstAttribute)
	{
		if (element.ValueTStr() == mTagName)
		{
			mResult.push_back(&element);
		}
		return true;
	}

	XmlElementList getResult() const
	{
		return mResult;
	}

private:
	XmlElementList mResult;
	const char* mTagName;
};

XmlElementList XmlProcessor::getElementsByTagName(const TiXmlElement* parent, const char* const name) const
{
	XmlElementFinder finder(name);
	parent->Accept(&finder);
	return finder.getResult();
}

XmlElementList XmlProcessor::getElementsByTagName(const TiXmlDocument* parent, const char* const name) const
{
	return getElementsByTagName(parent->RootElement(), name);
}

void XmlProcessor::setValueAsString(TiXmlElement *element, const CeGuiString &value) const
{
    RlAssert(element != NULL, "XmlProcessor::setValueAsString: Element must not be NULL");
    TiXmlText* text = new TiXmlText(value.c_str());
    element->LinkEndChild(text);
}

CeGuiString XmlProcessor::getValueAsString(const TiXmlElement* element) const
{
    RlAssert(element != NULL, "XmlProcessor::getValueAsString: Element must not be NULL");
	const utf8* value = reinterpret_cast<const utf8*>(element->FirstChild()->Value());
	return value ? CeGuiString(value) : CeGuiString();
}

std::string XmlProcessor::getValueAsStdString(const TiXmlElement* element) const
{
    RlAssert(element != NULL, "XmlProcessor::getValueAsStdString: Element must not be NULL");
	return element->FirstChild()->Value();
}

void XmlProcessor::setValueAsUtf8(TiXmlElement* element, utf8* value) const
{
    RlAssert(element != NULL, "XmlProcessor::setValueAsUtf8: Element must not be NULL");
    CeGuiString temp(value);
    element->LinkEndChild(new TiXmlText(temp.c_str()));
}

const utf8* XmlProcessor::getValueAsUtf8(const TiXmlElement* element) const
{
    RlAssert(element != NULL, "XmlProcessor::getValueAsUtf8: Element must not be NULL");
	return reinterpret_cast<const utf8*>(element->FirstChild()->Value());
}

bool XmlProcessor::hasAttribute(const TiXmlElement* element,const char* const name) const
{
    RlAssert(element != NULL, "XmlProcessor::hasAttribute: Element must not be NULL");
    return element->Attribute(name) != NULL;
}

bool XmlProcessor::hasNodeName(const TiXmlNode* node, const char* const name) const
{
    RlAssert(node != NULL, "XmlProcessor::hasNodeName: node must not be NULL");
    return node->ValueTStr() == name;
}

void XmlProcessor::setAttribute(TiXmlElement* element, const char* const name, const char* const value) const
{
    RlAssert(element != NULL, "XmlProcessor::setAttribute: Element must not be NULL");
    element->SetAttribute(name, value);
}

void XmlProcessor::setAttributeValueAsInteger(TiXmlElement *element, const char *const name, int value) const
{
    RlAssert(element != NULL, "XmlProcessor::setAttributeValueAsInteger: Element must not be NULL");
    element->SetAttribute(name, Ogre::StringConverter::toString(value).c_str());
}

int XmlProcessor::getAttributeValueAsInteger(const TiXmlElement* element,const char* const name) const
{
    RlAssert(element != NULL, "XmlProcessor::getAttributeValueAsInteger: Element must not be NULL");
    return Ogre::StringConverter::parseInt(element->Attribute(name));
}

void XmlProcessor::setAttributeValueAsInt64(TiXmlElement *element, const char *const name, RL_LONGLONG value) const
{
    RlAssert(element != NULL, "XmlProcessor::setAttributeValueAsInt64: Element must not be NULL");
    
    bool negative = false;
    if (value < 0)
    {
        value *= -1;
        negative = true;
    }

    std::string temp("");
    RL_LONGLONG divider = 1000000000;
    while (value > divider)
    {
        RL_LONGLONG remainder = value % divider;
        temp = Ogre::StringConverter::toString(static_cast<int>(remainder), 8, '0') + temp;
        value /= divider;
    }

    if (negative)
    {
        temp = "-" + temp;
    }
    
    element->SetAttribute(name, temp.c_str());
}

RL_LONGLONG XmlProcessor::getAttributeValueAsInt64(const TiXmlElement* element,const char* const name) const
{
    RlAssert(element != NULL, "XmlProcessor::getAttributeValueAsInteger: Element must not be NULL");
	Ogre::String value = element->Attribute(name);

    RL_LONGLONG sign = 1;
    if (value[0] == '-')
    {
        sign = -1;
        value = value.substr(1);
    }

    RL_LONGLONG rVal = 0;
    while (value.length() > 8)
    {
        RL_LONGLONG curPart = Ogre::StringConverter::parseInt(value.substr(0, 8));
        for (size_t i = 0; i < value.length() - 8; ++i)
        {
            curPart *= 10;
        }
        rVal += curPart;
        value = value.substr(8);
    }

    rVal += CEGUI::PropertyHelper::stringToInt(value);
	return rVal;
}

void XmlProcessor::setAttributeValueAsIntegerPair(TiXmlElement* element, const char* const name, IntPair value) const
{
    RlAssert(element != NULL, "XmlProcessor::setAttributeValueAsIntegerPair: Element must not be NULL");
    CeGuiString temp = Ogre::StringConverter::toString(value.first) + "," + Ogre::StringConverter::toString(value.second);
    element->SetAttribute(name, temp.c_str());
}

IntPair XmlProcessor::getAttributeValueAsIntegerPair(const TiXmlElement* element, const char* const name) const
{
    RlAssert(element != NULL, "XmlProcessor::getAttributeValueAsIntegerPair: Element must not be NULL");
    CeGuiString value = getAttributeValueAsString(element, name);
    CeGuiString::size_type comma1 = value.find(",");

	std::pair<int,int> intPairVal = std::make_pair(0, 0);
    if (comma1 != CeGuiString::npos)
    {
		intPairVal = std::make_pair(
			CEGUI::PropertyHelper::stringToInt(value.substr(0, comma1)),
			CEGUI::PropertyHelper::stringToInt(value.substr(comma1 + 1)));
    }

    return intPairVal;
}

void XmlProcessor::setAttributeValueAsIntegerTriple(TiXmlElement *element, const char *const name, Tripel<int> value) const
{
    RlAssert(element != NULL, "XmlProcessor::setAttributeValueAsIntegerTriple: Element must not be NULL");
    CeGuiString temp = Ogre::StringConverter::toString(value.first) + "," + Ogre::StringConverter::toString(value.second) + "," + Ogre::StringConverter::toString(value.third);
    element->SetAttribute(name, temp.c_str());
}

Tripel<int> XmlProcessor::getAttributeValueAsIntegerTriple(const TiXmlElement* element, const char* const name) const
{
    RlAssert(element != NULL, "XmlProcessor::getAttributeValueAsIntegerTriple: Element must not be NULL");
    CeGuiString value = getAttributeValueAsString(element, name);

    CeGuiString::size_type comma1 = value.find(",");
    CeGuiString::size_type comma2 = value.find(",", comma1 + 1);

    Tripel<int> intTripel(0,0,0);
    if (comma1 != CeGuiString::npos && comma2 != CeGuiString::npos)
    {
        intTripel.first = CEGUI::PropertyHelper::stringToFloat(value.substr(0, comma1));
        intTripel.second = CEGUI::PropertyHelper::stringToFloat(value.substr(comma1 + 1, comma2 - comma1 - 1));
        intTripel.third = CEGUI::PropertyHelper::stringToFloat(value.substr(comma2 + 1));
    }

    return intTripel;
}

void XmlProcessor::setAttributeValueAsReal(TiXmlElement *element, const char *const name, Ogre::Real value) const
{
    RlAssert(element != NULL, "XmlProcessor::setAttributeValueAsReal: Element must not be NULL");
    element->SetAttribute(name, Ogre::StringConverter::toString(value).c_str());
}

Ogre::Real XmlProcessor::getAttributeValueAsReal(const TiXmlElement* element,const char* const name) const
{
    RlAssert(element != NULL, "XmlProcessor::getAttributeValueAsReal: Element must not be NULL");
	Ogre::Real rVal = Ogre::StringConverter::parseReal(getAttributeValueAsStdString(element, name));
	return rVal;
}

void XmlProcessor::setAttributeValueAsString(TiXmlElement *element, const char *const name, const CeGuiString &value) const
{
    RlAssert(element != NULL, "XmlProcessor::setAttributeValueAsString: Element must not be NULL");
    element->SetAttribute(name, value.c_str());
}

CeGuiString XmlProcessor::getAttributeValueAsString(const TiXmlElement* element, const char* const name) const
{
    RlAssert(element != NULL, "XmlProcessor::getAttributeValueAsString: Element must not be NULL");
	const utf8* value = reinterpret_cast<const utf8*>(element->Attribute(name));
	return value ? CeGuiString(value) : CeGuiString();
}

void XmlProcessor::setAttributeValueAsStdString(TiXmlElement *element, const char *const name, const std::string &value) const
{
    RlAssert(element != NULL, "XmlProcessor::setAttributeValueAsStdString: Element must not be NULL");
    element->SetAttribute(name, value.c_str());
}

std::string XmlProcessor::getAttributeValueAsStdString(const TiXmlElement* element, const char* const name) const
{
    RlAssert(element != NULL, "XmlProcessor::getAttributeValueAsStdString: Element must not be NULL");
	const char* value = element->Attribute(name);
	return value ? std::string(value) : std::string("");
}

void XmlProcessor::setAttributeValueAsBool(TiXmlElement *element, const char *const name, bool value) const
{
    RlAssert(element != NULL, "XmlProcessor::setAttributeValueAsBool: Element must not be NULL");
    element->SetAttribute(name, Ogre::StringConverter::toString(value).c_str());
}

bool XmlProcessor::getAttributeValueAsBool(const TiXmlElement* element,const char* const name) const
{
    RlAssert(element != NULL, "XmlProcessor::getAttributeValueAsBool: Element must not be NULL");
	return getAttributeValueAsStdString(element, name) == "true";
}

void XmlProcessor::setValueAsBool(TiXmlElement *element, bool value) const
{
    RlAssert(element != NULL, "XmlProcessor::setValueAsBool: Element must not be NULL");
    element->LinkEndChild(new TiXmlText(Ogre::StringConverter::toString(value).c_str()));
}

bool XmlProcessor::getValueAsBool(const TiXmlElement* element) const
{
    RlAssert(element != NULL, "XmlProcessor::getValueAsBool: Element must not be NULL");
    return getValueAsStdString(element) == "true";
}

void XmlProcessor::setValueAsInteger(TiXmlElement *element, int value) const
{
    RlAssert(element != NULL, "XmlProcessor::setValueAsInteger: Element must not be NULL");
    element->LinkEndChild(new TiXmlText(Ogre::StringConverter::toString(value).c_str()));
}

int XmlProcessor::getValueAsInteger(const TiXmlElement* element) const
{
    RlAssert(element != NULL, "XmlProcessor::getValueAsInteger: Element must not be NULL");
	return Ogre::StringConverter::parseInt(element->FirstChild()->Value());
}

Ogre::Real XmlProcessor::getValueAsReal(const TiXmlElement* element) const
{
    RlAssert(element != NULL, "XmlProcessor::getValueAsInteger: Element must not be NULL");
    return Ogre::StringConverter::parseReal(getValueAsStdString(element));
}

void XmlProcessor::setValueAsIntegerPair(TiXmlElement *element, IntPair value) const
{
    RlAssert(element != NULL, "XmlProcessor::setValueAsIntegerPair: Element must not be NULL");
    CeGuiString temp = CEGUI::PropertyHelper::intToString(value.first) + ',' + CEGUI::PropertyHelper::intToString(value.second);
    element->LinkEndChild(new TiXmlText(temp.c_str()));
}

IntPair XmlProcessor::getValueAsIntegerPair(const TiXmlElement* element) const
{
    RlAssert(element != NULL, "XmlProcessor::getValueAsIntegerPair: Element must not be NULL");
    CeGuiString value = getValueAsString(element);
    CeGuiString::size_type comma1 = value.find(",");

	std::pair<int,int> intPairVal = std::make_pair(0, 0);
    if (comma1 != CeGuiString::npos)
    {
		intPairVal = std::make_pair(
			CEGUI::PropertyHelper::stringToInt(value.substr(0, comma1)),
			CEGUI::PropertyHelper::stringToInt(value.substr(comma1 + 1)));
    }
    return intPairVal;
}

void XmlProcessor::setValueAsIntegerTriple(TiXmlElement *element, Tripel<int> value) const
{
    RlAssert(element != NULL, "XmlProcessor::setValueAsIntegerTriple: Element must not be NULL");
    RlAssert(element != NULL, "XmlProcessor::setValueAsIntegerPair: Element must not be NULL");
    CeGuiString temp = CEGUI::PropertyHelper::intToString(value.first) + ',' + CEGUI::PropertyHelper::intToString(value.second) + ',' + CEGUI::PropertyHelper::intToString(value.third);
    element->LinkEndChild(new TiXmlText(temp.c_str()));
}

Tripel<int> XmlProcessor::getValueAsIntegerTriple(const TiXmlElement *element) const
{
    RlAssert(element != NULL, "XmlProcessor::getValueAsIntegerTriple: Element must not be NULL");
    CeGuiString value = getValueAsString(element);

    CeGuiString::size_type comma1 = value.find(",");
    CeGuiString::size_type comma2 = value.find(",", comma1 + 1);

    Tripel<int> intTripel(0,0,0);
    if (comma1 != CeGuiString::npos && comma2 != CeGuiString::npos)
    {
        intTripel.first = CEGUI::PropertyHelper::stringToFloat(value.substr(0, comma1));
        intTripel.second = CEGUI::PropertyHelper::stringToFloat(value.substr(comma1 + 1, comma2 - comma1 - 1));
        intTripel.third = CEGUI::PropertyHelper::stringToFloat(value.substr(comma2 + 1));
    }
    return intTripel;
}

void XmlProcessor::setAttributeValueAsVector3(TiXmlElement *element, const char* const name, Ogre::Vector3 value) const
{
    RlAssert(element != NULL, "XmlProcessor::setAttributeValueAsVector3: Element must not be NULL");
    std::string temp = Ogre::StringConverter::toString(value.x) + "," + Ogre::StringConverter::toString(value.y) + "," + Ogre::StringConverter::toString(value.z);
    element->SetAttribute(name, temp.c_str());
}

Ogre::Vector3 XmlProcessor::getAttributeValueAsVector3(const TiXmlElement* element, const char* const name) const
{
    RlAssert(element != NULL, "XmlProcessor::getAttributeValueAsVector3: Element must not be NULL");
    CeGuiString value = getAttributeValueAsString(element, name);

    CeGuiString::size_type comma1 = value.find(",");
    CeGuiString::size_type comma2 = value.find(",", comma1 + 1);

    Ogre::Vector3 vec(0,0,0);
    if (comma1 != CeGuiString::npos && comma2 != CeGuiString::npos)
    {
        vec.x = CEGUI::PropertyHelper::stringToFloat(value.substr(0, comma1));
        vec.y = CEGUI::PropertyHelper::stringToFloat(value.substr(comma1 + 1, comma2 - comma1 - 1));
        vec.z = CEGUI::PropertyHelper::stringToFloat(value.substr(comma2 + 1));
    }

    return vec;
}

void XmlProcessor::setValueAsVector3(TiXmlElement *element, Ogre::Vector3 value) const
{
    RlAssert(element != NULL, "XmlProcessor::setValueAsVector3: Element must not be NULL");
    setAttribute(element, "x", Ogre::StringConverter::toString(value.x).c_str());
    setAttribute(element, "y", Ogre::StringConverter::toString(value.y).c_str());
    setAttribute(element, "z", Ogre::StringConverter::toString(value.z).c_str());
}

Ogre::Vector3 XmlProcessor::getValueAsVector3(const TiXmlElement* element) const
{
    RlAssert(element != NULL, "XmlProcessor::getValueAsVector3: Element must not be NULL");
	RlAssert(
		hasAttribute(element, "x") 
		&& hasAttribute(element, "y") 
		&& hasAttribute(element, "z"),
		"Element must have attributes x, y and z");
	return Ogre::Vector3(
		getAttributeValueAsReal(element, "x"),
		getAttributeValueAsReal(element, "y"),
		getAttributeValueAsReal(element, "z"));
}

void XmlProcessor::setValueAsQuaternion(TiXmlElement *element, Ogre::Quaternion value) const
{
    RlAssert(element != NULL, "XmlProcessor::setValueAsQuaternion: Element must not be NULL");
    setAttribute(element, "x", Ogre::StringConverter::toString(value.x).c_str());
    setAttribute(element, "y", Ogre::StringConverter::toString(value.y).c_str());
    setAttribute(element, "z", Ogre::StringConverter::toString(value.z).c_str());
    setAttribute(element, "w", Ogre::StringConverter::toString(value.w).c_str());
}

Ogre::Quaternion XmlProcessor::getValueAsQuaternion(const TiXmlElement* element) const
{
    RlAssert(element != NULL, "XmlProcessor::getValueAsQuaternion: Element must not be NULL");
	RlAssert(
		hasAttribute(element, "x") 
		&& hasAttribute(element, "y") 
		&& hasAttribute(element, "z")
        && hasAttribute(element, "w"),
		"Element must have attributes x, y z and w");
    return Ogre::Quaternion(
		getAttributeValueAsReal(element, "x"),
		getAttributeValueAsReal(element, "y"),
		getAttributeValueAsReal(element, "z"),
        getAttributeValueAsReal(element, "w"));
}

void XmlProcessor::setAttributeValueAsQuaternion(TiXmlElement *element, const char* const name, Ogre::Quaternion value) const
{
    RlAssert(element != NULL, "XmlProcessor::setAttributeValueAsQuaternion: Element must not be NULL");
    std::string temp = Ogre::StringConverter::toString(value.w) + "," + Ogre::StringConverter::toString(value.x) + "," + Ogre::StringConverter::toString(value.y) + "," + Ogre::StringConverter::toString(value.z);
    element->SetAttribute(name, temp.c_str());
}

Ogre::Quaternion XmlProcessor::getAttributeValueAsQuaternion(const TiXmlElement* element, const char* const name) const
{
    RlAssert(element != NULL, "XmlProcessor::getAttributeValueAsQuaternion: Element must not be NULL");
    CeGuiString value = getAttributeValueAsString(element, name);

    CeGuiString::size_type comma1 = value.find(",");
    CeGuiString::size_type comma2 = value.find(",", comma1 + 1);
    CeGuiString::size_type comma3 = value.find(",", comma2 + 1);

    Ogre::Quaternion quat(Ogre::Quaternion::IDENTITY);
    if (comma1 != CeGuiString::npos 
        && comma2 != CeGuiString::npos 
        && comma3 != CeGuiString::npos)
    {
        quat.w = CEGUI::PropertyHelper::stringToFloat(value.substr(0, comma1));
        quat.x = CEGUI::PropertyHelper::stringToFloat(value.substr(comma1 + 1, comma2 - comma1 - 1));
        quat.y = CEGUI::PropertyHelper::stringToFloat(value.substr(comma2 + 1, comma3 - comma2 - 1));
        quat.z = CEGUI::PropertyHelper::stringToFloat(value.substr(comma3 + 1));
    }
    else if (comma1 != CeGuiString::npos 
        && comma2 != CeGuiString::npos 
        && comma3 == CeGuiString::npos)
    {
        Quaternion rotX, rotY, rotZ;

        rotX.FromAngleAxis(
		    Ogre::Degree(CEGUI::PropertyHelper::stringToFloat(value.substr(0, comma1))), 
		    Ogre::Vector3::UNIT_X);
        rotY.FromAngleAxis(
		    Ogre::Degree(CEGUI::PropertyHelper::stringToFloat(value.substr(comma1 + 1, comma2 - comma1 - 1))), 
		    Ogre::Vector3::UNIT_Y);
        rotZ.FromAngleAxis(
		    Ogre::Degree(CEGUI::PropertyHelper::stringToFloat(value.substr(comma2 + 1))), 
		    Ogre::Vector3::UNIT_Z);

        quat = rotX * rotY * rotZ;
    }
    else 
    {
        LOG_ERROR(Logger::COMMON, "'" + value + "' is no valid quaternion. Format is 'w, x, y, z' or 'rotX, rotY, rotZ'.");
    }

    return quat;
}

TiXmlDocument* XmlProcessor::loadDocument(const Ogre::String& resourceName, const Ogre::String& resourceGroup)
{
    XmlPtr res = XmlResourceManager::getSingleton().getByName(resourceName);
    if (res.isNull())
    {
        Ogre::String group = ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
        if (resourceGroup != "" 
            && ResourceGroupManager::getSingleton().resourceExists(
                    resourceGroup, resourceName))
        {
            group = resourceGroup;
        }
        res = XmlResourceManager::getSingleton().create(resourceName, group);
    }

    TiXmlDocument* doc = parseToXmlDocument(res->getContent());

    return doc;
}

TiXmlDocument* XmlProcessor::loadDocument(const Ogre::DataStreamPtr& stream)
{
	TiXmlDocument* doc = NULL;
	size_t size = stream->size();
	if (size > 0)
	{
		char* buffer = new char[size + 1];
		stream->read(buffer, size);
		buffer[size] = 0;
		doc = parseToXmlDocument(buffer);
		delete[] buffer;
	}

    return doc;
}

TiXmlDocument* XmlProcessor::parseToXmlDocument(const char* content) const
{
    TiXmlDocument* doc = new TiXmlDocument();
    doc->Parse(content);

    if (doc->Error())
    {
    	std::string msg;
    	msg += "XML Error '";
		msg += doc->ErrorDesc();
		msg += "' at row ";
		msg += doc->ErrorRow();
		msg += ", col ";
		msg += doc->ErrorCol();
    	LOG_ERROR(Logger::COMMON, msg);

    	return NULL;
    }

    return doc;
}

} // end namespace rl
