//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file Texte.h
//@+at
// Texte.h: Schnittstelle für die Textliterale
// 
// $Revision: 1.5 $
// $Date: 2004/10/17 19:12:40 $
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

#if !defined(TEXTE_H)
#define TEXTE_H

//@<< includes >>
//@+node:<< includes >>
#include <string>
#include <memory>
#include <map>

#include "NDSABasis.h"
//@nonl
//@-node:<< includes >>
//@nl

// irgendwo wurde IN definiert, dass stört hier
#undef IN

class wxTextFile;

namespace DSA {
  std::string Lang();
  std::string StandardLang();
}

//@<< struct Texte >>
//@+node:<< struct Texte >>
//! hält alle String-Literale des Programms
/*! Dadurch ist später eine einfache Übersetzung in andere Sprachen möglich.
/*/
struct Texte  
	{
			static void Initialisiere(const std::string& datei = "texte.xml");
//			static void Deinitialisiere();

			static std::string Text(const std::string&);

		private:
			//! Versteckter Konstruktor
			Texte(const std::string& datei);
			//! Destruktor
			virtual ~Texte();

			DDict lexikon_;

			static Texte* instanz_;
	}; // Texte
//@nonl
//@-node:<< struct Texte >>
//@nl

// Hier kann man wählen. Wählt man das erste, so wird die gesamte Arbeit über die
// Texte-Klasse durchgeführt, was teilweise das Programm recht verlangsamen kann.
// Wählte man die zweite Zeile, dann wird stets der Default-Wert verwendet - in
// diesem Fall also Deutsch - dafür aber ist das Programm schnell wie eh und jeh.
//#ifdef _DEBUG
inline std::string _t(const std::string& t) { return Texte::Text(t); }
//#endif
//#ifndef _DEBUG
//#define _t(T) std::string(T)
//#endif

#endif // !defined(TEXTE_H)

//@<< Version History >>
//@+node:<< Version History >>
// $Log: Texte.h,v $
// Revision 1.5  2004/10/17 19:12:40  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.4  2004/04/13 21:45:45  vrandezo
// Akademische Ausbildung (Magier) implementiert
// Aufräumarbeiten (besser in Leo integriert)
//
// Revision 1.3  2003/11/28 16:46:16  vrandezo
// Zeilenvorlauf am Dateiende (Warnung bei gcc)
//
// Revision 1.2  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.1.1.1  2003/02/17 15:34:35  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.12  2002/11/03 00:39:48  vrandezo
// Betrachter für einzelne Objekte implementiert
// CVS (fast) vollständig aufgeräumt
//
// Revision 1.11  2002/11/02 19:01:11  vrandezo
// Exportfenster hinzugefügt
//
// Revision 1.10  2002/10/19 18:58:33  vrandezo
// Texte in eigene, externe Datei untergebracht
//
// Revision 1.9  2002/10/19 17:53:58  vrandezo
// Erweiterung von Texte
// Version 1.11 begonnen
//
// Revision 1.8  2002/10/17 10:54:44  vrandezo
// Talent und Talente eingefügt
//
// Revision 1.7  2002/10/13 15:18:42  vrandezo
// Voraussetzungen für Professionen implementiert
// CVS aufgeräumt für Modifkatoren und Voraussetzungen
//
// Revision 1.6  2002/10/11 14:13:23  vrandezo
// Modifikatoren für Eigenschaften und Abgeleitete Werte eingeführt
//
// Revision 1.5  2002/10/09 19:58:34  vrandezo
// Auswahl von Rasse, Kultur, Profession ermöglicht
//
// Revision 1.4  2002/10/02 01:28:57  vrandezo
// Erweiterung von GUI und DSA.lib
//
// Revision 1.3  2002/09/26 14:52:20  vrandezo
// DSA.lib-Anbindung der GUI geschrieben
//
// Revision 1.2  2002/09/25 23:50:51  vrandezo
// Beginn der Aufteilung in DSA.lib und Anwendung
//
// Revision 1.1  2002/07/17 18:26:04  Zdenko Vrandecic
// Einchecken
//
//@nonl
//@-node:<< Version History >>
//@nl
//@-node:@file Texte.h
//@-leo
