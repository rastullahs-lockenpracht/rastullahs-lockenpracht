//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file Charakteristikum.h
//@+at
// Charakteristikum.h: Implementierung für Charakteristika
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

#if !defined(CHARAKTERISTIKUM_H)
#define CHARAKTERISTIKUM_H

//@<< includes >>
//@+node:<< includes >>
#include "Menge.h"
#include "Voraussetzung.h"
#include "Auswahl.h"

#include "Definition.h"
//@nonl
//@-node:<< includes >>
//@nl

namespace DSA {
	class PersistenzManager;
  class Person;

	//@  @+others
	//@+node:class CharakteristikumDefinition
	class CharakteristikumDefinition : public Definition {
	public:
		//@  << Administrativa >>
		//@+node:<< Administrativa >>
		CharakteristikumDefinition(const std::string& id = "") :
		  Definition(id),
		  gp_(0),
		  gebunden_(false),
		  unsichtbar_(false),
		  hatStufen_(false),
		  stufe_(0),
		  gpPro2Punkte_(false),
		  schlechteEigenschaft_(false),
		  brauchtSpezifizierung_(false),
		  generiert_(false) {}
		  
		static std::string Klasse() { return ("Charakteristikum"); }
		virtual std::string VirtuelleKlasse() const { return Klasse(); }
		static std::string Klassen() { return ("Charakteristika"); }
		
		std::string Text(const Person& p) const;
		//@-node:<< Administrativa >>
		//@nl
	
	  std::string Bezeichnung(const std::string& sprache = DSA::Lang()) const;
	  using Definition::Bezeichnung;
	  
		void GP(int gp) { gp_ = gp; }
		int GP() const { return gp_; }
	
	  bool Gebunden() const { return gebunden_; }
	  void Gebunden(const bool g) { gebunden_ = g; }
	  bool Unsichtbar() const { return unsichtbar_; }
	  void Unsichtbar(const bool u) { unsichtbar_ = u; }
	  bool HatStufen() const { return hatStufen_; }
	  void HatStufen(const bool s) { hatStufen_ = s; }
	  int MaximalStufe() const  { return stufe_; }
	  void MaximalStufe(const int s) { stufe_ = s; }
	  bool SchlechteEigenschaft() const { return schlechteEigenschaft_; }
	  void SchlechteEigenschaft(const bool s) { schlechteEigenschaft_ = s; }
	  bool GPPro2Punkte() const { return gpPro2Punkte_; }
	  void GPPro2Punkte(const bool g) { gpPro2Punkte_ = g; }
	  bool BrauchtSpezifizierung() const { return brauchtSpezifizierung_; }
	  void BrauchtSpezifizierung(const bool b) { brauchtSpezifizierung_ = b; }
	
	  bool Hinzufuegbar(const Person&) const;
	  
		//@  << Semantische Abfragen >>
		//@+node:<< Semantische Abfragen >>
		bool Vorteil() const { return gp_>0; }
		bool Nachteil() const { return gp_<0; }
		
		bool IstGeneriert() const { return generiert_; }
		void IstGeneriert(bool g) { generiert_ = g; }
		
		bool IstHerausragendeEigenschaft() const;
		Eigenschaftsart HerausragendeEigenschaft() const;
		bool IstMiserableEigenschaft() const;
		Eigenschaftsart MiserableEigenschaft() const;
		
		bool IstBegabungTalent() const;
		std::string BegabungTalent() const;
		bool IstUnfaehigkeitTalent() const;
		std::string UnfaehigkeitTalent() const;
		bool IstBegabungTalentgruppe() const;
		Talentgruppe BegabungTalentgruppe() const;
		bool IstUnfaehigkeitTalentgruppe() const;
		Talentgruppe UnfaehigkeitTalentgruppe() const;
		bool IstUnfaehigkeitSprachen() const;
		//@-node:<< Semantische Abfragen >>
		//@nl
	
	  std::string BasiertAuf() const { return basiertAuf_; }
	  void BasiertAuf(const std::string& b) { basiertAuf_ = b; }
	  bool HatBasis() const { return BasiertAuf()!=""; }
	  std::string BasisFuer() const;
	  bool IstBasis() const { return BasisFuer()!=""; }
	  
