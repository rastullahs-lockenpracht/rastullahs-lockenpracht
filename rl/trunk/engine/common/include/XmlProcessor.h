/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __XmlHelper_h__
#define __XmlHelper_h__

#include <OgreDataStream.h>

#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/util/XMLChar.hpp>
#include <xercesc/util/TransService.hpp>
#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/sax2/Attributes.hpp>

class XERCES_CPP_NAMESPACE::SAXParseException;

#include "CommonPrerequisites.h"

#include "Tripel.h"

// Da sollten wir uns auf etwas einigen
// So ist das laestig.
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#   pragma warning (push)
#   pragma warning (disable : 4267)
#   include <CEGUIString.h>
#   pragma warning (pop)
#else
#   include <CEGUI/CEGUIString.h>
#endif
using XERCES_CPP_NAMESPACE::XMLTranscoder;
using XERCES_CPP_NAMESPACE::XMLTransService;
using XERCES_CPP_NAMESPACE::Attributes;
using CEGUI::utf8;
using CEGUI::String;

namespace rl {

/**
 * This class offers helper methods for XML processing
 * To use this methods, just inherit this class privately 
 */
class _RlCommonExport XmlProcessor
    : public XERCES_CPP_NAMESPACE::ErrorHandler
{
public:
    
    /**
	 * Erzeugt einen Kindknoten eines DOM-Elements, das einen bestimmten Namen hat, 
	 * dieser ist wiederum ein Element-Knoten
	 *
     * @param doc Das DOM-Document
	 * @param parent Knoten, dessen Kind erzeugt wird
	 * @param name Name des Kindknotens von parent
	 *
	 * @return Der Kindknoten
	 */
    XERCES_CPP_NAMESPACE::DOMElement* appendChildElement(XERCES_CPP_NAMESPACE::DOMDocument* doc, 
        XERCES_CPP_NAMESPACE::DOMElement* parent, const char* const name) const;

    /**
	 * Ermittelt einen Kindknoten eines DOM-Elements, das einen bestimmten Namen hat, 
	 * dieser ist wiederum ein Element-Knoten
	 *
	 * @param parent Knoten, dessen Kinder gesucht sind
	 * @param name Name des gesuchten Kindknotens von parent
	 *
	 * @return Der Kindknoten
	 */
	XERCES_CPP_NAMESPACE::DOMElement* getChildNamed(XERCES_CPP_NAMESPACE::DOMElement* parent, const char* const name) const;
	
    /**
	 * Setzt den Text eines Elementknotens
	 *
	 * @param element the DOM element node
     * @param string Der zu setzende String
	 * @return the DOM element node
	 */
    void setValueAsString(XERCES_CPP_NAMESPACE::DOMDocument* doc, XERCES_CPP_NAMESPACE::DOMElement* element, const CeGuiString &value) const;
    
    /**
	 * Ermittelt den Text eines Elementknotens
	 *
	 * @param element the DOM element node
	 * @return Text innerhalb der Element-Tags, als CeGuiString
	 */
	CeGuiString getValueAsString(XERCES_CPP_NAMESPACE::DOMElement* element) const;
	
    /**
	 * Ermittelt den Text eines Elementknotens
	 *
	 * @param element the DOM element node
	 * @return Text innerhalb der Element-Tags, als CeGuiString
	 */
    std::string getValueAsStdString(XERCES_CPP_NAMESPACE::DOMElement* element) const;

    /**
	 * Setzt den Text eines Elementknotens, als UTF-8
	 * vor Benutzung muss initializeTranscoder() aufgerufen werden
	 *
	 * @param element the DOM element node
     * @param utf Der zu setzende UTF String
	 * @return the DOM element node
	 */
	void setValueAsUtf8(XERCES_CPP_NAMESPACE::DOMDocument* doc, XERCES_CPP_NAMESPACE::DOMElement* element, utf8* value) const;
    
    /**
	 * Ermittelt den Text eines Elementknotens, als UTF-8
	 * vor Benutzung muss initializeTranscoder() aufgerufen werden
	 *
	 * @param element the DOM element node
	 * @return Text innerhalb der Element-Tags, als utf8* (muss selbst gelöscht werden)
	 * @see initializeTranscoder()
	 */
	utf8* getValueAsUtf8(XERCES_CPP_NAMESPACE::DOMElement* element) const;

	
    /**
	 * Formt eine Boolean in einen String um und setzt diesen als Wert
	 *
	 * @param element the DOM element node
     * @param value Der zu setzende Bool
	 * @return the DOM element node
	 */
	void setValueAsBool(XERCES_CPP_NAMESPACE::DOMDocument* doc, XERCES_CPP_NAMESPACE::DOMElement* element, bool value) const;

    
    /**
	 * Get the text value of an element node and parse it into a bool
	 *
	 * @param element the DOM element node
	 * @return boolean within the element tags
	 */
	bool getValueAsBool(XERCES_CPP_NAMESPACE::DOMElement* element) const;
    
    
    /**
	 * Formt eine Ganzzahl in einen String um und setzt diesen als Wert
	 *
	 * @param element the DOM element node
     * @param value Der zu setzende Integer
	 * @return the DOM element node
	 */
	void setValueAsInteger(XERCES_CPP_NAMESPACE::DOMDocument* doc, XERCES_CPP_NAMESPACE::DOMElement* element, int value) const;

    
    /**
	 * Get the text value of an element node and parse it into an integer
	 *
	 * @param element the DOM element node
	 * @return number within the element tags
	 */
	int getValueAsInteger(XERCES_CPP_NAMESPACE::DOMElement* element) const;

    /**
	 * Get the text value of an element node and parse it into a real number
	 *
	 * @param element the DOM element node
	 * @return number within the element tags
	 */
    Ogre::Real getValueAsReal(XERCES_CPP_NAMESPACE::DOMElement* element) const;

    /**
	 * Convert an integer pair into a string and sets it as the text of a DOM element
	 *
	 * @param element the DOM element node
     * @param value the integer pair
	 * @return the DOM element node
	 */
    void setValueAsIntegerPair(XERCES_CPP_NAMESPACE::DOMDocument* doc, XERCES_CPP_NAMESPACE::DOMElement* element, IntPair value) const;

    /**
	 * Get the text value of an element node and parse it into an integer pair
	 *
	 * @param element the DOM element node
	 * @return pair within the element tags
	 */
    IntPair getValueAsIntegerPair(XERCES_CPP_NAMESPACE::DOMElement* element) const;

    /**
	 * Convert an integer triple into a string and set is as the text of a DOM element
	 *
	 * @param element the DOM element node
     * @param value the integer triple
	 * @return the DOM element node
	 */
    void setValueAsIntegerTriple(XERCES_CPP_NAMESPACE::DOMDocument* doc, XERCES_CPP_NAMESPACE::DOMElement* element, Tripel<int> value) const;

    /**
	 * Get the text value of an element node and parse it into an integer triple
	 *
	 * @param element the DOM element node
	 * @return triple within the element tags
	 */
    Tripel<int> getValueAsIntegerTriple(XERCES_CPP_NAMESPACE::DOMElement* element) const;
	
    /**
     * Convert a vector3 to an xml tag
	 *
	 * @param element The DOM element
     * @param value The vector input
	 * @return The DOM element
	 */
    void setValueAsVector3(XERCES_CPP_NAMESPACE::DOMElement* element, Ogre::Vector3 value) const;

    /**
	 * Processes an element node of shape <any_node_name x="0" y="0" z="0"/>
	 * into a three component Ogre vector
	 *
	 * @param element The DOM element
	 * @return the vector
	 */
	Ogre::Vector3 getValueAsVector3(XERCES_CPP_NAMESPACE::DOMElement* element) const;

    /**
     * Convert a quaternion to an xml tag
	 *
	 * @param element The DOM element
     * @param value The quaternion input
	 * @return The DOM element
	 */
    void setValueAsQuaternion(XERCES_CPP_NAMESPACE::DOMElement* element, Ogre::Quaternion value) const;

    /**
	 * Processes an element node of shape <any_node_name x="0" y="0" z="0" w="1"/>
	 * into a three component Ogre vector
	 *
	 * @param element The DOM element
	 * @return the quaternion
	 */
    Ogre::Quaternion getValueAsQuaternion(XERCES_CPP_NAMESPACE::DOMElement* element) const;

	
    /**
	 * Ermittelt, ob ein DOMElement ein bestimmtes Attribut bestitzt
	 *
	 * @param element the DOM element node
	 * @param name Name des Attributes
	 * @return <code>true</code>, wenn das Attribut an diesem Element existiert, sonst <code>false</code>
	 */
	bool hasAttribute(XERCES_CPP_NAMESPACE::DOMElement* element, const char* const name) const;

    /**
	 * Setzt den Wert von einem Attribut eines Elementes
	 *
	 * @param element the DOM element node
	 * @param name Name des Attributes
     * @param value Wert der gesetzt werden muss
	 * @return <code>true</code>, wenn das Attribut an diesem Element existiert, sonst <code>false</code>
	 */
    void setAttribute(XERCES_CPP_NAMESPACE::DOMElement* element, const char* const name, const char* const value) const;

    /**
	 * Checks whether a DOMNode has a certain name
	 *
	 * @param node the DOM node
	 * @param name the name to check
	 * @return <code>true</code>, if the DOM node the name <code>name</code>, <code>false</code> otherwise
	 */
	bool hasNodeName(XERCES_CPP_NAMESPACE::DOMNode* node, const char* const name) const;

    
    /**
	 * Wandelt eine Ganzzahl in einen Text um und setzt diesen als Attribut 
	 *
	 * @param element the DOM element node
	 * @param name Name des Attributes
     * @param value das zu setzende Integer
	 * @return the DOM element node
	 */
	void setAttributeValueAsInteger(XERCES_CPP_NAMESPACE::DOMElement* element, const char* const name, int value) const;

    /**
	 * Ermittelt den Text eines DOMElement-Attributes und parst ihn, 
	 * um eine Ganzzahl daraus zu machen
	 *
	 * @param element the DOM element node
	 * @param name Name des Attributes
     * @return Zahlenwert des Attributes
	 */
	int getAttributeValueAsInteger(XERCES_CPP_NAMESPACE::DOMElement* element, const char* const name) const;

    /**
	 * Converts a 64 bit integer (long long) into a string and sets it as the text of a DOM element attribute
	 *
	 * @param element the DOM element node
	 * @return number within the element tags
	 */
	void setAttributeValueAsInt64(XERCES_CPP_NAMESPACE::DOMElement* element, const char* const name, RL_LONGLONG value) const;

    /**
	 * Gets the text value of an element attribute and parse it into a 64 bit integer (long long) 
	 *
	 * @param element the DOM element node
	 * @return number within the element tags
	 */
	RL_LONGLONG getAttributeValueAsInt64(XERCES_CPP_NAMESPACE::DOMElement* element, const char* const name) const;

    /**
	 * Wandelt eine Ganzzahldubel in einen Text um und setzt diesen als Attribut 
	 *
	 * @param element the DOM element node
	 * @param name Name des Attributes
     * @param value das zu setzende IntegerPair
	 * @return the DOM element node
	 */
    void setAttributeValueAsIntegerPair(XERCES_CPP_NAMESPACE::DOMElement* element, const char* const name, IntPair value) const;

    /**
	 * Ermittelt den Text eines DOMElement-Attributes und parst ihn, 
	 * um eine Ganzzahldubel daraus zu machen
	 *
	 * @param element the DOM element node
	 * @param name Name des Attributes
     * @return Zahlendubel des Attributes
	 */
    IntPair getAttributeValueAsIntegerPair(XERCES_CPP_NAMESPACE::DOMElement* element, const char* const name) const;

    /**
	 * Wandelt eine Ganzzahltripel in einen Text um und setzt diesen als Attribut 
	 *
	 * @param element the DOM element node
	 * @param name Name des Attributes
     * @param value das zu setzende Tripel<int>
	 * @return the DOM element node
	 */
    void setAttributeValueAsIntegerTriple(XERCES_CPP_NAMESPACE::DOMElement* element, const char* const name, Tripel<int> value) const;

    /**
	 * Ermittelt den Text eines DOMElement-Attributes und parst ihn, 
	 * um eine Ganzzahltripel daraus zu machen
	 *
	 * @param element the DOM element node
	 * @param name Name des Attributes
     * @return Zahlentripel des Attributes
	 */
    Tripel<int> getAttributeValueAsIntegerTriple(XERCES_CPP_NAMESPACE::DOMElement* element, const char* const name) const;
	
	
    /**
	 * Wandelt einen Vektor in einen Text um und setzt diesen als Attribut 
	 *
	 * @param element the DOM element node
	 * @param name Name des Attributes
     * @param value das zu setzende Vector3
	 * @return the DOM element node
	 */
    void setAttributeValueAsVector3(XERCES_CPP_NAMESPACE::DOMElement* element, const char* const name, Ogre::Vector3 value) const;

    /**
	 * Ermittelt den Text eines DOMElement-Attributes und parst ihn, 
	 * um ein Vector3 daraus zu machen
	 *
	 * @param element the DOM element node
	 * @param name Name des Attributes
     * @return Vector3 des Attributes
	 */
    Ogre::Vector3 getAttributeValueAsVector3(XERCES_CPP_NAMESPACE::DOMElement* element, const char* const name) const;

    /**
	 * Wandelt eine Quaternion in einen Text um und setzt diesen als Attribut 
	 *
	 * @param element the DOM element node
	 * @param name Name des Attributes
     * @param value das zu setzende Quaternion
	 * @return the DOM element node
	 */
    void setAttributeValueAsQuaternion(XERCES_CPP_NAMESPACE::DOMElement* element, const char* const name, Ogre::Quaternion value) const;

    /**
	 * Ermittelt den Text eines DOMElement-Attributes und parst ihn, 
	 * um eine Quaternion daraus zu machen
	 *
	 * @param element the DOM element node
	 * @param name Name des Attributes
     * @return Quaternion des Attributes
	 */
    Ogre::Quaternion getAttributeValueAsQuaternion(XERCES_CPP_NAMESPACE::DOMElement* element, const char* const name) const;
    

    /**
	 * Setzt den Text eines DOMElement-Attributes
	 * 
	 * @param element the DOM element node
	 * @param name Name des Attributes
	 * @param value das zu setzende CeGuiString
	 * @return the DOM element node
	 */	
	void setAttributeValueAsString(XERCES_CPP_NAMESPACE::DOMElement* element, const char* const name, const CeGuiString &value) const;
    
    /**
	 * Ermittelt den Text eines DOMElement-Attributes und konvertiert ihn zu einem CeGuiString
	 * 
	 * @param element the DOM element node
	 * @param name Name des Attributes
	 * @return Konvertierter Text als CeGuiString
	 */	
	CeGuiString getAttributeValueAsString(XERCES_CPP_NAMESPACE::DOMElement* element, const char* const name) const;

    /**
	 * Setzt den Text eines DOMElement-Attributes
	 * 
	 * @param element the DOM element node
	 * @param name Name des Attributes
	 * @param value der zu setzende std::string
	 * @return the DOM element node
	 */	
    void setAttributeValueAsStdString(XERCES_CPP_NAMESPACE::DOMElement* element, const char* const name, const std::string &value) const;

	/**
	 * Ermittelt den Text eines DOMElement-Attributes und konvertiert ihn zu einem std::string
	 * 
	 * @param element the DOM element node
	 * @param name Name des Attributes
	 * @return Konvertierter Text als std::string
	 */	
    std::string getAttributeValueAsStdString(XERCES_CPP_NAMESPACE::DOMElement* element, const char* const name) const;
	
    /**
	 * Ermittelt den Text eines SAX Element-Attributes und konvertiert ihn zu einem CeGuiString
	 *
	 * @param element Liste aller Attribute des aktuellen Elementes
	 * @param name Name des Attributes
	 * @return Konvertierter Text als CeGuiString
	 */
	CeGuiString getAttributeValueAsString(const XERCES_CPP_NAMESPACE::Attributes& attributes, const char* const name) const;

    /**
	 * Setzt den Text eines DOMElement-Attributes
	 * 
	 * @param element the DOM element node
	 * @param name Name des Attributes
	 * @param value der zu setzende boolean
	 * @return the DOM element node
	 */	
	void setAttributeValueAsBool(XERCES_CPP_NAMESPACE::DOMElement* element, const char* const name, bool value) const;

	/**
	 * Ermittelt den Text eines DOMElement-Attributes und interpretier ihn als bool
	 * 
	 * @param element the DOM element node
	 * @param name Name des Attributes
	 * @return Konvertierter Text als bool
	 */	
	bool getAttributeValueAsBool(XERCES_CPP_NAMESPACE::DOMElement* element, const char* const name) const;

    /**
	 * Setzt den Text eines DOMElement-Attributes
	 * 
	 * @param element the DOM element node
	 * @param name Name des Attributes
	 * @param value der zu setzende Real Wert
	 * @return the DOM element node
	 */	
    void setAttributeValueAsReal(XERCES_CPP_NAMESPACE::DOMElement* element, const char* const name, Ogre::Real value) const;

	/**
	 * Ermittelt den Text eines DOMElement-Attributes und konvertiert ihn zu Ogre::Real
	 * 
	 * @param element the DOM element node
	 * @param name Name des Attributes
	 * @return Konvertierter Text als Ogre::Real
	 */	
	Ogre::Real getAttributeValueAsReal(XERCES_CPP_NAMESPACE::DOMElement* element, const char* const name) const;


	/**
	 * Initialisiert den XML<->UTF-8 Transcoder, 
	 * davor muss XMLPlatformUtils::Initialize() aufgerufen worden sein
	 */
	void initializeTranscoder();

	/**
	 * Konvertiert einen Xerces-XMLCh* in eine CeGuiString
	 * Intern wird eine Konvertierung in utf8* vorgenommen,
	 * wobei man sich hier um das nachträgliche aufräumen 
	 * nicht mehr zu kümmern braucht.
	 * 
	 * @param string16 Der zu konvertierende Xerces-XMLCh*
	 * @return Konvertierter Text als CeGuiString
	 * @see transcodeToUtf8(const XMLCh* const string16) const;
	 */
	CeGuiString transcodeToString(const XMLCh* const string16) const;

	std::string transcodeToStdString(const XMLCh* const string16) const;

    XERCES_CPP_NAMESPACE::DOMDocument* loadDocument(
        const Ogre::String& resourceName, const Ogre::String& resourceGroup = "");
    XERCES_CPP_NAMESPACE::DOMDocument* loadDocument(
        const Ogre::DataStreamPtr& stream);

    void initializeXml();
    void shutdownXml();

        virtual void warning(const XERCES_CPP_NAMESPACE::SAXParseException& exc);
        virtual void error(const XERCES_CPP_NAMESPACE::SAXParseException& exc);
        virtual void fatalError(const XERCES_CPP_NAMESPACE::SAXParseException& exc);
        virtual void resetErrors();

protected:
	static XMLTranscoder* sTranscoder;
	static XMLTransService::Codes sFailCode;

        std::string toString( const std::string& type,
                              const XERCES_CPP_NAMESPACE::SAXParseException& exc ) const;
        
        std::string mOpenXmlFileName;

	/**
	 * Konvertiert ein Xerces-XMLCh* in einen UTF-8-String
	 * Vor Benutzung muss initializeTranscoder() aufgerufen werden
	 *
	 * @param string16 Der zu konvertierende Xerces-XMLCh*
	 * @return Konvertierter Text als utf8* (muss selbst gelöscht werden)
	 * @see initializeTranscoder()
	 */
	utf8* transcodeToUtf8(const XMLCh* const string16) const;
};

class _RlCommonExport AutoXMLCh
{
public:
    AutoXMLCh() : mData(0) { }

