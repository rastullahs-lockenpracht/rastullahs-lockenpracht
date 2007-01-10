//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file LaderXerces.cpp
//@+at
// LaderXerces.cpp: Implementierung der 
// ILader-Schnittstelle mit Apache Xerces
// 
// $Revision: 1.11 $
// $Date: 2004/10/17 19:12:38 $
//@-at
//@@c

//@<< header >>
//@+node:<< header >>
//@+at
// Copyright (c) 2002-2004, Zdenko Denny Vrandecic and 
// contributors.
// All rights reserved.
// (The license follows closely the new 
// BSD-OpenSource-License)
// 
// Redistribution and use in source and binary forms, with 
// or without
// modification, are permitted provided that the following 
// conditions are met:
// Redistributions of source code must retain the above 
// copyright notice, this list of conditions and the 
// following disclaimer.
// Redistributions in binary form must reproduce the above 
// copyright notice, this list of conditions and the 
// following disclaimer in the documentation
// and/or other materials provided with the distribution.
// Neither the name of the copyrightholder or one of the 
// contributers to this source may be used to endorse or 
// promote products derived from this software without 
// specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
// CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED 
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
// PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS 
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
// (INCLUDING, BUT  NOT LIMITED TO, PROCUREMENT OF 
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND 
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// If you use this source, please inform me, so I can keep 
// track of how useful it really is. That would be very 
// appreciated.
// 
// The data distributed with this code is copyrighted by 
// Fantasy Production, Erkrath, Germany, and *not* subject 
// to the here given license. Distributing and using this 
// data is granted by FanPro for the purpose of using 
// computer driven tools for the role playing game "Das 
// Schwarze Auge" / "The Dark Eye". Any other use is only 
// allowed with prior written permission by FanPro.
//@-at
//@@c

#ifdef MSVC
#pragma warning(disable:4786 4503)
#endif

//@-node:<< header >>
//@nl

//@<< includes >>
//@+node:<< includes >>
#include "LaderXerces.h"

#include "NDSAUtil.h"

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp> 
#include <xercesc/framework/MemBufFormatTarget.hpp> 

#include <fstream>

#include "Log.h"
//#include "dsa/Texte.h"
//@-node:<< includes >>
//@nl

//@<< filename >>
//@+node:<< filename >>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

//@-node:<< filename >>
//@nl

#define X(str) xercesc::XMLString::transcode(str.c_str())

//@+others
//@+node:LaderXerces
//////////////////////////////////////////////////////////////////////
// Konstruktoren / Destruktoren
//////////////////////////////////////////////////////////////////////

LaderXerces::LaderXerces() : doc(0)
	{
		try
			{
        xercesc::XMLPlatformUtils::Initialize();
			}

    catch (const xercesc::XMLException& toCatch)
			{
				Log::Error("Fehler w�hrend der XML-Initalisierung");
				//Log::Add(StrX(toCatch.getMessage()), Log::Error);
        return;
			}

		XMLCh tempStr[100];
		xercesc::XMLString::transcode("LS", tempStr, 99);
		xercesc::DOMImplementation *impl = 
			xercesc::DOMImplementationRegistry::getDOMImplementation(tempStr);
		parser = ((xercesc::DOMImplementationLS*)impl)->
			           createDOMBuilder(xercesc::DOMImplementationLS::MODE_SYNCHRONOUS, 0);
		parser->setFeature(xercesc::XMLString::transcode("whitespace-in-element-content"), false);

    doc = impl->createDocument(0, X(std::string("XDIML")), 0);
		Wurzel()->setAttribute(X(std::string("version")), X(std::string("1.01")));
		Wurzel()->setAttribute(X(std::string("lang")), X(std::string("de")));

		m_Fehler = "kein Fehler gemeldet";
		Log::Info("Lader initialisiert");
	}
//@nonl
//@-node:LaderXerces
//@+node:LaderXerces
//@+at 
//@nonl
// Konstruktor
//@-at
//@@c

LaderXerces::~LaderXerces()
	{
		parser->release();
		Log::Info("Lader destruiert");
	}
//@nonl
//@-node:LaderXerces
//@+node:LaderVersion
//@+at 
//@nonl
// Destruktor
//@-at
//@@c

const std::string LaderXerces::LaderVersion() const {
  return "Xerces " XERCES_FULLVERSIONDOT ;
}
//@nonl
//@-node:LaderVersion
//@+node:Lade
//@+at 
//@nonl
// LaderXerces
// 
// Methoden zur Dateibearbeitung
//@-at
//@@c

int LaderXerces::Lade (std::string DateiName)
	{
		Log::Debug("Lader::Lade");
		Log::Debug(DateiName);
    if (doc != 0) { delete doc; doc = 0; } // Eine Datei ist schon geladen! Was nun?
		m_DateiName = DateiName;
    doc = parser->parseURI(m_DateiName.c_str());

		// TODO Hier m�ssen Fehlerbehandlung
		// und anderes rein!
		Log::Debug("Lader::Lade beendet");
		
		return 0;
	}
//@nonl
//@-node:Lade
//@+node:NeuesDokument
//@+at 
//@nonl
// Lade
//@-at
//@@c

