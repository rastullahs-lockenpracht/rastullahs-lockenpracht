//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file SpracheSchrift.h
//@+at
// SpracheSchrift.h: Implementierung von Sprachen und 
// Schriften
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

#if !defined(SPRACHESCHRIFT_H)
#define SPRACHESCHRIFT_H

//@<< includes >>
//@+node:<< includes >>
#include "NDSABasis.h"
#include "Menge.h"
#include "Regeln.h"
#include "Auswahl.h"
#include "Definition.h"
//@nonl
//@-node:<< includes >>
//@nl

namespace DSA {
	class PersistenzManager;
  class Person;
    
	//@  @+others
	//@+node:class SpracheDefinition
	class SpracheDefinition : public Definition {
	public:
		SpracheDefinition(const std::string& id = "") : Definition(id), familie_(("Garethi")), komplexitaet_(15), skt_(A) {} // TODO magische Konstante
		static std::string Klasse() { return ("Sprache"); }
	  virtual std::string VirtuelleKlasse() const { return Klasse(); }
		static std::string Klassen() { return ("Sprachen"); }
	
		typedef std::string Kriterium;
		static std::string Kriteriumsart() { return ("Sprachfamilie"); }
		static std::string Kriteriumstring(const std::string& s) { return s; }
		std::string ZuKriterium() const { return Familie(); }
		bool IstKriterium(const std::string& s) const { return s == Familie(); }
		void SetzeKriterium(const std::string& s) { Familie(s); }
	  
	  void ID(const std::string& s) { Definition::ID(s); }
		std::string ID(const bool lang = false) const { return (lang?(_t("Sprachen kennen")+": "):"")+id_; } // TODO
	    
		void Familie(const std::string& familie) { familie_ = familie; }
		std::string Familie() const { return familie_; }
		void Komplexitaet(int komplexitaet) { komplexitaet_ = komplexitaet; }
		int Komplexitaet() const { return komplexitaet_; }
		void SKTSpalte_(SKTSpalte skt) { skt_ = skt; }
		SKTSpalte SKTSpalte_() const { return skt_; }
	
		bool operator==(const SpracheDefinition& other) const
		{ return ((Definition::operator==(other))
	           && (familie_==other.familie_)
	           && (komplexitaet_==other.komplexitaet_)
	           && (skt_==other.skt_)); }
		bool operator!=(const SpracheDefinition& other) const { return !operator==(other); }
	
		// std::string Text() const;
	    
	    std::string Execute(const std::string& cmd);
	    std::string Return(const std::string& request) const;
	
		int Lese(PersistenzManager* pm, const std::string& id = "");
		int Schreibe(PersistenzManager* pm) const;
		int Pruefe();
		static int IDListe(PersistenzManager* pm, StringListe* liste);
		
	private:
		std::string familie_;
		int komplexitaet_;
		SKTSpalte skt_;
	}; // SpracheDefinition
	//@-node:class SpracheDefinition
	//@+node:class Sprache
	class Sprache {
	public:
		Sprache(const std::string& id = "", int wert = 0) : wert_(wert), id_(id), muttersprache_(false), zweitsprache_(false), person_(0) {}
		Sprache(const SpracheDefinition& def, int wert = 0) : wert_(wert), id_(def.ID()), muttersprache_(false), zweitsprache_(false), person_(0) {}
		Sprache(const SpracheDefinition* const def, int wert = 0) : wert_(wert), id_(def->ID()), muttersprache_(false), zweitsprache_(false), person_(0) {}
	
	  void Bezugsperson(Person* p) { person_=p; }
	
		static std::string Klasse() { return ("Sprache"); }
		static std::string Klassen() { return ("Sprachen"); }
	
