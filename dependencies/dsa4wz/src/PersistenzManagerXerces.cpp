//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file PersistenzManagerXerces.cpp
//@+at
// PersistenzManagerXerces.cpp: Implementierung des PM mit 
// Xerces
// 
// $Revision: 1.11 $
// $Date: 2004/10/17 19:12:39 $
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
#include "PersistenzManagerXerces.h"
#include "Texte.h"
#include "NDSAUtil.h"
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

namespace DSA {
//@<< class PersistenzManagerXerces >>
//@+node:<< class PersistenzManagerXerces >>
//@+others
//@+node:ctor
	PersistenzManagerXerces::PersistenzManagerXerces()
	:	lader_(new LaderXerces()) {
		knoten_ = lader_->Wurzel();
	} // ctor
//@nonl
//@-node:ctor
//@+node:dtor
  PersistenzManagerXerces::~PersistenzManagerXerces() {
	} // dtor
//@nonl
//@-node:dtor
//@+node:LaderVersion
  const std::string PersistenzManagerXerces::LaderVersion() const {
    return lader_->LaderVersion();
  } // LaderVersion
//@nonl
//@-node:LaderVersion
//@+node:Lade
	int PersistenzManagerXerces::Lade(const std::string& dateiname) {
		int rv = lader_->Lade(dateiname);
		knoten_ = lader_->Wurzel();
    rv += Update::Commit(this);
		return rv;
	} // Lade
//@nonl
//@-node:Lade
//@+node:Speicher
	int PersistenzManagerXerces::Speicher(const std::string& dateiname) {
		return lader_->Speicher(dateiname, 4194303); // TODO Konstante: Dateigroessenbegrenzung auf 4 MB
	} // Speicher
//@nonl
//@-node:Speicher
//@+node:Version
	std::string PersistenzManagerXerces::Version() const {
		std::string v;
		lader_->LeseAttribut(lader_->Wurzel(), "version", &v);
		return v;
	} // Version
	
  void PersistenzManagerXerces::Version(const std::string& version) {
    lader_->AttributiereKnoten(lader_->Wurzel(), "version", version);
  } // Version

//@-node:Version
//@+node:OeffneZumLesen
	int PersistenzManagerXerces::OeffneZumLesen (const std::string& tag, std::string& id) {
		xercesc::DOMElement* ziel;
		StringListe pfad;
		pfad.push_back(tag);
		if (id!="") {
			pfad.push_back("ID");
			pfad.push_back(id);
			}
		int rv = lader_->gehePfad(pfad, &ziel, 0, knoten_);
		if (!rv) knoten_=ziel;
		if (id=="")
			if (lader_->ExistiertAttribut(knoten_, "ID"))
				rv += lader_->LeseAttribut(knoten_, "ID", &id);
		return rv;
	} // OeffneZumLesen

	int PersistenzManagerXerces::OeffneZumLesen (const std::string& tag, int nr) {
		xercesc::DOMElement* ziel;
		StringListe pfad;
		pfad.push_back(tag);
		int rv = lader_->gehePfad(pfad, &ziel, nr, knoten_);
		if (!rv) knoten_=ziel;
		return rv;
	} // OeffneZumLesen
  
  int PersistenzManagerXerces::OeffneZumLesen (const std::string& tag, int nr, std::string& id) {
		xercesc::DOMElement* ziel;
		StringListe pfad;
		pfad.push_back(tag);
		int rv = lader_->gehePfad(pfad, &ziel, nr, knoten_);
    id = "";
    if (!rv) {
      knoten_=ziel;
      if (lader_->ExistiertAttribut(knoten_, "ID"))
        rv += lader_->LeseAttribut(knoten_, "ID", &id);
    }
		return rv;
  } // OeffneZumLesen

