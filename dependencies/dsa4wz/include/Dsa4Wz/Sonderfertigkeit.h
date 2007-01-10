//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file Sonderfertigkeit.h
//@+at
// Sonderfertigkeit.h: Sonderfertigkeiten und ihre 
// Definition
// 
// $Revision: 1.21 $
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

#if !defined(SONDERFERTIGKEIT_H)
#define SONDERFERTIGKEIT_H

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
	//@+node:class SonderfertigkeitDefinition
	class SonderfertigkeitDefinition : public Definition {
	public:
		//@  << Administrativa >>
		//@+node:<< Administrativa >>
		SonderfertigkeitDefinition(const std::string& id = "") : Definition(id), gp_(0), ap_(0), waffenloserkampfstil_(false), waffenlosesonderfertigkeit_(false), zaubertanz_(false), brauchtSpezifizierung_(false), generiert_(false) {}
		static std::string Klasse() { return ("Sonderfertigkeit"); }
		virtual std::string VirtuelleKlasse() const { return Klasse(); }
		static std::string Klassen() { return ("Sonderfertigkeiten"); }
		
		void ID(const std::string& id) { id_ = id; }
		std::string ID() const { return id_; }
		std::string Text() const;
		
		std::string NurID() const;
		//@nonl
		//@-node:<< Administrativa >>
		//@nl
	  
	  std::string Bezeichnung(const std::string& sprache = DSA::Lang()) const;
	  using Definition::Bezeichnung;
	
		void GP(int gp) { gp_ = gp; }
		int GP() const { return gp_; }
		void AP(int ap) { ap_ = ap; }
		int AP() const { return ap_; }
	    
		//@  << Semantische Abfragen >>
		//@+node:<< Semantische Abfragen >>
		bool IstGelaendekunde() const;
		bool IstRepraesentation() const;
		Zauberrepraesentation Repraesentation() const;
		bool IstRitualkenntnis() const;
		Zauberrepraesentation Ritualkenntnis() const;
		bool IstRitualkenntnisZaubertaenzer() const;
		bool IstMerkmalskenntnis() const;
		Zaubermerkmal Merkmalskenntnis() const;
		Erzdaemon Domaene() const;
		enum Element Element() const;
		
		void IstWaffenloserKampfstil(bool w) { waffenloserkampfstil_ = w; }
		bool IstWaffenloserKampfstil() const { return waffenloserkampfstil_; }
		void IstWaffenloseSonderfertigkeit(bool w) { waffenlosesonderfertigkeit_ = w; }
		bool IstWaffenloseSonderfertigkeit() const { return waffenlosesonderfertigkeit_; }
		void IstZaubertanz(bool z) { zaubertanz_ = z; }
		bool IstZaubertanz() const { return zaubertanz_; }
		
		bool IstGeneriert() const { return generiert_; }
		void IstGeneriert(bool g) { generiert_ = g; }
		
		bool IstScharfschuetze() const;
		std::string Scharfschuetze() const;
		bool IstMeisterschuetze() const;
		std::string Meisterschuetze() const;
		
		//@-node:<< Semantische Abfragen >>
		//@nl
	  
	  std::string Extra(int i = 0) const;
	  void Extra(std::string x, int i=0);
	  StringListe Extras() const { return extras_; }
	  void Extras(StringListe x) { extras_ = x; }
	  
	  bool BrauchtSpezifizierung() const { return brauchtSpezifizierung_; }
	  void BrauchtSpezifizierung(const bool b) { brauchtSpezifizierung_ = b; }
	
	  bool HatStufen() const;
	  int Stufe() const;
	  int MaximalStufe() const;
	  std::string Stufe(int stufe) const;
	
	  bool Hinzufuegbar(const Person&, bool schlaftest = false) const;
	  
		//@  << operator== >>
		//@+node:<< operator== >>
		bool operator==(const SonderfertigkeitDefinition& other) const;
		bool operator!=(const SonderfertigkeitDefinition& other) const { return !operator==(other); }
		//@nonl
		//@-node:<< operator== >>
		//@nl
		
	  // std::string Text() const;
	  
		//@  << Lese / Schreibe >>
		//@+node:<< Lese / Schreibe >>
		int Lese(PersistenzManager* pm, const std::string& id = "");
		int Schreibe(PersistenzManager* pm) const;
		int Pruefe();
		static int IDListe(PersistenzManager* pm, StringListe* liste);
		//@nonl
		//@-node:<< Lese / Schreibe >>
		//@nl
	  
	  Voraussetzungen voraussetzungen_;
		
	private:
		//@  << private >>
		//@+node:<< private >>
		int gp_;
		int ap_;
		bool waffenloserkampfstil_;
		bool waffenlosesonderfertigkeit_;
		bool zaubertanz_;
		StringListe extras_;
		bool brauchtSpezifizierung_;
		bool generiert_;
		//@nonl
		//@-node:<< private >>
		//@nl
	}; // SonderfertigkeitDefinition
	//@-node:class SonderfertigkeitDefinition
	//@+node:class Sonderfertigkeit
	class Sonderfertigkeit {
	public:
		//@  << Administrativa >>
		//@+node:<< Administrativa >>
		Sonderfertigkeit() : id_(""), spezifizierung_(""), stufe_(0) {}
		Sonderfertigkeit(const std::string& id) : id_(id), spezifizierung_(""), stufe_(0) {}
		Sonderfertigkeit(const SonderfertigkeitDefinition& def) : id_(def.ID()), stufe_(0) {}
		Sonderfertigkeit(const SonderfertigkeitDefinition* const def) : id_(def->ID()), stufe_(0) {}
		static std::string Klasse() { return ("Sonderfertigkeit"); }
		static std::string Klassen() { return ("Sonderfertigkeiten"); }
		
		void ID(const std::string& id) { id_ = id; }
		std::string ID() const { return id_; }
		//@nonl
		//@-node:<< Administrativa >>
		//@nl
	  
	  bool HatStufen() const { return stufe_!=0; }
	  int Stufe() const { return stufe_; }
	  void Stufe(const int s) { stufe_ = s; }
	  int MaximaleStufe(const Person&) const;
	  SKTSpalte SKTSpalte_(const Person&) const;
	
	  int GPKosten(const Person&) const;
	  int APKosten(const Person&) const;
	  std::string Spezifizierung() const { return spezifizierung_; }
	  void Spezifizierung(const std::string& s) { spezifizierung_ = s; }
	
		const SonderfertigkeitDefinition* Definition() const;
	
		bool operator==(const Sonderfertigkeit& other) const {
	    return ((id_==other.id_)
		       && (stufe_==other.stufe_)
	         && (spezifizierung_==other.spezifizierung_)); }
		bool operator!=(const Sonderfertigkeit& other) const { return !operator==(other); }
	
		std::string Text() const;
		std::string Text(Person* p) const;
	  std::string ErsetzeTag(const std::string& tag) const;
	  
		//@  << Lese / Schreibe >>
		//@+node:<< Lese / Schreibe >>
		int Lese(PersistenzManager* pm, const std::string& id = "");
		int Schreibe(PersistenzManager* pm) const;
		int Pruefe();
		static int IDListe(PersistenzManager* pm, StringListe* liste);
		//@nonl
		//@-node:<< Lese / Schreibe >>
		//@nl
	  	
	private:
	  int Kosten(const int kosten, const Person& p) const;
	  int DynamischeKosten(const Person& p) const;
	  bool eigeneRK(const Person& p) const;
		std::string id_;
		int stufe_;
	  std::string spezifizierung_;
	}; // Sonderfertigkeit
	//@nonl
	//@-node:class Sonderfertigkeit
	//@+node:class Sonderfertigkeiten
	class Sonderfertigkeiten : public Menge<Sonderfertigkeit> {
	public:
	  Sonderfertigkeiten Bereinige() const;
	}; // Sonderfertigkeiten
	
	class SonderfertigkeitenAuswahl : public AuswahlMenge<Sonderfertigkeit> {
	}; // SonderfertigkeitenAuswahl
	//@nonl
	//@-node:class Sonderfertigkeiten
	//@-others
  
	class SonderfertigkeitDefinitionen : public Menge<SonderfertigkeitDefinition> {
	}; // SonderfertigkeitDefinitionen

	SonderfertigkeitDefinitionen* sonderfertigkeiten();
  
  SonderfertigkeitDefinitionen* generierteSonderfertigkeiten();
  void GeneriereSonderfertigkeiten();
  
} // DSA

