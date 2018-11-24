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

#include <CEGUI/PropertyHelper.h>

#include "XmlProcessor.h"

#include "Exception.h"
#include "Logger.h"
#include "XmlResource.h"
#include "XmlResourceManager.h"

using namespace Ogre;
using CEGUI::utf8;

namespace rl
{

    tinyxml2::XMLElement* XmlProcessor::appendChildElement(tinyxml2::XMLElement* parent, const char* const name) const
    {
        RlAssert(parent != NULL, "XmlProcessor::appendChildElement: parent must not be NULL");

        tinyxml2::XMLElement* child = parent->GetDocument()->NewElement(name);
        parent->LinkEndChild(child);
        return child;
    }

    tinyxml2::XMLElement* XmlProcessor::appendChildElement(tinyxml2::XMLDocument* parent, const char* const name) const
    {
        RlAssert(parent != NULL, "XmlProcessor::appendChildElement: parent must not be NULL");

        return appendChildElement(parent->RootElement(), name);
    }

    const tinyxml2::XMLElement* XmlProcessor::getChildNamed(
        const tinyxml2::XMLElement* parent, const char* const name) const
    {
        RlAssert(parent != NULL, "XmlProcessor::getChildNamed: parent must not be NULL");

        for (const tinyxml2::XMLNode* cur = parent->FirstChild(); cur; cur = cur->NextSibling())
        {
            const tinyxml2::XMLElement* elem = cur->ToElement();
            if (elem && strcmp(elem->Name(), name) == 0)
            {
                return cur->ToElement();
            }
        }

        return NULL;
    }

    tinyxml2::XMLElement* XmlProcessor::getChildNamed(tinyxml2::XMLElement* parent, const char* const name) const
    {
        RlAssert(parent != NULL, "XmlProcessor::getChildNamed: parent must not be NULL");

        for (tinyxml2::XMLNode* cur = parent->FirstChild(); cur; cur = cur->NextSibling())
        {
            tinyxml2::XMLElement* elem = cur->ToElement();
            if (elem && strcmp(elem->Name(), name) == 0)
            {
                return cur->ToElement();
            }
        }

        return NULL;
    }

    class XmlElementFinder : public tinyxml2::XMLVisitor
    {
    public:
        XmlElementFinder(const char* const tagName)
            : mResult()
            , mTagName(tagName)
        {
        }