int LaderXerces::NeuesDokument (std::string DateiName, const std::string& Wurzelname)
	{
		if (doc != 0) { delete doc; doc = 0; } // Eine Datei ist schon geladen! Was nun?
		m_DateiName = DateiName;
    xercesc::DOMImplementation* impl =  xercesc::DOMImplementationRegistry::getDOMImplementation(X(std::string("Core")));

    doc = impl->createDocument(0, X(Wurzelname), 0);
    if (Wurzelname=="XDIML") {
      Wurzel()->setAttribute(X(std::string("version")), X(std::string("1.01")));
		  Wurzel()->setAttribute(X(std::string("lang")), X(std::string("de")));
    }

		// TODO Hier m�ssen Fehlerbehandlung
		// und anderes rein!
		
		return 0;
	}
//@nonl
//@-node:NeuesDokument
//@+node:Speicher
//@+at 
//@nonl
// Lade
//@-at
//@@c

int LaderXerces::Speicher (std::string DateiName, const unsigned int maximaleDateigroesse)
	{
		Log::Debug("Lader::Speicher");
		if (DateiName=="") DateiName = Dateiname();
		Log::Debug(DateiName);

		XMLCh tempStr[100];
		xercesc::XMLString::transcode("LS", tempStr, 99);
		xercesc::DOMImplementation *impl = xercesc::DOMImplementationRegistry::getDOMImplementation(tempStr);
		xercesc::DOMWriter* theSerializer = ((xercesc::DOMImplementationLS*)impl)->createDOMWriter();

		if (theSerializer->canSetFeature(xercesc::XMLUni::fgDOMWRTDiscardDefaultContent, true))
			theSerializer->setFeature(xercesc::XMLUni::fgDOMWRTDiscardDefaultContent, true);
		if (theSerializer->canSetFeature(xercesc::XMLUni::fgDOMWRTFormatPrettyPrint, true))
			theSerializer->setFeature(xercesc::XMLUni::fgDOMWRTFormatPrettyPrint, true);
		
		xercesc::XMLFormatTarget* myFormTarget = new xercesc::LocalFileFormatTarget(DateiName.c_str());
    //xercesc::MemBufFormatTarget* myFormTarget = new xercesc::MemBufFormatTarget(8388607);
		theSerializer->writeNode(myFormTarget, *doc);
    //std::ofstream ziel(DateiName.c_str());
    //ziel.write(reinterpret_cast<const char*>(myFormTarget->getRawBuffer()), myFormTarget->getLen());

		theSerializer->release();

		delete myFormTarget;

		return 0;
	}
//@nonl
//@-node:Speicher
//@+node:Dateiname
//@+at 
//@nonl
// Speicher
//@-at
//@@c

std::string LaderXerces::Dateiname()
	{
		return m_DateiName;
	}
//@nonl
//@-node:Dateiname
//@+node:LetzterFehler
//@+at 
//@nonl
// DateiName
//@-at
//@@c

std::string LaderXerces::LetzterFehler()
	{
		return m_Fehler;
	}
//@nonl
//@-node:LetzterFehler
//@+node:MeldeFehler
//@+at 
//@nonl
// LetzterFehler
//@-at
//@@c

void LaderXerces::MeldeFehler(std::string f)
	{
		m_Fehler = f;
		Log::Error(f);
	}
//@nonl
//@-node:MeldeFehler
//@+node:SchreibePI
//@+at 
//@nonl
// MeldeFehler
//@-at
//@@c

int LaderXerces::SchreibePI(const std::string& target, const std::string& data) {
  xercesc::DOMProcessingInstruction* pi = doc->createProcessingInstruction(X(target), X(data));
  doc->insertBefore(pi, Wurzel());
  return 0;
}
//@nonl
//@-node:SchreibePI
//@+node:IDListe
//@+at 
//@nonl
// SchreibePI
// 
// Methoden zur Datenbearbeitung
//@-at
//@@c

int LaderXerces::IDListe (const StringListe& Pfad,
													 StringListe* Liste)
	{
		// TODO es fehlt die Fehlerbehandlung
		Log::Debug("Lader::IDListe");

		Liste->clear();
		StringListe PfadZuVater(Pfad);
		std::string gesuchtesElement = PfadZuVater.back();
		PfadZuVater.pop_back();
		if (PfadZuVater.back() == "ID") return 1;
		xercesc::DOMElement* Vater;
		int rv = gehePfad(PfadZuVater, &Vater);
		if (rv) return rv;

		xercesc::DOMNodeList* kinder = Vater->getChildNodes();
		int anzahlKinder = kinder->getLength();
		if (anzahlKinder==0) return 1;
		for (int j=0; j<anzahlKinder; j++)
			if (kinder->item(j)->getNodeType() == xercesc::DOMNode::ELEMENT_NODE)
				if (x2s(kinder->item(j)->getNodeName())==(gesuchtesElement))
					{
						xercesc::DOMNamedNodeMap* attribs = kinder->item(j)->getAttributes();
						xercesc::DOMNode* idNode = attribs->getNamedItem(xercesc::XMLString::transcode("ID"));
						Liste->push_back(x2s(idNode->getNodeValue()));
					}
				
		//Log::Debug("Lader::IDListe beendet");
		return 0;
	}
//@nonl
//@-node:IDListe
//@+node:IDListe
//@+at 
//@nonl
// IDListe
//@-at
//@@c

