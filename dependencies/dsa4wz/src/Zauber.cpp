//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file Zauber.cpp
//@+at
// Zauber.cpp: Implementierung von Zaubern.
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
#include "Texte.h"
#include "Log.h"
#include "NDSAUtil.h"

#include "Person.h"

#include "Zauber.h"
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

  ZauberDefinitionen* zaubersprueche() {
    static ZauberDefinitionen* zauberspruche_ = new ZauberDefinitionen();
    return zauberspruche_;
  }  
  
	//@  @+others
	//@+node:old
	//@+others
	//@+node:Merkmale
	//@+at
	// StringListe const * const Merkmale() {
	//   static StringListe merkmale_;
	//   if (merkmale_.size()==0) {
	//     merkmale_.push_back(_t("Antimagie")); // TODO 
	// Hardcoded
	//     merkmale_.push_back(_t("Beschwörung"));
	//     merkmale_.push_back(_t("Dämonisch"));
	//     merkmale_.push_back(_t("Dämonisch (Blakharaz)")); 
	// // TODO Domänen hardcoded
	//     merkmale_.push_back(_t("Dämonisch (Belhalhar)"));
	//     merkmale_.push_back(_t("Dämonisch 
	// (Charyptoroth)"));
	//     merkmale_.push_back(_t("Dämonisch 
	// (Lolgramoth)"));
	//     merkmale_.push_back(_t("Dämonisch 
	// (Thargunitoth)"));
	//     merkmale_.push_back(_t("Dämonisch (Amazeroth)"));
	//     merkmale_.push_back(_t("Dämonisch (Nagrach)"));
	//     merkmale_.push_back(_t("Dämonisch (Asfaloth)"));
	//     merkmale_.push_back(_t("Dämonisch 
	// (Tasfarelel)"));
	//     merkmale_.push_back(_t("Dämonisch (Mishkhara)"));
	//     merkmale_.push_back(_t("Dämonisch (Agrimoth)"));
	//     merkmale_.push_back(_t("Dämonisch (Belkelel)"));
	//     merkmale_.push_back(_t("Eigenschaften"));
	//     merkmale_.push_back(_t("Einfluss"));
	//     merkmale_.push_back(_t("Elementar"));
	//     merkmale_.push_back(_t("Elementar (Feuer)")); // 
	// TODO Elemente hardcoded
	//     merkmale_.push_back(_t("Elementar (Wasser)"));
	//     merkmale_.push_back(_t("Elementar (Luft)"));
	//     merkmale_.push_back(_t("Elementar (Erz)"));
	//     merkmale_.push_back(_t("Elementar (Humus)"));
	//     merkmale_.push_back(_t("Elementar (Eis)"));
	//     merkmale_.push_back(_t("Form"));
	//     merkmale_.push_back(_t("Geisterwesen"));
	//     merkmale_.push_back(_t("Heilung"));
	//     merkmale_.push_back(_t("Hellsicht"));
	//     merkmale_.push_back(_t("Herbeirufung"));
	//     merkmale_.push_back(_t("Herrschaft"));
	//     merkmale_.push_back(_t("Illusion"));
	//     merkmale_.push_back(_t("Kraft"));
	//     merkmale_.push_back(_t("Limbus"));
	//     merkmale_.push_back(_t("Metamagie"));
	//     merkmale_.push_back(_t("Objekt"));
	//     merkmale_.push_back(_t("Schaden"));
	//     merkmale_.push_back(_t("Telekinese"));
	//     merkmale_.push_back(_t("Temporal"));
	//     merkmale_.push_back(_t("Umwelt"));
	//     merkmale_.push_back(_t("Verständigung"));
	//   }
	//   return &merkmale_;
	// } // Merkmale
	//@-at
	//@@c
	//@nonl
	//@-node:Merkmale
	//@+node:Repraesentationen
	//@+at
	// StringListe const * const Repraesentationen(bool 
	// lang) {
	//   static StringListe lang_;
	//   static StringListe kurz_;
	//   if (lang_.size()==0) {
	//     lang_.push_back(_t("Gildenmagier"));
	//     lang_.push_back(_t("Elfen"));
	//     lang_.push_back(_t("Druiden"));
	//     lang_.push_back(_t("Hexen"));
	//     lang_.push_back(_t("Geoden"));
	//     lang_.push_back(_t("Schelme"));
	//     lang_.push_back(_t("Scharlatane"));
	//     lang_.push_back(_t("Borbaradianer"));
	//     lang_.push_back(_t("Achaz-Kristallomanten"));
	//     for (StringListe::const_iterator sli = 
	// lang_.begin(); sli != lang_.end(); ++sli)
	//       kurz_.push_back(RepraesentationKurz(*sli));
	//   }
	//   if (lang)
	//     return &lang_;
	//   else
	//     return &kurz_;
	// } // Repraesentationen
	//@-at
	//@@c
	//@nonl
	//@-node:Repraesentationen
	//@+node:RepraesentationKurz
	//@+at
	// std::string RepraesentationKurz(const std::string& 
	// lang) {
	//   if (lang==_t("Gildenmagier"))          return 
	// _t("Mag");
	//   if (lang==_t("Elfen"))                 return 
	// _t("Elf");
	//   if (lang==_t("Druiden"))               return 
	// _t("Dru");
	//   if (lang==_t("Hexen"))                 return 
	// _t("Hex");
	//   if (lang==_t("Geoden"))                return 
	// _t("Geo");
	//   if (lang==_t("Schelme"))               return 
	// _t("Sch");
	//   if (lang==_t("Scharlatane"))           return 
	// _t("Srl");
	//   if (lang==_t("Borbaradianer"))         return 
	// _t("Bor");
	//   if (lang==_t("Achaz-Kristallomanten")) return 
	// _t("Ach");
	//   Log::Error(_t("In Zauber.cpp: 
	// DSA::RepraesentationKurz - unbekannte 
	// Repräsentation"));
	//   return lang;
	// } // RepraesentationKurz
	//@-at
	//@@c
	//@nonl
	//@-node:RepraesentationKurz
	//@+node:RepraesentationLang
	//@+at
	// std::string RepraesentationLang(const std::string& 
	// kurz) {
	//   if (kurz==_t("Mag")) return _t("Gildenmagier");
	//   if (kurz==_t("Elf")) return _t("Elfen");
	//   if (kurz==_t("Dru")) return _t("Druiden");
	//   if (kurz==_t("Hex")) return _t("Hexen");
	//   if (kurz==_t("Geo")) return _t("Geoden");
	//   if (kurz==_t("Sch")) return _t("Schelme");
	//   if (kurz==_t("Srl")) return _t("Scharlatane");
	//   if (kurz==_t("Bor")) return _t("Borbaradianer");
	//   if (kurz==_t("Ach")) return 
	// _t("Achaz-Kristallomanten");
	//   Log::Error(_t("In Zauber.cpp: 
	// DSA::RepraesentationKurz - unbekannte 
	// Repräsentation"));
	//   return kurz;
	// } // RepraesentationLang
	//@-at
	//@@c
	//@nonl
	//@-node:RepraesentationLang
	//@-others
	//@nonl
	//@-node:old
	//@+node:Merkmal
	void Merkmal::ID(const std::string& id) {
	  Merkmal_(ZaubermerkmalString(id));
	}
	
	std::string Merkmal::ID() const {
	  return ZaubermerkmalString(merkmal_, false);
	}
	
	bool Merkmal::operator==(const Merkmal& other) const {
	  if (merkmal_!=other.merkmal_) return false;
	  if (merkmal_==Daemonisch)
	    return (domaene_==other.domaene_);
	  if (merkmal_==element_)
	    return (element_==other.element_);
	  return true;
	}
	
	//@+others
	//@+node:Text
	std::string Merkmal::Text() const {
	  std::string rv = ZaubermerkmalString(merkmal_);
	  if (merkmal_==Daemonisch)
	    rv += " (" + ErzdaemonString(domaene_) + ")";
	  if (merkmal_==Elementar)
	    rv += " (" + ElementString(element_) + ")";
	  return rv;
	} // Text
	//@nonl
	//@-node:Text
	//@+node:Lese
	int Merkmal::Lese(PersistenzManager* pm, const std::string& id ) {
		std::string geleseneID = id;
		int rv = pm->OeffneZumLesen(Klasse(), geleseneID);
	  Zaubermerkmal merkmal = ZaubermerkmalString(geleseneID);
	  std::string sbuf;
	  Erzdaemon domaene = GesamtDaemonisch;
		DSA::Element element = GesamtElementar;
	  if (merkmal==Daemonisch) {
	    rv += pm->Lese(("Domaene"), sbuf);
	    domaene = ErzdaemonString(sbuf);
	  }
	  if (merkmal==Elementar) {
	    rv += pm->Lese(("Element"), sbuf);
	    element = ElementString(sbuf);
	  }
	
		if (!rv) {
	    Merkmal_(merkmal);
	    Domaene(domaene);
	    Element(element);
		}
		rv += pm->SchliesseNachLesen();
		return rv; // TODO
	} // Lese
	
	//@-node:Lese
	//@+node:Schreibe
	int Merkmal::Schreibe(PersistenzManager* pm) const {
		int rv = pm->OeffneZumSchreiben(Klasse(), ID());
	  if (merkmal_==Daemonisch)
	    rv += pm->Schreibe("Domaene", ErzdaemonString(domaene_, false));
	  if (merkmal_==Elementar)
	    rv += pm->Schreibe("Element", ElementString(element_, false));
		rv += pm->SchliesseNachSchreiben();
		return rv;
	} // Schreibe
	//@nonl
	//@-node:Schreibe
	//@+node:Pruefe
	int Merkmal::Pruefe() {
	  int rv = 0;
		return rv; // hier kann irgendwie nix schiefgehen, oder?
	} // Pruefe
	//@nonl
	//@-node:Pruefe
	//@+node:IDListe
	int Merkmal::IDListe(PersistenzManager* pm, StringListe* liste) {
		return pm->IDListe(Klasse(), liste); // TODO Prüfen
	} // IDListe
	//@nonl
	//@-node:IDListe
	//@-others
	//@-node:Merkmal
	//@+node:Verbreitung
	void Verbreitung::ID(const std::string& id) {
	  Gruppe(ZauberrepraesentationString(id));
	}
	
	std::string Verbreitung::ID() const {
	  return ZauberrepraesentationString(gruppe_, true, false);
	}
	
	void Verbreitung::Repraesentation(const Zauberrepraesentation rep) {
	  repraesentation_ = rep;
	}
	
	Zauberrepraesentation Verbreitung::Repraesentation() const {
	  return repraesentation_;
	}
	
	void Verbreitung::Gruppe(const Zauberrepraesentation rep) {
	  gruppe_ = rep;
	}
	
	Zauberrepraesentation Verbreitung::Gruppe() const {
	  return gruppe_;
	}
	
	//@+others
	//@+node:Text
	std::string Verbreitung::Text() const {
	  std::string rv = ZauberrepraesentationString(gruppe_, false) + " ";
	  if (Repraesentation()!=Gruppe())
	    rv += "(" + ZauberrepraesentationString(Repraesentation(), false) + ")" + " ";
	  rv += strings::int2sStr(Wert());
	  return rv;
	} // Text
	//@nonl
	//@-node:Text
	//@+node:Lese
	int Verbreitung::Lese(PersistenzManager* pm, const std::string& id ) {
		std::string geleseneID = id;
		int rv = pm->OeffneZumLesen(Klasse(), geleseneID);
	  Zauberrepraesentation gruppe = ZauberrepraesentationString(geleseneID);
		int wert;
		rv += pm->Lese(("Wert"), wert);
	  Zauberrepraesentation repraesentation = gruppe;
	  std::string sbuf;
	  if (pm->Existiert(("Repräsentation"))) {
	    rv += pm->Lese(("Repräsentation"), sbuf);
	    repraesentation = ZauberrepraesentationString(sbuf);
	  }
	
		if (!rv) {
	    Gruppe(gruppe);
	    Wert(wert);
	    Repraesentation(repraesentation);
		}
		rv += pm->SchliesseNachLesen();
		return rv; // TODO
	} // Lese
	//@nonl
	//@-node:Lese
	//@+node:Schreibe
	int Verbreitung::Schreibe(PersistenzManager* pm) const {
		int rv = pm->OeffneZumSchreiben(Klasse(), ID());
		rv += pm->Schreibe(("Wert"), Wert());
	  if (Repraesentation()!=Gruppe())
	    rv += pm->Schreibe(("Repräsentation"), ZauberrepraesentationString(Repraesentation(), true, false));
		rv += pm->SchliesseNachSchreiben();
		return rv;
	} // Schreibe
	//@nonl
	//@-node:Schreibe
	//@+node:Pruefe
	int Verbreitung::Pruefe() {
	  int rv = 0;
	//  const StringListe* rep = Repraesentationen();
	//  if (std::find(rep->begin(), rep->end(), ID())==rep->end()) {
	//    Log::Info(_t("Unbekannte Verbreitung") + " ");
	//    ++rv;
	//  }
	  if ((Wert()<1)||(Wert()>7)) { // TODO Konstanten
	    Log::Info("Verbreitung außerhalb der erlaubten Werte");
	    ++rv;
	  }
	//  if (std::find(rep->begin(), rep->end(), Repraesentation())==rep->end()) {
	//    Log::Info(_t("Unbekannte Repräsentation") + " ");
	//    ++rv;
	//  }
	  if (rv!=0) Log::Info("in " + Klasse() + " " + ID());
		return rv;
	} // Pruefe
	//@nonl
	//@-node:Pruefe
	//@+node:IDListe
	int Verbreitung::IDListe(PersistenzManager* pm, StringListe* liste) {
		return pm->IDListe(Klasse(), liste); // TODO Prüfen
	} // IDListe
	//@nonl
	//@-node:IDListe
	//@-others
	//@-node:Verbreitung
	//@+node:ZauberDefinition
	//@+others
	//@+node:Varianten
	//@+at
	// Gibt eine Liste der Bezeichner der verfügbaren 
	// Varianten zurück, in der zur Zeit eingestellten 
	// Sprache.
	//@-at
	//@@c
	StringListe ZauberDefinition::Varianten() const {
	  StringListe sl;
	  forallconst(DDict, i, varianten_) {
	    Dict::const_iterator j = i->second.find(DSA::Lang());
	    if (j != i->second.end())
	      sl.push_back(j->second);
	    else {
	      j = i->second.find(DSA::StandardLang());
	      if (j != i->second.end())
	        sl.push_back(j->second);
	      else
	        if (i->second.size() > 0)
	          sl.push_back(i->second.begin()->second);
	    }
	  }
	  return sl;
	}
	
	//@+at
	// Setzt eine neue Variante hinein. Name wird als der 
	// Bezeichner fuer die zur Zeit eingestellte Sprache 
	// verwendet, und aus ihm wird auch die ID erstellt.
	//@-at
	//@@c
	void ZauberDefinition::VarianteHinzu(const std::string& name) {
	  Dict d;
	  d[DSA::Lang()] = name;
	  std::string spezid = strings::MacheID("Variante", ID() + name);
	  varianten_[spezid] = d;
	}
	
	//@+at
	// Löscht eine Variante über den Bezeichner.
	//@-at
	//@@c
	void ZauberDefinition::VarianteWeg(const std::string& name) {
	  std::string key = Schluessel(name, varianten_);
	  if (key!="") {
	    varianten_.erase(varianten_.find(key));
	    return;
	  }
	  DDict::iterator i = varianten_.find(name);
	  if (i != varianten_.end())
	    varianten_.erase(i);
	}
	
	//@+at
	// Benennt eine Variante um. Hierbei wird der alte Name 
	// und der neue Name angegeben.
	//@-at
	//@@c
	void ZauberDefinition::VarianteUmbenennen(const std::string& alterName, const std::string& neuerName) {
	  std::string key = Schluessel(alterName, varianten_);
	  if (key!="") {
	    varianten_[key].operator[](DSA::Lang()) = neuerName;
	    return;
	  }
	  DDict::iterator i = varianten_.find(alterName);
	  if (i != varianten_.end())
	    varianten_[alterName].operator[](DSA::Lang()) = neuerName;
	}
	//@nonl
	//@-node:Varianten
	//@+node:Eigenschaften
	void ZauberDefinition::Eigenschaften(Eigenschaftsart e1, Eigenschaftsart e2, Eigenschaftsart e3) {
		eigenschaft_[0] = e1;
		eigenschaft_[1] = e2;
		eigenschaft_[2] = e3;
	} // Eigenschaften
	//@nonl
	//@-node:Eigenschaften
	//@+node:Eigenschaft
	Eigenschaftsart ZauberDefinition::Eigenschaft(int i) const {
		if (i < 0) return unbekannteEigenschaft;
		if (i >= EigenschaftenProTalentprobe) return unbekannteEigenschaft;
		return eigenschaft_[i];
	} // Eigenschaft
	//@nonl
	//@-node:Eigenschaft
	//@+node:ProbeText
	std::string ZauberDefinition::ProbeText(const std::string& trennzeichen, bool uebersetzt) const {
		std::string s = EigenschaftString(eigenschaft_[0], false, uebersetzt);
		for (int i = 1; i != DSA::EigenschaftenProTalentprobe; ++i)
			s += trennzeichen + EigenschaftString(eigenschaft_[i], false, uebersetzt);
		return s;
	} // ProbeText
	//@nonl
	//@-node:ProbeText
	//@+node:VerfuegbareRepraesentationen
	std::vector<Zauberrepraesentation> ZauberDefinition::VerfuegbareRepraesentationen() const {
	  std::vector<Zauberrepraesentation> reps;
	  forallconst (Verbreitungen, v, verbreitung_)
	    if (std::find(reps.begin(), reps.end(), v->second.Repraesentation())==reps.end())
	      reps.push_back(v->second.Repraesentation());
	  return reps;
	}
	//@nonl
	//@-node:VerfuegbareRepraesentationen
	//@+node:Lese
	int ZauberDefinition::Lese(PersistenzManager* pm, const std::string& id) {
		std::string geleseneID = id;
		int rv = pm->OeffneZumLesen(Klasse(), geleseneID);
	  Dict bezeichnung;
	  Dict beschreibung;
	  rv += LeseBezeichnung(pm, geleseneID, bezeichnung, beschreibung);
		std::string sBuf;
	  int temp  = pm->Lese(("Komplexitaet"), sBuf);
	  if (temp!=0)
	    rv += pm->Lese(("Komplexität"), sBuf);
		SKTSpalte komplexitaet = SKTSpalteString(sBuf);
		std::string probe;
		rv += pm->Lese(("Probe"), probe); // TODO checken, ob wirklich Probe vorhanden
		Eigenschaftsart eigenschaft[EigenschaftenProTalentprobe];
		if (!rv) for (int i=0; i != EigenschaftenProTalentprobe; ++i)
			eigenschaft[i] = EigenschaftString(probe.substr(i*3, 2)); // TODO try/catch?
	  Merkmale merkmale;
		//@  << LeseMerkmale >>
		//@+node:<< LeseMerkmale >>
		if (pm->Existiert("Merkmalliste")) {
		  rv += merkmale.Lese(pm);
		} else {
		  StringListe m;
		  rv += pm->Lese(m, ("Merkmal"), ("Merkmale"));
		  for (StringListe::const_iterator sli = m.begin(); sli != m.end(); ++sli) {
		    Merkmal mm;
		    if (strings::BeginntMit(*sli, ZaubermerkmalString(Daemonisch, false))) {
		      mm.Merkmal_(Daemonisch);
		      mm.Domaene(ErzdaemonString(strings::InKlammern(*sli)));
		    } else if (strings::BeginntMit(*sli, ZaubermerkmalString(Elementar, false))) {
		      mm.Merkmal_(Elementar);
		      mm.Element(ElementString(strings::InKlammern(*sli)));
		    } else {
		      mm.Merkmal_(ZaubermerkmalString(*sli));
		    }
		    merkmale.FuegeEin(mm);
		  }
		}
		//@-node:<< LeseMerkmale >>
		//@nl
	  DDict varianten;
	  rv += pm->Lese(varianten, ("Variante"), ("Varianten"), "", geleseneID);
	  Verbreitungen verbreitung;
	  rv += verbreitung.Lese(pm);
	
		if (!rv) {
			ID(geleseneID);
			Eigenschaften(eigenschaft[0], eigenschaft[1], eigenschaft[2]);
	    Komplexitaet(komplexitaet);
	    merkmale_ = merkmale;
	    varianten_ = varianten;
	    verbreitung_ = verbreitung;
	    bezeichnung_ = bezeichnung;
	    beschreibung_ = beschreibung;
		}
		rv += pm->SchliesseNachLesen();
		return rv; // TODO
	} // Lese
	//@nonl
	//@-node:Lese
	//@+node:Schreibe
	int ZauberDefinition::Schreibe(PersistenzManager* pm) const {
		int rv = pm->OeffneZumSchreiben(Klasse(), id_);
	  rv += SchreibeBezeichnung(pm);
		rv += pm->Schreibe(("Probe"), ProbeText("/", false));
		rv += pm->Schreibe(("Komplexität"), SKTSpalteString(Komplexitaet()));
	  rv += merkmale_.Schreibe(pm);
	  rv += pm->Schreibe(varianten_, ("Variante"), ("Varianten"));
	  rv += verbreitung_.Schreibe(pm);
		rv += pm->SchliesseNachSchreiben();
		return rv;
	} // Schreibe
	//@nonl
	//@-node:Schreibe
	//@+node:Pruefe
	int ZauberDefinition::Pruefe() {
	  int rv = 0;
	  for (int i=0; i != EigenschaftenProTalentprobe; ++i)
	    if (eigenschaft_[i]==unbekannteEigenschaft) {
	      Log::Info(("Unbekannte Eigenschaft"));
	      ++rv;
	    }
	  rv += verbreitung_.Pruefe();
	  rv += merkmale_.Pruefe();
	  if (rv!=0) Log::Info("in " + Klasse() + " " + ID());
		return rv;
	} // Pruefe
	//@nonl
	//@-node:Pruefe
	//@+node:IDListe
	int ZauberDefinition::IDListe(PersistenzManager* pm, StringListe* liste) {
		return pm->IDListe(Klasse(), liste); // TODO Prüfen
	} // IDListe
	//@nonl
	//@-node:IDListe
	//@-others
	//@nonl
	//@-node:ZauberDefinition
	//@+node:Zauberfertigkeit
	//@+others
	//@+node:ID
	std::string Zauberfertigkeit::ID() const {
	  return zauberspruch_ + " (" + ZauberrepraesentationString(repraesentation_) + ")";
	}
	
	void Zauberfertigkeit::ID(const std::string& id) {
	  if ((id.find("(")<id.size())&&(id.find(")")<id.size())) {
	    Repraesentation(ZauberrepraesentationString(id.substr(id.find_last_of("(")+1, id.find_last_of(")")-(id.find_last_of("(")+1))));
	    Zauberspruch(id.substr(0, id.find_last_of("(")-1));
	  }
	}
	//@-node:ID
	//@+node:Maximalwert
	int Zauberfertigkeit::Maximalwert() const {
	  if (person_ == 0) return 0;
	
	  forallconst (Sonderfertigkeiten, sf, person_->sonderfertigkeiten_)
	    if (sf->second.Definition()->IstRepraesentation())
	      if (sf->second.Definition()->Repraesentation() == Repraesentation())
	        return max(person_->Wert(Definition()->Eigenschaft(0)),
	                   person_->Wert(Definition()->Eigenschaft(1)),
	                   person_->Wert(Definition()->Eigenschaft(2))) + 3; // TODO Konstante
	
	  return min(person_->Wert(Definition()->Eigenschaft(0)),
	             person_->Wert(Definition()->Eigenschaft(1)),
	             person_->Wert(Definition()->Eigenschaft(2)));
	}
	//@nonl
	//@-node:Maximalwert
	//@+node:Extra
	void Zauberfertigkeit::Extra(const std::string& extra) {
	  if (extra==("Hauszauber"))
	    Hauszauber(true);
	  else
	    Log::Error("Zauberfertigkeit::Extra: Unbekanntes Extra " + extra);
	}
	//@nonl
	//@-node:Extra
	//@+node:Lernschwierigkeit
	SKTSpalte Zauberfertigkeit::Lernschwierigkeit() const {
	  SKTSpalte komp = Definition()->Komplexitaet();
	  if (person_ == 0) return komp;
	
	  int mod = 0;
	  
		//@  << Repräsentation >>
		//@+node:<< Repräsentation >>
		bool sch = (Repraesentation() == Schelme);
		bool srl = (Repraesentation() == Scharlatane);
		bool mag = (Repraesentation() == Gildenmagier);
		bool kannsch = false;
		bool kannsrl = false;
		bool kannmag = false;
		bool fremd = true; // gehen wir davon aus, dass dies nicht die eigene rep ist
		int anzahlrep = 0;
		
		forallconst (Sonderfertigkeiten, sf, person_->sonderfertigkeiten_)
		  if (sf->second.Definition()->IstRepraesentation()) {
		    if (sf->second.Definition()->Repraesentation() == Repraesentation())
		      fremd = false; // eigene rep!
		    kannsch = (sf->second.Definition()->Repraesentation() == Schelme);
		    kannsrl = (sf->second.Definition()->Repraesentation() == Scharlatane);
		    kannmag = (sf->second.Definition()->Repraesentation() == Gildenmagier);
		    ++anzahlrep;
		  }
		
		if (fremd)
		         if (mag && kannsrl) mod += 1;
		    else if (sch && kannsrl) mod += 2;
		    else if (srl && kannsch) mod += 2;
		    else if (sch && !kannsrl) mod += 3;
		    else { mod += 2; // Immer noch fremd, als um 2 Spalten erschwert
		           if (kannsch&&(anzahlrep==1)) ++mod; // bei (exklusiven) Schelmen um 3
		         }
		//@nonl
		//@-node:<< Repräsentation >>
		//@nl
	  
		//@  << Merkmale >>
		//@+node:<< Merkmale >>
		forallconst (Sonderfertigkeiten, sf, person_->sonderfertigkeiten_)
		  if (sf->second.Definition()->IstMerkmalskenntnis())
		    forallconst (Merkmale, m, Definition()->merkmale_)
		      if (m->second.Merkmal_()==Daemonisch) {
		        if (sf->second.Definition()->Merkmalskenntnis() == Daemonisch)
		          if (m->second.Domaene() == sf->second.Definition()->Domaene()) {
		            --mod;
		          if (sf->second.Definition()->Domaene() == GesamtDaemonisch)
		            --mod;
		        }
		      } else if (m->second.Merkmal_()==Elementar) {
		        if (sf->second.Definition()->Merkmalskenntnis() == Elementar) {
		          if (m->second.Element() == sf->second.Definition()->Element())
		            --mod;
		          if (sf->second.Definition()->Element() == GesamtElementar)
		            --mod;
		        }
		      } else if (m->second.Merkmal_() == sf->second.Definition()->Merkmalskenntnis())
		        --mod;      
		//@-node:<< Merkmale >>
		//@nl
	  
	  if (Hauszauber())
	    --mod;
	  
	  while (mod < 0) { komp = SpalteLeichter(komp); ++mod; }
	  while (mod > 0) { komp = SpalteSchwerer(komp); --mod; }
	  
	  return komp;
	}
	//@-node:Lernschwierigkeit
	//@+node:Text
	std::string Zauberfertigkeit::Text(int stil) const {
	  bool kurz = ((RepKurz & stil)!=0);
	  bool zeigen = ((ZeigeRep & stil)!=0);
	  bool nurExoten = ((RepNichtWennStandard & stil)!=0);
	  
	  std::string s = Zauberspruch();
	  std::string r = ZauberrepraesentationString(Repraesentation(), !kurz);
	  if (zeigen) // TODO checke Exoten
	    s += " (" + r + ")";
	  
	  return s + " " + strings::int2sStr(Wert(), true);
	} // Text
	//@-node:Text
	//@+node:ErsetzeTag
	std::string Zauberfertigkeit::ErsetzeTag(const std::string& tag) const {
	  if (""==tag) return Text();
	  if ("Name"==tag) return ID();
	  if ("W"==tag) return strings::int2sStr(Wert(), true);
	  if ("Probe"==tag) return Definition()->ProbeText();
	  if ("p1"==tag) return EigenschaftString(Definition()->Eigenschaft(0));
	  if ("p2"==tag) return EigenschaftString(Definition()->Eigenschaft(1));
	  if ("p3"==tag) return EigenschaftString(Definition()->Eigenschaft(2));
	  if ("K"==tag) return SKTSpalteString(Lernschwierigkeit());
	  if ("Beschreibung"==tag) return Definition()->Beschreibung();
	  return tag;
	} // ErsetzeTag
	//@nonl
	//@-node:ErsetzeTag
	//@+node:Definition
	const ZauberDefinition* Zauberfertigkeit::Definition() const {
	  // TODO bei unterschiedlichen Repraesentationen
	  ZauberDefinition* def = zaubersprueche()->Zeige(zauberspruch_);
	  if (def!=0) return def;
	  Log::Error("Zauber::Definition: Undefinierter Zauber " + ID());
	  static ZauberDefinition* sdef = new ZauberDefinition();
	  return sdef; // TODO workaround. Sollte bei nicht vorhandenen Defs intelligenter reagieren (doch wie?) // TODO Speicherleck
	} // Definition
	//@nonl
	//@-node:Definition
	//@+node:Lese
	int Zauberfertigkeit::Lese(PersistenzManager* pm, const std::string& id) {
	  std::string geleseneID = id;
	  int rv = pm->OeffneZumLesen(Klasse(), geleseneID);
	  std::string zauberspruch;
	  rv += pm->Lese(("Zauberspruch"), zauberspruch);
	  Zauberrepraesentation repraesentation;
	  std::string sbuf;
	  rv += pm->Lese(("Repräsentation"), sbuf);
	  repraesentation = ZauberrepraesentationString(sbuf);
	  int wert;
	  rv += pm->Lese(("Wert"), wert);
	  bool hauszauber = pm->Existiert(("Hauszauber"));
	  if (!rv) {
	    person_ = 0;
	    Zauberspruch(zauberspruch);
	    Repraesentation(repraesentation);
	    Wert(wert);
	  }
	  pm->SchliesseNachLesen();
	  return rv; // TODO
	} // Lese
	//@nonl
	//@-node:Lese
	//@+node:Schreibe
	int Zauberfertigkeit::Schreibe(PersistenzManager* pm) const {
	  int rv = pm->OeffneZumSchreiben(Klasse(), ID());
	  rv += pm->Schreibe(("Zauberspruch"), Zauberspruch());
	  rv += pm->Schreibe(("Repräsentation"), Repraesentation());
	  rv += pm->Schreibe(("Wert"), Wert());
	  if (Hauszauber()) {
	    rv += pm->OeffneZumSchreiben(("Hauszauber"));
	    rv += pm->SchliesseNachSchreiben();
	  }
	  rv += pm->SchliesseNachSchreiben();
	  return rv;
	} // Schreibe
	//@nonl
	//@-node:Schreibe
	//@+node:Pruefe
	int Zauberfertigkeit::Pruefe() {
	  int rv = 0;
	  if (zaubersprueche()->Zeige(ID()) == 0) {
	    Log::Info("Zauberfertigkeit::Pruefe: unbekannter Zauberspruch " + ID());
	    std::string s = zaubersprueche()->Finde(ID());
	    if (s != "") {
	      Log::Info("ist eigentlich " + s + " (wurde korrigiert)");
	      ID(s);
	    }
	    ++rv;
	  } else {
	    std::vector<Zauberrepraesentation> rep = Definition()->VerfuegbareRepraesentationen();
	    if (std::find(rep.begin(), rep.end(), Repraesentation())==rep.end()) {
	      Log::Info(_t("Unbekannte Repräsentation") + " " + ZauberrepraesentationString(Repraesentation()) + " " + _t("für") + " " + _t("Zauberspruch") + " " + Zauberspruch());
	      ++rv;
	    }
	  }
	  return rv;
	} // Pruefe
	//@-node:Pruefe
	//@+node:IDListe
	int Zauberfertigkeit::IDListe(PersistenzManager* pm, StringListe* liste) {
		return pm->IDListe(Klasse(), liste); // TODO Prüfen
	} // IDListe
	//@nonl
	//@-node:IDListe
	//@-others
	//@nonl
	//@-node:Zauberfertigkeit
	//@-others
}

