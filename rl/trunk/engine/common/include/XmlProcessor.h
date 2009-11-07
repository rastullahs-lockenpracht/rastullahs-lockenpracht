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

#ifndef __XmlHelper_h__
#define __XmlHelper_h__

#include "CommonPrerequisites.h"

#include <tinyxml.h>

#include "Tripel.h"

namespace rl {

typedef std::vector<const TiXmlElement*> XmlElementList;

/**
 * This class offers helper methods for XML processing
 * To use this methods, just inherit this class privately 
 */
class _RlCommonExport XmlProcessor
{
public:
    XmlProcessor() {}

    /**
	 * Erzeugt einen Kindknoten eines DOM-Elements, das einen bestimmten Namen hat, 
	 * dieser ist wiederum ein Element-Knoten
	 *
	 * @param parent Knoten, dessen Kind erzeugt wird
	 * @param name Name des Kindknotens von parent
	 *
	 * @return Der Kindknoten
	 */
    TiXmlElement* appendChildElement(TiXmlElement* parent, const char* const name) const;

    /**
	 * Erzeugt einen Kindknoten des Roots eines DOM-Dokuments, das einen bestimmten Namen hat,
	 * dieser ist wiederum ein Element-Knoten
	 *
	 * @param parent Dokument
	 * @param name Name des Kindknotens von parent
	 *
	 * @return Der Kindknoten
	 */
    TiXmlElement* appendChildElement(TiXmlDocument* parent, const char* const name) const;

    /**
	 * Ermittelt einen Kindknoten eines DOM-Elements, das einen bestimmten Namen hat, 
	 * dieser ist wiederum ein Element-Knoten
	 *
	 * @param parent Knoten, dessen Kinder gesucht sind
	 * @param name Name des gesuchten Kindknotens von parent
	 *
	 * @return Der Kindknoten
	 */
    const TiXmlElement* getChildNamed(const TiXmlElement* parent, const char* const name) const;

    /**
	 * Ermittelt einen Kindknoten eines DOM-Elements, das einen bestimmten Namen hat,
	 * dieser ist wiederum ein Element-Knoten
	 *
	 * @param parent Knoten, dessen Kinder gesucht sind
	 * @param name Name des gesuchten Kindknotens von parent
	 *
	 * @return Der Kindknoten
	 */
    TiXmlElement* getChildNamed(TiXmlElement* parent, const char* const name) const;

    /**
     * Collects all element nodes with a specified tag name
	 *
	 * @param parent start node
	 * @param name node name to search
	 *
	 * @return vector of found element nodes
     */
    XmlElementList getElementsByTagName(const TiXmlElement* parent, const char* const name) const;
	
    /**
     * Collects all element nodes with a specified tag name
	 *
	 * @param parent start node
	 * @param name node name to search
	 *
	 * @return vector of found element nodes
     */
    XmlElementList getElementsByTagName(const TiXmlDocument* parent, const char* const name) const;

    /**
	 * Setzt den Text eines Elementknotens
	 *
	 * @param element the DOM element node
     * @param string Der zu setzende String
	 * @return the DOM element node
	 */
    void setValueAsString(TiXmlElement* element, const CeGuiString &value) const;
    
    /**
	 * Ermittelt den Text eines Elementknotens
	 *
	 * @param element the DOM element node
	 * @return Text innerhalb der Element-Tags, als CeGuiString
	 */
	CeGuiString getValueAsString(const TiXmlElement* element) const;
	
    /**
	 * Ermittelt den Text eines Elementknotens
	 *
	 * @param element the DOM element node
	 * @return Text innerhalb der Element-Tags, als CeGuiString
	 */
    std::string getValueAsStdString(const TiXmlElement* element) const;

    /**
	 * Setzt den Text eines Elementknotens, als UTF-8
	 * vor Benutzung muss initializeTranscoder() aufgerufen werden
	 *
	 * @param element the DOM element node
     * @param utf Der zu setzende UTF String
	 * @return the DOM element node
	 */
	void setValueAsUtf8(TiXmlElement* element, CEGUI::utf8* value) const;
    