        virtual bool VisitEnter(const tinyxml2::XMLElement& element, const tinyxml2::XMLAttribute* firstAttribute)
        {
            if (strcmp(element.Value(), mTagName) == 0)
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

    XmlElementList XmlProcessor::getElementsByTagName(const tinyxml2::XMLElement* parent, const char* const name) const
    {
        XmlElementFinder finder(name);
        parent->Accept(&finder);
        return finder.getResult();
    }

    XmlElementList XmlProcessor::getElementsByTagName(const tinyxml2::XMLDocument* parent, const char* const name) const
    {
        return getElementsByTagName(parent->RootElement(), name);
    }

    void XmlProcessor::setValueAsString(tinyxml2::XMLElement* element, const CeGuiString& value) const
    {
        RlAssert(element != NULL, "XmlProcessor::setValueAsString: Element must not be NULL");
        tinyxml2::XMLText* text = element->GetDocument()->NewText(value.c_str());
        element->LinkEndChild(text);
    }

    CeGuiString XmlProcessor::getValueAsString(const tinyxml2::XMLElement* element) const
    {
        RlAssert(element != NULL, "XmlProcessor::getValueAsString: Element must not be NULL");
#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UNICODE
        const utf8* value = reinterpret_cast<const utf8*>(element->FirstChild()->Value());
        return value ? CeGuiString(value) : CeGuiString();
#else
        return getValueAsStdString(element);
#endif
    }

    std::string XmlProcessor::getValueAsStdString(const tinyxml2::XMLElement* element) const
    {
        RlAssert(element != NULL, "XmlProcessor::getValueAsStdString: Element must not be NULL");
        return element->FirstChild()->Value();
    }

    bool XmlProcessor::hasAttribute(const tinyxml2::XMLElement* element, const char* const name) const
    {
        RlAssert(element != NULL, "XmlProcessor::hasAttribute: Element must not be NULL");
        return element->Attribute(name) != NULL;
    }

    bool XmlProcessor::hasNodeName(const tinyxml2::XMLNode* node, const char* const name) const
    {
        RlAssert(node != NULL, "XmlProcessor::hasNodeName: node must not be NULL");
        return strcmp(node->Value(), name) == 0;
    }

    void XmlProcessor::setAttribute(
        tinyxml2::XMLElement* element, const char* const name, const char* const value) const
    {
        RlAssert(element != NULL, "XmlProcessor::setAttribute: Element must not be NULL");
        element->SetAttribute(name, value);
    }

    void XmlProcessor::setAttributeValueAsInteger(
        tinyxml2::XMLElement* element, const char* const name, int value) const
    {
        RlAssert(element != NULL, "XmlProcessor::setAttributeValueAsInteger: Element must not be NULL");
        element->SetAttribute(name, Ogre::StringConverter::toString(value).c_str());
    }

    int XmlProcessor::getAttributeValueAsInteger(const tinyxml2::XMLElement* element, const char* const name) const
    {
        RlAssert(element != NULL, "XmlProcessor::getAttributeValueAsInteger: Element must not be NULL");
        return Ogre::StringConverter::parseInt(element->Attribute(name));
    }

    void XmlProcessor::setAttributeValueAsInt64(
        tinyxml2::XMLElement* element, const char* const name, RL_LONGLONG value) const
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

    RL_LONGLONG XmlProcessor::getAttributeValueAsInt64(
        const tinyxml2::XMLElement* element, const char* const name) const
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

        rVal += CEGUI::PropertyHelper<int>::fromString(value);
        return rVal;
    }

    void XmlProcessor::setAttributeValueAsIntegerPair(
        tinyxml2::XMLElement* element, const char* const name, IntPair value) const
    {
        RlAssert(element != NULL, "XmlProcessor::setAttributeValueAsIntegerPair: Element must not be NULL");
        CeGuiString temp
            = Ogre::StringConverter::toString(value.first) + "," + Ogre::StringConverter::toString(value.second);
        element->SetAttribute(name, temp.c_str());
    }

    IntPair XmlProcessor::getAttributeValueAsIntegerPair(
        const tinyxml2::XMLElement* element, const char* const name) const
    {
        RlAssert(element != NULL, "XmlProcessor::getAttributeValueAsIntegerPair: Element must not be NULL");
        CeGuiString value = getAttributeValueAsString(element, name);
        CeGuiString::size_type comma1 = value.find(",");

        std::pair<int, int> intPairVal = std::make_pair(0, 0);
        if (comma1 != CeGuiString::npos)
        {
            intPairVal = std::make_pair(CEGUI::PropertyHelper<int>::fromString(value.substr(0, comma1)),
                CEGUI::PropertyHelper<int>::fromString(value.substr(comma1 + 1)));
        }

        return intPairVal;
    }

    void XmlProcessor::setAttributeValueAsIntegerTriple(
        tinyxml2::XMLElement* element, const char* const name, Tripel<int> value) const
    {
        RlAssert(element != NULL, "XmlProcessor::setAttributeValueAsIntegerTriple: Element must not be NULL");
        CeGuiString temp = Ogre::StringConverter::toString(value.first) + ","
            + Ogre::StringConverter::toString(value.second) + "," + Ogre::StringConverter::toString(value.third);
        element->SetAttribute(name, temp.c_str());
    }