int LaderXerces::IDListe(const xercesc::DOMElement* const knoten,
													const std::string& tag,
													StringListe* liste) const {
	liste->clear();	
	std::string gesuchtesElement = tag;
	const xercesc::DOMElement* const Vater = knoten;

	xercesc::DOMNodeList* kinder = Vater->getChildNodes();
	int anzahlKinder = kinder->getLength();
	if (anzahlKinder==0) return 0;
	for (int j=0; j<anzahlKinder; j++)
		if (kinder->item(j)->getNodeType() == xercesc::DOMNode::ELEMENT_NODE)
			if (x2s(kinder->item(j)->getNodeName())==(gesuchtesElement))
				{
					xercesc::DOMNamedNodeMap* attribs = kinder->item(j)->getAttributes();
					xercesc::DOMNode* idNode = attribs->getNamedItem(xercesc::XMLString::transcode("ID"));
     if (idNode == 0) continue;
					liste->push_back(x2s(idNode->getNodeValue()));
				}
			
	return 0; // TODO Fehlerbehandlung
}
//@nonl
//@-node:IDListe
//@+node:LeseString
//@+at 
//@nonl
// IDListe
//@-at
//@@c

int LaderXerces::LeseString (const StringListe& Pfad,
															std::string* Inhalt)
	{
		Log::Error("Lader::LeseString nicht implementiert");
		xercesc::DOMElement* node;
		int rv = gehePfad(Pfad, &node);
		if (rv) return rv;
		// Inhalt->assign(x2s(node->getNodeValue())); // Falsch!
		Log::Info("Lader::LeseString nicht implementiert");
		return 0;
	}
//@nonl
//@-node:LeseString
//@+node:LeseString
//@+at 
//@nonl
// LeseString
//@-at
//@@c

int LaderXerces::LeseString (xercesc::DOMElement* const knoten, std::string* wert) {
	if (!knoten->hasChildNodes()) return 1;
	xercesc::DOMNodeList* list = knoten->getChildNodes();
	for (int i = list->getLength(); i != 0; --i) {
		xercesc::DOMNode* text = list->item(i-1);
		if (text->getNodeType() == xercesc::DOMNode::TEXT_NODE) {
			*wert = x2s(text->getNodeValue());
			return 0;
		}
	}
	return 1; // TODO Fehlerbehandlung
}
//@nonl
//@-node:LeseString
//@+node:LeseInteger
//@+at 
//@nonl
// LeseString
//@-at
//@@c

int LaderXerces::LeseInteger (const StringListe& Pfad,
															 int* Inhalt)
	{
		Log::Error("Lader::LeseString nicht implementiert");
		return 0;
	}
//@nonl
//@-node:LeseInteger
//@+node:LeseInteger
//@+at 
//@nonl
// LeseInteger
//@-at
//@@c

int LaderXerces::LeseInteger (xercesc::DOMElement* const knoten, int* wert) {
	if (!knoten->hasChildNodes()) return 1;
	xercesc::DOMNodeList* list = knoten->getChildNodes();
	for (int i = list->getLength(); i != 0; --i) {
		xercesc::DOMNode* text = list->item(i-1);
		if (text->getNodeType() == xercesc::DOMNode::TEXT_NODE) {
			*wert = strings::sStr2int(x2s(text->getNodeValue()));
			return 0;
		}
	}
	return 1; // TODO Fehlerbehandlung
}
//@nonl
//@-node:LeseInteger
//@+node:SchreibeString
//@+at 
//@nonl
// LeseString
//@-at
//@@c

int LaderXerces::SchreibeString (const StringListe& Pfad,
																	std::string Inhalt)
	{
		Log::Error("Lader::SchreibeString nicht implementiert");
		return 0;
	}
//@nonl
//@-node:SchreibeString
//@+node:SchreibeString
//@+at 
//@nonl
// SchreibeString
//@-at
//@@c

int LaderXerces::SchreibeString (xercesc::DOMElement* const knoten, const std::string& Inhalt)
	{
	  xercesc::DOMText* text = doc->createTextNode(X(Inhalt));
	  knoten->appendChild(text);
		return 0;
	}
//@nonl
//@-node:SchreibeString
//@+node:SchreibeInteger
//@+at 
//@nonl
// SchreibeString
//@-at
//@@c

int LaderXerces::SchreibeInteger (const StringListe& Pfad,
																	 int Inhalt)
	{
		Log::Error("Lader::SchreibeString nicht implementiert");
		return 0;
	}
//@nonl
//@-node:SchreibeInteger
//@+node:SchreibeInteger
//@+at 
//@nonl
// SchreibeString
//@-at
//@@c

int LaderXerces::SchreibeInteger (xercesc::DOMElement* const knoten,
																	 int Inhalt)
	{
	  xercesc::DOMText* text = doc->createTextNode(X(strings::int2sStr(Inhalt)));
	  knoten->appendChild(text);
		return 0;
	}
//@nonl
//@-node:SchreibeInteger
//@+node:NeuerKnoten
//@+at 
//@nonl
// SchreibeInteger
//@-at
//@@c

