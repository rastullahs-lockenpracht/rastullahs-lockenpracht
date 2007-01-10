//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file Sonderfertigkeit.cpp
//@+at
// Sonderferigkeit.cpp: Implementierung von 
// Sonderferigkeiten.
// 
// $Revision: 1.28 $
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
#include "Sonderfertigkeit.h"

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

	//@  @+others
	//@+node:class SonderfertigkeitDefinition
	//@+others
	//@+node:operator==
	bool SonderfertigkeitDefinition::operator==(const SonderfertigkeitDefinition& other) const {
		if (Definition::operator!=(other)) return false;
	  if (voraussetzungen_!=other.voraussetzungen_) return false;
	  if (waffenloserkampfstil_!=other.waffenloserkampfstil_) return false;
	  if (waffenlosesonderfertigkeit_!=other.waffenlosesonderfertigkeit_) return false;
	  if (zaubertanz_!=other.zaubertanz_) return false;
	  if (extras_!=other.extras_) return false;
	  if (brauchtSpezifizierung_!=other.brauchtSpezifizierung_) return false;
	  if (ap_!=other.ap_) return false;
	  if (gp_!=other.gp_) return false;
	  return true;
	}
	//@-node:operator==
	//@+node:Bezeichnung
	std::string SonderfertigkeitDefinition::Bezeichnung(const std::string& sprache) const {
	  if (!IstGeneriert())
	    return Definition::Bezeichnung(sprache);
	    
	  if (IstScharfschuetze())
	    return _t("Scharfschütze") + " (" + kampftechniken()->Zeige(Scharfschuetze())->Bezeichnung() + ")";
	
	  if (IstMeisterschuetze())
	    return _t("Meisterschütze") + " (" + kampftechniken()->Zeige(Meisterschuetze())->Bezeichnung() + ")";
	
	  if (IstRepraesentation())
	    return _t("Repräsentation") + " (" + ZauberrepraesentationString(Repraesentation()) + ")";
	
	  if (IstRitualkenntnis())
	    return _t("Ritualkenntnis") + " (" + ZauberrepraesentationString(Ritualkenntnis()) + ")";
	
	  return ID();
	}
	//@nonl
	//@-node:Bezeichnung
	//@+node:Semantische Abfragen
	//@+others
	//@+node:Schütze
	bool SonderfertigkeitDefinition::IstScharfschuetze() const {
	  if (!IstGeneriert()) return false;
	  return strings::BeginntMit(ID(), "SonderfertigkeitScharfschuetze");
	}
	
	std::string SonderfertigkeitDefinition::Scharfschuetze() const {
	  if (!IstScharfschuetze()) return "";
	  return strings::OhneAnfangExakt(ID(), "SonderfertigkeitScharfschuetze");
	}
	
	bool SonderfertigkeitDefinition::IstMeisterschuetze() const {
	  if (!IstGeneriert()) return false;
	  return strings::BeginntMit(ID(), "SonderfertigkeitMeisterschuetze");
	}
	
	std::string SonderfertigkeitDefinition::Meisterschuetze() const {
	  if (!IstMeisterschuetze()) return "";
	  return strings::OhneAnfangExakt(ID(), "SonderfertigkeitMeisterschuetze");
	}
	//@-node:Schütze
	//@+node:Gelaendekunde
	bool SonderfertigkeitDefinition::IstGelaendekunde() const {
	  std::string temp = "kundig";
	  if (ID().size()>temp.size())
	    if (ID().substr(ID().size()-temp.size(), temp.size())==temp)
	      return true;
	  return false;
	}
	//@-node:Gelaendekunde
	//@+node:Repraesentation
	bool SonderfertigkeitDefinition::IstRepraesentation() const {
	  if (!IstGeneriert()) return false;
	  return strings::BeginntMit(ID(), "SonderfertigkeitRepraesentation");
	}
	
	Zauberrepraesentation SonderfertigkeitDefinition::Repraesentation() const {
	  if (!IstRepraesentation()) return (Zauberrepraesentation)(-1);
	  return ZauberrepraesentationString(strings::OhneAnfangExakt(ID(), "SonderfertigkeitRepraesentation"));
	}
	
	//@-node:Repraesentation
	//@+node:Ritualkenntnis
	bool SonderfertigkeitDefinition::IstRitualkenntnis() const {
	  if (IstRitualkenntnisZaubertaenzer()) return true;
	  if (!IstGeneriert()) return false;
	  return strings::BeginntMit(ID(), "SonderfertigkeitRitualkenntnis");
	}
	
	Zauberrepraesentation SonderfertigkeitDefinition::Ritualkenntnis() const {
	  if (!IstRitualkenntnis()) return unbekannteRepraesentation;
	  if (IstRitualkenntnisZaubertaenzer()) Log::Info("SonderfertigkeitDefinition::Ritualkenntnis: SonderfertigkeitRitualkenntnisZaubertaenzer falsch abgefragt");
	  return ZauberrepraesentationString(strings::OhneAnfangExakt(ID(), "SonderfertigkeitRitualkenntnis"));
	}
	
	bool SonderfertigkeitDefinition::IstRitualkenntnisZaubertaenzer() const {
	  return ID()=="SonderfertigkeitRitualkenntnisZaubertaenzer";
	}
	
	//@-node:Ritualkenntnis
	//@+node:Merkmalskenntnis
	bool SonderfertigkeitDefinition::IstMerkmalskenntnis() const {
	  return strings::BeginntMit(ID(), "SonderfertigkeitMerkmalskenntnis");
	}
	
	Zaubermerkmal SonderfertigkeitDefinition::Merkmalskenntnis() const {
	  if (!IstMerkmalskenntnis()) {
	    Log::Warning("SonderfertigkeitDefinition::Merkmalskenntnis: Es wurde nach Merkmalskenntnis gefragt, obwohl SF keine Merkmalskenntnis ist. ID: " + ID());
	    return unbekanntesMerkmal;
	  }
	  return ZaubermerkmalString(strings::OhneAnfangExakt(ID(), "SonderfertigkeitMerkmalskenntnis"));
	}
	
	Erzdaemon SonderfertigkeitDefinition::Domaene() const {
	  if (Merkmalskenntnis()!=Daemonisch)  {
	    Log::Warning("SonderfertigkeitDefinition::Domaene: Es wurde nach Domäne gefragt, obwohl SF keine Merkmalskenntnis Dämonisch ist. ID: " + ID());
	    return unbekannteDomaene;
	  }
	  return ErzdaemonString(strings::OhneAnfangExakt(ID(), "SonderfertigkeitMerkmalskenntnisDaemonisch"));
	}
	
	Element SonderfertigkeitDefinition::Element() const {
	  if (Merkmalskenntnis()!=Elementar)  {
	    Log::Warning("SonderfertigkeitDefinition::Element: Es wurde nach Element gefragt, obwohl SF keine Merkmalskenntnis Elementar ist. ID: " + ID());
	    return unbekanntesElement;
	  }
	  return ElementString(strings::OhneAnfangExakt(ID(), "SonderfertigkeitMerkmalskenntnisElementar"));
	}
	
	
	//@-node:Merkmalskenntnis
	//@-others
	//@nonl
	//@-node:Semantische Abfragen
	//@+node:NurID
	// Ach Du meine Güte, die folgenden vier Funktionen sind eher
	// lustlos geschrieben, aber auch nur, weil die Regeln in diesem
	// Bereich ein wenig verkorkst sind. Was soll, hauptsache, es tut.
	// Und das tut es - wenn es nur die Stufen I-III gibt!
	std::string SonderfertigkeitDefinition::NurID() const {
	  if (!HatStufen())
	    return ID();
	  return ID().substr(0, ID().size()-(Stufe()));
	}
	//@nonl
	//@-node:NurID
	//@+node:Text
	std::string SonderfertigkeitDefinition::Text() const {
	  if (IstWaffenloserKampfstil())
	    return _t("Waffenloser Kampfstil") + ": " + ID();
	  else if (IstZaubertanz())
	    return _t("Zaubertanz") + ": " + ID();
	  else
	    return ID();
	} // Text
	//@nonl
	//@-node:Text
	//@+node:HatStufen
	bool SonderfertigkeitDefinition::HatStufen() const {
	  return (Stufe()!=0);
	} // HatStufen
	//@nonl
	//@-node:HatStufen
	//@+node:Stufe
	int SonderfertigkeitDefinition::Stufe() const {
	  std::string id = ID();
	  if (id.size()<3) return 0;
	  if (id.substr(id.size()-1, 1)!="I")   return 0;
	  if (id.substr(id.size()-2, 2)!="II")  return 1;
	  if (id.substr(id.size()-3, 3)!="III") return 2;
	                                        return 3;
	} // Stufe
	
	std::string SonderfertigkeitDefinition::Stufe(int stufe) const {
	  if (!HatStufen()) return "";
	  if (stufe>MaximalStufe()) return "";
	  if (stufe==3) return NurID() + "III";
	  if (stufe==2) return NurID() + "II";
	  if (stufe==1) return NurID() + "I";
	  return "";
	} // Stufe
	//@nonl
	//@-node:Stufe
	//@+node:MaximalStufe
	int SonderfertigkeitDefinition::MaximalStufe() const {
	  if (!HatStufen()) return 0;
	  if (sonderfertigkeiten()->Existiert(NurID() + "III")) return 3;
	  if (sonderfertigkeiten()->Existiert(NurID() + "II"))  return 2;
	  if (sonderfertigkeiten()->Existiert(NurID() + "I"))   return 1;
	  Log::Debug("SonderfertigkeitDefinition::MaximalStufe - Fehler");
	  return 0;
	} // MaximalStufe
	//@nonl
	//@-node:MaximalStufe
	//@+node:Hinzufuegbar
	bool SonderfertigkeitDefinition::Hinzufuegbar(const Person& p, bool schlaftest) const {
	
	//@+at
	// Wird nicht mehr benötigt, da dies jetzt durch Daten 
	// realisiert werden kann.
	//   // Voraussetzung von Binden ist u.a. Meisterparade 
	// oder Parierwaffen I
	//   if (ID()==_t("Binden"))
	//     if 
	// (!((p.sonderfertigkeiten_.Existiert(_t("Meisterparade")))
	// ||(p.sonderfertigkeiten_.Existiert(_t("Parierwaffen 
	// I")))))
	//       return false;
	//@-at
	//@@c
	
		//@  << Repräsentationen >>
		//@+node:<< Repräsentationen >>
		if (IstRepraesentation()) {
		  // Wieviele hat der Held schon?
		  int anzahl = 0;
		  forallconst (Sonderfertigkeiten, sf, p.sonderfertigkeiten_)
		    if (sf->second.Definition()->IstRepraesentation())
		      ++anzahl;
		  
		  if (schlaftest) --anzahl; // wenn wir hier testen, ob eine rep schlafen ist, dann ist sie ja schon dabei, und wie muessen um eine weniger testen
		  
		  if (anzahl == 1) {
		    // Die Standardvoraussetzungen prüfen schon auf Voll- und Halbzauberer
		    if (p.Wert(Klugheit) < 15) // TODO Konstante
		      return false;
		    if (p.Wert(Intuition) < 15) // TODO Konstante
		      return false;      
		  }
		  if (anzahl == 2) {
		    if (!p.charakteristika_.Existiert("CharakteristikumVollzauberer"))
		      return false;
		    if (p.Wert(Klugheit) < 18) // TODO Konstante
		      return false;      
		  }
		
		  int zfsumme = 0;
		DSA::Zauberrepraesentation rep = DSA::ZauberrepraesentationString(strings::InKlammern(ID()));
		  forallconst (Zauberfertigkeiten, zfw, p.zauberfertigkeiten_)
		    if (zfw->second.Repraesentation() == rep)
		      zfsumme += zfw->second.Wert();
		  if (zfsumme < 50) // TODO Konstante
		    return false;    
		  
		
		}
		//@nonl
		//@-node:<< Repräsentationen >>
		//@nl
	  
		//@  << Merkmalskenntnisse >>
		//@+node:<< Merkmalskenntnisse >>
		if (IstMerkmalskenntnis()) {
		  // Wieviele hat der Held schon?
		  int anzahl = 0;
		  forallconst (Sonderfertigkeiten, sf, p.sonderfertigkeiten_)
		    if (sf->second.Definition()->IstMerkmalskenntnis())
		      ++anzahl;
		  
		  if (schlaftest) --anzahl; // wenn wir hier testen, ob eine rep schlafen ist, dann ist sie ja schon dabei, und wie muessen um eine weniger testen
		  
		  if (anzahl == 1) {
		    // Die Standardvoraussetzungen prüfen schon auf Spruchzauberer
		    if (p.Wert(Klugheit) < 13) // TODO Konstante
		      return false;
		  }
		  if (anzahl > 1) {
		    if (p.Wert(Klugheit) < (13 + anzahl)) // TODO Konstante
		      return false;      
		  }
		}
		//@nonl
		//@-node:<< Merkmalskenntnisse >>
		//@nl
	
	  return voraussetzungen_.Erfuellt(p);
	} // Hinzufuegbar
	//@-node:Hinzufuegbar
	//@+node:Extra
	std::string SonderfertigkeitDefinition::Extra(int i) const {
	  if (extras_.size()>i)
	    return extras_[i];
	  else
	    return "";
	}
	
	void SonderfertigkeitDefinition::Extra(std::string x, int i) {
	  if (extras_.size()>i)
	    extras_[i] = x;
	  else
	    extras_.push_back(x);
	}
	//@nonl
	//@-node:Extra
	//@+node:Lese
	int SonderfertigkeitDefinition::Lese(PersistenzManager* pm, const std::string& id) {
		std::string geleseneID = id;
		int rv = pm->OeffneZumLesen(Klasse(), geleseneID);
	  Dict bezeichnung;
	  Dict beschreibung;
	  rv += LeseBezeichnung(pm, geleseneID, bezeichnung, beschreibung);
		int ap;
	  rv += pm->Lese(("AP"), ap);
		int gp;
	  rv += pm->Lese(("GP"), gp);
	  bool waffenloserkampfstil = pm->Existiert(("WaffenloserKampfstil"));
	  bool waffenlosesonderfertigkeit = pm->Existiert(("WaffenloseSonderfertigkeit"));
	  bool zaubertanz = pm->Existiert(("Zaubertanz"));
	  int z = 0;
	  if (waffenloserkampfstil) ++z;
	  if (waffenlosesonderfertigkeit) ++z;
	  if (zaubertanz) ++z;
	  if (z>1) {
	    Log::Info("SonderfertigkeitDefinition::Lese - Sonderfertigkeit " + id + " ist gleichzeitig Waffenloser Kampfstil, waffenlose Sonderfertigkeit und/oder Zaubertanz.");
	    ++rv;
	  }
	  StringListe extras;
	  std::string extra;
	  if (waffenloserkampfstil) {
	    pm->Lese(("WaffenloserKampfstil"), extra);
	    extras.push_back(extra);
	  }
	  if (zaubertanz) {
	    pm->Lese(("Zaubertanz"), extra);
	    extras.push_back(extra);
	  }
	  if (waffenlosesonderfertigkeit) {
	    pm->Lese(extras, ("Kampfstil"), ("WaffenloseSonderfertigkeit"));
	  }
	  bool brauchtSpezifizierung = pm->Existiert(("brauchtSpezifizierung"));
	  Voraussetzungen voraussetzungen;
	  rv += voraussetzungen.Lese(pm);
	
		if (!rv) {
			ID(geleseneID);
	 bezeichnung_ = bezeichnung;
	 beschreibung_ = beschreibung;
			GP(gp);
	    AP(ap);
	    IstWaffenloserKampfstil(waffenloserkampfstil);
	    IstWaffenloseSonderfertigkeit(waffenlosesonderfertigkeit);
	    IstZaubertanz(zaubertanz);
	    BrauchtSpezifizierung(brauchtSpezifizierung);
	    Extras(extras);
	    voraussetzungen_ = voraussetzungen;
		}
		rv += pm->SchliesseNachLesen();
		return rv; // TODO
	} // Lese
	//@nonl
	//@-node:Lese
	//@+node:Schreibe
	int SonderfertigkeitDefinition::Schreibe(PersistenzManager* pm) const {
		int rv = pm->OeffneZumSchreiben(Klasse(), id_);
	  rv += SchreibeBezeichnung(pm);
	  rv += pm->Schreibe(("AP"), ap_);
		rv += pm->Schreibe(("GP"), gp_);
	  if (IstWaffenloserKampfstil())
	    rv += pm->Schreibe(("WaffenloserKampfstil"), Extra());
	  if (IstZaubertanz())
	    rv += pm->Schreibe(("Zaubertanz"), Extra());
	  if (IstWaffenloseSonderfertigkeit()) {
	    StringListe s = Extras();
	    rv += pm->Schreibe(s, ("Kampfstil"), ("WaffenloseSonderfertigkeit"));
	    if (s.size()==0) {
	      rv += pm->OeffneZumSchreiben(("WaffenloseSonderfertigkeit"));
	      rv += pm->SchliesseNachSchreiben();
	    }
	  }
	  if (BrauchtSpezifizierung()) {
	    rv += pm->OeffneZumSchreiben(("brauchtSpezifizierung"));
	    rv += pm->SchliesseNachSchreiben();
	  }
	  voraussetzungen_.Schreibe(pm);
		rv += pm->SchliesseNachSchreiben();
		return rv; // TODO
	} // Schreibe
	//@nonl
	//@-node:Schreibe
	//@+node:Pruefe
	int SonderfertigkeitDefinition::Pruefe() {
	  int rv = 0;
	  int i = 0;
	//@+at 
	//@nonl
	// Die nächsten erhöhen i um jeweils 1, sofern sie 
	// zutreffen. i darf natürlich nicht größer als 1 
	// werden.
	//@-at
	//@@c  
		//@  << Zaubertanz >>
		//@+node:<< Zaubertanz >>
		if (IstZaubertanz()) {
		  if (extras_.size()!=1) {
		    Log::Info("SonderfertigkeitDefinition::Pruefe: Zaubertanz " + ID() + " hat mehr als eine Repräsentation.");
		    ++rv;
		  }
		  if (!charakteristika()->Existiert(Extra())) {
		    Log::Info("SonderfertigkeitDefinition::Pruefe: Zaubertanz " + ID() + " hat Repräsentation " + Extra() + ", die aber nicht existiert.");
		    std::string s= charakteristika()->Finde(Extra());
		    if (s  != "") {
		      Log::Info("Ist eigentlich " + s + " (wurde korrigiert)");
		      Extra(s);
		    }
		    ++rv;
		  }
		  if ((Extra()!=("CharakteristikumTulamidischeSharisad"))
		   && (Extra()!=("CharakteristikumNovadischeSharisad"))
		   && (Extra()!=("CharakteristikumMajuna"))
		   && (Extra()!=("CharakteristikumHazaqi"))) {
		    Log::Info("SonderfertigkeitDefinition::Pruefe: Zaubertanz " + ID() + " hat Repräsentation " + Extra() + ", die aber unbekannt ist.");
		    ++rv;
		  }
		  ++i;
		}
		//@nonl
		//@-node:<< Zaubertanz >>
		//@nl
		//@  << WaffenloserKampfstil >>
		//@+node:<< WaffenloserKampfstil >>
		if (IstWaffenloserKampfstil()) {
		  if (extras_.size()!=1) {
		    Log::Info("SonderfertigkeitDefinition::Pruefe: Waffenloser Kampfstil " + ID() + " wirkt nicht auf die richtige Anzahl Kampftechniken");
		    ++rv;
		  }
		  if (!kampftechniken()->Existiert(Extra())) {
		    Log::Info("SonderfertigkeitDefinition::Pruefe: Waffenloser Kampfstil " + ID() + " wirkt auf " + Extra() + ", das aber nicht existiert.");
		    std::string s= kampftechniken()->Finde(Extra());
		    if (s  != "") {
		      Log::Info("Ist eigentlich " + s + " (wurde korrigiert)");
		      Extra(s);
		    }
		    ++rv;
		  }
		  ++i;
		}
		//@nonl
		//@-node:<< WaffenloserKampfstil >>
		//@nl
		//@  << WaffenloseSF >>
		//@+node:<< WaffenloseSF >>
		if (IstWaffenloseSonderfertigkeit()) {
		  for (int j = 0; j < extras_.size(); ++j) {
		    if (!sonderfertigkeiten()->Existiert(Extra(j))) {
		      Log::Info("SonderfertigkeitDefinition::Pruefe: Waffenlose Sonderfertigkeit " + ID() + " hat als Stil " + Extra(j) + ", der aber nicht existiert.");
		      std::string s = sonderfertigkeiten()->Finde(Extra(j));
		      if (s  != "") {
		        Log::Info("Ist eigentlich " + s + " (wurde korrigiert)");
		        Extra(s, j);
		      } else {
		        if (Extra(j) == "Gladiatorenstil") {
		          Log::Info("Ist eigentlich Gladiatorenstil (wurde korrigiert)");
		          s = sonderfertigkeiten()->Finde("Gladiatorenstil (Raufen)");
		          Extra(s, j);
		          s = sonderfertigkeiten()->Finde("Gladiatorenstil (Ringen)");
		          Extra(s, 999);   
		        }
		      }
		      ++rv;
		    }
		  }
		  ++i;
		}
		//@nonl
		//@-node:<< WaffenloseSF >>
		//@nl
	  if (i>1) { Log::Info("SonderferigkeitDefinition::Pruefe: Sonderfertigkeit " + ID() + " ist gleichzeitig Waffenloser Kampfstil, Waffenlose Sonderfertigkeit und/oder Zaubertanz."); }
	  if (AP()<0) { Log::Info("SonderferigkeitDefinition::Pruefe: negative AP-Kosten in Sonderfertigkeit " + ID()); }
	  if (GP()<0) { Log::Info("SonderferigkeitDefinition::Pruefe: negative GP-Kosten in Sonderfertigkeit " + ID()); }
	  rv += voraussetzungen_.Pruefe();
		return rv;
	} // Pruefe
	//@nonl
	//@-node:Pruefe
	//@+node:IDListe
	int SonderfertigkeitDefinition::IDListe(PersistenzManager* pm, StringListe* liste) {
		return pm->IDListe(Klasse(), liste); // TODO Prüfen
	} // IDListe
	//@nonl
	//@-node:IDListe
	//@-others
	//@-node:class SonderfertigkeitDefinition
	//@+node:GeneriereSonderfertigkeiten
	void GeneriereSonderfertigkeiten() {
	
	  generierteSonderfertigkeiten()->clear();
	  
		//@  << Scharf- und Meisterschütze >>
		//@+node:<< Scharf- und Meisterschütze >>
		forallconstp (KampftechnikDefinitionen, kt, kampftechniken())
		  if (kt->second.KampfArt()==Fernkampf) {
		    SonderfertigkeitDefinition sfdef;
		    sfdef.IstGeneriert(true);
		    sfdef.Stufe(0);
		    sfdef.AP(300); // TODO Konstante
		    sfdef.GP(6); // TODO Konstante
		    std::string s = "SonderfertigkeitScharfschuetze" + kt->first;
		  
		    Voraussetzung vor;
		    vor.ID(kt->first);
		    vor.MinimalWert(7); // TODO Konstante
		    vor.Minimal(true);
		  
		    sfdef.ID(s);
		    sfdef.voraussetzungen_.clear();
		    sfdef.voraussetzungen_.FuegeEin(vor);
		    generierteSonderfertigkeiten()->FuegeEin(sfdef);
		    
		    Voraussetzung vor2;
		    vor2.ID(s);
		    s = "SonderfertigkeitMeisterschuetze" + kt->first;
		    sfdef.ID(s);
		    vor.MinimalWert(15); // TODO Konstante
		    sfdef.voraussetzungen_.clear();
		    sfdef.voraussetzungen_.FuegeEin(vor);
		    sfdef.voraussetzungen_.FuegeEin(vor2);
		    generierteSonderfertigkeiten()->FuegeEin(sfdef);
		}
		//@-node:<< Scharf- und Meisterschütze >>
		//@nl
		//@  << Repraesentationen >>
		//@+node:<< Repraesentationen >>
		for (int i = 0; i < DSA::AnzahlZauberrepraesentationen; ++i) {
		  SonderfertigkeitDefinition sfdef;
		  sfdef.IstGeneriert(true);
		  sfdef.Stufe(0);
		  sfdef.AP(0); // TODO Konstante
		  sfdef.GP(0); // TODO Konstante
		 std::string s = "SonderfertigkeitRepraesentation" +  DSA::ZauberrepraesentationString((DSA::Zauberrepraesentation)i, true, false);
		  sfdef.ID(s);
		  Voraussetzung vor;
		  vor.ID("CharakteristikumMagiebegabt");
		  sfdef.voraussetzungen_.FuegeEin(vor);
		  vor.ID("CharakteristikumViertelzauberer");
		  vor.Kein(true);
		  sfdef.voraussetzungen_.FuegeEin(vor);
		  generierteSonderfertigkeiten()->FuegeEin(sfdef);   
		}
		//@nonl
		//@-node:<< Repraesentationen >>
		//@nl
		//@  << Ritualkenntnis >>
		//@+node:<< Ritualkenntnis >>
		for (int i = 0; i < DSA::AnzahlZauberrepraesentationen; ++i) {
		 if ((DSA::Zauberrepraesentation)i == Elfen) continue; // es gibt keine elfische RK
		  SonderfertigkeitDefinition sfdef;
		  sfdef.IstGeneriert(true);
		  sfdef.Stufe(0);
		  sfdef.AP(250); // TODO Konstante
		  sfdef.GP(5); // TODO Konstante
		 std::string s = "SonderfertigkeitRitualkenntnis" + DSA::ZauberrepraesentationString((DSA::Zauberrepraesentation)i, true, false);
		  sfdef.ID(s);
		  Voraussetzung vor;
		  vor.ID("CharakteristikumMagiebegabt");
		  sfdef.voraussetzungen_.FuegeEin(vor);
		  vor.ID("CharakteristikumViertelzauberer");
		  vor.Kein(true);
		  sfdef.voraussetzungen_.FuegeEin(vor);
		  generierteSonderfertigkeiten()->FuegeEin(sfdef);   
		}
		//@nonl
		//@-node:<< Ritualkenntnis >>
		//@nl
	  
	}
	//@-node:GeneriereSonderfertigkeiten
	//@+node:class Sonderfertigkeit
	//@+others
	//@+node:Text
	std::string Sonderfertigkeit::Text() const {
	  std::string s = ID();
	  if (Definition()->IstWaffenloserKampfstil())
	    s = _t("Waffenloser Kampfstil") + ": " + s;
	  if (Definition()->IstZaubertanz())
	    s = _t("Zaubertanz") + ": " + s;
	  if (HatStufen())
	    s = s + " " + strings::int2sStr(Stufe());
	  if (!(Spezifizierung()==""))
	    s = s + " (" + Spezifizierung() + ")";
	  return s;
	} // Text
	
	std::string Sonderfertigkeit::Text(Person* p) const {
	  std::string s = Definition()->Text();
	  if (HatStufen())
	    s = s + " " + strings::int2sStr(Stufe());
	  if (!(Spezifizierung()==""))
	    s = s + " (" + Spezifizierung() + ")";
	  if (!Definition()->Hinzufuegbar(*p, true))
	    s += " (" + _t("schlafend") + ")";
	  return s;
	} // Text
	//@-node:Text
	//@+node:ErsetzeTag
	std::string Sonderfertigkeit::ErsetzeTag(const std::string& tag) const {
	  if (""==tag) return Text(); // TODO, um schlafend.
	  if ("Name"==tag) return ID();
	  if ("Beschreibung"==tag) return Definition()->Beschreibung();
	  return tag;
	} // ErsetzeTag
	//@nonl
	//@-node:ErsetzeTag
	//@+node:MaximaleStufe
	int Sonderfertigkeit::MaximaleStufe(const Person& p) const {
	  if (!Definition()->IstRitualkenntnis()) return 0;
	   
	  if (eigeneRK(p))
	    return p.Leiteigenschaftswert()+3;   // TODO magische Konstante
	  else
	    return p.Leiteigenschaftswert();
	}
	//@nonl
	//@-node:MaximaleStufe
	//@+node:eigeneRK
	bool Sonderfertigkeit::eigeneRK(const Person& p) const {
	  forallconst (Sonderfertigkeiten, sf, p.sonderfertigkeiten_)
	    if (sf->second.Definition()->IstRepraesentation())
	      if (Definition()->Ritualkenntnis() == sf->second.Definition()->Repraesentation())
	    return true;
	    
	  if (Definition()->IstRitualkenntnisZaubertaenzer())
	    if (p.charakteristika_.Verfuegt(("CharakteristikumZaubertaenzer")))
	      return true;
	      
	  return false;
	}
	//@-node:eigeneRK
	//@+node:SKTSpalte_
	SKTSpalte Sonderfertigkeit::SKTSpalte_(const Person& p) const {
	  return eigeneRK(p) ? E : G;  // TODO Magische Konstanten
	}
	//@nonl
	//@-node:SKTSpalte_
	//@+node:Kosten
	//@@color
	
	int Sonderfertigkeit::Kosten(const int kosten, const Person& p) const {
	  int punkte = kosten;
	  
		//@  @+others
		//@+node:Akademische Ausbildung
		if (strings::BeginntMit(ID(), ("SonderfertigkeitRuestungsgewoehnung")))
		  if (p.charakteristika_.Existiert(("CharakteristikumAkademischeAusbildungMagier")))
		    punkte = ((punkte*3)+1)/2;
		
		bool akademiebonus = false;
		
		if (p.charakteristika_.Existiert(("CharakteristikumAkademischeAusbildungMagier"))) {
		  forallconst (Voraussetzungen, vor, Definition()->voraussetzungen_) {
		    if ((vor->second.ID()==("SonderfertigkeitRitualkenntnisGildenmagier"))||(vor->second.ID()==("SonderfertigkeitRepraesentationGildenmagier"))) {
		      if (!vor->second.Kein()) {
		        akademiebonus = true;
		        break;
		      }
		    }
		  
		    if (vor->second.OderAnzahl()>0)
		      for (int i = 0; i < vor->second.OderAnzahl(); ++i)
		        if ((vor->second.ID()==("SonderfertigkeitRitualkenntnisGildenmagier"))||(vor->second.ID()==("SonderfertigkeitRepraesentationGildenmagier")))
		          if (!vor->second.Kein())
		            akademiebonus = true;
		  
		    if (akademiebonus)
		      break;
		  }
		
		  if (Definition()->IstMerkmalskenntnis())
		    akademiebonus = true;
		
		  // TODO if (zauberspezialisierung()) akademiebonus = true;
		
		  if ((ID()==("SonderfertigkeitApport"))
		    ||(ID()==("SonderfertigkeitAstraleMeditation"))
		    ||(ID()==("SonderfertigkeitAuraverhuellen"))
		    ||(ID()==("SonderfertigkeitAurapanzer"))
		    ||(ID()==("SonderfertigkeitBlutmagie"))
		    ||(ID()==("SonderfertigkeitDruidenrache"))
		    ||(ID()==("SonderfertigkeitEisernerWilleI"))
		    ||(ID()==("SonderfertigkeitEisernerWilleII"))
		    ||(ID()==("SonderfertigkeitElementarharmonisierteAura"))
		    ||(ID()==("SonderfertigkeitFernzauberei"))
		    ||(ID()==("SonderfertigkeitGedankenschutz"))
		    ||(ID()==("SonderfertigkeitGefaessderSterne"))
		    ||(ID()==("SonderfertigkeitGrosseMeditation"))
		    ||(ID()==("SonderfertigkeitHypervehemenz"))
		    ||(ID()==("SonderfertigkeitKonzentrationsstaerke"))
		    ||(ID()==("SonderfertigkeitKraftkontrolle"))
		    ||(ID()==("SonderfertigkeitKraftlinienmagieI"))
		    ||(ID()==("SonderfertigkeitKraftlinienmagieII"))
		    ||(ID()==("SonderfertigkeitMatrixkontrolle"))
		    ||(ID()==("SonderfertigkeitMatrixregenerationI"))
		    ||(ID()==("SonderfertigkeitMatrixregenerationII"))
		    ||(ID()==("SonderfertigkeitMatrixverstaendnis"))
		    ||(ID()==("SonderfertigkeitMeisterlicheRegeneration"))
		    ||(ID()==("SonderfertigkeitMeisterlicheZauberkontrolle"))
		    ||(ID()==("SonderfertigkeitRegenerationI"))
		    ||(ID()==("SonderfertigkeitRegenerationII"))
		    ||(ID()==("SonderfertigkeitSignaturkenntnis"))
		    ||(ID()==("SonderfertigkeitSimultanzaubern"))
		    ||(ID()==("SonderfertigkeitStapeleffekt"))
		    ||(ID()==("SonderfertigkeitTanzderMada"))
		    ||(ID()==("SonderfertigkeitVerbotenePforten"))
		    ||(ID()==("SonderfertigkeitZauberbereithalten"))
		    ||(ID()==("SonderfertigkeitZauberkontrolle"))
		    ||(ID()==("SonderfertigkeitZauberroutine"))
		    ||(ID()==("SonderfertigkeitZauberunterbrechen"))
		    ||(ID()==("SonderfertigkeitZaubervereinigen"))
		    ||(ID()==("SonderfertigkeitZauberzeichen")))
		    akademiebonus = true;
		    
		  
		
		  if (akademiebonus)
		    punkte = ((punkte*3)+2)/4;
		}
		else if (p.charakteristika_.Existiert(("CharakteristikumAkademischeAusbildungKrieger"))) {
		  if (Definition()->IstWaffenloserKampfstil())
			akademiebonus = true;
		
		  // TODO Vervollständigen (alle Kampfbezogenen SF)
		  if ((ID()==("SonderfertigkeitAufmerksamkeit"))
		    ||(ID()==("SonderfertigkeitAusfall"))
				||(strings::BeginntMit(ID(), ("SonderfertigkeitAusweichen")))
		    ||(ID()==("SonderfertigkeitBefreiungsschlag"))
				||(strings::BeginntMit(ID(), ("SonderfertigkeitBeidhaendigerKampf")))
		    ||(ID()==("SonderfertigkeitBerittenerSchuetze"))
		    ||(ID()==("SonderfertigkeitBetaeubungsschlag"))
			||(ID()==("SonderfertigkeitBinden"))
			||(ID()==("SonderfertigkeitBlindkampf"))
			||(ID()==("SonderfertigkeitDefensiverKampfstil"))
			||(ID()==("SonderfertigkeitDoppelangriff"))
			||(ID()==("SonderfertigkeitEisenhagel"))
			||(ID()==("SonderfertigkeitEntwaffnen"))
			||(ID()==("SonderfertigkeitFinte"))
			||(ID()==("SonderfertigkeitFormation"))
			||(ID()==("SonderfertigkeitGegenhalten"))
			||(ID()==("SonderfertigkeitGezielterStich"))
			||(ID()==("SonderfertigkeitHammerschlag"))
			||(ID()==("SonderfertigkeitImprovisierteWaffen"))
			||(ID()==("SonderfertigkeitKampfimWasser"))
			||(ID()==("SonderfertigkeitKampfgespuer"))
			||(ID()==("SonderfertigkeitKampfreflexe"))
			||(ID()==("SonderfertigkeitKlingensturm"))
			||(ID()==("SonderfertigkeitKlingenwand"))
			||(ID()==("SonderfertigkeitKriegsreiterei"))
			||(ID()==("SonderfertigkeitLinkhand"))
			||(ID()==("SonderfertigkeitMeisterlichesEntwaffnen"))
			||(ID()==("SonderfertigkeitMeisterparade"))
			||(strings::BeginntMit(ID(), ("SonderfertigkeitMeisterschuetze")))
			||(ID()==("SonderfertigkeitNiederwerfen"))
				||(strings::BeginntMit(ID(), ("SonderfertigkeitParierwaffen")))
			||(ID()==("SonderfertigkeitReiterkampf"))
				||(strings::BeginntMit(ID(), ("SonderfertigkeitRuestungsgewoehnung")))
			||(strings::BeginntMit(ID(), ("SonderfertigkeitScharfschuetze")))
				||(strings::BeginntMit(ID(), ("SonderfertigkeitSchildkampf")))
			||(ID()==("SonderfertigkeitSchildspalter"))
				||(strings::BeginntMit(ID(), ("SonderfertigkeitSchnellladen")))
			||(ID()==("SonderfertigkeitSchnellziehen"))
			||(ID()==("SonderfertigkeitSpiessgespann"))
			||(ID()==("SonderfertigkeitSturmangriff"))
			||(ID()==("SonderfertigkeitTodvonLinks"))
			||(ID()==("SonderfertigkeitTodesstoss"))
			||(ID()==("SonderfertigkeitTurnierreiterei"))
			||(ID()==("SonderfertigkeitUmreissen"))
			||(ID()==("SonderfertigkeitUnterwasserkampf"))
			||(ID()==("SonderfertigkeitWaffezerbrechen"))
		//TODO			||(ID()==("SonderfertigkeitWaffenspezialisierung"))
			||(ID()==("SonderfertigkeitWindmuehle"))
			||(ID()==("SonderfertigkeitWuchtschlag"))
			)
		    akademiebonus = true;
		    
		  
		
		  if (akademiebonus)
		    punkte = ((punkte*3)+2)/4;
		}
		//@-node:Akademische Ausbildung
		//@+node:Astrale Meditation
		if (ID() == ("SonderfertigkeitAstraleMeditation"))
		  if (p.charakteristika_.Existiert(("CharakteristikumHalbzauberer")))
		    punkte *= 2;
		//@-node:Astrale Meditation
		//@+node:Aura verhüllen
		if (ID() == ("SonderfertigkeitAuraverhuellen"))
		  if (p.charakteristika_.Existiert(("CharakteristikumSchwacheAusstrahlung")))
		    punkte = (punkte+1)/2;
		//@nonl
		//@-node:Aura verhüllen
		//@+node:Blutmagie
		if (ID() == ("SonderfertigkeitBlutmagie"))
		  if (p.charakteristika_.Existiert(("CharakteristikumViertelzauberer")))
		    punkte *= 2;
		//@-node:Blutmagie
		//@+node:Druidenrache
		if (ID() == ("SonderfertigkeitDruidenrache"))
		  if (p.sonderfertigkeiten_.Existiert(("SonderfertigkeitVerbotenePforten")))
		    punkte = (punkte+1)/2;
		
		//@-node:Druidenrache
		//@+node:Regeneration II
		if (ID() == ("SonderfertigkeitRegenerationII"))
		  if (p.charakteristika_.Existiert(("CharakteristikumHalbzauberer")))
		    punkte *= 2;
		//@-node:Regeneration II
		//@+node:Meisterliche Regeneration
		if (ID() == ("SonderfertigkeitMeisterlicheRegeneration"))
		  if (p.charakteristika_.Existiert(("CharakteristikumHalbzauberer")))
		    punkte *= 2;
		//@-node:Meisterliche Regeneration
		//@+node:Kraftkontrolle
		if (ID() == ("SonderfertigkeitKraftkontrolle"))
		  if (p.sonderfertigkeiten_.Existiert(("SonderfertigkeitMerkmalskenntnisKraft")))
		    punkte = (punkte+1)/2;
		//@-node:Kraftkontrolle
		//@+node:Kraftlinienmagie II
		if (ID() == ("SonderfertigkeitKraftlinienmagieII"))
		  if ((p.sonderfertigkeiten_.Existiert(("SonderfertigkeitMerkmalskenntnisKraft")))
		    ||(p.sonderfertigkeiten_.Existiert(("SonderfertigkeitMerkmalskenntnisHellsicht")))
		    ||(p.sonderfertigkeiten_.Existiert(("SonderfertigkeitMerkmalskenntnisMetamagie"))))    
		    punkte = (punkte+1)/2;
		//@-node:Kraftlinienmagie II
		//@+node:Matrixregeneration II
		if (ID() == ("SonderfertigkeitMatrixregenerationII"))
		  if (p.sonderfertigkeiten_.Existiert(("SonderfertigkeitMerkmalskenntnisKraft")))
		    punkte = (punkte+1)/2;
		//@-node:Matrixregeneration II
		//@+node:Zauber bereithalten
		if (ID() == ("SonderfertigkeitZauberbereithalten"))
		  if (p.sonderfertigkeiten_.Existiert(("SonderfertigkeitMerkmalskenntnisMetamagie")))
		    punkte = (punkte+1)/2;
		//@-node:Zauber bereithalten
		//@+node:Zauber unterbrechen
		if (ID() == ("SonderfertigkeitZauberunterbrechen"))
		  if (p.sonderfertigkeiten_.Existiert(("SonderfertigkeitMerkmalskenntnisMetamagie")))
		    punkte = (punkte+1)/2;
		//@-node:Zauber unterbrechen
		//@+node:Zauber vereinigen
		if (ID() == ("SonderfertigkeitZaubervereinigen"))
		  if (p.sonderfertigkeiten_.Existiert(("SonderfertigkeitMerkmalskenntnisMetamagie")))
		    punkte = (punkte+1)/2;
		//@-node:Zauber vereinigen
		//@+node:Manöver ohne Stil teurer
		if (Definition()->IstWaffenloseSonderfertigkeit()) {
		  bool hatStil = false;
		  StringListe stile = Definition()->Extras();
		  for (StringListe::iterator sli = stile.begin(); sli != stile.end(); ++sli)
		    if (p.sonderfertigkeiten_.Existiert(*sli))
		      hatStil = true;
		  if (!hatStil) punkte *= 5;
		}
		//@-node:Manöver ohne Stil teurer
		//@+node:die zweite Geländekunde gibt es für 2/3 des Preises!
		bool zweiteGelaendekunde = false;
		if (Definition()->IstGelaendekunde())
		  for (Sonderfertigkeiten::const_iterator i = p.sonderfertigkeiten_.begin(); i != p.sonderfertigkeiten_.end(); ++i)
		    if (i->second.Definition()->IstGelaendekunde())
		      if (i->first != ID())
		        zweiteGelaendekunde = true;
		if (zweiteGelaendekunde)
		  punkte = ((2*punkte)+1)/3;
		//@nonl
		//@-node:die zweite Geländekunde gibt es für 2/3 des Preises!
		//@+node:Schlangenmensch-Bonus: Ausweichen SF kosten nur die Hälfte
		if (Definition()->NurID()==("SonderfertigkeitAusweichen"))
		  if (p.charakteristika_.Existiert(("CharakteristikumSchlangenmensch")))
		    punkte = (punkte+1)/2;
		//@nonl
		//@-node:Schlangenmensch-Bonus: Ausweichen SF kosten nur die Hälfte
		//@+node:Beidhändig-Boni
		if (p.charakteristika_.Existiert(("CharakteristikumBeidhaendig"))) {
		  if ((Definition()->NurID()==("SonderfertigkeitBeidhaendigerKampf"))
		    ||(Definition()->NurID()==("SonderfertigkeitLinkhand")))
		    punkte = (punkte+1)/2;
		  if ((Definition()->NurID()==("SonderfertigkeitSchildkampf"))
		    ||(Definition()->NurID()==("SonderfertigkeitParierwaffen"))
		    ||(Definition()->NurID()==("SonderfertigkeitTodvonLinks"))
		    ||(Definition()->NurID()==("SonderfertigkeitDoppelangriff")))
		    punkte = ((3*punkte)+2)/4;
		}
		//@nonl
		//@-node:Beidhändig-Boni
		//@+node:Linkshänder-Boni
		if (p.charakteristika_.Existiert(("SonderfertigkeitLinkshaender")))
		  if ((Definition()->NurID()==("SonderfertigkeitBeidhaendigerKampf"))
		    ||(Definition()->NurID()==("SonderfertigkeitLinkhand")))
		    punkte = ((3*punkte)+2)/4;
		//@nonl
		//@-node:Linkshänder-Boni
		//@+node:Zaubertänze anderer Repräsentationen kosten das Anderthalbfache
		if (Definition()->IstZaubertanz())
		  if (!p.charakteristika_.Existiert(Definition()->Extra()))
		    punkte = ((3*punkte)+1)/2;
		//@nonl
		//@-node:Zaubertänze anderer Repräsentationen kosten das Anderthalbfache
		//@+node:verbilligte SF
		for (int j = 0; j != p.verbilligteSonderfertigkeiten_.size(); ++j)
		  if (p.verbilligteSonderfertigkeiten_[j]==ID())
		    punkte = (punkte+1)/2;
		//@-node:verbilligte SF
		//@-others
	  
	  return punkte;
	} // Kosten
	//@-node:Kosten
	//@+node:GPKosten
	int Sonderfertigkeit::GPKosten(const Person& p) const {
	
	  if (Definition()->IstRepraesentation())
	    return Kosten(DynamischeKosten(p), p);
	
	  return Kosten(Definition()->GP(), p);
	} // GPKosten
	//@nonl
	//@-node:GPKosten
	//@+node:APKosten
	int Sonderfertigkeit::APKosten(const Person& p) const {
	
	  if (Definition()->IstRepraesentation())
	    return Kosten(DynamischeKosten(p)*50, p);
	
	  return Kosten(Definition()->AP(), p);
	} // APKosten
	//@nonl
	//@-node:APKosten
	//@+node:DynamischeKosten
	int Sonderfertigkeit::DynamischeKosten(const Person& p) const {
	
	  if (Definition()->IstRepraesentation()) {
	    int repnummer = 0;
	    for (Sonderfertigkeiten::const_iterator i = p.sonderfertigkeiten_.begin(); i != p.sonderfertigkeiten_.end(); ++i)
	      if (i->second.Definition()->IstRepraesentation()) {
	        ++repnummer;
	        if (i->first == ID()) break;
	      }
	
	    if (repnummer == 1)
	      return 0; // TODO Konstante, hier wie auch im weiteren
	      // Erste Rep gibts umsonst
	    if (repnummer == 2)
	      if (p.charakteristika_.Existiert(("CharakteristiukmVollzauberer")))
	        return 40; // Zweite Rep, entspricht 2000 AP
	      else
	        return 60; // Zweite Rep für Halbzauberer, entspricht 3000 AP
	    if (repnummer == 3)
	      return 80; // Dritte Rep, entspricht 4000 AP
	  }
	  
	  return 0;
	}
	//@nonl
	//@-node:DynamischeKosten
	//@+node:Definition
		const SonderfertigkeitDefinition* Sonderfertigkeit::Definition() const {
			SonderfertigkeitDefinition* def = sonderfertigkeiten()->Zeige(id_);
			if (def!=0) return def;
	    def = generierteSonderfertigkeiten()->Zeige(id_);
			if (def!=0) return def;
			Log::Error("Sonderfertigkeit::Definition: Undefinierte Sonderfertigkeit " + id_);
			static SonderfertigkeitDefinition* sdef = new SonderfertigkeitDefinition();
			return sdef; // TODO workaround. Sollte bei nicht vorhandenen Defs intelligenter
			// reagieren (doch wie?) // TODO Speicherleck
		} // Definition
	//@-node:Definition
	//@+node:Lese
	int Sonderfertigkeit::Lese(PersistenzManager* pm, const std::string& id) {
		std::string geleseneID = id;
		int rv = pm->OeffneZumLesen(Klasse(), geleseneID);
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
	int Sonderfertigkeit::Schreibe(PersistenzManager* pm) const {
		int rv = pm->OeffneZumSchreiben(Klasse(), ID());
		if (HatStufen()) rv += pm->Schreibe(("Stufe"), Stufe());
	  if (Spezifizierung()!="")
	    rv += pm->Schreibe(("Spezifizierung"), Spezifizierung());
		rv += pm->SchliesseNachSchreiben();
		return rv;
	} // Schreibe
	//@nonl
	//@-node:Schreibe
	//@+node:Pruefe
	int Sonderfertigkeit::Pruefe() {
	  int rv = 0;
	  if ((!sonderfertigkeiten()->Existiert(ID()))&&(!generierteSonderfertigkeiten()->Existiert(ID()))) {
	    Log::Info("Sonderfertigkeit::Pruefe: unbekannte Sonderfertigkeit " + ID());
	    std::string s = sonderfertigkeiten()->Finde(ID());
	    if (s != "") {
	      Log::Info("ist eigentlich " + s + " (wurde korrigiert)");
	      ID(s);
	    }
	    s = generierteSonderfertigkeiten()->Finde(ID());
	    if (s != "") {
	      Log::Info("ist eigentlich " + s + " (wurde korrigiert)");
	      ID(s);
	    }
	    ++rv;
	  }
	  if (HatStufen())
	    if (!Definition()->IstRitualkenntnis()) {
	      Log::Info("Sonderfertigkeit::Pruefe: Stufe bei stufenloser Sonderfertigkeit " + ID());
	      ++rv;
	  }
	//@+at 
	//@nonl
	// kann so nicht geprüft werden, da Charakteristika in 
	// RKP durchaus leer sein dürfen
	// if (def->BrauchtSpezifizierung())
	//   if (Spezifizierung()=="") {
	//     Log::Info(_t("leere Spezifizierung"));
	//     ++rv;
	// }
	//@-at
	//@@c
		return rv;
	} // Pruefe
	
	//@-node:Pruefe
	//@+node:IDListe
	int Sonderfertigkeit::IDListe(PersistenzManager* pm, StringListe* liste) {
		return pm->IDListe(Klasse(), liste); // TODO Prüfen
	} // IDListe
	//@nonl
	//@-node:IDListe
	//@-others
	//@nonl
	//@-node:class Sonderfertigkeit
	//@+node:class Sonderfertigkeiten
	
	Sonderfertigkeiten Sonderfertigkeiten::Bereinige() const {
	  Sonderfertigkeiten sfs;
	  Sonderfertigkeit sf;
	  for (const_iterator i = begin(); i != end(); ++i) {
	    if (((sonderfertigkeiten()->Existiert(sf.ID()))
	       ||(generierteSonderfertigkeiten()->Existiert(sf.ID())))
	      &&((sonderfertigkeiten()->Existiert(i->second.ID()))
	       ||(generierteSonderfertigkeiten()->Existiert(i->second.ID()))))
	      if (i->second.Definition()->NurID()!=sf.Definition()->NurID())
	        sfs.FuegeEin(sf);
	    sf = i->second;
	  }
	  if (size()!=0)
	    sfs.FuegeEin(sf);
	  return sfs;
	} // Bereinige
	//@nonl
	//@-node:class Sonderfertigkeiten
	//@-others
  
  SonderfertigkeitDefinitionen* sonderfertigkeiten() {
    static SonderfertigkeitDefinitionen* sonderfertigkeiten_ = new SonderfertigkeitDefinitionen();
    return sonderfertigkeiten_;
  }
    
	SonderfertigkeitDefinitionen* generierteSonderfertigkeiten() {
    static SonderfertigkeitDefinitionen* sonderfertigkeiten_ = new SonderfertigkeitDefinitionen();
    return sonderfertigkeiten_;
  } // generierteCharakteristika
    
} // DSA

