//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file Person.cpp
//@+at
// Person.cpp: Implementierung der Klasse Person.
// 
// $Revision: 1.24 $
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
#include "Person.h"

#include "Texte.h"
#include "PersistenzManager.h"
#include "Wuerfel.h"

#include "PersistenzManager.h"
#include <numeric>
#include <algorithm>
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
         
  std::string s(int i) { return strings::int2sStr(i); }

	//@  @+others
	//@+node:Class Person
	//@+others
	//@+node:Administrativa
	//@+others
	//@+node:ctor
	Person::Person() : alter_(0), gewicht_(0), groesse_(0), gp_(regeln("Start-GP")) {
	  int i = 0;
	  for (i = 0; i!=AnzahlEigenschaften; ++i) {
	    eigenschaft_[i].Art(static_cast<Eigenschaftsart>(i));
	    eigenschaft_[i].Bezugsperson(this);
	  }
	  for (i = 0; i!=AnzahlAbgeleiteterWerte; ++i) {
	    abgeleiteterWert_[i].Art(static_cast<AbgeleiteterWertArt>(i));
	    abgeleiteterWert_[i].Bezugsperson(this);
	  }
	  weiblich_ = (W(2)==1);
	  sonderfertigkeiten_.ID(("tatsächliche"));
	} // ctor
	//@nonl
	//@-node:ctor
	//@+node:copyctor
	  Person::Person(const Person& other) :
	      id_(other.id_),
	      name_(other.name_),
	      gp_(other.gp_),
	      rasse_(other.rasse_),
	      kultur_(other.kultur_),
	      profession_(other.profession_),
	      talente_(other.talente_),
	      kampftechniken_(other.kampftechniken_),
	      sprachen_(other.sprachen_),
	      schriften_(other.schriften_),
	      zauberfertigkeiten_(other.zauberfertigkeiten_),
	      charakteristika_(other.charakteristika_),
	      sonderfertigkeiten_(other.sonderfertigkeiten_),
	      verbilligteSonderfertigkeiten_(other.verbilligteSonderfertigkeiten_),
	      alter_(other.alter_),
	      groesse_(other.groesse_),
	      gewicht_(other.gewicht_),
	      weiblich_(other.weiblich_),
	      augenfarbe_(other.augenfarbe_),
	      hauptfarbe_(other.hauptfarbe_),
	      beschreibung_(other.beschreibung_) {
	    int i = 0;
	    for (i = 0; i!=AnzahlEigenschaften; ++i) {
	      eigenschaft_[i] = other.eigenschaft_[i];
	      eigenschaft_[i].Bezugsperson(this);
	    }
	    for (i = 0; i!=AnzahlAbgeleiteterWerte; ++i) {
	      abgeleiteterWert_[i] = other.abgeleiteterWert_[i];
	      abgeleiteterWert_[i].Bezugsperson(this);
	    }
	    for (Kampftechniken::iterator k = kampftechniken_.begin(); k != kampftechniken_.end(); ++k)
	      k->second.Bezugsperson(this);
	    for (Talente::iterator t = talente_.begin(); t != talente_.end(); ++t)
	      t->second.Bezugsperson(this);
	    for (Sprachen::iterator s = sprachen_.begin(); s != sprachen_.end(); ++s)
	      s->second.Bezugsperson(this);
	    for (Schriften::iterator sch = schriften_.begin(); sch != schriften_.end(); ++sch)
	      sch->second.Bezugsperson(this);
	    for (Zauberfertigkeiten::iterator z = zauberfertigkeiten_.begin(); z != zauberfertigkeiten_.end(); ++z)
	      z->second.Bezugsperson(this);
	    sonderfertigkeiten_.ID(("tatsächliche"));
	  } // ctor
	//@nonl
	//@-node:copyctor
	//@+node:dtor
	Person::~Person() {} // dtor
	//@nonl
	//@-node:dtor
	//@+node:operator==
	  bool Person::operator== (const Person& other) const {
	    if (id_ != other.id_) return false;
	    if (name_ != other.name_) return false;
	    if (gp_ != other.gp_) return false;
	    if (beschreibung_ != other.beschreibung_) return false;
	        int i = 0;
	    for (i=0; i<AnzahlEigenschaften; ++i)
	      if (eigenschaft_[i] != other.eigenschaft_[i]) return false;
	    for (i=0; i<AnzahlAbgeleiteterWerte; ++i)
	      if (abgeleiteterWert_[i] != other.abgeleiteterWert_[i]) return false;
	    if (charakteristika_ != other.charakteristika_) return false;
	    if (sonderfertigkeiten_ != other.sonderfertigkeiten_) return false;
	    if (verbilligteSonderfertigkeiten_ != other.verbilligteSonderfertigkeiten_) return false;
	    if (talente_ != other.talente_) return false;
	    if (sprachen_ != other.sprachen_) return false;
	    if (schriften_ != other.schriften_) return false;
	    if (kampftechniken_ != other.kampftechniken_) return false;
	    if (zauberfertigkeiten_ != other.zauberfertigkeiten_) return false;
	    if (rasse_.operator!=(other.rasse_)) return false;
	    if (kultur_ != other.kultur_) return false;
	    if (profession_ != other.profession_) return false;
	    if (alter_ != other.alter_) return false;
	    if (augenfarbe_ != other.augenfarbe_) return false;
	    if (hauptfarbe_ != other.hauptfarbe_) return false;
	    if (gewicht_ != other.gewicht_) return false;
	    if (groesse_ != other.groesse_) return false;
	    if (weiblich_ != other.weiblich_) return false;
	    return true;
	  } // == // TODO
	//@nonl
	//@-node:operator==
	//@-others
	//@nonl
	//@-node:Administrativa
	//@+node:Leiteigenschaft
	Eigenschaftsart Person::Leiteigenschaft() const {
	  for (Charakteristika::const_iterator i = charakteristika_.begin(); i != charakteristika_.end(); ++i)
	    if (strings::BeginntMit(i->first, ("Leiteigenschaft")))
	      return EigenschaftString(strings::OhneAnfang(i->first, "Leiteigenschaft:"));
	   return unbekannteEigenschaft;
	} // Leiteigenschaft
	
	int Person::Leiteigenschaftswert() const {
	  if (Leiteigenschaft() == unbekannteEigenschaft)
	    return 0;
	  return Wert(Leiteigenschaft());
	} // Leiteigenschaftswert
	
	//@-node:Leiteigenschaft
	//@+node:Vorteile
	  Charakteristika Person::Vorteile() const {
	    Charakteristika vor;
	    for (Charakteristika::const_iterator i = charakteristika_.begin(); i != charakteristika_.end(); ++i)
	      if (i->second.Vorteil()) vor.FuegeEin(i->second);
	    return vor;
	  } // Vorteile
	//@nonl
	//@-node:Vorteile
	//@+node:Nachteile
	  Charakteristika Person::Nachteile() const {
	    Charakteristika nach;
	    for (Charakteristika::const_iterator i = charakteristika_.begin(); i != charakteristika_.end(); ++i)
	      if (i->second.Nachteil()) nach.FuegeEin(i->second);
	    return nach;
	  } // Nachteile
	//@nonl
	//@-node:Nachteile
	//@+node:Talente_
	  Talente Person::Talente_(Talentgruppe gruppe) const {
	    Talente tal;
	    for (Talente::const_iterator i = talente_.begin(); i != talente_.end(); ++i)
	      if (i->second.Gruppe()==gruppe) tal.FuegeEin(i->second);
	    return tal;
	  } // Talente
	//@nonl
	//@-node:Talente_
	//@+node:ErsetzeTag
	  std::string Person::ErsetzeTag(const std::string& tag) const {
	    if (tag.size()==0) return tag;
	
	    if ("Name"==tag) return Name();
	    if ("ID"==tag) return ID();
	    if ("Rasse"==tag) return Rasse_().ID();
	    if ("Kultur"==tag) return Kultur_().ID();
	    if ("Profession"==tag) return Profession_().ID();
	    if ("Geschlecht"==tag) return Maennlich()?_t("männlich"):_t("weiblich");
	    if ("Alter"==tag) return s(Alter());
	    if ("Gewicht"==tag) return s(Gewicht());
	    if ("Groesse"==tag) return s(Groesse());
	    if ("Augenfarbe"==tag) return Augenfarbe();
	    if ("Hauptfarbe"==tag) return Hauptfarbe(); // HauptfarbeID!
	    if ("Beschreibung"==tag) return Beschreibung();
	    if ("MU"==tag) return s(Wert(Mut));
	    if ("KL"==tag) return s(Wert(Klugheit));
	    if ("IN"==tag) return s(Wert(Intuition));
	    if ("CH"==tag) return s(Wert(Charisma));
	    if ("FF"==tag) return s(Wert(Fingerfertigkeit));
	    if ("GE"==tag) return s(Wert(Gewandtheit));
	    if ("KO"==tag) return s(Wert(Konstitution));
	    if ("KK"==tag) return s(Wert(Koerperkraft));
	    if ("LE"==tag) return s(Wert(Lebensenergie));
	    if ("AU"==tag) return s(Wert(Ausdauer));
	    if ("AT"==tag) return s(Wert(AttackeBasis));
	    if ("PA"==tag) return s(Wert(ParadeBasis));
	    if ("FK"==tag) return s(Wert(FernkampfBasis));
	    if ("INI"==tag) return s(Wert(InitiativeBasis));
	    if ("MR"==tag) return s(Wert(Magieresistenz));
	    if ("AE"==tag) return s(Wert(Astralenergie));
	    if ("SO"==tag) return s(Wert(Sozialstatus));
	    if ("AP"==tag) return s(0);
	    if ("APV"==tag) return s(0);
	    if ("APG"==tag) return s(0);
	    if ("ST"==tag) return s(1);
	
	    if (tag.size()<3) return tag;
	    std::string subtag = tag.substr(2, tag.size()-2);
	    std::string rv = subtag;
	    if ("ch" == tag.substr(0, 2)) rv = charakteristika_.Sichtbar().ErsetzeTag(subtag);
	    if ("vt" == tag.substr(0, 2)) rv = Vorteile().ErsetzeTag(subtag);
	    if ("nt" == tag.substr(0, 2)) rv = Nachteile().ErsetzeTag(subtag);
	    if ("sf" == tag.substr(0, 2)) rv = sonderfertigkeiten_.Bereinige().ErsetzeTag(subtag);
	    if ("ta" == tag.substr(0, 2)) rv = talente_.ErsetzeTag(subtag);
	    if ("tk" == tag.substr(0, 2)) rv = Talente_(Koerperlich).ErsetzeTag(subtag);
	    if ("tg" == tag.substr(0, 2)) rv = Talente_(Gesellschaftlich).ErsetzeTag(subtag);
	    if ("tn" == tag.substr(0, 2)) rv = Talente_(Natur).ErsetzeTag(subtag);
	    if ("tw" == tag.substr(0, 2)) rv = Talente_(Wissen).ErsetzeTag(subtag);
	    if ("th" == tag.substr(0, 2)) rv = Talente_(Handwerk).ErsetzeTag(subtag);
	    //if ("ta" == tag.substr(0, 2)) rv = Talente_(Gaben).ErsetzeTag(subtag);
	    if ("sp" == tag.substr(0, 2)) rv = sprachen_.ErsetzeTag(subtag);
	    if ("sc" == tag.substr(0, 2)) rv = schriften_.ErsetzeTag(subtag);
	    if ("kt" == tag.substr(0, 2)) rv = kampftechniken_.ErsetzeTag(subtag);
	    if (subtag==rv) return tag; else return rv;
	
	    return tag;
	  } // ErsetzeTag
	//@nonl
	//@-node:ErsetzeTag
	//@+node:Lese
	int Person::Lese(PersistenzManager* pm, const std::string& id) {
	  std::string geleseneID = id;
	  int rv = pm->OeffneZumLesen(Klasse(), geleseneID);
	  std::string name, sBuf;
	  rv += pm->Lese(("Name"), name);
	  int gp = regeln("Start-GP");
	  if (pm->Existiert(("GP")))
	    rv += pm->Lese(("GP"), gp);
	  rv += pm->Lese(("Geschlecht"), sBuf);
	  bool weiblich = true;
	  if (sBuf==("männlich")) weiblich = false;
	
	  Rasse rasse;
	  Kultur kultur;
	  Profession profession;    
	
	  rv += pm->OeffneZumLesen(("Rasse"));
	  bool ganzeRKP = pm->Existiert(("GP"));
	  rv += pm->SchliesseNachLesen();
	    
	  if (!ganzeRKP) {
	    std::string rassestring, kulturstring, professionstring;
	    rv += pm->Lese(("Rasse"), rassestring);
	    rv += pm->Lese(("Kultur"), kulturstring);
	    rv += pm->Lese(("Profession"), professionstring);
	    rasse.ID(rassestring);
	    kultur.ID(kulturstring);
	    profession.ID(professionstring);
	  } else {
	    rv += rasse.Lese(pm);
	    rv += kultur.Lese(pm);
	    rv += profession.Lese(pm);
	  }      
	        
	  rv += pm->OeffneZumLesen(("Eigenschaften"));
	  Eigenschaft eigenschaft[AnzahlEigenschaften];
	  int i = 0;
	  for (i = 0; i!=AnzahlEigenschaften; ++i)
	    rv += eigenschaft[i].Lese(pm, EigenschaftString(static_cast<Eigenschaftsart>(i)));
	  rv += pm->SchliesseNachLesen();
	  rv += pm->OeffneZumLesen(("AbgeleiteteWerte"));
	  AbgeleiteterWert abgeleiteterWert[AnzahlAbgeleiteterWerte];
	  for (i = 0; i!=AnzahlAbgeleiteterWerte; ++i)
	    rv += abgeleiteterWert[i].Lese(pm, AbgeleiteterWertString(static_cast<AbgeleiteterWertArt>(i)));
	  rv += pm->SchliesseNachLesen();
	
	  std::string hauptfarbe, augenfarbe;
	  int groesse, gewicht, alter;
	  rv += pm->Lese(("Hauptfarbe"), hauptfarbe);
	  rv += pm->Lese(("Augenfarbe"), augenfarbe);
	  rv += pm->Lese(("Groesse"), groesse);
	  rv += pm->Lese(("Gewicht"), gewicht);
	  rv += pm->Lese(("Alter"), alter);
	
	  Talente talente;
	  rv += talente.Lese(pm);
	  Sprachen sprachen;
	  rv += sprachen.Lese(pm);
	  Schriften schriften;
	  rv += schriften.Lese(pm);
	  Kampftechniken kampftechniken;
	  rv += kampftechniken.Lese(pm);
	  Charakteristika charakteristika;
	  rv += charakteristika.Lese(pm);
	  Sonderfertigkeiten sonderfertigkeiten;
	  rv += sonderfertigkeiten.Lese(pm, ("tatsächliche"));
	  StringListe verbilligteSonderfertigkeiten;
	  rv += pm->Lese(verbilligteSonderfertigkeiten, "Sonderfertigkeit", "Sonderfertigkeiten", "verbilligte");
	  Zauberfertigkeiten zauberfertigkeiten;
	  rv += zauberfertigkeiten.Lese(pm);
	  std::string beschreibung;
	  if (pm->Existiert(_t("Beschreibung")))
	    rv += pm->Lese(_t("Beschreibung"), beschreibung);
	
	  if (!rv) {
	    ID(geleseneID);
	    Name(name);
	    GP(gp);
	    Weiblich(weiblich);
	    Rasse_(rasse);
	    Kultur_(kultur);
	    Profession_(profession);
	    int i = 0;
	    for (i = 0; i!=AnzahlEigenschaften; ++i) {
	      eigenschaft_[i] = eigenschaft[i];
	      eigenschaft_[i].Bezugsperson(this);
	    }
	    for (i = 0; i!=AnzahlAbgeleiteterWerte; ++i) {
	      abgeleiteterWert_[i] = abgeleiteterWert[i];
	      abgeleiteterWert_[i].Bezugsperson(this);
	    }
	    Hauptfarbe(hauptfarbe);
	    Augenfarbe(augenfarbe);
	    Groesse(groesse);
	    Gewicht(gewicht);
	    Alter(alter);
	    talente_ = talente;
	    for (Talente::iterator t = talente_.begin(); t != talente_.end(); ++t)
	      t->second.Bezugsperson(this);
	    
	    // Sprachen werden gesondert behandelt, damit Mutter- und Zweitsprachen funktionieren
	    sprachen_.clear();
	    for (Sprachen::iterator s = sprachen.begin(); s != sprachen.end(); ++s) {
	      Sprache sp = s->second;
	      sp.Bezugsperson(this);
	      sp.Wert(s->second.Wert());
	      sprachen_.FuegeEin(sp);
	    }
	    
	    schriften_ = schriften;
	    for (Schriften::iterator sch = schriften_.begin(); sch != schriften_.end(); ++sch)
	      sch->second.Bezugsperson(this);
	    kampftechniken_ = kampftechniken;
	    for (Kampftechniken::iterator k = kampftechniken_.begin(); k != kampftechniken_.end(); ++k)
	      k->second.Bezugsperson(this);
	    charakteristika_ = charakteristika;
	    sonderfertigkeiten_ = sonderfertigkeiten;
	    verbilligteSonderfertigkeiten_ = verbilligteSonderfertigkeiten;
	    zauberfertigkeiten_ = zauberfertigkeiten;
	    for (Zauberfertigkeiten::iterator z = zauberfertigkeiten_.begin(); z != zauberfertigkeiten_.end(); ++z)
	      z->second.Bezugsperson(this);
	    Beschreibung(beschreibung);
	  }
	  rv +=  pm->SchliesseNachLesen();
	  return rv; // TODO
	} // Lese
	//@-node:Lese
	//@+node:Schreibe
	int Person::Schreibe(PersistenzManager* pm) const {
	  int rv = pm->OeffneZumSchreiben(Klasse(), ID());
	  rv += pm->Schreibe(("Name"), Name());
	  rv += pm->Schreibe(("GP"), GP());
	  rv += pm->Schreibe(("Geschlecht"), ((Weiblich())?("weiblich"):("männlich")));
	
	  rv += Rasse_().Schreibe(pm);
	  rv += Kultur_().Schreibe(pm);
	  rv += Profession_().Schreibe(pm);
	
	  rv += pm->OeffneZumSchreiben(("Eigenschaften"));
	  int i = 0;
	  for (i = 0; i!=AnzahlEigenschaften; ++i)
	    rv += eigenschaft_[i].Schreibe(pm);
	  rv += pm->SchliesseNachSchreiben();
	  rv += pm->OeffneZumSchreiben(("AbgeleiteteWerte"));
	  for (i = 0; i!=AnzahlAbgeleiteterWerte; ++i)
	    rv += abgeleiteterWert_[i].Schreibe(pm);
	  rv += pm->SchliesseNachSchreiben();
	
	  rv += pm->Schreibe(("Hauptfarbe"), Hauptfarbe());
	  rv += pm->Schreibe(("Augenfarbe"), Augenfarbe());
	  rv += pm->Schreibe(("Groesse"), Groesse());
	  rv += pm->Schreibe(("Gewicht"), Gewicht());
	  rv += pm->Schreibe(("Alter"), Alter());
	
	  rv += talente_.Schreibe(pm);
	  rv += sprachen_.Schreibe(pm);
	  rv += schriften_.Schreibe(pm);
	  rv += kampftechniken_.Schreibe(pm);
	  rv += charakteristika_.Schreibe(pm);
	  rv += sonderfertigkeiten_.Schreibe(pm);
	  rv += pm->Schreibe(verbilligteSonderfertigkeiten_, ("Sonderfertigkeit"), ("Sonderfertigkeiten"), ("verbilligte"));
	  rv += zauberfertigkeiten_.Schreibe(pm);
	
	  if (Beschreibung()!="")
	    rv += pm->Schreibe(("Beschreibung"), Beschreibung());
	
	  rv += pm->SchliesseNachSchreiben();
	
	  return rv; // TODO
	} // Schreibe
	//@nonl
	//@-node:Schreibe
	//@+node:Pruefe
	int Person::Pruefe() {
	  int rv = 0;
	  int i;
	  for (i = 0; i!=AnzahlEigenschaften; ++i) {
	    if (eigenschaft_[i].Art()!=static_cast<Eigenschaftsart>(i)) {
	      Log::Info("Person::Pruefe: Eigenschaftenkonsistenz nicht eingehalten (" + _t("fatal") + ")");
	      ++rv;
	    }
	    rv += eigenschaft_[i].Pruefe();
	  }
	  for (i = 0; i!=AnzahlAbgeleiteterWerte; ++i) {
	    if (abgeleiteterWert_[i].Art()!=static_cast<AbgeleiteterWertArt>(i)) {
	      Log::Info("Person::Pruefe: Konsistenz der Abgeleiteten Werte nicht eingehalten (" + _t("fatal") + ")");
	      ++rv;
	    }
	    rv += abgeleiteterWert_[i].Pruefe();
	  }
	  rv += talente_.Pruefe();
	  rv += sprachen_.Pruefe();
	  rv += schriften_.Pruefe();
	  rv += kampftechniken_.Pruefe();
	  rv += zauberfertigkeiten_.Pruefe();
	  rv += charakteristika_.Pruefe();
	  if (sonderfertigkeiten_.ID()!=("tatsächliche")) {
	    Log::Info("Person::Pruefe: ID von Sonderfertigkeiten falsch (" + _t("fatal") + ")");
	    ++rv;
	  }      
	  rv += sonderfertigkeiten_.Pruefe();
	  for (StringListe::const_iterator si = verbilligteSonderfertigkeiten_.begin(); si != verbilligteSonderfertigkeiten_.end(); ++si)
	    if (!((sonderfertigkeiten()->Existiert(*si))
	        ||(generierteSonderfertigkeiten()->Existiert(*si))))
	      Log::Info("Person::Pruefe: Unbekannte Sonderfertigkeit " + *si);
	
	  if (rv!=0) Log::Info(_t("in") + " " + Klasse() + " " + ID());
	  return rv;
	} // Pruefe
	//@-node:Pruefe
	//@-others
	//@-node:Class Person
	//@-others

  Personen* personen() { static Personen* personen_ = new Personen(); return personen_; }
}

