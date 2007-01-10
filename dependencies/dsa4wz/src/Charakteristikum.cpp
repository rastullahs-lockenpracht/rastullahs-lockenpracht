//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file Charakteristikum.cpp
//@+at
// Charakteristikum.cpp: Implementierung von 
// Charakteristika.
// 
// $Revision: 1.23 $
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

//@<< includes >>
//@+node:<< includes >>
#include "Charakteristikum.h"

#include "Log.h"
#include "NDSAUtil.h"

#include "PersistenzManager.h"
#include "Person.h"
#include <numeric>
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

	//@  << class CharakteristikumDefinition >>
	//@+node:<< class CharakteristikumDefinition >>
	//@+others
	//@+node:Bezeichnung
	std::string CharakteristikumDefinition::Bezeichnung(const std::string& sprache) const {
	  if (!IstGeneriert())
	    return Definition::Bezeichnung(sprache);
	    
	  if (IstHerausragendeEigenschaft())
	    return _t("Herausragende Eigenschaft") + " " + EigenschaftString(HerausragendeEigenschaft());
	    
	  if (IstMiserableEigenschaft())
	    return _t("Miserable Eigenschaft") + " " + EigenschaftString(MiserableEigenschaft());
	  
	  if (IstBegabungTalent())
	    if (talente()->Zeige(BegabungTalent())==0)
	      return _t("Begabung für") + " " + kampftechniken()->Zeige(BegabungTalent())->Bezeichnung();
	    else
	      return _t("Begabung für") + " " + talente()->Zeige(BegabungTalent())->Bezeichnung();
	  
	  if (IstUnfaehigkeitTalent())
	    if (talente()->Zeige(UnfaehigkeitTalent())==0)
	      return _t("Unfähigkeit für") + " " + kampftechniken()->Zeige(UnfaehigkeitTalent())->Bezeichnung();
	    else
	      return _t("Unfähigkeit für") + " " + talente()->Zeige(UnfaehigkeitTalent())->Bezeichnung();
	  
	  if (IstUnfaehigkeitSprachen())
	    return _t("Unfähigkeit für") + " " + _t("Sprachen");
	
	  if (IstBegabungTalentgruppe())
	    return _t("Begabung für") + " " + TalentgruppeString(BegabungTalentgruppe());
	  
	  if (IstUnfaehigkeitTalentgruppe())
	    return _t("Unfähigkeit für") + " " + TalentgruppeString(UnfaehigkeitTalentgruppe());
	  
	  return ID();
	}
	//@nonl
	//@-node:Bezeichnung
	//@+node:Semantische Eigenschaften
	//@+others
	//@+node:Eigenschaften
	bool CharakteristikumDefinition::IstHerausragendeEigenschaft() const {
	  if (!IstGeneriert()) return false;
	  return (strings::BeginntMit(ID(), "CharakteristikumHerausragendeEigenschaft"));
	}
	
	Eigenschaftsart CharakteristikumDefinition::HerausragendeEigenschaft() const {
	  if (!IstHerausragendeEigenschaft()) return unbekannteEigenschaft;
	  return EigenschaftString(strings::OhneAnfangExakt(ID(), "CharakteristikumHerausragendeEigenschaft"));
	}
	
	bool CharakteristikumDefinition::IstMiserableEigenschaft() const {
	  if (!IstGeneriert()) return false;
	  return (strings::BeginntMit(ID(), "CharakteristikumMiserableEigenschaft"));
	}
	
	Eigenschaftsart CharakteristikumDefinition::MiserableEigenschaft() const {
	  if (!IstHerausragendeEigenschaft()) return unbekannteEigenschaft;
	  return EigenschaftString(strings::OhneAnfangExakt(ID(), "CharakteristikumMiserableEigenschaft"));
	}
	//@-node:Eigenschaften
	//@+node:Talente
	bool CharakteristikumDefinition::IstBegabungTalent() const {
	  if (!IstGeneriert()) return false;
	  if (strings::BeginntMit(ID(), "CharakteristikumBegabungfuerTalentgruppe")) return false;
	  return (strings::BeginntMit(ID(), "CharakteristikumBegabungfuerTalent") || strings::BeginntMit(ID(), "CharakteristikumBegabungfuerKampftechnik"));
	}
	
	std::string CharakteristikumDefinition::BegabungTalent() const {
	  if (!IstBegabungTalent()) return "";
	  return (strings::OhneAnfangExakt(ID(), "CharakteristikumBegabungfuer"));
	}
	
	bool CharakteristikumDefinition::IstUnfaehigkeitTalent() const {
	  if (!IstGeneriert()) return false;
	  if (strings::BeginntMit(ID(), "CharakteristikumUnfaehigkeitfuerTalentgruppe")) return false;
	  return (strings::BeginntMit(ID(), "CharakteristikumUnfaehigkeitfuerTalent") || strings::BeginntMit(ID(), "CharakteristikumUnfaehigkeitfuerKampftechnik"));
	}
	
	std::string CharakteristikumDefinition::UnfaehigkeitTalent() const {
	  if (!IstUnfaehigkeitTalent()) return "";
	  return (strings::OhneAnfangExakt(ID(), "CharakteristikumUnfaehigkeitfuer"));
	}
	//@-node:Talente
	//@+node:Talentgruppen
	bool CharakteristikumDefinition::IstBegabungTalentgruppe() const {
	  if (!IstGeneriert()) return false;
	  if (strings::BeginntMit(ID(), "CharakteristikumBegabungfuerTalentgruppe")) return true;
	  if (strings::BeginntMit(ID(), "CharakteristikumBegabungfuerTalent")) return false;
	  if (strings::BeginntMit(ID(), "CharakteristikumBegabungfuerKampftechnik")) return false;
	  return (strings::BeginntMit(ID(), "CharakteristikumBegabungfuer"));
	}
	
	Talentgruppe CharakteristikumDefinition::BegabungTalentgruppe() const {
	  if (!IstBegabungTalentgruppe()) return sonstige;
	  return TalentgruppeString(strings::OhneAnfangExakt(ID(), "CharakteristikumBegabungfuer"));
	}
	
	bool CharakteristikumDefinition::IstUnfaehigkeitTalentgruppe() const {
	  if (!IstGeneriert()) return false;
	  if (strings::BeginntMit(ID(), "CharakteristikumUnfaehigkeitfuerTalent")) return true;
	  if (strings::BeginntMit(ID(), "CharakteristikumUnfaehigkeitfuerTalent")) return false;
	  if (strings::BeginntMit(ID(), "CharakteristikumUnfaehigkeitfuerKampftechnik")) return false;
	  return (strings::BeginntMit(ID(), "CharakteristikumUnfaehigkeitfuer"));
	}
	
	Talentgruppe CharakteristikumDefinition::UnfaehigkeitTalentgruppe() const {
	  if (!IstUnfaehigkeitTalentgruppe()) return sonstige;
	  return TalentgruppeString(strings::OhneAnfangExakt(ID(), "CharakteristikumUnfaehigkeitfuer"));
	}
	
	bool CharakteristikumDefinition::IstUnfaehigkeitSprachen() const {
	  if (!IstGeneriert()) return false;
	  return (ID() == "CharakteristikumUnfaehigkeitfuerSprachen");
	}
	//@-node:Talentgruppen
	//@-others
	//@-node:Semantische Eigenschaften
	//@+node:Text
	std::string CharakteristikumDefinition::Text(const Person& p) const {
	  std::string s = ID();
	  Charakteristikum ch(this);
	  s += " (" + strings::int2sStr(ch.GPKosten(p)) + " " + _t("GP");
	  if (HatStufen()) s += " " + _t("je Stufe");
	  if (SchlechteEigenschaft() && !GPPro2Punkte()) s += " " + _t("je Punkt");
	  if (SchlechteEigenschaft() && GPPro2Punkte()) s += " " + _t("je 2 Punkte");
	  s += ")";
	  if (Unsichtbar()) s = "(" + s + ")";
	
	  return s;
	} // Text
	//@-node:Text
	//@+node:Hinzufuegbar
	bool CharakteristikumDefinition::Hinzufuegbar(const Person& p) const {
	  if (Gebunden()) return false;
	  if (!voraussetzungen_.Erfuellt(p)) return false;
	  if (IstBasis()) if (p.charakteristika_.Existiert(BasisFuer())) return false;
	
	  // Herausragende Eigenschaften
	  if (IstHerausragendeEigenschaft()) {
	    Eigenschaftsart e = HerausragendeEigenschaft();
	    if (!((p.Wert(e)>=p.Basiswert(e))&&(p.Basiswert(e)>=regeln("Maximaler Startwert einer Eigenschaft"))))
	      return false;
	  }
	
	  // Miserable Eigenschaften
	  if (IstMiserableEigenschaft()) {
	    Eigenschaftsart e = MiserableEigenschaft();
	    if (!((p.Wert(e)<=p.Basiswert(e))&&(p.Basiswert(e)<=regeln("Minimaler Startwert einer Eigenschaft"))))
	      return false;
	  }
	
	  return !p.charakteristika_.Existiert(ID());
	} // Hinzufuegbar
	//@-node:Hinzufuegbar
	//@+node:BasisFuer
	std::string CharakteristikumDefinition::BasisFuer() const {
	  for (CharakteristikumDefinitionen::iterator c = charakteristika()->begin(); c != charakteristika()->end(); ++c)
	    if (c->second.BasiertAuf()==ID())
	      return c->first;
	  return "";
	} // BasisFuer
	//@nonl
	//@-node:BasisFuer
	//@+node:Lese
	int CharakteristikumDefinition::Lese(PersistenzManager* pm, const std::string& id) {
		std::string geleseneID = id;
		int rv = pm->OeffneZumLesen(("Charakteristikum"), geleseneID);
	  Dict bezeichnung;
	  Dict beschreibung;
	  rv += LeseBezeichnung(pm, geleseneID, bezeichnung, beschreibung);
		int gp;
		rv += pm->Lese(("GP"), gp);
	  bool gebunden = pm->Existiert(("gebunden"));
	  bool unsichtbar = pm->Existiert(("unsichtbar"));
	  bool brauchtSpezifizierung = pm->Existiert(("brauchtSpezifizierung"));
	  bool schlechteEigenschaft = pm->Existiert(("SchlechteEigenschaft"));
	  bool gpPro2Punkte = pm->Existiert(("GPPro2Punkte"));
	  bool hatStufen = pm->Existiert(("Stufen"));
	  int stufe = 0;
	  if (hatStufen) rv += pm->Lese(("Stufen"), stufe);
	  Voraussetzungen voraussetzungen;
	  rv += voraussetzungen.Lese(pm);
	  std::string basiertAuf;
	  if (pm->Existiert(("Basis")))
	    rv += pm->Lese(("Basis"), basiertAuf);
	
		if (!rv) {
			ID(geleseneID);
			GP(gp);
	    Gebunden(gebunden);
	    Unsichtbar(unsichtbar);
	    BrauchtSpezifizierung(brauchtSpezifizierung);
	    HatStufen(hatStufen);
	    MaximalStufe(stufe);
	    voraussetzungen_ = voraussetzungen;
	    BasiertAuf(basiertAuf);
	   bezeichnung_ = bezeichnung;
	   beschreibung_ = beschreibung;
	    GPPro2Punkte(gpPro2Punkte);
	    SchlechteEigenschaft(schlechteEigenschaft);
		}
		rv += pm->SchliesseNachLesen();
		return rv; // TODO
	} // Lese
	//@nonl
	//@-node:Lese
	//@+node:Schreibe
	int CharakteristikumDefinition::Schreibe(PersistenzManager* pm) const {
		int rv = pm->OeffneZumSchreiben(Klasse(), id_);
	  rv += SchreibeBezeichnung(pm);
		rv += pm->Schreibe(("GP"), gp_);
	  if (Gebunden()) {
	    rv += pm->OeffneZumSchreiben(("gebunden"));
	    rv += pm->SchliesseNachSchreiben();
	  }
	  if (Unsichtbar()) {
	    rv += pm->OeffneZumSchreiben(("unsichtbar"));
	    rv += pm->SchliesseNachSchreiben();
	  }
	  if (BrauchtSpezifizierung()) {
	    rv += pm->OeffneZumSchreiben(("brauchtSpezifizierung"));
	    rv += pm->SchliesseNachSchreiben();
	  }
	  if (SchlechteEigenschaft()) {
	    rv += pm->OeffneZumSchreiben(("SchlechteEigenschaft"));
	    rv += pm->SchliesseNachSchreiben();
	  }
	  if (GPPro2Punkte()) {
	    rv += pm->OeffneZumSchreiben(("GPPro2Punkte"));
	    rv += pm->SchliesseNachSchreiben();
	  }
	  if (HatStufen())
	    rv += pm->Schreibe(("Stufen"), MaximalStufe());
	  voraussetzungen_.Schreibe(pm);
	  if (HatBasis())
	    rv += pm->Schreibe(("Basis"), BasiertAuf());
		rv += pm->SchliesseNachSchreiben();
		return rv; // TODO
	} // Schreibe
	//@nonl
	//@-node:Schreibe
	//@+node:Pruefe
	int CharakteristikumDefinition::Pruefe() {
	  int rv = 0;
	  if (SchlechteEigenschaft() && (GP()>0)) {
	    Log::Info(("Schlechte Eigenschaft kostet Punkte"));
	    ++rv;
	  }
	  if (GPPro2Punkte()&&!SchlechteEigenschaft()) {
	    Log::Info(("2 GP pro Stufe, aber nicht Schlechte Eigenschaft"));
	    ++rv;
	  }
	
	  if (SchlechteEigenschaft() && !Nachteil()) {
	    Log::Info(("Schlechte Eigenschaft, die kein Nachteil ist"));
	    ++rv;
	  }
	  if (MaximalStufe()<0) {
	    Log::Info(("Negative Maximale Stufe"));
	    ++rv;
	  }
	  if (HatStufen()&&SchlechteEigenschaft()) {
	    Log::Info(("Stufige Schlechte Eigenschaft"));
	    ++rv;
	  }
	  if (HatBasis()&&!charakteristika()->Existiert(BasiertAuf())) {
	    Log::Info("Basiert auf unbekannter Charakteristika " + BasiertAuf());
	    std::string s;
	    if ((s = charakteristika()->Finde(BasiertAuf())) != "") {
	      Log::Info("ist eigentlich " + s + " (wurde korrigiert)");
	      BasiertAuf(s);
	    }
	    else
	      ++rv;
	  }
	  if (Unsichtbar()&&!Gebunden()) {
	    Log::Info(("Ungebundene unsichtbare Charakteristika"));
	    ++rv;
	  }
	  if (Unsichtbar()&&GP()!=0) {
	    Log::Info(("Unsichtbare Charakteristika mit Kosten"));
	    ++rv;
	  }
	  rv += voraussetzungen_.Pruefe();
	  if (rv!=0) Log::Info("in " + Klasse() + " " + ID());
		return rv;
	} // Pruefe
	//@nonl
	//@-node:Pruefe
	//@+node:IDListe
	int CharakteristikumDefinition::IDListe(PersistenzManager* pm, StringListe* liste) {
		return pm->IDListe(("Charakteristikum"), liste); // TODO Prüfen
	} // IDListe
	//@nonl
	//@-node:IDListe
	//@-others
	//@nonl
	//@-node:<< class CharakteristikumDefinition >>
	//@nl
  
  
	CharakteristikumDefinitionen* charakteristika() 
		{ static CharakteristikumDefinitionen* charakteristika_ = new CharakteristikumDefinitionen(); return charakteristika_; }

	CharakteristikumDefinitionen* generierteCharakteristika() {
    static CharakteristikumDefinitionen* charakteristika_ = new CharakteristikumDefinitionen();
    return charakteristika_;
  } // generierteCharakteristika
  
	//@  << GeneriereCharakteristika >>
	//@+node:<< GeneriereCharakteristika >>
	void GeneriereCharakteristika() {
	
	  generierteCharakteristika()->clear();
	  CharakteristikumDefinition chdef;
	  chdef.IstGeneriert(true);
	  
		//@  << Herausragende und miserable Eigenschaften >>
		//@+node:<< Herausragende und miserable Eigenschaften >>
		// Herausragende Eigenschaften
		int i;
		chdef.HatStufen(true);
		for (i=0; i != AnzahlEigenschaften; ++i) {
		  std::string s = "CharakteristikumHerausragendeEigenschaft" + EigenschaftString((Eigenschaftsart)i, true, false);
		  
		  chdef.ID(s);
		  chdef.GP(8); // TODO Konstante
		  generierteCharakteristika()->FuegeEin(chdef);
		}
		
		chdef.HatStufen(false);
		
		// Miserable Eigenschaften
		for (i=0; i != AnzahlEigenschaften; ++i) {
		  std::string s = "CharakteristikumMiserableEigenschaft" + EigenschaftString(static_cast<Eigenschaftsart>(i), true, false);
		  chdef.ID(s);
		  chdef.GP(-10); // TODO Konstante
		  generierteCharakteristika()->FuegeEin(chdef);
		}
		//@nonl
		//@-node:<< Herausragende und miserable Eigenschaften >>
		//@nl
	  
		//@  << Begabung für Talente >>
		//@+node:<< Begabung für Talente >>
		// Begabung für Talente
		TalentDefinitionen::iterator tal;
		for (tal = talente()->begin(); tal != talente()->end(); ++tal) {
		  chdef.ID("CharakteristikumBegabungfuer" + tal->second.ID());
		  if (regeln()->Regeloption("Begabungen und Unfähigkeiten nach Errata")) {
		    chdef.GP(4); // TODO Konstante
		  } else {
		    chdef.GP(regeln()->Aktivierungsfaktor(tal->second.SKTSpalte_())*3); // TODO Konstante
		  }
		  chdef.voraussetzungen_.clear();
		  Voraussetzung v;
		  v.Kein(true);
		  v.ID("CharakteristikumBegabungfuer" + TalentgruppeString(tal->second.Gruppe(), false, false));
		  chdef.voraussetzungen_.FuegeEin(v);
		  v.ID("CharakteristikumUnfaehigkeitfuer" + TalentgruppeString(tal->second.Gruppe(), false, false));
		  chdef.voraussetzungen_.FuegeEin(v);
		  for (TalentDefinitionen::iterator t = talente()->begin(); t != talente()->end(); ++t)
		   if ((t->first!=tal->first)&&(t->first!=(("TalentLesenSchreiben")))) {
		      Voraussetzung v;
		      v.Kein(true);
		      v.ID("CharakteristikumBegabungfuer" + t->first);
		      chdef.voraussetzungen_.FuegeEin(v);
		    }
		  for (KampftechnikDefinitionen::iterator k = kampftechniken()->begin(); k != kampftechniken()->end(); ++k) {
		    Voraussetzung v;
		    v.Kein(true);
		    v.ID("CharakteristikumBegabungfuer" + k->first);
		    chdef.voraussetzungen_.FuegeEin(v);
		  }
		  if (tal->second.Art()==Basis) {
		    Voraussetzung v;
		    v.Kein(true);
		    v.ID("CharakteristikumUnfaehigkeitfuer" + tal->second.ID());
		    chdef.voraussetzungen_.FuegeEin(v);
		  }
		  if (chdef.ID()!=("CharakteristikumBegabungfuerTalentLesenSchreiben"))
		    generierteCharakteristika()->FuegeEin(chdef);
		}
		//@-node:<< Begabung für Talente >>
		//@nl
		//@  << Begabung für Kampftechniken >>
		//@+node:<< Begabung für Kampftechniken >>
		KampftechnikDefinitionen::iterator k;
		for (k = kampftechniken()->begin(); k != kampftechniken()->end(); ++k) {
		  chdef.ID("CharakteristikumBegabungfuer" + k->second.ID());
		  if (regeln()->Regeloption("Begabungen und Unfähigkeiten nach Errata")) {
		    chdef.GP(6); // TODO Konstante
		  } else {
		    chdef.GP(regeln()->Aktivierungsfaktor(k->second.SKTSpalte_())*3); // TODO Konstante
		  }
		  chdef.voraussetzungen_.clear();
		  Voraussetzung v;
		  v.Kein(true);
		  v.ID("CharakteristikumBegabungfuer" + TalentgruppeString(Kampf, false, false));
		  chdef.voraussetzungen_.FuegeEin(v);
		  v.ID("CharakteristikumUnfaehigkeitfuer" + TalentgruppeString(Kampf, false, false));
		  chdef.voraussetzungen_.FuegeEin(v);
		  for (TalentDefinitionen::iterator t = talente()->begin(); t != talente()->end(); ++t)
		   if (t->first!=(("TalentLesenSchreiben"))) {
		      Voraussetzung v;
		      v.Kein(true);
		      v.ID("CharakteristikumBegabungfuer" + t->first);
		      chdef.voraussetzungen_.FuegeEin(v);
		    }
		  for (KampftechnikDefinitionen::iterator kt = kampftechniken()->begin(); kt != kampftechniken()->end(); ++kt)
		    if (kt->first!=k->first) {
		      Voraussetzung v;
		      v.Kein(true);
		      v.ID("CharakteristikumBegabungfuer" + kt->first);
		      chdef.voraussetzungen_.FuegeEin(v);
		    }
		  if (k->second.Art()==Basis) {
		    Voraussetzung v;
		    v.Kein(true);
		    v.ID("CharakteristikumUnfaehigkeitfuer" + k->second.ID());
		    chdef.voraussetzungen_.FuegeEin(v);
		  }
		  generierteCharakteristika()->FuegeEin(chdef);
		}
		//@nonl
		//@-node:<< Begabung für Kampftechniken >>
		//@nl
		//@  << Begabung für Talentgruppe >>
		//@+node:<< Begabung für Talentgruppe >>
		// Begabungen für Talentgruppen
		for (i = 0; i != AnzahlTalentgruppen-2; ++i) {
		  std::string s = "CharakteristikumBegabungfuer" + TalentgruppeString(static_cast<Talentgruppe>(i), false, false);
		  chdef.ID(s);
		  chdef.voraussetzungen_.clear();
		  if ((static_cast<Talentgruppe>(i)==Kampf)||(((static_cast<Talentgruppe>(i)==Koerperlich))&&(!regeln()->Regeloption("Begabungen und Unfähigkeiten nach Errata"))))
		    chdef.GP(30); // TODO Konstante
		  else
		    chdef.GP(20); // TODO Konstante
		  for (int j = 0; j != AnzahlTalentgruppen-2; ++j)
		    if (i==j) {
		      Voraussetzung v;
		      v.Kein(true);
		      v.ID("CharakteristikumUnfaehigkeitfuer" + TalentgruppeString(static_cast<Talentgruppe>(i), false, false));
		      chdef.voraussetzungen_.FuegeEin(v);
		      if (static_cast<Talentgruppe>(i)==Kampf) {
		        for (KampftechnikDefinitionen::iterator k = kampftechniken()->begin(); k != kampftechniken()->end(); ++k) {
		          Voraussetzung v;
		          v.Kein(true);
		          v.ID("CharakteristikumBegabungfuer" + k->first);
		          chdef.voraussetzungen_.FuegeEin(v);
		        }
		      } else {
		        for (TalentDefinitionen::iterator t = talente()->begin(); t != talente()->end(); ++t)
		          if (t->second.Gruppe()==static_cast<Talentgruppe>(i)) {
		            Voraussetzung v;
		            v.Kein(true);
		            v.ID("CharakteristikumBegabungfuer" + t->first);
		            chdef.voraussetzungen_.FuegeEin(v);
		          }
		      }
		    } else {
		      Voraussetzung v;
		      v.Kein(true);          
		      v.ID("CharakteristikumBegabungfuer" + TalentgruppeString(static_cast<Talentgruppe>(j), false, false));
		      chdef.voraussetzungen_.FuegeEin(v);
		    }      
		  generierteCharakteristika()->FuegeEin(chdef);
		}
		//@nonl
		//@-node:<< Begabung für Talentgruppe >>
		//@nl
	  
		//@  << Unfähigkeit für Talente >>
		//@+node:<< Unfähigkeit für Talente >>
		for (tal = talente()->begin(); tal != talente()->end(); ++tal)
		  if (tal->second.Art()==Basis) {
		    chdef.ID("CharakteristikumUnfaehigkeitfuer" + tal->second.ID());
		    if (regeln()->Regeloption("Begabungen und Unfähigkeiten nach Errata")) {
		      chdef.GP(-2); // TODO Konstante
		    } else {
		      if (tal->second.Gruppe()==Koerperlich) {
		      if ((tal->first==("TalentSchwimmen"))
		        ||(tal->first==("TalentSichVerstecken"))
		        ||(tal->first==("TalentSingen"))
		        ||(tal->first==("TalentTanzen"))
		        ||(tal->first==("TalentZechen"))) // TODO Konstante Menge!
		          chdef.GP(-4); // TODO Konstante
		        else
		          chdef.GP(-8); // TODO Konstante
		      } else
		        chdef.GP(-4); // TODO Konstante
		    }
		    chdef.voraussetzungen_.clear();
		    Voraussetzung v;
		    v.Kein(true);
		    v.ID("CharakteristikumBegabungfuer" + TalentgruppeString(tal->second.Gruppe(), false, false));
		    chdef.voraussetzungen_.FuegeEin(v);
		    v.ID("CharakteristikumUnfaehigkeitfuer" + TalentgruppeString(tal->second.Gruppe(), false, false));
		    chdef.voraussetzungen_.FuegeEin(v);
		    v.ID("CharakteristikumBegabungfuer" + tal->second.ID());
		    chdef.voraussetzungen_.FuegeEin(v);
		    for (TalentDefinitionen::iterator t = talente()->begin(); t != talente()->end(); ++t)
		      if ((t->second.Art()==Basis)&&(t->first!=tal->first)) {
		        Voraussetzung v;
		        v.Kein(true);
		        v.ID("CharakteristikumUnfaehigkeitfuer" + t->first);
		        chdef.voraussetzungen_.FuegeEin(v);
		      }
		    for (KampftechnikDefinitionen::iterator k = kampftechniken()->begin(); k != kampftechniken()->end(); ++k)
		      if (k->second.Art()==Basis) {
		        Voraussetzung v;
		        v.Kein(true);
		        v.ID("CharakteristikumUnfaehigkeitfuer" + k->first);
		        chdef.voraussetzungen_.FuegeEin(v);
		      }
		    generierteCharakteristika()->FuegeEin(chdef);
		  }
		//@-node:<< Unfähigkeit für Talente >>
		//@nl
		//@  << Unfähigkeit für Kampftechniken >>
		//@+node:<< Unfähigkeit für Kampftechniken >>
		for (k = kampftechniken()->begin(); k != kampftechniken()->end(); ++k)
		  if (k->second.Art()==Basis) {
		    chdef.ID("CharakteristikumUnfaehigkeitfuer" + k->second.ID());
		    if (regeln()->Regeloption("Begabungen und Unfähigkeiten nach Errata")) {
		      chdef.GP(-3); // TODO Konstante
		    } else {
		      chdef.GP(-4); // TODO Konstante
		    }
		    chdef.voraussetzungen_.clear();
		    Voraussetzung v;
		    v.Kein(true);
		    v.ID("CharakteristikumBegabungfuer" + TalentgruppeString(Kampf, false, false));
		    chdef.voraussetzungen_.FuegeEin(v);
		    v.ID("CharakteristikumUnfaehigkeitfuer" + TalentgruppeString(Kampf, false, false));
		    chdef.voraussetzungen_.FuegeEin(v);
		    v.ID("CharakteristikumBegabungfuer" + k->second.ID());
		    chdef.voraussetzungen_.FuegeEin(v);
		    for (TalentDefinitionen::iterator t = talente()->begin(); t != talente()->end(); ++t) 
		      if (t->second.Art()==Basis) {
		        Voraussetzung v;
		        v.Kein(true);
		        v.ID("CharakteristikumUnfaehigkeitfuer" + t->first);
		        chdef.voraussetzungen_.FuegeEin(v);
		      }
		    for (KampftechnikDefinitionen::iterator kt = kampftechniken()->begin(); kt != kampftechniken()->end(); ++kt)
		      if ((kt->second.Art()==Basis)&&(kt->first!=k->first)) {
		        Voraussetzung v;
		        v.Kein(true);
		        v.ID("CharakteristikumUnfaehigkeitfuer" + kt->first);
		        chdef.voraussetzungen_.FuegeEin(v);
		      }
		    generierteCharakteristika()->FuegeEin(chdef);
		  }
		//@nonl
		//@-node:<< Unfähigkeit für Kampftechniken >>
		//@nl
		//@  << Unfähigkeit für Talentgruppen >>
		//@+node:<< Unfähigkeit für Talentgruppen >>
		for (i = 0; i != AnzahlTalentgruppen-2; ++i) {
		  std::string s = "CharakteristikumUnfaehigkeitfuer" + TalentgruppeString(static_cast<Talentgruppe>(i), false, false);
		  chdef.ID(s);
		  chdef.voraussetzungen_.clear();
		  if (regeln()->Regeloption("Begabungen und Unfähigkeiten nach Errata")) {
		    if (static_cast<Talentgruppe>(i)==Kampf)
		      chdef.GP(-15); // TODO Konstante
		    else
		      chdef.GP(-10); // TODO Konstante
		  } else {
		    if (static_cast<Talentgruppe>(i)==Koerperlich)
		      chdef.GP(-20); // TODO Konstante
		    else if (static_cast<Talentgruppe>(i)==Kampf)
		      chdef.GP(-15); // TODO Konstante
		    else
		      chdef.GP(-10); // TODO Konstante
		  }
		  for (int j = 0; j != AnzahlTalentgruppen-2; ++j)
		    if (i==j) {
		      Voraussetzung v;
		      v.Kein(true);
		      v.ID("CharakteristikumBegabungfuer" + TalentgruppeString(static_cast<Talentgruppe>(i), false, false));
		      chdef.voraussetzungen_.FuegeEin(v);
		      if (static_cast<Talentgruppe>(i)==Kampf) {
		        for (KampftechnikDefinitionen::iterator k = kampftechniken()->begin(); k != kampftechniken()->end(); ++k)
		          if (k->second.Art()==Basis) {
		            Voraussetzung v;
		            v.Kein(true);
		            v.ID("CharakteristikumUnfaehigkeitfuer" + k->first);
		            chdef.voraussetzungen_.FuegeEin(v);
		          }
		      } else {
		        for (TalentDefinitionen::iterator t = talente()->begin(); t != talente()->end(); ++t)
		          if (t->second.Gruppe()==static_cast<Talentgruppe>(i)) 
		            if (t->second.Art()==Basis) {
		              Voraussetzung v;
		              v.Kein(true);
		              v.ID("CharakteristikumUnfaehigkeitfuer" + t->first);
		              chdef.voraussetzungen_.FuegeEin(v);
		            }
		      }
		    } else {
		      Voraussetzung v;
		      v.Kein(true);          
		      v.ID("CharakteristikumUnfaehigkeitfuer" + TalentgruppeString(static_cast<Talentgruppe>(j), false, false));
		      chdef.voraussetzungen_.FuegeEin(v);
		    }
		  Voraussetzung v;
		  v.Kein(true);
		  v.ID(("CharakteristikumUnfaehigkeitfuerSprachen"));
		  chdef.voraussetzungen_.FuegeEin(v);
		  generierteCharakteristika()->FuegeEin(chdef);
		}
		//@-node:<< Unfähigkeit für Talentgruppen >>
		//@nl
		//@  << Unfähigkeit für Sprachen >>
		//@+node:<< Unfähigkeit für Sprachen >>
		{
		  std::string s = ("CharakteristikumUnfaehigkeitfuerSprachen");
		  chdef.ID(s);
		  chdef.voraussetzungen_.clear();
		  if (regeln()->Regeloption("Begabungen und Unfähigkeiten nach Errata"))
		    chdef.GP(-10); // TODO Konstante
		  else
		    chdef.GP(-5); // TODO Konstante
		  for (int j = 0; j != AnzahlTalentgruppen-2; ++j) {
		    Voraussetzung v;
		    v.Kein(true);          
		    v.ID("CharakteristikumBegabungfuer" + TalentgruppeString(static_cast<Talentgruppe>(j), false, false));
		    chdef.voraussetzungen_.FuegeEin(v);
		  }      
		  generierteCharakteristika()->FuegeEin(chdef);
		}
		//@nonl
		//@-node:<< Unfähigkeit für Sprachen >>
		//@nl
	  
	} // GeneriereCharakteristika
	
	
	//@-node:<< GeneriereCharakteristika >>
	//@nl
  
	//@  << class Charakteristikum >>
	//@+node:<< class Charakteristikum >>
	//@+others
	//@+node:Text
	std::string Charakteristikum::Text() const {
	  std::string s = ID();
	  if ((HatStufen())||(SchlechteEigenschaft()))
	    s = s + " " + strings::int2sStr(Stufe());
	  if (!(Spezifizierung()==""))
	    s = s + " (" + Spezifizierung() + ")";
	  return s;
	} // Text
	//@nonl
	//@-node:Text
	//@+node:ErsetzeTag
	std::string Charakteristikum::ErsetzeTag(const std::string& tag) const {
	  if (""==tag) return Text();
	  if ("Name"==tag) return ID();
	  if ("S"==tag) if (Stufe()!=0) return strings::int2sStr(Stufe()); else return "";
	  if ("Beschreibung"==tag) return Definition()->Beschreibung();
	  return tag;
	} // ErsetzeTag
	//@nonl
	//@-node:ErsetzeTag
	//@+node:GPKosten
	int Charakteristikum::GPKosten(const Person& p) const {
	  int kosten = Definition()->GP();
	
	  // Thorwaler: Bonus von 3GP auf Blutrausch und Kampfrausch
	  if ((ID()==("CharakteristikumKampfrausch"))
	    ||(ID()==("CharakteristikumBlutrausch")))
	   if ((p.Rasse_().ID()==("RasseThorwaler"))
	     ||(rassen()->Hole(p.Rasse_().ID()).VarianteVon()==("RasseThorwaler")))
	      kosten -= 3; // TODO Konstante
	
	  // Herausragende Eigenschaft
	  if (Definition()->IstHerausragendeEigenschaft()) {
	    int s = 2; // TODO Konstante
	    return kosten*Stufe() + (Stufe()-1)*Stufe()*s/2;
	  }
	  
	  // Astralmacht für Viertelzauberer
	  if (ID()==("CharakteristikumAstralmacht"))
	    if (p.charakteristika_.Existiert(("CharakteristikumViertelzauberer")))
	      kosten = 3; // TODO Konstante
	
	  // Astrale Regeneration für Viertelzauberer      
	  if (ID()==("CharakteristikumAstraleRegeneration"))
	    if (p.charakteristika_.Existiert(("CharakteristikumViertelzauberer")))
	      kosten = 7; // TODO Konstante
	
	  if (Stufe()==0)
	    return kosten;
	  else
	    if (Definition()->GPPro2Punkte())
	      return kosten*((Stufe()+1)/2);
	    else
	      return kosten*Stufe();
	} // GPKosten
	//@-node:GPKosten
	//@+node:Definition
	const CharakteristikumDefinition* Charakteristikum::Definition() const {
		CharakteristikumDefinition* def = charakteristika()->Zeige(id_);
		if (def!=0) return def;
	  def = generierteCharakteristika()->Zeige(id_);
		if (def!=0) return def;
		Log::Error("Charakteristikum::Definition: Undefiniertes Charakteristikum " + id_);
		static CharakteristikumDefinition* sdef = new CharakteristikumDefinition();
		return sdef; // TODO workaround. Sollte bei nicht vorhandenen Defs intelligenter
		// reagieren (doch wie?) // TODO Speicherleck
	} // Definition
	//@nonl
	//@-node:Definition
	//@+node:Lese
	int Charakteristikum::Lese(PersistenzManager* pm, const std::string& id) {
		std::string geleseneID = id;
		int rv = pm->OeffneZumLesen(("Charakteristikum"), geleseneID);
	  int stufe = 0;
	  if (pm->Existiert(("Stufe")))
	    rv += pm->Lese(("Stufe"), stufe);
	  std::string spezifizierung = "";
	  if (pm->Existiert(("Spezifizierung")))
	    rv += pm->Lese(("Spezifizierung"), spezifizierung);
	
		if (!rv) {
			ID(geleseneID);
	    Stufe(stufe);
	    Spezifizierung(spezifizierung);
		}
		pm->SchliesseNachLesen();
		return rv; // TODO
	} // Lese
	//@nonl
	//@-node:Lese
	//@+node:Schreibe
	int Charakteristikum::Schreibe(PersistenzManager* pm) const {
		int rv = pm->OeffneZumSchreiben(Klasse(), ID());
	  if (HatStufen()||SchlechteEigenschaft()) rv += pm->Schreibe(("Stufe"), Stufe());
	  if (Spezifizierung()!="") rv += pm->Schreibe(("Spezifizierung"), Spezifizierung());
		rv += pm->SchliesseNachSchreiben();
		return rv;
	} // Schreibe
	//@nonl
	//@-node:Schreibe
	//@+node:Pruefe
	int Charakteristikum::Pruefe() {
	  int rv = 0;
	  CharakteristikumDefinition const* def = Definition();
	  if (def == 0) {
	    Log::Info("Charakteristikum::Pruefe: unbekanntes Charakteristikum " + ID());
	    std::string s = charakteristika()->Finde(ID());
	    if (s != "") {
	      Log::Info("ist eigentlich " + s + " (wurde korrigiert)");
	      ID(s);
	    }
	    s = generierteCharakteristika()->Finde(ID());
	    if (s != "") {
	      Log::Info("ist eigentlich " + s + " (wurde korrigiert)");
	      ID(s);
	    }
	    ++rv;
	  }
	//@+at 
	//@nonl
	// kann so nicht geprüft werden, da Charakteristika in 
	// RKP durchaus leer sein dürfen
	//   if (def->BrauchtSpezifizierung())
	//     if (Spezifizierung()=="") {
	//       Log::Info(_t("leere Spezifizierung"));
	//       ++rv;
	//   }
	//@-at
	//@@c
	  if (rv!=0) Log::Info("in " + Klasse() + " " + ID());
		return rv;
	} // Pruefe
	
	//@-node:Pruefe
	//@+node:IDListe
	int Charakteristikum::IDListe(PersistenzManager* pm, StringListe* liste) {
		return pm->IDListe(("Charakteristikum"), liste); // TODO Prüfen
	} // IDListe
	//@nonl
	//@-node:IDListe
	//@-others
	//@nonl
	//@-node:<< class Charakteristikum >>
	//@nl
  
	//@  << class Charakteristika >>
	//@+node:<< class Charakteristika >>
	bool Charakteristika::Verfuegt(const std::string& id) const {
	  if (Existiert(id)) return true;
	  
	  for (const_iterator i = begin(); i != end(); ++i)
	    if (i->second.Definition()->BasiertAuf() == id)
	      return true;
	    
	  return false;
	}
	
	Charakteristika Charakteristika::Sichtbar() const {
	  Charakteristika chs;
	  Charakteristikum ch;
	  
	  for (const_iterator i = begin(); i != end(); ++i)
	    if (!i->second.Definition()->Unsichtbar())
	      chs.FuegeEin(i->second);
	      
	  return chs;
	} // Bereinige
	//@-node:<< class Charakteristika >>
	//@nl
  
}

