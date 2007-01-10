//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file Talent.h
//@+at
// Talent.h: Implementierung für Talente
// 
// $Revision: 1.12 $
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

#if !defined(TALENT_H)
#define TALENT_H

//@<< includes >>
//@+node:<< includes >>
#include "Definition.h"
#include "NDSABasis.h"
#include "Menge.h"
#include "Auswahl.h"
#include "Regeln.h"
#include "Voraussetzung.h"
//@-node:<< includes >>
//@nl

namespace DSA {
  class PersistenzManager;
    
	//@  @+others
	//@+node:TalentDefinition
	class TalentDefinition : public Definition {
	public:
		//@  << Verwaltung >>
		//@+node:<< Verwaltung >>
		TalentDefinition(const std::string& id = "")
		  : Definition(id), allgemeineVoraussetzungen_(("allgemein")), meisterlicheVoraussetzungen_(("meisterlich")), eBE_(KeineBE) {}
		static std::string Klasse() { return ("Talent"); }
		virtual std::string VirtuelleKlasse() const { return Klasse(); }
		static std::string Klassen() { return ("Talente"); }
		
		typedef Talentgruppe Kriterium;
		static std::string Kriteriumsart() { return ("Talentgruppe"); }
		static std::string Kriteriumstring(Talentgruppe gruppe) { return TalentgruppeString(gruppe, false, false); }
		static Talentgruppe Kriteriumstring(const std::string& s) { return TalentgruppeString(s); }
		Talentgruppe ZuKriterium() const { return Gruppe(); }
		bool IstKriterium(Talentgruppe gruppe) const { return Gruppe() == gruppe; }
		void SetzeKriterium(Talentgruppe gruppe) { Gruppe(gruppe); }
		//@nonl
		//@-node:<< Verwaltung >>
		//@nl
	  
	  void Eigenschaften(Eigenschaftsart, Eigenschaftsart, Eigenschaftsart);
	  Eigenschaftsart Eigenschaft(int) const;
	  void Art(Talentart art) { art_ = art; }
	  Talentart Art() const { return art_; }
	  void Gruppe(Talentgruppe gruppe) { gruppe_ = gruppe; }
	  Talentgruppe Gruppe() const { return gruppe_; }
	  SKTSpalte SKTSpalte_() const { return regeln()->TalentgruppenSKT(gruppe_); }
	  void EffektiveBehinderung(EBe ebe) { eBE_ = ebe; }
	  EBe EffektiveBehinderung() const { return eBE_; }
	  
		//@  << operator== >>
		//@+node:<< operator== >>
		bool operator!=(const TalentDefinition& other) const { return !operator==(other); }
		bool operator==(const TalentDefinition& other) const
		{ return ((Definition::operator==(other))
			      && (art_==other.art_)
						&& (gruppe_==other.gruppe_)
						&& (eigenschaft_[0]==other.eigenschaft_[0])
						&& (eigenschaft_[1]==other.eigenschaft_[1])
						&& (eigenschaft_[2]==other.eigenschaft_[2])
		        && (eBE_==other.eBE_)
		        && (spezialisierungen_==other.spezialisierungen_)
						&& (allgemeineVoraussetzungen_==other.allgemeineVoraussetzungen_)
						&& (meisterlicheVoraussetzungen_==other.meisterlicheVoraussetzungen_));
			} // TODO AnzahlEigenschaftenproTalenprobe ist hier fest eingegeben
		//@nonl
		//@-node:<< operator== >>
		//@nl
	  
	  std::string ProbeText(const std::string& trennzeichen = "/", bool uebersetzt = true) const;
	
	  // std::string Text() const;
	
	  int Lese(PersistenzManager* pm, const std::string& id = "");
	  int Schreibe(PersistenzManager* pm) const;
	  int Pruefe();
	  static int IDListe(PersistenzManager* pm, StringListe* liste);
	  
	  Voraussetzungen meisterlicheVoraussetzungen_;
	  Voraussetzungen allgemeineVoraussetzungen_;
	    
		//@  << Spezialisierungen >>
		//@+node:<< Spezialisierungen >>
		//@+at
		// Gibt eine Liste der Bezeichner der verfügbaren 
		// Spezialisierungen zurück, in der zur Zeit 
		// eingestellten Sprache.
		//@-at
		//@@c
		StringListe Spezialisierungen() const;
		