    Tripel<int> XmlProcessor::getAttributeValueAsIntegerTriple(
        const tinyxml2::XMLElement* element, const char* const name) const
    {
        RlAssert(element != NULL, "XmlProcessor::getAttributeValueAsIntegerTriple: Element must not be NULL");
        CeGuiString value = getAttributeValueAsString(element, name);

        CeGuiString::size_type comma1 = value.find(",");
        CeGuiString::size_type comma2 = value.find(",", comma1 + 1);

        Tripel<int> intTripel(0, 0, 0);
        if (comma1 != CeGuiString::npos && comma2 != CeGuiString::npos)
        {
            intTripel.first = CEGUI::PropertyHelper<float>::fromString(value.substr(0, comma1));
            intTripel.second = CEGUI::PropertyHelper<float>::fromString(value.substr(comma1 + 1, comma2 - comma1 - 1));
            intTripel.third = CEGUI::PropertyHelper<float>::fromString(value.substr(comma2 + 1));
        }

        return intTripel;
    }

    void XmlProcessor::setAttributeValueAsReal(
        tinyxml2::XMLElement* element, const char* const name, Ogre::Real value) const
    {
        RlAssert(element != NULL, "XmlProcessor::setAttributeValueAsReal: Element must not be NULL");
        element->SetAttribute(name, Ogre::StringConverter::toString(value).c_str());
    }

    Ogre::Real XmlProcessor::getAttributeValueAsReal(const tinyxml2::XMLElement* element, const char* const name) const
    {
        RlAssert(element != NULL, "XmlProcessor::getAttributeValueAsReal: Element must not be NULL");
        Ogre::Real rVal = Ogre::StringConverter::parseReal(getAttributeValueAsStdString(element, name));
        return rVal;
    }

    void XmlProcessor::setAttributeValueAsString(
        tinyxml2::XMLElement* element, const char* const name, const CeGuiString& value) const
    {
        RlAssert(element != NULL, "XmlProcessor::setAttributeValueAsString: Element must not be NULL");
        element->SetAttribute(name, value.c_str());
    }

    CeGuiString XmlProcessor::getAttributeValueAsString(
        const tinyxml2::XMLElement* element, const char* const name) const
    {
        RlAssert(element != NULL, "XmlProcessor::getAttributeValueAsString: Element must not be NULL");
#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UNICODE
        const utf8* value = reinterpret_cast<const utf8*>(element->Attribute(name));
        return value ? CeGuiString(value) : CeGuiString();
#else
        return getAttributeValueAsStdString(element, name);
#endif
    }

    void XmlProcessor::setAttributeValueAsStdString(
        tinyxml2::XMLElement* element, const char* const name, const std::string& value) const
    {
        RlAssert(element != NULL, "XmlProcessor::setAttributeValueAsStdString: Element must not be NULL");
        element->SetAttribute(name, value.c_str());
    }

    std::string XmlProcessor::getAttributeValueAsStdString(
        const tinyxml2::XMLElement* element, const char* const name) const
    {
        RlAssert(element != NULL, "XmlProcessor::getAttributeValueAsStdString: Element must not be NULL");
        const char* value = element->Attribute(name);
        return value ? std::string(value) : std::string("");
    }

    void XmlProcessor::setAttributeValueAsBool(tinyxml2::XMLElement* element, const char* const name, bool value) const
    {
        RlAssert(element != NULL, "XmlProcessor::setAttributeValueAsBool: Element must not be NULL");
        element->SetAttribute(name, Ogre::StringConverter::toString(value).c_str());
    }

    bool XmlProcessor::getAttributeValueAsBool(const tinyxml2::XMLElement* element, const char* const name) const
    {
        RlAssert(element != NULL, "XmlProcessor::getAttributeValueAsBool: Element must not be NULL");
        return getAttributeValueAsStdString(element, name) == "true";
    }

    void XmlProcessor::setValueAsBool(tinyxml2::XMLElement* element, bool value) const
    {
        RlAssert(element != NULL, "XmlProcessor::setValueAsBool: Element must not be NULL");
        Ogre::String temp = Ogre::StringConverter::toString(value);
        tinyxml2::XMLText* text = element->GetDocument()->NewText(temp.c_str());
        element->LinkEndChild(text);
    }

    bool XmlProcessor::getValueAsBool(const tinyxml2::XMLElement* element) const
    {
        RlAssert(element != NULL, "XmlProcessor::getValueAsBool: Element must not be NULL");
        return getValueAsStdString(element) == "true";
    }