    /**
	 * Ermittelt den Text eines Elementknotens, als UTF-8
	 * vor Benutzung muss initializeTranscoder() aufgerufen werden
	 *
	 * @param element the DOM element node
	 * @return Text innerhalb der Element-Tags, als utf8* (muss selbst gel�scht werden)
	 * @see initializeTranscoder()
	 */
	const CEGUI::utf8* getValueAsUtf8(const TiXmlElement* element) const;

	
    /**
	 * Formt eine Boolean in einen String um und setzt diesen als Wert
	 *
	 * @param element the DOM element node
     * @param value Der zu setzende Bool
	 * @return the DOM element node
	 */
	void setValueAsBool(TiXmlElement* element, bool value) const;

    
    /**
	 * Get the text value of an element node and parse it into a bool
	 *
	 * @param element the DOM element node
	 * @return boolean within the element tags
	 */
	bool getValueAsBool(const TiXmlElement* element) const;
    
    
    /**
	 * Formt eine Ganzzahl in einen String um und setzt diesen als Wert
	 *
	 * @param element the DOM element node
     * @param value Der zu setzende Integer
	 * @return the DOM element node
	 */
	void setValueAsInteger(TiXmlElement* element, int value) const;

    
    /**
	 * Get the text value of an element node and parse it into an integer
	 *
	 * @param element the DOM element node
	 * @return number within the element tags
	 */
	int getValueAsInteger(const TiXmlElement* element) const;

    /**
	 * Get the text value of an element node and parse it into a real number
	 *
	 * @param element the DOM element node
	 * @return number within the element tags
	 */
    Ogre::Real getValueAsReal(const TiXmlElement* element) const;

    /**
	 * Convert an integer pair into a string and sets it as the text of a DOM element
	 *
	 * @param element the DOM element node
     * @param value the integer pair
	 * @return the DOM element node
	 */
    void setValueAsIntegerPair(TiXmlElement* element, IntPair value) const;

    /**
	 * Get the text value of an element node and parse it into an integer pair
	 *
	 * @param element the DOM element node
	 * @return pair within the element tags
	 */
    IntPair getValueAsIntegerPair(const TiXmlElement* element) const;

    /**
	 * Convert an integer triple into a string and set is as the text of a DOM element
	 *
	 * @param element the DOM element node
     * @param value the integer triple
	 * @return the DOM element node
	 */
    void setValueAsIntegerTriple(TiXmlElement* element, Tripel<int> value) const;

    /**
	 * Get the text value of an element node and parse it into an integer triple
	 *
	 * @param element the DOM element node
	 * @return triple within the element tags
	 */
    Tripel<int> getValueAsIntegerTriple(const TiXmlElement* element) const;
	
    /**
     * Convert a vector3 to an xml tag
	 *
	 * @param element The DOM element
     * @param value The vector input
	 * @return The DOM element
	 */
    void setValueAsVector3(TiXmlElement* element, Ogre::Vector3 value) const;

    /**
	 * Processes an element node of shape <any_node_name x="0" y="0" z="0"/>
	 * into a three component Ogre vector
	 *
	 * @param element The DOM element
	 * @return the vector
	 */
	Ogre::Vector3 getValueAsVector3(const TiXmlElement* element) const;

    /**
     * Convert a quaternion to an xml tag
	 *
	 * @param element The DOM element
     * @param value The quaternion input
	 * @return The DOM element
	 */
    void setValueAsQuaternion(TiXmlElement* element, Ogre::Quaternion value) const;

    /**
	 * Processes an element node of shape <any_node_name x="0" y="0" z="0" w="1"/>
	 * into a three component Ogre vector
	 *
	 * @param element The DOM element
	 * @return the quaternion
	 */
    Ogre::Quaternion getValueAsQuaternion(const TiXmlElement* element) const;

	
    /**
	 * Ermittelt, ob ein DOMElement ein bestimmtes Attribut bestitzt
	 *
	 * @param element the DOM element node
	 * @param name Name des Attributes
	 * @return <code>true</code>, wenn das Attribut an diesem Element existiert, sonst <code>false</code>
	 */
	bool hasAttribute(const TiXmlElement* element, const char* const name) const;