  int PersistenzManagerXerces::OeffneZumLesen(const std::string& tag) {
		std::string s;
		return OeffneZumLesen(tag, s);
	} // OeffneZumLesen
//@nonl
//@-node:OeffneZumLesen
//@+node:Lese
	int PersistenzManagerXerces::Lese (const std::string& tag, std::string& wert) {
		if (!lader_->ExistiertKnoten(knoten_, tag)) return 1;
		int rv = OeffneZumLesen(tag);
		rv += lader_->LeseString(knoten_, &wert);
		rv += SchliesseNachLesen();
		return rv;
	} // Lese

	int PersistenzManagerXerces::Lese	(const std::string& tag, int& wert) {
		if (!lader_->ExistiertKnoten(knoten_, tag)) return 1;
		int rv = OeffneZumLesen(tag);
		rv += lader_->LeseInteger(knoten_, &wert);
		rv += SchliesseNachLesen();
		return rv;
	} // Lese
	
	int PersistenzManagerXerces::Lese (const std::string& tag, const std::string& id, std::string& wert) {
		if (!lader_->ExistiertKnoten(knoten_, tag, id)) return 1;
		std::string geleseneID = id;
		int rv = OeffneZumLesen(tag, geleseneID);
		if (geleseneID != id) ++rv;
		rv += lader_->LeseString(knoten_, &wert);
		rv += SchliesseNachLesen();
		return rv;
	} // Lese

	int PersistenzManagerXerces::Lese	(const std::string& tag, const std::string& id, int& wert) {
		if (!lader_->ExistiertKnoten(knoten_, tag, id)) return 1;
		std::string geleseneID = id;
		int rv = OeffneZumLesen(tag, geleseneID);
		if (geleseneID != id) ++rv;
		rv += lader_->LeseInteger(knoten_, &wert);
		rv += SchliesseNachLesen();
		return rv;
	} // Lese
    
//@+others
//@+node:Lese StringListe
int PersistenzManagerXerces::Lese (StringListe& liste, const std::string& einzeltag, const std::string& tag, std::string id) {
  liste.clear();
  if (!Existiert(tag, id)) return 0;
  int rv = OeffneZumLesen(tag, id);
  bool offen = (rv==0);
  int x = Anzahl(einzeltag);
  for (int i = 0; i < x; ++i) {
    std::string s;
    rv += LeseMehrfach(einzeltag, s, i);
    liste.push_back(s);
  }
  if (offen) rv += SchliesseNachLesen();
  return rv;
} // Lese StringListe
//@nonl
//@-node:Lese StringListe
//@+node:Lese DDict
int PersistenzManagerXerces::Lese (DDict& dict, const std::string& einzeltag, const std::string& tag, std::string id, const std::string& oberid) {
  dict.clear();
  if (!Existiert(tag, id)) return 0;
  int rv = OeffneZumLesen(tag, id);
  bool offen = (rv==0);
  
  if (Existiert(einzeltag)) {
    StringListe pfad;
    pfad.push_back(einzeltag);
    xercesc::DOMElement* node = 0;
    lader_->gehePfad(pfad, &node, 0, knoten_);
    if (!lader_->ExistiertAttribut(node, "ID")) {
      rv += SchliesseNachLesen();
      //return 0;
      StringListe sl;
      rv += Lese(sl, einzeltag, tag, id);
      for (StringListe::const_iterator i = sl.begin(); i != sl.end(); ++i) {
        Dict d;
        d[DSA::StandardLang()] = *i;
        dict[strings::MacheID(einzeltag, oberid + *i)] = d;
      }
      return rv;
    }
  }
  
  StringListe liste;
  rv += IDListe(einzeltag, &liste);
  
  for (StringListe::const_iterator i = liste.begin(); i != liste.end(); ++i) {
    std::string id = *i;
    int rv2 = OeffneZumLesen(einzeltag, id);
    if (rv2!=0) { rv += rv2; continue; }
    
    StringListe langs;
    rv += IDListe("Bezeichnung", &langs);
    Dict d;
    for (StringListe::const_iterator j = langs.begin(); j != langs.end(); ++j) {
      std::string s;
      rv += Lese("Bezeichnung", *j, s);
      d[*j] = s;
    }
    dict[*i] = d;
    rv += SchliesseNachLesen();
  }

  if (offen) rv += SchliesseNachLesen();
  return rv;
}
//@nonl
//@-node:Lese DDict
//@-others
//@nonl
//@-node:Lese
//@+node:LeseMehrfach
  int PersistenzManagerXerces::LeseMehrfach (const std::string& tag, std::string& wert, int nr) {
		if (!lader_->ExistiertKnoten(knoten_, tag)) return 1;
		int rv = OeffneZumLesen(tag, nr);
		rv += lader_->LeseString(knoten_, &wert);
		rv += SchliesseNachLesen();
		return rv;
  } // LeseMehrfach