		//@  << operator== >>
		//@+node:<< operator== >>
		bool operator==(const CharakteristikumDefinition& other) const {
			return ((Definition::operator==(other))
		       && (hatStufen_==other.hatStufen_)
		       && (stufe_==other.stufe_)
		       && (basiertAuf_==other.basiertAuf_)
		       && (voraussetzungen_==other.voraussetzungen_)
		       && (gebunden_==other.gebunden_)
		       && (unsichtbar_==other.unsichtbar_)
		       && (gpPro2Punkte_==other.gpPro2Punkte_)
		       && (schlechteEigenschaft_==other.schlechteEigenschaft_)
		       && (brauchtSpezifizierung_==other.brauchtSpezifizierung_)
		       && (gp_==other.gp_)); }
		bool operator!=(const CharakteristikumDefinition& other) const { return !operator==(other); }
		//@nonl
		//@-node:<< operator== >>
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
	  
	  Voraussetzungen voraussetzungen_;
		
	private:
		//@  << private >>
		//@+node:<< private >>
		int gp_;
		std::string basiertAuf_;
		bool gebunden_;
		bool unsichtbar_;
		int stufe_;
		bool hatStufen_;
		bool schlechteEigenschaft_;
		bool gpPro2Punkte_;
		bool brauchtSpezifizierung_;
		bool generiert_;
		//@nonl
		//@-node:<< private >>
		//@nl
	}; // CharakteristikumDefinition
	//@-node:class CharakteristikumDefinition
	//@+node:class Charakteristikum
	class Charakteristikum {
	public:
		//@  << Administrativa >>
		//@+node:<< Administrativa >>
		Charakteristikum(const std::string& id = "") : id_(id), stufe_(0), spezifizierung_("") {}
		Charakteristikum(const CharakteristikumDefinition& def) : id_(def.ID()), stufe_((def.HatStufen()?1:0)) {}
		Charakteristikum(const CharakteristikumDefinition* const def) : id_(def->ID()), stufe_((def->HatStufen()?1:0)) {}
		static std::string Klasse() { return ("Charakteristikum"); }
		static std::string Klassen() { return ("Charakteristika"); }
		
		void ID(const std::string& id) { id_ = id; }
		std::string ID() const { return id_; }
		//@-node:<< Administrativa >>
		//@nl
	  
	  std::string ErsetzeTag(const std::string& tag) const;
	
	  bool Vorteil() const { return Definition()->Vorteil(); }
	  bool Nachteil() const { return Definition()->Nachteil(); }
	  bool SchlechteEigenschaft() const { return Definition()->SchlechteEigenschaft(); }
	  int GPKosten(const Person&) const;
	  bool HatStufen() const { return Definition()->HatStufen(); }
	  int Stufe() const { return stufe_; }
	  void Stufe(const int s) { stufe_ = s; }
	  std::string Spezifizierung() const { return spezifizierung_; }
	  void Spezifizierung(const std::string& s) { spezifizierung_ = s; }
	
		const CharakteristikumDefinition* Definition() const;
	
		bool operator==(const Charakteristikum& other) const {
	    return ((id_==other.id_)
	         && (spezifizierung_==other.spezifizierung_)
	         && (stufe_==other.stufe_)); }
		bool operator!=(const Charakteristikum& other) const { return !operator==(other); }
	
		std::string Text() const;
	  
		//@  << Lese/Schreibe >>
		//@+node:<< Lese/Schreibe >>
		int Lese(PersistenzManager* pm, const std::string& id = "");
		int Schreibe(PersistenzManager* pm) const;
		int Pruefe();
		static int IDListe(PersistenzManager* pm, StringListe* liste);
		//@nonl
		//@-node:<< Lese/Schreibe >>
		//@nl
	  
	private:
	  std::string id_;
	  int stufe_;
	  std::string spezifizierung_;
	}; // Charakteristikum
	//@nonl
	//@-node:class Charakteristikum
	//@+node:class Charakteristika
	class Charakteristika : public Menge<Charakteristikum> {
	public:
	  bool Verfuegt(const std::string& id) const;
	  Charakteristika Sichtbar() const;
	//		bool FuegeEin(Modifikator modifikator);
	}; // Charakteristika
	//@nonl
	//@-node:class Charakteristika
	//@-others
  
	class CharakteristikumDefinitionen : public Menge<CharakteristikumDefinition> {};

	CharakteristikumDefinitionen* charakteristika();

  CharakteristikumDefinitionen* generierteCharakteristika();
  void GeneriereCharakteristika();
 
  class CharakteristikaAuswahl : public AuswahlMenge<Charakteristikum> {};
  
}

#endif // !defined(CHARAKTERISTIKUM_H)