//@<< Version History >>
//@+node:<< Version History >>
// $Log: Sonderfertigkeit.cpp,v $
// Revision 1.28  2004/10/17 19:12:40  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.27  2004/04/25 17:32:11  gnaddelwarz
// - "Akademische Ausbildung (Krieger)" hinzugefügt
//
// Revision 1.26  2004/04/23 22:07:41  vrandezo
// diverse SF hinzugefuegt
//
// Revision 1.25  2004/04/13 21:45:45  vrandezo
// Akademische Ausbildung (Magier) implementiert
// Aufräumarbeiten (besser in Leo integriert)
//
// Revision 1.24  2004/04/12 13:20:23  vrandezo
// Merkmalskenntnisse implementiert
//
// Revision 1.23  2004/04/12 01:44:13  vrandezo
// Astrale Regneration
// Vorbereitung auf Merkmalskenntnisse
//
// Revision 1.22  2004/04/11 21:13:06  vrandezo
// Voraussetzungen koennen jetzt mit "oder" verknuepft werden
//
// Revision 1.21  2004/04/11 01:32:50  vrandezo
// generiere Ritualkenntnisse
// Test auf "schlafend" verbessert
//
// Revision 1.20  2004/04/10 22:38:02  vrandezo
// SF Repraesentation
//
// Revision 1.19  2004/01/07 23:07:06  vrandezo
// RKP können jetzt [eine Geländekunde] haben
//
// Revision 1.18  2003/11/28 16:45:37  vrandezo
// Manöver
//
// Revision 1.17  2003/10/30 00:02:44  vrandezo
// Steigerbare Ritualkenntnisse
//
// Revision 1.16  2003/10/27 18:24:50  vrandezo
// unsichtbare Vor- und Nachteile
// Tänze fremder Repräsentationen teurer
//
// Revision 1.15  2003/10/27 01:24:53  vrandezo
// Zaubertänze
//
// Revision 1.14  2003/10/26 23:16:08  vrandezo
// RK hinzufügbar zu RKP und steigerbar
//
// Revision 1.13  2003/10/25 22:33:27  vrandezo
// SF-Text
//
// Revision 1.12  2003/10/25 02:11:18  vrandezo
// generierte Sonderfertigkeiten
//
// Revision 1.11  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.10  2003/03/25 18:42:38  vrandezo
// Kampftechniken in der Kompaktübersicht mit AT/PA statt +AT/+PA
// Bug: Sonderfertigkeiten unabhängig von alphabetischer Reihenfolge
// Sonderfertigkeiten: Binden, Entwaffnen, Meisterliches Entwaffnen, Waffe zerbrechen
//
// Revision 1.9  2003/03/09 15:25:34  vrandezo
// Waffenlose Kampfstile
// Aufgeteilte Editoren für Waffenlose Kampfstile und sonstige SF
// Voraussetzungen können auch auf Kampftechniken sein
//
// Revision 1.8  2003/03/04 15:23:19  vrandezo
// weitere Tags implementiert für die Druckvorlagen
//
// Revision 1.7  2003/02/24 17:37:42  vrandezo
// Prüfen implementiert
//
// Revision 1.6  2003/02/20 13:53:22  vrandezo
// Vorteile: Beidhändig, Linkshänder, Blutrausch, Kampfrausch
// weitere SF
// Eigenschaften haben Maximalwerte
//
// Revision 1.5  2003/02/20 09:19:45  vrandezo
// Prüfe-Signatur geändert
//
// Revision 1.4  2003/02/20 00:21:58  vrandezo
// Sonderfertigkeiten vollständig integriert
// schlafende SF
//
// Revision 1.3  2003/02/19 17:45:14  vrandezo
// Kulturen zeigen mögliche und übliche Rassen an,
// Professionen mögliche Kulturen
//
// Revision 1.2  2003/02/18 18:48:48  vrandezo
// SF bei Generierung
// Geländekunden außer der ersten für nur 2/3 des Preises
//
// Revision 1.1.1.1  2003/02/17 15:34:18  vrandezo
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
// Revision 1.1  2003/02/14 21:42:50  vrandezo
// Sonderfertigkeiten hinzugefügt
//
//@nonl
//@-node:<< Version History >>
//@nl
//@-node:@file Sonderfertigkeit.cpp
//@-leo