int LaderXerces::NeuerKnoten (const StringListe& Pfad,
															 std::string Bezeichner,
															 bool Ueberschreiben,
															 std::string ID)
	{
		Log::Error("Lader::NeuerKnoten nicht implementiert");
		return 0;
	}
//@nonl
//@-node:NeuerKnoten
//@+node:NeuerKnoten
//@+at 
//@nonl
// NeuerKnoten
//@-at
//@@c

int LaderXerces::NeuerKnoten(xercesc::DOMElement* const knoten, const std::string& Bezeichner, const std::string& ID, bool Ueberschreiben)
	{
	  int rv = 0;
	  if (ExistiertKnoten(knoten, Bezeichner, ID))
			if (Ueberschreiben)
				rv += LoescheKnoten(knoten, Bezeichner, ID);
			else
				return rv;

		xercesc::DOMElement* neuerKnoten = doc->createElement(X(Bezeichner));
		if (ID!="") neuerKnoten->setAttribute(X(std::string("ID")), X(ID));
		knoten->appendChild(neuerKnoten);
				
	  return rv;
	}
//@nonl
//@-node:NeuerKnoten
//@+node:NeuerKnoten
//@+at 
//@nonl
// NeuerKnoten
//@-at
//@@c

int LaderXerces::NeuerKnoten(xercesc::DOMElement* const knoten, const std::string& Bezeichner, int nr, bool Ueberschreiben)
	{
	  int rv = 0;
	  if (ExistiertKnoten(knoten, Bezeichner, nr))
			if (Ueberschreiben)
      {} //rv += LoescheKnoten(knoten, Bezeichner, nr);
			else
				return rv;

		xercesc::DOMElement* neuerKnoten = doc->createElement(X(Bezeichner));
		//if (ID!="") neuerKnoten->setAttribute(X(_t("ID")), X(ID));
		knoten->appendChild(neuerKnoten);
				
	  return rv;
	}
//@nonl
//@-node:NeuerKnoten
//@+node:ExistiertKnoten
//@+at 
//@nonl
// NeuerKnoten
//@-at
//@@c

bool LaderXerces::ExistiertKnoten (const StringListe& Pfad)
	{
		//Log::Debug("Lader::ExistiertKnoten");
		xercesc::DOMElement* node;
		int rv = gehePfad(Pfad, &node);
		if (rv) return false;
		if (node==0) return false;
		return true;
	}
//@nonl
//@-node:ExistiertKnoten
//@+node:ExistiertKnoten
//@+at 
//@nonl
// ExistiertKnoten
//@-at
//@@c

bool LaderXerces::ExistiertKnoten (const xercesc::DOMElement* const knoten, const std::string& Bezeichner, const std::string& ID)
	{
		//Log::Debug("Lader::ExistiertKnoten");
		xercesc::DOMElement* node;
		StringListe pfad;
		pfad.push_back(Bezeichner);
		if (ID!="") { pfad.push_back("ID"); pfad.push_back(ID); }
		int rv = gehePfad(pfad, &node, 0, knoten);
		if (rv) return false;
		if (node==0) return false;
		return true;
	}
//@nonl
//@-node:ExistiertKnoten
//@+node:ExistiertKnoten
//@+at 
//@nonl
// ExistiertKnoten
//@-at
//@@c

bool LaderXerces::ExistiertKnoten (const xercesc::DOMElement* const knoten, const std::string& Bezeichner, int nr)
	{
		//Log::Debug("Lader::ExistiertKnoten");
		xercesc::DOMElement* node;
		StringListe pfad;
		pfad.push_back(Bezeichner);
		int rv = gehePfad(pfad, &node, nr, knoten);
		if (rv) return false;
		if (node==0) return false;
		return true;
	}
//@nonl
//@-node:ExistiertKnoten
//@+node:LoescheKnoten
//@+at 
//@nonl
// ExistiertKnoten
//@-at
//@@c

int LaderXerces::LoescheKnoten (const StringListe& Pfad)
	{
		Log::Error("Lader::LoescheKnoten nicht implementiert");
		return 0;
	}
//@nonl
//@-node:LoescheKnoten
//@+node:LoescheKnoten
//@+at 
//@nonl
// LoescheKnoten
//@-at
//@@c

int LaderXerces::LoescheKnoten (xercesc::DOMElement* const knoten, const std::string& Bezeichner, const std::string& ID)
	{
	  xercesc::DOMElement* kind;
		StringListe pfad;
		pfad.push_back(Bezeichner);
		if (ID!="") { pfad.push_back("ID"); pfad.push_back(ID); }
		int rv = gehePfad(pfad, &kind, 0, knoten);

		/*delete*/ knoten->removeChild(kind); // TODO M�glicherweise Speicherleck?

		return rv;
	}
//@nonl
//@-node:LoescheKnoten
//@+node:AnzahlKnoten
//@+at 
//@nonl
// LoescheKnoten
//@-at
//@@c

