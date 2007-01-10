//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file PersonenKomponente.cpp
//@+at
// PersonenKomponente.cpp: Implementierung der Basisklasse 
// PersonenKomponente.
// 
// $Revision: 1.16 $
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
#include "PersonenKomponente.h"
#include "Log.h"
#include "PersistenzManager.h"
#include "Texte.h"
#include "NDSAUtil.h"
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
	//@+node:class PersonenKomponente
	//@+others
	//@+node:ctor
	PersonenKomponente::PersonenKomponente() : id_(""), gp_(0), geschlechtsspezifisch_(false) {
	  sonderfertigkeiten_.ID(("automatische"));
	  sprachen_.ID(("Sprachen kennen"));
	}
	//@-node:ctor
	//@+node:Lese
	int PersonenKomponente::Lese(PersistenzManager* pm, const std::string& typ, const std::string& id) {
		std::string geleseneID = id;
		int rv = pm->OeffneZumLesen(typ, geleseneID);
		int gp = 0;
		rv += pm->Lese(("GP"), gp);
		Modifikatoren modifikatoren;
		rv += modifikatoren.Lese(pm);
	  Voraussetzungen voraussetzungen;
	  rv += voraussetzungen.Lese(pm);
		Talente talente;
		rv += talente.Lese(pm);
	  Kampftechniken kampftechniken;
	  rv += kampftechniken.Lese(pm);
	  Sprachen sprachen;
	  // Folgendes aufgrund einer kleinen Formatänderung im XDIML - sonst
	  // würde einfach die dritte Zeile reichen
	//  if (pm->Existiert(SprachenAuswahl::Klassen())) {
	//    if (pm->Existiert(SprachenAuswahl::Klassen(), _t("Sprachen kennen")))
	      rv += sprachen.Lese(pm, ("Sprachen kennen"));
	//    else {
	//      rv += sprachen.Lese(pm);
	//      if ((sprachen.ID()==_t("Muttersprache")) || (sprachen.ID()==_t("Zweitsprache")))
	//        sprachen.clear();
	//    }
	//  }
	  Schriften schriften;
	  rv += schriften.Lese(pm);
	  Zauberfertigkeiten zauber;
	  rv += zauber.Lese(pm);
		Charakteristika charakteristika;
		rv += charakteristika.Lese(pm);
	  Sonderfertigkeiten sonderfertigkeiten;
	  rv += sonderfertigkeiten.Lese(pm, ("automatische"));
	  StringListe verbilligteSonderfertigkeiten;
	  rv += pm->Lese(verbilligteSonderfertigkeiten, ("Sonderfertigkeit"), ("Sonderfertigkeiten"), ("verbilligte"));
	//  std::string beschreibung;
	//  if (pm->Existiert(_t("Beschreibung")))
	//    rv += pm->Lese(_t("Beschreibung"), beschreibung);
	//  std::string varianteVon = "";
	//  if (pm->Existiert(_t("VarianteVon")))
	//    rv += pm->Lese(_t("VarianteVon"), varianteVon);
	  bool geschlechtsspezifisch = pm->Existiert(("geschlechtsspezifisch"));
	  std::string bezeichnung = "";
	  rv += pm->Lese(("Bezeichnung"), bezeichnung);
	
	  if (!rv) {
			ID(geleseneID);
			GP(gp);
			modifikatoren_ = modifikatoren;
	    voraussetzungen_ = voraussetzungen;
			talente_ = talente;
	    kampftechniken_ = kampftechniken;
			charakteristika_ = charakteristika;
	    sonderfertigkeiten_ = sonderfertigkeiten;
	    verbilligteSonderfertigkeiten_ = verbilligteSonderfertigkeiten;
	    sprachen_ = sprachen;
	    schriften_ = schriften;
	    zauber_ = zauber;
	    IstGeschlechtsspezifisch(geschlechtsspezifisch);
	    Bezeichnung(bezeichnung);
	//    VarianteVon(varianteVon);
	//    Beschreibung(beschreibung);
		}
		rv += pm->SchliesseNachLesen();
	  // Bug-Korrektur
	  kampftechniken_.ID("");
	  sprachen_.ID(("Sprachen kennen"));
	  schriften_.ID("");
		return rv; // TODO
	} // Lese
	//@nonl
	//@-node:Lese
	//@+node:Schreibe
	int PersonenKomponente::Schreibe(PersistenzManager* pm, const std::string& typ) const {
		int rv = pm->OeffneZumSchreiben(typ, ID());
	  rv += pm->Schreibe(("Bezeichnung"), Bezeichnung());
	
	  if (IstGeschlechtsspezifisch()) {
	    rv += pm->OeffneZumSchreiben(("geschlechtsspezifisch"));
	    rv += pm->SchliesseNachSchreiben();
	  }
		rv += pm->Schreibe(("GP"), GP());
		rv += modifikatoren_.Schreibe(pm);
	  rv += voraussetzungen_.Schreibe(pm);
		rv += talente_.Schreibe(pm);
	  rv += kampftechniken_.Schreibe(pm);
		rv += charakteristika_.Schreibe(pm);
	  rv += sonderfertigkeiten_.Schreibe(pm);
	  rv += pm->Schreibe(verbilligteSonderfertigkeiten_, ("Sonderfertigkeit"), ("Sonderfertigkeiten"), ("verbilligte"));
	  rv += sprachen_.Schreibe(pm);
	  rv += schriften_.Schreibe(pm);
	  rv += zauber_.Schreibe(pm);
	//  if (Beschreibung()!="")
	//    rv += pm->Schreibe(_t("Beschreibung"), Beschreibung());
		rv += pm->SchliesseNachSchreiben();
		return rv; // TODO
	} // Schreibe
	//@nonl
	//@-node:Schreibe
	//@+node:Pruefe
	int PersonenKomponente::Pruefe() {
	  int rv = 0;
	  rv += modifikatoren_.Pruefe();
	  rv += voraussetzungen_.Pruefe();
		rv += talente_.Pruefe();
	  rv += sprachen_.Pruefe();
	  rv += schriften_.Pruefe();
	  rv += kampftechniken_.Pruefe();
	  rv += zauber_.Pruefe();
		rv += charakteristika_.Pruefe();
	  rv += sonderfertigkeiten_.Pruefe();
	  forallconst (StringListe, s, verbilligteSonderfertigkeiten_)
	    if (!sonderfertigkeiten()->Existiert(*s))
	      if (!generierteSonderfertigkeiten()->Existiert(*s))
	        ++rv;
	  return rv;
	}
	
	//@-node:Pruefe
	//@-others
	//@nonl
	//@-node:class PersonenKomponente
	//@+node:class PersonenKomponenteDefinition
	//@+others
	//@+node:ctor
	PersonenKomponenteDefinition::PersonenKomponenteDefinition
	  (const std::string& typ, int abzug) 
	  : Definition(""), typ_(typ), gp_(-(abzug+5)/10), 
	    maennlich_(true), weiblich_(true) {
	  sonderfertigkeiten_.ID(("automatische"));
	  sprachen_.ID(("Sprachen kennen"));
	} // ctor
	//@nonl
	//@-node:ctor
	//@+node:NurID
	std::string PersonenKomponenteDefinition::NurID() const {
	  return Bezeichnung();
	} // NurID
	
	//void PersonenKomponenteDefinition::NurID(const std::string& id) {
	//  id_=id;
	//} // NurID
	//@nonl
	//@-node:NurID
	//@+node:ID
	/*
	std::string PersonenKomponenteDefinition::ID() const {
	  if (!IstGeschlechtsspezifisch()) return id_;
	  if (NurMaennlich()) return id_ + " (" + _t("männlich") + ")";
	  if (NurWeiblich()) return id_ + " (" + _t("weiblich") + ")";
	  return id_;
	} // ID
	
	void PersonenKomponenteDefinition::ID(const std::string& id) {
	  std::string s = id;
	  int klammer = s.find("(" + _t("männlich") + ")");
	  if (s.size()>=klammer) {
	    maennlich_ = true;
	    weiblich_ = false;
	    id_=s.substr(0, s.find("(" + _t("männlich") + ")")-1);
	    return;
	  }
	  klammer = s.find("(" + _t("weiblich") + ")");
	  if (s.size()>=klammer) {
	    maennlich_ = false;
	    weiblich_ = true;
	    id_=s.substr(0, s.find("(" + _t("weiblich") + ")")-1);
	    return;
	  }
	  maennlich_=weiblich_=true;
	  id_=s;
	}
	*/
	//@nonl
	//@-node:ID
	//@+node:NurMaennlich/Weiblich
	bool PersonenKomponenteDefinition::NurMaennlich() const {
	  return ((maennlich_)&&(!weiblich_));
	} // NurMaennlich
	
	bool PersonenKomponenteDefinition::NurWeiblich() const {
	  return ((!maennlich_)&&(weiblich_));
	} // NurWeiblich
	//@nonl
	//@-node:NurMaennlich/Weiblich
	//@+node:Text
	std::string PersonenKomponenteDefinition::Text(const std::string& trennzeichenDerEinzelne, const std::string& zwischenEinzelneTitelUndSachen, const std::string& trennzeichenImEinzelnen, const std::string& wennEinzelnesLeer) const {
	  std::string s = typ_ + zwischenEinzelneTitelUndSachen + Bezeichnung() + trennzeichenDerEinzelne;
	  if ((VarianteVon()!=NurID())&&(IstVariante())) s += _t("Variante von") + " " + VarianteVon() + trennzeichenDerEinzelne;
	  if (IstGeschlechtsspezifisch()) s += (NurWeiblich() ? _t("Nur für Frauen verwendbar") : _t("Nur für Männer verwendbar")) + trennzeichenDerEinzelne;
	  if (Beschreibung()!="") s += Beschreibung() + trennzeichenDerEinzelne;
	  s += _t("Generierungspunkte") + zwischenEinzelneTitelUndSachen + strings::int2sStr(GP());
		if (voraussetzungen_.size()!=0) s += trennzeichenDerEinzelne + _t("Voraussetzungen") + zwischenEinzelneTitelUndSachen + voraussetzungen_.Text(trennzeichenImEinzelnen, wennEinzelnesLeer);
		if (modifikatoren_.size()!=0) s += trennzeichenDerEinzelne + _t("Modifikatoren") + zwischenEinzelneTitelUndSachen + modifikatoren_.Text(trennzeichenImEinzelnen, wennEinzelnesLeer);
		if (charakteristika_.size()!=0) s += trennzeichenDerEinzelne + _t("Vor- und Nachteile") + zwischenEinzelneTitelUndSachen + charakteristika_.Text(trennzeichenImEinzelnen, wennEinzelnesLeer);
		if (kampftechniken_.size()!=0) s += trennzeichenDerEinzelne + _t("Kampftechniken") + zwischenEinzelneTitelUndSachen + kampftechniken_.Text(trennzeichenImEinzelnen, wennEinzelnesLeer);
		if (talente_.size()!=0) s += trennzeichenDerEinzelne + _t("Talente") + zwischenEinzelneTitelUndSachen + talente_.Text(trennzeichenImEinzelnen, wennEinzelnesLeer);
		if (sprachen_.size()!=0) s += trennzeichenDerEinzelne + _t("Sprachen") + zwischenEinzelneTitelUndSachen + sprachen_.Text(trennzeichenImEinzelnen, wennEinzelnesLeer);
		if (schriften_.size()!=0) s += trennzeichenDerEinzelne + _t("Schriften") + zwischenEinzelneTitelUndSachen + schriften_.Text(trennzeichenImEinzelnen, wennEinzelnesLeer);
		if (zauber_.size()!=0) s += trennzeichenDerEinzelne + _t("Zauber") + zwischenEinzelneTitelUndSachen + zauber_.Text(trennzeichenImEinzelnen, wennEinzelnesLeer);
	  return s;
	} // Text
	//@nonl
	//@-node:Text
	//@+node:Lese
	int PersonenKomponenteDefinition::Lese(PersistenzManager* pm, const std::string& id) {
		std::string geleseneID = id;
		int rv = pm->OeffneZumLesen(typ_, geleseneID);
	
	  Dict bezeichnung;
	  Dict beschreibung;
	  rv += LeseBezeichnung(pm, geleseneID, bezeichnung, beschreibung);
	  
		int gp = 0;
		rv += pm->Lese(("GP"), gp);
		Modifikatoren modifikatoren;
		rv += modifikatoren.Lese(pm);
	  Voraussetzungen voraussetzungen;
	  rv += voraussetzungen.Lese(pm);
		TalenteAuswahl talente;
		rv += talente.Lese(pm);
	  KampftechnikenAuswahl kampftechniken;
	  rv += kampftechniken.Lese(pm);
	  SprachenAuswahl sprachen;
	  // Folgendes aufgrund einer kleinen Formatänderung im XDIML - sonst
	  // würde einfach die dritte Zeile reichen
	  if (pm->Existiert(SprachenAuswahl::Klassen())) {
	    if (pm->Existiert(SprachenAuswahl::Klassen(), ("Sprachen kennen")))
	      rv += sprachen.Lese(pm, ("Sprachen kennen"));
	    else {
	      rv += sprachen.Lese(pm);
	      if ((sprachen.ID()==("Muttersprache")) || (sprachen.ID()==("Zweitsprache")))
	        sprachen.clear();
	    }
	  }
	  SchriftenAuswahl schriften;
	  rv += schriften.Lese(pm);
	  ZauberAuswahl zauber;
	  rv += zauber.Lese(pm);
		CharakteristikaAuswahl charakteristika;
		rv += charakteristika.Lese(pm);
	  SonderfertigkeitenAuswahl sonderfertigkeiten;
	  rv += sonderfertigkeiten.Lese(pm, ("automatische"));
	  StringListe verbilligteSonderfertigkeiten;
	  rv += pm->Lese(verbilligteSonderfertigkeiten, ("Sonderfertigkeit"), ("Sonderfertigkeiten"), ("verbilligte"));
	  std::string varianteVon = "";
	  if (pm->Existiert(("VarianteVon")))
	    rv += pm->Lese(("VarianteVon"), varianteVon);
	
	  if (!rv) {
			ID(geleseneID);
	    bezeichnung_ = bezeichnung;
	   beschreibung_ = beschreibung;
			GP(gp);
			modifikatoren_ = modifikatoren;
	    voraussetzungen_ = voraussetzungen;
			talente_ = talente;
	    kampftechniken_ = kampftechniken;
			charakteristika_ = charakteristika;
	    sonderfertigkeiten_ = sonderfertigkeiten;
	    verbilligteSonderfertigkeiten_ = verbilligteSonderfertigkeiten;
	    sprachen_ = sprachen;
	    schriften_ = schriften;
	    zauber_ = zauber;
	    VarianteVon(varianteVon);
		}
		rv += pm->SchliesseNachLesen();
	  // Bug-Korrektur
	  kampftechniken_.ID("");
	  sprachen_.ID(("Sprachen kennen"));
	  schriften_.ID("");
		return rv; // TODO
	} // Lese
	//@nonl
	//@-node:Lese
	//@+node:Schreibe
	int PersonenKomponenteDefinition::Schreibe(PersistenzManager* pm) const {
		int rv = pm->OeffneZumSchreiben(typ_, ID());
	  if (IstVariante())
	    rv += pm->Schreibe(("VarianteVon"), VarianteVon());
	  rv += SchreibeBezeichnung(pm);
		rv += pm->Schreibe(("GP"), GP());
		rv += modifikatoren_.Schreibe(pm);
	  rv += voraussetzungen_.Schreibe(pm);
		rv += talente_.Schreibe(pm);
	  rv += kampftechniken_.Schreibe(pm);
		rv += charakteristika_.Schreibe(pm);
	  rv += sonderfertigkeiten_.Schreibe(pm);
	  rv += pm->Schreibe(verbilligteSonderfertigkeiten_, ("Sonderfertigkeit"), ("Sonderfertigkeiten"), ("verbilligte"));
	  rv += sprachen_.Schreibe(pm);
	  rv += schriften_.Schreibe(pm);
	  rv += zauber_.Schreibe(pm);
	//  if (Beschreibung()!="")
	//    rv += pm->Schreibe(("Beschreibung"), Beschreibung());
		rv += pm->SchliesseNachSchreiben();
		return rv; // TODO
	} // Schreibe
	//@nonl
	//@-node:Schreibe
	//@+node:Pruefe
	int PersonenKomponenteDefinition::Pruefe() {
	  int rv = 0;
	  rv += modifikatoren_.Pruefe();
	  // TODO Modifikatoren
	  rv += voraussetzungen_.Pruefe();
	  rv += zauber_.Pruefe(zaubersprueche());
	  // TODO Zauber
	  
	  StringListe sl;
		//@  << Pruefe Talente >>
		//@+node:<< Pruefe Talente >>
		sl.push_back("[ein Körperliches Talent]");
		sl.push_back("[ein Gesellschaftliches Talent]");
		sl.push_back("[ein Natur-Talent]");
		sl.push_back("[ein Wissenstalent]");
		sl.push_back("[ein Handwerkliches Talent]");
		rv += talente_.Pruefe(talente(), &sl);
		sl.clear();
		sl.push_back("[eine Kampftechnik]");
		rv += kampftechniken_.Pruefe(kampftechniken(), &sl);
		sl.clear();
		sl.push_back("[eine Sprache]");
		rv += sprachen_.Pruefe(sprachen(), &sl);
		sl.clear();
		sl.push_back("[eine Schrift]");
		rv += schriften_.Pruefe(schriften(), &sl);
		sl.clear();
		//@-node:<< Pruefe Talente >>
		//@nl
	  
	  forallconstp (CharakteristikumDefinitionen, ch, generierteCharakteristika())
	    sl.push_back(ch->first);
	  rv += charakteristika_.Pruefe(charakteristika(), &sl);
	
	  // TODO VarianteVon muss in RKP geprüft werden
	  
	  if (IstGeschlechtsspezifisch())
	    if (!IstVariante()) {
		    Log::Info("Person::Pruefe: Geschlechtsspezifische Variante nicht als Variante markiert");
			  ++rv;
			}
	  
		//@  << Pruefe SF >>
		//@+node:<< Pruefe SF >>
		if (sonderfertigkeiten_.ID()!=("automatische")) {
		  Log::Info("ID von Sonderfertigkeiten falsch (fatal)");
		  ++rv;
		}
		
		// normale
		sl.clear();
		//forallconstp (SonderfertigkeitDefinitionen, sf, generierteSonderfertigkeiten())
		 // sl.push_back(sf->first);
		sl.push_back(("[eine Geländekunde]"));
		rv += sonderfertigkeiten_.Pruefe(sonderfertigkeiten(), generierteSonderfertigkeiten(), &sl);
		
		// verbilligte
		StringListe v2;
		forallconst (StringListe, si, verbilligteSonderfertigkeiten_)
		  if (!((sonderfertigkeiten()->Existiert(*si))
		      ||(generierteSonderfertigkeiten()->Existiert(*si)))) {
		    Log::Info("Person::Pruefe: Unbekannte Sonderfertigkeit " + *si);
		    std::string s = sonderfertigkeiten()->Finde(*si);
		    if (s != "") {
		      Log::Info("ist eigentlich " + s + " (wurde korrigiert)");
		      v2.push_back(s);
		    } else {
		      s = generierteSonderfertigkeiten()->Finde(*si);
		      if (s != "") {
		        Log::Info("ist eigentlich " + s + " (wurde korrigiert)");
		        v2.push_back(s);
		      } else v2.push_back(*si);
		    }
		    ++rv;    
		  } else v2.push_back(*si);
		verbilligteSonderfertigkeiten_ = v2;
		  
		
		
		
		
		
		
		//@-node:<< Pruefe SF >>
		//@nl
	  
	  if (rv!=0) Log::Info("in " + typ_ + " " + ID());
	  return rv;
	} // Pruefe
	
	
	//@-node:Pruefe
	//@+node:IDListe
	int PersonenKomponenteDefinition::IDListe(PersistenzManager* pm, StringListe* liste) {
		return pm->IDListe(typ_, liste); // TODO Prüfen
	} // IDListe
	//@nonl
	//@-node:IDListe
	//@-others
	//@nonl
	//@-node:class PersonenKomponenteDefinition
	//@+node:Alter Code
	//@+at
	// bool 
	// CPersonenKomponente::istCharakteristikum(std::string 
	// charStr)
	// 	{
	// 		LKCharakteristikum::iterator listenIterator;
	// 		for (listenIterator=m_Charakteristika->begin();
	// 				 listenIterator!=m_Charakteristika->end();
	// 				 listenIterator++)
	// 			if ((*listenIterator)->Bezeichner()==charStr)
	// 				return true;
	// 		return false;
	// 	};
	// 
	// void 
	// CPersonenKomponente::neuesCharakteristikum(std::string 
	// charStr)
	// 	{
	// 		LKCharakteristikum::iterator listenIterator;
	// 		bool gefunden = false;
	// 		for (listenIterator=m_Charakteristika->begin();
	// 				 listenIterator!=m_Charakteristika->end();
	// 				 listenIterator++)
	// 			if ((*listenIterator)->Bezeichner()==charStr)
	// 				gefunden = true;
	// 		if (!gefunden)
	// 			{
	// 				IKCharakteristikum* Char = 
	// IKCharakteristikum::Erzeuge();
	// 				Char->setzeBezeichner(charStr);
	// 				m_Charakteristika->push_back(Char);
	// 			};
	// 	};
	// 
	// void 
	// CPersonenKomponente::loescheCharakteristikum(std::string 
	// charStr)
	// 	{
	// 		LKCharakteristikum::iterator listenIterator;
	// 		LKCharakteristikum::iterator gesuchtesChar;
	// 		bool gefunden = false;
	// 		for (listenIterator=m_Charakteristika->begin();
	// 				 listenIterator!=m_Charakteristika->end();
	// 				 listenIterator++)
	// 			if ((*listenIterator)->Bezeichner()==charStr)
	// 				{
	// 					gefunden = true;
	// 					gesuchtesChar = listenIterator;
	// 				};
	// 		if (gefunden)
	// 			{
	// 				delete (*gesuchtesChar);
	// 				m_Charakteristika->erase(gesuchtesChar);
	// 			};
	// 	};
	// 
	// IKCharakteristikum* 
	// CPersonenKomponente::Charakteristikum(std::string 
	// charStr)
	// 	{
	// 		LKCharakteristikum::iterator listenIterator;
	// 		for (listenIterator=m_Charakteristika->begin();
	// 				 listenIterator!=m_Charakteristika->end();
	// 				 listenIterator++)
	// 			if ((*listenIterator)->Bezeichner()==charStr)
	// 				return *listenIterator;
	// 		return 0;
	// 	};
	// 
	// LKCharakteristikum* 
	// CPersonenKomponente::Charakteristika()
	// 	{
	// 		return m_Charakteristika;
	// 	}
	// 
	// void CPersonenKomponente::setzeTalent(IKTalent* 
	// talent)
	// 	{
	// 		m_TK->setze(talent);
	// 	}
	// 
	// void CPersonenKomponente::loescheTalent(std::string 
	// strTalent)
	// 	{
	// 		m_TK->loesche(strTalent);
	// 	}
	// 
	// const IKTalent* 
	// CPersonenKomponente::Talent(std::string strTalent)
	// 	{
	// 		return m_TK->Talent(strTalent);
	// 	}
	// 
	// const IKTalent* 
	// CPersonenKomponente::Talent(DSA::Talentgruppe g, int 
	// i)
	// 	{
	// 		return m_TK->Talent(g, i);
	// 	}
	// 
	// const IKTalent* CPersonenKomponente::Talent(int i)
	// 	{
	// 		return m_TK->Talent(i);
	// 	}
	// 
	// std::string CPersonenKomponente::TalentText()
	// 	{
	// 		return m_TK->text();
	// 	}
	//@-at
	//@@c
	//@nonl
	//@-node:Alter Code
	//@-others
}

