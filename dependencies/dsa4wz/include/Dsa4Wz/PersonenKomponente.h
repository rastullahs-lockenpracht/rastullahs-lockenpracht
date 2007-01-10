//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file PersonenKomponente.h
//@+at
// PersonenKomponente.h: Implementation der Basisklasse 
// PersonenKomponente.
// 
// $Revision: 1.15 $
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

#if !defined(PERSONENKOMPONENTE_H)
#define PERSONENKOMPONENTE_H

//@<< includes >>
//@+node:<< includes >>
#include "NDSABasis.h"
#include "Modifikator.h"
#include "Talent.h"
#include "Kampftechnik.h"
#include "Charakteristikum.h"
#include "Sonderfertigkeit.h"
#include "SpracheSchrift.h"
#include "Zauber.h"
//@nonl
//@-node:<< includes >>
//@nl

namespace DSA {

	class PersistenzManager;
	
	//@  @+others
	//@+node:class PersonenKomponente
	class PersonenKomponente {
	public:
		PersonenKomponente();
	
	  void ID(const std::string& id) { id_ = id; bezeichnung_ = id; }
		std::string ID() const { return id_; }
	  void Bezeichnung(const std::string& b) { bezeichnung_ = b; }
	  std::string Bezeichnung() const { return bezeichnung_; }
	
		void GP(int gp) { gp_ = gp; }
		int GP() const { return gp_; }
	
	  bool IstGeschlechtsspezifisch() const { return geschlechtsspezifisch_; }
	  void IstGeschlechtsspezifisch(bool g) { geschlechtsspezifisch_ = g; }
	
	  int Lese(PersistenzManager* pm, const std::string& typ, const std::string& id = "");
	 	int Schreibe(PersistenzManager* pm, const std::string& typ) const;
	 	int Pruefe();
	
		bool operator==(const PersonenKomponente& other) const
			{ return ((id_==other.id_) 
	           && (modifikatoren_==other.modifikatoren_)
	           && (bezeichnung_==other.bezeichnung_)
	           && (voraussetzungen_==other.voraussetzungen_)
	           && (talente_==other.talente_)
	           && (sprachen_==other.sprachen_)
	           && (schriften_==other.schriften_)
	           && (kampftechniken_==other.kampftechniken_)
	           && (zauber_==other.zauber_)
	           && (charakteristika_==other.charakteristika_)
	           && (geschlechtsspezifisch_==other.geschlechtsspezifisch_)
	           && (gp_==other.gp_)); }
		bool operator!=(const PersonenKomponente& other) const { return !operator==(other); }
	
		Modifikatoren modifikatoren_;
	  Voraussetzungen voraussetzungen_;
		Talente talente_;
	  Sprachen sprachen_;
	  Schriften schriften_;
	  Kampftechniken kampftechniken_;
	  Zauberfertigkeiten zauber_;
		Charakteristika charakteristika_;
	  Sonderfertigkeiten sonderfertigkeiten_;
	  StringListe verbilligteSonderfertigkeiten_;
	private:
		std::string id_;
		int gp_;
	  bool geschlechtsspezifisch_;
	  std::string bezeichnung_;
	}; // PersonenKomponente
	//@nonl
	//@-node:class PersonenKomponente
	//@+node:class PersonenKomponenteDefinition
	class PersonenKomponenteDefinition : public Definition {
	public:
	  PersonenKomponenteDefinition(const std::string& typ, int abzug);
	  virtual std::string VirtuelleKlasse() const { return typ_; }
	
	//  void ID(const std::string& id);
	//	std::string ID() const;
	  std::string NurID() const;
	//  void NurID(const std::string& id);
	  
		void GP(int gp) { gp_ = gp; }
		int GP() const { return gp_; }
	
	  void VarianteVon(const std::string& variante) { varianteVon_ = variante; }
	  std::string VarianteVon() const { return varianteVon_; }
	  bool IstVariante() const { return varianteVon_!=""; }
	
	  bool IstGeschlechtsspezifisch() const { return ((NurMaennlich())||(NurWeiblich())); }
	  bool NurMaennlich() const;
	  bool NurWeiblich() const;
	