//@<< Version History >>
//@+node:<< Version History >>
// $Log: Charakteristikum.h,v $
// Revision 1.15  2004/10/17 19:12:39  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.14  2004/01/05 02:45:29  vrandezo
// Charakteristika bei RPK-Definition von Mengen auf Auswahlmengen umgestellt
//
// Revision 1.13  2003/11/28 16:46:16  vrandezo
// Zeilenvorlauf am Dateiende (Warnung bei gcc)
//
// Revision 1.12  2003/10/27 18:24:50  vrandezo
// unsichtbare Vor- und Nachteile
// Tänze fremder Repräsentationen teurer
//
// Revision 1.11  2003/10/25 22:33:27  vrandezo
// SF-Text
//
// Revision 1.10  2003/10/25 15:06:48  vrandezo
// Leonizing
// Charakteristika mit Spezifizierung
//
// Revision 1.9  2003/10/25 02:11:17  vrandezo
// generierte Sonderfertigkeiten
//
// Revision 1.8  2003/10/24 23:50:41  vrandezo
// Leonizing
// Vor-/Nachteile als Voraussetzung ermöglicht
// Astralmacht, ZVH-Zauberer
//
// Revision 1.7  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.6  2003/03/24 18:56:18  vrandezo
// Neue Vorteile: Herausragende Eigenschaften (Forumswunsch Sebastian)
//
// Revision 1.5  2003/03/04 15:23:15  vrandezo
// weitere Tags implementiert für die Druckvorlagen
//
// Revision 1.4  2003/02/24 22:03:33  vrandezo
// Feature Request: beim Auswählen der Charakteristika werden die GP mit
// angegeben (King Lui)
//
// Revision 1.3  2003/02/24 17:37:23  vrandezo
// Prüfen implementiert
//
// Revision 1.2  2003/02/20 09:19:45  vrandezo
// Prüfe-Signatur geändert
//
// Revision 1.1.1.1  2003/02/17 15:32:43  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.9  2003/02/14 20:40:57  vrandezo
// Schlechte Eigenschaften
//
// Revision 1.8  2003/02/14 02:00:58  vrandezo
// Log::Error führt nicht mehr zu Extrafenster in Release-Version
// steigerbare VN mit Wert und Maximalwert
// einzelne VN: Gebildet, Ungebildet, Ausdauernd, Kurzatmig
//
// Revision 1.7  2003/02/09 23:58:10  vrandezo
// - steigerbare VN ohne Wert (Balance->Herausragende Balance)
// - Person-Vergleich korrigiert
// - Voraussetzung-Lesen/Schreiben korrigiert
//
// Revision 1.6  2003/02/08 00:22:51  vrandezo
// VN von RKP bei Generierung übernommen
// mehrfache VN bei RKP geben GP
// erweitere Voraussetzungen: nicht VN
// entfernbare VN
// auswählbare VN
// VN mit Voraussetzungen (nicht VN)
// Geschlecht neuerschaffener Helden ist zufällig
//
// Revision 1.5  2003/02/07 19:37:17  vrandezo
// Magische Professionen als Professionskategorie eingefügt
// Voraussetzungen können sich selber überprüfen (Implementierung fehlt)
// Charakteristika mit Beschreibung
// Charakteristika GP, Beschreibung editierbar
// KompaktPanel-GUI für Charakteristika angelegt
// VN bei Genererierung hinzufügen
// VN Liste erstellt
// VN-GUI bei RKP erstellt
// einfache VN möglich
//
// Revision 1.4  2003/02/07 10:49:33  vrandezo
// Bug: [eine Sprache] und [eine Schrift] führt zu Absturz
//
// Revision 1.3  2002/11/05 21:49:17  vrandezo
// Sprachen und Schriften im- und exportierbar
// bool const ueberschreiben aus Speicher-Schnittstelle entfernt
// Mengen haben IDs erhalten
// Voraussetzungen für Talente (allgemeine und meisterliche)
// MengeGUIwx beim Wechseln von Objekten mit Rückfrage
//
// Revision 1.2  2002/11/02 23:36:57  vrandezo
// Exportier und Importierfunktionen implementiert
// Vergleichsoperationen implementiert
// Kampftechnikfenstter implementiert
//
// Revision 1.1  2002/10/25 14:21:11  vrandezo
// Hauptmenü erweitert
// Charakteristika hinzugefügt
// eigenes Mengen-GUI-Element erzeugt
//
//@nonl
//@-node:<< Version History >>
//@nl

//@-node:@file Charakteristikum.h
//@-leo