    /**
	 * Setzt den Wert von einem Attribut eines Elementes
	 *
	 * @param element the DOM element node
	 * @param name Name des Attributes
     * @param value Wert der gesetzt werden muss
	 * @return <code>true</code>, wenn das Attribut an diesem Element existiert, sonst <code>false</code>
	 */
    void setAttribute(TiXmlElement* element, const char* const name, const char* const value) const;

    /**
	 * Checks whether a DOMNode has a certain name
	 *
	 * @param node the DOM node
	 * @param name the name to check
	 * @return <code>true</code>, if the DOM node the name <code>name</code>, <code>false</code> otherwise
	 */
	bool hasNodeName(const TiXmlNode* node, const char* const name) const;

    
    /**
	 * Wandelt eine Ganzzahl in einen Text um und setzt diesen als Attribut 
	 *
	 * @param element the DOM element node
	 * @param name Name des Attributes
     * @param value das zu setzende Integer
	 * @return the DOM element node
	 */
	void setAttributeValueAsInteger(TiXmlElement* element, const char* const name, int value) const;

    /**
	 * Ermittelt den Text eines DOMElement-Attributes und parst ihn, 
	 * um eine Ganzzahl daraus zu machen
	 *
	 * @param element the DOM element node
	 * @param name Name des Attributes
     * @return Zahlenwert des Attributes
	 */
	int getAttributeValueAsInteger(const TiXmlElement* element, const char* const name) const;

    /**
	 * Converts a 64 bit integer (long long) into a string and sets it as the text of a DOM element attribute
	 *
	 * @param element the DOM element node
	 * @return number within the element tags
	 */
	void setAttributeValueAsInt64(TiXmlElement* element, const char* const name, RL_LONGLONG value) const;

    /**
	 * Gets the text value of an element attribute and parse it into a 64 bit integer (long long) 
	 *
	 * @param element the DOM element node
	 * @return number within the element tags
	 */
	RL_LONGLONG getAttributeValueAsInt64(const TiXmlElement* element, const char* const name) const;

    /**
	 * Wandelt eine Ganzzahldubel in einen Text um und setzt diesen als Attribut 
	 *
	 * @param element the DOM element node
	 * @param name Name des Attributes
     * @param value das zu setzende IntegerPair
	 * @return the DOM element node
	 */
    void setAttributeValueAsIntegerPair(TiXmlElement* element, const char* const name, IntPair value) const;

    /**
	 * Ermittelt den Text eines DOMElement-Attributes und parst ihn, 
	 * um eine Ganzzahldubel daraus zu machen
	 *
	 * @param element the DOM element node
	 * @param name Name des Attributes
     * @return Zahlendubel des Attributes
	 */
    IntPair getAttributeValueAsIntegerPair(const TiXmlElement* element, const char* const name) const;

    /**
	 * Wandelt eine Ganzzahltripel in einen Text um und setzt diesen als Attribut 
	 *
	 * @param element the DOM element node
	 * @param name Name des Attributes
     * @param value das zu setzende Tripel<int>
	 * @return the DOM element node
	 */
    void setAttributeValueAsIntegerTriple(TiXmlElement* element, const char* const name, Tripel<int> value) const;

    /**
	 * Ermittelt den Text eines DOMElement-Attributes und parst ihn, 
	 * um eine Ganzzahltripel daraus zu machen
	 *
	 * @param element the DOM element node
	 * @param name Name des Attributes
     * @return Zahlentripel des Attributes
	 */
    Tripel<int> getAttributeValueAsIntegerTriple(const TiXmlElement* element, const char* const name) const;
	
	
    /**
	 * Wandelt einen Vektor in einen Text um und setzt diesen als Attribut 
	 *
	 * @param element the DOM element node
	 * @param name Name des Attributes
     * @param value das zu setzende Vector3
	 * @return the DOM element node
	 */
    void setAttributeValueAsVector3(TiXmlElement* element, const char* const name, Ogre::Vector3 value) const;

