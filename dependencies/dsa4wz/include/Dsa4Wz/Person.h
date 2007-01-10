//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file Person.h
//@+at
// Person.h: Schnittstelle für die Klasse CPerson.
// 
// $Revision: 1.15 $
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

#if !defined(PERSON_H)
#define PERSON_H

//@<< includes >>
//@+node:<< includes >>
#include "Eigenschaft.h"
#include "AbgeleiteterWert.h" 
#include "Rasse.h"
#include "Kultur.h"
#include "Profession.h"
#include "Talent.h"
#include "Charakteristikum.h"
#include "Sonderfertigkeit.h"
#include "Zauber.h"
//@nonl
//@-node:<< includes >>
//@nl

namespace DSA {
	//@  << class Person >>
	//@+node:<< class Person >>
	class Person {
	public:
		//@  << Administrativa >>
		//@+node:<< Administrativa >>
		Person();
		Person(const Person& other);
		~Person();
		
		bool operator==(const Person& other) const;
		bool operator!=(const Person& other) const { return !operator==(other); }
		
		static std::string Klasse() { return ("Person"); }
		static std::string Klassen() { return ("Personen"); }
		
		void ID(const std::string& id) { id_ = id; }
		std::string ID() const { return id_; }
		void Name(const std::string& name) { name_ = name; }
		std::string Name() const { return name_; }
		std::string Bezeichnung() const { return Name(); }
		void Bezeichnung(const std::string& bezeichnung) { Name(bezeichnung); }
		//@-node:<< Administrativa >>
		//@nl
	  
	  std::string ErsetzeTag(const std::string& tag) const;
	
	  void Beschreibung(const std::string& beschreibung) { beschreibung_ = beschreibung; }
	  std::string Beschreibung() const { return beschreibung_; }
	  int GP() const { return gp_; }
	  void GP(int g) { gp_ = g; }
	
	  void Rasse_(Rasse rasse) { rasse_ = rasse; }
	  Rasse Rasse_() const { return rasse_; }
	  void Kultur_(Kultur kultur) { kultur_ = kultur; }
	  Kultur Kultur_() const { return kultur_; }
	  void Profession_(Profession profession) { profession_ = profession; }
	  Profession Profession_() const { return profession_; }
	  
		//@  << Aussehen >>
		//@+node:<< Aussehen >>
		void Augenfarbe(const std::string& augenfarbe) { augenfarbe_ = augenfarbe; }
		std::string Augenfarbe() const { return augenfarbe_; }
		void Hauptfarbe(const std::string& hauptfarbe) { hauptfarbe_ = hauptfarbe; }
		std::string Hauptfarbe() const { return hauptfarbe_; }
		void Groesse(int groesse) { groesse_ = groesse; }
		int Groesse() const { return groesse_; }
		void Gewicht(int gewicht) { gewicht_ = gewicht; }
		int Gewicht() const { return gewicht_; }
		void Alter(int alter) { alter_ = alter; }
		int Alter() const { return alter_; }
		
		void Maennlich(bool m) { weiblich_ = !m; }
		bool Maennlich() const { return !weiblich_; }
		void Weiblich(bool w) { weiblich_ = w; }
		bool Weiblich() const { return weiblich_; }
		std::string Geschlecht() { return weiblich_ ? ("weiblich") : ("männlich"); }
		//@nonl
		//@-node:<< Aussehen >>
		//@nl
	  
		//@  << Lese/Schreibe >>
		//@+node:<< Lese/Schreibe >>
		int Lese(PersistenzManager* pm, const std::string& id = "");
		int Schreibe(PersistenzManager* pm) const;
		int Pruefe();
		static int IDListe(PersistenzManager* pm, StringListe* liste);
		//@nonl
		//@-node:<< Lese/Schreibe >>
		//@nl
	
	  int Wert(Eigenschaftsart e) const 
	    { return eigenschaft_[static_cast<int>(e)].Wert(); }
	  int Basiswert(Eigenschaftsart e) const 
	    { return eigenschaft_[static_cast<int>(e)].Basiswert(); }
	  int Wert(AbgeleiteterWertArt a) const 
	    { return abgeleiteterWert_[static_cast<int>(a)].Wert(); }
	  int Basiswert(AbgeleiteterWertArt a) const 
	    { return abgeleiteterWert_[static_cast<int>(a)].Basiswert(); }
	