int LaderXerces::AnzahlKnoten (const StringListe& Pfad)
	{
		// TODO es fehlt die Fehlerbehandlung
		Log::Debug("Lader::AnzahlKnoten");
		int anzahl = 0;
		std::string ID = "";
		StringListe PfadZuVater(Pfad);
		std::string gesuchtesElement = PfadZuVater.back();
		PfadZuVater.pop_back();
		if (PfadZuVater.back() == "ID")
			{
				PfadZuVater.pop_back();
				ID = gesuchtesElement;
				gesuchtesElement = PfadZuVater.back();
				PfadZuVater.pop_back();
			}
		xercesc::DOMElement* Vater;
		int rv = gehePfad(PfadZuVater, &Vater);
		if (rv) return 0;

		xercesc::DOMNodeList* kinder = Vater->getChildNodes();
		int anzahlKinder = kinder->getLength();
		if (anzahlKinder==0) return 0;
		for (int j=0; j<anzahlKinder; j++)
			if (kinder->item(j)->getNodeType() == xercesc::DOMNode::ELEMENT_NODE)
				if (x2s(kinder->item(j)->getNodeName())==(gesuchtesElement))
					{
						if (ID=="")
							anzahl++;
						else
							{
								xercesc::DOMNamedNodeMap* attribs = kinder->item(j)->getAttributes();
								xercesc::DOMNode* idNode = attribs->getNamedItem(xercesc::XMLString::transcode("ID"));
								if((x2s(idNode->getNodeValue()))==ID)
									anzahl++;
							}
					}
				
		Log::Debug("Lader::AnzahlKnoten beendet");
		return anzahl;
	}
//@nonl
//@-node:AnzahlKnoten
//@+node:AnzahlKnoten
//@+at 
//@nonl
// AnzahlKnoten
//@-at
//@@c

int LaderXerces::AnzahlKnoten (xercesc::DOMElement* const knoten, const std::string& Bezeichner, const std::string& ID) {
		// TODO es fehlt die Fehlerbehandlung
		int anzahl = 0;
		std::string id = ID;
		std::string gesuchtesElement = Bezeichner;
		xercesc::DOMElement* Vater = knoten;

		xercesc::DOMNodeList* kinder = Vater->getChildNodes();
		int anzahlKinder = kinder->getLength();
		if (anzahlKinder==0) return 0;
		for (int j=0; j<anzahlKinder; j++)
			if (kinder->item(j)->getNodeType() == xercesc::DOMNode::ELEMENT_NODE)
				if (x2s(kinder->item(j)->getNodeName())==(gesuchtesElement))
					{
						if (id=="")
							anzahl++;
						else
							{
								xercesc::DOMNamedNodeMap* attribs = kinder->item(j)->getAttributes();
								xercesc::DOMNode* idNode = attribs->getNamedItem(xercesc::XMLString::transcode("ID"));
								if((x2s(idNode->getNodeValue()))==ID)
									anzahl++;
							}
					}
				
		return anzahl;
	}
//@nonl
//@-node:AnzahlKnoten
//@+node:AttributiereKnoten
//@+at 
//@nonl
// AnzahlKnoten
//@-at
//@@c

int LaderXerces::AttributiereKnoten(xercesc::DOMElement* const knoten,
                                    const std::string& attribut,
                                    const std::string& attributswert) {
  knoten->setAttribute(X(attribut), X(attributswert));
  return 0;
}
//@nonl
//@-node:AttributiereKnoten
//@+node:AttributiereKnoten
//@+at 
//@nonl
// AttributiereKnoten
//@-at
//@@c

int LaderXerces::AttributiereKnoten (const StringListe& Pfad,
																			std::string Attribut,
																			std::string Attributswert)
	{
		Log::Error("Lader::AttributiereKnoten nicht implementiert");
		return 0;
	}
//@nonl
//@-node:AttributiereKnoten
//@+node:LeseAttribut
//@+at 
//@nonl
// AttributiereKnoten
//@-at
//@@c

int LaderXerces::LeseAttribut (const StringListe& Pfad,
																std::string Attribut,
																std::string* Attributswert)
	{
		//Log::Debug("Lader::LeseAttribut");
		xercesc::DOMElement* node;
		int rv = gehePfad(Pfad, &node);
		if (rv) return 1;
		xercesc::DOMNamedNodeMap* attribs = node->getAttributes();
		xercesc::DOMNode* attNode = attribs->getNamedItem(X(Attribut));
		Attributswert->assign(x2s(attNode->getNodeValue()));
		//Log::Debug("Lader::LeseAttribut " + _t("beendet"));
		return 0;
	}
//@nonl
//@-node:LeseAttribut
//@+node:LeseAttribut
//@+at 
//@nonl
// LeseAttribute
//@-at
//@@c

int LaderXerces::LeseAttribut (const xercesc::DOMElement* const knoten,
																const std::string& Attribut,
																std::string* Attributswert) {
		xercesc::DOMNamedNodeMap* attribs = knoten->getAttributes();
		xercesc::DOMNode* attNode = attribs->getNamedItem(X(Attribut));
		if (attNode==0) { MeldeFehler("Attribut " + Attribut + " existiert nicht."); return 1; }
		Attributswert->assign(x2s(attNode->getNodeValue()));
		//Log::Debug("Lader::LeseAttribut beendet");
		return 0;
	}
//@-node:LeseAttribut
//@+node:ExistiertAttribut

bool LaderXerces::ExistiertAttribut (const StringListe& Pfad,
																			std::string Attribut)
	{
		//Log::Debug("Lader::ExistiertAttribut");
		xercesc::DOMElement* node;
		int rv = gehePfad(Pfad, &node);
		if (rv) return false;
		xercesc::DOMNamedNodeMap* attribs = node->getAttributes();
		xercesc::DOMNode* attNode = attribs->getNamedItem(X(Attribut));
		return (attNode != 0);
	}
