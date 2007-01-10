//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file Zauber.h
//@+at
// Zauber.h: Implementierung für Zauber
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

#if !defined(ZAUBER_H)
#define ZAUBER_H

//@<< includes >>
//@+node:<< includes >>
#include "NDSABasis.h"
#include "Menge.h"
#include "Auswahl.h"
#include "Regeln.h"
#include "Definition.h"
//@-node:<< includes >>
//@nl

namespace DSA {
  class PersistenzManager;

	//@  << class Verbreitung >>
	//@+node:<< class Verbreitung >>
	class Verbreitung {
	public:
	//  Verbreitung(const std::string& id = "", int wert=1, const std::string& repraesentation = "") : id_(id), wert_(wert), repraesentation_((repraesentation=="")?id:repraesentation) {}
	  Verbreitung() : wert_(1), repraesentation_(Schelme), gruppe_(Schelme) {};
		static std::string Klasse() { return ("Verbreitung"); }
		static std::string Klassen() { return ("Verbreitungen"); }
	
		void ID(const std::string& id);
		std::string ID() const;
	  void Wert(int wert) { wert_ = wert; }
	  int Wert() const { return wert_; }
		void Repraesentation(const Zauberrepraesentation rep);
		Zauberrepraesentation Repraesentation() const;
		void Gruppe(const Zauberrepraesentation rep);
		Zauberrepraesentation Gruppe() const;
	
		bool operator!=(const Verbreitung& other) const { return !operator==(other); }
		bool operator==(const Verbreitung& other) const
		{ return ((wert_==other.wert_)
	          && (gruppe_==other.gruppe_)
	          && (repraesentation_==other.repraesentation_));
		} 
		
		std::string Text() const;
	
		int Lese(PersistenzManager* pm, const std::string& id = "");
		int Schreibe(PersistenzManager* pm) const;
		int Pruefe();
		static int IDListe(PersistenzManager* pm, StringListe* liste);
		
	private:
	//  std::string id_;
	  int wert_;
	  Zauberrepraesentation repraesentation_;
	  Zauberrepraesentation gruppe_;
	}; // Verbreitung
	//@-node:<< class Verbreitung >>
	//@nl
	//@  << class Merkmal >>
	//@+node:<< class Merkmal >>
	class Merkmal {
	public:
	  Merkmal() {};
		static std::string Klasse() { return ("Merkmal"); }
		static std::string Klassen() { return ("Merkmalliste"); }
	
		void ID(const std::string& id);
		std::string ID() const;
	  void Merkmal_(const Zaubermerkmal z) { merkmal_ = z; }
	  Zaubermerkmal Merkmal_() const { return merkmal_; }
		void Element(const DSA::Element e) { element_ = e; }
		DSA::Element Element() const { return element_; }
		void Domaene(const Erzdaemon e) { domaene_ = e; }
		Erzdaemon Domaene() const { return domaene_; }
	
		bool operator!=(const DSA::Merkmal& other) const { return !operator==(other); }
		bool operator==(const DSA::Merkmal& other) const;
	    	
		std::string Text() const;
	
		int Lese(PersistenzManager* pm, const std::string& id = "");
		int Schreibe(PersistenzManager* pm) const;
		int Pruefe();
		static int IDListe(PersistenzManager* pm, StringListe* liste);
		
	private:
	//  std::string id_;
	  Zaubermerkmal merkmal_;
	  Erzdaemon domaene_;
	  DSA::Element element_;
	}; // Verbreitung
	//@-node:<< class Merkmal >>
	//@nl
  
  class Verbreitungen : public Menge<Verbreitung> {};
  //typedef Menge<Verbreitung> Verbreitungen;
    
  class Merkmale : public Menge<Merkmal> {};
  //typedef Menge<Merkmal> Merkmale;
    
	//@  << class ZauberDefinition >>
	//@+node:<< class ZauberDefinition >>
	class ZauberDefinition : public Definition {
	public:
		ZauberDefinition(const std::string& id = "") : Definition(id), komplexitaet_(D) {}
		static std::string Klasse() { return ("Zauber"); }
	  virtual std::string VirtuelleKlasse() const { return Klasse(); }
		static std::string Klassen() { return ("Zaubersprüche"); }
	
