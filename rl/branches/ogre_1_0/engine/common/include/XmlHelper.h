/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */

#ifndef __XmlHelper_h__
#define __XmlHelper_h__

#include "CommonPrerequisites.h"

#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/util/XMLChar.hpp>
#include <xercesc/util/TransService.hpp>

// Da sollten wir uns auf etwas enigen
// So ist das laestig.
#if OGRE_PLATFORM == PLATFORM_WIN32
#   include <CEGUIString.h>
#else
#   include <CEGUI/CEGUIString.h>
#endif
using XERCES_CPP_NAMESPACE::XMLTranscoder;
using XERCES_CPP_NAMESPACE::XMLTransService;
using CEGUI::utf8;

namespace rl {

/**
 * Diese Klasse bietet Helferfunktionen, um XML einfacher verarbeiten zu können
 * Alle Methoden sind statisch, es muss also keine Instanz dieser Klasse gebildet werden
 */
class _RlCommonExport XmlHelper
{
public:

	/**
	 * Ermittelt einen Kindknoten eines DOM-Elements, das einen bestimmten Namen hat, 
	 * dieser ist wiederum ein Element-Knoten
	 *
	 * @param parent Knoten, dessen Kinder gesucht sind
	 * @param name Name des gesuchten Kindknotens von parent
	 *
	 * @return Der Kindknoten
	 */
	static XERCES_CPP_NAMESPACE::DOMElement* getChildNamed(XERCES_CPP_NAMESPACE::DOMElement* parent, const char* name);
	
	/**
	 * Ermittelt den Text eines Elementknotens
	 *
	 * @param element Das DOM-Element
	 * @return Text innerhalb der Element-Tags, als char* (muss selbst gelöscht werden, z.B. mittels XMLString::release)
	 */
	static char* getValueAsString(XERCES_CPP_NAMESPACE::DOMElement* element);
	
	/**
	 * Ermittelt den Text eines Elementknotens, als UTF-8
	 * vor Benutzung muss initializeTranscoder() aufgerufen werden
	 *
	 * @param element Das DOM-Element
	 * @return Text innerhalb der Element-Tags, als utf8* (muss selbst gelöscht werden, z.B. mittels XMLString::release)
	 * @see initializeTranscoder()
	 */
	static utf8* getValueAsUtf8(XERCES_CPP_NAMESPACE::DOMElement* element);

	/**
	 * Ermittelt den Text eines Elementknotens und parst ihn, um eine Ganzzahl daraus zu machen
	 *
	 * @param element Das DOM-Element
	 * @return Zahl, die innerhalb der Element-Tags steht
	 */
	static int getValueAsInteger(XERCES_CPP_NAMESPACE::DOMElement* element);

	/**
	 * Initialisiert den XML<->UTF-8 Transcoder, 
	 * davor muss XMLPlatformUtils::Initialize() aufgerufen worden sein
	 */
	static void initializeTranscoder();

	/**
	 * Konvertiert ein Xerces-XMLCh* in einen UTF-8-String
	 *
	 * davor muss initializeTranscoder() aufgerufen worden sein
	 */
	static utf8* transcodeToUtf8(const XMLCh* const string16);

private:
	static XMLTranscoder* sTranscoder;
	static XMLTransService::Codes sFailCode;

};

}

#endif
