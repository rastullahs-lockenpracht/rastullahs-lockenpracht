//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file Voraussetzung.cpp
//@+at
// Voraussetzung.cpp: Implementation für die Voraussetzung
// 
// $Revision: 1.23 $
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
#include "Voraussetzung.h"

#include "PersistenzManager.h"
#include "Log.h"
#include "NDSAUtil.h"
#include "Person.h"
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

	//@  << class Voraussetzung >>
	//@+node:<< class Voraussetzung >>
	Voraussetzung::~Voraussetzung() { delete alternativ_; }
	
	//@+others
	//@+node:copyctor
	Voraussetzung::Voraussetzung(const Voraussetzung& other)
		: id_(other.id_),
	    minimal_(other.minimal_),
	    maximal_(other.maximal_),
	    kein_(other.kein_),
	    minWert_(other.minWert_),
	    maxWert_(other.maxWert_),
	    alternativ_(0)
	{
	  if (other.alternativ_!=0) {
	    alternativ_ = new Voraussetzungen(("alternativ"));
	    forallconstp (Voraussetzungen, vor, other.alternativ_)
	      alternativ_->FuegeEin(vor->second);
	  }
	}
	
	Voraussetzung& Voraussetzung::operator=(const Voraussetzung& other) {
	  id_ = other.id_;
	  minimal_ = other.minimal_;
	  maximal_ = other.maximal_;
	  kein_ = other.kein_;
	  minWert_ = other.minWert_;
	  maxWert_ = other.maxWert_;
	  if (other.alternativ_!=0) {
	    alternativ_ = new Voraussetzungen(("alternativ"));
	    forallconstp (Voraussetzungen, vor, other.alternativ_)
	      alternativ_->FuegeEin(vor->second);
	  }
	  return *this;
	}
	
	
	//@-node:copyctor
	//@+node:==
	bool Voraussetzung::operator==(const Voraussetzung& other) const {
	  if (id_!=other.id_) return false;
	  if (minimal_!=other.minimal_) return false;
	  if (minimal_) if (minWert_!=other.minWert_) return false;
	  if (maximal_!=other.maximal_) return false;
	  if (maximal_) if (maxWert_!=other.maxWert_) return false;
	  if (kein_!=other.kein_) return false;
	  if (Oder()!=other.Oder()) return false;
	  if (Oder()) {
	    if (alternativ_->size() != other.alternativ_->size()) return false;
	    Voraussetzungen::const_iterator v1 = alternativ_->begin();
	    Voraussetzungen::const_iterator v2 = other.alternativ_->begin();
	    while (v1 != alternativ_->end()) {
	      if (*v1 != *v2) return false;
	      ++v1; ++v2;
	    } 
	  }
	  return true;
	}
	//@nonl
	//@-node:==
	//@+node:Text
	std::string Voraussetzung::Text() const {
	  std::string s = "";
	  if (Minimal()) {
			if (Maximal())
				s = TextID(ID()) + " " + strings::int2sStr(MinimalWert()) + "-" + strings::int2sStr(MaximalWert());
			else
				s = TextID(ID()) + " " + strings::int2sStr(MinimalWert());
	  } else if (Maximal())
	    s = TextID(ID()) + " " + _t("höchstens") + " " + strings::int2sStr(MaximalWert());
	  else if (Kein())
	    s = _t("ohne") + " " + TextID(ID());
	  else
	    s = TextID(ID());
	  if (!Oder())
	    return s;
	  
	  for (Voraussetzungen::const_iterator vor = alternativ_->begin(); vor != alternativ_->end(); ++vor)
	    s += (" " + _t("oder") + " ") + vor->second.Text();
	  
	  return s;
	} // Text
	//@nonl
	//@-node:Text
	//@+node:TextID
	std::string Voraussetzung::TextID(const std::string& id) const {
	  if (IstEigenschaft(id))
	    return EigenschaftString(EigenschaftString(id));
	  if (IstAbgeleiteterWert(id))
	    return AbgeleiteterWertString(AbgeleiteterWertString(id));
	  if (talente()->Existiert(id))
	    return talente()->Zeige(id)->Bezeichnung();
	  if (kampftechniken()->Existiert(id))
	    return kampftechniken()->Zeige(id)->Bezeichnung();
	  if (sonderfertigkeiten()->Existiert(id))
	    return sonderfertigkeiten()->Zeige(id)->Bezeichnung();
	  if (generierteSonderfertigkeiten()->Existiert(id))
	    return generierteSonderfertigkeiten()->Zeige(id)->Bezeichnung();
	  if (charakteristika()->Existiert(id))
	    return charakteristika()->Zeige(id)->Bezeichnung();
	  if (generierteCharakteristika()->Existiert(id))
	    return generierteCharakteristika()->Zeige(id)->Bezeichnung();
	  return id;
	}
	//@nonl
	//@-node:TextID
	//@+node:Oder
	void Voraussetzung::Oder(Voraussetzung vor) {
	  if (alternativ_==0)
	    alternativ_ = new Voraussetzungen(("alternativ"));
	  alternativ_->FuegeEin(vor);
	}
	
	Voraussetzung Voraussetzung::Oder(int i) const {
	  if ((i<1)||(alternativ_==0))
	    return *this;
	  if (i >= alternativ_->size())
	    return *this;
	  return alternativ_->Hole(i);
	}
	
	int Voraussetzung::OderAnzahl() const {
	  if (alternativ_==0)
	    return 1;
	  else
	    return alternativ_->size();
	}
	
	void Voraussetzung::OderLoeschen() {
	  delete alternativ_;
	  alternativ_ = 0;
	}
	
	void Voraussetzung::OderLoeschen(int i) {
	  Log::Error("Voraussetzung::OderLoeschen: nicht implementiert");
	}
	
	bool Voraussetzung::Oder() const {
	  return (alternativ_!=0);
	}
	//@nonl
	//@-node:Oder
	//@+node:Erfuellt
	bool Voraussetzung::Erfuellt(const Person& person) const {
	  if (!Oder())
	    return ErfuelltPrivat(person);
	    
	  if (ErfuelltPrivat(person)==true)
	    return true;
	    
	  for (Voraussetzungen::const_iterator vor = alternativ_->begin(); vor != alternativ_->end(); ++vor)
	    if (vor->second.Erfuellt(person))
	      return true;
	      
	  return false;
	}
	
	//@+others
	//@+node:Privat
	bool Voraussetzung::ErfuelltPrivat(const Person& person) const {
	  bool gefunden = false;
	  int wert = 0;
	  if (Maximal() || Minimal()) {
	    if (EigenschaftString(ID()) != unbekannteEigenschaft) {
	      gefunden = true;
	      wert = person.Wert(EigenschaftString(ID()));
	    }
	    if (AbgeleiteterWertString(ID()) != unbekannterAbgeleiteterWert) {
	      gefunden = true;
	      wert = person.Wert(AbgeleiteterWertString(ID()));
	    }
	    if (talente()->Existiert(ID())) {
	      gefunden = true;
	      if (person.talente_.Existiert(ID()))
	        wert = person.talente_.Hole(ID()).Wert();
	      else
	        wert = -10; // TODO ist halt nicht
	    }
	    if (kampftechniken()->Existiert(ID())) {
	      gefunden = true;
	      if (person.kampftechniken_.Existiert(ID()))
	        wert = person.kampftechniken_.Hole(ID()).Wert();
	      else
	        wert = -10; // TODO ist halt nicht
	    }
	    if ((sonderfertigkeiten()->Existiert(ID()))
	      ||(generierteSonderfertigkeiten()->Existiert(ID()))) {
	      gefunden = true;
	      if (person.sonderfertigkeiten_.Existiert(ID()))
	        wert = person.sonderfertigkeiten_.Zeige(ID())->Stufe();
	      if (person.charakteristika_.Existiert(ID()))
	        wert = person.charakteristika_.Zeige(ID())->Stufe();
	    }
	    
	    if (gefunden) {
	      if (Maximal())
	        if (wert > MaximalWert()) return false;
	      if (Minimal())
	        if (wert < MinimalWert()) return false;
	      return true;
	    }
	    Log::Debug(ID());
	    Log::Error("Voraussetzung::Erfuellt - Voraussetzung nicht erkannt.");
	    return false;
	  }
	  gefunden = false;
	  if ((charakteristika()->Existiert(ID()))
	   || (generierteCharakteristika()->Existiert(ID())))
	    gefunden = true;
	  if ((sonderfertigkeiten()->Existiert(ID()))
	    ||(generierteSonderfertigkeiten()->Existiert(ID())))     
	    gefunden = true;
	
	  if (ID()==("Spruchzauberer")) {
	    DSA::Voraussetzung spruchzauberer;
	    for (DSA::SonderfertigkeitDefinitionen::const_iterator sf = DSA::generierteSonderfertigkeiten()->begin(); sf != DSA::generierteSonderfertigkeiten()->end(); ++sf) 
	      if (sf->second.IstRepraesentation())
	        if (spruchzauberer.ID()=="")
	          spruchzauberer.ID(sf->first);
	        else
	          spruchzauberer.Oder(sf->first);
	      if (Kein())
	        return !spruchzauberer.Erfuellt(person);  
	      else
	        return spruchzauberer.Erfuellt(person);  
	  }
	    
	  if (!gefunden) {
	    Log::Info(ID());
	    Log::Error("Voraussetzung::Erfuellt - Voraussetzung nicht erkannt.");
	    return false;
	  }
	  if (Kein()) {
	    if (person.charakteristika_.Verfuegt(ID()))
	      return false;
	    if (person.sonderfertigkeiten_.Existiert(ID()))
	      return false;
	    return true;
	  }
	  if (person.sonderfertigkeiten_.Existiert(ID()))
	    return true;
	  if (person.charakteristika_.Verfuegt(ID()))
	    return true;
	  return false;
	} // Erfüllt
	//@nonl
	//@-node:Privat
	//@-others
	
	//@-node:Erfuellt
	//@+node:Lese
	int Voraussetzung::Lese(PersistenzManager* pm, const std::string& id) {
		std::string geleseneID = id;
		int rv = pm->OeffneZumLesen(("Voraussetzung"), geleseneID);
		int maxwert = 0;
	  int minwert = 0;
	  bool max = pm->Existiert(("Wert"), ("maximal"));
	  bool min = pm->Existiert(("Wert"), ("minimal"));
	  if (max) rv += pm->Lese(("Wert"), ("maximal"), maxwert);
	  if (min) rv += pm->Lese(("Wert"), ("minimal"), minwert);
	  bool kein = pm->Existiert(("kein"));
	  Voraussetzungen* vor = new Voraussetzungen();
	  rv += vor->Lese(pm);
	  
		if (!rv) {
			ID(geleseneID);
	    Maximal(max);
	    Minimal(min);
	    MaximalWert(maxwert);
	    MinimalWert(minwert);
	    Kein(kein);
	    
	    OderLoeschen();
	    for (Voraussetzungen::const_iterator v = vor->begin(); v != vor->end(); ++v)
	      Oder(v->second);
	
	    delete vor;    
		}
	    
		pm->SchliesseNachLesen();
		return rv; // TODO
	} // Lese
	//@nonl
	//@-node:Lese
	//@+node:Schreibe
	int Voraussetzung::Schreibe(PersistenzManager* pm) const {
		int rv = pm->OeffneZumSchreiben(Klasse(), ID());
		if (Minimal()) rv += pm->Schreibe(("Wert"), ("minimal"), MinimalWert());
		if (Maximal()) rv += pm->Schreibe(("Wert"), ("maximal"), MaximalWert());
	  if (Kein()) { rv += pm->OeffneZumSchreiben(("kein")); rv += pm->SchliesseNachSchreiben(); }
	  if (Oder()) rv += alternativ_->Schreibe(pm);
		rv += pm->SchliesseNachSchreiben();
		return rv; // TODO
	} // Schreibe
	//@nonl
	//@-node:Schreibe
	//@+node:Pruefe
	int Voraussetzung::Pruefe() {
	  int rv = 0;
	  if ((Maximal()) && (Minimal()) && (MinimalWert() > MaximalWert())) {
	    Log::Info(("Minimalwert größer als der Maximalwert"));
	    ++rv;
	  }
	  if ((Kein()) && (Minimal() || Maximal())) {
	    Log::Info(("Gleichzeitges Verlangen von Min-/Maxwert und Nichtexistenz"));
	    ++rv;
	  }
	  bool gefunden = false;
	  if (Maximal() || Minimal()) {
	    if (IstEigenschaft(ID()))
	      gefunden = true;
	    if (IstAbgeleiteterWert(ID()))
	      gefunden = true;
	    if (talente()->Existiert(ID()))
	      gefunden = true;
	    if (kampftechniken()->Existiert(ID()))
	      gefunden = true;
	    if (sonderfertigkeiten()->Existiert(ID()))
	      if (sonderfertigkeiten()->Zeige(ID())->IstRitualkenntnis())
	        gefunden = true;
	    if (generierteSonderfertigkeiten()->Existiert(ID()))
	      if (generierteSonderfertigkeiten()->Zeige(ID())->IstRitualkenntnis())
	        gefunden = true;
	  }
	  if (Kein()) {
	    if ((charakteristika()->Existiert(ID()))
	     || (generierteCharakteristika()->Existiert(ID())))
	      gefunden = true;
	    if ((sonderfertigkeiten()->Existiert(ID()))
	      ||(generierteSonderfertigkeiten()->Existiert(ID())))     
	      gefunden = true;
	  }
	  if (!(Maximal() || Minimal() || Kein())) {
	    if ((sonderfertigkeiten()->Existiert(ID()))
	      ||(generierteSonderfertigkeiten()->Existiert(ID())))     
	      gefunden = true;
	    if (charakteristika()->Existiert(ID()))
	      gefunden = true;
	  }
	  if (ID()==("Spruchzauberer"))
	    gefunden = true;
	
	  if (!gefunden) {
	    Log::Info("Unbekannte Voraussetzung: " + ID());
	    std::string s;
	    if ((s = talente()->Finde(ID())) != "") {
	      Log::Info("ist eigentlich " + s + " (wurde korrigiert)");
	      ID(s);
	    }
	    else
	    if ((s = kampftechniken()->Finde(ID())) != "") {
	      Log::Info("ist eigentlich " + s + " (wurde korrigiert)");
	      ID(s);
	    }
	    else
	    if ((s = charakteristika()->Finde(ID())) != "") {
	      Log::Info("ist eigentlich " + s + " (wurde korrigiert)");
	      ID(s);
	    }
	    else
	    if ((s = sonderfertigkeiten()->Finde(ID())) != "") {
	      Log::Info("ist eigentlich " + s + " (wurde korrigiert)");
	      ID(s);
	    }
	    else
	    if ((s = generierteCharakteristika()->Finde(ID())) != "") {
	      Log::Info("ist eigentlich " + s + " (wurde korrigiert)");
	      ID(s);
	    }
	    else
	    if ((s = generierteSonderfertigkeiten()->Finde(ID())) != "") {
	      Log::Info("ist eigentlich " + s + " (wurde korrigiert)");
	      ID(s);
	    }
	
	//    else
	      ++rv;
	  }
	  if (Oder())
	    rv += alternativ_->Pruefe();
	
	  if (rv!=0) Log::Info(" in " + Klasse() + " " + ID());
		return rv;
	} // Pruefe
	//@-node:Pruefe
	//@+node:IDListe
	int Voraussetzung::IDListe(PersistenzManager* pm, StringListe* liste) {
		return pm->IDListe(_t("Voraussetzung"), liste); // TODO Prüfen
	} // IDListe
	//@nonl
	//@-node:IDListe
	//@-others
	//@-node:<< class Voraussetzung >>
	//@nl
  
	//@  << class Voraussetzungen >>
	//@+node:<< class Voraussetzungen >>
	bool Voraussetzungen::FuegeEin(Voraussetzung voraussetzung) {
		if (Existiert(voraussetzung.ID())) {
			Voraussetzung vor2 = Hole(voraussetzung.ID());
			if (voraussetzung.Maximal()==vor2.Maximal()) return false;
			if (voraussetzung.Minimal()==vor2.Minimal()) return false;
			Voraussetzung gemVor(vor2.ID());
			gemVor.Maximal(true);
			gemVor.Minimal(true);
			gemVor.MaximalWert( vor2.Maximal() ? vor2.MaximalWert() : voraussetzung.MaximalWert() );
			gemVor.MinimalWert( vor2.Minimal() ? vor2.MinimalWert() : voraussetzung.MinimalWert() );
			Loesche(gemVor.ID());
			return Menge<Voraussetzung>::FuegeEin(gemVor);
		}
		return Menge<Voraussetzung>::FuegeEin(voraussetzung);
	} // FuegeEin
	
	bool Voraussetzungen::Erfuellt(const Person& person) const {
	  for (const_iterator v = begin(); v != end(); ++v)
	    if (!v->second.Erfuellt(person)) return false;
	  return true;
	} // Erfuellt
	//@nonl
	//@-node:<< class Voraussetzungen >>
	//@nl

}