    void XmlProcessor::setValueAsInteger(tinyxml2::XMLElement* element, int value) const
    {
        RlAssert(element != NULL, "XmlProcessor::setValueAsInteger: Element must not be NULL");
        Ogre::String temp = Ogre::StringConverter::toString(value);
        tinyxml2::XMLText* text = element->GetDocument()->NewText(temp.c_str());
        element->LinkEndChild(text);
    }

    int XmlProcessor::getValueAsInteger(const tinyxml2::XMLElement* element) const
    {
        RlAssert(element != NULL, "XmlProcessor::getValueAsInteger: Element must not be NULL");
        return Ogre::StringConverter::parseInt(element->FirstChild()->Value());
    }

    Ogre::Real XmlProcessor::getValueAsReal(const tinyxml2::XMLElement* element) const
    {
        RlAssert(element != NULL, "XmlProcessor::getValueAsInteger: Element must not be NULL");
        return Ogre::StringConverter::parseReal(getValueAsStdString(element));
    }

    void XmlProcessor::setValueAsIntegerPair(tinyxml2::XMLElement* element, IntPair value) const
    {
        RlAssert(element != NULL, "XmlProcessor::setValueAsIntegerPair: Element must not be NULL");
        CeGuiString temp = CEGUI::PropertyHelper<int>::toString(value.first) + ','
            + CEGUI::PropertyHelper<int>::toString(value.second);
        tinyxml2::XMLText* text = element->GetDocument()->NewText(temp.c_str());
        element->LinkEndChild(text);
    }

    IntPair XmlProcessor::getValueAsIntegerPair(const tinyxml2::XMLElement* element) const
    {
        RlAssert(element != NULL, "XmlProcessor::getValueAsIntegerPair: Element must not be NULL");
        CeGuiString value = getValueAsString(element);
        CeGuiString::size_type comma1 = value.find(",");

        std::pair<int, int> intPairVal = std::make_pair(0, 0);
        if (comma1 != CeGuiString::npos)
        {
            intPairVal = std::make_pair(CEGUI::PropertyHelper<int>::fromString(value.substr(0, comma1)),
                CEGUI::PropertyHelper<int>::fromString(value.substr(comma1 + 1)));
        }
        return intPairVal;
    }

    void XmlProcessor::setValueAsIntegerTriple(tinyxml2::XMLElement* element, Tripel<int> value) const
    {
        RlAssert(element != NULL, "XmlProcessor::setValueAsIntegerTriple: Element must not be NULL");
        RlAssert(element != NULL, "XmlProcessor::setValueAsIntegerPair: Element must not be NULL");
        CeGuiString temp = CEGUI::PropertyHelper<int>::toString(value.first) + ','
            + CEGUI::PropertyHelper<int>::toString(value.second) + ','
            + CEGUI::PropertyHelper<int>::toString(value.third);
        tinyxml2::XMLText* text = element->GetDocument()->NewText(temp.c_str());
        element->LinkEndChild(text);
    }

    Tripel<int> XmlProcessor::getValueAsIntegerTriple(const tinyxml2::XMLElement* element) const
    {
        RlAssert(element != NULL, "XmlProcessor::getValueAsIntegerTriple: Element must not be NULL");
        CeGuiString value = getValueAsString(element);

        CeGuiString::size_type comma1 = value.find(",");
        CeGuiString::size_type comma2 = value.find(",", comma1 + 1);

        Tripel<int> intTripel(0, 0, 0);
        if (comma1 != CeGuiString::npos && comma2 != CeGuiString::npos)
        {
            intTripel.first = CEGUI::PropertyHelper<float>::fromString(value.substr(0, comma1));
            intTripel.second = CEGUI::PropertyHelper<float>::fromString(value.substr(comma1 + 1, comma2 - comma1 - 1));
            intTripel.third = CEGUI::PropertyHelper<float>::fromString(value.substr(comma2 + 1));
        }
        return intTripel;
    }

