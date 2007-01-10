//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file Profession.cpp
//@+at
// Profession.cpp: Implementierung der Klasse CProfession.
// 
// $Revision: 1.17 $
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
#include "Profession.h"
#include "Kultur.h"
#include "Log.h"
#include "PersistenzManager.h"
#include "Texte.h"

#include <algorithm>
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

	//@  @+others
	//@+node:class Profession
	//@+others
	//@+node:Lese
	int Profession::Lese(PersistenzManager* pm, const std::string& id) {
	  std::string geleseneID = id;
	  int rv = pm->OeffneZumLesen("Profession", geleseneID);
	
	  std::string mBez = "";
	  if (pm->Existiert("geschlechtsspezifischeBezeichnung", "männlich"))
	    rv += pm->Lese("geschlechtsspezifischeBezeichnung", "männlich", mBez);
	  std::string wBez = "";
	  if (pm->Existiert(("geschlechtsspezifischeBezeichnung"), ("weiblich")))
	    rv += pm->Lese(("geschlechtsspezifischeBezeichnung"), ("weiblich"), wBez);
	//  bool zeitaufwendig = pm->Existiert(_t("zeitaufwendig"));
	//  bool erstprofession = pm->Existiert(_t("Erstprofession"));
	  int dauer = 0;
	  if (pm->Existiert(("Dauer")))
	    rv += pm->Lese(("Dauer"), dauer);
	  StringListe zweitsprache;
	  rv += pm->Lese(zweitsprache, ("Sprache"), ("Sprachen"), ("Zweitsprache"));
	  StringListe bekannteSprueche;
	  rv += pm->Lese(bekannteSprueche, ("Zauber"), ("Sprüche"), ("bekannte"));
	
	
	//@+at 
	//@nonl
	// kritische Stelle
	//  @c
	//     StringListe ueblicheKulturen;
	//     rv += pm->Lese(ueblicheKulturen, _t("Kultur"), 
	// _t("Kulturen"));
	//@-at
	//@@c
	
	  pm->SchliesseNachLesen();
	  rv += PersonenKomponente::Lese(pm, ("Profession"), geleseneID);
	
	  if (!rv) {
	//@+at 
	//@nonl
	// kritische Stelle
	//  @c
	//       StringListe::iterator si;
	//       for (si = ueblicheKulturen.begin(); si != 
	// ueblicheKulturen.end(); ++si) {
	//         DSA::KulturDefinition* k = 
	// DSA::kulturen()->Zeige(*si);
	//         if (k==0) continue;
	//         if 
	// (std::find(k->moeglicheProfessionen_.begin(), 
	// k->moeglicheProfessionen_.end(), 
	// ID())==k->moeglicheProfessionen_.end())
	//           k->moeglicheProfessionen_.push_back(ID());
	//         std::sort(k->moeglicheProfessionen_.begin(), 
	// k->moeglicheProfessionen_.end());
	//       }
	//@-at
	//@@c
	
	    MaennlicheBezeichnung(mBez);
	    WeiblicheBezeichnung(wBez);
	//    Erstprofession(erstprofession);
	//    Zeitaufwendig(zeitaufwendig);
	    zweitsprache_ = zweitsprache;
	    bekannteSprueche_ = bekannteSprueche;
	    Dauer(dauer);
	  }
	  return rv; // TODO
	} // Lese
	
	//@-node:Lese
	//@+node:Schreibe
	int Profession::Schreibe(PersistenzManager* pm) const {
	  int rv = PersonenKomponente::Schreibe(pm, ("Profession"));
	  rv += pm->OeffneZumSchreiben(("Profession"), ID());
	  if (mBez_!="")
	    rv += pm->Schreibe(("geschlechtsspezifischeBezeichnung"), ("männlich"), mBez_);
	  if (wBez_!="")
	    rv += pm->Schreibe(("geschlechtsspezifischeBezeichnung"), ("weiblich"), wBez_);
	//  if (Zeitaufwendig()) {
	//    rv += pm->OeffneZumSchreiben(_t("zeitaufwendig"));
	//    rv += pm->SchliesseNachSchreiben();
	//  }
	//  if (Erstprofession()) {
	//    rv += pm->OeffneZumSchreiben(_t("Erstprofession"));
	//    rv += pm->SchliesseNachSchreiben();
	//  }
	  if (Dauer()!=0)
	    rv += pm->Schreibe(("Dauer"), Dauer());
	  rv += pm->Schreibe(zweitsprache_, ("Sprache"), ("Sprachen"), ("Zweitsprache"));
	  rv += pm->Schreibe(bekannteSprueche_, ("Zauber"), ("Sprüche"), ("bekannte"));
	
	//@+at 
	//@nonl
	// kritische Stelle
	//  @c
	//     StringListe m = MoeglicheKulturen();
	//     rv += pm->Schreibe(m, _t("Kultur"), 
	// _t("Kulturen"));
	//@-at
	//@@c
	  rv += pm->SchliesseNachSchreiben();
	  return rv; // TODO
	} // Schreibe
	
	//@-node:Schreibe
	//@-others
	//@nonl
	//@-node:class Profession
	//@+node:class ProfessionDefinition
	//@+others
	//@+node:Zeitaufwendig
	void ProfessionDefinition::Zeitaufwendig(const bool z) {
	  if (zeitaufwendig_==z) return;
	  if (zeitaufwendig_)
	    dauer_ -= 3;
	  else {
	    erstprofession_ = false;
	    dauer_ += 3;
	  }
	  zeitaufwendig_ = z;
	} // Zeitaufwendig
	//@nonl
	//@-node:Zeitaufwendig
	//@+node:Erstprofession
	void ProfessionDefinition::Erstprofession(const bool e) {
	  if (erstprofession_==e) return;
	  if (!erstprofession_) Zeitaufwendig(false);
	  erstprofession_ = e;
	} // Erstprofession
	//@nonl
	//@-node:Erstprofession
	//@+node:Text
	std::string ProfessionDefinition::Text(const std::string& trennzeichenDerEinzelne, const std::string& zwischenEinzelneTitelUndSachen, const std::string& trennzeichenImEinzelnen, const std::string& wennEinzelnesLeer) const {
	  std::string s = kategorie_ + trennzeichenDerEinzelne;
	  s += PersonenKomponenteDefinition::Text(trennzeichenDerEinzelne, zwischenEinzelneTitelUndSachen, trennzeichenImEinzelnen, wennEinzelnesLeer);
	  if (zweitsprache_.size()!=0) s += trennzeichenDerEinzelne + _t("Zweitsprache") + zwischenEinzelneTitelUndSachen + liste2string(zweitsprache_, trennzeichenImEinzelnen, wennEinzelnesLeer);
	  return s;
	} // Text
	//@nonl
	//@-node:Text
	//@+node:MoeglicheKulturen
	StringListe ProfessionDefinition::MoeglicheKulturen() const {
	  StringListe sl;
	  for (KulturDefinitionen::iterator ki = kulturen()->begin(); ki != kulturen()->end(); ++ki)
	    if (std::find(ki->second.moeglicheProfessionen_.begin(), ki->second.moeglicheProfessionen_.end(), ID()) != ki->second.moeglicheProfessionen_.end())
	      sl.push_back(ki->first);
	  return sl;
	} // MoeglicheKulturen
	//@nonl
	//@-node:MoeglicheKulturen
	//@+node:Lese
	int ProfessionDefinition::Lese(PersistenzManager* pm, const std::string& id) {
	  std::string geleseneID = id;
	  int rv = pm->OeffneZumLesen(("Profession"), geleseneID);
	
	  std::string mBez = geleseneID;
	  if (pm->Existiert(("Bezeichnung"), ("männlich")))
	    rv += pm->Lese(("Bezeichnung"), ("männlich"), mBez);
	  std::string wBez = geleseneID;
	  if (pm->Existiert(("Bezeichnung"), ("weiblich")))
	    rv += pm->Lese(("Bezeichnung"), ("weiblich"), wBez);
	  bool zeitaufwendig = pm->Existiert(("zeitaufwendig"));
	  bool erstprofession = pm->Existiert(("Erstprofession"));
	  int dauer = 0;
	  if (pm->Existiert(("Dauer")))
	    rv += pm->Lese(("Dauer"), dauer);
	  StringListe zweitsprache;
	  rv += pm->Lese(zweitsprache, ("Sprache"), ("Sprachen"), ("Zweitsprache"));
	  StringListe bekannteSprueche;
	  rv += pm->Lese(bekannteSprueche, ("Zauber"), ("Sprüche"), ("bekannte"));
	
	
	//@+at 
	//@nonl
	// kritische Stelle
	//  @c
	//     StringListe ueblicheKulturen;
	//     rv += pm->Lese(ueblicheKulturen, _t("Kultur"), 
	// _t("Kulturen"));
	//@-at
	//@@c
	
	  pm->SchliesseNachLesen();
	  rv += PersonenKomponenteDefinition::Lese(pm, geleseneID);
	
	  if (!rv) {
	//@+at 
	//@nonl
	// kritische Stelle
	//  @c
	//       StringListe::iterator si;
	//       for (si = ueblicheKulturen.begin(); si != 
	// ueblicheKulturen.end(); ++si) {
	//         DSA::KulturDefinition* k = 
	// DSA::kulturen()->Zeige(*si);
	//         if (k==0) continue;
	//         if 
	// (std::find(k->moeglicheProfessionen_.begin(), 
	// k->moeglicheProfessionen_.end(), 
	// ID())==k->moeglicheProfessionen_.end())
	//           k->moeglicheProfessionen_.push_back(ID());
	//         std::sort(k->moeglicheProfessionen_.begin(), 
	// k->moeglicheProfessionen_.end());
	//       }
	//@-at
	//@@c
	
	    MaennlicheBezeichnung(mBez);
	    WeiblicheBezeichnung(wBez);
	    Erstprofession(erstprofession);
	    Zeitaufwendig(zeitaufwendig);
	    Dauer(dauer);
	    zweitsprache_ = zweitsprache;
	    bekannteSprueche_ = bekannteSprueche;
	  }
	  return rv; // TODO
	} // Lese
	//@nonl
	//@-node:Lese
	//@+node:Schreibe
	int ProfessionDefinition::Schreibe(PersistenzManager* pm) const {
	  int rv = PersonenKomponenteDefinition::Schreibe(pm);
	  rv += pm->OeffneZumSchreiben(Klasse(), ID());
	  if (mBez_!="")
	    rv += pm->Schreibe(("Bezeichnung"), ("männlich"), mBez_);
	  if (wBez_!="")
	    rv += pm->Schreibe(("Bezeichnung"), ("weiblich"), wBez_);
	  if (Zeitaufwendig()) {
	    rv += pm->OeffneZumSchreiben(("zeitaufwendig"));
	    rv += pm->SchliesseNachSchreiben();
	  }
	  if (Erstprofession()) {
	    rv += pm->OeffneZumSchreiben(("Erstprofession"));
	    rv += pm->SchliesseNachSchreiben();
	  }
	  if (Dauer()!=0)
	    rv += pm->Schreibe(("Dauer"), Dauer());
	  rv += pm->Schreibe(zweitsprache_, ("Sprache"), ("Sprachen"), ("Zweitsprache"));
	  rv += pm->Schreibe(bekannteSprueche_, ("Zauber"), ("Sprüche"), ("bekannte"));
	
	//@+at 
	//@nonl
	// kritische Stelle
	//  @c
	//     StringListe m = MoeglicheKulturen();
	//     rv += pm->Schreibe(m, _t("Kultur"), 
	// _t("Kulturen"));
	//@-at
	//@@c
	  rv += pm->SchliesseNachSchreiben();
	  return rv; // TODO
	} // Schreibe
	//@nonl
	//@-node:Schreibe
	//@-others
	
	//@-node:class ProfessionDefinition
	//@-others
  
  ProfessionDefinitionen* professionen() 
    { static ProfessionDefinitionen* professionen_ = new ProfessionDefinitionen(); return professionen_; }

}