		void ID(const std::string& id) { id_ = id; }
		std::string ID(const bool lang = false) const { return (lang?(_t("Sprachen kennen")+": "):"")+id_; }
		void Wert(int wert);
		int Wert() const;
	  bool Muttersprache() const { return muttersprache_; }
	  void Muttersprache(bool m) { muttersprache_ = m; }
	  bool Zweitsprache() const { return zweitsprache_; }
	  void Zweitsprache(bool z) { zweitsprache_ = z; }
	  void Extra(const std::string& extra);
	
		bool operator==(const Sprache& other) const	{
	    return ((id_==other.id_)
	         && (wert_==other.wert_)
	         && (muttersprache_==other.muttersprache_)
	         && (zweitsprache_==other.zweitsprache_)); }
		bool operator!=(const Sprache& other) const { return !operator==(other); }
	
	  const SpracheDefinition* Definition() const;
	
		std::string Familie() const { return Definition()->Familie(); }
		int Komplexitaet() const { return Definition()->Komplexitaet(); }
		SKTSpalte SKTSpalte_() const { return Definition()->SKTSpalte_(); }
		std::string Text(const bool nurNameUndWert = true) const;
	  std::string ErsetzeTag(const std::string& tag) const;
	
		int Lese(PersistenzManager* pm, const std::string& id = "");
		int Schreibe(PersistenzManager* pm) const;
		int Pruefe();
		static int IDListe(PersistenzManager* pm, StringListe* liste);
		
	private:
		std::string id_;
		int wert_;
	  bool muttersprache_, zweitsprache_;
	  Person* person_;
	}; // Sprache
	//@nonl
	//@-node:class Sprache
	//@+node:class SchriftDefinition
	class SchriftDefinition : public Definition {
	public:
		SchriftDefinition(const std::string& id = "") : Definition(id), komplexitaet_(15), skt_(A) {} // TODO magische Konstante
		static std::string Klasse() { return ("Schrift"); }
	  virtual std::string VirtuelleKlasse() const { return Klasse(); }
		static std::string Klassen() { return ("Schriften"); }
	
	  void ID(const std::string& s) { Definition::ID(s); }
	  std::string ID() const { return Definition::ID(); }
	  std::string Name(const bool lang = false) const { return (lang?(_t("Lesen / Schreiben")+": "):"")+id_; } // TODO
	  
		void Komplexitaet(int komplexitaet) { komplexitaet_ = komplexitaet; }
		int Komplexitaet() const { return komplexitaet_; }
		void SKTSpalte_(SKTSpalte skt) { skt_ = skt; }
		SKTSpalte SKTSpalte_() const { return skt_; }
	
		bool operator==(const SchriftDefinition& other) const	{
		  return ((Definition::operator==(other))
	         && (komplexitaet_==other.komplexitaet_)
	         && (skt_==other.skt_)); }
		bool operator!=(const SchriftDefinition& other) const { return !operator==(other); }
	
		// std::string Text() const;
	
		int Lese(PersistenzManager* pm, const std::string& id = "");
		int Schreibe(PersistenzManager* pm) const;
		int Pruefe();
		static int IDListe(PersistenzManager* pm, StringListe* liste);
		
	private:
		int komplexitaet_;
		SKTSpalte skt_;
	}; // SchriftDefinition
	//@nonl
	//@-node:class SchriftDefinition
	//@+node:class Schrift
	class Schrift {
	public:
		Schrift(const std::string& id = "", int wert = 0) : wert_(wert), id_(id) {}
		Schrift(const SchriftDefinition& def, int wert = 0) : wert_(wert), id_(def.ID()) {}
		Schrift(const SchriftDefinition* const def, int wert = 0) : wert_(wert), id_(def->ID()) {}
	
	  void Bezugsperson(Person* p) { person_=p; }
	
		static std::string Klasse() { return ("Schrift"); }
		static std::string Klassen() { return ("Schriften"); }
	
		void ID(const std::string& id) { id_ = id; }
	  std::string ID(const bool lang = false) const { return (lang?(_t("Lesen / Schreiben")+": "):"")+id_; }
		void Wert(int wert)	{ wert_ = wert; }
		int Wert() const { return wert_; }
	  void Extra(const std::string& extra);
	