//@<< Version History >>
//@+node:<< Version History >>
// $Log: Person.cpp,v $
// Revision 1.24  2004/10/17 19:12:39  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.23  2004/04/12 21:08:09  vrandezo
// Lehrsprachen implementiert
// Mutter-/Zweitsprachensystem umgebaut
//
// Revision 1.22  2004/04/09 23:49:55  vrandezo
// Zauberfertigkeiten bei RKP
// Personen verwalten ZF besser
//
// Revision 1.21  2004/04/09 22:25:50  vrandezo
// Zauberfertigkeiten fuer Personen eintragbar
//
// Revision 1.20  2003/11/28 23:22:49  vrandezo
// speicherbare konkrete RKP
//
// Revision 1.19  2003/10/30 00:02:44  vrandezo
// Steigerbare Ritualkenntnisse
//
// Revision 1.18  2003/10/27 18:24:50  vrandezo
// unsichtbare Vor- und Nachteile
// Tänze fremder Repräsentationen teurer
//
// Revision 1.17  2003/10/25 02:11:17  vrandezo
// generierte Sonderfertigkeiten
//
// Revision 1.16  2003/10/24 13:30:07  vrandezo
// DEBUG_NEW raus
// Varianten jetzt von Hand eintragbar
//
// Revision 1.15  2003/10/20 02:11:50  vrandezo
// IstMagisch?
//
// Revision 1.14  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.13  2003/03/27 14:28:41  vrandezo
// Personen haben eigene Start-GP
//
// Revision 1.12  2003/03/25 09:40:16  vrandezo
// Begabung für [Talentgruppe]
//
// Revision 1.11  2003/03/24 18:57:16  vrandezo
// Neue Vorteile: Herausragende Eigenschaften (Forumswunsch Sebastian)
//
// Revision 1.10  2003/03/07 15:06:42  vrandezo
// AT/PA-Werte bei Kampftechniken (Heldenbrief Gerhard Mayer)
//
// Revision 1.9  2003/03/06 17:22:01  vrandezo
// das for-Tag funktioniert, ebenso Variable $
// die meisten Tags funktionieren / leicht korrigiert
//
// Revision 1.8  2003/03/04 18:30:32  vrandezo
// schon wieder möglichen Bug beim Lesen von Personen (SF) korrigiert
// Korrekturen Standardtags
//
// Revision 1.7  2003/03/04 15:23:18  vrandezo
// weitere Tags implementiert für die Druckvorlagen
//
// Revision 1.6  2003/03/04 02:24:22  vrandezo
// Vorlagendateien implementiert
// if implementiert
//
// Revision 1.5  2003/02/24 17:37:30  vrandezo
// Prüfen implementiert
//
// Revision 1.4  2003/02/20 09:19:45  vrandezo
// Prüfe-Signatur geändert
//
// Revision 1.3  2003/02/20 00:21:58  vrandezo
// Sonderfertigkeiten vollständig integriert
// schlafende SF
//
// Revision 1.2  2003/02/19 19:34:27  vrandezo
// Bug: Verrechnungspunkte für Schlechte Eigenschaften sind nun korrekt
// verbilligte SF bei Personen und RKP eingefügt
// RKP mit SF und VSF ausstattbar
//
// Revision 1.1.1.1  2003/02/17 15:33:17  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.30  2003/02/14 21:42:50  vrandezo
// Sonderfertigkeiten hinzugefügt
//
// Revision 1.29  2003/02/13 20:13:02  vrandezo
// gcc-Portabilität verbessert
//
// Revision 1.28  2003/02/13 11:44:46  vrandezo
// Bug entfernt: Sprachen und Schriften bei Personen bleiben erhalten
// Bug entfernt: Kulturen-Persistenz korrigiert (L/S Sprachen)
// Person-Vergleich korrigiert
// Dateibegrenzung auf 4MB erhöht
// XML geändert: Sprachen jetzt mit "Sprachen kennen"-ID bei RKP
//
// Revision 1.27  2003/02/08 00:22:54  vrandezo
// VN von RKP bei Generierung übernommen
// mehrfache VN bei RKP geben GP
// erweitere Voraussetzungen: nicht VN
// entfernbare VN
// auswählbare VN
// VN mit Voraussetzungen (nicht VN)
// Geschlecht neuerschaffener Helden ist zufällig
//
// Revision 1.26  2003/01/26 14:27:29  vrandezo
// Astralenergie auf 0 gesetzt
// Helden haben jetzt Raum für Notizen
//
// Revision 1.25  2003/01/24 20:33:33  vrandezo
// übliche Kulturen auswählbar bei Heldenerschaffung
// Wert der Mutter- und Zweitsprache auch bei Modifikatoren korrekt
// Geschlecht wechselbar
//
// Revision 1.24  2003/01/18 18:10:41  vrandezo
// Personen können Sprachen und Schriften
// übliche Kulturen bei Rassen hinzugefügt
// GUI für mögliche und übliche Kulturen und für mögliche Professionen geändert
// Varianten ausblendbar bei der Eingabe möglicher Kulturen und Professionen
//
// Revision 1.23  2003/01/17 17:45:32  vrandezo
// Notebook-Bug  korrigiert (kam nur in der Release-Version vor)
// Varianten automatisch zugelassen, wenn Basis möglich
// Auswahl von Rasse, Profession, Kultur vereinfacht
// Talentauswahlauflösung implementiert (einfach und Liste)
//
// Revision 1.22  2002/12/06 15:28:00  vrandezo
// Spezies bei den Rassen einstellbar
// Kampftechniken bei PK und bei Person hinzugefügt
//
// Revision 1.21  2002/11/05 21:49:18  vrandezo
// Sprachen und Schriften im- und exportierbar
// bool const ueberschreiben aus Speicher-Schnittstelle entfernt
// Mengen haben IDs erhalten
// Voraussetzungen für Talente (allgemeine und meisterliche)
// MengeGUIwx beim Wechseln von Objekten mit Rückfrage
//
// Revision 1.20  2002/11/03 17:48:36  vrandezo
// Haarfarbe und Augenfarbe jetzt auch bei der Heldenerschaffung
//
// Revision 1.19  2002/11/03 14:25:33  vrandezo
// Wuerfel angepasst
// Haar- und Augenfarbe hinzugefügt bei Rassen
//
// Revision 1.18  2002/11/02 23:36:57  vrandezo
// Exportier und Importierfunktionen implementiert
// Vergleichsoperationen implementiert
// Kampftechnikfenstter implementiert
//
// Revision 1.17  2002/11/02 15:39:48  vrandezo
// Importfenster hinzugefügt
//
// Revision 1.16  2002/10/27 17:29:21  vrandezo
// Die meisten Teile sind speicherbar
//
// Revision 1.15  2002/10/25 17:51:06  vrandezo
// Modifikatoren für Personen
// GP-Kosten für Personenkomponenten veränderbar
//
// Revision 1.14  2002/10/19 20:35:32  vrandezo
// feste Stringliterale entfernt
//
// Revision 1.13  2002/10/19 18:58:32  vrandezo
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
// Revision 1.7  2002/09/28 00:28:37  vrandezo
// Beginn des Generators mit DSA.lib und in wxWindows
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
//@-node:@file Person.cpp
//@-leo