	  void Eigenschaften(Eigenschaftsart, Eigenschaftsart, Eigenschaftsart);
		Eigenschaftsart Eigenschaft(int) const;
	  void Komplexitaet(SKTSpalte k) { komplexitaet_ = k; }
		SKTSpalte Komplexitaet() const { return komplexitaet_; }
	  
	  std::vector<Zauberrepraesentation> VerfuegbareRepraesentationen() const;
	  
		//@  << operator== >>
		//@+node:<< operator== >>
		bool operator!=(const ZauberDefinition& other) const { return !operator==(other); }
		bool operator==(const ZauberDefinition& other) const
		{ return ((Definition::operator==(other))
						&& (eigenschaft_[0]==other.eigenschaft_[0])
						&& (eigenschaft_[1]==other.eigenschaft_[1])
						&& (eigenschaft_[2]==other.eigenschaft_[2])
		        && (varianten_==other.varianten_)
		        && (verbreitung_==other.verbreitung_)
		        && (merkmale_==other.merkmale_)
		        && (komplexitaet_==other.komplexitaet_));
			} // TODO AnzahlEigenschaftenproTalenprobe ist hier fest eingegeben
		//@nonl
		//@-node:<< operator== >>
		//@nl
	  
		std::string ProbeText(const std::string& trennzeichen = "/", bool uebersetzt = true) const;
	    
		//@  << Varianten >>
		//@+node:<< Varianten >>
		//@+at
		// Gibt eine Liste der Bezeichner der verfügbaren 
		// Varianten zurück, in der zur Zeit eingestellten 
		// Sprache.
		//@-at
		//@@c
		StringListe Varianten() const;
		
		//@+at
		// Setzt eine neue Variante hinein. Name wird als der 
		// Bezeichner fuer die zur Zeit eingestellte Sprache 
		// verwendet, und aus ihm wird auch die ID erstellt.
		//@-at
		//@@c
		void VarianteHinzu(const std::string& name);
		
		//@+at
		// Löscht eine Variante über den Bezeichner.
		//@-at
		//@@c
		void VarianteWeg(const std::string& name);
		
		//@+at
		// Benennt eine Variante um. Hierbei wird der alte 
		// Name und der neue Name angegeben.
		//@-at
		//@@c
		void VarianteUmbenennen(const std::string& alterName, const std::string& neuerName);
		//@nonl
		//@-node:<< Varianten >>
		//@nl
	
		// std::string Text() const;
	
		int Lese(PersistenzManager* pm, const std::string& id = "");
		int Schreibe(PersistenzManager* pm) const;
		int Pruefe();
		static int IDListe(PersistenzManager* pm, StringListe* liste);
		
	    
	  Merkmale merkmale_;
	  Verbreitungen verbreitung_;
	private:
	  DDict varianten_;
		Eigenschaftsart eigenschaft_[EigenschaftenProTalentprobe];
	  SKTSpalte komplexitaet_;
	}; // ZauberDefinition
	//@-node:<< class ZauberDefinition >>
	//@nl
	//@  << class Zauberfertigkeit >>
	//@+node:<< class Zauberfertigkeit >>
	class Zauberfertigkeit {
	public:
	  Zauberfertigkeit() : wert_(0), person_(0), hauszauber_(false) {};
	//@+at
	// 	Zauberfertigkeit(const std::string& id = "", const 
	// std::string& repraesentation = "", int wert = 0) : 
	// wert_(wert), repraesentation_(repraesentation), 
	// id_(id) {}
	// 	Zauberfertigkeit(const ZauberDefinition& def, const 
	// std::string& repraesentation = "", int wert = 0) : 
	// wert_(wert), repraesentation_(repraesentation), 
	// id_(def.ID()) {}
	// 	Zauberfertigkeit(const ZauberDefinition* const def, 
	// const std::string& repraesentation = "", int wert = 
	// 0) : wert_(wert), repraesentation_(repraesentation), 
	// id_(def->ID()) {}
	//@-at
	//@@c
	  void Bezugsperson(Person* p) { person_=p; }
	
	  static std::string Klasse() { return ("Zauberfertigkeit"); }
	  static std::string Klassen() { return ("Zauberfertigkeiten"); }
	 
	  void ID(const std::string& id);
	  std::string ID() const; // { return zauberspruch_ + " (" + repraesentation_ + ")"; }
	  void Zauberspruch(const std::string& zauberspruch) { zauberspruch_ = zauberspruch; }
	  std::string Zauberspruch() const { return zauberspruch_; }
	  void Repraesentation(const Zauberrepraesentation repraesentation) { repraesentation_ = repraesentation; }
	  Zauberrepraesentation Repraesentation() const { return repraesentation_; }
	  