	  virtual std::string Text(const std::string& trennzeichenDerEinzelne = "\n\n", const std::string& zwischenEinzelneTitelUndSachen = ": ", const std::string& trennzeichenImEinzelnen = ", ", const std::string& wennEinzelnesLeer = "-") const;
	
		int Lese(PersistenzManager* pm, const std::string& id = "");
		int Schreibe(PersistenzManager* pm) const;
		int Pruefe();
		int IDListe(PersistenzManager* pm, StringListe* liste);
	    
		//@  << operator== >>
		//@+node:<< operator== >>
		bool operator==(const PersonenKomponenteDefinition& other) const
			{ return ((Definition::operator==(other))
		         && (modifikatoren_==other.modifikatoren_)
		         && (voraussetzungen_==other.voraussetzungen_)
		         && (kampftechniken_==other.kampftechniken_)
		         && (talente_==other.talente_)
		         && (sprachen_==other.sprachen_)
		         && (schriften_==other.schriften_)
		         && (zauber_==other.zauber_)
		         && (charakteristika_==other.charakteristika_)
		         && (sonderfertigkeiten_==other.sonderfertigkeiten_)
		         && (verbilligteSonderfertigkeiten_==other.verbilligteSonderfertigkeiten_)
		         && (gp_==other.gp_)
		         && (varianteVon_==other.varianteVon_)
		         && (maennlich_==other.maennlich_)
		       && (weiblich_==other.weiblich_)); }
		bool operator!=(const PersonenKomponenteDefinition& other) const { return !operator==(other); }
		//@nonl
		//@-node:<< operator== >>
		//@nl
		//@  << Member >>
		//@+node:<< Member >>
		// nach Demeter raus!
		Modifikatoren modifikatoren_;
		Voraussetzungen voraussetzungen_;
		TalenteAuswahl talente_;
		KampftechnikenAuswahl kampftechniken_;
		SprachenAuswahl sprachen_;
		SchriftenAuswahl schriften_;
		ZauberAuswahl zauber_;
		CharakteristikaAuswahl charakteristika_;
		SonderfertigkeitenAuswahl sonderfertigkeiten_;
		StringListe verbilligteSonderfertigkeiten_;
		private:
		std::string typ_;
		int gp_;
		std::string varianteVon_;
		bool maennlich_;
		bool weiblich_;
		//@nonl
		//@-node:<< Member >>
		//@nl
	}; // PersonenKomponenteDefinition
	//@nonl
	//@-node:class PersonenKomponenteDefinition
	//@-others
  
}

#endif // !defined(PERSONENKOMPONENTE_H)

