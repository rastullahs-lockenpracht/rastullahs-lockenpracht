//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file Kultur.cpp
//@+at
// Kultur.cpp: Implementierung der Klasse CKultur.
// 
// $Revision: 1.13 $
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
#include "Kultur.h"
#include "Log.h"
#include "PersistenzManager.h"
#include "Texte.h"
#include "Rasse.h"
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
	//@+node:class Kultur
	//@+others
	//@+node:Lese
	int Kultur::Lese(PersistenzManager* pm, const std::string& id) {
	  std::string geleseneID = id;
	  int rv = pm->OeffneZumLesen(("Kultur"), geleseneID);
	
	  StringListe moeglicheProfessionen;
	  rv += pm->Lese(moeglicheProfessionen, ("Profession"), ("Professionen"));
	  StringListe muttersprache;
	  rv += pm->Lese(muttersprache, ("Sprache"), ("Sprachen"), ("Muttersprache"));
	  StringListe zweitsprache;
	  rv += pm->Lese(zweitsprache, ("Sprache"), ("Sprachen"), ("Zweitsprache"));
	
	  pm->SchliesseNachLesen();
	  rv += PersonenKomponente::Lese(pm, ("Kultur"), geleseneID);
	
	  if (!rv) {
	    moeglicheProfessionen_ = moeglicheProfessionen;
	    muttersprache_ = muttersprache;
	    zweitsprache_ = zweitsprache;
	  }
	  return rv; // TODO
	} // Lese
	//@-node:Lese
	//@+node:Schreibe
	int Kultur::Schreibe(PersistenzManager* pm) const {
	  int rv = PersonenKomponente::Schreibe(pm, ("Kultur"));
	  rv += pm->OeffneZumSchreiben(("Kultur"), ID());
	
	  rv += pm->Schreibe(moeglicheProfessionen_, ("Profession"), ("Professionen"));
	  rv += pm->Schreibe(muttersprache_, ("Sprache"), ("Sprachen"), ("Muttersprache"));
	  rv += pm->Schreibe(zweitsprache_, ("Sprache"), ("Sprachen"), ("Zweitsprache"));
	
	  rv += pm->SchliesseNachSchreiben();
	  return rv; // TODO
	} // Schreibe
	//@nonl
	//@-node:Schreibe
	//@-others
	//@nonl
	//@-node:class Kultur
	//@+node:class KulturDefinition
	//@+others
	//@+node:Text
	std::string KulturDefinition::Text(const std::string& trennzeichenDerEinzelne, const std::string& zwischenEinzelneTitelUndSachen, const std::string& trennzeichenImEinzelnen, const std::string& wennEinzelnesLeer) const {
	  std::string s;
	  s += PersonenKomponenteDefinition::Text(trennzeichenDerEinzelne, zwischenEinzelneTitelUndSachen, trennzeichenImEinzelnen, wennEinzelnesLeer);
	  s += trennzeichenDerEinzelne + _t("Muttersprache") + zwischenEinzelneTitelUndSachen + liste2string(muttersprache_, trennzeichenImEinzelnen, wennEinzelnesLeer);
	  if (zweitsprache_.size()!=0) s += trennzeichenDerEinzelne + _t("Zweitsprache") + zwischenEinzelneTitelUndSachen + liste2string(zweitsprache_, trennzeichenImEinzelnen, wennEinzelnesLeer);
	  s += trennzeichenDerEinzelne + _t("Mögliche Professionen") + zwischenEinzelneTitelUndSachen + liste2string(moeglicheProfessionen_, trennzeichenImEinzelnen, wennEinzelnesLeer);
	  return s;
	} // Text
	//@nonl
	//@-node:Text
	//@+node:MoeglicheRassen
	StringListe KulturDefinition::MoeglicheRassen() const {
	  StringListe sl;
	  for (RasseDefinitionen::iterator ri = rassen()->begin(); ri != rassen()->end(); ++ri)
	    if (std::find(ri->second.moeglicheKulturen_.begin(), ri->second.moeglicheKulturen_.end(), ID()) != ri->second.moeglicheKulturen_.end())
	      sl.push_back(ri->first);
	  return sl;
	} // MoeglicheRassen
	//@nonl
	//@-node:MoeglicheRassen
	//@+node:UeblicheRassen
	StringListe KulturDefinition::UeblicheRassen() const {
	  StringListe sl;
	  for (RasseDefinitionen::iterator ri = rassen()->begin(); ri != rassen()->end(); ++ri)
	    if (std::find(ri->second.ueblicheKulturen_.begin(), ri->second.ueblicheKulturen_.end(), ID()) != ri->second.ueblicheKulturen_.end())
	      sl.push_back(ri->first);
	  return sl;
	} // UeblicheRassen
	//@nonl
	//@-node:UeblicheRassen
	//@+node:Lese
	int KulturDefinition::Lese(PersistenzManager* pm, const std::string& id) {
	  std::string geleseneID = id;
	  int rv = pm->OeffneZumLesen(("Kultur"), geleseneID);
	
	  StringListe moeglicheProfessionen;
	  rv += pm->Lese(moeglicheProfessionen, ("Profession"), ("Professionen"));
	//@+at 
	//@nonl
	// kritische Stelle
	//  @c
	//     StringListe ueblicheRassen;
	//     rv += pm->Lese(ueblicheRassen, _t("Rasse"), 
	// _t("Rassen"), _t("übliche"));
	//     StringListe moeglicheRassen;
	//     rv += pm->Lese(moeglicheRassen, _t("Rasse"), 
	// _t("Rassen"), _t("mögliche"));
	//@-at
	//@@c
	  StringListe muttersprache;
	  rv += pm->Lese(muttersprache, ("Sprache"), ("Sprachen"), ("Muttersprache"));
	  StringListe zweitsprache;
	  rv += pm->Lese(zweitsprache, ("Sprache"), ("Sprachen"), ("Zweitsprache"));
	
	  pm->SchliesseNachLesen();
	  rv += PersonenKomponenteDefinition::Lese(pm, geleseneID);
	
	  if (!rv) {
	//@+at 
	//@nonl
	// kritische Stelle
	//  @c
	//       StringListe::iterator si;
	//       for (si = ueblicheRassen.begin(); si != 
	// ueblicheRassen.end(); ++si) {
	//         DSA::RasseDefinition* r = 
	// DSA::rassen()->Zeige(*si);
	//         if (r==0) continue;
	//         if (std::find(r->ueblicheKulturen_.begin(), 
	// r->ueblicheKulturen_.end(), 
	// ID())==r->ueblicheKulturen_.end())
	//           r->ueblicheKulturen_.push_back(ID());
	//         std::sort(r->ueblicheKulturen_.begin(), 
	// r->ueblicheKulturen_.end());
	//       }
	//       for (si = moeglicheRassen.begin(); si != 
	// moeglicheRassen.end(); ++si) {
	//         DSA::RasseDefinition* r = 
	// DSA::rassen()->Zeige(*si);
	//         if (r==0) continue;
	//         if (std::find(r->moeglicheKulturen_.begin(), 
	// r->moeglicheKulturen_.end(), 
	// ID())==r->moeglicheKulturen_.end())
	//         r->moeglicheKulturen_.push_back(ID());
	//         std::sort(r->moeglicheKulturen_.begin(), 
	// r->moeglicheKulturen_.end());
	//       }
	//@-at
	//@@c
	
	    moeglicheProfessionen_ = moeglicheProfessionen;
	    muttersprache_ = muttersprache;
	    zweitsprache_ = zweitsprache;
	  }
	  return rv; // TODO
	} // Lese
	//@nonl
	//@-node:Lese
	//@+node:Schreibe
	int KulturDefinition::Schreibe(PersistenzManager* pm) const {
	  int rv = PersonenKomponenteDefinition::Schreibe(pm);
	  rv += pm->OeffneZumSchreiben(Klasse(), ID());
	
	  rv += pm->Schreibe(moeglicheProfessionen_, ("Profession"), ("Professionen"));
	  
	//@+at 
	//@nonl
	// kritische Stelle
	//  @c
	//   StringListe u = UeblicheRassen();
	//   rv += pm->Schreibe(u, _t("Rasse"), _t("Rassen"), 
	// _t("übliche"));
	//   StringListe m = MoeglicheRassen();
	//   rv += pm->Schreibe(m, _t("Rasse"), _t("Rassen"), 
	// _t("mögliche"));
	//@-at
	//@@c
	  rv += pm->Schreibe(muttersprache_, ("Sprache"), ("Sprachen"), ("Muttersprache"));
	  rv += pm->Schreibe(zweitsprache_, ("Sprache"), ("Sprachen"), ("Zweitsprache"));
	
	  rv += pm->SchliesseNachSchreiben();
	  return rv; // TODO
	} // Schreibe
	//@nonl
	//@-node:Schreibe
	//@-others
	//@nonl
	//@-node:class KulturDefinition
	//@-others
  
  KulturDefinitionen* kulturen() 
    { static KulturDefinitionen* kulturen_ = new KulturDefinitionen(); return kulturen_; }

}