  int PersistenzManagerXerces::LeseMehrfach (const std::string& tag, int& wert, int nr) {
		if (!lader_->ExistiertKnoten(knoten_, tag)) return 1;
		int rv = OeffneZumLesen(tag, nr);
		rv += lader_->LeseInteger(knoten_, &wert);
		rv += SchliesseNachLesen();
		return rv;
  } // LeseMehrfach
//@nonl
//@-node:LeseMehrfach
//@+node:Anzahl
  int PersistenzManagerXerces::Anzahl (const std::string& tag, const std::string& id) {
    return lader_->AnzahlKnoten(knoten_, tag, id);
  } // AnzahlKnoten

	int PersistenzManagerXerces::SchliesseNachLesen() {
		knoten_ = lader_->VaterKnoten(knoten_); // TODO: Fehlerabfrage
		return 0;
	} // SchliesseZumLesen
//@nonl
//@-node:Anzahl
//@+node:OeffneZumSchreiben
 	int PersistenzManagerXerces::OeffneZumSchreiben (const std::string& tag, const std::string& id) {
		xercesc::DOMElement* ziel;
		StringListe pfad;
		pfad.push_back(tag);
		if (id!="") {
			pfad.push_back("ID");
			pfad.push_back(id);
		}
		int rv = 0;
		if (!lader_->ExistiertKnoten(knoten_, tag, id))
      rv = lader_->NeuerKnoten(knoten_, tag, id);
		rv += lader_->gehePfad(pfad, &ziel, 0, knoten_);
		if (!rv) knoten_=ziel;
			
		return rv; // TODO
	} // OeffneZumSchreiben

  int PersistenzManagerXerces::OeffneZumSchreiben (const std::string& tag, int nr) {
		xercesc::DOMElement* ziel;
		StringListe pfad;
		pfad.push_back(tag);
    int rv = 0;
    if (!lader_->ExistiertKnoten(knoten_, tag, nr))
      rv = lader_->NeuerKnoten(knoten_, tag, nr);
    nr = lader_->AnzahlKnoten(knoten_, tag)-1;
    rv += lader_->gehePfad(pfad, &ziel, nr, knoten_);
    if (!rv) knoten_=ziel;
      
    return rv;
  } // OeffneZumSchreiben
//@nonl
//@-node:OeffneZumSchreiben
//@+node:Schreibe
	int PersistenzManagerXerces::Schreibe (const std::string& tag, const std::string& wert) {
		int rv = 0;
		rv += OeffneZumSchreiben(tag);
		rv += lader_->SchreibeString(knoten_, wert);
		rv += SchliesseNachSchreiben();
		return rv;
	} // Schreibe

	int PersistenzManagerXerces::Schreibe (const std::string& tag, const int wert) {
		int rv = 0;
		rv += OeffneZumSchreiben(tag);
		rv += lader_->SchreibeInteger(knoten_, wert);
		rv += SchliesseNachSchreiben();
		return rv;
	} // Schreibe

	int PersistenzManagerXerces::Schreibe (const std::string& tag, const std::string& id, const std::string& wert) {
		int rv = 0;
		rv += OeffneZumSchreiben(tag, id);
		rv += lader_->SchreibeString(knoten_, wert);
		rv += SchliesseNachSchreiben();
		return rv;
	} // Schreibe