//@nonl
//@-node:ExistiertAttribut
//@+node:ExistiertAttribut
//@+at 
//@nonl
// ExistiertAttribut
//@-at
//@@c

bool LaderXerces::ExistiertAttribut(const xercesc::DOMElement* const knoten,
																		 const std::string& Attribut) {
		xercesc::DOMNamedNodeMap* attribs = knoten->getAttributes();
    if (attribs==0) return false;
		xercesc::DOMNode* attNode = attribs->getNamedItem(X(Attribut));
		return (attNode != 0);
	}
//@-node:ExistiertAttribut
//@+node:LeseString
//@+at 
//@nonl
// Methoden zur Schnittstellenvereinfachung
//@-at
//@@c

std::string LaderXerces::LeseString (const StringListe& Pfad)
	{
		std::string s;
		LeseString(Pfad, &s);
		return s;
	}
//@nonl
//@-node:LeseString
//@+node:LeseInteger
//@+at 
//@nonl
// LeseString
//@-at
//@@c

int LaderXerces::LeseInteger (const StringListe& Pfad)
	{
	  int i;
		LeseInteger(Pfad, &i);
		return i;
	}
//@nonl
//@-node:LeseInteger
//@+node:ExistiertKnoten
//@+at 
//@nonl
// LeseInteger
//@-at
//@@c

bool LaderXerces::ExistiertKnoten (const StringListe& Pfad,
																		std::string Bezeichner,
																		std::string ID)
	{
		return ExistiertKnoten(NeuerPfad(Pfad, Bezeichner, ID));
	}
//@nonl
//@-node:ExistiertKnoten
//@+node:LoescheKnoten
//@+at 
//@nonl
// ExistiertKnoten
//@-at
//@@c

int LaderXerces::LoescheKnoten (const StringListe& Pfad,
																 std::string Bezeichner,
																 std::string ID)
	{
		return LoescheKnoten(NeuerPfad(Pfad, Bezeichner, ID));
	}
//@nonl
//@-node:LoescheKnoten
//@+node:AnzahlKnoten
//@+at 
//@nonl
// LoescheKnoten
//@-at
//@@c

int LaderXerces::AnzahlKnoten (const StringListe& Pfad,
																std::string Bezeichner,
																std::string ID)
	{
		return AnzahlKnoten(NeuerPfad(Pfad, Bezeichner, ID));
	}
//@nonl
//@-node:AnzahlKnoten
//@+node:AttributiereKnoten
//@+at 
//@nonl
// AnzahlKnoten
//@-at
//@@c

int LaderXerces::AttributiereKnoten (const StringListe& Pfad,
																			std::string Attribut,
																			int Attributswert)
	{
		return AttributiereKnoten(Pfad,	Attribut, strings::int2sStr(Attributswert));
	}
//@nonl
//@-node:AttributiereKnoten
//@+node:AttributiereKnoten
//@+at 
//@nonl
// AttributiereKnoten
//@-at
//@@c

int LaderXerces::AttributiereKnoten (const StringListe& Pfad,
																			std::string Bezeichner,
																			std::string ID,
																			std::string Attribut,
																			std::string Attributswert)
	{
		return AttributiereKnoten(NeuerPfad(Pfad, Bezeichner, ID),
			                        Attribut, Attributswert);
	}
//@nonl
//@-node:AttributiereKnoten
//@+node:AttributiereKnoten
//@+at 
//@nonl
// AttributiereKnoten
//@-at
//@@c

int LaderXerces::AttributiereKnoten (const StringListe& Pfad,
																			std::string Bezeichner,
																			std::string ID,
																			std::string Attribut,
																			int Attributswert)
	{
		return AttributiereKnoten(NeuerPfad(Pfad, Bezeichner, ID),
			                        Attribut, strings::int2sStr(Attributswert));
	}
//@nonl
//@-node:AttributiereKnoten
//@+node:LeseAttribut
//@+at 
//@nonl
// AttributiereKnoten
//@-at
//@@c

int LaderXerces::LeseAttribut (const StringListe& Pfad,
																std::string Bezeichner,
																std::string ID,
																std::string Attribut,
																int* Attributswert)
	{
		return  LeseAttribut(NeuerPfad(Pfad, Bezeichner, ID),
			                   Attribut, Attributswert);
	}
//@nonl
//@-node:LeseAttribut
//@+node:LeseAttribut
//@+at 
//@nonl
// LeseAttribut
//@-at
//@@c

int LaderXerces::LeseAttribut (const StringListe& Pfad,
																std::string Bezeichner,
																std::string ID,
																std::string Attribut,
																std::string* Attributswert)
	{
		return LeseAttribut(NeuerPfad(Pfad, Bezeichner, ID),
			                  Attribut, Attributswert);
	}
//@nonl
//@-node:LeseAttribut
//@+node:LeseAttribut
//@+at 
//@nonl
// LeseAttribut
//@-at
//@@c