		//@+at
		// Setzt eine neue Spezialisierung hinein. Name wird 
		// als der Bezeichner fuer die zur Zeit eingestellte 
		// Sprache verwendet, und aus ihm wird auch die ID 
		// erstellt.
		//@-at
		//@@c
		void SpezialisierungHinzu(const std::string& name);
		
		//@+at
		// Löscht eine Spezialisierung über den Bezeichner.
		//@-at
		//@@c
		void SpezialisierungWeg(const std::string& name);
		
		//@+at
		// Benennt eine Spezialsierung um. Hierbei wird der 
		// alte Name und der neue Name angegeben.
		//@-at
		//@@c
		void SpezialisierungUmbenennen(const std::string& alterName, const std::string& neuerName);
		//@nonl
		//@-node:<< Spezialisierungen >>
		//@nl
	  
	//@<<private >>
	//@+node:<<private >>
	private:
	  DDict spezialisierungen_;
		Eigenschaftsart eigenschaft_[EigenschaftenProTalentprobe];
		Talentart art_;
		Talentgruppe gruppe_;
	  EBe eBE_;
	//@nonl
	//@-node:<<private >>
	//@nl
	}; // TalentDefinition
	//@-node:TalentDefinition
	//@+node:Talent
	class Talent {
	public:
	  Talent(const std::string& id = "", int wert = 0) : person_(0), wert_(wert), id_(id) {}
	  Talent(const TalentDefinition& def, int wert = 0) : person_(0), wert_(wert), id_(def.ID()) {}
	  Talent(const TalentDefinition* const def, int wert = 0) : person_(0), wert_(wert), id_(def->ID()) {}
	  void Bezugsperson(Person* p) { person_=p; }
	
	  static std::string Klasse() { return ("Talent"); }
	  static std::string Klassen() { return ("Talente"); }
	
	  typedef Talentgruppe Kriterium;
	  static std::string Kriteriumsart() { return ("Talentgruppe"); }
	  static std::string Kriteriumstring(Talentgruppe gruppe) { return TalentgruppeString(gruppe, false); }
	  static Talentgruppe Kriteriumstring(std::string s) { return TalentgruppeString(s); }
	  Talentgruppe ZuKriterium() const { return Gruppe(); }
	  bool IstKriterium(Talentgruppe gruppe) const { return Gruppe() == gruppe; }
	  void SetzeKriterium(Talentgruppe gruppe) {}
	  
	  void ID(const std::string& id) { id_ = id; }
	  std::string ID() const { return id_; }
	  void Wert(int wert)	{ wert_ = wert; }
	  int Wert() const { return wert_; }
	  int Maximalwert() const;
	  void Extra(const std::string& extra);
	
	  std::string ErsetzeTag(const std::string& tag) const;
	
	  const TalentDefinition* Definition() const;
	
	  bool operator==(const Talent& other) const
	    { return ((id_==other.id_) && (wert_==other.wert_)); }
	  bool operator!=(const Talent& other) const { return !operator==(other); }
	
	  Eigenschaftsart Eigenschaft(int e) const { return Definition()->Eigenschaft(e); }
	  Talentart Art() const { return Definition()->Art(); }
	  Talentgruppe Gruppe() const { return Definition()->Gruppe(); }
	  SKTSpalte SKTSpalte_() const;
	  std::string Text() const;
	
	  int Lese(PersistenzManager* pm, const std::string& id = "");
	  int Schreibe(PersistenzManager* pm) const;
	  int Pruefe();
	  static int IDListe(PersistenzManager* pm, StringListe* liste);
	  
	private:
	  std::string id_;
	  int wert_;
	  Person* person_;
	}; // Talent
	//@nonl
	//@-node:Talent
	//@-others
  
  class TalentDefinitionen : public SortierteMenge<TalentDefinition> {
  }; // TalentDefinitionen

  TalentDefinitionen* talente();

  class Person;
  
  typedef SortierteMenge<Talent> Talente;
  //class Talente : public SortierteMenge<Talent> {
  //}; // Talente

  class TalenteAuswahl : public AuswahlMenge<Talent> {
  }; // TalenteAuswahl

}

#endif // !defined(TALENT_H)