	  void Wert(int wert)	{ wert_ = wert; }
	  int Wert() const { return wert_; }
	  int Maximalwert() const;
	  
	  void Extra(const std::string& extra);
	  
	  bool Hauszauber() const { return hauszauber_; }
	  void Hauszauber(bool hauszauber) { hauszauber_ = hauszauber; }
	
	  std::string ErsetzeTag(const std::string& tag) const;
	
	  const ZauberDefinition* Definition() const;
	
	  bool operator==(const Zauberfertigkeit& other) const
		{ return ((zauberspruch_==other.zauberspruch_)
	           && (repraesentation_==other.repraesentation_)
	           && (hauszauber_==other.hauszauber_)
	           && (wert_==other.wert_)); }
	  bool operator!=(const Zauberfertigkeit& other) const { return !operator==(other); }
	
	  Eigenschaftsart Eigenschaft(int e) const { return Definition()->Eigenschaft(e); }
	  SKTSpalte Lernschwierigkeit() const;
	    
	  enum TextStil {
	    RepKurz = 1,
	    ZeigeRep = 2,
	    RepNichtWennStandard = 4
	  };
	  std::string Text(int stil = RepKurz | ZeigeRep | RepNichtWennStandard) const;
	
	  int Lese(PersistenzManager* pm, const std::string& id = "");
	  int Schreibe(PersistenzManager* pm) const;
	  int Pruefe();
	  static int IDListe(PersistenzManager* pm, StringListe* liste);
		
	private:
	  std::string zauberspruch_;
	  Zauberrepraesentation repraesentation_;
	  int wert_;
	  bool hauszauber_;
	  Person* person_;
	}; // Zauberfertigkeit
	
	//@-node:<< class Zauberfertigkeit >>
	//@nl

  class ZauberDefinitionen : public Menge<ZauberDefinition> {};
  ZauberDefinitionen* zaubersprueche();
    
  class Person;
    
  typedef Menge<Zauberfertigkeit> Zauberfertigkeiten;
  class ZauberAuswahl : public AuswahlMenge<Zauberfertigkeit> {};

}

//@+others
//@+node:old
//@+at
// StringListe const * const Merkmale();
// StringListe const * const Merkmale();
// 
// StringListe const * const Repraesentationen(bool lang = 
// true);
// std::string RepraesentationKurz(const std::string& 
// lang);
// std::string RepraesentationLang(const std::string& 
// kurz);
//@-at
//@@c
//@nonl
//@-node:old
//@-others

#endif // !defined(ZAUBER_H)

//@<< Version History >>
//@+node:<< Version History >>
// $Log: Zauber.h,v $
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
// Revision 1.10  2004/04/10 01:26:08  vrandezo
// Helden erhalten ZF von RKP
//
// Revision 1.9  2004/04/09 23:49:55  vrandezo
// Zauberfertigkeiten bei RKP
// Personen verwalten ZF besser
//
// Revision 1.8  2004/04/09 22:25:50  vrandezo
// Zauberfertigkeiten fuer Personen eintragbar
//
// Revision 1.7  2003/11/28 16:46:16  vrandezo
// Zeilenvorlauf am Dateiende (Warnung bei gcc)
//
// Revision 1.6  2003/11/20 00:35:07  vrandezo
// compilierbarkeit mit gcc verbessert
//
// Revision 1.5  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.4  2003/03/20 14:36:58  vrandezo
// Zauber: Repräsentationen & Verbreitung
//
// Revision 1.3  2003/03/19 11:27:46  vrandezo
// Merkmale bei Zaubern
// 30 Verrechnungspunkte für Erstprofessionen
//
// Revision 1.2  2003/03/17 22:18:19  vrandezo
// Komplexität und Spontane Modifikationen bei Zaubern
//
// Revision 1.1  2003/03/15 14:20:25  vrandezo
// Bug: CTRL – X schließt das Programm nicht, dafür aber ALT – F4 (Achim Heidelberger)
// Zauber eingefügt: Probe, Beschreibung
//
//@nonl
//@-node:<< Version History >>
//@nl
//@-node:@file Zauber.h
//@-leo
