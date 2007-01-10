//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file Texte.cpp
//@+at
// Texte.cpp: Schnittstelle für die Textliterale
// 
// $Revision: 1.6 $
// $Date: 2004/10/17 22:26:55 $
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
#include "Log.h"
//#include "wx/textfile.h"
#include "Texte.h"

#include "detconfig.h"
#include "LaderXerces.h"
//@nonl
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

//@<< Warnung? >>
//@+node:<< Warnung? >>
//@@language c

//#define FEHLENDE_TERME_ANZEIGEN
//@nonl
//@-node:<< Warnung? >>
//@nl

Texte* Texte::instanz_ = 0;

//@+others
//@+node:Lang
namespace DSA {
  std::string Lang() {
    return detConfig::Sprache();
  }
  
  std::string StandardLang() {
    return "Deutsch";
  }

}
//@-node:Lang
//@+node:Text
std::string Texte::Text(const std::string& t) {
  if (instanz_==0) { 
#ifdef _DEBUG      
      Log::Error("Texte::instanz_ war nicht initialisiert"); 
#endif
      Initialisiere();
  }
  DDict::const_iterator i = instanz_->lexikon_.find(t);
  if (i == instanz_->lexikon_.end()) {
    // Log::Error("Texte::Text: unbekannter Ausdruck " + t); TODO Kommentar weg
#ifdef FEHLENDE_TERME_ANZEIGEN
    Log::Debug("                              Texte::Text: unbekannter Term " + t);
#endif
    return t;
  }
  Dict::const_iterator j = i->second.find(DSA::Lang());
  if (j != i->second.end())
    return j->second;
  j = i->second.find(DSA::StandardLang());
  if (j != i->second.end())
    return j->second;
  if (i->second.size() > 0)
    return i->second.begin()->second;

  Log::Debug("                           Texte::Text(2): unbekannter Term " + t);
  return t;
}

//@-node:Text
//@+node:Initialisiere
void Texte::Initialisiere(const std::string& datei) {
	if (instanz_ != 0) delete instanz_;
	instanz_ = new Texte(datei);
}
//@nonl
//@-node:Initialisiere
//@+node:Deinitialisiere
//void Texte::Deinitialisiere() {
//	delete instanz_;
//}
//@-node:Deinitialisiere
//@+node:Texte
Texte::Texte(const std::string& datei) {//: datei_(new wxTextFile(datei.c_str())) {
//@<< alt >>
//@+node:<< alt >>
//	if (datei_->Exists())
//		datei_->Open();
//	else {
//		datei_->Create();
//		datei_->AddLine("Enthält die Texte für das DSA4 Werkzeug. Jede zweite Zeile "
//			              "gibt den internen, deutschen Text des Programms an, in der "
//										"darauf folgenden Zeile steht dann der tatsächlich vom "
//										"Programm zu verwendende Text. Dadurch sollte eine "
//										"Übersetzung des Programms leicht werden. Achtung: diese "
//										"Texte sind insbesondere auch für das Datenformat relevant.");
//		datei_->AddLine("--");
//	}
//	int laenge = datei_->GetLineCount();
//	if ((laenge % 2) != 0) --laenge;
//	if (laenge<3) return;
//	laenge /= 2;
//	for (int i = 1; i < laenge; ++i) {
//		std::string dies = datei_->GetLine(i*2).c_str();
//		std::string jenes = datei_->GetLine(i*2+1).c_str();
//		lexikon_.insert(std::make_pair(dies, jenes));
//	}
//@nonl
//@-node:<< alt >>
//@nl
  LaderXerces* l = new LaderXerces();
  l->Lade(datei);
  if (!l->DocLeer()) { 
    int z = l->AnzahlKnoten(l->Wurzel(), "Begriff");  
    if (z>0) {
      StringListe ids;
      l->IDListe(l->Wurzel(), "Begriff", &ids);
      for (StringListe::iterator s = ids.begin(); s != ids.end(); ++s) {
        xercesc::DOMElement* node = 0;
        StringListe sl;
        sl.push_back("Begriffe");
        sl.push_back("Begriff");
        sl.push_back("ID");
        sl.push_back(*s);
        l->gehePfad(sl, &node);
        z = l->AnzahlKnoten(node, "Sprache");
        if (z>0) {
          StringListe sprachen;
          l->IDListe(node, "Sprache", &sprachen);
          for (StringListe::iterator s2 = sprachen.begin(); s2 != sprachen.end(); ++s2) {
            xercesc::DOMElement* node2 = 0;
            StringListe sl2;
//            sl2.push_back("Begriffe");
//            sl2.push_back("Begriff");
//            sl2.push_back("ID");
//            sl2.push_back(*s);
            sl2.push_back("Sprache");
            sl2.push_back("ID");
            sl2.push_back(*s2);
            l->gehePfad(sl2, &node2, 0, node);
            std::string w;
            l->LeseString(node2, &w);
            lexikon_[*s][*s2] = w;
          }
        }
      }
    }
  }
  delete l;
}
//@nonl
//@-node:Texte
//@+node:Texte
Texte::~Texte() {
//	int l;
//	while ((l = datei_->GetLineCount()) > 2) datei_->RemoveLine(l-1);
//	for (std::map<std::string, std::string>::iterator i = lexikon_.begin(); i != lexikon_.end(); ++i) {
//		datei_->AddLine(i->first.c_str());
//		datei_->AddLine(i->second.c_str());
//	}
//	datei_->Write();
}
//@nonl
//@-node:Texte
//@-others

