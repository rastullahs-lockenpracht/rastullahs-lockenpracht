//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file SpracheSchrift.cpp
//@+at
// SpracheSchrift.cpp: Implementierung von Sprachen und 
// Schriften.
// 
// $Revision: 1.11 $
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
//@<< includes >>
//@+node:<< includes >>
#include "SpracheSchrift.h"

#include "Person.h"

#include "Texte.h"
#include "Log.h"
#include "NDSAUtil.h"

#include "PersistenzManager.h"
#include <numeric>
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

// Laden/Speichern

namespace DSA {

	//@  << SpracheDefinition >>
	//@+node:<< SpracheDefinition >>
	//@+others
	//@+node:Lese
	int SpracheDefinition::Lese(PersistenzManager* pm, const std::string& id) {
		std::string geleseneID = id;
		int rv = pm->OeffneZumLesen(Klasse(), geleseneID);
	  Dict bezeichnung;
	  Dict beschreibung;
	  rv += LeseBezeichnung(pm, geleseneID, bezeichnung, beschreibung);
		std::string sBuf;
		int rv0 = pm->Lese("SKT-Spalte", sBuf);
		if (rv0 != 0) sBuf = "A";
		SKTSpalte skt = SKTSpalteString(sBuf);
		int komplexitaet = 0;
	  int temp  = pm->Lese(("Komplexitaet"), komplexitaet);
	  if (temp!=0)
	    rv += pm->Lese(("Komplexität"), komplexitaet);
	
		if (!rv) {
			ID(geleseneID);
			SKTSpalte_(skt);
			Komplexitaet(komplexitaet);
	    bezeichnung_ = bezeichnung;
	    beschreibung_ = beschreibung;
		}
		rv += pm->SchliesseNachLesen();
		return rv; // TODO
	} // Lese
	//@nonl
	//@-node:Lese
	//@+node:Schreibe
	int SpracheDefinition::Schreibe(PersistenzManager* pm) const {
		int rv = pm->OeffneZumSchreiben(Klasse(), ID());
	  rv += SchreibeBezeichnung(pm);
		rv += pm->Schreibe("Komplexitaet", Komplexitaet());
		rv += pm->Schreibe("SKT-Spalte", SKTSpalteString(SKTSpalte_()));
		rv += pm->SchliesseNachSchreiben();
		return rv;
	} // Schreibe
	//@nonl
	//@-node:Schreibe
	//@+node:Pruefe
	int SpracheDefinition::Pruefe() {
	  int rv = 0;
	  if (Komplexitaet()<0) {
	    Log::Info(("Negative Komplexität"));
	    ++rv;
	  }
	  if (rv!=0) Log::Info("in " + Klasse() + " " + ID());
		return rv;
	} // Pruefe
	//@nonl
	//@-node:Pruefe
	//@+node:IDListe
	int SpracheDefinition::IDListe(PersistenzManager* pm, StringListe* liste) {
		return pm->IDListe(Klasse(), liste); // TODO Prüfen
	} // IDListe
	//@nonl
	//@-node:IDListe
	//@+node:Execute
	std::string SpracheDefinition::Execute(const std::string& cmd) {
	  if ("sktplus"==cmd) {
	    SKTSpalte_(SpalteSchwerer(SKTSpalte_()));
	    return "";
	  }
	  if ("sktminus"==cmd) {
	    SKTSpalte_(SpalteLeichter(SKTSpalte_()));
	    return "";
	  }
	  return ("Unbekannter Befehl");
	}
	//@nonl
	//@-node:Execute
	//@+node:Return
	std::string SpracheDefinition::Return(const std::string& request) const {
	//  if (""==request) return Text();
	  if ("name"==request) return ID();
	  if ("familie"==request) return Familie();
	  if ("komplexitaet"==request) return strings::int2sStr(Komplexitaet());
	  if ("skt"==request) return SKTSpalteString(SKTSpalte_());
	  if ("beschreibung"==request) return Beschreibung();
	  return ("Unbekannte Abfrage");
	}
	//@-node:Return
	//@-others
	
	//@-node:<< SpracheDefinition >>
	//@nl
  
  SpracheDefinitionen* sprachen() {
    static SpracheDefinitionen* spracheDefinitionen_ = new SpracheDefinitionen();
    return spracheDefinitionen_;
  }

	//@  << Sprache >>
	//@+node:<< Sprache >>
	//@+others
	//@+node:Wert
	void Sprache::Wert(int wert) {
	  if (!(Muttersprache()||Zweitsprache())) {
	    wert_ = wert;
	    return;
	  }
	  if (person_==0) {
	    wert_ = wert;
	    return;
	  }
	  
	  int diff = Muttersprache() ? 2 : 4; // TODO Konstante
	  wert_ = wert - (person_->Wert(Klugheit)-diff);
	}
	