//@<< Version History >>
//@+node:<< Version History >>
// $Log: Kultur.cpp,v $
// Revision 1.13  2004/10/17 19:12:39  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.12  2004/01/29 09:06:18  dertron
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
// Revision 1.3  2003/02/19 19:34:27  vrandezo
// Bug: Verrechnungspunkte für Schlechte Eigenschaften sind nun korrekt
// verbilligte SF bei Personen und RKP eingefügt
// RKP mit SF und VSF ausstattbar
//
// Revision 1.2  2003/02/19 17:45:06  vrandezo
// Kulturen zeigen mögliche und übliche Rassen an,
// Professionen mögliche Kulturen
//
// Revision 1.1.1.1  2003/02/17 15:32:52  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.11  2003/01/18 18:10:41  vrandezo
// Personen können Sprachen und Schriften
// übliche Kulturen bei Rassen hinzugefügt
// GUI für mögliche und übliche Kulturen und für mögliche Professionen geändert
// Varianten ausblendbar bei der Eingabe möglicher Kulturen und Professionen
//
// Revision 1.10  2003/01/16 16:32:16  vrandezo
// Mutter- und Zweitsprache bei Kultur einstellbar
//
// Revision 1.9  2002/11/05 21:49:18  vrandezo
// Sprachen und Schriften im- und exportierbar
// bool const ueberschreiben aus Speicher-Schnittstelle entfernt
// Mengen haben IDs erhalten
// Voraussetzungen für Talente (allgemeine und meisterliche)
// MengeGUIwx beim Wechseln von Objekten mit Rückfrage
//
// Revision 1.8  2002/11/05 13:12:43  vrandezo
// Spezies bei den Rassen einegführt
// SortierteMenge verallgemeinert
//
// Revision 1.7  2002/10/27 17:29:20  vrandezo
// Die meisten Teile sind speicherbar
//
// Revision 1.6  2002/10/20 00:20:47  vrandezo
// Mögliche Kulturen und Professionen implementiert
//
// Revision 1.5  2002/10/11 14:13:20  vrandezo
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
// Revision 1.1  2002/07/17 18:26:02  Zdenko Vrandecic
// Einchecken
//
//@nonl
//@-node:<< Version History >>
//@nl
//@-node:@file Kultur.cpp
//@-leo