//@<< Version History >>
//@+node:<< Version History >>

// $Log: Texte.cpp,v $
// Revision 1.6  2004/10/17 22:26:55  josch1710
// - Bugfixes
// - Autotools-Unterstützung.
//
// Revision 1.5  2004/10/17 20:04:52  twel
// Gross-/Kleinschreibung
//
// Revision 1.4  2004/10/17 19:12:40  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.3  2004/04/13 21:45:45  vrandezo
// Akademische Ausbildung (Magier) implementiert
// Aufräumarbeiten (besser in Leo integriert)
//
// Revision 1.2  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.1.1.1  2003/02/17 15:34:35  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.12  2002/10/19 18:58:33  vrandezo
// Texte in eigene, externe Datei untergebracht
//
// Revision 1.11  2002/10/19 17:53:58  vrandezo
// Erweiterung von Texte
// Version 1.11 begonnen
//
// Revision 1.10  2002/10/17 10:54:44  vrandezo
// Talent und Talente eingefügt
//
// Revision 1.9  2002/10/13 15:18:42  vrandezo
// Voraussetzungen für Professionen implementiert
// CVS aufgeräumt für Modifkatoren und Voraussetzungen
//
// Revision 1.8  2002/10/11 14:13:23  vrandezo
// Modifikatoren für Eigenschaften und Abgeleitete Werte eingeführt
//
// Revision 1.7  2002/10/09 19:58:34  vrandezo
// Auswahl von Rasse, Kultur, Profession ermöglicht
//
// Revision 1.6  2002/10/02 01:28:57  vrandezo
// Erweiterung von GUI und DSA.lib
//
// Revision 1.5  2002/09/26 14:52:20  vrandezo
// DSA.lib-Anbindung der GUI geschrieben
//
// Revision 1.4  2002/09/25 23:50:51  vrandezo
// Beginn der Aufteilung in DSA.lib und Anwendung
//
// Revision 1.3  2002/08/30 15:06:18  vrandezo
// Apache Xerces ersetzt den MSXML
// Zahlreiche Speicherlecks entfernt
// Entwurf verstärkt
//
// Revision 1.2  2002/08/08 18:51:09  vrandezo
// da Xerces und andere XML-Parser XPath nicht
// unterstützen, wurde die Navigation im DOM-Baum
// von XPath auf eine generische Variante umgestellt.
// Dazu mussten allen Schnittstellen verändert werden.
//
// Revision 1.1  2002/07/17 18:26:03  Zdenko Vrandecic
// Einchecken
//
//@nonl
//@-node:<< Version History >>
//@nl
//@nonl
//@-node:@file Texte.cpp
//@-leo