    AutoXMLCh(const char* rhs) : mData(0)
    {
        mData = XERCES_CPP_NAMESPACE::XMLString::transcode(rhs);
    }

    AutoXMLCh(const AutoXMLCh& rhs) : mData(0)
    {
        mData = XERCES_CPP_NAMESPACE::XMLString::replicate(rhs.mData);
    }

    ~AutoXMLCh()
    {
        XERCES_CPP_NAMESPACE::XMLString::release(&mData);
    }

    const AutoXMLCh& operator=(const AutoXMLCh& rhs)
    {
        if (mData != NULL)
        {
            XERCES_CPP_NAMESPACE::XMLString::release(&mData);
        }
        mData = XERCES_CPP_NAMESPACE::XMLString::replicate(rhs.mData);
        return *this;
    }

    const AutoXMLCh& operator=(const char* rhs)
    {
        if (mData != NULL)
        {
            XERCES_CPP_NAMESPACE::XMLString::release(&mData);
        }
        mData = XERCES_CPP_NAMESPACE::XMLString::transcode(rhs);
        return *this;
    }

    XMLCh* data() const { return mData; }
private:
    XMLCh* mData;
};

class _RlCommonExport AutoChar
{
public:
    AutoChar() : mData(0) { }

    AutoChar(const XMLCh* str) : mData(0)
    {
        mData = XERCES_CPP_NAMESPACE::XMLString::transcode(str);
    }

    AutoChar(const AutoChar& rhs) : mData(0)
    {
        mData = XERCES_CPP_NAMESPACE::XMLString::replicate(rhs.mData);
    }

    ~AutoChar()
    {
        XERCES_CPP_NAMESPACE::XMLString::release(&mData);
    }

    const AutoChar& operator=(const AutoChar& rhs)
    {
        XERCES_CPP_NAMESPACE::XMLString::release(&mData);
        mData = XERCES_CPP_NAMESPACE::XMLString::replicate(rhs.mData);
        return *this;
    }

    const AutoChar& operator=(XMLCh* rhs)
    {
        if (mData != NULL)
        {
            XERCES_CPP_NAMESPACE::XMLString::release(&mData);
        }
        mData = XERCES_CPP_NAMESPACE::XMLString::transcode(rhs);
        return *this;
    }

    char* data() const { return mData; }
private:
    char* mData;
};

}

#endif
