//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file Profession.h
//@+at
// Profession.h: Schnittstelle für die Klasse CProfession.
// 
// $Revision: 1.10 $
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

#if !defined(PROFESSION_H)
#define PROFESSION_H

//@<< includes >>
//@+node:<< includes >>
#include <map>
#include "NDSABasis.h"
#include "PersonenKomponente.h"
#include "Texte.h"
#include "Menge.h"
#include "Voraussetzung.h"
//@nonl
//@-node:<< includes >>
//@nl

namespace DSA {

  class PersistenzManager;
  
	//@  << class Profession >>
	//@+node:<< class Profession >>
	class Profession : public PersonenKomponente {
	public:
	  Profession() : dauer_(0) {};
	
	  std::string MaennlicheBezeichnung() const { return mBez_; }
	  void MaennlicheBezeichnung(const std::string& b) { mBez_ = b; }
	  std::string WeiblicheBezeichnung() const { return wBez_; }
	  void WeiblicheBezeichnung(const std::string& b) { wBez_ = b; }
	  std::string GeschlechtlichechtsspezifischeBezeichnung(const bool maennlich) const { return (maennlich?MaennlicheBezeichnung():WeiblicheBezeichnung()); }
	  void GeschlechtlichechtsspezifischeBezeichnung(const bool maennlich, const std::string& b) { if (maennlich) MaennlicheBezeichnung(b); else WeiblicheBezeichnung(b); }
	
	  int Dauer() const { return dauer_; }
	  void Dauer(const int d) { dauer_ = d; }
	
	  bool operator==(const Profession& other) const {
	  return ((PersonenKomponente::operator==(other))
	       && (mBez_==other.mBez_)
	       && (dauer_==other.dauer_)
	       && (wBez_==other.wBez_)
	       && (bekannteSprueche_==other.bekannteSprueche_)
	       && (zweitsprache_==other.zweitsprache_));
	  }
	  bool operator!=(const Profession& other) const { return !operator==(other); }
	
	  int Lese(PersistenzManager* pm, const std::string& id = "");
	  int Schreibe(PersistenzManager* pm) const;
	
	  StringListe zweitsprache_;
	  StringListe bekannteSprueche_;
	
	private:
	  std::string mBez_, wBez_;
	  int dauer_;
	};
	//@nonl
	//@-node:<< class Profession >>
	//@nl
  
	//@  << class ProfessionDefinition >>
	//@+node:<< class ProfessionDefinition >>
	class ProfessionDefinition : public PersonenKomponenteDefinition {
	public:
	  ProfessionDefinition() : PersonenKomponenteDefinition(Klasse(), Verrechnungspunkteabzug()), kategorie_(("Handwerkliche Profession")), zeitaufwendig_(false), erstprofession_(false), dauer_(0) {};
	  static std::string Klasse() { return ("Profession"); }
	  static std::string Klassen() { return ("Professionen"); }
	  static int Verrechnungspunkteabzug() { return 250; } // TODO Magische Konstante
	
	  typedef std::string Kriterium;
	  static std::string Kriteriumsart() { return ("Kategorie"); }
	  static std::string Kriteriumstring(const std::string& s) { return s; }
	  std::string ZuKriterium() const { return kategorie_; }
	  bool IstKriterium(const std::string& s) const { return s == kategorie_; }
	  void SetzeKriterium(const std::string& s) { kategorie_ = s; }
	  
	  std::string Kategorie() const { return kategorie_; }
	  void Kategorie(const std::string& k) { kategorie_ = k; }
	  std::string MaennlicheBezeichnung() const { return mBez_; }
	  void MaennlicheBezeichnung(const std::string& b) { mBez_ = b; }
	  std::string WeiblicheBezeichnung() const { return wBez_; }
	  void WeiblicheBezeichnung(const std::string& b) { wBez_ = b; }
	  std::string GeschlechtsspezifischeBezeichnung(const bool maennlich) const { return (maennlich?MaennlicheBezeichnung():WeiblicheBezeichnung()); }
	  void GeschlechtsspezifischeBezeichnung(const bool maennlich, const std::string& b) { if (maennlich) MaennlicheBezeichnung(b); else WeiblicheBezeichnung(b); }
	
	  bool Zeitaufwendig() const { return zeitaufwendig_; }
	  void Zeitaufwendig(const bool z);
	  bool Erstprofession() const { return erstprofession_; }
	  void Erstprofession(const bool e);
	  int Dauer() const { return dauer_; }
	  void Dauer(const int d) { dauer_ = d; }
	
	  bool operator==(const ProfessionDefinition& other) const {
	  return ((PersonenKomponenteDefinition::operator==(other))
	       && (kategorie_==other.kategorie_)
	       && (zeitaufwendig_==other.zeitaufwendig_)
	       && (erstprofession_==other.erstprofession_)
	       && (dauer_==other.dauer_)
	       && (mBez_==other.mBez_)
	       && (wBez_==other.wBez_)
	       && (bekannteSprueche_==other.bekannteSprueche_)
	       && (zweitsprache_==other.zweitsprache_));
	}
	  bool operator!=(const ProfessionDefinition& other) const { return !operator==(other); }
	