	int PersistenzManagerXerces::Schreibe (const std::string& tag, const std::string& id, const int wert) {
		int rv = 0;
		rv += OeffneZumSchreiben(tag, id);
		rv += lader_->SchreibeInteger(knoten_, wert);
		rv += SchliesseNachSchreiben();
		return rv;
	} // Schreibe

//@+others
//@+node:Schreibe StringListe
int PersistenzManagerXerces::Schreibe (const StringListe& liste, const std::string& einzeltag, const std::string& tag, const std::string& id) {
  if (liste.size()==0) return 0;
  int rv = OeffneZumSchreiben(tag, id);
//    if (rv!=0) return rv;
  for (StringListe::const_iterator s = liste.begin(); s != liste.end(); ++s)
    rv += SchreibeMehrfach(einzeltag, *s);
  rv += SchliesseNachSchreiben();
  return rv;
} // Schreibe StringListe
//@nonl
//@-node:Schreibe StringListe
//@+node:Schreibe DDict
int PersistenzManagerXerces::Schreibe (const DDict& dict, const std::string& einzeltag, const std::string& tag, const std::string& id) {
  if (dict.size()==0) return 0;
  int rv = OeffneZumSchreiben(tag, id);
  for (DDict::const_iterator i = dict.begin(); i != dict.end(); ++i) {
    rv += OeffneZumSchreiben(einzeltag, i->first);
    for (Dict::const_iterator j = i->second.begin(); j != i->second.end(); ++j) {
      rv += Schreibe("Bezeichnung", j->first, j->second);
    }
    rv += SchliesseNachSchreiben();
  }
  rv += SchliesseNachSchreiben();
  return rv;
}

//@-node:Schreibe DDict
//@-others
//@nonl
//@-node:Schreibe
//@+node:SchreibeMehrfach
  int PersistenzManagerXerces::SchreibeMehrfach (const std::string& tag, const std::string& wert) {
		int rv = 0;
		rv += OeffneZumSchreiben(tag, lader_->AnzahlKnoten(knoten_, tag)+1);
		rv += lader_->SchreibeString(knoten_, wert);
		rv += SchliesseNachSchreiben();
		return rv;
  } // SchreibeMehrfach

  int PersistenzManagerXerces::SchreibeMehrfach (const std::string& tag, int wert) {
		int rv = 0;
		rv += OeffneZumSchreiben(tag, lader_->AnzahlKnoten(knoten_, tag)+1);
		rv += lader_->SchreibeInteger(knoten_, wert);
		rv += SchliesseNachSchreiben();
		return rv;
  } // SchreibeMehrfach
//@nonl
//@-node:SchreibeMehrfach
//@+node:SchliesseNachSchreiben
	int PersistenzManagerXerces::SchliesseNachSchreiben() {
		knoten_ = lader_->VaterKnoten(knoten_); // TODO: Fehlerabfrage
		return 0;
	} // SchliesseZumLesen
//@nonl
//@-node:SchliesseNachSchreiben
//@+node:SchreibePI
  int PersistenzManagerXerces::SchreibePI(const std::string& target, const std::string& data) {
    return lader_->SchreibePI(target, data);
  } // SchreibePI
//@nonl
//@-node:SchreibePI
//@+node:Existiert
	bool PersistenzManagerXerces::Existiert(const std::string& tag, const std::string& id) {
		return lader_->ExistiertKnoten(knoten_, tag, id);
	} // Existiert
//@nonl
//@-node:Existiert
//@+node:Loesche
	int PersistenzManagerXerces::Loesche(const std::string& tag, const std::string& id) {
		return lader_->LoescheKnoten(knoten_, tag, id);
	} // Loesche
//@nonl
//@-node:Loesche
//@+node:IDListe
	int PersistenzManagerXerces::IDListe (const std::string& tag, StringListe* liste) const {
		return lader_->IDListe(knoten_, tag, liste);
	} // IDListe