		const SchriftDefinition* Definition() const;
	
		int Komplexitaet() const { return Definition()->Komplexitaet(); }
		SKTSpalte SKTSpalte_() const { return Definition()->SKTSpalte_(); }
		std::string Text(const bool nurNameUndWert = true) const;
	  std::string ErsetzeTag(const std::string& tag) const;
	
		bool operator==(const Schrift& other) const	{
	    return ((id_==other.id_)
	         && (wert_==other.wert_)); }
		bool operator!=(const Schrift& other) const { return !operator==(other); }
	
		int Lese(PersistenzManager* pm, const std::string& id = "");
		int Schreibe(PersistenzManager* pm) const;
		int Pruefe();
		static int IDListe(PersistenzManager* pm, StringListe* liste);
		
	private:
		std::string id_;
		int wert_;
	  Person* person_;
	}; // Schrift
	//@nonl
	//@-node:class Schrift
	//@-others
  
	class SpracheDefinitionen : public SortierteMenge<SpracheDefinition> {};
	SpracheDefinitionen* sprachen();
    
  typedef Menge<Sprache> Sprachen;
	//class Sprachen : public Menge<Sprache> {}; // Sprachen
  class SprachenAuswahl : public AuswahlMenge<Sprache> {};
  
	class SchriftDefinitionen : public Menge<SchriftDefinition> {};
	SchriftDefinitionen* schriften();
    
  typedef Menge<Schrift> Schriften;
	//class Schriften : public Menge<Schrift> {}; // Schriften
  class SchriftenAuswahl : public AuswahlMenge<Schrift> {};

}

#endif // !defined(SPRACHESCHRIFT_H)

//@<< Version History >>
//@+node:<< Version History >>
// $Log: SpracheSchrift.h,v $
// Revision 1.10  2004/10/17 19:12:40  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.9  2004/04/17 15:06:01  vrandezo
// um Execute und Return erweitert
//
// Revision 1.8  2004/04/13 14:06:52  vrandezo
// Hauszauber implementiert
//
// Revision 1.7  2004/04/12 21:08:09  vrandezo
// Lehrsprachen implementiert
// Mutter-/Zweitsprachensystem umgebaut
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
// Revision 1.3  2003/02/24 17:37:45  vrandezo
// Prüfen implementiert
//
// Revision 1.2  2003/02/20 09:19:45  vrandezo
// Prüfe-Signatur geändert
//
// Revision 1.1.1.1  2003/02/17 15:34:27  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.9  2003/02/07 10:49:34  vrandezo
// Bug: [eine Sprache] und [eine Schrift] führt zu Absturz
//
// Revision 1.8  2003/01/19 01:05:22  vrandezo
// Mutter- und Zweitsprachen bei Heldenerschaffung vollständig integriert
// Werte passen sich ständig der aktuellen Klugheit an
// Sprachen und Schriften in Heldenerschaffung integriert
// Bei möglicher Talentaktivierung werden nur passende Talente angezeigt
//
// Revision 1.7  2003/01/18 18:10:42  vrandezo
// Personen können Sprachen und Schriften
// übliche Kulturen bei Rassen hinzugefügt
// GUI für mögliche und übliche Kulturen und für mögliche Professionen geändert
// Varianten ausblendbar bei der Eingabe möglicher Kulturen und Professionen
//
// Revision 1.6  2003/01/16 16:32:17  vrandezo
// Mutter- und Zweitsprache bei Kultur einstellbar
//
// Revision 1.5  2003/01/14 22:26:46  vrandezo
// Sprachen und Schriften jetzt auch bei den Personenkomponenten hinzufügbar
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
//@nonl
//@-node:<< Version History >>
//@nl
//@nonl
//@-node:@file SpracheSchrift.h
//@-leo