//@<< Version History >>
//@+node:<< Version History >>
// $Log: Profession.cpp,v $
// Revision 1.17  2004/10/17 19:12:40  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.16  2004/04/12 23:15:50  vrandezo
// Professionen bieten bekannte Zaubersprueche mit
//
// Revision 1.14  2004/04/06 09:23:42  vrandezo
// Bug: Professionen ohne geschlechtsspezifische Bezeichnung
// wurden nicht mehr geladen (Twel)
//
// Revision 1.13  2004/04/04 17:29:27  vrandezo
// Re-Generierungs-bug gefixt
//
// Revision 1.12  2004/01/29 09:06:19  dertron
// doppelte CVS Log-Tags entfernt
//
// Revision 1.11  2003/11/28 23:22:49  vrandezo
// speicherbare konkrete RKP
//
// Revision 1.9  2003/10/24 19:21:07  vrandezo
// RPK-Verknüpfung von Grund auf erneuert
//
// Revision 1.8  2003/10/24 13:30:07  vrandezo
// DEBUG_NEW raus
// Varianten jetzt von Hand eintragbar
//
// Revision 1.7  2003/10/24 00:51:09  vrandezo
// Leonizing
// hoffentlich RKP-Verknüpfungsbug gelöst
//
// Revision 1.6  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.5  2003/10/17 23:44:01  vrandezo
// beschleunigtes Lesen und Schreiben von Professionen und Kulturen
//
// Revision 1.4  2003/02/20 16:03:45  vrandezo
// zeitaufwendig, Erstprofession, Ausbildungsdauer
// kleiner Bug bei möglichen Kulturen
// Verrechnungspunkte für SF und VSF werden eingerechnet
//
// Revision 1.3  2003/02/19 19:34:28  vrandezo
// Bug: Verrechnungspunkte für Schlechte Eigenschaften sind nun korrekt
// verbilligte SF bei Personen und RKP eingefügt
// RKP mit SF und VSF ausstattbar
//
// Revision 1.2  2003/02/19 17:45:12  vrandezo
// Kulturen zeigen mögliche und übliche Rassen an,
// Professionen mögliche Kulturen
//
// Revision 1.1.1.1  2003/02/17 15:34:02  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.15  2003/01/26 13:42:22  vrandezo
// Voraussetzungen bei Generierung gültig
// Auswahl aus einer Talentgruppe möglich bei Generierung
// Geschlecht bei Generierung implementiert
//
// Revision 1.14  2003/01/24 20:33:35  vrandezo
// übliche Kulturen auswählbar bei Heldenerschaffung
// Wert der Mutter- und Zweitsprache auch bei Modifikatoren korrekt
// Geschlecht wechselbar
//
// Revision 1.13  2003/01/18 18:10:42  vrandezo
// Personen können Sprachen und Schriften
// übliche Kulturen bei Rassen hinzugefügt
// GUI für mögliche und übliche Kulturen und für mögliche Professionen geändert
// Varianten ausblendbar bei der Eingabe möglicher Kulturen und Professionen
//
// Revision 1.12  2003/01/16 22:57:35  vrandezo
// männliche und weibliche Bezeichnungen bei den Professionen
// Geschlechtsspezifische Personenkomponenten (als Varianten realisiert) implementiert
//
// Revision 1.11  2003/01/15 20:02:30  vrandezo
// Voraussetzungen jetzt auch für Rassen und Professionen
// GUI für Modifikatoren bei Personenkomponenten verbessert
// Lesen von Modifikatoren erweitert
//
// Revision 1.10  2002/11/05 21:49:18  vrandezo
// Sprachen und Schriften im- und exportierbar
// bool const ueberschreiben aus Speicher-Schnittstelle entfernt
// Mengen haben IDs erhalten
// Voraussetzungen für Talente (allgemeine und meisterliche)
// MengeGUIwx beim Wechseln von Objekten mit Rückfrage
//
// Revision 1.9  2002/10/27 17:29:21  vrandezo
// Die meisten Teile sind speicherbar
//
// Revision 1.8  2002/10/20 00:20:47  vrandezo
// Mögliche Kulturen und Professionen implementiert
//
// Revision 1.7  2002/10/19 18:58:33  vrandezo
// Texte in eigene, externe Datei untergebracht
//
// Revision 1.6  2002/10/13 15:18:42  vrandezo
// Voraussetzungen für Professionen implementiert
// CVS aufgeräumt für Modifkatoren und Voraussetzungen
//
// Revision 1.5  2002/10/11 14:13:22  vrandezo
// Modifikatoren für Eigenschaften und Abgeleitete Werte eingeführt
//
// Revision 1.4  2002/10/10 21:41:38  vrandezo
// PersonenKomponente als Grundlage für Rasse, Kultur und Profession eingeführt
// Toten Code aus dem Projekt entfernt (noch nicht aus dem CVS)
//
// Revision 1.3  2002/10/09 19:58:34  vrandezo
// Auswahl von Rasse, Kultur, Profession ermöglicht
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

//@-node:@file Profession.cpp
//@-leo
