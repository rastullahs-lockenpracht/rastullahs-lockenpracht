//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file LaderXerces.h
//@+at
// LaderXerces.h: Implementiert die ILader-Schnittstelle 
// mit Xerces
// 
// $Revision: 1.9 $
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

#if !defined(LADERXERCES_H)
#define LADERXERCES_H

//@<< includes >>
//@+node:<< includes >>
#include "xercesc/dom/DOM.hpp"
#include "NDSABasis.h"
//@nonl
//@-node:<< includes >>
//@nl

//@<< class LaderXerces >>
//@+node:<< class LaderXerces >>
class LaderXerces
	{
		public:
			LaderXerces();
			~LaderXerces();

      const std::string LaderVersion() const;

      int NeuesDokument (std::string Dateiname = "", const std::string& Wurzelname = "XDIML");

			int Lade (std::string DateiName="");
			int Speicher (std::string DateiName="", const unsigned int maximaleDateiGroesse = 4194303);

			int IDListe (const StringListe& Pfad, StringListe* Liste);
      int IDListe(const xercesc::DOMElement* const knoten, const std::string& tag, StringListe* liste) const;
		
			int LeseString (const StringListe& Pfad, std::string* Inhalt);
			int LeseString (xercesc::DOMElement* const knoten, std::string* wert);
			std::string LeseString (const StringListe& Pfad);
			int LeseInteger (const StringListe& Pfad,	int* Inhalt);
			int LeseInteger (xercesc::DOMElement* const knoten, int* wert);
			int LeseInteger (const StringListe& Pfad);

			int SchreibeString (const StringListe& Pfad, std::string Inhalt);
			int SchreibeString (xercesc::DOMElement* const knoten, const std::string& Inhalt);
			int SchreibeInteger (const StringListe& Pfad,	int Inhalt);
			int SchreibeInteger (xercesc::DOMElement* const knoten, int Inhalt);

			int NeuerKnoten (const StringListe& Pfad,	std::string Bezeichner,	bool Ueberschreiben, std::string ID = "");
			int NeuerKnoten (xercesc::DOMElement* const knoten, const std::string& Bezeichner, const std::string& ID = "", bool Ueberschreiben = false);
      int NeuerKnoten(xercesc::DOMElement* const knoten, const std::string& Bezeichner, int nr, bool Ueberschreiben = false);

      int SchreibePI (const std::string& target, const std::string& data = "");

			bool ExistiertKnoten (const StringListe& Pfad, std::string Bezeichner, std::string ID = "");
			bool ExistiertKnoten (const StringListe& Pfad);
			bool ExistiertKnoten (const xercesc::DOMElement* const knoten, const std::string& Bezeichner, const std::string& ID = "");
      bool ExistiertKnoten (const xercesc::DOMElement* const knoten, const std::string& Bezeichner, int nr);

			int LoescheKnoten (const StringListe& Pfad,	std::string Bezeichner,	std::string ID = "");
			int LoescheKnoten (const StringListe& Pfad);
			int LoescheKnoten (xercesc::DOMElement* const knoten, const std::string& Bezeichner, const std::string& ID = "");

			int AnzahlKnoten (const StringListe& Pfad, std::string Bezeichner, std::string ID = "");
			int AnzahlKnoten (const StringListe& Pfad);
      int AnzahlKnoten (xercesc::DOMElement* const knoten, const std::string& Bezeichner, const std::string& ID = "");

			int AttributiereKnoten (const StringListe& Pfad, std::string Attribut, std::string Attributswert);
			int AttributiereKnoten (const StringListe& Pfad, std::string Attribut, int Attributswert);
			int AttributiereKnoten (const StringListe& Pfad, std::string Bezeichner, std::string ID, std::string Attribut, std::string Attributswert);
			int AttributiereKnoten (const StringListe& Pfad, std::string Bezeichner, std::string ID, std::string Attribut, int Attributswert);
      int AttributiereKnoten(xercesc::DOMElement* const knoten,
                             const std::string& attribut,
                             const std::string& attributswert);

			int LeseAttribut (const xercesc::DOMElement* const knoten, const std::string& Attribut, std::string* Attributswert);
			int LeseAttribut (const StringListe& Pfad, std::string Attribut, std::string* Attributswert);
			int LeseAttribut (const xercesc::DOMElement* const knoten, const std::string& Attribut, int* Attributswert);
			int LeseAttribut (const StringListe& Pfad, std::string Attribut, int* Attributswert);
			int LeseAttribut (const StringListe& Quelle, std::string Bezeichner, std::string ID, std::string Attribut, std::string* Attributswert);
			int LeseAttribut (const StringListe& Quelle, std::string Bezeichner, std::string ID, std::string Attribut, int* Attributswert);	
			
			bool ExistiertAttribut (const StringListe& Pfad, std::string Attribut);
			bool ExistiertAttribut (const xercesc::DOMElement* const knoten, const std::string& Attribut);
			bool ExistiertAttribut (const StringListe& Quelle, std::string Bezeichner, std::string ID, std::string Attribut);

			std::string Dateiname();

			std::string LetzterFehler();

			xercesc::DOMElement* Wurzel();
			xercesc::DOMElement* VaterKnoten(const xercesc::DOMElement* const) const;
			int gehePfad(const StringListe& Pfad, xercesc::DOMElement** node, int Nummer = 0, const xercesc::DOMElement* const start = 0);

      bool DocLeer() const { return doc == 0; }

		private:
			std::string MacheXPath(const StringListe& Pfad);
			StringListe NeuerPfad(const StringListe& alterPfad, std::string Bezeichner, std::string ID="");
			std::string x2s(const XMLCh* const xmlch) const;
			void MeldeFehler(std::string f);

			std::string m_DateiName;
			std::string m_Fehler;
			xercesc::DOMDocument* doc;
			xercesc::DOMBuilder* parser;
	}; // Class LaderXerces
