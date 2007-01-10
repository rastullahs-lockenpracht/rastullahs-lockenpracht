//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file PersonHandlerGenerator.cpp
//@+at
// PersonHandlerGenerator.cpp: Implementierung der 
// Heldenerschaffung
// 
// $Revision: 1.56 $
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
#include "PersonHandlerGenerator.h"
#include "Regeln.h"
#include "Hole.h"

#include "NDSAUtil.h"
#include "Log.h"

#include <algorithm>
#include <math.h>
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
	//@+node:class PersonHandlerGenerator
	//@+others
	//@+node:ctor
	PersonHandlerGenerator::PersonHandlerGenerator() 
	  : person_(new DSA::Person()) {
	  for (int i=0; i!=AnzahlEigenschaften; ++i)
	    P()->eigenschaft_[i].Basiswert(regeln("Durchschnittswert einer Eigenschaft"));
	  // Setze SO-Startwert
	  P()->abgeleiteterWert_[(int)Sozialstatus].modifikatoren_.FuegeEin
	    (Modifikator(regeln("Durchschnittsstartwert SO"), ("Startwert")));
	  // Setze Basis-Talente
	  for (TalentDefinitionen::iterator t = talente()->begin(); t != talente()->end(); ++t) {
	    if (t->second.Art() != Basis) continue;
	    Talent talent(t->second.ID(), 0);
	    talent.Bezugsperson(P());
	    P()->talente_.FuegeEin(talent);
	  }
	  talentebasis_ = P()->talente_;
	  // Setze Basis-Kampftechniken
	  for (KampftechnikDefinitionen::iterator k = kampftechniken()->begin(); k != kampftechniken()->end(); ++k) {
	    if (k->second.Art() != Basis) continue;
	    Kampftechnik kampftechnik(k->second.ID(), 0);
	    kampftechnik.Bezugsperson(P());
	    P()->kampftechniken_.FuegeEin(kampftechnik);
	  }
	  kampftechnikenbasis_ = P()->kampftechniken_;
	  autoID = true;
	} // ctor
	//@nonl
	//@-node:ctor
	//@+node:AutoID
	std::string PersonHandlerGenerator::AutoID(const std::string& name) {
	  int i = 0;
	  std::string id;
	  while ((i<name.size())&&((isalpha(name[i]))|(name[i]=='Ä')|(name[i]=='Ö')|(name[i]=='Ü')|(name[i]=='ä')|(name[i]=='ö')|(name[i]=='ü'))) {
	    id += name[i];
	    ++i;
	  }
	  if (id=="") return id;
	  if (!personen()->Existiert(id)) return(id);
	
	  std::string idn;
	  i = 0;
	  do idn = id + strings::int2sStr(++i); while (personen()->Existiert(idn));
	  return idn;
	} // AutoID
	
	//@-node:AutoID
	//@+node:P()
	void PersonHandlerGenerator::P(std::auto_ptr<Person> person) {
	  person_ = person;
	  
	  // Was ist mit sfeigen_?
	  sfeigen_ = person_->sonderfertigkeiten_;
	
	  // Hier werden char nicht angefasst!
	  Rasse_(person_->Rasse_(), true);
	  Kultur_(person_->Kultur_(), true);
	  Profession_(person_->Profession_(), true);
	  
	  // Berechne chareigen_
	  chareigen_.clear();
	  charfinal_ = P()->charakteristika_;
	  BerechneCharFinal(false, true);
	
	  ID(AutoID(P()->Name()));
	  autoID = true;
	    
	//  for (Charakteristika::iterator ch = chareigen_.begin(); ch != chareigen_.end(); ++ch)
	//    if (ch->second.SchlechteEigenschaft())
	//      if (charbasis_.Existiert(ch->first))
	//        ch->second.Stufe(ch->second.Stufe()-charbasis_.Zeige(ch->first)->Stufe());
	
	  BerechneSFFinal(true);
	
	//  for (Sonderfertigkeiten::iterator sf = sfeigen_.begin(); sf != sfeigen_.end(); ++sf)
	//    if (sf->second.HatStufen())
	//      if (sfbasis_.Existiert(sf->first))
	//        sf->second.Stufe(sf->second.Stufe()-sfbasis_.Zeige(sf->first)->Stufe());
	}
	
	
	//@-node:P()
	//@+node:Name
	void PersonHandlerGenerator::Name(const std::string& name) {
	  P()->Name(name);
	  if (autoID) P()->ID(AutoID(name));
	} // Name
	//@nonl
	//@-node:Name
	//@+node:ID
	void PersonHandlerGenerator::ID(const std::string& id) {
	  P()->ID(id);
	  autoID = (id == AutoID(P()->Name()));
	} // ID
	//@nonl
	//@-node:ID
	//@+node:Eigenschaften
	//@+others
	//@+node:Steigerbar Eigenschaft
	bool PersonHandlerGenerator::Steigerbar(Eigenschaftsart e) {
	  if (voraussetzungen_.Existiert(EigenschaftString(e)))
	    if (voraussetzungen_.Zeige(EigenschaftString(e))->Maximal())
	      return (voraussetzungen_.Zeige(EigenschaftString(e))->MaximalWert() > P()->Wert(e));
	  return ((regeln("Maximaler Startwert einer Eigenschaft")>P()->Basiswert(e))
	  ||(generierteCharakteristika()->Zeige("Charakteristikum:Herausragende Eigenschaft " + EigenschaftString(e))->Hinzufuegbar(*P()))
	  ||(P()->charakteristika_.Existiert("Charakteristikum:Herausragende Eigenschaft " + EigenschaftString(e))));
	} // Steigerbar
	//@nonl
	//@-node:Steigerbar Eigenschaft
	//@+node:Senkbar Eigenschaft
	bool PersonHandlerGenerator::Senkbar(Eigenschaftsart e) {
	  if (voraussetzungen_.Existiert(EigenschaftString(e)))
	    if (voraussetzungen_.Zeige(EigenschaftString(e))->Minimal())
	      return (voraussetzungen_.Zeige(EigenschaftString(e))->MinimalWert() < P()->Wert(e));
	  return ((regeln("Minimaler Startwert einer Eigenschaft")<P()->Basiswert(e))
	  ||(generierteCharakteristika()->Zeige("Charakteristikum:Miserable Eigenschaft " + EigenschaftString(e))->Hinzufuegbar(*P())));
	} // Senkbar
	//@nonl
	//@-node:Senkbar Eigenschaft
	//@+node:Steiger Eigenschaft
	void PersonHandlerGenerator::Steiger(Eigenschaftsart e) {
	 std::string s = "Charakteristikum:Miserable Eigenschaft " + EigenschaftString(e);
	  if (P()->charakteristika_.Existiert(s))
	    CharakteristikumSenken(s);
	  else if (regeln("Maximaler Startwert einer Eigenschaft")>P()->Basiswert(e))
	    PersonHandler::Steiger(e);
	  else {
	   s = "Charakteristikum:Herausragende Eigenschaft " + EigenschaftString(e);
	    if (!P()->charakteristika_.Existiert(s))
	      FuegeHinzu(s);
	    else
	      CharakteristikumSteigern(s);
	  }
	//@+at
	//   if (e!=Klugheit) return;
	//   // Hardcodiertes verändern der Mutter- und 
	// Zweitsprache
	//   int i = 0;
	//   for (i = 0; i < 
	// P()->Kultur_().muttersprache_.size(); ++i) {
	//     std::string name = 
	// P()->Kultur_().muttersprache_[i];
	// sprachenbasis_.Zeige(name)->Wert(sprachenbasis_.Zeige(name)->Wert()+1);
	// P()->sprachen_.Zeige(name)->Wert(P()->sprachen_.Zeige(name)->Wert()+1);
	//   }
	//   for (i = 0; i < 
	// P()->Kultur_().zweitsprache_.size(); ++i) {
	//     std::string name = 
	// P()->Kultur_().zweitsprache_[i];
	// sprachenbasis_.Zeige(name)->Wert(sprachenbasis_.Zeige(name)->Wert()+1);
	// P()->sprachen_.Zeige(name)->Wert(P()->sprachen_.Zeige(name)->Wert()+1);
	//   }
	//@-at
	//@@c
	} // Steiger
	//@nonl
	//@-node:Steiger Eigenschaft
	//@+node:Senke Eigenschaft
	void PersonHandlerGenerator::Senke(Eigenschaftsart e) {
	 std::string s = "Charakteristikum:Herausragende Eigenschaft " + EigenschaftString(e);
	  if (P()->charakteristika_.Existiert(s))
	    CharakteristikumSenken(s);
	  else if (regeln("Minimaler Startwert einer Eigenschaft")<P()->Basiswert(e))
	    PersonHandler::Senke(e);
	  else
	   FuegeHinzu("Charakteristikum:Miserable Eigenschaft " + EigenschaftString(e));
	
	//@+at
	//   if (e!=Klugheit) return;
	//   // Hardcodiertes verändern der Mutter- und 
	// Zweitsprache
	//   int i = 0;
	//   for (i = 0; i < 
	// P()->Kultur_().muttersprache_.size(); ++i) {
	//     std::string name = 
	// P()->Kultur_().muttersprache_[i];
	// sprachenbasis_.Zeige(name)->Wert(sprachenbasis_.Zeige(name)->Wert()-1);
	// P()->sprachen_.Zeige(name)->Wert(P()->sprachen_.Zeige(name)->Wert()-1);
	//   }
	//   for (i = 0; i < 
	// P()->Kultur_().zweitsprache_.size(); ++i) {
	//     std::string name = 
	// P()->Kultur_().zweitsprache_[i];
	// sprachenbasis_.Zeige(name)->Wert(sprachenbasis_.Zeige(name)->Wert()-1);
	// P()->sprachen_.Zeige(name)->Wert(P()->sprachen_.Zeige(name)->Wert()-1);
	//   }
	//@-at
	//@@c
	} // Senke
	//@nonl
	//@-node:Senke Eigenschaft
	//@+node:BerechneEigenschaftenMaxima
	void PersonHandlerGenerator::BerechneEigenschaftenMaxima() {
	  for (int i = 0; i < AnzahlEigenschaften; ++i) {
	    int max = ((P()->eigenschaft_[i].Basiswert()*3)+1)/2;
	    if (max > regeln("Üblicher Maximalwert einer Eigenschaft"))
	      max = regeln("Üblicher Maximalwert einer Eigenschaft");
	    // Fettleibig
	   if (P()->charakteristika_.Existiert("Charakteristikum:Fettleibig")) {
	      if ((static_cast<Eigenschaftsart>(i) == Gewandtheit)
	        ||(static_cast<Eigenschaftsart>(i) == Fingerfertigkeit))
	        if (max > 16) max = 16; // TODO Magische Konstante
	    }
	
	    max = P()->eigenschaft_[i].modifikatoren_.Verrechne(max);
	    P()->eigenschaft_[i].Maximalwert(max);
	  }
	} // BerechneEigenschaftenMaxima
	//@nonl
	//@-node:BerechneEigenschaftenMaxima
	//@-others
	//@nonl
	//@-node:Eigenschaften
	//@+node:Abgeleitete Werte
	//@+others
	//@+node:SOStartWert
	Modifikator& PersonHandlerGenerator::SOStartwert() {
	  return P()->abgeleiteterWert_[(int)Sozialstatus].modifikatoren_.find("Startwert")->second;
	} // SOStartwert
	//@nonl
	//@-node:SOStartWert
	//@+node:Steigerbar Abgeleitet
	bool PersonHandlerGenerator::Steigerbar(AbgeleiteterWertArt a) {
	  for (int i = 0; i != 4; ++i) {
	    AbgeleiteterWertArt a0; std::string contra; std::string pro;
	   if (i==0) { a0 = Lebensenergie; contra = "Charakteristikum:Niedrige Lebenskraft"; pro = "Charakteristikum:Hohe Lebenskraft"; }
	    if (i==1) { a0 = Ausdauer; contra = "Charakteristikum:Kurzatmig"; pro = "Charakteristikum:Ausdauernd"; }
	    if (i==2) { a0 = Magieresistenz; contra = "Charakteristikum:Niedrige Magieresistenz"; pro = "Charakteristikum:Hohe Magieresistenz"; }
	    if (i==3) { a0 = Astralenergie; contra = "Charakteristikum:Niedrige Astralkraft"; pro = "Charakteristikum:Astralmacht"; }
	    if (a==a0)
	      if (charakteristika()->Zeige(pro)!=0)
	        if (charakteristika()->Zeige(pro)->Hinzufuegbar(*P()))
	          return true;
	        else
	          return (CharakteristikumSteigerbar(pro) || CharakteristikumSenkbar(contra));
	  }
	  if (a==Sozialstatus) {
	    if (voraussetzungen_.Existiert(AbgeleiteterWertString(a)))
	      if (voraussetzungen_.Zeige(AbgeleiteterWertString(a))->Maximal())
	        return (voraussetzungen_.Zeige(AbgeleiteterWertString(a))->MaximalWert() > SOStartwert().Wert());
	    return (regeln("Maximaler Startwert SO")>
	            SOStartwert().Wert());
	  }
	  return false;
	} // Steigerbar
	//@nonl
	//@-node:Steigerbar Abgeleitet
	//@+node:Senkbar Abgeleitet
	bool PersonHandlerGenerator::Senkbar(AbgeleiteterWertArt a) {
	  for (int i = 0; i != 4; ++i) {
	    AbgeleiteterWertArt a0; std::string contra; std::string pro;
	    if (i==0) { a0 = Lebensenergie; contra = "Charakteristikum:Niedrige Lebenskraft"; pro = "Charakteristikum:Hohe Lebenskraft"; }
	    if (i==1) { a0 = Ausdauer; contra = "Charakteristikum:Kurzatmig"; pro = "Charakteristikum:Ausdauernd"; }
	    if (i==2) { a0 = Magieresistenz; contra = "Charakteristikum:Niedrige Magieresistenz"; pro = "Charakteristikum:Hohe Magieresistenz"; }
	    if (i==3) { a0 = Astralenergie; contra = "Charakteristikum:Niedrige Astralkraft"; pro = "Charakteristikum:Astralmacht"; }
	    if (a==Magieresistenz) if (P()->Wert(a)<1) return false; // MR darf nicht durch Niedrige MR unter 0 fallen (AH 115)
	    if (a==a0) {
	      if (charakteristika()->Zeige(contra)!=0)
	        if (charakteristika()->Zeige(contra)->Hinzufuegbar(*P()))
	          return true;
	      return (CharakteristikumSteigerbar(contra) || CharakteristikumSenkbar(pro));
	    }
	  }
	  if (a==Sozialstatus) {
	    if (voraussetzungen_.Existiert(AbgeleiteterWertString(a)))
	      if (voraussetzungen_.Zeige(AbgeleiteterWertString(a))->Minimal())
	        return (voraussetzungen_.Zeige(AbgeleiteterWertString(a))->MinimalWert() < SOStartwert().Wert());
	    return (regeln("Minimaler Startwert SO")<
	            SOStartwert().Wert());
	  }
	  return false;
	} // Senkbar
	//@nonl
	//@-node:Senkbar Abgeleitet
	//@+node:Steiger Abgeleitet
	void PersonHandlerGenerator::Steiger(AbgeleiteterWertArt a) {
	  for (int i = 0; i != 4; ++i) {
	    AbgeleiteterWertArt a0; std::string contra; std::string pro;
	    if (i==0) { a0 = Lebensenergie; contra = "Charakteristikum:Niedrige Lebenskraft"; pro = "Charakteristikum:Hohe Lebenskraft"; }
	    if (i==1) { a0 = Ausdauer; contra = "Charakteristikum:Kurzatmig"; pro = "Charakteristikum:Ausdauernd"; }
	    if (i==2) { a0 = Magieresistenz; contra = "Charakteristikum:Niedrige Magieresistenz"; pro = "Charakteristikum:Hohe Magieresistenz"; }
	    if (i==3) { a0 = Astralenergie; contra = "Charakteristikum:Niedrige Astralkraft"; pro = "Charakteristikum:Astralmacht"; }
	    if (a==a0) {
	      if (CharakteristikumSteigerbar(pro))
	        CharakteristikumSteigern(pro);
	      else if (CharakteristikumSenkbar(contra))
	        CharakteristikumSenken(contra);
	      else
	        FuegeHinzu(pro);
	      return;
	    }
	  }
	  if (a==Sozialstatus) {
	    SOStartwert().Wert(SOStartwert().Wert() + 1);
	    return;
	  }
	
	  Log::Error("Steigern von " + AbgeleiteterWertString(a) + " nicht implementiert");
	} // Steiger
	//@nonl
	//@-node:Steiger Abgeleitet
	//@+node:Senke Abgeleitet
	void PersonHandlerGenerator::Senke(AbgeleiteterWertArt a) {
	  for (int i = 0; i != 4; ++i) {
	    AbgeleiteterWertArt a0; std::string contra; std::string pro;
	    if (i==0) { a0 = Lebensenergie; contra = "Charakteristikum:Niedrige Lebenskraft"; pro = "Charakteristikum:Hohe Lebenskraft"; }
	    if (i==1) { a0 = Ausdauer; contra = "Charakteristikum:Kurzatmig"; pro = "Charakteristikum:Ausdauernd"; }
	    if (i==2) { a0 = Magieresistenz; contra = "Charakteristikum:Niedrige Magieresistenz"; pro = "Charakteristikum:Hohe Magieresistenz"; }
	    if (i==3) { a0 = Astralenergie; contra = "Charakteristikum:Niedrige Astralkraft"; pro = "Charakteristikum:Astralmacht"; }
	    if (a==a0) {
	      if (CharakteristikumSteigerbar(contra))
	        CharakteristikumSteigern(contra);
	      else if (CharakteristikumSenkbar(pro))
	        CharakteristikumSenken(pro);
	      else
	        FuegeHinzu(contra);
	      return;
	    }
	  }
	  if (a==Sozialstatus) {
	    SOStartwert().Wert(SOStartwert().Wert() - 1);
	    return;
	  }
	
	  Log::Error("Senken von " + AbgeleiteterWertString(a) + " nicht implementiert");
	} // Senke
	//@nonl
	//@-node:Senke Abgeleitet
	//@-others
	//@nonl
	//@-node:Abgeleitete Werte
	//@+node:Rasse_
	void PersonHandlerGenerator::Rasse_(Rasse rasse, bool reGen) {
	  if (!reGen) {
	    Entferne(P()->Rasse_().voraussetzungen_, ("Rasse"));
	     Entferne(P()->Rasse_().modifikatoren_, ("Rasse"));
	    Entferne(P()->Rasse_().zauber_);
	    Entferne(P()->Rasse_().kampftechniken_);
	     Entferne(P()->Rasse_().talente_);
	    Entferne(P()->Rasse_().sprachen_);
	    Entferne(P()->Rasse_().schriften_);
	  }
	  P()->Rasse_(rasse);
	  Setze(P()->Rasse_().schriften_, reGen);
	  Setze(P()->Rasse_().sprachen_, reGen);
	  Setze(P()->Rasse_().talente_, reGen);
	  Setze(P()->Rasse_().kampftechniken_, reGen);
	  Setze(P()->Rasse_().zauber_, reGen);
	  Setze(P()->Rasse_().modifikatoren_, ("Rasse"));
	  Setze(P()->Rasse_().voraussetzungen_, ("Rasse"));
	  if (rasse.augenfarbe_.Farbe(Augenfarbe()) < 1) Augenfarbe(rasse.augenfarbe_.Farbe());
	  if (rasse.hauptfarbe_.Farbe(Hauptfarbe()) < 1) Hauptfarbe(rasse.hauptfarbe_.Farbe());
	  if ((Groesse() < (rasse.groesseBasis_+rasse.groesseW6_+rasse.groesseW20_)) ||
	      (Groesse() > (rasse.groesseBasis_+rasse.groesseW6_*6+rasse.groesseW20_*20)))
	        Groesse(rasse.Groesse());
	  PersonHandler::Gewicht(Groesse()-rasse.gewichtDifferenz_);
	  if ((Alter() < (rasse.alterBasis_+rasse.alterW6_+rasse.alterW3_)) ||
	      (Alter() > (rasse.alterBasis_+rasse.alterW6_*6+rasse.alterW3_*3)))
	      PersonHandler::Alter(rasse.Alter());
	  if (!reGen) BerechneCharFinal();
	//  BerechneSFFinal();
	} // Rasse_
	//@nonl
	//@-node:Rasse_
	//@+node:Kultur_
	void PersonHandlerGenerator::Kultur_(Kultur kultur, bool reGen) {
	  if (!reGen) {
	    Entferne(P()->Kultur_().voraussetzungen_, ("Kultur"));
	    Entferne(P()->Kultur_().modifikatoren_, ("Kultur"));
	    Entferne(P()->Kultur_().zauber_);
	    Entferne(P()->Kultur_().kampftechniken_);
	    Entferne(P()->Kultur_().talente_);
	    Entferne(P()->Kultur_().sprachen_);
	    Entferne(P()->Kultur_().schriften_);
	    
	    EntferneMuttersprache(P()->Kultur_().muttersprache_);
	    EntferneMuttersprache(P()->Kultur_().zweitsprache_);
	    SetzeMuttersprache(P()->Profession_().zweitsprache_, false);
	  }
	  
	  P()->Kultur_(kultur);
	
	  SetzeMuttersprache(P()->Kultur_().zweitsprache_, false);
	  SetzeMuttersprache(P()->Kultur_().muttersprache_, true);
	
	  Setze(P()->Kultur_().schriften_, reGen);
	  Setze(P()->Kultur_().sprachen_, reGen);
	  Setze(P()->Kultur_().talente_, reGen);
	  Setze(P()->Kultur_().kampftechniken_, reGen);
	  Setze(P()->Kultur_().zauber_, reGen);
	  Setze(P()->Kultur_().modifikatoren_, ("Kultur"));
	  Setze(P()->Kultur_().voraussetzungen_, ("Kultur"));
	
	  if (!reGen) BerechneCharFinal();
	//  BerechneSFFinal();  
	} // Kultur_
	//@nonl
	//@-node:Kultur_
	//@+node:Profession_
	void PersonHandlerGenerator::Profession_(Profession profession, bool reGen) {
	  if (!reGen) {
	    Entferne(P()->Profession_().voraussetzungen_, ("Profession"));
	    Entferne(P()->Profession_().modifikatoren_, ("Profession"));
	    Entferne(P()->Profession_().zauber_);
	    Entferne(P()->Profession_().kampftechniken_);
	    Entferne(P()->Profession_().talente_);
	    Entferne(P()->Profession_().sprachen_);
	    Entferne(P()->Profession_().schriften_);
	
	    EntferneMuttersprache(P()->Profession_().zweitsprache_);
	    SetzeMuttersprache(P()->Kultur_().zweitsprache_, false);
	    SetzeMuttersprache(P()->Kultur_().muttersprache_, true);
	  }
	  
	  P()->Profession_(profession);
	
	  SetzeMuttersprache(P()->Profession_().zweitsprache_, false);
	
	  Setze(P()->Profession_().schriften_, reGen);
	  Setze(P()->Profession_().sprachen_, reGen);
	  Setze(P()->Profession_().talente_, reGen);
	  Setze(P()->Profession_().kampftechniken_, reGen);
	  Setze(P()->Profession_().zauber_, reGen);
	  Setze(P()->Profession_().modifikatoren_, ("Profession"));
	  Setze(P()->Profession_().voraussetzungen_, ("Profession"));
	  if (!reGen) BerechneCharFinal();
	//  BerechneSFFinal();
	} // Profession_
	//@-node:Profession_
	//@+node:Entferne Muttersprache
	void PersonHandlerGenerator::EntferneMuttersprache(const StringListe& sprachen) {
	  for (int j = 0; j < sprachen.size(); ++j) {
	    std::string sprachenname = sprachen[j];
	    for (int i = 0; i < 2; ++i) {
	      Sprachen& spracheref = (i==1) ? sprachenbasis_ : P()->sprachen_;
	      spracheref.Zeige(sprachenname)->Muttersprache(false);
	      spracheref.Zeige(sprachenname)->Zweitsprache(false);
	      if (spracheref.Zeige(sprachenname)->Wert()==0)
	        spracheref.Loesche(sprachenname);
	    }
	  }
	}
	//@nonl
	//@-node:Entferne Muttersprache
	//@+node:SetzeMuttersprache
	void PersonHandlerGenerator::SetzeMuttersprache(const StringListe& sprachen, bool muttersprache) {
	  for (int j = 0; j < sprachen.size(); ++j) {
	    std::string sprachenname = sprachen[j];
	    for (int i = 0; i < 2; ++i) {
	        Sprachen& spracheref = (i==1) ? sprachenbasis_ : P()->sprachen_;
	      if (spracheref.Existiert(sprachenname)) {
	        spracheref.Zeige(sprachenname)->Muttersprache(muttersprache);
	        spracheref.Zeige(sprachenname)->Zweitsprache(!muttersprache);
	      } else {
	        SpracheDefinition def = DSA::sprachen()->Hole(sprachenname);
	        Sprache s(def);
	        s.ID(sprachenname);
	        s.Muttersprache(muttersprache);
	        s.Zweitsprache(!muttersprache);
	        s.Bezugsperson(P());
	        spracheref.FuegeEin(s);
	      }
	    }
	  }
	}
	//@nonl
	//@-node:SetzeMuttersprache
	//@+node:Entferne Mod
	void PersonHandlerGenerator::Entferne(Modifikatoren mods, const std::string& grund) {
	  for (DSA::Modifikatoren::iterator m = mods.begin(); m != mods.end(); ++m) {
	    std::string typ = m->second.ID();
	    Eigenschaftsart e = EigenschaftString(typ);
	    if (e != unbekannteEigenschaft) {
	      // Setzen für Zweit- und Muttersprache
	      if (e == Klugheit) {
	        int i = 0;
	        for (i = 0; i < P()->Kultur_().muttersprache_.size(); ++i) {
	          StringListe s = P()->Kultur_().muttersprache_;
	          P()->sprachen_.Zeige(s[i])->Wert(P()->sprachen_.Zeige(s[i])->Wert()-m->second.Wert());
	          sprachenbasis_.Zeige(s[i])->Wert(sprachenbasis_.Zeige(s[i])->Wert()-m->second.Wert());
	        }
	        for (i = 0; i < P()->Kultur_().zweitsprache_.size(); ++i) {
	          StringListe s = P()->Kultur_().zweitsprache_;
	          P()->sprachen_.Zeige(s[i])->Wert(P()->sprachen_.Zeige(s[i])->Wert()-m->second.Wert());
	          sprachenbasis_.Zeige(s[i])->Wert(sprachenbasis_.Zeige(s[i])->Wert()-m->second.Wert());
	        }
	      }
	      if (!P()->eigenschaft_[(int)e].modifikatoren_.Loesche(grund))
	        Log::Error("PersonHandlerGenerator::Entferne: Nicht existenter zu löschender Modifikator " + typ);
	      continue;
	    }
	    AbgeleiteterWertArt a	= AbgeleiteterWertString(typ);
	    if (a != DSA::unbekannterAbgeleiteterWert) {
	      if (!P()->abgeleiteterWert_[(int)a].modifikatoren_.Loesche(grund))
	        Log::Error("PersonHandlerGenerator::Entferne: Nicht existenter zu löschender Modifikator " + typ);
	      continue;
	    }
	    Log::Error("PersonHandlerGenerator::Entferne: Unbekannter Modifikator auf " + typ);
	  }
	} // Entferne (modifikatoren)
	//@nonl
	//@-node:Entferne Mod
	//@+node:Setze Mod
	void PersonHandlerGenerator::Setze(Modifikatoren mods, const std::string& grund) {
	  for (DSA::Modifikatoren::iterator m = mods.begin(); m != mods.end(); ++m) {
	    Modifikator mod = m->second;
	    std::string typ = mod.ID();
	    mod.ID(grund);
	    Eigenschaftsart e = EigenschaftString(typ);
	    if (e != unbekannteEigenschaft) {
	      // Setzen für Zweit- und Muttersprache
	      if (e == Klugheit) {
	        int i = 0;
	        for (i = 0; i < P()->Kultur_().muttersprache_.size(); ++i) {
	          StringListe s = P()->Kultur_().muttersprache_;
	          P()->sprachen_.Zeige(s[i])->Wert(P()->sprachen_.Zeige(s[i])->Wert()+m->second.Wert());
	          sprachenbasis_.Zeige(s[i])->Wert(sprachenbasis_.Zeige(s[i])->Wert()+m->second.Wert());
	        }
	        for (i = 0; i < P()->Kultur_().zweitsprache_.size(); ++i) {
	          StringListe s = P()->Kultur_().zweitsprache_;
	          P()->sprachen_.Zeige(s[i])->Wert(P()->sprachen_.Zeige(s[i])->Wert()+m->second.Wert());
	          sprachenbasis_.Zeige(s[i])->Wert(sprachenbasis_.Zeige(s[i])->Wert()+m->second.Wert());
	        }
	      }
	      if (!P()->eigenschaft_[(int)e].modifikatoren_.FuegeEin(mod))
	        Log::Error("PersonHandlerGenerator::Setze: Bereits existenter Modifikator " + typ);
	      continue;
	    }
	    AbgeleiteterWertArt a = AbgeleiteterWertString(typ);
	    if (a != DSA::unbekannterAbgeleiteterWert) {
	      if (!P()->abgeleiteterWert_[(int)a].modifikatoren_.FuegeEin(mod))
	        Log::Error("PersonHandlerGenerator::Setze: Bereits existenter Modifikator " + typ);
	      continue;
	    }
	    Log::Error("PersonHandlerGenerator::Setze: Unbekannter Modifikator auf " + typ);
	  }
	} // Setze (modifikatoren)
	//@nonl
	//@-node:Setze Mod
	//@+node:Entferne Voraussetzung
	void PersonHandlerGenerator::Entferne(Voraussetzungen voraussetzungen, const std::string& grund) {
	  for (Voraussetzungen::iterator v = voraussetzungen.begin(); v != voraussetzungen.end(); ++v) {
	    std::multimap<std::string, Voraussetzung>::iterator vi = voraussetzungenliste_.lower_bound(v->first);
	    std::multimap<std::string, Voraussetzung>::iterator viLast = voraussetzungenliste_.upper_bound(v->first);
	    for (; vi != viLast; ++vi)
	      if (vi->second==v->second) {
	        voraussetzungenliste_.erase(vi); goto nachSchleife;
	      }
	  }
	  nachSchleife:
	
	  ErzeugeVoraussetzungen();
	} // Entferne (Voraussetzungen)
	//@nonl
	//@-node:Entferne Voraussetzung
	//@+node:Setze Voraussetzung
	void PersonHandlerGenerator::Setze(Voraussetzungen voraussetzungen, const std::string& grund) {
	  for (Voraussetzungen::iterator v = voraussetzungen.begin(); v != voraussetzungen.end(); ++v)
	    voraussetzungenliste_.insert(std::pair<std::string, Voraussetzung>(v->first, v->second));
	
	  ErzeugeVoraussetzungen();
	
	  // Passe an die neuen Voraussetzungen an
	  for (int i=0; i!=AnzahlEigenschaften; ++i)
	    if (voraussetzungen_.Existiert(EigenschaftString((Eigenschaftsart)i)))
	      while (voraussetzungen_.Zeige(EigenschaftString((Eigenschaftsart)i))->MinimalWert()
	           > P()->Wert((Eigenschaftsart)i))
	        PersonHandler::Steiger((Eigenschaftsart)i);
	  if (voraussetzungen_.Existiert(("Sozialstatus"))) {
	    if (voraussetzungen_.Zeige(("Sozialstatus"))->Maximal())
	        while (voraussetzungen_.Zeige(("Sozialstatus"))->MaximalWert()
	             < P()->Wert(Sozialstatus))
	          Senke(Sozialstatus);
	    if (voraussetzungen_.Zeige(_t("Sozialstatus"))->Minimal())
	      while (voraussetzungen_.Hole(_t("Sozialstatus")).MinimalWert()
	           > P()->Wert(Sozialstatus))
	        Steiger(Sozialstatus);
	    }
	} // Setze (Voraussetzungen)
	//@nonl
	//@-node:Setze Voraussetzung
	//@+node:Erzeuge Voraussetzungen
	void PersonHandlerGenerator::ErzeugeVoraussetzungen() {
	  voraussetzungen_.clear();
	  for (std::multimap<std::string, Voraussetzung>::iterator vi = voraussetzungenliste_.begin(); vi != voraussetzungenliste_.end(); ++vi)
	    if (!voraussetzungen_.FuegeEin(vi->second)) {
	      if (vi->second.Minimal())
	        if (vi->second.MinimalWert() > voraussetzungen_.find(vi->first)->second.MinimalWert())
	          voraussetzungen_.find(vi->first)->second.MinimalWert(vi->second.MinimalWert());
	      if (vi->second.Maximal())
	        if (vi->second.MaximalWert() < voraussetzungen_.find(vi->first)->second.MaximalWert())
	          voraussetzungen_.find(vi->first)->second.MaximalWert(vi->second.MaximalWert());
	    }
	} // ErzeugeVoraussetzungen
	//@nonl
	//@-node:Erzeuge Voraussetzungen
	//@+node:Talentoide
	//@+others
	//@+node:Entferne Kampftechniken
	void PersonHandlerGenerator::Entferne(Kampftechniken kampftechniken) {
	  for (int i = 0; i < 2; ++i) {
	    Kampftechniken& kampfref = (i==1) ? kampftechnikenbasis_ : P()->kampftechniken_;
	    for (Kampftechniken::iterator kampftechnik = kampftechniken.begin(); kampftechnik != kampftechniken.end(); ++kampftechnik) {
	      if (kampfref.Hole(kampftechnik->first).Wert()==kampftechnik->second.Wert())
	        if (kampftechnik->second.Art()!=Basis) {
	          kampfref.Loesche(kampftechnik->first);
	          continue;
	        }
	    if (kampfref.Zeige(kampftechnik->first)!=0)
	        kampfref.Zeige(kampftechnik->first)->Wert(kampfref.Zeige(kampftechnik->first)->Wert()-kampftechnik->second.Wert());
	    }
	  }
	} // Entferne (kampftechniken)
	//@nonl
	//@-node:Entferne Kampftechniken
	//@+node:Setze Kampftechniken
	void PersonHandlerGenerator::Setze(Kampftechniken kampftechniken, bool reGen) {
	  for (int i = 0; i < 2; ++i) {
	    if ((i==0)&&(reGen)) continue;
	    Kampftechniken& kampfref = (i==1) ? kampftechnikenbasis_ : P()->kampftechniken_;
	    for (Kampftechniken::iterator kampftechnik = kampftechniken.begin(); kampftechnik != kampftechniken.end(); ++kampftechnik) {
	      if (kampfref.Existiert(kampftechnik->first))
	        kampfref.Zeige(kampftechnik->first)->Wert(kampfref.Zeige(kampftechnik->first)->Wert()+kampftechnik->second.Wert());
	      else {
	        Kampftechnik kt = kampftechnik->second;
	        kt.Bezugsperson(P());
	        kampfref.FuegeEin(kt);
	      }
	    }
	  }
	} // Setze (kampftechniken)
	//@nonl
	//@-node:Setze Kampftechniken
	//@+node:Entferne Talente
	void PersonHandlerGenerator::Entferne(Talente talente) {
	  for (int i = 0; i < 2; ++i) {
	    Talente& talref = (i==1) ? talentebasis_ : P()->talente_;
	    for (Talente::iterator talent = talente.begin(); talent != talente.end(); ++talent) {
	      if (talref.Hole(talent->first).Wert()==talent->second.Wert())
	        if (talent->second.Art()!=Basis) {
	          talref.Loesche(talent->first);
	          continue;
	        }
	    
	   if (talref.Zeige(talent->first)!=0)
	        talref.Zeige(talent->first)->Wert(talref.Zeige(talent->first)->Wert()-talent->second.Wert());
	    }
	  }
	} // Entferne (talente)
	
	//@-node:Entferne Talente
	//@+node:Setze Talente
	void PersonHandlerGenerator::Setze(Talente talente, bool reGen) {
	  for (int i = 0; i < 2; ++i) {
	    if ((i==0)&&(reGen)) continue;
	    Talente& talref = (i==1) ? talentebasis_ : P()->talente_;
	    for (Talente::iterator talent = talente.begin(); talent != talente.end(); ++talent)
	      if (talref.Existiert(talent->first))
	        talref.Zeige(talent->first)->Wert(talref.Zeige(talent->first)->Wert()+talent->second.Wert());
	      else {
	        talent->second.Bezugsperson(P());
	        talref.FuegeEin(talent->second);
	      }
	  }
	} // Setze (talente)
	//@nonl
	//@-node:Setze Talente
	//@+node:Entferne Sprachen
	void PersonHandlerGenerator::Entferne(Sprachen sprachen) {
	  for (int i = 0; i < 2; ++i) {
	    Sprachen& spracheref = (i==1) ? sprachenbasis_ : P()->sprachen_;
	    for (Sprachen::const_iterator sprache = sprachen.begin(); sprache != sprachen.end(); ++sprache) {
	      if (spracheref.Hole(sprache->first).Wert()==sprache->second.Wert()) {
	        spracheref.Loesche(sprache->first);
	        continue;
	      }
	    
	    if (spracheref.Zeige(sprache->first)!=0)
	        spracheref.Zeige(sprache->first)->Wert(spracheref.Zeige(sprache->first)->Wert()-sprache->second.Wert());
	    }
	  }
	} // Entferne (sprachen)
	//@nonl
	//@-node:Entferne Sprachen
	//@+node:Setze Sprachen
	void PersonHandlerGenerator::Setze(Sprachen sprachen, bool reGen) {
	  for (int i = 0; i < 2; ++i) {
	    if ((i==0)&&(reGen)) continue;
	    Sprachen& spracheref = (i==1) ? sprachenbasis_ : P()->sprachen_;
	    for (Sprachen::iterator sprache = sprachen.begin(); sprache != sprachen.end(); ++sprache)
	      if (spracheref.Existiert(sprache->first))
	        spracheref.Zeige(sprache->first)->Wert(spracheref.Zeige(sprache->first)->Wert()+sprache->second.Wert());
	      else {
	        sprache->second.Bezugsperson(P());
	        spracheref.FuegeEin(sprache->second);
	      }
	  }
	} // Setze (sprachen)
	//@nonl
	//@-node:Setze Sprachen
	//@+node:Entferne Schriften
	void PersonHandlerGenerator::Entferne(Schriften schriften) {
	  for (int i = 0; i < 2; ++i) {
	    Schriften& schriftref = (i==1) ? schriftenbasis_ : P()->schriften_;
	    for (Schriften::iterator schrift = schriften.begin(); schrift != schriften.end(); ++schrift) {
	      if (schriftref.Hole(schrift->first).Wert()==schrift->second.Wert()) {
	        schriftref.Loesche(schrift->first);
	        continue;
	      }
	    
	    if (schriftref.Zeige(schrift->first)!=0)
	      schriftref.Zeige(schrift->first)->Wert(schriftref.Zeige(schrift->first)->Wert()-schrift->second.Wert());
	    }
	  }
	} // Entferne (schriften)
	//@nonl
	//@-node:Entferne Schriften
	//@+node:Setze Schriften
	void PersonHandlerGenerator::Setze(Schriften schriften, bool reGen) {
	  for (int i = 0; i < 2; ++i) {
	    if ((i==0)&&(reGen)) continue;
	    Schriften& schriftref = (i==1) ? schriftenbasis_ : P()->schriften_;
	    for (Schriften::iterator schrift = schriften.begin(); schrift != schriften.end(); ++schrift)
	      if (schriftref.Existiert(schrift->first))
	        schriftref.Zeige(schrift->first)->Wert(schriftref.Zeige(schrift->first)->Wert()+schrift->second.Wert());
	      else {
	        schrift->second.Bezugsperson(P());
	        schriftref.FuegeEin(schrift->second);
	      }
	  }
	} // Setze (schriften)
	//@-node:Setze Schriften
	//@+node:SpracheOderSchrift
	std::pair<bool, bool> PersonHandlerGenerator::SpracheOderSchrift(std::string& spracheOderSchrift) {
	  std::pair<bool, bool> rv;
	  rv.first = true;
	  if (spracheOderSchrift.find(":")>=spracheOderSchrift.size()) { rv.first = false; return rv; }
	  rv.second = !(spracheOderSchrift.substr(0, spracheOderSchrift.find(":")) == _t("Lesen / Schreiben"));
	  spracheOderSchrift = spracheOderSchrift.substr(spracheOderSchrift.find(":")+2, spracheOderSchrift.size()-(spracheOderSchrift.find(":")+2));
	  return rv;
	} // SpracheOderSchrift
	//@nonl
	//@-node:SpracheOderSchrift
	//@+node:Entferne Zauber
	void PersonHandlerGenerator::Entferne(Zauberfertigkeiten zauber) {
	  for (int i = 0; i < 2; ++i) {
	    Zauberfertigkeiten& zref = (i==1) ? zauberbasis_ : P()->zauberfertigkeiten_;
	    for (Zauberfertigkeiten::iterator z = zauber.begin(); z != zauber.end(); ++z) {
	      if (zref.Zeige(z->first) == 0)
	        continue;
	        
	      if (zref.Zeige(z->first)->Wert()==z->second.Wert()) {
	        zref.Loesche(z->first);
	        continue;
	      }
	      zref.Zeige(z->first)->Wert(zref.Zeige(z->first)->Wert()-z->second.Wert());
	      if (z->second.Hauszauber())
	        zref.Zeige(z->first)->Hauszauber(false);
	    }
	  }
	} // Entferne (zauber)
	//@nonl
	//@-node:Entferne Zauber
	//@+node:Setze Zauber
	void PersonHandlerGenerator::Setze(Zauberfertigkeiten zauber, bool reGen) {
	
	  for (int i = 0; i < 2; ++i) {
	    
	    if ((i==0)&&(reGen)) continue;
	    
	    Zauberfertigkeiten& zref = (i==1) ? zauberbasis_ : P()->zauberfertigkeiten_;
	        
	    for (Zauberfertigkeiten::iterator z = zauber.begin(); z != zauber.end(); ++z)
	        
	      if (zref.Existiert(z->first)) {
	            
	        zref.Zeige(z->first)->Wert(zref.Zeige(z->first)->Wert()+z->second.Wert());
	        if (z->second.Hauszauber())
	          zref.Zeige(z->first)->Hauszauber(true);
	          
	      }	else {
	      
	        z->second.Bezugsperson(P());
	        zref.FuegeEin(z->second);
	                
	      }
	      
	  }
	} // Setze (zauber)
	//@-node:Setze Zauber
	//@+node:TalentAktivierbar
	bool PersonHandlerGenerator::TalentAktivierbar(const Talentgruppe gruppe) {
	  if (!(AktivierteTalente() < AktivierteTalenteMax())) return false;
	  if ((gruppe == Kampf)||(gruppe==sonstige))
	    for (KampftechnikDefinitionen::iterator k = kampftechniken()->begin(); k != kampftechniken()->end(); ++k)
	      if (!P()->kampftechniken_.Existiert(k->first))
	        if (AktivierungsKosten(k->first)<=(TalentGPMax()-TalentGP()))
	          if (!P()->charakteristika_.Existiert(_t("Unfähigkeit für") + " " + TalentgruppeString(Kampf, false)))
	            return true;
	  if ((gruppe != Kampf)&&(gruppe!=SprachenUndSchriften)&&(gruppe!=Gabe))
	    for (TalentDefinitionen::iterator t = talente()->begin(); t != talente()->end(); ++t)
	      if (!P()->talente_.Existiert(t->first))
	        if (AktivierungsKosten(t->first)<=(TalentGPMax()-TalentGP()))
	          if ((t->second.IstKriterium(gruppe))||(gruppe==sonstige))
	            if (!t->second.IstKriterium(SprachenUndSchriften))
	              if (!P()->charakteristika_.Existiert(_t("Unfähigkeit für") + " " + TalentgruppeString(t->second.Gruppe(), false)))
	                return true;
	  if ((gruppe == SprachenUndSchriften)||(gruppe==sonstige)) {
	    for (SpracheDefinitionen::iterator sp = sprachen()->begin(); sp != sprachen()->end(); ++sp)
	      if (!P()->sprachen_.Existiert(sp->first))
	        if (AktivierungsKosten(sp->second.ID(true))<=(TalentGPMax()-TalentGP()))
	          if (!P()->charakteristika_.Existiert(_t("Unfähigkeit für") + " " + _t("Sprachen")))
	            return true;
	    for (SchriftDefinitionen::iterator sch = schriften()->begin(); sch != schriften()->end(); ++sch)
	      if (!P()->schriften_.Existiert(sch->first))
	        if (AktivierungsKosten(sch->second.ID())<=(TalentGPMax()-TalentGP()))
	          return true;
	  }
	  return false;
	} // TalentAktivierbar
	//@nonl
	//@-node:TalentAktivierbar
	//@+node:AktivierbareTalente
	void PersonHandlerGenerator::AktivierbareTalente(StringListe& liste, const Talentgruppe gruppe) {
	  liste.clear();
	    
	  if ((gruppe == Kampf)||(gruppe==sonstige))
	    for (KampftechnikDefinitionen::iterator k = kampftechniken()->begin(); k != kampftechniken()->end(); ++k)
	      if (!P()->kampftechniken_.Existiert(k->first))
	        if (AktivierungsKosten(k->first)<=(TalentGPMax()-TalentGP()))
	          if (!P()->charakteristika_.Existiert(_t("Unfähigkeit für") + " " + TalentgruppeString(Kampf, false)))
	            liste.push_back(k->first);
	            
	  if ((gruppe != Kampf)&&(gruppe!=SprachenUndSchriften)&&(gruppe!=Gabe))
	    for (TalentDefinitionen::iterator t = talente()->begin(); t != talente()->end(); ++t)
	      if (!P()->talente_.Existiert(t->first))
	        if (AktivierungsKosten(t->first)<=(TalentGPMax()-TalentGP()))
	          if ((t->second.IstKriterium(gruppe))||(gruppe==sonstige))
	            if (!t->second.IstKriterium(SprachenUndSchriften))
	              if (!P()->charakteristika_.Existiert(_t("Unfähigkeit für") + " " + TalentgruppeString(t->second.Gruppe(), false)))
	                liste.push_back(t->first);
	                
	  if ((gruppe == SprachenUndSchriften)||(gruppe==sonstige)) {
	  
	    for (SpracheDefinitionen::iterator sp = sprachen()->begin(); sp != sprachen()->end(); ++sp)
	      if (!P()->sprachen_.Existiert(sp->first))
	        if (AktivierungsKosten(sp->second.ID(true))<=(TalentGPMax()-TalentGP()))
	          if (!P()->charakteristika_.Existiert(_t("Unfähigkeit für") + " " + _t("Sprachen")))
	            liste.push_back(sp->second.ID(true));
	            
	    for (SchriftDefinitionen::iterator sch = schriften()->begin(); sch != schriften()->end(); ++sch)
	      if (!P()->schriften_.Existiert(sch->first))
	        if (AktivierungsKosten(sch->second.ID())<=(TalentGPMax()-TalentGP()))
	          liste.push_back(sch->second.ID());
	  }
	  std::sort(liste.begin(), liste.end());
	} // AktivierbareTalente
	//@-node:AktivierbareTalente
	//@+node:ZauberAktivierbar
	bool PersonHandlerGenerator::ZauberAktivierbar() {
	  if (!(AktivierteZauber() < AktivierteZauberMax())) return false;
	
	  bool istSpruchzauberer = false;
	  for (Sonderfertigkeiten::const_iterator sf = P()->sonderfertigkeiten_.begin(); sf != P()->sonderfertigkeiten_.end(); ++sf)
	    if (sf->second.Definition()->IstRepraesentation())
	      istSpruchzauberer = true;
	      
	  if (!istSpruchzauberer)
	    return false;
	
	  for (DSA::ZauberDefinitionen::const_iterator z = DSA::zaubersprueche()->begin(); z != DSA::zaubersprueche()->end(); ++z) {
	   std::vector<DSA::Zauberrepraesentation> reps = z->second.VerfuegbareRepraesentationen();
	    for (std::vector<DSA::Zauberrepraesentation>::const_iterator r = reps.begin(); r != reps.end(); r++) {
	      Zauberfertigkeit neu;
	      neu.Zauberspruch(z->first);
	      neu.Repraesentation(*r);
	      if (!P()->zauberfertigkeiten_.Existiert(neu.ID()))
	        return true;
	    }
	  }
	  return false;
	} // TalentAktivierbar
	//@-node:ZauberAktivierbar
	//@+node:AktivierbareZauber
	Zauberfertigkeiten PersonHandlerGenerator::AktivierbareZauber() {
	  bool istSpruchzauberer = false;
	  for (Sonderfertigkeiten::const_iterator sf = P()->sonderfertigkeiten_.begin(); sf != P()->sonderfertigkeiten_.end(); ++sf)
	    if (sf->second.Definition()->IstRepraesentation())
	      istSpruchzauberer = true;
	      
	  Zauberfertigkeiten zauber;
	  
	  if (!istSpruchzauberer)
	    return zauber;
	
	  for (DSA::ZauberDefinitionen::const_iterator z = DSA::zaubersprueche()->begin(); z != DSA::zaubersprueche()->end(); ++z) {
	   std::vector<DSA::Zauberrepraesentation> reps = z->second.VerfuegbareRepraesentationen();
	    for (std::vector<DSA::Zauberrepraesentation>::const_iterator r = reps.begin(); r != reps.end(); r++) {
	      Zauberfertigkeit neu;
	      neu.Zauberspruch(z->first);
	      neu.Repraesentation(*r);
	      if (!P()->zauberfertigkeiten_.Existiert(neu.ID()))
	        zauber.FuegeEin(neu);
	    }
	  }
	  return zauber; // TODO Aktivierungskosten checken?
	}
	//@nonl
	//@-node:AktivierbareZauber
	//@+node:AktivierungsKosten
	int PersonHandlerGenerator::AktivierungsKosten(const std::string& talent) {
	
	  if (talente()->Existiert(talent)) {
	    float faktor = 1.0;
	    
	    if (talente()->Zeige(talent)->Gruppe()==Wissen)
	      if (P()->charakteristika_.Existiert(_t("Akademische Ausbildung (Magier)"))
	            || P()->charakteristika_.Existiert(_t("Akademische Ausbildung (Gelehrter)")))
	        faktor = 0.5;
	        
	// "Begabung für ..." ist nicht kumulativ zu Akademische Ausbildung
	    if (P()->charakteristika_.Existiert(_t("Begabung für") + " " + talente()->Zeige(talent)->ID()))
	      return regeln()->Aktivierungsfaktor(SpalteLeichter(talente()->Zeige(talent)->SKTSpalte_()))+0.5;
	    else if (P()->charakteristika_.Existiert(_t("Begabung für") + " " + TalentgruppeString(talente()->Zeige(talent)->Gruppe(), false)))
	      return regeln()->Aktivierungsfaktor(SpalteLeichter(talente()->Zeige(talent)->SKTSpalte_()))+0.5;
	    else
	      return ceil((regeln()->Aktivierungsfaktor(talente()->Zeige(talent)->SKTSpalte_())+0.5)*faktor);
	  }
	
	  if (kampftechniken()->Existiert(talent)) {
	    float faktor = 1.0;
	    
	    if (P()->charakteristika_.Existiert(_t("Akademische Ausbildung (Krieger)")))
	      faktor = 0.5;
	
	    
	    if (P()->charakteristika_.Existiert(_t("Begabung für") + " " + TalentgruppeString(Kampf, false)))
	      return regeln()->Aktivierungsfaktor(SpalteLeichter(kampftechniken()->Zeige(talent)->SKTSpalte_()))+0.5;
	    else if (P()->charakteristika_.Existiert(_t("Begabung für") + " " + kampftechniken()->Zeige(talent)->ID()))
	      return regeln()->Aktivierungsfaktor(SpalteLeichter(kampftechniken()->Zeige(talent)->SKTSpalte_()))+0.5;
	    else
	      return ceil((regeln()->Aktivierungsfaktor(kampftechniken()->Zeige(talent)->SKTSpalte_())+0.5)*faktor);
	  }
	
	  std::string temptal = talent;
	  std::pair<bool, bool> rv = SpracheOderSchrift(temptal);
	  if (rv.first) {
	    float faktor = 1.0;
	    
	    if (P()->charakteristika_.Existiert(_t("Akademische Ausbildung (Magier)")))
	      faktor = 0.5;
	  
	    if (rv.second) {
	      if (sprachen()->Existiert(temptal)) {
	        if (P()->charakteristika_.Existiert(_t("Akademische Ausbildung (Gelehrter)")))
	          faktor = 0.5;
	        return ceil((regeln()->Aktivierungsfaktor(sprachen()->Zeige(temptal)->SKTSpalte_())+0.5)*faktor);
	      }
	      } else {
	      if (schriften()->Existiert(temptal))
	        return ceil((regeln()->Aktivierungsfaktor(schriften()->Zeige(temptal)->SKTSpalte_())+0.5)*faktor);
	    }
	  }
	  
	  if ((talent.find("(")<talent.size())&&(talent.find(")")<talent.size())) {
	   DSA::Zauberrepraesentation rep = DSA::ZauberrepraesentationString(talent.substr(talent.find_last_of("(")+1, talent.find_last_of(")")-(talent.find_last_of("(")+1)));
	    std::string spruch = talent.substr(0, talent.find_last_of("(")-1);
	
	    if (zaubersprueche()->Existiert(spruch)) {
	      Zauberfertigkeit neu;
	      neu.Zauberspruch(spruch);
	      neu.Repraesentation(rep);
	      neu.Bezugsperson(P());
	      return regeln()->Aktivierungsfaktor(neu.Lernschwierigkeit());
	    }
	  }
	
	  Log::Error("PersonHandlerGenerator::AktivierungsKosten - Talent " + talent + " nicht erkannt.");
	  return 0;
	} // AktivierungsKosten
	//@nonl
	//@-node:AktivierungsKosten
	//@+node:Aktiviere Talent
	void PersonHandlerGenerator::Aktiviere(const std::string& talent) {
	  if (talente()->Existiert(talent)) {
	    Talent neu(talente()->Zeige(talent));
	    neu.Bezugsperson(P());
	    P()->talente_.FuegeEin(neu);
	    return;
	  }
	  if (kampftechniken()->Existiert(talent)) {
	    Kampftechnik neu(kampftechniken()->Zeige(talent));
	    neu.Bezugsperson(P());
	    P()->kampftechniken_.FuegeEin(neu);
	    return;
	  }
	  std::string temptal = talent;
	  std::pair<bool, bool> rv = SpracheOderSchrift(temptal);
	  if (rv.first) 
	    if (rv.second) {
	      if (sprachen()->Existiert(temptal)) {
	        Sprache neu(sprachen()->Zeige(temptal));
	        neu.Bezugsperson(P());
	        P()->sprachen_.FuegeEin(neu);
	        return;
	      }
	    } else {
	      if (schriften()->Existiert(temptal)) {
	        Schrift neu(schriften()->Zeige(temptal));
	        neu.Bezugsperson(P());
	        P()->schriften_.FuegeEin(neu);
	        return;
	      }
	    }
	
	  if ((talent.find("(")<talent.size())&&(talent.find(")")<talent.size())) {
	   DSA::Zauberrepraesentation rep = DSA::ZauberrepraesentationString(talent.substr(talent.find_last_of("(")+1, talent.find_last_of(")")-(talent.find_last_of("(")+1)));
	    std::string spruch = talent.substr(0, talent.find_last_of("(")-1);
	
	    if (zaubersprueche()->Existiert(spruch)) {
	      Zauberfertigkeit neu;
	      neu.Zauberspruch(spruch);
	      neu.Repraesentation(rep);
	      neu.Bezugsperson(P());
	      P()->zauberfertigkeiten_.FuegeEin(neu);
	      return;  
	    }
	  }
	  
	  Log::Error("PersonHandlerGenerator::Aktiviere - unbekanntes Objekt " + talent);
	} // Aktiviere
	//@nonl
	//@-node:Aktiviere Talent
	//@+node:TalentSteigerbar
	bool PersonHandlerGenerator::TalentSteigerbar(const std::string& talent) {
		//@  << Talente >>
		//@+node:<< Talente >>
		if (P()->talente_.Existiert(talent)) {
		
		  // hindernde Unfaehigkeiten?
		  if ((P()->charakteristika_.Existiert(_t("Unfähigkeit für") + " " + talent))
		    ||(P()->charakteristika_.Existiert(_t("Unfähigkeit für") + " " + TalentgruppeString(P()->talente_.Zeige(talent)->Gruppe(), false))))
		    return false;
		    
		  float faktor = 1.0;
		  
		  if (talente()->Zeige(talent)->Gruppe()==Wissen)
		    if (P()->charakteristika_.Existiert(_t("Akademische Ausbildung (Magier)"))
		            || P()->charakteristika_.Existiert(_t("Akademische Ausbildung (Gelehrter)")))
		        {
		          int maxaka = 10;
		          if ( P()->charakteristika_.Existiert(_t("Akademische Ausbildung (Gelehrter)"))
		              && P()->charakteristika_.Existiert(_t("Veteran")))
		      maxaka = 15;	
		      if (P()->talente_.Zeige(talent)->Wert()<maxaka)
		        faktor = 0.5;
		        }
		
		// "Begabung für ..." ist nicht kumulativ mit Akademischer Ausbildung		
		  if (P()->charakteristika_.Existiert(_t("Begabung für") + " " + talente()->Zeige(talent)->ID()) ||
		     (P()->charakteristika_.Existiert(_t("Begabung für") + " " + TalentgruppeString(talente()->Zeige(talent)->Gruppe(), false))))
		     faktor = 1.0;
		     
		  // genug TGP?
		  if (ceil(regeln()->SKT(P()->talente_.Hole(talent).SKTSpalte_(), P()->talente_.Zeige(talent)->Wert()+1)*faktor)
		      > (TalentGPMax()-TalentGP())) return false;
		  
		  Talent* t = P()->talente_.Zeige(talent);
		  return (t->Wert() < t->Maximalwert());
		}
		//@-node:<< Talente >>
		//@nl
		//@  << Kampftechniken >>
		//@+node:<< Kampftechniken >>
		if (P()->kampftechniken_.Existiert(talent)) {
		  // hindernde Unfaehigkeiten
		  if ((P()->charakteristika_.Existiert(_t("Unfähigkeit für") + " " + talent))
		    ||(P()->charakteristika_.Existiert(_t("Unfähigkeit für") + " " + TalentgruppeString(Kampf, false))))
		    return false;
		    
		  float faktor = 1.0;
		
		  if (P()->charakteristika_.Existiert(_t("Akademische Ausbildung (Krieger)"))) {
		  int maxaka = 10;
		  if (P()->charakteristika_.Existiert(_t("Veteran")))
		    maxaka = 15;
		    if (P()->kampftechniken_.Zeige(talent)->Wert()<maxaka)
		      faktor = 0.5;
		  }
		      
		// "Begabung für ..." ist nicht kumulativ zu Akademische Ausbildung
		  if (P()->charakteristika_.Existiert(_t("Begabung für") + " " + talent) ||
		     (P()->charakteristika_.Existiert(_t("Begabung für") + " " + TalentgruppeString(Kampf, false))))
		     faktor = 1.0;
		
		  // genug TGP?
		  if (ceil(regeln()->SKT(P()->kampftechniken_.Hole(talent).SKTSpalte_(), P()->kampftechniken_.Zeige(talent)->Wert()+1)*faktor)
		   > (TalentGPMax()-TalentGP())) return false;
		   
		  Kampftechnik* k = P()->kampftechniken_.Zeige(talent);    
		  return (k->Wert() < k->Maximalwert());
		}
		//@nonl
		//@-node:<< Kampftechniken >>
		//@nl
		//@  << Sprachen und Schriften >>
		//@+node:<< Sprachen und Schriften >>
		std::string temptal = talent;
		std::pair<bool, bool> rv = SpracheOderSchrift(temptal);
		if (rv.first) {
		  bool aka = (P()->charakteristika_.Existiert(_t("Akademische Ausbildung (Magier)")));
		  if (rv.second) { // ist eine Sprache
		  
		    if (P()->sprachen_.Existiert(temptal)) {
		      if (P()->charakteristika_.Existiert(_t("Unfähigkeit für") + " " + _t("Sprachen")))
		        return false;
		          
		          int maxaka = 10;
		      if (P()->charakteristika_.Existiert(_t("Akademische Ausbildung (Gelehrter)")))
		      {
		        aka = true;
		        if (P()->charakteristika_.Existiert(_t("Veteran")))
		          maxaka = 15;
		      }
		
		      if ((P()->sprachen_.Zeige(temptal)->Wert()<maxaka)&&(aka))
		        if (ceil(regeln()->SKT(sprachen()->Hole(temptal).SKTSpalte_(), P()->sprachen_.Zeige(temptal)->Wert()+1)*0.5)
		       > (TalentGPMax()-TalentGP())) return false;
		      else
		        if (regeln()->SKT(sprachen()->Hole(temptal).SKTSpalte_(), P()->sprachen_.Zeige(temptal)->Wert()+1)
		         > (TalentGPMax()-TalentGP())) return false;
		
		      int talwert = P()->sprachen_.Zeige(temptal)->Wert();
		      // Komplexität
		      if (sprachen()->Hole(temptal).Komplexitaet()<=talwert) return false;
		      int maxeigwert = max(P()->Wert(Klugheit), P()->Wert(Intuition), P()->Wert(Charisma));
		      // TODO Magische Eigenschaften
		      return (talwert < maxeigwert + 3); // TODO Magische Zahl
		    }
		    
		  } else { // ist eine Schrift
		  
		    if (P()->schriften_.Existiert(temptal)) {
		      
		      if ((P()->schriften_.Zeige(temptal)->Wert()<10)&&(aka))
		        if (ceil(regeln()->SKT(schriften()->Hole(temptal).SKTSpalte_(), P()->schriften_.Zeige(temptal)->Wert()+1)*0.5)
		       > (TalentGPMax()-TalentGP())) return false;
		       else
		        if (regeln()->SKT(schriften()->Hole(temptal).SKTSpalte_(), P()->schriften_.Zeige(temptal)->Wert()+1)
		         > (TalentGPMax()-TalentGP())) return false;
		      int talwert = P()->schriften_.Zeige(temptal)->Wert();
		      // Komplexität
		      if (schriften()->Hole(temptal).Komplexitaet()<=talwert) return false;
		      if (talente()->Zeige(_t("Lesen / Schreiben"))==0) return true; // Wenn das Talent fehlt
		      int maxeigwert = max(P()->Wert(talente()->Zeige(_t("Lesen / Schreiben"))->Eigenschaft(0)),
		      	P()->Wert(talente()->Zeige(_t("Lesen / Schreiben"))->Eigenschaft(1)),
		      	P()->Wert(talente()->Zeige(_t("Lesen / Schreiben"))->Eigenschaft(2)));
		      return (talwert < maxeigwert + 3); // TODO Magische Zahl
		    }
		  }
		}
		//@nonl
		//@-node:<< Sprachen und Schriften >>
		//@nl
		//@  << Zauber >>
		//@+node:<< Zauber >>
		if (P()->zauberfertigkeiten_.Existiert(talent)) {
		
		  // hindernde Unfaehigkeiten? TODO
		  //if ((P()->charakteristika_.Existiert(_t("Unfähigkeit für") + " " + talent))
		//    ||(P()->charakteristika_.Existiert(_t("Unfähigkeit für") + " " + TalentgruppeString(P()->talente_.Zeige(talent)->Gruppe(), false))))
		//    return false;
		    
		  Zauberfertigkeit* z = P()->zauberfertigkeiten_.Zeige(talent);
		
		  // genug TGP?
		  if (regeln()->SKT(z->Lernschwierigkeit(), z->Wert()+1)
		      > (TalentGPMax()-TalentGP())) return false;
		  
		  return (z->Wert() < z->Maximalwert());
		}
		//@nonl
		//@-node:<< Zauber >>
		//@nl
	  // wenn nicht bisher abgehandelt:
	  return false;
	} // TalentSteigerbar
	//@nonl
	//@-node:TalentSteigerbar
	//@+node:TalentSenkbar
	bool PersonHandlerGenerator::TalentSenkbar(const std::string& talent) {
	  if (P()->kampftechniken_.Existiert(talent)) {
	    if (!kampftechnikenbasis_.Existiert(talent)) return true;
	    return (P()->kampftechniken_.Zeige(talent)->Wert() > kampftechnikenbasis_.Zeige(talent)->Wert());
	  }
	  if (P()->talente_.Existiert(talent)) {
	    if (!talentebasis_.Existiert(talent)) return true;
	    return (P()->talente_.Zeige(talent)->Wert() > talentebasis_.Zeige(talent)->Wert());
	  }
	  std::string temptal = talent;
	  std::pair<bool, bool> rv = SpracheOderSchrift(temptal);
	  if (rv.first)
	    if (rv.second) {
	      if (P()->sprachen_.Existiert(temptal)) {
	        if (!sprachenbasis_.Existiert(temptal)) return true;
	        return (P()->sprachen_.Zeige(temptal)->Wert() > sprachenbasis_.Zeige(temptal)->Wert());
	      }
	    } else {
	      if (P()->schriften_.Existiert(temptal)) {
	        if (!schriftenbasis_.Existiert(temptal)) return true;
	        return (P()->schriften_.Zeige(temptal)->Wert() > schriftenbasis_.Zeige(temptal)->Wert());
	      }
	    }
	  if (P()->zauberfertigkeiten_.Existiert(talent)) {
	    if (!zauberbasis_.Existiert(talent)) return true;
	    return (P()->zauberfertigkeiten_.Zeige(talent)->Wert() > zauberbasis_.Zeige(talent)->Wert());
	  }
	
	  return false;
	} // TalentSenkbar
	//@nonl
	//@-node:TalentSenkbar
	//@+node:TalentSteigern
	void PersonHandlerGenerator::TalentSteigern(const std::string& talent) {
	  if (!TalentSteigerbar(talent)) { Log::Debug("PersonHandlerGenerator::TalentSteigern: " + _t("Konnte Talent nicht steigern.")); return; }
	  if (talente()->Existiert(talent)) {
	    P()->talente_.Zeige(talent)->Wert(P()->talente_.Zeige(talent)->Wert()+1);
	    return;
	  }
	  if (kampftechniken()->Existiert(talent)) {
	    P()->kampftechniken_.Zeige(talent)->Wert(P()->kampftechniken_.Zeige(talent)->Wert()+1);
	    return;
	  }
	  std::string temptal = talent;
	  std::pair<bool, bool> rv = SpracheOderSchrift(temptal);
	  if (rv.first)
	    if (rv.second) {
	      if (sprachen()->Existiert(temptal)) {
	        P()->sprachen_.Zeige(temptal)->Wert(P()->sprachen_.Zeige(temptal)->Wert()+1);
	        return;
	      }
	    } else {
	      if (schriften()->Existiert(temptal)) {
	        P()->schriften_.Zeige(temptal)->Wert(P()->schriften_.Zeige(temptal)->Wert()+1);
	        return;
	      }
	    }
	  if (P()->zauberfertigkeiten_.Existiert(talent)) {
	    Zauberfertigkeit* z = P()->zauberfertigkeiten_.Zeige(talent);
	    z->Wert(z->Wert()+1);
	    return;
	  }
	} // TalentSteigern
	//@nonl
	//@-node:TalentSteigern
	//@+node:TalentSenken
	void PersonHandlerGenerator::TalentSenken(const std::string& talent) {
	  if (!TalentSenkbar(talent)) { Log::Debug("PersonHandlerGenerator::TalentSenken: " + _t("Konnte Talent nicht senken.")); return; }
	  if (talente()->Existiert(talent)) {
	    if (!talentebasis_.Existiert(talent))
	      if (P()->talente_.Zeige(talent)->Wert() == 0) {
	        P()->talente_.Loesche(talent);
	        return;
	      }
	    P()->talente_.Zeige(talent)->Wert(P()->talente_.Zeige(talent)->Wert()-1);
	    return;
	  }
	  if (kampftechniken()->Existiert(talent)) {
	    if (!kampftechnikenbasis_.Existiert(talent))
	      if (P()->kampftechniken_.Zeige(talent)->Wert() == 0) {
	        P()->kampftechniken_.Loesche(talent);
	        return;
	       }
	    P()->kampftechniken_.Zeige(talent)->Wert(P()->kampftechniken_.Zeige(talent)->Wert()-1);
	    return;
	  }
	  std::string temptal = talent;
	  std::pair<bool, bool> rv = SpracheOderSchrift(temptal);
	  if (rv.first)
	    if (rv.second) {
	      if (sprachen()->Existiert(temptal)) {
	        if (!sprachenbasis_.Existiert(temptal))
	          if (P()->sprachen_.Zeige(temptal)->Wert() == 0) {
	            P()->sprachen_.Loesche(temptal);
	            return;
	          }
	        P()->sprachen_.Zeige(temptal)->Wert(P()->sprachen_.Zeige(temptal)->Wert()-1);
	        return;
	      }
	    } else {
	      if (schriften()->Existiert(temptal)) {
	        if (!schriftenbasis_.Existiert(temptal))
	          if (P()->schriften_.Zeige(temptal)->Wert() == 0) {
	            P()->schriften_.Loesche(temptal);
	            return;
	          }
	        P()->schriften_.Zeige(temptal)->Wert(P()->schriften_.Zeige(temptal)->Wert()-1);
	        return;
	      }
	    }
	  if (P()->zauberfertigkeiten_.Existiert(talent)) {
	    if (!zauberbasis_.Existiert(talent))
	      if (P()->zauberfertigkeiten_.Zeige(talent)->Wert() == 0) {
	        P()->zauberfertigkeiten_.Loesche(talent);
	        return;
	      }
	    P()->zauberfertigkeiten_.Zeige(talent)->Wert(P()->zauberfertigkeiten_.Zeige(talent)->Wert()-1);
	    return;
	  }
	    
	} // TalentSenken
	//@nonl
	//@-node:TalentSenken
	//@+node:AufATSteigerbar
	bool PersonHandlerGenerator::AufATSteigerbar(const std::string& kampftechnik) {
	  if (!P()->kampftechniken_.Existiert(kampftechnik)) return false;
	  Kampftechnik* k = P()->kampftechniken_.Zeige(kampftechnik);
	  return k->WaereOKAufAT(k->AufAT()+1);
	} // AufATSteigerbar
	//@nonl
	//@-node:AufATSteigerbar
	//@+node:AuPASteigerbar
	bool PersonHandlerGenerator::AufPASteigerbar(const std::string& kampftechnik) {
	  if (!P()->kampftechniken_.Existiert(kampftechnik)) return false;
	  Kampftechnik* k = P()->kampftechniken_.Zeige(kampftechnik);
	  return k->WaereOKAufPA(k->AufPA()+1);
	} // AufPASteigerbar
	//@nonl
	//@-node:AuPASteigerbar
	//@+node:AufATSteigern
	void PersonHandlerGenerator::AufATSteigern(const std::string& kampftechnik) {
	  if (!AufATSteigerbar(kampftechnik)) { Log::Debug("PersonHandlerGenerator::AufATSteigern: " + _t("Konnte AT nicht steigern.")); return; }
	  Kampftechnik* k = P()->kampftechniken_.Zeige(kampftechnik);
	  k->AufAT(k->AufAT()+1);
	} // AufATSteigern
	//@nonl
	//@-node:AufATSteigern
	//@+node:AufPASteigern
	void PersonHandlerGenerator::AufPASteigern(const std::string& kampftechnik) {
	  if (!AufPASteigerbar(kampftechnik)) { Log::Debug("PersonHandlerGenerator::AufPASteigern: " + _t("Konnte PA nicht steigern.")); return; }
	  Kampftechnik* k = P()->kampftechniken_.Zeige(kampftechnik);
	  k->AufPA(k->AufPA()+1);
	} // AufPASteigern
	//@nonl
	//@-node:AufPASteigern
	//@-others
	//@nonl
	//@-node:Talentoide
	//@+node:Vor- und Nachteile
	//@+others
	//@+node:CharakteristikaHinzufügbar
	bool PersonHandlerGenerator::CharakteristikaHinzufuegbar(int modus) {
	  CharakteristikumDefinitionen::iterator c;
	  for (c = charakteristika()->begin(); c != charakteristika()->end(); ++c)
	    if ((modus==0) || ((modus==1)&&(c->second.Vorteil())) || ((modus==2)&&(c->second.Nachteil())))
	      if (c->second.Hinzufuegbar(*P()))
	        return true;
	  for (c = generierteCharakteristika()->begin(); c != generierteCharakteristika()->end(); ++c)
	    if ((modus==0) || ((modus==1)&&(c->second.Vorteil())) || ((modus==2)&&(c->second.Nachteil())))
	      if (c->second.Hinzufuegbar(*P()))
	        return true;
	  return false;
	}
	
	//@-node:CharakteristikaHinzufügbar
	//@+node:HinzufuegbareCharakteristika
	void PersonHandlerGenerator::HinzufuegbareCharakteristika(StringListe& sl, int modus) {
	  CharakteristikumDefinitionen::iterator c;
	  for (c = charakteristika()->begin(); c != charakteristika()->end(); ++c)
	    if ((modus==0) || ((modus==1)&&(c->second.Vorteil())) || ((modus==2)&&(c->second.Nachteil())))
	      if (c->second.Hinzufuegbar(*P()))
	        sl.push_back(c->second.Text(*P()));
	  for (c = generierteCharakteristika()->begin(); c != generierteCharakteristika()->end(); ++c)
	    if ((modus==0) || ((modus==1)&&(c->second.Vorteil())) || ((modus==2)&&(c->second.Nachteil())))
	      if (c->second.Hinzufuegbar(*P()))
	        sl.push_back(c->second.Text(*P()));
	  std::sort(sl.begin(), sl.end());
	} // HinzufuegbareCharakteristika
	//@nonl
	//@-node:HinzufuegbareCharakteristika
	//@+node:FuegeHinzu Charakteristika
	void PersonHandlerGenerator::FuegeHinzu(const std::string& c) {
	  std::string cs = c;
	  if (!charakteristika()->Existiert(c)) {
	    CharakteristikumDefinitionen::iterator ci;
	    for (ci = charakteristika()->begin(); ci != charakteristika()->end(); ++ci)
	      if (ci->second.Text(*P())==c)
	        cs = ci->second.ID();
	    for (ci = generierteCharakteristika()->begin(); ci != generierteCharakteristika()->end(); ++ci)
	      if (ci->second.Text(*P())==c)
	        cs = ci->second.ID();
	  }
	  CharakteristikumDefinition chdef = (charakteristika()->Existiert(cs)) ? charakteristika()->Hole(cs) : generierteCharakteristika()->Hole(cs);
	  Charakteristikum ch(chdef); // TODO Verwandlung von CharDef in Char
	  if (chdef.BrauchtSpezifizierung())
	    ch.Spezifizierung(DSA::hole()->Text(_t("Wofür gilt") + " " + chdef.ID() +"?"));    
	
	  if (chdef.SchlechteEigenschaft()) {
	    ch.Stufe(regeln()->Regel("Minimaler Startwert einer Schlechten Eigenschaft"));
	    if (chdef.GPPro2Punkte()) if ((ch.Stufe()%2)==1) ch.Stufe(ch.Stufe()+1);
	  }
	  chareigen_.FuegeEin(ch);
	  BerechneCharFinal();
	  BerechneSFFinal();
	  PasseAn();
	} // FuegeHinzu (Charakteristikum)
	//@nonl
	//@-node:FuegeHinzu Charakteristika
	//@+node:Entferne Charakteristika
	/*  void PersonHandlerGenerator::Entferne(Charakteristika charakteristika) {
	  for (Charakteristika::iterator c = charakteristika.begin(); c != charakteristika.end(); ++c) {
	    P()->charakteristika_.Loesche(c->first);
	    charakteristikabasis_.Loesche(c->first);
	  }
	} // Entferne (charakteristika)
	
	void PersonHandlerGenerator::Setze(Charakteristika charakteristika) {
	  for (Charakteristika::iterator c = charakteristika.begin(); c != charakteristika.end(); ++c) {
	    P()->charakteristika_.FuegeEin(c->second);
	    charakteristikabasis_.FuegeEin(c->second);
	  }
	} // Setze (charakteristika)
	*/
	//@nonl
	//@-node:Entferne Charakteristika
	//@+node:CharakteristikumSteigerbar
	bool PersonHandlerGenerator::CharakteristikumSteigerbar(const std::string& c) {
	  Charakteristikum* chp = charfinal_.Zeige(c);
	  if (chp==0) return false;
	  if (!chp->Definition()->IstBasis()) {
	    if (!chp->HatStufen())
	      if (chp->SchlechteEigenschaft())
	        return ((((chp->Definition()->GPPro2Punkte())?1:0)+chp->Stufe())<regeln("Maximaler Startwert einer Schlechten Eigenschaft"));
	      else
	        return false;
	    if (chp->Definition()->MaximalStufe()==0) return true;
	    return (chp->Stufe()<chp->Definition()->MaximalStufe());
	  }
	  CharakteristikumDefinition* chdp = charakteristika()->Zeige(chp->Definition()->BasisFuer());
	  if (chdp==0) return false;
	  return (!chdp->Gebunden());
	} // CharakteristikumSteigerbar
	//@nonl
	//@-node:CharakteristikumSteigerbar
	//@+node:CharakteristikumSteigern
	void PersonHandlerGenerator::CharakteristikumSteigern(const std::string& c) {
	  if (!CharakteristikumSteigerbar(c)) return;
	  const CharakteristikumDefinition* chdp = charfinal_.Zeige(c)->Definition();
	  std::string cplus = chdp->BasisFuer();
	  Charakteristikum* chp = chareigen_.Zeige(c);
	  if (chdp->IstBasis()) {
	    if (chp!=0) chareigen_.Loesche(c);
	    FuegeHinzu(cplus);
	  } else if ((chdp->HatStufen())||(chdp->SchlechteEigenschaft())) {
	    if (chp==0) {
	      Charakteristikum chneu(chdp);
	      chneu.Stufe(chdp->GPPro2Punkte()?2:1);
	      chareigen_.FuegeEin(chneu);
	      BerechneCharFinal();
	      BerechneSFFinal();
	    } else {
	      chp->Stufe(chp->Stufe()+(chp->Definition()->GPPro2Punkte()?2:1));
	      BerechneCharFinal();
	      BerechneSFFinal();
	    }
	  }
	} // CharakteristikumSteigern
	//@nonl
	//@-node:CharakteristikumSteigern
	//@+node:CharakteristikumSenkbar
	bool PersonHandlerGenerator::CharakteristikumSenkbar(const std::string& c) {
	  return chareigen_.Existiert(c);
	} // CharakteristikumSenkbar
	//@nonl
	//@-node:CharakteristikumSenkbar
	//@+node:CharakteristikumSenken
	void PersonHandlerGenerator::CharakteristikumSenken(const std::string& c) {
	  if (!CharakteristikumSenkbar(c)) return;
	  Charakteristikum* chp = chareigen_.Zeige(c);
	  const CharakteristikumDefinition* chdp = chp->Definition();
	  if ((chp->HatStufen())||(chp->SchlechteEigenschaft()))
	    if (chp->Stufe()>(chdp->GPPro2Punkte()?2:1)) {
	      chp->Stufe(chp->Stufe()-(chdp->GPPro2Punkte()?2:1));
	      BerechneCharFinal();
	      return;
	    }
	  chareigen_.Loesche(c);
	  BerechneCharFinal();
	  BerechneSFFinal();
	} // CharakteristikumSenken
	//@nonl
	//@-node:CharakteristikumSenken
	//@-others
	//@nonl
	//@-node:Vor- und Nachteile
	//@+node:Sonderfertigkeiten
	//@+others
	//@+node:Sonderfertigkeiten Hinzufügbar
	bool PersonHandlerGenerator::SonderfertigkeitenHinzufuegbar() {
	  SonderfertigkeitDefinitionen::iterator sf;
	  for (sf = sonderfertigkeiten()->begin(); sf != sonderfertigkeiten()->end(); ++sf)
	    if (!P()->sonderfertigkeiten_.Existiert(sf->second.ID()))
	      if (sf->second.Hinzufuegbar(*P()))
	        return true;
	  for (sf = generierteSonderfertigkeiten()->begin(); sf != generierteSonderfertigkeiten()->end(); ++sf)
	    if (!P()->sonderfertigkeiten_.Existiert(sf->second.ID()))
	      if (sf->second.Hinzufuegbar(*P()))
	        return true;
	  return false;
	}
	//@nonl
	//@-node:Sonderfertigkeiten Hinzufügbar
	//@+node:HinzfuegbareSonderfertigkeiten
	void PersonHandlerGenerator::HinzufuegbareSonderfertigkeiten(StringListe& sl) {
	  SonderfertigkeitDefinitionen::iterator sf;
	  for (sf = sonderfertigkeiten()->begin(); sf != sonderfertigkeiten()->end(); ++sf)
	    if (!P()->sonderfertigkeiten_.Existiert(sf->second.ID()))
	      if (sf->second.Hinzufuegbar(*P()))
	        sl.push_back(sf->second.Text());
	  for (sf = generierteSonderfertigkeiten()->begin(); sf != generierteSonderfertigkeiten()->end(); ++sf)
	    if (!P()->sonderfertigkeiten_.Existiert(sf->second.ID()))
	      if (sf->second.Hinzufuegbar(*P()))
	        sl.push_back(sf->second.Text());
	} // HinzufuegbareSonderfertigkeiten
	//@nonl
	//@-node:HinzfuegbareSonderfertigkeiten
	//@+node:FuegeHinzu SF
	void PersonHandlerGenerator::FuegeHinzuSF(const std::string& sf) {
	  std::string sfs = sf;
	  if (!sonderfertigkeiten()->Existiert(sf)) {
	    SonderfertigkeitDefinitionen::iterator sfi;
	    for (sfi = sonderfertigkeiten()->begin(); sfi != sonderfertigkeiten()->end(); ++sfi)
	      if (sfi->second.Text()==sf) sfs = sfi->first;
	    for (sfi = generierteSonderfertigkeiten()->begin(); sfi != generierteSonderfertigkeiten()->end(); ++sfi)
	      if (sfi->second.Text()==sf) sfs = sfi->first;
	  }    
	  SonderfertigkeitDefinition* sfdef = (sonderfertigkeiten()->Existiert(sfs)) ? sonderfertigkeiten()->Zeige(sfs) : generierteSonderfertigkeiten()->Zeige(sfs);
	  if (sfdef==0) return;
	  if (!sfdef->Hinzufuegbar(*P())) return;
	  DSA::Sonderfertigkeit s = Sonderfertigkeit(sfdef);
	  if (sfdef->BrauchtSpezifizierung())
	    s.Spezifizierung(DSA::hole()->Text(_t("Wofür gilt") + " " + sfdef->ID() +"?"));
	  if (strings::BeginntMit(s.ID(), _t("Ritualkenntnis")))
	    s.Stufe(3); // TODO Magische Konstante
	
	  sfeigen_.FuegeEin(s);
	  BerechneSFFinal();
	} // FuegeHinzuSF
	//@nonl
	//@-node:FuegeHinzu SF
	//@+node:Entfernbar SF
	bool PersonHandlerGenerator::EntfernbarSF(const std::string& sf) {
	  if (sfeigen_.Existiert(sf))
	    return true;
	  return false;
	} // EntfernbarSF
	//@nonl
	//@-node:Entfernbar SF
	//@+node:Entferne SF
	void PersonHandlerGenerator::EntferneSF(const std::string& sf) {
	  if (!EntfernbarSF(sf)) return;
	  sfeigen_.Loesche(sf);
	  PasseAn();
	} // EntferneSF
	//@nonl
	//@-node:Entferne SF
	//@+node:Steigerbar SF
	bool PersonHandlerGenerator::SteigerbarSF(const std::string& sf) {
	  if (!strings::BeginntMit(sf, _t("Ritualkenntnis"))) return false;
	  
	  Sonderfertigkeit* s = sffinal_.Zeige(sf);
	  if (s==0) return false;
	  
	  return (s->Stufe()<s->MaximaleStufe(*P()));
	}
	//@nonl
	//@-node:Steigerbar SF
	//@+node:Steiger SF
	void PersonHandlerGenerator::SteigerSF(const std::string& sf) {
	  if (!SteigerbarSF(sf)) return;
	  Sonderfertigkeit* s = sffinal_.Zeige(sf);
	  if (s==0) return;
	  Sonderfertigkeit* se = sfeigen_.Zeige(sf);
	  if (se==0) {
	    Sonderfertigkeit RK = Sonderfertigkeit(s->Definition());
	    RK.Stufe(1);
	    sfeigen_.FuegeEin(RK);
	  } else {
	    se->Stufe(se->Stufe()+1);
	  }
	  BerechneSFFinal();
	}
	//@nonl
	//@-node:Steiger SF
	//@+node:Senkbar SF
	bool PersonHandlerGenerator::SenkbarSF(const std::string& sf) {
	  if (!strings::BeginntMit(sf, _t("Ritualkenntnis")))
	    return false;
	  return (sfeigen_.Existiert(sf));
	}
	//@nonl
	//@-node:Senkbar SF
	//@+node:Senke SF
	void PersonHandlerGenerator::SenkeSF(const std::string& sf) {
	  if (!SenkbarSF(sf)) return;
	  Sonderfertigkeit* se = sfeigen_.Zeige(sf);
	  Sonderfertigkeit* sg = sffinal_.Zeige(sf);
	  if ((se==0)||(sg==0)) return;
	  
	  if ((se->Stufe()==1)||((se->Stufe()==sg->Stufe())&&(se->Stufe()==3)))
	    EntferneSF(sf);
	  else
	    se->Stufe(se->Stufe()-1);
	
	  BerechneSFFinal();
	
	}
	//@nonl
	//@-node:Senke SF
	//@-others
	//@nonl
	//@-node:Sonderfertigkeiten
	//@+node:Alter
	int PersonHandlerGenerator::Alter() {
	  int alter = P()->Alter();
	  alter += P()->Profession_().Dauer();
	  if (P()->charakteristika_.Existiert(_t("Veteran")))
	    alter += 5;
	  if (P()->charakteristika_.Existiert(_t("Breitgefächerte Bildung")))
	    alter += 6;
	  if (P()->charakteristika_.Existiert(_t("Gebildet")))
	    alter += (P()->charakteristika_.Hole(_t("Gebildet")).Stufe()+1)/2;
	  return alter;
	} // Alter
	//@nonl
	//@-node:Alter
	//@+node:Gewicht
	int PersonHandlerGenerator::Gewicht() {
	  int gewicht = P()->Gewicht();
	  if (P()->charakteristika_.Existiert(_t("Fettleibig")))
	    gewicht *= 1.8;
	  return gewicht;
	} // Alter
	//@nonl
	//@-node:Gewicht
	//@+node:PasseAn
	void PersonHandlerGenerator::PasseAn() {
	  {
	    for (Kampftechniken::iterator k = P()->kampftechniken_.begin(); k != P()->kampftechniken_.end(); ++k)
	      while (true) {
	        if (kampftechnikenbasis_.Existiert(k->first)!=0)
	          if (kampftechnikenbasis_.Zeige(k->first)->Wert() >= k->second.Wert())
	            break;
	        int maxwert = (P()->Wert(Mut)>P()->Wert(Koerperkraft)) ?
	                       P()->Wert(Mut):P()->Wert(Koerperkraft);
	        // TODO Magische Eigenschaften
	        maxwert += 3; // TODO Magische Zahl
	        if ((P()->charakteristika_.Existiert(_t("Unfähigkeit für") + " " + k->first))
	          ||(P()->charakteristika_.Existiert(_t("Unfähigkeit für") + " " + TalentgruppeString(Kampf, false)))) {
	          if (kampftechnikenbasis_.Existiert(k->first))
	            maxwert = kampftechnikenbasis_.Zeige(k->first)->Wert();
	          else
	            maxwert = 0;
	        }
	        if (k->second.Wert() > maxwert)
	          TalentSenken(k->first);
	        else {
	          if ((k->second.Art()!=Basis)&&(maxwert==0)) {
	            TalentSenken(k->first);           
	            goto nochmal;
	          }
	          break;
	        }
	      }
	  }
	  {
	    for (Talente::iterator t = P()->talente_.begin(); t != P()->talente_.end(); ++t)
	      while (true) {
	        if (talentebasis_.Existiert(t->first))
	          if (talentebasis_.Zeige(t->first)->Wert() >= t->second.Wert())
	            break;
	        int talwert = t->second.Wert();
	        int eigwert = 0;
	        int maxwert = 0;
	        for (int i = 0; i < EigenschaftenProTalentprobe; ++i)
	          maxwert = // da std::max in meiner Bib nicht existiert, folgender workaround
	            ((eigwert = P()->Wert(t->second.Eigenschaft(i))) > maxwert) ?
	              eigwert : maxwert;
	        maxwert += 3; // TODO Magische Zahl
	        if ((P()->charakteristika_.Existiert(_t("Unfähigkeit für") + " " + t->first))
	          ||(P()->charakteristika_.Existiert(_t("Unfähigkeit für") + " " + TalentgruppeString(t->second.Gruppe(), false)))) {
	          if (talentebasis_.Existiert(t->first))
	            maxwert = talentebasis_.Zeige(t->first)->Wert();
	          else
	            maxwert = 0;
	        }
	        if (talwert > maxwert)
	          TalentSenken(t->first);
	        else {
	          if ((t->second.Art()!=Basis)&&(maxwert==0)) {
	            TalentSenken(t->first);           
	            goto nochmal;
	          }
	          break;
	        }
	      }
	  }
	  {
	    for (Sprachen::iterator sp = P()->sprachen_.begin(); sp != P()->sprachen_.end(); ++sp)
	      while (true) {
	        if (sprachenbasis_.Existiert(sp->first))
	          if (sprachenbasis_.Zeige(sp->first)->Wert() >= sp->second.Wert())
	            break;
	        int talwert = sp->second.Wert();
	        int maxwert = 0;
	        maxwert = // da std::max in meiner Bib nicht existiert, folgender workaround
	          P()->Wert(Klugheit) > P()->Wert(Intuition) ?
	          P()->Wert(Klugheit) : P()->Wert(Intuition);
	        maxwert =
	          maxwert > P()->Wert(Charisma) ?
	          maxwert : P()->Wert(Charisma);
	        maxwert += 3; // TODO Magische Zahl
	        if (P()->charakteristika_.Existiert(_t("Unfähigkeit für") + " " + _t("Sprachen"))) {
	          if (sprachenbasis_.Existiert(sp->first))
	            maxwert = sprachenbasis_.Zeige(sp->first)->Wert();
	          else
	            maxwert = 0;
	        }
	        if (talwert > maxwert)
	          TalentSenken(sp->second.ID(true));
	        else {
	          if (maxwert==0) {
	            TalentSenken(sp->second.ID(true));
	            goto nochmal;
	          }
	          break;
	        }
	      }
	  }
	  {
	    for (Schriften::iterator sch = P()->schriften_.begin(); sch != P()->schriften_.end(); ++sch)
	      while (true) {
	        if (schriftenbasis_.Zeige(sch->first)!=0)
	          if (schriftenbasis_.Zeige(sch->first)->Wert() >= sch->second.Wert())
	            break;
	        if (talente()->Zeige(_t("Lesen / Schreiben"))==0) break; // Wenn das Talent fehlt
	        int talwert = sch->second.Wert();
	        int eigwert = 0;
	        int maxeigwert = 0;
	        for (int i = 0; i < EigenschaftenProTalentprobe; ++i)
	          maxeigwert = // da std::max in meiner Bib nicht existiert, folgender workaround
	            ((eigwert = P()->Wert(talente()->Zeige(_t("Lesen / Schreiben"))->Eigenschaft(i))) > maxeigwert) ?
	              eigwert : maxeigwert;
	        if (talwert > maxeigwert + 3) // TODO Magische Zahl
	          TalentSenken(sch->second.ID(true));
	        else
	          break;
	      }
	  }
	  {
	    for (int sfiter = 0; sfiter < P()->sonderfertigkeiten_.size(); ++sfiter) {
	      Sonderfertigkeit sf = P()->sonderfertigkeiten_.Hole(sfiter);
	      if (EntfernbarSF(sf.ID()))
	        if (!sf.Definition()->Hinzufuegbar(*P())) {
	          P()->sonderfertigkeiten_.Loesche(sf.ID());
	          --sfiter;
	        }
	    }
	  }
	  return;
	  nochmal: PasseAn();
	} // PasseAn
	//@nonl
	//@-node:PasseAn
	//@+node:Abschliessen
	void PersonHandlerGenerator::Abschliessen() {
	  P()->GP(regeln("Start-GP"));
	  BerechneEigenschaftenMaxima();
	  PersonHandler::Alter(Alter());
	  PersonHandler::Gewicht(Gewicht());
	  Profession p = P()->Profession_();
	  p.Bezeichnung(P()->Profession_().GeschlechtlichechtsspezifischeBezeichnung(P()->Maennlich()));
	  P()->Profession_(p);
	} // Abschliessen
	//@-node:Abschliessen
	//@+node:BerechneCharFinal
	int PersonHandlerGenerator::BerechneCharFinal(const bool nurNachteile, const bool reGen) {
	  int gp = 0;
	  
		//@  << Chars aus RKP >>
		//@+node:<< Chars aus RKP >>
		// Speicher in chartemp die Chars der Rasse
		Charakteristika chartemp;
		chartemp.clear();
		
		int c = 0;
		for (c = 0; c < P()->Rasse_().charakteristika_.size(); ++c) {
		  chartemp.FuegeEin(P()->Rasse_().charakteristika_.Hole(c));
		}
		
		//@+at
		// Füge die Chars der Kultur und Profession hinzu. Bei 
		// Doppelungen werden GP frei bzw. die nächst höhere 
		// Stufe wird genommen.
		//@-at
		//@@c
		
		for (int a = 0; a != 2; ++a) {
		  Charakteristika chars =
		    (a==0?P()->Kultur_().charakteristika_:P()->Profession_().charakteristika_);
		  for (c = 0; c != chars.size(); ++c) {
		    Charakteristikum ch = chars.Hole(c);
		    const CharakteristikumDefinition* chd = ch.Definition();
		    if (chartemp.Existiert(ch.ID())) {
		      if (chd->IstBasis()) {
		        chartemp.Loesche(ch.ID());
		        Charakteristikum cherweitert(charakteristika()->Hole(chd->BasisFuer()));
		        chartemp.FuegeEin(cherweitert);
		      } else if (chd->SchlechteEigenschaft()) {
		        chartemp.Zeige(ch.ID())->Stufe(chartemp.Zeige(ch.ID())->Stufe()+ch.Stufe());
		      } else if (chd->HatStufen()) {
		        int neueStufe = chartemp.Zeige(ch.ID())->Stufe() + ch.Stufe();
		//@+at
		//         if (neueStufe>chd->MaximalStufe()) {
		//           int diff = neueStufe - 
		// chd->MaximalStufe();
		//           neueStufe = chd->MaximalStufe();
		//           gp += Hinzu(_t("Doppelter Vor-/Nachteil") 
		// + ": " + chd->ID(), -((chd->GP()*diff)+1)/3);
		//         }
		// 
		// Dieser Codeschnipsel wäre erforderlich, wenn die 
		// Stufenaddition eines VNT nicht den Maximalwert 
		// überschreiten dürfte. Laut AZ 11 ist das jedoch 
		// ausdrücklich erlaubt.
		//@-at
		//@@c
		        chartemp.Zeige(ch.ID())->Stufe(neueStufe);
		      } else
		        gp += Hinzu(_t("Doppelter Vor-/Nachteil") + ": " + chd->ID(), -(chd->GP()+1)/3);
		    } else {
		      if (chd->IstBasis()) {
		        if (chartemp.Existiert(chd->BasisFuer()))
		          gp += Hinzu(_t("Doppelter Vor-/Nachteil") + ": " + chd->ID(), -(chd->GP()+1)/3);
		        else
		          chartemp.FuegeEin(ch);
		      } else {
		        if (chd->HatBasis()) {
		          if (chartemp.Existiert(chd->BasiertAuf())) {
		            chartemp.Loesche(chd->BasiertAuf());
		            gp += Hinzu(_t("Doppelter Vor-/Nachteil") + ": " + chd->BasiertAuf(), -(charakteristika()->Hole(chd->BasiertAuf()).GP()+1)/3);
		            chartemp.FuegeEin(ch);
		          } else
		            chartemp.FuegeEin(ch);
		        } else
		          chartemp.FuegeEin(ch);
		      }
		    }
		  }
		}
		
		
		// Setze den Helden auf die Chars von RKP
		P()->charakteristika_ = charbasis_ = chartemp;
		if (nurNachteile) gp = 0;
		//@nonl
		//@-node:<< Chars aus RKP >>
		//@nl
	  
	  if (reGen) {
			//@    << Berechne chareigen_ >>
			//@+node:<< Berechne chareigen_ >>
			// gesucht: chareigen = charfinal - charbasis
			chareigen_ = charfinal_;
			
			for (Charakteristika::const_iterator abzieh = charbasis_.begin(); abzieh != charbasis_.end(); ++abzieh) {
			
			  if (charfinal_.Existiert(abzieh->first))
			    if ((abzieh->second.HatStufen()) || (abzieh->second.SchlechteEigenschaft())) {
			      int eigenwert = charfinal_.Zeige(abzieh->first)->Stufe();
			      int basiswert = abzieh->second.Stufe();
			      if (eigenwert > basiswert)
					    chareigen_.Zeige(abzieh->first)->Stufe(eigenwert - basiswert);
			      else
			        chareigen_.Loesche(abzieh->first);
			    } else
					  chareigen_.Loesche(abzieh->first);
			
			}
			        
			        
			      
			//@nonl
			//@-node:<< Berechne chareigen_ >>
			//@nl
	  }
	  
		//@  << Ueberpruefe chareigen_ >>
		//@+node:<< Ueberpruefe chareigen_ >>
		// Behandle jetzt jedes einzelne, hinzugefügt Char (aus chareigen_)
		Charakteristika chartemp2;
		Charakteristika charrest = chareigen_;
		while (charrest != chartemp2) {
		  charrest = chartemp2;
		  for (c = 0; c != chareigen_.size(); ++c) {
		    Charakteristikum* chp = chareigen_.Zeige(c);
		  
				//@    << if Niedrige Magieresistenz >>
				//@+node:<< if Niedrige Magieresistenz >>
				if (chp->ID() == _t("Niedrige Magieresistenz")) // MR darf nicht durch Niedrige MR unter 0 fallen, AH 114
				  if (P()->Wert(Magieresistenz) < 1)
				    continue;
				  else
				    if (P()->Wert(Magieresistenz) < chp->Stufe())
				      chp->Stufe(P()->Wert(Magieresistenz));
				//@nonl
				//@-node:<< if Niedrige Magieresistenz >>
				//@nl
				//@    << if Schlechte Eigenschaft >>
				//@+node:<< if Schlechte Eigenschaft >>
				if (chp->SchlechteEigenschaft()) {
				  if (charbasis_.Existiert(chp->ID())) {
				    int basis = charbasis_.Hole(chp->ID()).Stufe();
				    int diff = (basis + chp->Stufe()) - regeln("Maximaler Startwert einer Schlechten Eigenschaft");
				    if (diff>0)
				      if (basis>regeln("Maximaler Startwert einer Schlechten Eigenschaft"))
				        continue;
				      else
				        chp->Stufe(regeln("Maximaler Startwert einer Schlechten Eigenschaft")-basis);
				    if (chp->Definition()->GPPro2Punkte())
				      if ((chp->Stufe()%2)==1)
				        chp->Stufe(chp->Stufe()-1);
				    if (chp->Stufe()!=0)
				      chartemp2.FuegeEin(chareigen_.Hole(c));
				  } else
				  if (chp->Stufe() < regeln("Minimaler Startwert einer Schlechten Eigenschaft"))
				    continue;
				}
				//@nonl
				//@-node:<< if Schlechte Eigenschaft >>
				//@nl
				//@    << if Stufig >>
				//@+node:<< if Stufig >>
				if (chp->HatStufen()) {
				  if (charbasis_.Existiert(chp->ID())) {
				    int basis = charbasis_.Hole(chp->ID()).Stufe();
				    int max = chp->Definition()->MaximalStufe();
				    int diff = (basis + chp->Stufe()) - max;
				    if (diff>0)
				      if (basis>max)
				        continue;
				      else
				        chp->Stufe(max-basis);
				    if (chp->Stufe()!=0)
				      chartemp2.FuegeEin(chareigen_.Hole(c));
				  } 
				}
				//@nonl
				//@-node:<< if Stufig >>
				//@nl
		  
		    if (chp->Definition()->Hinzufuegbar(*P())) {
		      chartemp2.FuegeEin(chareigen_.Hole(c));
		      P()->charakteristika_.FuegeEin(chareigen_.Hole(c)); // ? nicht sicher
		    }
		  }
		}
		chareigen_ = chartemp2;
		//@nonl
		//@-node:<< Ueberpruefe chareigen_ >>
		//@nl
	  
	  for (c = 0; c != chareigen_.size(); ++c) {
	    Charakteristikum ch = chareigen_.Hole(c);
	    const CharakteristikumDefinition* chd = ch.Definition();
	    
	    if ((chd->HatBasis())&&(chartemp.Existiert(chd->BasiertAuf()))) {
	      if (!((ch.Vorteil())&&(nurNachteile)))
	
	        gp += Hinzu(_t("Doppelter Vor-/Nachteil") + ": " + chd->BasiertAuf(), -chartemp.Zeige(chd->BasiertAuf())->Definition()->GP());
	      chartemp.Loesche(chd->BasiertAuf());
	    }
	    
	    if ((ch.SchlechteEigenschaft())&&(charbasis_.Existiert(ch.ID()))) {
	      chartemp.Zeige(ch.ID())->
	        Stufe(chartemp.Zeige(ch.ID())->Stufe()+ch.Stufe());
	      if (!((ch.Vorteil())&&(nurNachteile)))
	        gp += Hinzu(ch.Text(), ch.GPKosten(*P()));
	      continue;
	    }
	
	    if ((ch.HatStufen())&&(charbasis_.Existiert(ch.ID()))) {
	      chartemp.Zeige(ch.ID())->
	        Stufe(chartemp.Zeige(ch.ID())->Stufe()+ch.Stufe());
	      if (!((ch.Vorteil())&&(nurNachteile)))
	        gp += Hinzu(ch.Text(), ch.GPKosten(*P()));
	      continue;
	    }
	      
	    if (!((ch.Vorteil())&&(nurNachteile)))
	      gp += Hinzu(ch.Text(), ch.GPKosten(*P()));
	    chartemp.FuegeEin(ch);
	  }
	
	  P()->charakteristika_ = charfinal_ = chartemp;
	  
		//@  << Begabung für Talent >>
		//@+node:<< Begabung für Talent >>
		// Begabung für Talent erhöht Talent um 1
		if (talentbegabung_ !="") {
		  if (talente()->Existiert(talentbegabung_)) {
		    Talente tals;
		    Talent t(talente()->Hole(talentbegabung_));
		    t.Wert(1);
		    tals.FuegeEin(t);
		    Entferne(tals);
		    talentbegabung_ = "";
		  } else {// Es war eine Kampftechnik
		    Kampftechniken ks;
		    Kampftechnik k(kampftechniken()->Hole(talentbegabung_));
		    k.Wert(1);
		    ks.FuegeEin(k);
		    Entferne(ks);
		    talentbegabung_ = "";
		  }
		}
		Charakteristika::iterator chi;
		for (chi = P()->charakteristika_.begin(); chi != P()->charakteristika_.end(); ++chi)
		  if (strings::BeginntMit(chi->first, _t("Begabung für"))) {
		    std::string tal = strings::OhneAnfang(chi->first, _t("Begabung für"));
		    if (talente()->Existiert(tal)) {
		      if (talentbegabung_!="") Log::Error("PersonHandlerGenerator::BerechneCharFinal: talentbegabung_ != 0");
		      talentbegabung_ = tal;
		      Talente tals;
		      Talent t(talente()->Hole(tal));
		      t.Wert(1);
		      tals.FuegeEin(t);
		      Setze(tals);
		    }
		    if (kampftechniken()->Existiert(tal)) {
		      if (talentbegabung_!="") Log::Error("PersonHandlerGenerator::BerechneCharFinal: talentbegabung_ != 0");
		      talentbegabung_ = tal;
		      Kampftechniken ks;
		      Kampftechnik k(kampftechniken()->Hole(tal));
		      k.Wert(1);
		      ks.FuegeEin(k);
		      Setze(ks);
		    }
		  }
		//@nonl
		//@-node:<< Begabung für Talent >>
		//@nl
	  if (regeln()->Regeloption("Begabungen und Unfähigkeiten nach Errata")) {
			//@    << Begabung für Talentgruppe >>
			//@+node:<< Begabung für Talentgruppe >>
			// Begabung für Talentgeruppe erhöht alle bereits aktivierten Talente um 1
			if (talentgruppenbegabung_ !="") {
			  Talentgruppe g = TalentgruppeString(talentgruppenbegabung_);
			  if ((g==Koerperlich)||(g==Gesellschaftlich)||(g==Natur)||(g==Wissen)||(g==Handwerk)) {
			    Entferne(begabtetalentegruppe_);
			    talentgruppenbegabung_ = "";
			  } else if (g==Kampf) {
			    Entferne(begabtekampftechnikengruppe_);
			    talentgruppenbegabung_ = "";
			  }
			}
			
			for (chi = P()->charakteristika_.begin(); chi != P()->charakteristika_.end(); ++chi)
			  if (strings::BeginntMit(chi->first, _t("Begabung für"))) {
			    std::string talgruppe = strings::OhneAnfang(chi->first, _t("Begabung für"));
			    Talentgruppe g = TalentgruppeString(talgruppe);
			    if (talentgruppenbegabung_!="") Log::Error("PersonHandlerGenerator::BerechneCharFinal: talentgruppenbegabung_ != 0");
			    if ((g==Koerperlich)||(g==Gesellschaftlich)||(g==Natur)||(g==Wissen)||(g==Handwerk)) {
			      talentgruppenbegabung_ = talgruppe;
			      Talente tals;
			      for (Talente::const_iterator ti = talentebasis_.begin(); ti != talentebasis_.end(); ++ti)
			        if (ti->second.Gruppe()==g) {
			          Talent t(ti->second.Definition());
			          t.Wert(1);
			          tals.FuegeEin(t);
			        }
			      begabtetalentegruppe_ = tals;
			      Setze(begabtetalentegruppe_);
			    } else if (g==Kampf) {
			      talentgruppenbegabung_ = talgruppe;
			      Kampftechniken ks;
			      for (Kampftechniken::const_iterator ki = kampftechnikenbasis_.begin(); ki != kampftechnikenbasis_.end(); ++ki) {
			        Kampftechnik k(ki->second.Definition());
			        k.Wert(1);
			        ks.FuegeEin(k);
			      }
			      begabtekampftechnikengruppe_ = ks;
			      Setze(begabtekampftechnikengruppe_);
			    }
			  }
			//@-node:<< Begabung für Talentgruppe >>
			//@nl
			//@    << Unfähigkeit für Talent >>
			//@+node:<< Unfähigkeit für Talent >>
			// Unfähigkeit für Talent senkt Talent um 1
			if (talentunfaehigkeit_ !="") {
			  if (talente()->Existiert(talentunfaehigkeit_)) {
			    Talente tals;
			    Talent t(talente()->Hole(talentunfaehigkeit_));
			    t.Wert(-1);
			    tals.FuegeEin(t);
			    Entferne(tals);
			    talentunfaehigkeit_ = "";
			  } else {// Es war eine Kampftechnik
			    Kampftechniken ks;
			    Kampftechnik k(kampftechniken()->Hole(talentunfaehigkeit_));
			    k.Wert(-1);
			    ks.FuegeEin(k);
			    Entferne(ks);
			    talentunfaehigkeit_ = "";
			  }
			}
			for (chi = P()->charakteristika_.begin(); chi != P()->charakteristika_.end(); ++chi)
			  if (strings::BeginntMit(chi->first, _t("Unfähigkeit für"))) {
			    std::string tal = strings::OhneAnfang(chi->first, _t("Unfähigkeit für"));
			    if (talente()->Existiert(tal)) {
			      if (talentunfaehigkeit_!="") Log::Error("PersonHandlerGenerator::BerechneCharFinal: talentunfaehigkeit_ != 0");
			      talentunfaehigkeit_ = tal;
			      Talente tals;
			      Talent t(talente()->Hole(tal));
			      t.Wert(-1);
			      tals.FuegeEin(t);
			      Setze(tals);
			    }
			    if (kampftechniken()->Existiert(tal)) {
			      if (talentunfaehigkeit_!="") Log::Error("PersonHandlerGenerator::BerechneCharFinal: talentunfaehigkeit_ != 0");
			      talentunfaehigkeit_ = tal;
			      Kampftechniken ks;
			      Kampftechnik k(kampftechniken()->Hole(tal));
			      k.Wert(-1);
			      ks.FuegeEin(k);
			      Setze(ks);
			    }
			  }
			//@nonl
			//@-node:<< Unfähigkeit für Talent >>
			//@nl
			//@    << Unfähigkeit für Talentgruppe >>
			//@+node:<< Unfähigkeit für Talentgruppe >>
			// Unfähigkeit für Talentgeruppe senkt alle bereits aktivierten Talente um 1
			if (talentgruppenunfaehigkeit_ !="") {
			  Talentgruppe g = TalentgruppeString(talentgruppenunfaehigkeit_);
			  if ((g==Koerperlich)||(g==Gesellschaftlich)||(g==Natur)||(g==Wissen)||(g==Handwerk)) {
			    Entferne(unfaehigetalentegruppe_);
			    talentgruppenunfaehigkeit_ = "";
			  } else if (g==Kampf) {
			    Entferne(unfaehigekampftechnikengruppe_);
			    talentgruppenunfaehigkeit_ = "";
			  }
			}
			
			for (chi = P()->charakteristika_.begin(); chi != P()->charakteristika_.end(); ++chi)
			  if (strings::BeginntMit(chi->first, _t("Unfähigkeit für"))) {
			    std::string talgruppe = strings::OhneAnfang(chi->first, _t("Unfähigkeit für"));
			    Talentgruppe g = TalentgruppeString(talgruppe);
			    if (talentgruppenunfaehigkeit_!="") Log::Error("PersonHandlerGenerator::BerechneCharFinal: talentgruppenunfaehigkeit_ != 0");
			    if ((g==Koerperlich)||(g==Gesellschaftlich)||(g==Natur)||(g==Wissen)||(g==Handwerk)) {
			      talentgruppenunfaehigkeit_ = talgruppe;
			      Talente tals;
			      for (Talente::const_iterator ti = talentebasis_.begin(); ti != talentebasis_.end(); ++ti)
			        if (ti->second.Gruppe()==g) {
			          Talent t(ti->second.Definition());
			          t.Wert(-1);
			          tals.FuegeEin(t);
			        }
			      unfaehigetalentegruppe_ = tals;
			      Setze(unfaehigetalentegruppe_);
			    } else if (g==Kampf) {
			      talentgruppenunfaehigkeit_ = talgruppe;
			      Kampftechniken ks;
			      for (Kampftechniken::const_iterator ki = kampftechnikenbasis_.begin(); ki != kampftechnikenbasis_.end(); ++ki) {
			        Kampftechnik k(ki->second.Definition());
			        k.Wert(-1);
			        ks.FuegeEin(k);
			      }
			      unfaehigekampftechnikengruppe_ = ks;
			      Setze(unfaehigekampftechnikengruppe_);
			    }
			  }
			//@-node:<< Unfähigkeit für Talentgruppe >>
			//@nl
	  }
	  
	  return gp;
	} // BerechneCharFinal
	//@-node:BerechneCharFinal
	//@+node:BerechneSFFinal
	int PersonHandlerGenerator::BerechneSFFinal(const bool regen) {
	  int gp = 0;
	  Sonderfertigkeiten sftemp;
	  
		//@  << Setze SF aus RKP >>
		//@+node:<< Setze SF aus RKP >>
		int i = 0;
		for (i = 0; i != P()->Rasse_().sonderfertigkeiten_.size(); ++i) {
		  sftemp.FuegeEin(P()->Rasse_().sonderfertigkeiten_.Hole(i));
		}
		sfverbilligt_ = P()->Rasse_().verbilligteSonderfertigkeiten_;
		
		for (int a = 0; a != 2; ++a) {
		  Sonderfertigkeiten sfs =
		    (a==0?P()->Kultur_().sonderfertigkeiten_:P()->Profession_().sonderfertigkeiten_);
		  StringListe sfl =
		    (a==0?P()->Kultur_().verbilligteSonderfertigkeiten_:P()->Profession_().verbilligteSonderfertigkeiten_);
		  for (i = 0; i != sfs.size(); ++i) {
		    Sonderfertigkeit sf = sfs.Hole(i);
		    const SonderfertigkeitDefinition* sfd = sf.Definition();
		    if (sfd->HatStufen()) {
		      // Stufen addieren
		      int stufensumme = sfd->Stufe();
		      for (int stufe = 1; stufe <= sfd->MaximalStufe(); ++stufe) {
		        if (sftemp.Existiert(sfd->Stufe(stufe))) {
		          stufensumme += stufe;
		          sftemp.Loesche(sfd->Stufe(stufe));
		        }
		      }
		      if (stufensumme > sfd->MaximalStufe())
		        stufensumme = sfd->MaximalStufe();
			
			Sonderfertigkeit newsf = Sonderfertigkeit(sfd->Stufe(stufensumme));
			if (newsf.Definition()->BrauchtSpezifizierung()) {
			    newsf.Spezifizierung(sf.Spezifizierung());
			}
		      sftemp.FuegeEin(newsf);
		    } else { // nicht (HatStufen())
		      if (sftemp.Existiert(sf.ID())) // gibts schon
		        gp += Hinzu(_t("Doppelte Sonderfertigkeit") + ": " + sfd->ID(), -(sfd->GP()+1)/2); 
		      else { // gibts noch nicht
		        sftemp.FuegeEin(sf);
		      }        
		    } // end if (HatStufen())
		  }
		
		  // Verbilligte SF
		  for (i = 0; i != sfl.size(); ++i)
		    sfverbilligt_.push_back(sfl[i]);
		  sfl.clear();
		  for (i = 0; i != sfverbilligt_.size(); ++i)
		    if (sftemp.Existiert(sfverbilligt_[i]))
		      gp += Hinzu(_t("Doppelte verbilligte Sonderfertigkeit") + ": " + sfverbilligt_[i], -1); // Setze 1 GP frei
		    else
		      sfl.push_back(sfverbilligt_[i]);
		  sfverbilligt_ = sfl;
		} // für Profession und Kultur
		//@nonl
		//@-node:<< Setze SF aus RKP >>
		//@nl
	  
	  P()->sonderfertigkeiten_ = sftemp;
	  P()->verbilligteSonderfertigkeiten_ = sfverbilligt_;
	  
		//@  << Füge für alle SF mit Stufe > 1 die kleineren SF ein >>
		//@+node:<< Füge für alle SF mit Stufe > 1 die kleineren SF ein >>
		for (i = 0; i != P()->sonderfertigkeiten_.size(); ++i) {
		  if (P()->sonderfertigkeiten_.Hole(i).Definition()->Stufe()>1)
		    sftemp.FuegeEin(Sonderfertigkeit(P()->sonderfertigkeiten_.Hole(i).Definition()->Stufe(1)));
		  if (P()->sonderfertigkeiten_.Hole(i).Definition()->Stufe()>2)
		    sftemp.FuegeEin(Sonderfertigkeit(P()->sonderfertigkeiten_.Hole(i).Definition()->Stufe(2)));
		}
		//@nonl
		//@-node:<< Füge für alle SF mit Stufe > 1 die kleineren SF ein >>
		//@nl
	  
	  P()->sonderfertigkeiten_ = sfbasis_ = sftemp;
	  
	  // fertig mit automatischen, nun die eigenen
	
	  // Idee (wg Bug, s. ToDo): solange einfügen, bis der Rest sich
	  // nicht mehr ändert
	  Sonderfertigkeiten sftemp2;
	  Sonderfertigkeiten sfrest = sfeigen_;
	  while (sfrest != sftemp2) {
	    sfrest = sftemp2;
	    for (i = 0; i != sfeigen_.size(); ++i) {
	
	      std::string sfid = sfeigen_.Zeige(i)->ID();
	      
				//@      << Ausnahme für RK >>
				//@+node:<< Ausnahme für RK >>
				if (strings::BeginntMit(sfid, _t("Ritualkenntnis")))
				  if (sfbasis_.Existiert(sfid)) {
				    int basiswert = sfbasis_.Zeige(sfid)->Stufe();
				    int eigenwert = sfeigen_.Zeige(i)->Stufe();
				    int maxwert = sfbasis_.Zeige(sfid)->MaximaleStufe(*P());
				    if (regen) {
				        eigenwert -= basiswert;
				        if (eigenwert < 0) eigenwert = 0;
				        sfeigen_.Zeige(i)->Stufe(eigenwert);
				      }
				    else
				      if ((basiswert+eigenwert) > maxwert) {
				        eigenwert = maxwert-basiswert;
				        if (eigenwert < 0) eigenwert = 0;
				        sfeigen_.Zeige(i)->Stufe(eigenwert);
				      }
				    sftemp2.FuegeEin(sfeigen_.Hole(i));
				    sftemp.Zeige(sfid)->Stufe(basiswert + eigenwert);
				    P()->sonderfertigkeiten_.Zeige(sfid)->Stufe(basiswert + eigenwert);
				  }
				//@nonl
				//@-node:<< Ausnahme für RK >>
				//@nl
	      
	      if (!sfbasis_.Existiert(sfeigen_.Zeige(i)->ID()))
	        if (sfeigen_.Zeige(i)->Definition()->Hinzufuegbar(*P())) {
	          sftemp2.FuegeEin(sfeigen_.Hole(i));
	          P()->sonderfertigkeiten_.FuegeEin(sfeigen_.Hole(i));
	        }
	      }
	  }
	  sfeigen_ = sftemp2;
	  
		//@  << Merkmale berechnen >>
		//@+node:<< Merkmale berechnen >>
		//@+at
		// Um diese kompliziert aussehende Berechnung zu 
		// verstehen, schaut zunächst auf die entsprechende 
		// Regel in Aventurische Zauberer, S. 158.
		//@-at
		//@@c
		
		int anzahlVerrechneteMerkmale = 0;
		Sonderfertigkeiten::const_iterator sfm;
		for (sfm = sfbasis_.begin(); sfm != sfbasis_.end(); ++sfm)
		  if (sfm->second.Definition()->IstMerkmalskenntnis())
		    ++anzahlVerrechneteMerkmale;
		
		Sonderfertigkeiten eigeneMerkmale;
		for (sfm = sfeigen_.begin(); sfm != sfeigen_.end(); ++sfm)
		  if (sfm->second.Definition()->IstMerkmalskenntnis())
		    eigeneMerkmale.FuegeEin(sfm->second);
		
		if (eigeneMerkmale.size()>0) {
		  while ((anzahlVerrechneteMerkmale < 3) && (eigeneMerkmale.size() > 0)) {
		    int faktor = 1; // TODO Konstante
		    if (anzahlVerrechneteMerkmale == 1)
		      faktor = 2; // TODO Konstante
		    if (anzahlVerrechneteMerkmale == 2)
		      faktor = 5; // TODO Konstante
		
		    // teuerste Merkmale zuerst
		    Sonderfertigkeit teuerstesMerkmal = eigeneMerkmale.begin()->second;
		    for (sfm = eigeneMerkmale.begin(); sfm != eigeneMerkmale.end(); ++sfm)
		      if (sfm->second.APKosten(*P()) > teuerstesMerkmal.APKosten(*P()))
		        teuerstesMerkmal = sfm->second;
		        
		    gp += Hinzu(teuerstesMerkmal.Text(), ((teuerstesMerkmal.APKosten(*P())*faktor)+25)/50);
		    sftemp.FuegeEin(teuerstesMerkmal);
		    eigeneMerkmale.Loesche(teuerstesMerkmal.ID());
		    ++anzahlVerrechneteMerkmale;
		  }
		  
		  // alle restlichen mit Faktor 8 hinzufuegen
		
		  for (sfm = eigeneMerkmale.begin(); sfm != eigeneMerkmale.end(); ++sfm) {
		    gp += Hinzu(sfm->second.Text(), ((sfm->second.APKosten(*P())*8)+25)/50); // TODO Konstante
		    sftemp.FuegeEin(sfm->second);
		  }
		
		}
		    
		//@nonl
		//@-node:<< Merkmale berechnen >>
		//@nl
	  
	  for (i = 0; i != sfeigen_.size(); ++i) 
	      if  (!sftemp.Existiert(sfeigen_.Zeige(i)->ID())){   
	        Sonderfertigkeit sf = sfeigen_.Hole(i);
	        //const SonderfertigkeitDefinition* sfd = sf.Definition();
	        gp += Hinzu(sf.Text(), sf.GPKosten(*P()));
	        sftemp.FuegeEin(sf);
	      }
	      
	  P()->sonderfertigkeiten_ = sffinal_ = sftemp;
	  return gp;
	} // BerechneSFFinal
	//@-node:BerechneSFFinal
	//@+node:GP
	int PersonHandlerGenerator::GP() {
	  gptext_ = "";
	  return 
	      GPGuteEigenschaften(true)
	    + Hinzu(_t("Sozialstatus"), SOStartwert().Wert()) // SO-Startwert
	    + Hinzu(_t("Rasse"), P()->Rasse_().GP())
	    + Hinzu(_t("Kultur"), P()->Kultur_().GP())
	    + Hinzu(_t("Profession"), P()->Profession_().GP())
	    + BerechneCharFinal()
	    + BerechneSFFinal()
	    ;
	} // GP
	//@nonl
	//@-node:GP
	//@+node:Hinzu
	int PersonHandlerGenerator::Hinzu(const std::string& text, int gp) {
	  gptext_ += text + " (" + strings::int2sStr(gp, true) + " GP)\n";
	  return gp;
	}
	//@nonl
	//@-node:Hinzu
	//@+node:GPGuteEigenschaften
	int PersonHandlerGenerator::GPGuteEigenschaften(bool schreiben) {
	  int gp = 0;
	  for (int i=0; i!=AnzahlEigenschaften; ++i)
	    if (schreiben)
	        gp += Hinzu(P()->eigenschaft_[i].Text(), P()->eigenschaft_[i].Basiswert());
	    else
	        gp += P()->eigenschaft_[i].Basiswert();
	  return gp;
	} // GPGuteEigenschaften
	//@nonl
	//@-node:GPGuteEigenschaften
	//@+node:GPSchlechteEigenschaften
	int PersonHandlerGenerator::GPSchlechteEigenschaften() {
	  int gp = 0;
	  for (Charakteristika::iterator ch = chareigen_.begin(); ch != chareigen_.end(); ++ch)
	    if (ch->second.SchlechteEigenschaft())
	      gp += ch->second.GPKosten(*P());
	  return -gp;
	} // GPSchlechteEigenschaften
	//@nonl
	//@-node:GPSchlechteEigenschaften
	//@+node:GPNachteile
	int PersonHandlerGenerator::GPNachteile() {
	  return -BerechneCharFinal(true);
	} // GPNachteile
	//@nonl
	//@-node:GPNachteile
	//@+node:AktivierteTalente
	int PersonHandlerGenerator::AktivierteTalente() {
	  int anzahl = 0;
	  for (Talente::iterator t = P()->talente_.begin(); t != P()->talente_.end(); ++t)
	    if (!talentebasis_.Existiert(t->first))
	      ++anzahl;
	  for (Kampftechniken::iterator k = P()->kampftechniken_.begin(); k != P()->kampftechniken_.end(); ++k)
	    if (!kampftechnikenbasis_.Existiert(k->first))
	      ++anzahl;
	  for (Sprachen::iterator sp = P()->sprachen_.begin(); sp != P()->sprachen_.end(); ++sp)
	    if (!sprachenbasis_.Existiert(sp->first))
	      ++anzahl;
	  for (Schriften::iterator sch = P()->schriften_.begin(); sch != P()->schriften_.end(); ++sch)
	    if (!schriftenbasis_.Existiert(sch->first))
	      ++anzahl;
	  return anzahl;
	} //AktivierteTalente
	//@nonl
	//@-node:AktivierteTalente
	//@+node:Aktivierte Zauber
	int PersonHandlerGenerator::AktivierteZauber() {
	  int anzahl = 0;
	  for (Zauberfertigkeiten::iterator z = P()->zauberfertigkeiten_.begin(); z != P()->zauberfertigkeiten_.end(); ++z)
	    if (!zauberbasis_.Existiert(z->first))
	      ++anzahl;
	  return anzahl;
	} //AktivierteZauber
	//@nonl
	//@-node:Aktivierte Zauber
	//@+node:Aktivierte Zauber Max
	int PersonHandlerGenerator::AktivierteZauberMax() {
	  if (P()->charakteristika_.Existiert(_t("Vollzauberer")))
	    return 10; // TODO Konstante
	  if (P()->charakteristika_.Existiert(_t("Halbzauberer")))
	    return 5; // TODO Konstante
	  return 0;
	}
	//@nonl
	//@-node:Aktivierte Zauber Max
	//@+node:TalentGP
	int PersonHandlerGenerator::TalentGP() {
	  int tgp = 0;
	    
	  for (Kampftechniken::iterator k = P()->kampftechniken_.begin(); k != P()->kampftechniken_.end(); ++k) {
	      bool aka = (P()->charakteristika_.Existiert(_t("Akademische Ausbildung (Krieger)")));
	
	    int maxaka = 10;
	    if (P()->charakteristika_.Existiert(_t("Veteran")))
	      maxaka = 15;
	
	    int basiswert = 0;
	    if (!kampftechnikenbasis_.Existiert(k->first))
	      tgp += regeln()->Aktivierungsfaktor(k->second.SKTSpalte_());
	    else
	      basiswert = kampftechnikenbasis_.Zeige(k->first)->Wert();
	    int zielwert = P()->kampftechniken_.Zeige(k->first)->Wert();
	    // "Begabung für ..." ist nicht kumulativ zu Akademische Ausbildung
	      if (P()->charakteristika_.Existiert(_t("Begabung für") + " " + k->first) ||
	         (P()->charakteristika_.Existiert(_t("Begabung für") + " " + TalentgruppeString(Kampf, false))))
	            aka = false;
	
	    
	    while (zielwert > basiswert)
	      if((basiswert<maxaka)&&(aka))
	      tgp += ceil(regeln()->SKT(k->second.SKTSpalte_(), ++basiswert)*0.5);
	      else
	      tgp += regeln()->SKT(k->second.SKTSpalte_(), ++basiswert);
	  }
	    
	  for (Talente::iterator t = P()->talente_.begin(); t != P()->talente_.end(); ++t) {
	    bool aka = ( (t->second.Definition()->Gruppe()==Wissen) &&
	                   (P()->charakteristika_.Existiert(_t("Akademische Ausbildung (Magier)")) || 
	                    P()->charakteristika_.Existiert(_t("Akademische Ausbildung (Gelehrter)"))));
	        
	    int basiswert = 0;
	    if (!talentebasis_.Existiert(t->first))
	      tgp += regeln()->Aktivierungsfaktor(t->second.SKTSpalte_());
	    else
	      basiswert = talentebasis_.Zeige(t->first)->Wert();
	    int zielwert = P()->talente_.Zeige(t->first)->Wert();
	    // "Begabung für ..." ist nicht kumulativ zu Akademische Ausbildung
	      if (P()->charakteristika_.Existiert(_t("Begabung für") + " " + t->first) ||
	       (P()->charakteristika_.Existiert(_t("Begabung für") + " " + TalentgruppeString(t->second.Gruppe(), false))))
	         aka = false;
	          
	          int maxaka = 10;
	          if ( P()->charakteristika_.Existiert(_t("Akademische Ausbildung (Gelehrter)")) &&
	               P()->charakteristika_.Existiert(_t("Veteran")))
	            maxaka = 15;
	
	    while (zielwert > basiswert)
	      if ((basiswert<maxaka)&&(aka))
	        tgp += ceil(regeln()->SKT(t->second.SKTSpalte_(), ++basiswert)*0.5);
	      else
	        tgp += regeln()->SKT(t->second.SKTSpalte_(), ++basiswert);
	  }
	    
	  for (Sprachen::iterator sp = P()->sprachen_.begin(); sp != P()->sprachen_.end(); ++sp) {
	    bool aka = ( P()->charakteristika_.Existiert(_t("Akademische Ausbildung (Magier)")) ||
	                   P()->charakteristika_.Existiert(_t("Akademische Ausbildung (Gelehrter)")));
	
	    int basiswert = 0;
	    if (!sprachenbasis_.Existiert(sp->first))
	      tgp += AktivierungsKosten(sp->second.ID(true));
	    else
	      basiswert = sprachenbasis_.Zeige(sp->first)->Wert();
	    int zielwert = P()->sprachen_.Zeige(sp->first)->Wert();
	    
	    // "Begabung für ..." ist nicht kumulativ zu Akademische Ausbildung
	      if (P()->charakteristika_.Existiert(_t("Begabung für") + " " + sp->first) ||
	         (P()->charakteristika_.Existiert(_t("Begabung für") + " " + TalentgruppeString(SprachenUndSchriften, false))))
	            aka = false;
	          
	          int maxaka = 10;
	          if ( P()->charakteristika_.Existiert(_t("Akademische Ausbildung (Gelehrter)")) &&
	               P()->charakteristika_.Existiert(_t("Veteran")))
	            maxaka = 15;
	
	    while (zielwert > basiswert)
	      if ((basiswert<maxaka)&&(aka))
	        tgp += ceil(regeln()->SKT(sp->second.SKTSpalte_(), ++basiswert)*0.5);
	      else
	        tgp += regeln()->SKT(sp->second.SKTSpalte_(), ++basiswert);
	  }
	    
	  for (Schriften::iterator sch = P()->schriften_.begin(); sch != P()->schriften_.end(); ++sch) {
	    bool aka = P()->charakteristika_.Existiert(_t("Akademische Ausbildung (Magier)"));
	
	    int basiswert = 0;
	    if (!schriftenbasis_.Existiert(sch->first))
	      tgp += AktivierungsKosten(sch->second.ID(true));
	    else
	      basiswert = schriftenbasis_.Zeige(sch->first)->Wert();
	    int zielwert = P()->schriften_.Zeige(sch->first)->Wert();
	
	    // "Begabung für ..." ist nicht kumulativ zu Akademische Ausbildung
	      if (P()->charakteristika_.Existiert(_t("Begabung für") + " " + sch->first) ||
	         (P()->charakteristika_.Existiert(_t("Begabung für") + " " + TalentgruppeString(SprachenUndSchriften, false))))
	            aka = false;
	
	    while (zielwert > basiswert)
	      if ((basiswert<10)&&(aka))
	        tgp += ceil(regeln()->SKT(sch->second.SKTSpalte_(), ++basiswert)*0.5);
	      else
	        tgp += regeln()->SKT(sch->second.SKTSpalte_(), ++basiswert);
	  }
	    
	  for (Zauberfertigkeiten::iterator z = P()->zauberfertigkeiten_.begin(); z != P()->zauberfertigkeiten_.end(); ++z) {
	    int basiswert = 0;
	    if (!zauberbasis_.Existiert(z->first))
	      tgp += AktivierungsKosten(z->first);
	    else
	      basiswert = zauberbasis_.Zeige(z->first)->Wert();
	    int zielwert = P()->zauberfertigkeiten_.Zeige(z->first)->Wert();
	    while (zielwert > basiswert)
	      tgp += regeln()->SKT(z->second.Lernschwierigkeit(), ++basiswert);
	  }
	  
	  Sonderfertigkeiten::iterator sf;
	  // Berechne Ritualkenntnis-TGP
	  for (sf = sfeigen_.begin(); sf != sfeigen_.end(); ++sf)
	    if (strings::BeginntMit(sf->first, _t("Ritualkenntnis"))) {
	      int basiswert = 3; // TODO Magische Konstante, Startwert Ritualkenntnis
	      int zielwert = 0;
	      if (sfbasis_.Existiert(sf->first)) {
	        basiswert = sfbasis_.Zeige(sf->first)->Stufe();
	        zielwert = sf->second.Stufe()+basiswert;
	      } else {
	        basiswert = 3;
	        zielwert = sf->second.Stufe();
	      }
	      while (zielwert > basiswert)
	        tgp += regeln()->SKT(sf->second.SKTSpalte_(*P()), ++basiswert);
	  }
	  
	  // Berechne TGP für waffenlose Sonderfertigkeiten
	  for (sf = sfeigen_.begin(); sf != sfeigen_.end(); ++sf)
	    if (sf->second.Definition()->IstWaffenloseSonderfertigkeit())
	      tgp += sf->second.APKosten(*P());
	  
	  return tgp;
	} // TalentGP
	
	
	
	//@-node:TalentGP
	//@+node:TalentGPMax
	int PersonHandlerGenerator::TalentGPMax() {
	  int tgp = (P()->Wert(Klugheit)+P()->Wert(Intuition))*20; // TODO Konstante
	  if (P()->charakteristika_.Existiert(_t("Gebildet")))
	    tgp += P()->charakteristika_.Hole(_t("Gebildet")).Stufe()*30; // TODO Konstante
	  if (P()->charakteristika_.Existiert(_t("Ungebildet")))
	    tgp -= P()->charakteristika_.Hole(_t("Ungebildet")).Stufe()*30; // TODO Konstante
	  return tgp;
	} // TalentGPMax
	//@nonl
	//@-node:TalentGPMax
	//@+node:GenerierungBeendbar
	bool PersonHandlerGenerator::GenerierungBeendbar() {
	  if (GP()>GPMax()) return false;
	  if (GPGuteEigenschaften()>GPGuteEigenschaftenMax()) return false;
	  if (GPSchlechteEigenschaften()>GPSchlechteEigenschaftenMax()) return false;
	  if (GPNachteile()>GPNachteileMax()) return false;
	  if (AktivierteTalente()>AktivierteTalenteMax()) return false;
	  if (TalentGP()>TalentGPMax()) return false;
	  if (personen()->Existiert(P()->ID())) return false;
	  if (P()->ID()=="") return false;
	  if (P()->Rasse_().ID()=="") return false;
	  if (P()->Kultur_().ID()=="") return false;
	  if (P()->Profession_().ID()=="") return false;
	  return true;
	} // GenerierungBeendbar
	//@nonl
	//@-node:GenerierungBeendbar
	//@+node:GenerierungBedungslosBeendbar
	bool PersonHandlerGenerator::GenerierungBedingungslosBeendbar() {
	  if (GP()<GPMax()) return false;
	  if (TalentGP()<TalentGPMax()) return false;
	  return true;
	} // GenerierungBedingungslosBeendbar
	//@nonl
	//@-node:GenerierungBedungslosBeendbar
	//@+node:GenerierungBeendbarText
	std::string PersonHandlerGenerator::GenerierungBeendbarText() {
	  if (P()->Rasse_().ID()=="")
	    return _t("Entscheiden Sie Sich bitte für eine Rasse.");
	  if (P()->Kultur_().ID()=="")
	    return _t("Entscheiden Sie Sich bitte für eine Kultur.");
	  if (P()->Profession_().ID()=="")
	    return _t("Entscheiden Sie Sich bitte für eine Profession.");
	  if (GP()>GPMax())
	    return strings::int2sStr(GP()-GPMax()) + " " + _t("GP zuviel verbraucht.");
	  if (GPGuteEigenschaften()>GPGuteEigenschaftenMax())
	    return 
	    strings::int2sStr(GPGuteEigenschaften()-GPGuteEigenschaftenMax())
	    + " " + _t("Punkte zu viel auf die Guten Eigenschaften verteilt.");
	  if (GPNachteile()>GPNachteileMax())
	    return 
	    strings::int2sStr(GPNachteile()-GPNachteileMax())
	    + " " + _t("GP zuviel durch Nachteile gewonnen.");
	  if (GPSchlechteEigenschaften()>GPSchlechteEigenschaftenMax())
	    return 
	    strings::int2sStr(GPSchlechteEigenschaften()-GPSchlechteEigenschaftenMax())
	    + " " + _t("GP zuviel durch Schlechte Eigenschaften gewonnen.");
	  if (personen()->Existiert(P()->ID()))
	    return _t("Die gewählte ID existiert bereits") + ": " + P()->ID() + ".";
	  if (P()->ID()=="")
	    return _t("Noch keine ID gewählt.");
	  if (AktivierteTalente()>AktivierteTalenteMax())
	    return strings::int2sStr(AktivierteTalente()-AktivierteTalenteMax()) + " " + _t("Talente zuviel aktiviert.");
	  if (TalentGP()>TalentGPMax())
	    return strings::int2sStr(TalentGP()-TalentGPMax()) + " " + _t("Talente-GP zuviel verbraucht.");
	  if (GP()<GPMax())
	    return strings::int2sStr(GPMax()-GP()) + " " + _t("GP sind noch übrig.");
	  if (TalentGP()<TalentGPMax())
	    return strings::int2sStr(TalentGPMax()-TalentGP()) + " " + _t("Talente-GP übrig.");
	
	  return _t("Alles OK. Die Generierung kann beendet werden.");
	}
	//@nonl
	//@-node:GenerierungBeendbarText
	//@+node:GenerierungBedingungslosBeendbarText
	std::string PersonHandlerGenerator::GenerierungBedingungslosBeendbarText() {
	  if (GP()<GPMax())
	    return strings::int2sStr(GPMax()-GP()) + " " + _t("GP sind noch übrig.") + " " + _t("Sie werden bei Beenden der Generierung ersatzlos verfallen.");
	  if (TalentGP()<TalentGPMax())
	    return strings::int2sStr(TalentGPMax()-TalentGP()) + " " + _t("Talente-GP übrig.") + " " + _t("Sie werden bei Beenden der Generierung ersatzlos verfallen.");
	  return "";
	} // GenerierungBedingungslosBeendbar
	
	//@-node:GenerierungBedingungslosBeendbarText
	//@-others
	//@-node:class PersonHandlerGenerator
	//@-others
  
}