int LaderXerces::LeseAttribut (const StringListe& Pfad,
																std::string Attribut,
																int* Attributswert)
	{
		std::string s;
		int rc =  LeseAttribut(Pfad, Attribut, &s);
		*Attributswert = strings::sStr2int(s);
		return rc;
	}
//@nonl
//@-node:LeseAttribut
//@+node:LeseAttribut
//@+at 
//@nonl
// LeseAttribut
//@-at
//@@c

int LaderXerces::LeseAttribut (const xercesc::DOMElement* const knoten,
																const std::string& Attribut,
																int* Attributswert) {
		std::string s;
		int rc =  LeseAttribut(knoten, Attribut, &s);
		*Attributswert = strings::sStr2int(s);
		return rc;
	}
//@-node:LeseAttribut
//@+node:ExistiertAttribut

bool LaderXerces::ExistiertAttribut (const StringListe& Pfad,
																			std::string Bezeichner,
																			std::string ID,
																			std::string Attribut)
	{
		return ExistiertAttribut(NeuerPfad(Pfad, Bezeichner, ID), Attribut);
	}
//@nonl
//@-node:ExistiertAttribut
//@+node:NeuerPfad
//@+at 
//@nonl
// ExistiertAttribut
// 
// Hilfsmethoden
//@-at
//@@c

StringListe LaderXerces::NeuerPfad(const StringListe& alterPfad,
																		std::string Bezeichner,
																		std::string ID)
	{
		StringListe pfad = StringListe(alterPfad);	
		pfad.push_back(Bezeichner);
		if (ID=="") return pfad;
		pfad.push_back("ID");
		pfad.push_back(ID);
		return pfad;
	}
//@nonl
//@-node:NeuerPfad
//@+node:Wurzel
//@+at 
//@nonl
// NeuerPfad
//@-at
//@@c

xercesc::DOMElement* LaderXerces::Wurzel()
	{
		return doc->getDocumentElement();
	}
//@nonl
//@-node:Wurzel
//@+node:gehePfad
//@+at 
//@nonl
// Wurzel
// Schlechter Ersatz f�r XPath...
//@-at
//@@c

int LaderXerces::gehePfad(const StringListe& Pfad, xercesc::DOMElement** node, int Nummer, const xercesc::DOMElement* const start)
	{
		StringListe::const_iterator i = Pfad.begin();
		if (start!=0)
			*node = const_cast<xercesc::DOMElement*>(start);
		else
			{
				*node = Wurzel();
				if (x2s((*node)->getNodeName()) != (*i))
					return 2; // Wurzel entspricht nicht dem ersten Element der Liste
				i++;
			}
		xercesc::DOMNode* knoten;
		knoten = *node;
		while (i!=Pfad.end())
			{
				std::string gesucht = (*i);
				std::string ID = "";
				i++;
				if (i!=Pfad.end())
					if ((*i)=="ID")
						{
							i++;
							ID = (*i);
							i++;
						}
				xercesc::DOMNodeList* kinder = knoten->getChildNodes();
				int anzahlKinder = kinder->getLength();
				if (anzahlKinder==0) return 2;
				for (int j=0; j<anzahlKinder; j++)
					if (kinder->item(j)->getNodeType() == xercesc::DOMNode::ELEMENT_NODE)
						if (x2s(kinder->item(j)->getNodeName())==(gesucht))
							{
								if (ID=="")
									{
										knoten = kinder->item(j);
                    if (Nummer--==0) goto gefunden;
									}
								else
									{
										// vergleiche ID
										xercesc::DOMNamedNodeMap* attribs = kinder->item(j)->getAttributes();
										xercesc::DOMNode* idNode = attribs->getNamedItem(xercesc::XMLString::transcode("ID"));
                    if (idNode!=0)
  										  if (x2s(idNode->getNodeValue())==ID)
  											  {
												  knoten = kinder->item(j);
												  if (Nummer--==0) goto gefunden;
											  }
									}
							}
			}
		return 2;

    gefunden:
		*node = static_cast<xercesc::DOMElement*>(knoten); // TODO safe?
		return 0;				
	}
//@-node:gehePfad
//@+node:VaterKnoten
//@+at 
//@nonl
// gehePfad
//@-at
//@@c

xercesc::DOMElement* LaderXerces::VaterKnoten(const xercesc::DOMElement* const kind) const {
	return static_cast<xercesc::DOMElement*>(kind->getParentNode()); // TODO safe?
	}
//@-node:VaterKnoten
//@+node:x2s

std::string LaderXerces::x2s(const XMLCh* const xmlch) const
	{
		std::string sStr;
		sStr.assign(xercesc::XMLString::transcode(xmlch));
		return sStr;
	}
//@nonl
//@-node:x2s
//@+node:MacheXPath
//@+at 
//@nonl
// x2s
//@-at
//@@c

std::string LaderXerces::MacheXPath(const StringListe& Pfad)
	{
		std::string XPath = "";
		StringListe::const_iterator i = Pfad.begin();

		while (i != Pfad.end())
			{
				if ((*i)=="ID")
					{
						i++;
						if (i==Pfad.end())
							return XPath;
						XPath += "[@ID = \"" + (*i) + "\"]";
					}
				else
					{
						if (i!=Pfad.begin())
							XPath += "/" + (*i);
						else
							XPath = (*i);
					}
				i++;
			}

		return XPath;
	}
