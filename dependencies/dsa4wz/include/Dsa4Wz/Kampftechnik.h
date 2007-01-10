//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file Kampftechnik.h
//@+at
// Kampftechnik.h: Implementierung für Kampftechniken
// 
// $Revision: 1.13 $
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

#if !defined(KAMPFTECHNIK_H)
#define KAMPFTECHNIK_H

//@<< includes >>
//@+node:<< includes >>
#include "NDSABasis.h"
#include "Menge.h"
#include "Regeln.h"
#include "Auswahl.h"
#include "Definition.h"
//@-node:<< includes >>
//@nl

namespace DSA {
    
	//@  @+others
	//@+node:KampftechnikDefinition
	class KampftechnikDefinition : public Definition {
	public:
		KampftechnikDefinition(const std::string& id = "") : Definition(id), eBE_(KeineBE), skt_(D) {}
		static std::string Klasse() { return ("Kampftechnik"); }
	  virtual std::string VirtuelleKlasse() const { return Klasse(); }
		static std::string Klassen() { return ("Kampftechniken"); }
	
		typedef Kampfart Kriterium;
		static std::string Kriteriumsart() { return ("Kampfart"); }
		static std::string Kriteriumstring(Kampfart kampfart) { return KampfartString(kampfart, false); }
		static Kampfart Kriteriumstring(std::string s) { return KampfartString(s); }
		Kampfart ZuKriterium() const { return KampfArt(); }
		bool IstKriterium(Kampfart art) const { return KampfArt() == art; }
		void SetzeKriterium(Kampfart art) { KampfArt(art); }
	  
		void Art(Talentart art) { art_ = art; }
		Talentart Art() const { return art_; }
		void KampfArt(Kampfart art) { kampfart_ = art; }
		Kampfart KampfArt() const { return kampfart_; }
		void SKTSpalte_(SKTSpalte skt) { skt_ = skt; }
		SKTSpalte SKTSpalte_() const { return skt_; }
	  void EffektiveBehinderung(EBe ebe) { eBE_ = ebe; }
	  EBe EffektiveBehinderung() const { return eBE_; }
	
		bool operator==(const KampftechnikDefinition& other) const
		{ return ((Definition::operator==(other))
	           && (art_==other.art_)
	           && (kampfart_==other.kampfart_)
	           && (skt_==other.skt_)
	          && (eBE_==other.eBE_)); }
		bool operator!=(const KampftechnikDefinition& other) const { return !operator==(other); }
	
		// std::string Text() const;
	
		int Lese(PersistenzManager* pm, const std::string& id = "");
		int Schreibe(PersistenzManager* pm) const;
		int Pruefe();
		static int IDListe(PersistenzManager* pm, StringListe* liste);
		
	private:
		Talentart art_;
		Kampfart kampfart_;
		SKTSpalte skt_;
	  EBe eBE_;
	}; // KampftechnikDefinition
	//@-node:KampftechnikDefinition
	//@+node:Kampftechnik
	class Kampftechnik {
	public:
		Kampftechnik(const std::string& id = "", int wert = 0) : person_(0), wert_(wert), id_(id), autoAufteilenATPA_(true), aufAT_(0) {}
		Kampftechnik(const KampftechnikDefinition& def, int wert = 0) : person_(0), wert_(wert), id_(def.ID()), autoAufteilenATPA_(true), aufAT_(0) {}
		Kampftechnik(const KampftechnikDefinition* const def, int wert = 0) : person_(0), wert_(wert), id_(def->ID()), autoAufteilenATPA_(true), aufAT_(0) {}
	  void Bezugsperson(Person* p) { person_=p; }
	
		static std::string Klasse() { return ("Kampftechnik"); }
		static std::string Klassen() { return ("Kampftechniken"); }
	
	  typedef int Kriterium; // wird für die Auswahl benötigt, ignorieren
	
		void ID(const std::string& id) { id_ = id; }
		std::string ID() const { return id_; }
		void Wert(int wert);
		int Wert() const { return wert_; }
	  int Maximalwert() const;
	  void Extra(const std::string& extra);
	
	  std::string ErsetzeTag(const std::string& tag) const;
	