//@<< Version History >>
//@+node:<< Version History >>
// $Log: PersonHandlerGenerator.cpp,v $
// Revision 1.56  2004/10/17 19:12:39  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.55  2004/06/24 00:27:09  twel
// Akademische Ausbildung (Gelehrter) implementiert
//
// Revision 1.54  2004/05/27 00:27:21  twel
// Versuch letzten Commit aufzuheben
//
// Revision 1.52  2004/04/25 17:30:39  gnaddelwarz
// - "Akademische Ausbildung (Krieger)" hinzugefügt
// - "Begabung für [Talent]" ist nicht kumulativ zu einer Akademischen Ausbildung
// - Beim hinzufügen von automatischen SF mit (vorgegebener) Spezifizierung und Stufen
//     (Rüstungsgewöhnung) über Kultur oder Profession wurde die Spezifizierung nicht
//     übernommen
//
// Revision 1.51  2004/04/13 21:45:45  vrandezo
// Akademische Ausbildung (Magier) implementiert
// Aufräumarbeiten (besser in Leo integriert)
//
// Revision 1.50  2004/04/13 14:06:52  vrandezo
// Hauszauber implementiert
//
// Revision 1.49  2004/04/12 21:08:09  vrandezo
// Lehrsprachen implementiert
// Mutter-/Zweitsprachensystem umgebaut
//
// Revision 1.48  2004/04/12 13:20:23  vrandezo
// Merkmalskenntnisse implementiert
//
// Revision 1.47  2004/04/11 23:02:44  vrandezo
// - maechtig das Programm beschleunigt (Faktor wasweissich!)
// - Bug entfernt, der verhinderte, dass manche SF
// (mit Voraussetzung RK > bestimmter Wert) Personen
// hinzugefuegt werden konnten
//
// Revision 1.46  2004/04/11 01:33:31  vrandezo
// selbsthinzugefuegte RK richtig steiger- und senkbar
//
// Revision 1.45  2004/04/10 20:22:41  vrandezo
// Schlechte Eigenschaften bei Re-Gen korrigiert
// Stufige VNT von RKP werden korrekt eingebunden
//
// Revision 1.44  2004/04/10 01:26:08  vrandezo
// Helden erhalten ZF von RKP
//
// Revision 1.43  2004/04/09 22:25:50  vrandezo
// Zauberfertigkeiten fuer Personen eintragbar
//
// Revision 1.42  2004/04/06 21:24:30  vrandezo
// GP werden bei Heldenerschaffung aufgeschlüsselt (Twelwan)
//
// Revision 1.41  2004/04/06 18:48:56  vrandezo
// Bug: altes for-scoping korrigiert
// Vorbereitung für GP-Aufschluesselung
//
// Revision 1.40  2004/04/06 13:57:58  vrandezo
// Begabung und Unfaehigkeit von Talenten und Talentgruppen jetzt
// nach Errata implementiert (Twel)
//
// Revision 1.39  2004/04/04 23:37:40  vrandezo
// Bug: beim Re-Generieren war der RK zu hoch
//
// Revision 1.38  2004/04/04 20:21:16  vrandezo
// Bug: bei der Generierung von IDs aus Namen wurden Umlaute ignoriert
//
// Revision 1.37  2004/04/04 17:29:27  vrandezo
// Re-Generierungs-bug gefixt
//
// Revision 1.36  2004/01/11 20:41:26  twel
// Errata (Talentmaximum) und Code teilweise auf Hilfsfunktionen umgestellt
//
// Revision 1.35  2004/01/08 01:57:56  twel
// - Z&H Errata zu Begabung/Unfaehigkeit eingearbeitet bis auf Startboni/-mali
// - Verdoppelungsregel bringt 1/3 der GP statt 1/2 (auch Errata)
//
// Revision 1.34  2004/01/01 18:57:31  vrandezo
// Schlechte Eigenschaften werden bei Re-Generierung nicht verdoppelt
//
// Revision 1.33  2003/12/31 03:51:58  vrandezo
// Re-Generation erweitert
//
// Revision 1.32  2003/12/06 15:02:29  vrandezo
// Twelwan: Regelbug; Max für Kampftalente ist Max(GE, KK)+3, nicht Max(MU, KK)+3
//
// Revision 1.31  2003/11/28 16:46:16  vrandezo
// Zeilenvorlauf am Dateiende (Warnung bei gcc)
//
// Revision 1.30  2003/11/20 00:35:07  vrandezo
// compilierbarkeit mit gcc verbessert
//
// Revision 1.29  2003/10/30 00:02:44  vrandezo
// Steigerbare Ritualkenntnisse
//
// Revision 1.28  2003/10/26 23:16:08  vrandezo
// RK hinzufügbar zu RKP und steigerbar
//
// Revision 1.27  2003/10/25 22:33:27  vrandezo
// SF-Text
//
// Revision 1.26  2003/10/25 16:45:38  vrandezo
// um Hole erweitert
//
// Revision 1.25  2003/10/25 15:06:48  vrandezo
// Leonizing
// Charakteristika mit Spezifizierung
//
// Revision 1.24  2003/10/25 02:11:17  vrandezo
// generierte Sonderfertigkeiten
//
// Revision 1.23  2003/10/24 23:50:41  vrandezo
// Leonizing
// Vor-/Nachteile als Voraussetzung ermöglicht
// Astralmacht, ZVH-Zauberer
//
// Revision 1.22  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.21  2003/03/27 14:28:42  vrandezo
// Personen haben eigene Start-GP
//
// Revision 1.20  2003/03/25 18:42:36  vrandezo
// Kampftechniken in der Kompaktübersicht mit AT/PA statt +AT/+PA
// Bug: Sonderfertigkeiten unabhängig von alphabetischer Reihenfolge
// Sonderfertigkeiten: Binden, Entwaffnen, Meisterliches Entwaffnen, Waffe zerbrechen
//
// Revision 1.19  2003/03/25 16:24:15  vrandezo
// Unfähigkeit für [Talentgruppe] und [Talent]
//
// Revision 1.18  2003/03/25 13:01:19  vrandezo
// Begabung für [Talent] jetzt auch für Kampffertigkeiten
//
// Revision 1.17  2003/03/25 12:41:48  vrandezo
// Begabung für [Talent]
//
// Revision 1.16  2003/03/25 10:28:57  vrandezo
// Bug: Wenn eine Kultur Modifikatoren auf Klugheit hat, stürzte das Programm
// ab, weil es versuchte, den Wert für die noch nicht verhandene Muttersprache
// zu verändern (Anonyme Bugmeldung)
//
// Revision 1.15  2003/03/25 09:40:17  vrandezo
// Begabung für [Talentgruppe]
//
// Revision 1.14  2003/03/24 20:22:56  vrandezo
// Layoutänderungen (viele Anregungen von Tim, Achim, Wolfgang, King Lui, Twelwan...)
// Neu: Miserable Eigenschaften
//
// Revision 1.13  2003/03/24 18:57:16  vrandezo
// Neue Vorteile: Herausragende Eigenschaften (Forumswunsch Sebastian)
//
// Revision 1.12  2003/03/09 15:26:30  vrandezo
// Waffenlose Kampfstile
// Aufgeteilte Editoren für Waffenlose Kampfstile und sonstige SF
// Voraussetzungen können auch auf Kampftechniken sein
//
// Revision 1.11  2003/03/08 00:09:31  vrandezo
// Drucken der Talentseite möglich
//
// Revision 1.10  2003/03/07 15:06:43  vrandezo
// AT/PA-Werte bei Kampftechniken (Heldenbrief Gerhard Mayer)
//
// Revision 1.9  2003/02/24 22:03:34  vrandezo
// Feature Request: beim Auswählen der Charakteristika werden die GP mit
// angegeben (King Lui)
//
// Revision 1.8  2003/02/23 12:36:27  vrandezo
// Charakteristika-Reiter umbenannt zu Vor-/Nachteile & SF (Tim von Umbscheiden)
// Generierung abschließbar, ohne alle TGP und GP verbraucht zu haben (Stefan Holzmüller)
// das Schließen des Fensters bei der Auswahl von Talenten bei der Vergabe einer
//   Profession, Rasse oder Kultur während der Heldengenerierung führt nicht mehr
//   zum Absturz (Stefan Holzmüller)
// Neuer Vorteil: Fettleibig (Volker Strunk)
//
// Revision 1.7  2003/02/20 17:48:30  vrandezo
// Alter des Helden
// VN mit Voraussetzungen eintragbar
// Abgeleitete Wert als Voraussetzungen möglich
//
// Revision 1.6  2003/02/20 13:53:22  vrandezo
// Vorteile: Beidhändig, Linkshänder, Blutrausch, Kampfrausch
// weitere SF
// Eigenschaften haben Maximalwerte
//
// Revision 1.5  2003/02/20 00:21:58  vrandezo
// Sonderfertigkeiten vollständig integriert
// schlafende SF
//
// Revision 1.4  2003/02/19 17:45:10  vrandezo
// Kulturen zeigen mögliche und übliche Rassen an,
// Professionen mögliche Kulturen
//
// Revision 1.3  2003/02/18 18:48:48  vrandezo
// SF bei Generierung
// Geländekunden außer der ersten für nur 2/3 des Preises
//
// Revision 1.2  2003/02/18 17:42:22  vrandezo
// verbesserte GUI für SF und für Charakteristika in Generierung
//
// Revision 1.1.1.1  2003/02/17 15:33:56  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.31  2003/02/17 12:36:16  vrandezo
// GU für Sprachen und Schriften bei der Heldengenerierung hinzugefügt
//
// Revision 1.30  2003/02/14 20:41:00  vrandezo
// Schlechte Eigenschaften
//
// Revision 1.29  2003/02/14 17:14:26  vrandezo
// einzelne VN: Hohe und Niedrige Lebenskraft und Magieresistenz
// Export: Button, um alle auf einmal zu exportieren
// Geweihte Profession möglich (kann sich durch G&D wieder ändern)
//
// Revision 1.28  2003/02/14 02:01:20  vrandezo
// Log::Error führt nicht mehr zu Extrafenster in Release-Version
// steigerbare VN mit Wert und Maximalwert
// einzelne VN: Gebildet, Ungebildet, Ausdauernd, Kurzatmig
//
// Revision 1.27  2003/02/13 22:29:30  vrandezo
// Auf Xerces 2.2.0 geupdatet
//
// Revision 1.26  2003/02/13 20:13:06  vrandezo
// gcc-Portabilität verbessert
//
// Revision 1.25  2003/02/13 13:34:36  vrandezo
// steigerbarer VN ohne Wert in Generierung
//
// Revision 1.24  2003/02/08 00:22:54  vrandezo
// VN von RKP bei Generierung übernommen
// mehrfache VN bei RKP geben GP
// erweitere Voraussetzungen: nicht VN
// entfernbare VN
// auswählbare VN
// VN mit Voraussetzungen (nicht VN)
// Geschlecht neuerschaffener Helden ist zufällig
//
// Revision 1.23  2003/02/07 19:37:25  vrandezo
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
// Revision 1.22  2003/01/26 16:49:04  vrandezo
// AT/PA jetzt bei Kampftechniken aufteilbar
//
// Revision 1.21  2003/01/26 13:42:22  vrandezo
// Voraussetzungen bei Generierung gültig
// Auswahl aus einer Talentgruppe möglich bei Generierung
// Geschlecht bei Generierung implementiert
//
// Revision 1.20  2003/01/24 20:33:34  vrandezo
// übliche Kulturen auswählbar bei Heldenerschaffung
// Wert der Mutter- und Zweitsprache auch bei Modifikatoren korrekt
// Geschlecht wechselbar
//
// Revision 1.19  2003/01/19 01:05:22  vrandezo
// Mutter- und Zweitsprachen bei Heldenerschaffung vollständig integriert
// Werte passen sich ständig der aktuellen Klugheit an
// Sprachen und Schriften in Heldenerschaffung integriert
// Bei möglicher Talentaktivierung werden nur passende Talente angezeigt
//
// Revision 1.18  2003/01/18 18:10:42  vrandezo
// Personen können Sprachen und Schriften
// übliche Kulturen bei Rassen hinzugefügt
// GUI für mögliche und übliche Kulturen und für mögliche Professionen geändert
// Varianten ausblendbar bei der Eingabe möglicher Kulturen und Professionen
//
// Revision 1.17  2002/12/06 15:28:01  vrandezo
// Spezies bei den Rassen einstellbar
// Kampftechniken bei PK und bei Person hinzugefügt
//
// Revision 1.16  2002/11/03 19:48:00  vrandezo
// Größe, Gewicht, Alter eingefügt
//
// Revision 1.15  2002/11/03 17:48:36  vrandezo
// Haarfarbe und Augenfarbe jetzt auch bei der Heldenerschaffung
//
// Revision 1.14  2002/11/03 14:25:33  vrandezo
// Wuerfel angepasst
// Haar- und Augenfarbe hinzugefügt bei Rassen
//
// Revision 1.13  2002/11/01 15:27:29  vrandezo
// Regeln neu implementiert
// ILader, MLader, IMLader, ILadbar und weitere Klassen entfernt
// XMLChar-Transcoding beschleunigt
//
// Revision 1.12  2002/10/20 00:20:47  vrandezo
// Mögliche Kulturen und Professionen implementiert
//
// Revision 1.11  2002/10/19 20:35:32  vrandezo
// feste Stringliterale entfernt
//
// Revision 1.10  2002/10/19 18:58:33  vrandezo
// Texte in eigene, externe Datei untergebracht
//
// Revision 1.9  2002/10/19 15:29:57  vrandezo
// Personenkomponenten um die Talentmodifikationen erweitert
//
// Revision 1.8  2002/10/18 00:50:16  vrandezo
// Talente bei der Heldenerschaffung einsetzbar
//
// Revision 1.7  2002/10/13 15:18:42  vrandezo
// Voraussetzungen für Professionen implementiert
// CVS aufgeräumt für Modifkatoren und Voraussetzungen
//
// Revision 1.6  2002/10/12 11:46:47  vrandezo
// Rasseneditor eingeführt
//
// Revision 1.5  2002/10/11 14:13:22  vrandezo
// Modifikatoren für Eigenschaften und Abgeleitete Werte eingeführt
//
// Revision 1.4  2002/10/10 21:41:38  vrandezo
// PersonenKomponente als Grundlage für Rasse, Kultur und Profession eingeführt
// Toten Code aus dem Projekt entfernt (noch nicht aus dem CVS)
//
// Revision 1.3  2002/10/09 19:58:34  vrandezo
// Auswahl von Rasse, Kultur, Profession ermöglicht
//
// Revision 1.2  2002/09/28 15:59:11  vrandezo
// Modifikatoren in GUI eingebunden
//
// Revision 1.1  2002/09/28 00:28:37  vrandezo
// Beginn des Generators mit DSA.lib und in wxWindows
//
//@nonl
//@-node:<< Version History >>
//@nl
//@-node:@file PersonHandlerGenerator.cpp
//@-leo