//@<< Version History >>
//@+node:<< Version History >>
// $Log: PersonenKomponente.h,v $
// Revision 1.15  2004/10/17 19:12:40  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.14  2004/04/09 23:49:55  vrandezo
// Zauberfertigkeiten bei RKP
// Personen verwalten ZF besser
//
// Revision 1.13  2004/04/06 13:57:58  vrandezo
// Begabung und Unfaehigkeit von Talenten und Talentgruppen jetzt
// nach Errata implementiert (Twel)
//
// Revision 1.12  2004/04/04 17:29:27  vrandezo
// Re-Generierungs-bug gefixt
//
// Revision 1.11  2004/01/05 03:35:19  vrandezo
// SF bei RKP-Defs sind jetzt auch Auswahlmengen statt einfacher Mengen
//
// Revision 1.10  2004/01/05 02:45:29  vrandezo
// Charakteristika bei RPK-Definition von Mengen auf Auswahlmengen umgestellt
//
// Revision 1.9  2003/11/28 23:22:49  vrandezo
// speicherbare konkrete RKP
//
// Revision 1.7  2003/10/24 00:51:09  vrandezo
// Leonizing
// hoffentlich RKP-Verknüpfungsbug gelöst
//
// Revision 1.6  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.5  2003/02/24 17:37:34  vrandezo
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
// Revision 1.1.1.1  2003/02/17 15:33:28  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.23  2003/02/09 23:58:12  vrandezo
// - steigerbare VN ohne Wert (Balance->Herausragende Balance)
// - Person-Vergleich korrigiert
// - Voraussetzung-Lesen/Schreiben korrigiert
//
// Revision 1.22  2003/01/26 20:10:40  vrandezo
// operator== für Voraussetzungen verbessert
// Datensätze umbenennen jetzt möglich
// gelöschte Daten werden aus der Sicht entfernt
//
// Revision 1.21  2003/01/26 13:42:22  vrandezo
// Voraussetzungen bei Generierung gültig
// Auswahl aus einer Talentgruppe möglich bei Generierung
// Geschlecht bei Generierung implementiert
//
// Revision 1.20  2003/01/18 18:10:42  vrandezo
// Personen können Sprachen und Schriften
// übliche Kulturen bei Rassen hinzugefügt
// GUI für mögliche und übliche Kulturen und für mögliche Professionen geändert
// Varianten ausblendbar bei der Eingabe möglicher Kulturen und Professionen
//
// Revision 1.19  2003/01/16 22:57:35  vrandezo
// männliche und weibliche Bezeichnungen bei den Professionen
// Geschlechtsspezifische Personenkomponenten (als Varianten realisiert) implementiert
//
// Revision 1.18  2003/01/16 19:35:07  vrandezo
// Variantenbeziehung eintragbar
// Professionen jetzt in die Kategorien aus dem Regelbuch unterteilt
// GUI für Geschlechtsspezifika steht schon
//
// Revision 1.17  2003/01/15 20:02:30  vrandezo
// Voraussetzungen jetzt auch für Rassen und Professionen
// GUI für Modifikatoren bei Personenkomponenten verbessert
// Lesen von Modifikatoren erweitert
//
// Revision 1.16  2003/01/14 22:26:46  vrandezo
// Sprachen und Schriften jetzt auch bei den Personenkomponenten hinzufügbar
//
// Revision 1.15  2003/01/13 00:30:49  vrandezo
// Release Version 1.12 dev
// Korrektur der Verrechnungspunkteberechnung für negative Talentmodifikatoren
// Talentauswahl implementiert
//
// Revision 1.14  2003/01/11 20:30:57  vrandezo
// Auf wxWindows 2.4.0 gewechselt
// Beschreibungen bei Personenkomponenten und Talentoiden hinzugefügt
// PersonenkomponentenGUI verändert
//
// Revision 1.13  2002/12/06 15:28:00  vrandezo
// Spezies bei den Rassen einstellbar
// Kampftechniken bei PK und bei Person hinzugefügt
//
// Revision 1.12  2002/11/05 21:49:18  vrandezo
// Sprachen und Schriften im- und exportierbar
// bool const ueberschreiben aus Speicher-Schnittstelle entfernt
// Mengen haben IDs erhalten
// Voraussetzungen für Talente (allgemeine und meisterliche)
// MengeGUIwx beim Wechseln von Objekten mit Rückfrage
//
// Revision 1.11  2002/11/05 13:12:43  vrandezo
// Spezies bei den Rassen einegführt
// SortierteMenge verallgemeinert
//
// Revision 1.10  2002/11/03 14:25:33  vrandezo
// Wuerfel angepasst
// Haar- und Augenfarbe hinzugefügt bei Rassen
//
// Revision 1.9  2002/11/02 23:36:57  vrandezo
// Exportier und Importierfunktionen implementiert
// Vergleichsoperationen implementiert
// Kampftechnikfenstter implementiert
//
// Revision 1.8  2002/10/25 14:21:12  vrandezo
// Hauptmenü erweitert
// Charakteristika hinzugefügt
// eigenes Mengen-GUI-Element erzeugt
//
// Revision 1.7  2002/10/18 00:50:16  vrandezo
// Talente bei der Heldenerschaffung einsetzbar
//
// Revision 1.6  2002/10/13 15:18:42  vrandezo
// Voraussetzungen für Professionen implementiert
// CVS aufgeräumt für Modifkatoren und Voraussetzungen
//
// Revision 1.5  2002/10/13 00:00:35  vrandezo
// Editor für Kulturen und Professionen eingeführt
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
//
//@nonl
//@-node:<< Version History >>
//@nl

//@-node:@file PersonenKomponente.h
//@-leo
