//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file Kampftechnik.cpp
//@+at
// Kampftechnik.cpp: Implementierung von Kampftechnik.
// 
// $Revision: 1.17 $
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
#include "Kampftechnik.h"

#include "Texte.h"
#include "Log.h"
#include "NDSAUtil.h"
#include "Person.h"
#include "PersistenzManager.h"
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

	//@  << KampftechnikDefinition >>
	//@+node:<< KampftechnikDefinition >>
	//@+others
	//@+node:Lese
	int KampftechnikDefinition::Lese(PersistenzManager* pm, const std::string& id) {
		std::string geleseneID = id;
		int rv = pm->OeffneZumLesen(Klasse(), geleseneID);
	  Dict bezeichnung;
	  Dict beschreibung;
	  rv += LeseBezeichnung(pm, geleseneID, bezeichnung, beschreibung);
		std::string sBuf;
		rv += pm->Lese("Art", sBuf);
		Talentart art = TalentartString(sBuf);
		int rv0 = pm->Lese("SKT-Spalte", sBuf);
		if (rv0 != 0) sBuf = "D";
		SKTSpalte skt = SKTSpalteString(sBuf);
	  if (pm->Existiert("eBE"))
	    rv += pm->Lese("eBE", sBuf);
	  else
	    sBuf = "-";
	  EBe eBE = EBeString(sBuf);
	
		if (!rv) {
			ID(geleseneID);
	 bezeichnung_ = bezeichnung;
	 beschreibung_ = beschreibung;
			Art(art);
			SKTSpalte_(skt);
	    EffektiveBehinderung(eBE);
		}
		rv += pm->SchliesseNachLesen();
		return rv; // TODO
	} // Lese
	//@-node:Lese
	//@+node:Schreibe
	int KampftechnikDefinition::Schreibe(PersistenzManager* pm) const {
		int rv = pm->OeffneZumSchreiben(Klasse(), ID());
	  rv += SchreibeBezeichnung(pm);
		rv += pm->Schreibe("Art", TalentartString(Art(), false));
		rv += pm->Schreibe("SKT-Spalte", SKTSpalteString(SKTSpalte_()));
	  if (EffektiveBehinderung()!=KeineBE)
	    pm->Schreibe("eBE", EBeString(EffektiveBehinderung(), false));
		rv += pm->SchliesseNachSchreiben();
		return rv;
	} // Schreibe
	//@-node:Schreibe
	//@+node:Pruefe
	int KampftechnikDefinition::Pruefe() {
	  int rv = 0; // Hier kann schlicht nichts schiefgehen, oder?
	  if (rv!=0) Log::Info("in " + Klasse() + " " + ID());
		return rv;
	} // Pruefe
	//@nonl
	//@-node:Pruefe
	//@+node:IDListe
	int KampftechnikDefinition::IDListe(PersistenzManager* pm, StringListe* liste) {
		return pm->IDListe(Klasse(), liste); // TODO Prüfen
	} // IDListe
	//@nonl
	//@-node:IDListe
	//@-others
	
	
	//@-node:<< KampftechnikDefinition >>
	//@nl
  
	KampftechnikDefinitionen* kampftechniken() 
		{ static KampftechnikDefinitionen* kampftechniken_ = new KampftechnikDefinitionen(); return kampftechniken_; }
        
	//@  << Kampftechnik >>
	//@+node:<< Kampftechnik >>
	//@+others
	//@+node:SKTSpalte_
	SKTSpalte Kampftechnik::SKTSpalte_() const {
	  if (person_==0)
	    return Definition()->SKTSpalte_();
	  SKTSpalte s = Definition()->SKTSpalte_();
	 forallconst(Charakteristika, ch, person_->charakteristika_) {
	    if (ch->second.Definition()->IstBegabungTalent()) 
	      if (ch->second.Definition()->BegabungTalent()==ID())
	        s = SpalteLeichter(s);
	    if (ch->second.Definition()->IstBegabungTalentgruppe()) 
	      if (ch->second.Definition()->BegabungTalentgruppe()==Kampf)
	        s = SpalteLeichter(s);
	    if (ch->second.Definition()->IstUnfaehigkeitTalent()) 
	      if (ch->second.Definition()->UnfaehigkeitTalent()==ID())
	        s = SpalteSchwerer(s);
	    if (ch->second.Definition()->IstUnfaehigkeitTalentgruppe()) 
	      if (ch->second.Definition()->UnfaehigkeitTalentgruppe()==Kampf)
	        s = SpalteSchwerer(s);
	  }
	  return s;
	} // SKTSpalte_
	
	//@-node:SKTSpalte_
	//@+node:Maximalwert
	int Kampftechnik::Maximalwert() const {
	  if (person_==0) return 0;
	
	  if (regeln()->Regeloption("Begabungen und Unfähigkeiten nach Errata"))
	    if ((person_->charakteristika_.Existiert(_t("Unfähigkeit für") + " " + ID()))
	      ||(person_->charakteristika_.Existiert(_t("Unfähigkeit für") + " " + TalentgruppeString(Kampf, false))))
	      return min(person_->Wert(Gewandtheit),
	                 person_->Wert(Koerperkraft));
	
	  int maxwert = max(person_->Wert(Gewandtheit),
	                    person_->Wert(Koerperkraft));
	  maxwert += 3; // TODO Konstante
	        
	  if (regeln()->Regeloption("Begabungen und Unfähigkeiten nach Errata"))
	    if ((person_->charakteristika_.Existiert(_t("Begabung für") + " " + ID()))
	      ||(person_->charakteristika_.Existiert(_t("Begabung für") + " " + TalentgruppeString(Kampf, false))))
	      maxwert += 2;
	  
	  return maxwert;
	} // Maximalwert
	//@nonl
	//@-node:Maximalwert
	//@+node:Extra
	void Kampftechnik::Extra(const std::string& extra) {
	  Log::Error("Kampftechnik::Extra: Unbekanntes Extra " + extra);
	}
	//@nonl
	//@-node:Extra
	//@+node:Text
	std::string Kampftechnik::Text(const bool mitATPA) const {
	  if (kampftechniken()->Zeige(id_)==0) return ID();
	  std::string s = ID() + " " + strings::int2sStr(Wert(), true);
	  if ((KampfArt()==Fernkampf)||(KampfArt()==BewaffneterKampf)) {
	    if ((mitATPA)&&(person_!=0))
	      return s + " (" + _t("AT") + " " + strings::int2sStr(Attacke(), false) + ")";
	    else
	      return s;
	  }
	  if (mitATPA)
	    if (person_==0)
	      s += " (" + _t("AT") + "/" + _t("PA") + " " + strings::int2sStr(AufAT(), true) + "/" + strings::int2sStr(AufPA(), true) + ")";
	    else
	      s += " (" + _t("AT") + "/" + _t("PA") + " " + strings::int2sStr(Attacke(), false) + "/" + strings::int2sStr(Parade(), false) + ")";
		return s;
	} // Text
	//@nonl
	//@-node:Text
	//@+node:Definition
	const KampftechnikDefinition* Kampftechnik::Definition() const {
		KampftechnikDefinition* def = kampftechniken()->Zeige(id_);
		if (def!=0) return def;
		Log::Error("Kampftechnik::Definition: " + _t("Undefinierte Kampftechnik") + " " + id_);
		static KampftechnikDefinition* sdef = new KampftechnikDefinition();
		return sdef; // TODO workaround. Sollte bei nicht vorhandenen Defs intelligenter
		// reagieren (doch wie?) // TODO Speicherleck
	} // Definition
	//@nonl
	//@-node:Definition
	//@+node:Wert
	void Kampftechnik::Wert(int wert) {
	  int altAufAT = AufAT();
	  int altAufPA = AufPA();
	  int altWert = Wert();
	  wert_ = wert;
	  if (AutoAufteilenATPA()) return;
	  if (Wert() > altWert)
	    if (altAufAT <= altAufPA)
	      AufAT(AufAT()+1);
	  if (Wert() < altWert)
	    if (altAufAT > altAufPA)
	      AufAT(AufAT()-1);
	} // Wert
	//@nonl
	//@-node:Wert
	//@+node:WaereOKAufAT
	bool Kampftechnik::WaereOKAufAT(int aufAT) const {
	  if ((KampfArt()==Fernkampf)||(KampfArt()==BewaffneterKampf)) return false;
	  if (Wert()<1) return false;
	  if (aufAT<0) return false;
	  if (aufAT>Wert()) return false;
	  int aufPA = Wert()-aufAT;
	  if ((aufPA-aufAT)>regeln("Maximale Differenz AT/PA")) return false;
	  if ((aufAT-aufPA)>regeln("Maximale Differenz AT/PA")) return false;
	  return true;
	} // WaereOKAufAT
	//@nonl
	//@-node:WaereOKAufAT
	//@+node:WaereOKAufPA
	bool Kampftechnik::WaereOKAufPA(int aufPA) const {
	  return WaereOKAufAT(Wert()-aufPA);
	} // WaereOKAufAT
	//@nonl
	//@-node:WaereOKAufPA
	//@+node:Attacke
	int Kampftechnik::Attacke() const {
	  if (person_==0) { Log::Error(_t("Kann keine Attacke bestimmen, keine Person zugeordnet")); return 0; }
	  int verbesserung = 0;
	  if (Definition()->KampfArt()==WaffenloserKampf) {
	    for (DSA::Sonderfertigkeiten::iterator sf = person_->sonderfertigkeiten_.begin(); sf != person_->sonderfertigkeiten_.end(); ++sf)
	      if (sf->second.Definition()->IstWaffenloserKampfstil())
	        if (sf->second.Definition()->Extra()==ID()) // TODO Verbessert = NachWahl?
	          ++verbesserung;
	    if (verbesserung>2) verbesserung=2; // TODO Konstante, MBK 50
	  }
	  if (Definition()->KampfArt() == Fernkampf)
	    return person_->Wert(FernkampfBasis) + AufAT() + verbesserung;
	  else
	    return person_->Wert(AttackeBasis) + AufAT() + verbesserung;
	} // Attacke
	//@nonl
	//@-node:Attacke
	//@+node:HatParade
	bool Kampftechnik::HatParade() const {
	  return ((Definition()->KampfArt()==BewaffneterNahkampf)||(Definition()->KampfArt()==WaffenloserKampf));
	} // HatParade
	//@nonl
	//@-node:HatParade
	//@+node:Parade
	int Kampftechnik::Parade() const {
	  if (person_==0) { Log::Error(_t("Kann keine Parade bestimmen, keine Person zugeordnet")); return 0; }
	  int verbesserung = 0;
	  if (Definition()->KampfArt()==WaffenloserKampf) {
	    for (DSA::Sonderfertigkeiten::iterator sf = person_->sonderfertigkeiten_.begin(); sf != person_->sonderfertigkeiten_.end(); ++sf)
	      if (sf->second.Definition()->IstWaffenloserKampfstil())
	        if (sf->second.Definition()->Extra()==ID()) // TODO Verbessert = NachWahl?
	          ++verbesserung;
	    if (verbesserung>2) verbesserung=2; // TODO Konstante, MBK 50
	  }
	  if (HatParade())
	    return person_->Wert(ParadeBasis) + AufPA() + verbesserung;
	  else
	    return 0;
	} // Parade
	//@nonl
	//@-node:Parade
	//@+node:AufAT
	void Kampftechnik::AufAT(int aufAT) {
	  autoAufteilenATPA_ = true;
	  if (!WaereOKAufAT(aufAT)) return;
	  aufAT_ = aufAT;
	  autoAufteilenATPA_ = false;
	} // AufAT
	
	int Kampftechnik::AufAT() const {
	  if ((KampfArt()==Fernkampf)||(KampfArt()==BewaffneterKampf)) return Wert();
	  if (AutoAufteilenATPA())
	    return (wert_+1)/2;
	  else
	    return aufAT_;
	} // AufAT
	//@-node:AufAT
	//@+node:AufPA
	void Kampftechnik::AufPA(int aufPA) {
	  AufAT(Wert()-aufPA);
	} // AufPA
	
	int Kampftechnik::AufPA() const {
	  return Wert()-AufAT();
	} // AufPA
	//@nonl
	//@-node:AufPA
	//@+node:AutoAufteilenATPA
	void Kampftechnik::AutoAufteilenATPA(bool autoAufteilenATPA) {
	  autoAufteilenATPA_ = autoAufteilenATPA;
	  if ((KampfArt()==Fernkampf)||(KampfArt()==BewaffneterKampf)) autoAufteilenATPA_ = true;
	} // AutoAufteilenATPA
	
	bool Kampftechnik::AutoAufteilenATPA() const {
	  return autoAufteilenATPA_;
	} // AutoAufteilenATPA
	//@nonl
	//@-node:AutoAufteilenATPA
	//@+node:ErsetzeTag
	std::string Kampftechnik::ErsetzeTag(const std::string& tag) const {
	  if (""==tag) return Text();
	  if ("Name"==tag) return ID();
	  if ("W"==tag) return strings::int2sStr(Wert(), true);
	  if ("aAT"==tag) return strings::int2sStr(AufAT(), true);
	  if ("aPA"==tag) if ((KampfArt()==Fernkampf)||(KampfArt()==BewaffneterKampf)) return ""; else return strings::int2sStr(AufPA(), true);
	  if ("AT"==tag) return strings::int2sStr(Attacke());
	  if ("PA"==tag) if ((KampfArt()==Fernkampf)||(KampfArt()==BewaffneterKampf)) return ""; else return strings::int2sStr(Parade());
	  if ("SKT"==tag) return SKTSpalteString(SKTSpalte_());
	  if ("Beschreibung"==tag) return Definition()->Beschreibung();
	  return tag;
	} // ErsetzeTag
	//@nonl
	//@-node:ErsetzeTag
	//@+node:Lese
	int Kampftechnik::Lese(PersistenzManager* pm, const std::string& id) {
		std::string geleseneID = id;
		int rv = pm->OeffneZumLesen(Klasse(), geleseneID);
		int wert;
		rv += pm->Lese(("Wert"), wert);
	  int aufAT;
	  bool autoATPA = !(pm->Existiert(("AufAT")));
	  if (!autoATPA)
	    rv += pm->Lese(("AufAT"), aufAT);
		rv += pm->SchliesseNachLesen();
	
		if (!rv) {
	    person_;
			ID(geleseneID);
			Wert(wert);
	    AutoAufteilenATPA(autoATPA);
	    if (!AutoAufteilenATPA()) AufAT(aufAT);
		}
		return rv; // TODO
	} // Lese
	//@nonl
	//@-node:Lese
	//@+node:Schreibe
	int Kampftechnik::Schreibe(PersistenzManager* pm) const {
		int rv = pm->OeffneZumSchreiben(Klasse(), ID());
		rv += pm->Schreibe(("Wert"), Wert());
	  rv += pm->Schreibe(("AufAT"), AufAT());
	  if (HatParade()) rv += pm->Schreibe(("AufPA"), AufPA());
	  if (person_!=0) {
	    rv += pm->Schreibe(("Attacke"), Attacke());
	    if (HatParade()) rv += pm->Schreibe(("Parade"), Parade());
	  }
	  rv += pm->SchliesseNachSchreiben();
		return rv;
	} // Schreibe
	//@nonl
	//@-node:Schreibe
	//@+node:Pruefe
	int Kampftechnik::Pruefe() {
	  int rv = 0;
	  if (!kampftechniken()->Existiert(ID())) {
	    Log::Info("Talent::Pruefe: unbekannte Kampftechnik " + ID());
	    std::string s = kampftechniken()->Finde(ID());
	    if (s != "") {
	      Log::Info("ist eigentlich " + s + " (wurde korrigiert)");
	      ID(s);
	    }
	    ++rv;
	  }
	  if (rv!=0) Log::Info(" in " + Klasse() + " " + ID());
		return rv;
	} // Pruefe
	//@-node:Pruefe
	//@+node:IDListe
	int Kampftechnik::IDListe(PersistenzManager* pm, StringListe* liste) {
		return pm->IDListe(Klasse(), liste); // TODO Prüfen
	} // IDListe
	//@nonl
	//@-node:IDListe
	//@-others
	//@nonl
	//@-node:<< Kampftechnik >>
	//@nl
  
}