	  // unbekannte Eigenschaft wenn undefiniert
	  Eigenschaftsart Leiteigenschaft() const; 
	  // 0 wenn undefiniert
	  int Leiteigenschaftswert() const;
	    
		//@  << Berechnete Info >>
		//@+node:<< Berechnete Info >>
		Charakteristika Vorteile() const;
		Charakteristika Nachteile() const;
		
		Talente Talente_(Talentgruppe gruppe) const;
		//@-node:<< Berechnete Info >>
		//@nl
	  
		//@  << Listen (sollten ob Demeter raus)>>
		//@+node:<< Listen (sollten ob Demeter raus)>>
		Eigenschaft eigenschaft_[AnzahlEigenschaften];
		AbgeleiteterWert abgeleiteterWert_[AnzahlAbgeleiteterWerte];
		Talente talente_;
		Sprachen sprachen_;
		Schriften schriften_;
		Kampftechniken kampftechniken_;
		Charakteristika charakteristika_;
		Sonderfertigkeiten sonderfertigkeiten_;
		StringListe verbilligteSonderfertigkeiten_;
		Zauberfertigkeiten zauberfertigkeiten_;
		//@nonl
		//@-node:<< Listen (sollten ob Demeter raus)>>
		//@nl
	
	//@<< private >>
	//@+node:<< private >>
	private:
	  std::string id_;
	  std::string name_;
	  std::string beschreibung_;
	  int gp_;
	  Rasse rasse_;
	  Kultur kultur_;
	  Profession profession_;
	  std::string augenfarbe_;
	  std::string hauptfarbe_;
	  int groesse_;
	  int gewicht_;
	  int alter_;
	  bool weiblich_;
	//@nonl
	//@-node:<< private >>
	//@nl
	
	}; // Person
	//@-node:<< class Person >>
	//@nl

  class Personen : public Menge<Person> {
  }; // Personen

  Personen* personen();
}

#endif // !defined(PERSON_H)