//@nonl
//@-node:MacheXPath
//@-others

//@<< Version History >>
//@+node:<< Version History >>
// $Log: LaderXerces.cpp,v $
// Revision 1.11  2004/10/17 19:12:38  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.10  2004/04/04 21:06:41  vrandezo
// Bug: wurde nach einem Knoten mit ID gesucht, und kam zuvor ein
// Knoten ohne ID, fuehrte das zu einem Absturz
//
// Revision 1.9  2004/04/04 17:29:03  vrandezo
// Re-Generierungs-bug gefixt
//
// Revision 1.8  2004/01/06 03:52:49  vrandezo
// detConfig f�r Applikationskonfigurationsdaten hinzugef�gt
//
// Revision 1.7  2003/11/28 16:45:03  vrandezo
// Zeilenvorlauf am Dateiende (Warnung bei gcc)
//
// Revision 1.6  2003/10/24 13:26:02  vrandezo
// DEBUG_NEW raus
//
// Revision 1.5  2003/10/19 20:22:16  vrandezo
// Eingecheckt in Leo
//
// Revision 1.4  2003/03/13 18:15:37  vrandezo
// Bug: Wenn man den XML- Export ausw�hlt und dann abbricht st�rzt das
// Programm ab (King Lui)
// Bug: Klickt man bei der Heldenerschaffung im Raster Talente
// auf den oberen Bereich (z.B. auf TaW), so wird das
// Programm sofort beendet (King Lui)
//
// Revision 1.3  2003/03/08 16:25:13  vrandezo
// BE bei Talenten und Kampftechniken (Gerhard Mayer), mit GUI
// M�gliche Spezialisierungen von Talenten
//
// Revision 1.2  2003/03/03 21:39:05  vrandezo
// Testdruck m�glich
// XML-Export, der mit Gerhard Mayers XSL arbeitet
//
// Revision 1.1.1.1  2003/02/17 15:32:15  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.18  2003/02/14 11:14:58  vrandezo
// Bug: im LaderXerces st�rzte Existiert(tag, id) mit angegebener id ab,
// wenn keine id vorhanden war
//
// Revision 1.17  2003/02/13 22:29:30  vrandezo
// Auf Xerces 2.2.0 geupdatet
//
// Revision 1.16  2003/02/13 11:44:46  vrandezo
// Bug entfernt: Sprachen und Schriften bei Personen bleiben erhalten
// Bug entfernt: Kulturen-Persistenz korrigiert (L/S Sprachen)
// Person-Vergleich korrigiert
// Dateibegrenzung auf 4MB erh�ht
// XML ge�ndert: Sprachen jetzt mit "Sprachen kennen"-ID bei RKP
//
// Revision 1.15  2003/01/17 01:35:11  vrandezo
// Speichern stark verk�rzt, von der Dauer her
// Erinnert beim Beenden des Programms an das Speichern ver�nderter Daten
//
// Revision 1.14  2003/01/13 00:30:49  vrandezo
// Release Version 1.12 dev
// Korrektur der Verrechnungspunkteberechnung f�r negative Talentmodifikatoren
// Talentauswahl implementiert
//
// Revision 1.13  2002/11/03 14:25:33  vrandezo
// Wuerfel angepasst
// Haar- und Augenfarbe hinzugef�gt bei Rassen
//
// Revision 1.12  2002/11/02 23:36:57  vrandezo
// Exportier und Importierfunktionen implementiert
// Vergleichsoperationen implementiert
// Kampftechnikfenstter implementiert
//
// Revision 1.11  2002/11/02 15:39:48  vrandezo
// Importfenster hinzugef�gt
//
// Revision 1.10  2002/11/01 15:27:29  vrandezo
// Regeln neu implementiert
// ILader, MLader, IMLader, ILadbar und weitere Klassen entfernt
// XMLChar-Transcoding beschleunigt
//
// Revision 1.9  2002/10/27 17:29:20  vrandezo
// Die meisten Teile sind speicherbar
//
// Revision 1.8  2002/10/25 23:06:33  vrandezo
// Speichern erweitert
//
// Revision 1.7  2002/10/25 14:21:12  vrandezo
// Hauptmen� erweitert
// Charakteristika hinzugef�gt
// eigenes Mengen-GUI-Element erzeugt
//
// Revision 1.6  2002/10/19 20:35:32  vrandezo
// feste Stringliterale entfernt
//
// Revision 1.5  2002/10/17 15:35:58  vrandezo
// TalentGUI hinzugef�gt
//
// Revision 1.4  2002/09/25 23:50:51  vrandezo
// Beginn der Aufteilung in DSA.lib und Anwendung
//
// Revision 1.3  2002/08/30 23:28:24  vrandezo
// Die alte Log-Klasse kompatibel zu wxWindows' Log gemacht
//
// Revision 1.2  2002/08/30 15:06:17  vrandezo
// Apache Xerces ersetzt den MSXML
// Zahlreiche Speicherlecks entfernt
// Entwurf verst�rkt
//
// Revision 1.1  2002/08/09 11:33:55  vrandezo
// Der Xerces Parser hinzugef�gt. Lesen kann er schon.
//
//@nonl
//@-node:<< Version History >>
//@nl
//@-node:@file LaderXerces.cpp
//@-leo