//@<< Version History >>
//@+node:<< Version History >>
// $Log: Kampftechnik.cpp,v $
// Revision 1.17  2004/10/17 19:12:39  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.16  2004/04/13 14:06:52  vrandezo
// Hauszauber implementiert
//
// Revision 1.15  2004/04/12 21:08:09  vrandezo
// Lehrsprachen implementiert
// Mutter-/Zweitsprachensystem umgebaut
//
// Revision 1.14  2004/04/06 13:57:58  vrandezo
// Begabung und Unfaehigkeit von Talenten und Talentgruppen jetzt
// nach Errata implementiert (Twel)
//
// Revision 1.13  2003/10/27 01:26:28  vrandezo
// Zaubertänze
//
// Revision 1.12  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.11  2003/03/25 18:42:35  vrandezo
// Kampftechniken in der Kompaktübersicht mit AT/PA statt +AT/+PA
// Bug: Sonderfertigkeiten unabhängig von alphabetischer Reihenfolge
// Sonderfertigkeiten: Binden, Entwaffnen, Meisterliches Entwaffnen, Waffe zerbrechen
//
// Revision 1.10  2003/03/25 16:24:15  vrandezo
// Unfähigkeit für [Talentgruppe] und [Talent]
//
// Revision 1.9  2003/03/25 09:37:38  vrandezo
// Begabung für [Talentgruppe]
//
// Revision 1.8  2003/03/11 19:06:50  vrandezo
// -
//
// Revision 1.7  2003/03/09 15:26:30  vrandezo
// Waffenlose Kampfstile
// Aufgeteilte Editoren für Waffenlose Kampfstile und sonstige SF
// Voraussetzungen können auch auf Kampftechniken sein
//
// Revision 1.6  2003/03/08 16:26:33  vrandezo
// BE bei Talenten und Kampftechniken (Gerhard Mayer), mit GUI
// Mögliche Spezialisierungen von Talenten
//
// Revision 1.5  2003/03/07 15:06:41  vrandezo
// AT/PA-Werte bei Kampftechniken (Heldenbrief Gerhard Mayer)
//
// Revision 1.4  2003/03/04 15:23:17  vrandezo
// weitere Tags implementiert für die Druckvorlagen
//
// Revision 1.3  2003/02/24 17:37:26  vrandezo
// Prüfen implementiert
//
// Revision 1.2  2003/02/20 09:19:45  vrandezo
// Prüfe-Signatur geändert
//
// Revision 1.1.1.1  2003/02/17 15:32:48  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.7  2003/02/17 12:36:15  vrandezo
// GU für Sprachen und Schriften bei der Heldengenerierung hinzugefügt
//
// Revision 1.6  2003/01/26 16:49:03  vrandezo
// AT/PA jetzt bei Kampftechniken aufteilbar
//
// Revision 1.5  2003/01/11 20:30:57  vrandezo
// Auf wxWindows 2.4.0 gewechselt
// Beschreibungen bei Personenkomponenten und Talentoiden hinzugefügt
// PersonenkomponentenGUI verändert
//
// Revision 1.4  2002/11/05 21:49:18  vrandezo
// Sprachen und Schriften im- und exportierbar
// bool const ueberschreiben aus Speicher-Schnittstelle entfernt
// Mengen haben IDs erhalten
// Voraussetzungen für Talente (allgemeine und meisterliche)
// MengeGUIwx beim Wechseln von Objekten mit Rückfrage
//
// Revision 1.3  2002/11/05 14:28:05  vrandezo
// Sprachen hinzugefügt
//
// Revision 1.2  2002/11/02 23:36:57  vrandezo
// Exportier und Importierfunktionen implementiert
// Vergleichsoperationen implementiert
// Kampftechnikfenstter implementiert
//
// Revision 1.1  2002/11/02 11:44:41  vrandezo
// Kampftechnik hinzugefügt
//
//@nonl
//@-node:<< Version History >>
//@nl
//@nonl
//@-node:@file Kampftechnik.cpp
//@-leo