//@<< Version History >>
//@+node:<< Version History >>
// $Log: Person.h,v $
// Revision 1.15  2004/10/17 19:12:39  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.14  2004/04/09 22:25:50  vrandezo
// Zauberfertigkeiten fuer Personen eintragbar
//
// Revision 1.13  2003/11/28 16:46:16  vrandezo
// Zeilenvorlauf am Dateiende (Warnung bei gcc)
//
// Revision 1.12  2003/10/30 00:02:44  vrandezo
// Steigerbare Ritualkenntnisse
//
// Revision 1.11  2003/10/25 02:11:17  vrandezo
// generierte Sonderfertigkeiten
//
// Revision 1.10  2003/10/20 02:11:50  vrandezo
// IstMagisch?
//
// Revision 1.9  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.8  2003/03/27 14:28:42  vrandezo
// Personen haben eigene Start-GP
//
// Revision 1.7  2003/03/04 15:23:19  vrandezo
// weitere Tags implementiert für die Druckvorlagen
//
// Revision 1.6  2003/03/04 02:24:22  vrandezo
// Vorlagendateien implementiert
// if implementiert
//
// Revision 1.5  2003/02/24 17:37:31  vrandezo
// Prüfen implementiert
//
// Revision 1.4  2003/02/20 09:19:45  vrandezo
// Prüfe-Signatur geändert
//
// Revision 1.3  2003/02/20 00:21:58  vrandezo
// Sonderfertigkeiten vollständig integriert
// schlafende SF
//
// Revision 1.2  2003/02/19 19:34:28  vrandezo
// Bug: Verrechnungspunkte für Schlechte Eigenschaften sind nun korrekt
// verbilligte SF bei Personen und RKP eingefügt
// RKP mit SF und VSF ausstattbar
//
// Revision 1.1.1.1  2003/02/17 15:33:20  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.24  2003/02/14 21:42:50  vrandezo
// Sonderfertigkeiten hinzugefügt
//
// Revision 1.23  2003/02/08 00:22:54  vrandezo
// VN von RKP bei Generierung übernommen
// mehrfache VN bei RKP geben GP
// erweitere Voraussetzungen: nicht VN
// entfernbare VN
// auswählbare VN
// VN mit Voraussetzungen (nicht VN)
// Geschlecht neuerschaffener Helden ist zufällig
//
// Revision 1.22  2003/01/26 14:27:29  vrandezo
// Astralenergie auf 0 gesetzt
// Helden haben jetzt Raum für Notizen
//
// Revision 1.21  2003/01/18 18:10:42  vrandezo
// Personen können Sprachen und Schriften
// übliche Kulturen bei Rassen hinzugefügt
// GUI für mögliche und übliche Kulturen und für mögliche Professionen geändert
// Varianten ausblendbar bei der Eingabe möglicher Kulturen und Professionen
//
// Revision 1.20  2003/01/17 17:45:32  vrandezo
// Notebook-Bug  korrigiert (kam nur in der Release-Version vor)
// Varianten automatisch zugelassen, wenn Basis möglich
// Auswahl von Rasse, Profession, Kultur vereinfacht
// Talentauswahlauflösung implementiert (einfach und Liste)
//
// Revision 1.19  2002/12/06 15:28:00  vrandezo
// Spezies bei den Rassen einstellbar
// Kampftechniken bei PK und bei Person hinzugefügt
//
// Revision 1.18  2002/11/05 21:49:18  vrandezo
// Sprachen und Schriften im- und exportierbar
// bool const ueberschreiben aus Speicher-Schnittstelle entfernt
// Mengen haben IDs erhalten
// Voraussetzungen für Talente (allgemeine und meisterliche)
// MengeGUIwx beim Wechseln von Objekten mit Rückfrage
//
// Revision 1.17  2002/11/03 17:48:36  vrandezo
// Haarfarbe und Augenfarbe jetzt auch bei der Heldenerschaffung
//
// Revision 1.16  2002/11/02 23:36:57  vrandezo
// Exportier und Importierfunktionen implementiert
// Vergleichsoperationen implementiert
// Kampftechnikfenstter implementiert
//
// Revision 1.15  2002/10/25 17:51:06  vrandezo
// Modifikatoren für Personen
// GP-Kosten für Personenkomponenten veränderbar
//
// Revision 1.14  2002/10/25 14:21:12  vrandezo
// Hauptmenü erweitert
// Charakteristika hinzugefügt
// eigenes Mengen-GUI-Element erzeugt
//
// Revision 1.13  2002/10/19 18:58:33  vrandezo
// Texte in eigene, externe Datei untergebracht
//
// Revision 1.12  2002/10/17 10:54:44  vrandezo
// Talent und Talente eingefügt
//
// Revision 1.11  2002/10/11 14:13:20  vrandezo
// Modifikatoren für Eigenschaften und Abgeleitete Werte eingeführt
//
// Revision 1.10  2002/10/10 21:41:38  vrandezo
// PersonenKomponente als Grundlage für Rasse, Kultur und Profession eingeführt
// Toten Code aus dem Projekt entfernt (noch nicht aus dem CVS)
//
// Revision 1.9  2002/10/09 19:58:34  vrandezo
// Auswahl von Rasse, Kultur, Profession ermöglicht
//
// Revision 1.8  2002/10/02 01:28:57  vrandezo
// Erweiterung von GUI und DSA.lib
//
// Revision 1.7  2002/09/28 15:59:11  vrandezo
// Modifikatoren in GUI eingebunden
//
// Revision 1.6  2002/09/27 19:06:35  vrandezo
// Erweiterung von GUI und DSA.lib
//
// Revision 1.5  2002/09/26 14:52:20  vrandezo
// DSA.lib-Anbindung der GUI geschrieben
//
// Revision 1.4  2002/09/25 23:50:51  vrandezo
// Beginn der Aufteilung in DSA.lib und Anwendung
//
// Revision 1.3  2002/08/30 15:06:17  vrandezo
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
//@-node:@file Person.h
//@-leo