//@<< Version History >>
//@+node:<< Version History >>
// $Log: Charakteristikum.cpp,v $
// Revision 1.23  2004/10/17 19:12:39  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.22  2004/04/29 00:53:45  gnaddelwarz
// stürzte unter Linux ab (Zugriff auf nicht vorhandene Eigenschaft)
//
// Revision 1.21  2004/04/10 20:21:25  vrandezo
// Astrale Regeneration fuer Viertelzauberer
//
// Revision 1.20  2004/04/06 13:57:58  vrandezo
// Begabung und Unfaehigkeit von Talenten und Talentgruppen jetzt
// nach Errata implementiert (Twel)
//
// Revision 1.19  2004/01/08 01:57:56  twel
// - Z&H Errata zu Begabung/Unfaehigkeit eingearbeitet bis auf Startboni/-mali
// - Verdoppelungsregel bringt 1/3 der GP statt 1/2 (auch Errata)
//
// Revision 1.18  2003/10/27 18:24:50  vrandezo
// unsichtbare Vor- und Nachteile
// Tänze fremder Repräsentationen teurer
//
// Revision 1.17  2003/10/25 22:33:27  vrandezo
// SF-Text
//
// Revision 1.16  2003/10/25 15:06:48  vrandezo
// Leonizing
// Charakteristika mit Spezifizierung
//
// Revision 1.15  2003/10/25 02:11:17  vrandezo
// generierte Sonderfertigkeiten
//
// Revision 1.14  2003/10/24 23:50:41  vrandezo
// Leonizing
// Vor-/Nachteile als Voraussetzung ermöglicht
// Astralmacht, ZVH-Zauberer
//
// Revision 1.13  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.12  2003/03/25 16:24:14  vrandezo
// Unfähigkeit für [Talentgruppe] und [Talent]
//
// Revision 1.11  2003/03/25 13:01:17  vrandezo
// Begabung für [Talent] jetzt auch für Kampffertigkeiten
//
// Revision 1.10  2003/03/25 12:41:48  vrandezo
// Begabung für [Talent]
//
// Revision 1.9  2003/03/25 09:37:37  vrandezo
// Begabung für [Talentgruppe]
//
// Revision 1.8  2003/03/24 20:22:56  vrandezo
// Layoutänderungen (viele Anregungen von Tim, Achim, Wolfgang, King Lui, Twelwan...)
// Neu: Miserable Eigenschaften
//
// Revision 1.7  2003/03/24 18:56:18  vrandezo
// Neue Vorteile: Herausragende Eigenschaften (Forumswunsch Sebastian)
//
// Revision 1.6  2003/03/04 15:23:14  vrandezo
// weitere Tags implementiert für die Druckvorlagen
//
// Revision 1.5  2003/02/24 22:03:32  vrandezo
// Feature Request: beim Auswählen der Charakteristika werden die GP mit
// angegeben (King Lui)
//
// Revision 1.4  2003/02/24 17:37:22  vrandezo
// Prüfen implementiert
//
// Revision 1.3  2003/02/20 13:53:22  vrandezo
// Vorteile: Beidhändig, Linkshänder, Blutrausch, Kampfrausch
// weitere SF
// Eigenschaften haben Maximalwerte
//
// Revision 1.2  2003/02/20 09:19:45  vrandezo
// Prüfe-Signatur geändert
//
// Revision 1.1.1.1  2003/02/17 15:32:39  vrandezo
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
// Revision 1.7  2003/02/13 13:34:36  vrandezo
// steigerbarer VN ohne Wert in Generierung
//
// Revision 1.6  2003/02/09 23:58:10  vrandezo
// - steigerbare VN ohne Wert (Balance->Herausragende Balance)
// - Person-Vergleich korrigiert
// - Voraussetzung-Lesen/Schreiben korrigiert
//
// Revision 1.5  2003/02/08 00:22:51  vrandezo
// VN von RKP bei Generierung übernommen
// mehrfache VN bei RKP geben GP
// erweitere Voraussetzungen: nicht VN
// entfernbare VN
// auswählbare VN
// VN mit Voraussetzungen (nicht VN)
// Geschlecht neuerschaffener Helden ist zufällig
//
// Revision 1.4  2003/02/07 19:37:17  vrandezo
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
// Revision 1.3  2002/11/05 21:49:17  vrandezo
// Sprachen und Schriften im- und exportierbar
// bool const ueberschreiben aus Speicher-Schnittstelle entfernt
// Mengen haben IDs erhalten
// Voraussetzungen für Talente (allgemeine und meisterliche)
// MengeGUIwx beim Wechseln von Objekten mit Rückfrage
//
// Revision 1.2  2002/10/27 17:29:17  vrandezo
// Die meisten Teile sind speicherbar
//
// Revision 1.1  2002/10/25 14:21:11  vrandezo
// Hauptmenü erweitert
// Charakteristika hinzugefügt
// eigenes Mengen-GUI-Element erzeugt
//
//@nonl
//@-node:<< Version History >>
//@nl
//@-node:@file Charakteristikum.cpp
//@-leo