#endif // !defined(SONDERFERTIGKEIT_H)

//@<< Version History >>
//@+node:<< Version History >>
// $Log: Sonderfertigkeit.h,v $
// Revision 1.21  2004/10/17 22:26:55  josch1710
// - Bugfixes
// - Autotools-Unterstützung.
//
// Revision 1.20  2004/10/17 19:12:40  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.19  2004/04/12 01:44:13  vrandezo
// Astrale Regneration
// Vorbereitung auf Merkmalskenntnisse
//
// Revision 1.18  2004/04/11 01:32:50  vrandezo
// generiere Ritualkenntnisse
// Test auf "schlafend" verbessert
//
// Revision 1.17  2004/04/10 22:38:02  vrandezo
// SF Repraesentation
//
// Revision 1.16  2004/01/07 23:07:06  vrandezo
// RKP können jetzt [eine Geländekunde] haben
//
// Revision 1.15  2004/01/05 03:35:19  vrandezo
// SF bei RKP-Defs sind jetzt auch Auswahlmengen statt einfacher Mengen
//
// Revision 1.14  2003/11/28 16:45:37  vrandezo
// Manöver
//
// Revision 1.13  2003/10/30 00:02:44  vrandezo
// Steigerbare Ritualkenntnisse
//
// Revision 1.12  2003/10/27 01:24:53  vrandezo
// Zaubertänze
//
// Revision 1.11  2003/10/26 23:16:08  vrandezo
// RK hinzufügbar zu RKP und steigerbar
//
// Revision 1.10  2003/10/25 22:33:27  vrandezo
// SF-Text
//
// Revision 1.9  2003/10/25 02:11:18  vrandezo
// generierte Sonderfertigkeiten
//
// Revision 1.8  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.7  2003/03/09 15:25:15  vrandezo
// Waffenlose Kampfstile
// Aufgeteilte Editoren für Waffenlose Kampfstile und sonstige SF
// Voraussetzungen können auch auf Kampftechniken sein
//
// Revision 1.6  2003/03/04 15:23:19  vrandezo
// weitere Tags implementiert für die Druckvorlagen
//
// Revision 1.5  2003/02/24 17:37:43  vrandezo
// Prüfen implementiert
//
// Revision 1.4  2003/02/20 09:19:45  vrandezo
// Prüfe-Signatur geändert
//
// Revision 1.3  2003/02/20 00:21:58  vrandezo
// Sonderfertigkeiten vollständig integriert
// schlafende SF
//
// Revision 1.2  2003/02/18 18:48:48  vrandezo
// SF bei Generierung
// Geländekunden außer der ersten für nur 2/3 des Preises
//
// Revision 1.1.1.1  2003/02/17 15:34:20  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.3  2003/02/17 14:10:47  vrandezo
// Beschreibung eintragbar für Helden während der Generierung
// Entsprechende Vor- und Nachteile machen Modifikatoren statt im Basiswert mitberechnet zu werden
//
// Revision 1.2  2003/02/16 17:49:32  vrandezo
// Bug entfernt: Button Anteil- bei der Augenfarbe tat nicht
// SF ex- und importierbar
// einfache Voraussetzung (weder "kein X" noch "X min. 7") hinzugefügt
//
// Revision 1.1  2003/02/14 21:42:51  vrandezo
// Sonderfertigkeiten hinzugefügt
//
//@nonl
//@-node:<< Version History >>
//@nl
//@-node:@file Sonderfertigkeit.h
//@-leo