	int Sprache::Wert() const {
	  if (!(Muttersprache()||Zweitsprache()))
	    return wert_;
	  if (person_==0)
	    return wert_;
	    
	  int diff = Muttersprache() ? 2 : 4; // TODO Konstante
	  return wert_ + (person_->Wert(Klugheit)-diff);
	}
	//@-node:Wert
	//@+node:Text
	std::string Sprache::Text(const bool nurNameUndWert) const {
	  std::string s = ID() + " " + strings::int2sStr(Wert(), true);
	  if (Muttersprache()) return _t("Muttersprache") + ": " + s;
	  if (Zweitsprache()) return _t("Zweitsprache") + ": " + s;
	  if (!nurNameUndWert) return s + " (" + Klasse() + ")";
	  return s;
	} // Text
	//@nonl
	//@-node:Text
	//@+node:Extra
	void Sprache::Extra(const std::string& extra) {
	  Log::Error("Sprache::Extra: Unbekanntes Extra " + extra);
	}
	//@nonl
	//@-node:Extra
	//@+node:ErsetzeTag
	std::string Sprache::ErsetzeTag(const std::string& tag) const {
	  if (""==tag) return Text();
	  if ("Name"==tag) return ID();
	  if ("M"==tag) if (Muttersprache()) return "Muttersprache"; else return "";
	  if ("Z"==tag) if (Zweitsprache()) return "Zweitsprache"; else return "";
	  if ("W"==tag) return strings::int2sStr(Wert(), true);
	  if ("SKT"==tag) return SKTSpalteString(SKTSpalte_());
	  if ("Beschreibung"==tag) return Definition()->Beschreibung();
	  return tag;
	} // ErsetzeTag
	//@nonl
	//@-node:ErsetzeTag
	//@+node:Definition
	const SpracheDefinition* Sprache::Definition() const {
		SpracheDefinition* def = sprachen()->Zeige(id_);
		if (def!=0) return def;
		Log::Error("Sprache::Definition: Undefinierte Sprache " + id_);
		static SpracheDefinition* sdef = new SpracheDefinition();
		return sdef; // TODO workaround. Sollte bei nicht vorhandenen Defs intelligenter
		// reagieren (doch wie?) // TODO Speicherleck
	} // Definition
	//@nonl
	//@-node:Definition
	//@+node:Lese
	int Sprache::Lese(PersistenzManager* pm, const std::string& id) {
		std::string geleseneID = id;
		int rv = pm->OeffneZumLesen(Klasse(), geleseneID);
		int wert;
		rv += pm->Lese(("Wert"), wert);
	  bool muttersprache = false;
	  bool zweitsprache = false;
	  if (pm->Existiert(("Art"))) {
	    std::string s;
	    rv += pm->Lese(("Art"), s);
	    if (s==("Muttersprache")) muttersprache = true;
	    if (s==("Zweitsprache")) zweitsprache = true;
	  }
	
	  if (!rv) {
	    person_ = 0;
			ID(geleseneID);
			Wert(wert);
	    Muttersprache(muttersprache);
	    Zweitsprache(zweitsprache);
		}
		pm->SchliesseNachLesen();
		return rv; // TODO
	} // Lese
	//@nonl
	//@-node:Lese
	//@+node:Schreibe
	int Sprache::Schreibe(PersistenzManager* pm) const {
		int rv = pm->OeffneZumSchreiben(Klasse(), ID());
		rv += pm->Schreibe(("Wert"), Wert());
	  if (Muttersprache()) rv += pm->Schreibe(("Art"), ("Muttersprache"));
	  if (Zweitsprache()) rv += pm->Schreibe(("Art"), ("Zweitsprache"));
		rv += pm->SchliesseNachSchreiben();
		return rv;
	} // Schreibe
	//@nonl
	//@-node:Schreibe
	//@+node:Pruefe
	int Sprache::Pruefe() {
	  int rv = 0;
	  if (sprachen()->Zeige(ID()) == 0) {
	    Log::Info("Sprache::Pruefe: unbekanntes Sprache " + ID());
	    std::string s = sprachen()->Finde(ID());
	    if (s != "") {
	      Log::Info("ist eigentlich " + s + " (wurde korrigiert)");
	      ID(s);
	    }
	    ++rv;
	  }
		return rv;
	} // Pruefe
	//@nonl
	//@-node:Pruefe
	//@+node:IDListe
	int Sprache::IDListe(PersistenzManager* pm, StringListe* liste) {
		return pm->IDListe(Klasse(), liste); // TODO Prüfen
	} // IDListe
	//@nonl
	//@-node:IDListe
	//@-others
	//@nonl
	//@-node:<< Sprache >>
	//@nl
  