	int PersistenzManagerXerces::IDListe (const std::string& tag, StringListe& liste) const {
		return lader_->IDListe(knoten_, tag, &liste);
	} // IDListe
//@nonl
//@-node:IDListe
//@-others
//@nonl
//@-node:<< class PersistenzManagerXerces >>
//@nl
}

//@<< Version History >>
//@+node:<< Version History >>
// $Log: PersistenzManagerXerces.cpp,v $
// Revision 1.11  2004/10/17 19:12:39  vrandezo
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
// Revision 1.9  2004/04/04 17:29:04  vrandezo
// Re-Generierungs-bug gefixt
//
// Revision 1.8  2004/01/29 09:06:16  dertron
// doppelte CVS Log-Tags entfernt
//
// Revision 1.7  2004/01/05 02:45:49  vrandezo
// Charakteristika bei RPK-Definition von Mengen auf Auswahlmengen umgestellt
//
// Revision 1.6  2003/11/28 16:45:03  vrandezo
// Zeilenvorlauf am Dateiende (Warnung bei gcc)
//
// Revision 1.5  2003/10/24 13:26:02  vrandezo
// DEBUG_NEW raus
//
// Revision 1.4  2003/10/19 20:22:16  vrandezo
// Eingecheckt in Leo
//
// Revision 1.3  2003/03/03 21:39:06  vrandezo
// Testdruck m�glich
// XML-Export, der mit Gerhard Mayers XSL arbeitet
//
// Revision 1.2  2003/02/19 19:33:45  vrandezo
// Bug: Verrechnungspunkte f�r Schlechte Eigenschaften sind nun korrekt
// verbilligte SF bei Personen und RKP eingef�gt
// RKP mit SF und VSF ausstattbar
//
// Revision 1.1.1.1  2003/02/17 15:32:24  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.12  2003/02/13 22:29:30  vrandezo
// Auf Xerces 2.2.0 geupdatet
//
// Revision 1.11  2003/02/13 11:44:46  vrandezo
// Bug entfernt: Sprachen und Schriften bei Personen bleiben erhalten
// Bug entfernt: Kulturen-Persistenz korrigiert (L/S Sprachen)
// Person-Vergleich korrigiert
// Dateibegrenzung auf 4MB erh�ht
// XML ge�ndert: Sprachen jetzt mit "Sprachen kennen"-ID bei RKP
//
// Revision 1.10  2003/01/16 22:57:35  vrandezo
// m�nnliche und weibliche Bezeichnungen bei den Professionen
// Geschlechtsspezifische Personenkomponenten (als Varianten realisiert) implementiert
//
// Revision 1.9  2003/01/16 16:32:17  vrandezo
// Mutter- und Zweitsprache bei Kultur einstellbar
//
// Revision 1.8  2003/01/13 00:30:49  vrandezo
// Release Version 1.12 dev
// Korrektur der Verrechnungspunkteberechnung f�r negative Talentmodifikatoren
// Talentauswahl implementiert
//
// Revision 1.7  2002/11/03 19:48:00  vrandezo
// Gr��e, Gewicht, Alter eingef�gt
//
// Revision 1.6  2002/11/02 23:36:57  vrandezo
// Exportier und Importierfunktionen implementiert
// Vergleichsoperationen implementiert
// Kampftechnikfenstter implementiert
//
// Revision 1.5  2002/11/02 15:39:48  vrandezo
// Importfenster hinzugef�gt
//
// Revision 1.4  2002/11/01 15:27:29  vrandezo
// Regeln neu implementiert
// ILader, MLader, IMLader, ILadbar und weitere Klassen entfernt
// XMLChar-Transcoding beschleunigt
//
// Revision 1.3  2002/10/27 17:29:21  vrandezo
// Die meisten Teile sind speicherbar
//
// Revision 1.2  2002/10/25 23:06:33  vrandezo
// Speichern erweitert
//
// Revision 1.1  2002/09/25 23:52:47  vrandezo
// Beginn der Aufteilung in DSA.lib und Anwendung
//
//@nonl
//@-node:<< Version History >>
//@nl
//@-node:@file PersistenzManagerXerces.cpp
//@-leo