    void XmlProcessor::setAttributeValueAsVector3(
        tinyxml2::XMLElement* element, const char* const name, Ogre::Vector3 value) const
    {
        RlAssert(element != NULL, "XmlProcessor::setAttributeValueAsVector3: Element must not be NULL");
        std::string temp = Ogre::StringConverter::toString(value.x) + "," + Ogre::StringConverter::toString(value.y)
            + "," + Ogre::StringConverter::toString(value.z);
        element->SetAttribute(name, temp.c_str());
    }

    Ogre::Vector3 XmlProcessor::getAttributeValueAsVector3(
        const tinyxml2::XMLElement* element, const char* const name) const
    {
        RlAssert(element != NULL, "XmlProcessor::getAttributeValueAsVector3: Element must not be NULL");
        CeGuiString value = getAttributeValueAsString(element, name);

        CeGuiString::size_type comma1 = value.find(",");
        CeGuiString::size_type comma2 = value.find(",", comma1 + 1);

        Ogre::Vector3 vec(0, 0, 0);
        if (comma1 != CeGuiString::npos && comma2 != CeGuiString::npos)
        {
            vec.x = CEGUI::PropertyHelper<float>::fromString(value.substr(0, comma1));
            vec.y = CEGUI::PropertyHelper<float>::fromString(value.substr(comma1 + 1, comma2 - comma1 - 1));
            vec.z = CEGUI::PropertyHelper<float>::fromString(value.substr(comma2 + 1));
        }

        return vec;
    }

    void XmlProcessor::setValueAsVector3(tinyxml2::XMLElement* element, Ogre::Vector3 value) const
    {
        RlAssert(element != NULL, "XmlProcessor::setValueAsVector3: Element must not be NULL");
        setAttribute(element, "x", Ogre::StringConverter::toString(value.x).c_str());
        setAttribute(element, "y", Ogre::StringConverter::toString(value.y).c_str());
        setAttribute(element, "z", Ogre::StringConverter::toString(value.z).c_str());
    }

    Ogre::Vector3 XmlProcessor::getValueAsVector3(const tinyxml2::XMLElement* element) const
    {
        RlAssert(element != NULL, "XmlProcessor::getValueAsVector3: Element must not be NULL");
        RlAssert(hasAttribute(element, "x") && hasAttribute(element, "y") && hasAttribute(element, "z"),
            "Element must have attributes x, y and z");
        return Ogre::Vector3(getAttributeValueAsReal(element, "x"), getAttributeValueAsReal(element, "y"),
            getAttributeValueAsReal(element, "z"));
    }

    void XmlProcessor::setValueAsQuaternion(tinyxml2::XMLElement* element, Ogre::Quaternion value) const
    {
        RlAssert(element != NULL, "XmlProcessor::setValueAsQuaternion: Element must not be NULL");
        setAttribute(element, "x", Ogre::StringConverter::toString(value.x).c_str());
        setAttribute(element, "y", Ogre::StringConverter::toString(value.y).c_str());
        setAttribute(element, "z", Ogre::StringConverter::toString(value.z).c_str());
        setAttribute(element, "w", Ogre::StringConverter::toString(value.w).c_str());
    }

    Ogre::Quaternion XmlProcessor::getValueAsQuaternion(const tinyxml2::XMLElement* element) const
    {
        RlAssert(element != NULL, "XmlProcessor::getValueAsQuaternion: Element must not be NULL");
        RlAssert(hasAttribute(element, "x") && hasAttribute(element, "y") && hasAttribute(element, "z")
                && hasAttribute(element, "w"),
            "Element must have attributes x, y z and w");
        return Ogre::Quaternion(getAttributeValueAsReal(element, "x"), getAttributeValueAsReal(element, "y"),
            getAttributeValueAsReal(element, "z"), getAttributeValueAsReal(element, "w"));
    }