	  virtual std::string Text(const std::string& trennzeichenDerEinzelne = "\n\n", const std::string& zwischenEinzelneTitelUndSachen = ": ", const std::string& trennzeichenImEinzelnen = ", ", const std::string& wennEinzelnesLeer = "-") const;
	
	  StringListe MoeglicheKulturen() const;
	
	  int Lese(PersistenzManager* pm, const std::string& id = "");
	  int Schreibe(PersistenzManager* pm) const;
	
	  StringListe zweitsprache_;
	  StringListe bekannteSprueche_;
	
	private:
	  std::string kategorie_;
	  std::string mBez_, wBez_;
	  bool zeitaufwendig_;
	  bool erstprofession_;
	  int dauer_;
	}; // ProfessionDefinition
	//@nonl
	//@-node:<< class ProfessionDefinition >>
	//@nl
  
  class ProfessionDefinitionen : public SortierteMenge<ProfessionDefinition> {
  }; // ProfessionDefinitionen

  ProfessionDefinitionen* professionen();
}

#endif // !defined(PROFESSION_H)

//@<< Version History >>
//@+node:<< Version History >>
// $Log: Profession.h,v $
// Revision 1.10  2004/10/17 19:12:40  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.9  2004/04/12 23:15:50  vrandezo
// Professionen bieten bekannte Zaubersprueche mit
//
// Revision 1.7  2003/11/28 23:22:49  vrandezo
// speicherbare konkrete RKP
//
// Revision 1.5  2003/10/24 00:51:09  vrandezo
// Leonizing
// hoffentlich RKP-Verknüpfungsbug gelöst
//
// Revision 1.4  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.3  2003/02/20 16:03:45  vrandezo
// zeitaufwendig, Erstprofession, Ausbildungsdauer
// kleiner Bug bei möglichen Kulturen
// Verrechnungspunkte für SF und VSF werden eingerechnet
//
// Revision 1.2  2003/02/19 17:45:13  vrandezo
// Kulturen zeigen mögliche und übliche Rassen an,
// Professionen mögliche Kulturen
//
// Revision 1.1.1.1  2003/02/17 15:34:05  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.19  2003/01/26 20:10:46  vrandezo
// operator== für Voraussetzungen verbessert
// Datensätze umbenennen jetzt möglich
// gelöschte Daten werden aus der Sicht entfernt
//
// Revision 1.18  2003/01/26 13:42:22  vrandezo
// Voraussetzungen bei Generierung gültig
// Auswahl aus einer Talentgruppe möglich bei Generierung
// Geschlecht bei Generierung implementiert
//
// Revision 1.17  2003/01/18 18:10:42  vrandezo
// Personen können Sprachen und Schriften
// übliche Kulturen bei Rassen hinzugefügt
// GUI für mögliche und übliche Kulturen und für mögliche Professionen geändert
// Varianten ausblendbar bei der Eingabe möglicher Kulturen und Professionen
//
// Revision 1.16  2003/01/16 22:57:35  vrandezo
// männliche und weibliche Bezeichnungen bei den Professionen
// Geschlechtsspezifische Personenkomponenten (als Varianten realisiert) implementiert
//
// Revision 1.15  2003/01/16 19:35:07  vrandezo
// Variantenbeziehung eintragbar
// Professionen jetzt in die Kategorien aus dem Regelbuch unterteilt
// GUI für Geschlechtsspezifika steht schon
//
// Revision 1.14  2003/01/15 20:02:30  vrandezo
// Voraussetzungen jetzt auch für Rassen und Professionen
// GUI für Modifikatoren bei Personenkomponenten verbessert
// Lesen von Modifikatoren erweitert
//
// Revision 1.13  2002/11/05 21:49:18  vrandezo
// Sprachen und Schriften im- und exportierbar
// bool const ueberschreiben aus Speicher-Schnittstelle entfernt
// Mengen haben IDs erhalten
// Voraussetzungen für Talente (allgemeine und meisterliche)
// MengeGUIwx beim Wechseln von Objekten mit Rückfrage
//
// Revision 1.12  2002/11/05 13:12:43  vrandezo
// Spezies bei den Rassen einegführt
// SortierteMenge verallgemeinert
//
// Revision 1.11  2002/11/02 23:36:57  vrandezo
// Exportier und Importierfunktionen implementiert
// Vergleichsoperationen implementiert
// Kampftechnikfenstter implementiert
//
// Revision 1.10  2002/10/27 17:29:21  vrandezo
// Die meisten Teile sind speicherbar
//
// Revision 1.9  2002/10/25 15:36:15  vrandezo
// PersonenkomponentenGUIs an neue MengenGUI angepasst
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
// Revision 1.5  2002/10/11 14:13:23  vrandezo
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
//@-node:@file Profession.h
//@-leo