	//@  << SchriftDefinition >>
	//@+node:<< SchriftDefinition >>
	//@+others
	//@+node:Lese
	int SchriftDefinition::Lese(PersistenzManager* pm, const std::string& id) {
		std::string geleseneID = id;
		int rv = pm->OeffneZumLesen(Klasse(), geleseneID);
	  Dict bezeichnung;
	  Dict beschreibung;
	  rv += LeseBezeichnung(pm, geleseneID, bezeichnung, beschreibung);
		std::string sBuf;
		int rv0 = pm->Lese("SKT-Spalte", sBuf);
		if (rv0 != 0) sBuf = "A";
		SKTSpalte skt = SKTSpalteString(sBuf);
		int komplexitaet = 0;
	  int temp  = pm->Lese(("Komplexitaet"), komplexitaet);
	  if (temp!=0)
	    rv += pm->Lese(("Komplexität"), komplexitaet);
	
		if (!rv) {
			ID(geleseneID);
			SKTSpalte_(skt);
			Komplexitaet(komplexitaet);
	    beschreibung_ = beschreibung;
	    bezeichnung_ = bezeichnung;
		}
		rv += pm->SchliesseNachLesen();
		return rv; // TODO
	} // Lese
	//@-node:Lese
	//@+node:Schreibe
	int SchriftDefinition::Schreibe(PersistenzManager* pm) const {
		int rv = pm->OeffneZumSchreiben(Klasse(), ID());
	  rv += SchreibeBezeichnung(pm);
		rv += pm->Schreibe(("Komplexitaet"), Komplexitaet());
		rv += pm->Schreibe(("SKT-Spalte"), SKTSpalteString(SKTSpalte_()));
		rv += pm->SchliesseNachSchreiben();
		return rv;
	} // Schreibe
	//@nonl
	//@-node:Schreibe
	//@+node:Pruefe
	int SchriftDefinition::Pruefe() {
	  int rv = 0;
	  if (Komplexitaet()<0) {
	    Log::Info(("Negative Komplexität"));
	    ++rv;
	  }
	  if (rv!=0) Log::Info("in " + Klasse() + " " + ID());
		return rv;
	} // Pruefe
	//@-node:Pruefe
	//@+node:IDListe
	int SchriftDefinition::IDListe(PersistenzManager* pm, StringListe* liste) {
		return pm->IDListe(Klasse(), liste); // TODO Prüfen
	} // IDListe
	//@nonl
	//@-node:IDListe
	//@-others
	//@-node:<< SchriftDefinition >>
	//@nl
  
  SchriftDefinitionen* schriften() {
    static SchriftDefinitionen* schriftDefinitionen_ = new SchriftDefinitionen();
    return schriftDefinitionen_;
  }
  