//@<< Version History >>
//@+node:<< Version History >>
// $Log: Talent.h,v $
// Revision 1.12  2004/10/17 19:12:40  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.11  2004/04/13 14:06:52  vrandezo
// Hauszauber implementiert
//
// Revision 1.10  2004/04/06 13:57:59  vrandezo
// Begabung und Unfaehigkeit von Talenten und Talentgruppen jetzt
// nach Errata implementiert (Twel)
//
// Revision 1.9  2003/12/06 15:01:29  vrandezo
// in LEO eingearbeitet
//
// Revision 1.8  2003/11/28 16:46:16  vrandezo
// Zeilenvorlauf am Dateiende (Warnung bei gcc)
//
// Revision 1.7  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.6  2003/03/25 09:40:18  vrandezo
// Begabung für [Talentgruppe]
//
// Revision 1.5  2003/03/08 16:26:33  vrandezo
// BE bei Talenten und Kampftechniken (Gerhard Mayer), mit GUI
// Mögliche Spezialisierungen von Talenten
//
// Revision 1.4  2003/03/04 15:23:21  vrandezo
// weitere Tags implementiert für die Druckvorlagen
//
// Revision 1.3  2003/02/24 17:37:45  vrandezo
// Prüfen implementiert
//
// Revision 1.2  2003/02/20 09:19:45  vrandezo
// Prüfe-Signatur geändert
//
// Revision 1.1.1.1  2003/02/17 15:34:33  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.18  2003/01/26 20:10:49  vrandezo
// operator== für Voraussetzungen verbessert
// Datensätze umbenennen jetzt möglich
// gelöschte Daten werden aus der Sicht entfernt
//
// Revision 1.17  2003/01/13 00:30:49  vrandezo
// Release Version 1.12 dev
// Korrektur der Verrechnungspunkteberechnung für negative Talentmodifikatoren
// Talentauswahl implementiert
//
// Revision 1.16  2003/01/11 20:30:58  vrandezo
// Auf wxWindows 2.4.0 gewechselt
// Beschreibungen bei Personenkomponenten und Talentoiden hinzugefügt
// PersonenkomponentenGUI verändert
//
// Revision 1.15  2002/11/05 21:49:19  vrandezo
// Sprachen und Schriften im- und exportierbar
// bool const ueberschreiben aus Speicher-Schnittstelle entfernt
// Mengen haben IDs erhalten
// Voraussetzungen für Talente (allgemeine und meisterliche)
// MengeGUIwx beim Wechseln von Objekten mit Rückfrage
//
// Revision 1.14  2002/11/05 16:24:38  vrandezo
// MengeGUIwx vereinfacht (nur ein Template-Parameter)
// SpracheGUIwx hinzugefügt
// SortierteMenge erweitert um Zeige
//
// Revision 1.13  2002/11/05 14:28:05  vrandezo
// Sprachen hinzugefügt
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
// Revision 1.10  2002/11/02 19:01:11  vrandezo
// Exportfenster hinzugefügt
//
// Revision 1.9  2002/11/02 15:39:48  vrandezo
// Importfenster hinzugefügt
//
// Revision 1.8  2002/11/01 15:27:29  vrandezo
// Regeln neu implementiert
// ILader, MLader, IMLader, ILadbar und weitere Klassen entfernt
// XMLChar-Transcoding beschleunigt
//
// Revision 1.7  2002/10/27 17:29:21  vrandezo
// Die meisten Teile sind speicherbar
//
// Revision 1.6  2002/10/25 14:21:12  vrandezo
// Hauptmenü erweitert
// Charakteristika hinzugefügt
// eigenes Mengen-GUI-Element erzeugt
//
// Revision 1.5  2002/10/20 14:25:12  vrandezo
// Eigenes Talentfenster eingeführt
//
// Revision 1.4  2002/10/19 18:58:33  vrandezo
// Texte in eigene, externe Datei untergebracht
//
// Revision 1.3  2002/10/18 00:50:16  vrandezo
// Talente bei der Heldenerschaffung einsetzbar
//
// Revision 1.2  2002/10/17 15:35:58  vrandezo
// TalentGUI hinzugefügt
//
// Revision 1.1  2002/10/17 10:54:44  vrandezo
// Talent und Talente eingefügt
//
//@nonl
//@-node:<< Version History >>
//@nl
//@-node:@file Talent.h
//@-leo