    /**
	 * Ermittelt den Text eines DOMElement-Attributes und parst ihn, 
	 * um ein Vector3 daraus zu machen
	 *
	 * @param element the DOM element node
	 * @param name Name des Attributes
     * @return Vector3 des Attributes
	 */
    Ogre::Vector3 getAttributeValueAsVector3(const TiXmlElement* element, const char* const name) const;

    /**
	 * Wandelt eine Quaternion in einen Text um und setzt diesen als Attribut 
	 *
	 * @param element the DOM element node
	 * @param name Name des Attributes
     * @param value das zu setzende Quaternion
	 * @return the DOM element node
	 */
    void setAttributeValueAsQuaternion(TiXmlElement* element, const char* const name, Ogre::Quaternion value) const;

    /**
	 * Ermittelt den Text eines DOMElement-Attributes und parst ihn, 
	 * um eine Quaternion daraus zu machen
	 *
	 * @param element the DOM element node
	 * @param name Name des Attributes
     * @return Quaternion des Attributes
	 */
    Ogre::Quaternion getAttributeValueAsQuaternion(const TiXmlElement* element, const char* const name) const;
    

    /**
	 * Setzt den Text eines DOMElement-Attributes
	 * 
	 * @param element the DOM element node
	 * @param name Name des Attributes
	 * @param value das zu setzende CeGuiString
	 * @return the DOM element node
	 */	
	void setAttributeValueAsString(TiXmlElement* element, const char* const name, const CeGuiString &value) const;
    
    /**
	 * Ermittelt den Text eines DOMElement-Attributes und konvertiert ihn zu einem CeGuiString
	 * 
	 * @param element the DOM element node
	 * @param name Name des Attributes
	 * @return Konvertierter Text als CeGuiString
	 */	
	CeGuiString getAttributeValueAsString(const TiXmlElement* element, const char* const name) const;

    /**
	 * Setzt den Text eines DOMElement-Attributes
	 * 
	 * @param element the DOM element node
	 * @param name Name des Attributes
	 * @param value der zu setzende std::string
	 * @return the DOM element node
	 */	
    void setAttributeValueAsStdString(TiXmlElement* element, const char* const name, const std::string &value) const;

	/**
	 * Ermittelt den Text eines DOMElement-Attributes und konvertiert ihn zu einem std::string
	 * 
	 * @param element the DOM element node
	 * @param name Name des Attributes
	 * @return Konvertierter Text als std::string
	 */	
    std::string getAttributeValueAsStdString(const TiXmlElement* element, const char* const name) const;
	
    /**
	 * Setzt den Text eines DOMElement-Attributes
	 * 
	 * @param element the DOM element node
	 * @param name Name des Attributes
	 * @param value der zu setzende boolean
	 * @return the DOM element node
	 */	
	void setAttributeValueAsBool(TiXmlElement* element, const char* const name, bool value) const;

	/**
	 * Ermittelt den Text eines DOMElement-Attributes und interpretier ihn als bool
	 * 
	 * @param element the DOM element node
	 * @param name Name des Attributes
	 * @return Konvertierter Text als bool
	 */	
	bool getAttributeValueAsBool(const TiXmlElement* element, const char* const name) const;

    /**
	 * Setzt den Text eines DOMElement-Attributes
	 * 
	 * @param element the DOM element node
	 * @param name Name des Attributes
	 * @param value der zu setzende Real Wert
	 * @return the DOM element node
	 */	
    void setAttributeValueAsReal(TiXmlElement* element, const char* const name, Ogre::Real value) const;

	/**
	 * Ermittelt den Text eines DOMElement-Attributes und konvertiert ihn zu Ogre::Real
	 * 
	 * @param element the DOM element node
	 * @param name Name des Attributes
	 * @return Konvertierter Text als Ogre::Real
	 */	
	Ogre::Real getAttributeValueAsReal(const TiXmlElement* element, const char* const name) const;


	TiXmlDocument* loadDocument(
        const Ogre::String& resourceName, const Ogre::String& resourceGroup = "");
	TiXmlDocument* loadDocument(
        const Ogre::DataStreamPtr& stream);

protected:
    std::string mOpenXmlFileName;

private:
    TiXmlDocument* parseToXmlDocument(const char* content) const;

};

}

#endif
