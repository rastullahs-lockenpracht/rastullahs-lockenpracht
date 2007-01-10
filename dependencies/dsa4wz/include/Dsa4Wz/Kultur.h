//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file Kultur.h
//@+at
// Kultur.h: Schnittstelle für die Klasse CKultur.
// 
// $Revision: 1.8 $
// $Date: 2004/10/17 19:12:39 $
// 
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

#if !defined(KULTUR_H)
#define KULTUR_H

//@<< includes >>
//@+node:<< includes >>
#include <map>
#include "NDSABasis.h"
#include "PersonenKomponente.h"
#include "Texte.h"
#include "Menge.h"
//@nonl
//@-node:<< includes >>
//@nl

namespace DSA {

  class PersistenzManager;
  
	//@  << class Kultur >>
	//@+node:<< class Kultur >>
	class Kultur : public PersonenKomponente {
	public:
	  int Lese(PersistenzManager* pm, const std::string& id = "");
	  int Schreibe(PersistenzManager *pm) const;
	
	  bool operator==(const Kultur& other) const {
	    return ((PersonenKomponente::operator==(other))
	         && (moeglicheProfessionen_==other.moeglicheProfessionen_)
	         && (muttersprache_==other.muttersprache_)
	         && (zweitsprache_==other.zweitsprache_)); }
	  bool operator!=(const Kultur& other) const { return !operator==(other); }
	
	  // nach Demeter raus!
	  StringListe moeglicheProfessionen_;
	  StringListe muttersprache_;
	  StringListe zweitsprache_;
	};
	//@nonl
	//@-node:<< class Kultur >>
	//@nl
  
	//@  << class KulturDefinition >>
	//@+node:<< class KulturDefinition >>
	class KulturDefinition : public PersonenKomponenteDefinition {
	public:
	  KulturDefinition() : PersonenKomponenteDefinition(Klasse(), Verrechnungspunkteabzug()) {}
	  //KulturDefinition(const KulturDefinition& other) : PersonenKomponenteDefinition(other), moeglicheProfessionen_(other.moeglicheProfessionen_), muttersprache_(other.muttersprache_), zweitsprache_(other.zweitsprache_) {}
	  static std::string Klasse() { return ("Kultur"); }
	  static std::string Klassen() { return ("Kulturen"); }
	  static int Verrechnungspunkteabzug() { return 75; } // TODO Magische Konstante
	
	  bool operator==(const KulturDefinition& other) const {
	    return ((PersonenKomponenteDefinition::operator==(other))
	         && (moeglicheProfessionen_==other.moeglicheProfessionen_)
	         && (muttersprache_==other.muttersprache_)
	         && (zweitsprache_==other.zweitsprache_)); }
	  bool operator!=(const KulturDefinition& other) const { return !operator==(other); }
	
	  virtual std::string Text(const std::string& trennzeichenDerEinzelne = "\n\n", const std::string& zwischenEinzelneTitelUndSachen = ": ", const std::string& trennzeichenImEinzelnen = ", ", const std::string& wennEinzelnesLeer = "-") const;
	
	  int Lese(PersistenzManager* pm, const std::string& id = "");
	  int Schreibe(PersistenzManager *pm) const;
	
	  StringListe MoeglicheRassen() const;
	  StringListe UeblicheRassen() const;
	
	  // nach Demeter raus!
	  StringListe moeglicheProfessionen_;
	  StringListe muttersprache_;
	  StringListe zweitsprache_;
	}; // KulturDefinition
	//@-node:<< class KulturDefinition >>
	//@nl
  
  class KulturDefinitionen : public Menge<KulturDefinition> {
  }; // KulturDefinitionen

  KulturDefinitionen* kulturen();
}

#endif // !defined(KULTUR_H)

//@<< Version History >>
//@+node:<< Version History >>
// $Log: Kultur.h,v $
// Revision 1.8  2004/10/17 19:12:39  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.7  2004/04/12 21:08:09  vrandezo
// Lehrsprachen implementiert
// Mutter-/Zweitsprachensystem umgebaut
//
// Revision 1.6  2003/11/28 23:22:49  vrandezo
// speicherbare konkrete RKP
//
// Revision 1.4  2003/10/24 00:51:09  vrandezo
// Leonizing
// hoffentlich RKP-Verknüpfungsbug gelöst
//
// Revision 1.3  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.2  2003/02/19 17:45:08  vrandezo
// Kulturen zeigen mögliche und übliche Rassen an,
// Professionen mögliche Kulturen
//
// Revision 1.1.1.1  2003/02/17 15:32:54  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.16  2003/01/19 01:05:21  vrandezo
// Mutter- und Zweitsprachen bei Heldenerschaffung vollständig integriert
// Werte passen sich ständig der aktuellen Klugheit an
// Sprachen und Schriften in Heldenerschaffung integriert
// Bei möglicher Talentaktivierung werden nur passende Talente angezeigt
//
// Revision 1.15  2003/01/18 18:10:41  vrandezo
// Personen können Sprachen und Schriften
// übliche Kulturen bei Rassen hinzugefügt
// GUI für mögliche und übliche Kulturen und für mögliche Professionen geändert
// Varianten ausblendbar bei der Eingabe möglicher Kulturen und Professionen
//
// Revision 1.14  2003/01/16 22:57:35  vrandezo
// männliche und weibliche Bezeichnungen bei den Professionen
// Geschlechtsspezifische Personenkomponenten (als Varianten realisiert) implementiert
//
// Revision 1.13  2003/01/16 16:32:17  vrandezo
// Mutter- und Zweitsprache bei Kultur einstellbar
//
// Revision 1.12  2002/11/05 21:49:18  vrandezo
// Sprachen und Schriften im- und exportierbar
// bool const ueberschreiben aus Speicher-Schnittstelle entfernt
// Mengen haben IDs erhalten
// Voraussetzungen für Talente (allgemeine und meisterliche)
// MengeGUIwx beim Wechseln von Objekten mit Rückfrage
//
// Revision 1.11  2002/11/05 13:12:43  vrandezo
// Spezies bei den Rassen einegführt
// SortierteMenge verallgemeinert
//
// Revision 1.10  2002/11/02 23:36:57  vrandezo
// Exportier und Importierfunktionen implementiert
// Vergleichsoperationen implementiert
// Kampftechnikfenstter implementiert
//
// Revision 1.9  2002/10/27 17:29:20  vrandezo
// Die meisten Teile sind speicherbar
//
// Revision 1.8  2002/10/25 15:36:14  vrandezo
// PersonenkomponentenGUIs an neue MengenGUI angepasst
//
// Revision 1.7  2002/10/20 00:20:47  vrandezo
// Mögliche Kulturen und Professionen implementiert
//
// Revision 1.6  2002/10/19 18:58:32  vrandezo
// Texte in eigene, externe Datei untergebracht
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
//@-node:@file Kultur.h
//@-leo