	  int Attacke() const;
	  int Parade() const;
	
	  bool HatParade() const;
	
	  void AufAT(int aufAT);
	  int AufAT() const;
	  bool WaereOKAufAT(int aufAT) const;
	  void AufPA(int aufPA);
	  int AufPA() const;
	  bool WaereOKAufPA(int aufPA) const;
	  void AutoAufteilenATPA(bool autoAufteilenATPA);
	  bool AutoAufteilenATPA() const;
	
		const KampftechnikDefinition* Definition() const;
	
		bool operator==(const Kampftechnik& other) const
			{ return ((id_==other.id_)
	           && (autoAufteilenATPA_==other.autoAufteilenATPA_)
	           && (aufAT_==other.aufAT_)
	           && (wert_==other.wert_)); }
		bool operator!=(const Kampftechnik& other) const { return !operator==(other); }
	
	  Talentart Art() const { return Definition()->Art(); }
		Kampfart KampfArt() const { return Definition()->KampfArt(); }
		SKTSpalte SKTSpalte_() const;
		std::string Text(const bool mitATPA = false) const;
	
		int Lese(PersistenzManager* pm, const std::string& id = "");
		int Schreibe(PersistenzManager* pm) const;
		int Pruefe();
		static int IDListe(PersistenzManager* pm, StringListe* liste);
		
	private:
	  Person* person_;
		std::string id_;
		int wert_;
	  int aufAT_;
	  bool autoAufteilenATPA_;
	}; // Kampftechnik
	//@nonl
	//@-node:Kampftechnik
	//@-others
  
	class KampftechnikDefinitionen : public SortierteMenge<KampftechnikDefinition> {
	}; // KampftechnikDefinitionen

	KampftechnikDefinitionen* kampftechniken();

  class Person;
  
  typedef Menge<Kampftechnik> Kampftechniken;
	//class Kampftechniken : public Menge<Kampftechnik> {
	//}; // Kampftechniken

  class KampftechnikenAuswahl : public AuswahlMenge<Kampftechnik> {
  }; // KampftechnikenAuswahl
}

#endif // !defined(KAMPFTECHNIK_H)

//@<< Version History >>
//@+node:<< Version History >>
// $Log: Kampftechnik.h,v $
// Revision 1.13  2004/10/17 19:12:39  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.12  2004/04/23 13:18:45  vrandezo
// Absturz bei Bauern - skt_ nicht initialisiert (Bugmeldung Twel u.a.)
//
// Revision 1.11  2004/04/13 14:06:52  vrandezo
// Hauszauber implementiert
//
// Revision 1.10  2004/04/06 13:57:58  vrandezo
// Begabung und Unfaehigkeit von Talenten und Talentgruppen jetzt
// nach Errata implementiert (Twel)
//
// Revision 1.9  2003/11/28 16:46:16  vrandezo
// Zeilenvorlauf am Dateiende (Warnung bei gcc)
//
// Revision 1.8  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.7  2003/03/25 09:37:39  vrandezo
// Begabung für [Talentgruppe]
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
// Revision 1.1.1.1  2003/02/17 15:32:51  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.9  2003/01/26 20:10:33  vrandezo
// operator== für Voraussetzungen verbessert
// Datensätze umbenennen jetzt möglich
// gelöschte Daten werden aus der Sicht entfernt
//
// Revision 1.8  2003/01/26 16:49:04  vrandezo
// AT/PA jetzt bei Kampftechniken aufteilbar
//
// Revision 1.7  2003/01/13 00:30:49  vrandezo
// Release Version 1.12 dev
// Korrektur der Verrechnungspunkteberechnung für negative Talentmodifikatoren
// Talentauswahl implementiert
//
// Revision 1.6  2003/01/11 20:30:57  vrandezo
// Auf wxWindows 2.4.0 gewechselt
// Beschreibungen bei Personenkomponenten und Talentoiden hinzugefügt
// PersonenkomponentenGUI verändert
//
// Revision 1.5  2002/12/06 15:28:00  vrandezo
// Spezies bei den Rassen einstellbar
// Kampftechniken bei PK und bei Person hinzugefügt
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
//@-node:@file Kampftechnik.h
//@-leo