//@<< Version History >>
//@+node:<< Version History >>
// $Log: PersonenKomponente.cpp,v $
// Revision 1.16  2004/10/17 19:12:40  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.15  2004/04/09 23:49:55  vrandezo
// Zauberfertigkeiten bei RKP
// Personen verwalten ZF besser
//
// Revision 1.14  2004/04/04 23:37:40  vrandezo
// Bug: beim Re-Generieren war der RK zu hoch
//
// Revision 1.13  2004/01/07 23:07:06  vrandezo
// RKP können jetzt [eine Geländekunde] haben
//
// Revision 1.12  2004/01/05 03:35:19  vrandezo
// SF bei RKP-Defs sind jetzt auch Auswahlmengen statt einfacher Mengen
//
// Revision 1.11  2004/01/05 02:45:29  vrandezo
// Charakteristika bei RPK-Definition von Mengen auf Auswahlmengen umgestellt
//
// Revision 1.10  2003/11/28 23:22:49  vrandezo
// speicherbare konkrete RKP
//
// Revision 1.9  2003/10/25 02:11:17  vrandezo
// generierte Sonderfertigkeiten
//
// Revision 1.8  2003/10/24 13:52:49  vrandezo
// Pruefe prüft nach variantenlosen geschlechtsspezifischen Varianten
//
// Revision 1.7  2003/10/24 13:30:07  vrandezo
// DEBUG_NEW raus
// Varianten jetzt von Hand eintragbar
//
// Revision 1.6  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.5  2003/02/24 17:37:32  vrandezo
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
// Revision 1.1.1.1  2003/02/17 15:33:25  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.21  2003/02/13 22:29:30  vrandezo
// Auf Xerces 2.2.0 geupdatet
//
// Revision 1.20  2003/02/13 11:44:46  vrandezo
// Bug entfernt: Sprachen und Schriften bei Personen bleiben erhalten
// Bug entfernt: Kulturen-Persistenz korrigiert (L/S Sprachen)
// Person-Vergleich korrigiert
// Dateibegrenzung auf 4MB erhöht
// XML geändert: Sprachen jetzt mit "Sprachen kennen"-ID bei RKP
//
// Revision 1.19  2003/01/26 14:27:29  vrandezo
// Astralenergie auf 0 gesetzt
// Helden haben jetzt Raum für Notizen
//
// Revision 1.18  2003/01/18 18:10:42  vrandezo
// Personen können Sprachen und Schriften
// übliche Kulturen bei Rassen hinzugefügt
// GUI für mögliche und übliche Kulturen und für mögliche Professionen geändert
// Varianten ausblendbar bei der Eingabe möglicher Kulturen und Professionen
//
// Revision 1.17  2003/01/16 22:57:35  vrandezo
// männliche und weibliche Bezeichnungen bei den Professionen
// Geschlechtsspezifische Personenkomponenten (als Varianten realisiert) implementiert
//
// Revision 1.16  2003/01/16 19:35:07  vrandezo
// Variantenbeziehung eintragbar
// Professionen jetzt in die Kategorien aus dem Regelbuch unterteilt
// GUI für Geschlechtsspezifika steht schon
//
// Revision 1.15  2003/01/15 20:02:30  vrandezo
// Voraussetzungen jetzt auch für Rassen und Professionen
// GUI für Modifikatoren bei Personenkomponenten verbessert
// Lesen von Modifikatoren erweitert
//
// Revision 1.14  2003/01/14 22:26:46  vrandezo
// Sprachen und Schriften jetzt auch bei den Personenkomponenten hinzufügbar
//
// Revision 1.13  2003/01/13 00:30:49  vrandezo
// Release Version 1.12 dev
// Korrektur der Verrechnungspunkteberechnung für negative Talentmodifikatoren
// Talentauswahl implementiert
//
// Revision 1.12  2003/01/11 20:30:57  vrandezo
// Auf wxWindows 2.4.0 gewechselt
// Beschreibungen bei Personenkomponenten und Talentoiden hinzugefügt
// PersonenkomponentenGUI verändert
//
// Revision 1.11  2002/12/06 15:28:00  vrandezo
// Spezies bei den Rassen einstellbar
// Kampftechniken bei PK und bei Person hinzugefügt
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
// Revision 1.8  2002/10/25 14:21:12  vrandezo
// Hauptmenü erweitert
// Charakteristika hinzugefügt
// eigenes Mengen-GUI-Element erzeugt
//
// Revision 1.7  2002/10/19 20:35:32  vrandezo
// feste Stringliterale entfernt
//
// Revision 1.6  2002/10/18 00:50:16  vrandezo
// Talente bei der Heldenerschaffung einsetzbar
//
// Revision 1.5  2002/10/13 15:18:42  vrandezo
// Voraussetzungen für Professionen implementiert
// CVS aufgeräumt für Modifkatoren und Voraussetzungen
//
// Revision 1.4  2002/10/11 14:13:21  vrandezo
// Modifikatoren für Eigenschaften und Abgeleitete Werte eingeführt
//
// Revision 1.3  2002/10/10 21:41:38  vrandezo
// PersonenKomponente als Grundlage für Rasse, Kultur und Profession eingeführt
// Toten Code aus dem Projekt entfernt (noch nicht aus dem CVS)
//
// Revision 1.2  2002/08/08 18:51:09  vrandezo
// da Xerces und andere XML-Parser XPath nicht
// unterstützen, wurde die Navigation im DOM-Baum
// von XPath auf eine generische Variante umgestellt.
// Dazu mussten allen Schnittstellen verändert werden.
//
// Revision 1.1  2002/07/17 18:29:55  Zdenko Vrandecic
// Einchecken
//}
//@nonl
//@-node:<< Version History >>
//@nl
//@nonl
//@-node:@file PersonenKomponente.cpp
//@-leo