//@<< Version History >>
//@+node:<< Version History >>
// $Log: Voraussetzung.cpp,v $
// Revision 1.23  2004/10/17 19:12:40  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.22  2004/04/17 13:24:02  vrandezo
// mehr Info
//
// Revision 1.21  2004/04/12 13:20:23  vrandezo
// Merkmalskenntnisse implementiert
//
// Revision 1.20  2004/04/12 01:43:47  vrandezo
// Bugs wegen Zeiger (Pointer Hell!)
//
// Revision 1.19  2004/04/11 23:02:45  vrandezo
// - maechtig das Programm beschleunigt (Faktor wasweissich!)
// - Bug entfernt, der verhinderte, dass manche SF
// (mit Voraussetzung RK > bestimmter Wert) Personen
// hinzugefuegt werden konnten
//
// Revision 1.18  2004/04/11 21:13:06  vrandezo
// Voraussetzungen koennen jetzt mit "oder" verknuepft werden
//
// Revision 1.17  2004/04/10 20:20:46  vrandezo
// maxwert wurde in MSVC nicht vorinitialisiert
//
// Revision 1.16  2004/01/29 09:06:20  dertron
// doppelte CVS Log-Tags entfernt
//
// Revision 1.15  2004/01/05 05:43:57  vrandezo
// Sonderfertigkeiten können jetzt negative Voraussetzungen sein
//
// Revision 1.14  2003/11/28 16:46:16  vrandezo
// Zeilenvorlauf am Dateiende (Warnung bei gcc)
//
// Revision 1.13  2003/10/25 22:33:27  vrandezo
// SF-Text
//
// Revision 1.12  2003/10/25 02:11:18  vrandezo
// generierte Sonderfertigkeiten
//
// Revision 1.11  2003/10/24 23:50:41  vrandezo
// Leonizing
// Vor-/Nachteile als Voraussetzung ermöglicht
// Astralmacht, ZVH-Zauberer
//
// Revision 1.10  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.9  2003/03/25 12:42:26  vrandezo
// Begabung für [Talent]
//
// Revision 1.8  2003/03/25 09:40:19  vrandezo
// Begabung für [Talentgruppe]
//
// Revision 1.7  2003/03/09 15:24:59  vrandezo
// Waffenlose Kampfstile
// Aufgeteilte Editoren für Waffenlose Kampfstile und sonstige SF
// Voraussetzungen können auch auf Kampftechniken sein
//
// Revision 1.6  2003/02/24 17:37:46  vrandezo
// Prüfen implementiert
//
// Revision 1.5  2003/02/20 09:19:45  vrandezo
// Prüfe-Signatur geändert
//
// Revision 1.4  2003/02/19 17:45:15  vrandezo
// Kulturen zeigen mögliche und übliche Rassen an,
// Professionen mögliche Kulturen
//
// Revision 1.3  2003/02/18 01:19:14  vrandezo
// (erste) GUI für SF und für Charakteristika in Generierung
//
// Revision 1.2  2003/02/17 18:12:02  vrandezo
// AP-Kosten für SF
// Voraussetzungen für SF nach Talentwert, Eigenschaft, andere SF
//
// Revision 1.1.1.1  2003/02/17 15:34:38  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.13  2003/02/09 23:58:12  vrandezo
// - steigerbare VN ohne Wert (Balance->Herausragende Balance)
// - Person-Vergleich korrigiert
// - Voraussetzung-Lesen/Schreiben korrigiert
//
// Revision 1.12  2003/02/08 00:22:54  vrandezo
// VN von RKP bei Generierung übernommen
// mehrfache VN bei RKP geben GP
// erweitere Voraussetzungen: nicht VN
// entfernbare VN
// auswählbare VN
// VN mit Voraussetzungen (nicht VN)
// Geschlecht neuerschaffener Helden ist zufällig
//
// Revision 1.11  2003/02/07 19:37:26  vrandezo
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
// Revision 1.10  2003/01/15 20:02:30  vrandezo
// Voraussetzungen jetzt auch für Rassen und Professionen
// GUI für Modifikatoren bei Personenkomponenten verbessert
// Lesen von Modifikatoren erweitert
//
// Revision 1.9  2002/11/05 21:49:19  vrandezo
// Sprachen und Schriften im- und exportierbar
// bool const ueberschreiben aus Speicher-Schnittstelle entfernt
// Mengen haben IDs erhalten
// Voraussetzungen für Talente (allgemeine und meisterliche)
// MengeGUIwx beim Wechseln von Objekten mit Rückfrage
//
// Revision 1.8  2002/11/02 23:36:57  vrandezo
// Exportier und Importierfunktionen implementiert
// Vergleichsoperationen implementiert
// Kampftechnikfenstter implementiert
//
// Revision 1.7  2002/10/27 17:29:21  vrandezo
// Die meisten Teile sind speicherbar
//
// Revision 1.6  2002/10/19 20:35:32  vrandezo
// feste Stringliterale entfernt
//
// Revision 1.5  2002/10/19 18:58:33  vrandezo
// Texte in eigene, externe Datei untergebracht
//
// Revision 1.4  2002/10/17 10:54:44  vrandezo
// Talent und Talente eingefügt
//
// Revision 1.3  2002/10/13 15:18:42  vrandezo
// Voraussetzungen für Professionen implementiert
// CVS aufgeräumt für Modifkatoren und Voraussetzungen
//
// Revision 1.2  2002/08/08 18:51:09  vrandezo
// da Xerces und andere XML-Parser XPath nicht
// unterstützen, wurde die Navigation im DOM-Baum
// von XPath auf eine generische Variante umgestellt.
// Dazu mussten allen Schnittstellen verändert werden.
//
// Revision 1.1  2002/06/02 13:40:34  Zdenko Vrandecic
// Startversion
//
//@nonl
//@-node:<< Version History >>
//@nl
//@-node:@file Voraussetzung.cpp
//@-leo