	//@  << Schrift >>
	//@+node:<< Schrift >>
	//@+others
	//@+node:Text
	std::string Schrift::Text(const bool nurNameUndWert) const {
	  std::string s = ID() + " " + strings::int2sStr(Wert(), true);
	  if (!nurNameUndWert) return s + " (" + Klasse() + ")";
	  return s;
	} // Text
	//@nonl
	//@-node:Text
	//@+node:ErsetzeTag
	std::string Schrift::ErsetzeTag(const std::string& tag) const {
	  if (""==tag) return Text();
	  if ("Name"==tag) return ID();
	  if ("W"==tag) return strings::int2sStr(Wert(), true);
	  if ("SKT"==tag) return SKTSpalteString(SKTSpalte_());
	  if ("Beschreibung"==tag) return Definition()->Beschreibung();
	  return tag;
	} // ErsetzeTag
	//@nonl
	//@-node:ErsetzeTag
	//@+node:Extra
	void Schrift::Extra(const std::string& extra) {
	  Log::Error("Schrift::Extra: Unbekanntes Extra " + extra);
	}
	//@nonl
	//@-node:Extra
	//@+node:Definition
	const SchriftDefinition* Schrift::Definition() const {
		SchriftDefinition* def = schriften()->Zeige(id_);
		if (def!=0) return def;
		Log::Error("Schrift::Definition: Undefinierte Schrift " + id_);
		static SchriftDefinition* sdef = new SchriftDefinition();
		return sdef; // TODO workaround. Sollte bei nicht vorhandenen Defs intelligenter
		// reagieren (doch wie?) // TODO Speicherleck
	} // Definition
	//@nonl
	//@-node:Definition
	//@+node:Lese
	int Schrift::Lese(PersistenzManager* pm, const std::string& id) {
		std::string geleseneID = id;
		int rv = pm->OeffneZumLesen(Klasse(), geleseneID);
		int wert;
		rv += pm->Lese(("Wert"), wert);
		if (!rv) {
	    person_ = 0;
			ID(geleseneID);
			Wert(wert);
		}
		pm->SchliesseNachLesen();
		return rv; // TODO
	} // Lese
	//@nonl
	//@-node:Lese
	//@+node:Schreibe
	int Schrift::Schreibe(PersistenzManager* pm) const {
		int rv = pm->OeffneZumSchreiben(Klasse(), ID());
		rv += pm->Schreibe(("Wert"), Wert());
		rv += pm->SchliesseNachSchreiben();
		return rv;
	} // Schreibe
	//@nonl
	//@-node:Schreibe
	//@+node:Pruefe
	int Schrift::Pruefe() {
	  int rv = 0;
	  if (schriften()->Zeige(ID()) == 0) {
	    Log::Info("Schrift::Pruefe: unbekanntes Schrift " + ID());
	    std::string s = schriften()->Finde(ID());
	    if (s != "") {
	      Log::Info("ist eigentlich " + s + " (wurde korrigiert)");
	      ID(s);
	    }
	    ++rv;
	  }
		return rv;
	} // Pruefe
	//@-node:Pruefe
	//@+node:IDListe
	int Schrift::IDListe(PersistenzManager* pm, StringListe* liste) {
		return pm->IDListe(Klasse(), liste); // TODO Prüfen
	} // IDListe
	//@nonl
	//@-node:IDListe
	//@-others
	//@nonl
	//@-node:<< Schrift >>
	//@nl

}

//@<< Version History >>
//@+node:<< Version History >>
// $Log: SpracheSchrift.cpp,v $
// Revision 1.11  2004/10/17 19:12:40  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.10  2004/04/17 15:06:01  vrandezo
// um Execute und Return erweitert
//
// Revision 1.9  2004/04/13 14:06:52  vrandezo
// Hauszauber implementiert
//
// Revision 1.8  2004/04/12 21:08:09  vrandezo
// Lehrsprachen implementiert
// Mutter-/Zweitsprachensystem umgebaut
//
// Revision 1.7  2004/01/29 09:06:20  dertron
// doppelte CVS Log-Tags entfernt
//
// Revision 1.6  2003/11/28 16:46:16  vrandezo
// Zeilenvorlauf am Dateiende (Warnung bei gcc)
//
// Revision 1.5  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.4  2003/03/04 15:23:21  vrandezo
// weitere Tags implementiert für die Druckvorlagen
//
// Revision 1.3  2003/02/24 17:37:44  vrandezo
// Prüfen implementiert
//
// Revision 1.2  2003/02/20 09:19:45  vrandezo
// Prüfe-Signatur geändert
//
// Revision 1.1.1.1  2003/02/17 15:34:23  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.6  2003/01/19 01:05:22  vrandezo
// Mutter- und Zweitsprachen bei Heldenerschaffung vollständig integriert
// Werte passen sich ständig der aktuellen Klugheit an
// Sprachen und Schriften in Heldenerschaffung integriert
// Bei möglicher Talentaktivierung werden nur passende Talente angezeigt
//
// Revision 1.5  2003/01/16 16:32:17  vrandezo
// Mutter- und Zweitsprache bei Kultur einstellbar
//
// Revision 1.4  2003/01/11 20:30:58  vrandezo
// Auf wxWindows 2.4.0 gewechselt
// Beschreibungen bei Personenkomponenten und Talentoiden hinzugefügt
// PersonenkomponentenGUI verändert
//
// Revision 1.3  2002/11/05 21:49:19  vrandezo
// Sprachen und Schriften im- und exportierbar
// bool const ueberschreiben aus Speicher-Schnittstelle entfernt
// Mengen haben IDs erhalten
// Voraussetzungen für Talente (allgemeine und meisterliche)
// MengeGUIwx beim Wechseln von Objekten mit Rückfrage
//
// Revision 1.2  2002/11/05 16:55:15  vrandezo
// Schriften, SchriftenGUI und sonstige Klassen hinzugefügt
// detFrame-Menü leicht überarbeitet (Talente gruppiert)
// Sprachen und Schriften werden geladen und gespeichert
//
// Revision 1.1  2002/11/05 14:28:05  vrandezo
// Sprachen hinzugefügt
//
//@nonl
//@-node:<< Version History >>
//@nl
//@-node:@file SpracheSchrift.cpp
//@-leo