//@nonl
//@-node:<< class LaderXerces >>
//@nl

#endif // !defined(LADERXERCES_H)

//@<< Version History >>
//@+node:<< Version History >>
// $Log: LaderXerces.h,v $
// Revision 1.9  2004/10/17 19:12:38  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.8  2004/04/04 17:29:04  vrandezo
// Re-Generierungs-bug gefixt
//
// Revision 1.7  2004/01/06 03:52:49  vrandezo
// detConfig f�r Applikationskonfigurationsdaten hinzugef�gt
//
// Revision 1.6  2003/12/04 17:10:50  vrandezo
// Trons BSD-Portierung
//
// Revision 1.5  2003/11/28 16:45:03  vrandezo
// Zeilenvorlauf am Dateiende (Warnung bei gcc)
//
// Revision 1.4  2003/11/17 01:33:49  vrandezo
// Xerces 2.3.0
//
// Revision 1.3  2003/10/19 20:22:16  vrandezo
// Eingecheckt in Leo
//
// Revision 1.2  2003/03/03 21:39:05  vrandezo
// Testdruck m�glich
// XML-Export, der mit Gerhard Mayers XSL arbeitet
//
// Revision 1.1.1.1  2003/02/17 15:32:18  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.10  2003/02/13 22:29:30  vrandezo
// Auf Xerces 2.2.0 geupdatet
//
// Revision 1.9  2003/02/13 11:44:46  vrandezo
// Bug entfernt: Sprachen und Schriften bei Personen bleiben erhalten
// Bug entfernt: Kulturen-Persistenz korrigiert (L/S Sprachen)
// Person-Vergleich korrigiert
// Dateibegrenzung auf 4MB erh�ht
// XML ge�ndert: Sprachen jetzt mit "Sprachen kennen"-ID bei RKP
//
// Revision 1.8  2003/01/13 00:30:49  vrandezo
// Release Version 1.12 dev
// Korrektur der Verrechnungspunkteberechnung f�r negative Talentmodifikatoren
// Talentauswahl implementiert
//
// Revision 1.7  2002/11/02 23:36:57  vrandezo
// Exportier und Importierfunktionen implementiert
// Vergleichsoperationen implementiert
// Kampftechnikfenstter implementiert
//
// Revision 1.6  2002/11/02 15:39:48  vrandezo
// Importfenster hinzugef�gt
//
// Revision 1.5  2002/11/01 15:27:29  vrandezo
// Regeln neu implementiert
// ILader, MLader, IMLader, ILadbar und weitere Klassen entfernt
// XMLChar-Transcoding beschleunigt
//
// Revision 1.4  2002/10/27 17:29:20  vrandezo
// Die meisten Teile sind speicherbar
//
// Revision 1.3  2002/10/25 23:06:33  vrandezo
// Speichern erweitert
//
// Revision 1.2  2002/09/25 23:50:51  vrandezo
// Beginn der Aufteilung in DSA.lib und Anwendung
//
// Revision 1.1  2002/08/09 11:33:56  vrandezo
// Der Xerces Parser hinzugef�gt. Lesen kann er schon.
//
//@nonl
//@-node:<< Version History >>
//@nl
//@-node:@file LaderXerces.h
//@-leo
