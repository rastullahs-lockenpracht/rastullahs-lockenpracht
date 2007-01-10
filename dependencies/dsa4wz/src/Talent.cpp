//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file Talent.cpp
//@+at
// Talent.cpp: Implementierung von Talent.
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
#include "Talent.h"

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

	//@  << TalentDefinition >>
	//@+node:<< TalentDefinition >>
	//@+others
	//@+node:Spezialisierungen
	//@+at
	// Gibt eine Liste der Bezeichner der verfügbaren 
	// Spezialisierungen zurück, in der zur Zeit 
	// eingestellten Sprache.
	//@-at
	//@@c
	StringListe TalentDefinition::Spezialisierungen() const {
	  StringListe sl;
	  for (DDict::const_iterator i = spezialisierungen_.begin(); i != spezialisierungen_.end(); ++i) {
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
	// Setzt eine neue Spezialisierung hinein. Name wird als 
	// der Bezeichner fuer die zur Zeit eingestellte Sprache 
	// verwendet, und aus ihm wird auch die ID erstellt.
	//@-at
	//@@c
	void TalentDefinition::SpezialisierungHinzu(const std::string& name) {
	  Dict d;
	  d[DSA::Lang()] = name;
	  std::string spezid = strings::MacheID("Spezialisierung", ID() + name);
	  spezialisierungen_[spezid] = d;
	}
	
	//@+at
	// Löscht eine Spezialisierung über den Bezeichner.
	//@-at
	//@@c
	void TalentDefinition::SpezialisierungWeg(const std::string& name) {
	  std::string key = Schluessel(name, spezialisierungen_);
	  if (key!="") {
	    spezialisierungen_.erase(spezialisierungen_.find(key));
	    return;
	  }
	  DDict::iterator i = spezialisierungen_.find(name);
	  if (i != spezialisierungen_.end())
	    spezialisierungen_.erase(i);
	}
	
	//@+at
	// Benennt eine Spezialsierung um. Hierbei wird der alte 
	// Name und der neue Name angegeben.
	//@-at
	//@@c
	void TalentDefinition::SpezialisierungUmbenennen(const std::string& alterName, const std::string& neuerName) {
	  std::string key = Schluessel(alterName, spezialisierungen_);
	  if (key!="") {
	    spezialisierungen_[key].operator[](DSA::Lang()) = neuerName;
	    return;
	  }
	  DDict::iterator i = spezialisierungen_.find(alterName);
	  if (i != spezialisierungen_.end())
	    spezialisierungen_[alterName].operator[](DSA::Lang()) = neuerName;
	}
	//@nonl
	//@-node:Spezialisierungen
	//@+node:Eigenschaft
	void TalentDefinition::Eigenschaften(Eigenschaftsart e1, Eigenschaftsart e2, Eigenschaftsart e3) {
		eigenschaft_[0] = e1;
		eigenschaft_[1] = e2;
		eigenschaft_[2] = e3;
	} // Eigenschaften
	
	Eigenschaftsart TalentDefinition::Eigenschaft(int i) const {
		if (i < 0) return unbekannteEigenschaft;
		if (i >= EigenschaftenProTalentprobe) return unbekannteEigenschaft;
		return eigenschaft_[i];
	} // Eigenschaft
	//@nonl
	//@-node:Eigenschaft
	//@+node:ProbeText
	std::string TalentDefinition::ProbeText(const std::string& trennzeichen, bool uebersetzt) const {
		std::string s = EigenschaftString(eigenschaft_[0], false, uebersetzt);
		for (int i = 1; i != DSA::EigenschaftenProTalentprobe; ++i)
			s += trennzeichen + EigenschaftString(eigenschaft_[i], false, uebersetzt);
		return s;
	} // ProbeText
	//@nonl
	//@-node:ProbeText
	//@+node:Lese
	int TalentDefinition::Lese(PersistenzManager* pm, const std::string& id) {
		std::string geleseneID = id;
		int rv = pm->OeffneZumLesen(Klasse(), geleseneID);
	
	  Dict bezeichnung;
	  Dict beschreibung;
	  rv += LeseBezeichnung(pm, geleseneID, bezeichnung, beschreibung);
	  
		std::string sBuf;
	  rv += pm->Lese(("Art"), sBuf);
		Talentart art = TalentartString(sBuf);
		std::string probe;
		rv += pm->Lese(("Probe"), probe); // TODO checken, ob wirklich Probe vorhanden
		Eigenschaftsart eigenschaft[EigenschaftenProTalentprobe];
		if (!rv) for (int i=0; i != EigenschaftenProTalentprobe; ++i)
			eigenschaft[i] = EigenschaftString(probe.substr(i*3, 2)); // TODO try/catch?
	  if (pm->Existiert(("eBE")))
	    rv += pm->Lese(("eBE"), sBuf);
	  else
	    sBuf = "-";
	  EBe eBE = EBeString(sBuf);
		Voraussetzungen allgemeineVoraussetzungen;
		rv += allgemeineVoraussetzungen.Lese(pm, ("allgemein"));
		Voraussetzungen meisterlicheVoraussetzungen;
		rv += meisterlicheVoraussetzungen.Lese(pm, ("meisterlich"));
	  DDict spezialisierungen;
	  rv += pm->Lese(spezialisierungen, ("Spezialisierung"), ("Spezialisierungen"), "", geleseneID);
	
		if (!rv) {
			ID(geleseneID);
	    bezeichnung_ = bezeichnung;
	   beschreibung_ = beschreibung;
			Eigenschaften(eigenschaft[0], eigenschaft[1], eigenschaft[2]);
			Art(art);
	    EffektiveBehinderung(eBE);
			allgemeineVoraussetzungen_ = allgemeineVoraussetzungen;
			meisterlicheVoraussetzungen_ = meisterlicheVoraussetzungen;
	    spezialisierungen_ = spezialisierungen;
		}
		rv += pm->SchliesseNachLesen();
		return rv; // TODO
	} // Lese
	//@-node:Lese
	//@+node:Schreibe
	int TalentDefinition::Schreibe(PersistenzManager* pm) const {
		int rv = pm->OeffneZumSchreiben(Klasse(), id_);
	
	  rv += SchreibeBezeichnung(pm);
	
		rv += pm->Schreibe(("Probe"), ProbeText("/", false));
		rv += pm->Schreibe(("Art"), TalentartString(art_, false));
	  if (EffektiveBehinderung()!=KeineBE)
	    pm->Schreibe(("eBE"), EBeString(EffektiveBehinderung(), false));
		rv += allgemeineVoraussetzungen_.Schreibe(pm);
		rv += meisterlicheVoraussetzungen_.Schreibe(pm);
	  rv += pm->Schreibe(spezialisierungen_, ("Spezialisierung"), ("Spezialisierungen"));
		rv += pm->SchliesseNachSchreiben();
		return rv;
	} // Schreibe
	//@nonl
	//@-node:Schreibe
	//@+node:Pruefe
	int TalentDefinition::Pruefe() {
	  int rv = 0;
	  rv += allgemeineVoraussetzungen_.Pruefe();
	  rv += meisterlicheVoraussetzungen_.Pruefe();
	  if ((Gruppe()==Kampf) || (Gruppe()==Gabe) || (Gruppe()==sonstige)) {
	    Log::Info(_t("Unpassende Talentgruppe"));
	    ++rv;
	  }
	  for (int i=0; i != EigenschaftenProTalentprobe; ++i)
	    if (eigenschaft_[i]==unbekannteEigenschaft) {
	      Log::Info(_t("Unbekannte Eigenschaft"));
	      ++rv;
	    }
	  if (rv!=0) Log::Info(_t("in") + " " + Klasse() + " " + ID());
		return rv;
	} // Pruefe
	//@nonl
	//@-node:Pruefe
	//@+node:IDListe
	int TalentDefinition::IDListe(PersistenzManager* pm, StringListe* liste) {
		return pm->IDListe(_t("Talent"), liste); // TODO Prüfen
	} // IDListe
	//@nonl
	//@-node:IDListe
	//@-others
	
	//@-node:<< TalentDefinition >>
	//@nl
  
	TalentDefinitionen* talente() 
		{ static TalentDefinitionen* talente_ = new TalentDefinitionen(); return talente_; }
        
	//@  << Talent >>
	//@+node:<< Talent >>
	//@+others
	//@+node:SKTSpalte_
	SKTSpalte Talent::SKTSpalte_() const {
	  if (person_==0)
	    return Definition()->SKTSpalte_();
	  SKTSpalte s = Definition()->SKTSpalte_();
	  if ((person_->charakteristika_.Existiert(_t("Begabung für") + " " + ID()))
	    ||(person_->charakteristika_.Existiert(_t("Begabung für") + " " + TalentgruppeString(Gruppe(), false))))
	    s = SpalteLeichter(s);
	  if ((person_->charakteristika_.Existiert(_t("Unfähigkeit für") + " " + ID()))
	    ||(person_->charakteristika_.Existiert(_t("Unfähigkeit für") + " " + TalentgruppeString(Gruppe(), false))))
	    s = SpalteSchwerer(s);
	  return s;
	} // SKTSpalte_
	//@nonl
	//@-node:SKTSpalte_
	//@+node:Maximalwert
	int Talent::Maximalwert() const {
	  if (person_==0) return 0;
	
	  if (regeln()->Regeloption("Begabungen und Unfähigkeiten nach Errata"))
	    if ((person_->charakteristika_.Existiert(_t("Unfähigkeit für") + " " + ID()))
	      ||(person_->charakteristika_.Existiert(_t("Unfähigkeit für") + " " + TalentgruppeString(Gruppe(), false))))
	      return min(person_->Wert(Eigenschaft(0)),
	                 person_->Wert(Eigenschaft(1)),
	                 person_->Wert(Eigenschaft(2)));
	
	  int maxwert = max(person_->Wert(Eigenschaft(0)),
	                    person_->Wert(Eigenschaft(1)),
	                    person_->Wert(Eigenschaft(2)));
	  maxwert += 3; // TODO Konstante
	                       
	  if (regeln()->Regeloption("Begabungen und Unfähigkeiten nach Errata"))
	    if ((person_->charakteristika_.Existiert(_t("Begabung für") + " " + ID()))
	      ||(person_->charakteristika_.Existiert(_t("Begabung für") + " " + TalentgruppeString(Gruppe(), false))))
	      maxwert += 2;
	  
	  return maxwert;
	} // Maximalwert
	//@nonl
	//@-node:Maximalwert
	//@+node:Extra
	void Talent::Extra(const std::string& extra) {
	  Log::Error("Talent::Extra: Unbekanntes Extra " + extra);
	}
	//@nonl
	//@-node:Extra
	//@+node:Text
	std::string Talent::Text() const {
		return ID() + " " + strings::int2sStr(Wert(), true);
	} // Text
	//@nonl
	//@-node:Text
	//@+node:ErsetzeTag
	std::string Talent::ErsetzeTag(const std::string& tag) const {
	  if (""==tag) return Text();
	  if ("Name"==tag) return ID();
	  if ("W"==tag) return strings::int2sStr(Wert(), true);
	  if ("Probe"==tag) return Definition()->ProbeText();
	  if ("p1"==tag) return EigenschaftString(Definition()->Eigenschaft(0));
	  if ("p2"==tag) return EigenschaftString(Definition()->Eigenschaft(1));
	  if ("p3"==tag) return EigenschaftString(Definition()->Eigenschaft(2));
	  if ("SKT"==tag) return SKTSpalteString(SKTSpalte_());
	  if ("Beschreibung"==tag) return Definition()->Beschreibung();
	  return tag;
	} // ErsetzeTag
	//@nonl
	//@-node:ErsetzeTag
	//@+node:Definition
	const TalentDefinition* Talent::Definition() const {
		TalentDefinition* def = talente()->Zeige(id_);
		if (def!=0) return def;
		Log::Error("Talent::Definition: " + _t("Undefiniertes Talent") + " " + id_);
		static TalentDefinition* sdef = new TalentDefinition();
		sdef->Gruppe(Kampf);
		return sdef; // TODO workaround. Sollte bei nicht vorhandenen Defs intelligenter
		// reagieren (doch wie?) // TODO Speicherleck
	} // Definition
	//@nonl
	//@-node:Definition
	//@+node:Lese
	int Talent::Lese(PersistenzManager* pm, const std::string& id) {
		std::string geleseneID = id;
		int rv = pm->OeffneZumLesen(("Talent"), geleseneID);
		int wert;
		rv += pm->Lese(("Wert"), wert);
		if (!rv) {
	    person_ = 0;
			ID(geleseneID);
			Wert(wert);
		}
		pm->SchliesseNachLesen();
		return rv; // TODO
	} // Lese
	//@nonl
	//@-node:Lese
	//@+node:Schreibe
	int Talent::Schreibe(PersistenzManager* pm) const {
		int rv = pm->OeffneZumSchreiben(Klasse(), ID());
		rv += pm->Schreibe(("Wert"), Wert());
		rv += pm->SchliesseNachSchreiben();
		return rv;
	} // Schreibe
	//@nonl
	//@-node:Schreibe
	//@+node:Pruefe
	int Talent::Pruefe() {
	  int rv = 0;
	  if (talente()->Zeige(ID()) == 0) {
	    Log::Info("Talent::Pruefe: unbekanntes Talent " + ID());
	    std::string s = talente()->Finde(ID());
	    if (s != "") {
	      Log::Info("ist eigentlich " + s + " (wurde korrigiert)");
	      ID(s);
	    }
	    ++rv;
	  }
		return rv;
	} // Pruefe
	
	//@-node:Pruefe
	//@+node:IDListe
	int Talent::IDListe(PersistenzManager* pm, StringListe* liste) {
		return pm->IDListe(_t("Talent"), liste); // TODO Prüfen
	} // IDListe
	//@nonl
	//@-node:IDListe
	//@-others
	
	//@-node:<< Talent >>
	//@nl
  
}

//@<< Version History >>
//@+node:<< Version History >>
// $Log: Talent.cpp,v $
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
// Revision 1.13  2004/04/06 13:57:59  vrandezo
// Begabung und Unfaehigkeit von Talenten und Talentgruppen jetzt
// nach Errata implementiert (Twel)
//
// Revision 1.12  2004/01/29 09:06:20  dertron
// doppelte CVS Log-Tags entfernt
//
// Revision 1.11  2003/12/06 15:01:29  vrandezo
// in LEO eingearbeitet
//
// Revision 1.10  2003/11/28 16:46:16  vrandezo
// Zeilenvorlauf am Dateiende (Warnung bei gcc)
//
// Revision 1.9  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.8  2003/03/25 16:24:16  vrandezo
// Unfähigkeit für [Talentgruppe] und [Talent]
//
// Revision 1.7  2003/03/25 09:40:17  vrandezo
// Begabung für [Talentgruppe]
//
// Revision 1.6  2003/03/13 18:16:19  vrandezo
// Bug: Wenn man den XML- Export auswählt und dann abbricht stürzt das
// Programm ab (King Lui)
// Bug: Klickt man bei der Heldenerschaffung im Raster Talente
// auf den oberen Bereich (z.B. auf TaW), so wird das
// Programm sofort beendet (King Lui)
//
// Revision 1.5  2003/03/08 16:26:33  vrandezo
// BE bei Talenten und Kampftechniken (Gerhard Mayer), mit GUI
// Mögliche Spezialisierungen von Talenten
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
// Revision 1.1.1.1  2003/02/17 15:34:30  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.13  2003/01/11 20:30:58  vrandezo
// Auf wxWindows 2.4.0 gewechselt
// Beschreibungen bei Personenkomponenten und Talentoiden hinzugefügt
// PersonenkomponentenGUI verändert
//
// Revision 1.12  2002/11/05 21:49:19  vrandezo
// Sprachen und Schriften im- und exportierbar
// bool const ueberschreiben aus Speicher-Schnittstelle entfernt
// Mengen haben IDs erhalten
// Voraussetzungen für Talente (allgemeine und meisterliche)
// MengeGUIwx beim Wechseln von Objekten mit Rückfrage
//
// Revision 1.11  2002/11/05 16:24:38  vrandezo
// MengeGUIwx vereinfacht (nur ein Template-Parameter)
// SpracheGUIwx hinzugefügt
// SortierteMenge erweitert um Zeige
//
// Revision 1.10  2002/11/05 13:12:43  vrandezo
// Spezies bei den Rassen einegführt
// SortierteMenge verallgemeinert
//
// Revision 1.9  2002/11/02 15:39:48  vrandezo
// Importfenster hinzugefügt
//
// Revision 1.8  2002/10/27 17:29:21  vrandezo
// Die meisten Teile sind speicherbar
//
// Revision 1.7  2002/10/25 14:21:12  vrandezo
// Hauptmenü erweitert
// Charakteristika hinzugefügt
// eigenes Mengen-GUI-Element erzeugt
//
// Revision 1.6  2002/10/20 14:25:12  vrandezo
// Eigenes Talentfenster eingeführt
//
// Revision 1.5  2002/10/19 20:35:32  vrandezo
// feste Stringliterale entfernt
//
// Revision 1.4  2002/10/19 18:58:33  vrandezo
// Texte in eigene, externe Datei untergebracht
//
// Revision 1.3  2002/10/18 00:50:16  vrandezo
// Talente bei der Heldenerschaffung einsetzbar
//
// Revision 1.2  2002/10/17 15:35:58  vrandezo
// TalentGUI hinzugefügt
//
// Revision 1.1  2002/10/17 10:54:44  vrandezo
// Talent und Talente eingefügt
//
//@nonl
//@-node:<< Version History >>
//@nl
//@-node:@file Talent.cpp
//@-leo