//@<< Version History >>
//@+node:<< Version History >>
// $Log: Zauber.cpp,v $
// Revision 1.16  2004/10/17 19:12:40  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.15  2004/04/13 14:06:52  vrandezo
// Hauszauber implementiert
//
// Revision 1.14  2004/04/12 21:08:09  vrandezo
// Lehrsprachen implementiert
// Mutter-/Zweitsprachensystem umgebaut
//
// Revision 1.13  2004/04/12 13:20:23  vrandezo
// Merkmalskenntnisse implementiert
//
// Revision 1.12  2004/04/11 01:34:20  vrandezo
// Ausnahmeregelungen fuer die Verwandschaft der Repraesentationen
//
// Revision 1.11  2004/04/10 22:37:33  vrandezo
// Repraesentation erhoeht ZfW-Maximalwert
//
// Revision 1.10  2004/04/10 20:16:54  vrandezo
// Maximaler ZfW
//
// Revision 1.9  2004/04/10 01:26:08  vrandezo
// Helden erhalten ZF von RKP
//
// Revision 1.8  2004/04/09 22:25:50  vrandezo
// Zauberfertigkeiten fuer Personen eintragbar
//
// Revision 1.7  2004/01/29 09:06:20  dertron
// doppelte CVS Log-Tags entfernt
//
// Revision 1.6  2003/11/28 16:46:16  vrandezo
// Zeilenvorlauf am Dateiende (Warnung bei gcc)
//
// Revision 1.5  2003/11/20 00:35:07  vrandezo
// compilierbarkeit mit gcc verbessert
//
// Revision 1.4  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.3  2003/03/20 14:36:58  vrandezo
// Zauber: Repräsentationen & Verbreitung
//
// Revision 1.2  2003/03/19 11:27:46  vrandezo
// Merkmale bei Zaubern
// 30 Verrechnungspunkte für Erstprofessionen
//
// Revision 1.1  2003/03/15 14:20:25  vrandezo
// Bug: CTRL – X schließt das Programm nicht, dafür aber ALT – F4 (Achim Heidelberger)
// Zauber eingefügt: Probe, Beschreibung
//
//@nonl
//@-node:<< Version History >>
//@nl
//@-node:@file Zauber.cpp
//@-leo