    void XmlProcessor::setAttributeValueAsQuaternion(
        tinyxml2::XMLElement* element, const char* const name, Ogre::Quaternion value) const
    {
        RlAssert(element != NULL, "XmlProcessor::setAttributeValueAsQuaternion: Element must not be NULL");
        std::string temp = Ogre::StringConverter::toString(value.w) + "," + Ogre::StringConverter::toString(value.x)
            + "," + Ogre::StringConverter::toString(value.y) + "," + Ogre::StringConverter::toString(value.z);
        element->SetAttribute(name, temp.c_str());
    }

    Ogre::Quaternion XmlProcessor::getAttributeValueAsQuaternion(
        const tinyxml2::XMLElement* element, const char* const name) const
    {
        RlAssert(element != NULL, "XmlProcessor::getAttributeValueAsQuaternion: Element must not be NULL");
        CeGuiString value = getAttributeValueAsString(element, name);

        CeGuiString::size_type comma1 = value.find(",");
        CeGuiString::size_type comma2 = value.find(",", comma1 + 1);
        CeGuiString::size_type comma3 = value.find(",", comma2 + 1);

        Ogre::Quaternion quat(Ogre::Quaternion::IDENTITY);
        if (comma1 != CeGuiString::npos && comma2 != CeGuiString::npos && comma3 != CeGuiString::npos)
        {
            quat.w = CEGUI::PropertyHelper<float>::fromString(value.substr(0, comma1));
            quat.x = CEGUI::PropertyHelper<float>::fromString(value.substr(comma1 + 1, comma2 - comma1 - 1));
            quat.y = CEGUI::PropertyHelper<float>::fromString(value.substr(comma2 + 1, comma3 - comma2 - 1));
            quat.z = CEGUI::PropertyHelper<float>::fromString(value.substr(comma3 + 1));
        }
        else if (comma1 != CeGuiString::npos && comma2 != CeGuiString::npos && comma3 == CeGuiString::npos)
        {
            Quaternion rotX, rotY, rotZ;

            rotX.FromAngleAxis(
                Ogre::Degree(CEGUI::PropertyHelper<float>::fromString(value.substr(0, comma1))), Ogre::Vector3::UNIT_X);
            rotY.FromAngleAxis(
                Ogre::Degree(CEGUI::PropertyHelper<float>::fromString(value.substr(comma1 + 1, comma2 - comma1 - 1))),
                Ogre::Vector3::UNIT_Y);
            rotZ.FromAngleAxis(Ogre::Degree(CEGUI::PropertyHelper<float>::fromString(value.substr(comma2 + 1))),
                Ogre::Vector3::UNIT_Z);

            quat = rotX * rotY * rotZ;
        }
        else
        {
            LOG_ERROR(Logger::COMMON,
                "'" + value + "' is no valid quaternion. Format is 'w, x, y, z' or 'rotX, rotY, rotZ'.");
        }

        return quat;
    }

    tinyxml2::XMLDocument* XmlProcessor::loadDocument(
        const Ogre::String& resourceName, const Ogre::String& resourceGroup)
    {
        XmlPtr res = std::dynamic_pointer_cast<XmlResource>(
            XmlResourceManager::getSingleton().getResourceByName(resourceName));
        if (!res)
        {
            Ogre::String group = ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
            if (resourceGroup != "" && ResourceGroupManager::getSingleton().resourceExists(resourceGroup, resourceName))
            {
                group = resourceGroup;
            }
            res = std::dynamic_pointer_cast<XmlResource>(
                XmlResourceManager::getSingleton().createOrRetrieve(resourceName, group).first);
        }

        tinyxml2::XMLDocument* doc = parseToXmlDocument(res->getContent());

        return doc;
    }

    tinyxml2::XMLDocument* XmlProcessor::loadDocument(const Ogre::DataStreamPtr& stream)
    {
        tinyxml2::XMLDocument* doc = NULL;
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

    tinyxml2::XMLDocument* XmlProcessor::parseToXmlDocument(const char* content) const
    {
        tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
        doc->Parse(content);

        if (doc->Error())
        {
            LOG_ERROR(Logger::COMMON, Ogre::StringUtil::format("XML Error %s", doc->ErrorName()));
            return NULL;
        }

        return doc;
    }

} // end namespace rl
