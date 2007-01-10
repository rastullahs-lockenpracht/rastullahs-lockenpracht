//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file Rasse.h
//@+at
// Rasse.h: Schnittstelle für die Klasse CRasse.
// 
// $Revision: 1.9 $
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

#if !defined(RASSE_H)
#define RASSE_H

//@<< includes >>
//@+node:<< includes >>
#include <map>
#include "NDSABasis.h"
#include "PersonenKomponente.h"
#include "Texte.h"
#include "Menge.h"
//@nonl
//@-node:<< includes >>
//@nl

namespace DSA {

	class PersistenzManager;
	
	//@  @+others
	//@+node:class Farbverteilung
	class Farbverteilung {
	public:
		Farbverteilung(std::string id = "") : id_(id) {}
	
		std::string ID() const { return id_; }
		void ID(const std::string& id) { id_ = id; }
	
		//! gibt eine zufällige Farbe zurück, gemäß der Verteilung
		std::string Farbe() const;
		//! setzt eine Farbe mit dem entsprechenden Anteil
		void Farbe(const std::string& farbe, int anteil);
		//! holt sich die Augenfarbe Nummer nummer
		std::pair<std::string, int> Farbe(int nummer) const
			{ if ((nummer>AnzahlFarben()) || (nummer<1)) return std::pair<std::string, int>("", 0); else return farben_[nummer-1]; }
		//! gibt den Anteil der entsprechenden Farbe zurück
		int Farbe(const std::string& farbe) const;
		//! gibt die Anzahl verschiedener Farben zurück
		int AnzahlFarben() const { return farben_.size(); }
		int SummeAnteile() const;
	
		std::string Text(const std::string& trennzeichen = ", ", const std::string& vorfarbe = " (", const std::string& nachfarbe = ")") const;
	
		bool operator==(const Farbverteilung& other) const
			{ return ((id_==other.id_) && (farben_==other.farben_)); }
		bool operator!=(const Farbverteilung& other) const { return !operator==(other); }
	
		int Lese(PersistenzManager* pm, const std::string& id = "");
		int Schreibe(PersistenzManager* pm) const;
		int Pruefe();
		int IDListe(PersistenzManager* pm, StringListe* liste);
	
	private:
		std::vector<std::pair<std::string, int> > farben_;
		std::string id_;
	}; // Farbverteilung
	//@nonl
	//@-node:class Farbverteilung
	//@+node:class RasseBasis
	class RasseBasis {
	public:
		RasseBasis()
			: augenfarbe_(("Augenfarbe")), hauptfarbe_(("Haarfarbe")), groesseBasis_(160),
			  groesseW6_(0), groesseW20_(2), alterBasis_(15), alterW6_(0), alterW3_(1), 
				gewichtDifferenz_(100) {}
	
		int Groesse() const;
		std::string GroesseText(bool mitRahmen = true) const;
		int Alter() const;
		std::string AlterText(bool mitRahmen = true) const;
	
		bool operator==(const RasseBasis& other) const
			{ return ((alterBasis_==other.alterBasis_)
	           && (alterW3_==other.alterW3_)
	           && (augenfarbe_==other.augenfarbe_)
	           && (gewichtDifferenz_==other.gewichtDifferenz_)
	           && (groesseBasis_==other.groesseBasis_)
	           && (groesseW20_==other.groesseW20_)
	           && (groesseW6_==other.groesseW6_)
	           && (hauptfarbe_==other.hauptfarbe_)); }
		bool operator!=(const RasseBasis& other) const { return !operator==(other); }
	
	  int Pruefe();
	  
		// nach Demeter raus!
		Farbverteilung augenfarbe_;
		Farbverteilung hauptfarbe_;
		int groesseBasis_;
		int groesseW6_;
		int groesseW20_;
		int alterBasis_;
		int alterW3_;
		int alterW6_;
		int gewichtDifferenz_;
	}; // RasseBasis
	//@nonl
	//@-node:class RasseBasis
	//@+node:class Rasse
	class Rasse : public PersonenKomponente, public RasseBasis {
	  public:
	    bool operator!=(const Rasse& other) const { return (PersonenKomponente::operator!=(other) && RasseBasis::operator!=(other)); }
	
		int Lese(PersistenzManager* pm, const std::string& id = "");
		int Schreibe(PersistenzManager* pm) const;
	}; // Rasse
	//@nonl
	//@-node:class Rasse
	//@+node:class RasseDefinition
	class RasseDefinition : public PersonenKomponenteDefinition, public RasseBasis {
	public:
		RasseDefinition()	: PersonenKomponenteDefinition(Klasse(), Verrechnungspunkteabzug()), spezies_(("SpeziesMenschen")) {}
	
		static std::string Klasse() { return ("Rasse"); }
		static std::string Klassen() { return ("Rassen"); }
	
		typedef std::string Kriterium;
		static std::string Kriteriumsart() { return ("Spezies"); }
		static std::string Kriteriumstring(const std::string& s) { return s; }
		std::string ZuKriterium() const { return spezies_; }
		bool IstKriterium(const std::string& s) const { return s == spezies_; }
		void SetzeKriterium(const std::string& s) { spezies_ = s; }
	  
		static int Verrechnungspunkteabzug() { return 85; } // TODO Magische Konstante
	
	  virtual std::string Text(const std::string& trennzeichenDerEinzelne = "\n\n", const std::string& zwischenEinzelneTitelUndSachen = ": ", const std::string& trennzeichenImEinzelnen = ", ", const std::string& wennEinzelnesLeer = "-") const;
	
	  bool operator==(const RasseDefinition& other) const
			{ return ((PersonenKomponenteDefinition::operator==(other))
	           && (spezies_==other.spezies_)
	           && (RasseBasis::operator ==(other))
	           && (ueblicheKulturen_==other.ueblicheKulturen_)
	           && (moeglicheKulturen_==other.moeglicheKulturen_)); }
		bool operator!=(const RasseDefinition& other) const { return !operator==(other); }
	
		int Lese(PersistenzManager* pm, const std::string& id = "");
		int Schreibe(PersistenzManager* pm) const;
	  int Pruefe();
	
		// nach Demeter raus!
		StringListe moeglicheKulturen_;
	  StringListe ueblicheKulturen_;
		std::string spezies_;
	}; // RasseDefinition
	//@nonl
	//@-node:class RasseDefinition
	//@-others
  
	class RasseDefinitionen : public SortierteMenge<RasseDefinition> {
	}; // RasseDefinitionen

	RasseDefinitionen* rassen();
}

#endif // !defined(RASSE_H)

//@<< Version History >>
//@+node:<< Version History >>
// $Log: Rasse.h,v $
// Revision 1.9  2004/10/17 19:12:40  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.8  2003/11/28 23:22:49  vrandezo
// speicherbare konkrete RKP
//
// Revision 1.6  2003/10/27 18:24:50  vrandezo
// unsichtbare Vor- und Nachteile
// Tänze fremder Repräsentationen teurer
//
// Revision 1.5  2003/10/24 00:51:09  vrandezo
// Leonizing
// hoffentlich RKP-Verknüpfungsbug gelöst
//
// Revision 1.4  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.3  2003/02/24 17:37:40  vrandezo
// Prüfen implementiert
//
// Revision 1.2  2003/02/20 09:19:45  vrandezo
// Prüfe-Signatur geändert
//
// Revision 1.1.1.1  2003/02/17 15:34:13  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.18  2003/02/13 20:13:06  vrandezo
// gcc-Portabilität verbessert
//
// Revision 1.17  2003/01/18 18:10:42  vrandezo
// Personen können Sprachen und Schriften
// übliche Kulturen bei Rassen hinzugefügt
// GUI für mögliche und übliche Kulturen und für mögliche Professionen geändert
// Varianten ausblendbar bei der Eingabe möglicher Kulturen und Professionen
//
// Revision 1.16  2003/01/11 20:30:57  vrandezo
// Auf wxWindows 2.4.0 gewechselt
// Beschreibungen bei Personenkomponenten und Talentoiden hinzugefügt
// PersonenkomponentenGUI verändert
//
// Revision 1.15  2002/11/05 21:49:18  vrandezo
// Sprachen und Schriften im- und exportierbar
// bool const ueberschreiben aus Speicher-Schnittstelle entfernt
// Mengen haben IDs erhalten
// Voraussetzungen für Talente (allgemeine und meisterliche)
// MengeGUIwx beim Wechseln von Objekten mit Rückfrage
//
// Revision 1.14  2002/11/05 13:12:43  vrandezo
// Spezies bei den Rassen einegführt
// SortierteMenge verallgemeinert
//
// Revision 1.13  2002/11/03 19:48:00  vrandezo
// Größe, Gewicht, Alter eingefügt
//
// Revision 1.12  2002/11/03 17:48:36  vrandezo
// Haarfarbe und Augenfarbe jetzt auch bei der Heldenerschaffung
//
// Revision 1.11  2002/11/03 14:25:33  vrandezo
// Wuerfel angepasst
// Haar- und Augenfarbe hinzugefügt bei Rassen
//
// Revision 1.10  2002/11/02 23:36:57  vrandezo
// Exportier und Importierfunktionen implementiert
// Vergleichsoperationen implementiert
// Kampftechnikfenstter implementiert
//
// Revision 1.9  2002/10/27 17:29:21  vrandezo
// Die meisten Teile sind speicherbar
//
// Revision 1.8  2002/10/25 15:36:16  vrandezo
// PersonenkomponentenGUIs an neue MengenGUI angepasst
//
// Revision 1.7  2002/10/20 00:20:47  vrandezo
// Mögliche Kulturen und Professionen implementiert
//
// Revision 1.6  2002/10/19 18:58:33  vrandezo
// Texte in eigene, externe Datei untergebracht
//
// Revision 1.5  2002/10/11 14:13:23  vrandezo
// Modifikatoren für Eigenschaften und Abgeleitete Werte eingeführt
//
// Revision 1.4  2002/10/10 21:41:38  vrandezo
// PersonenKomponente als Grundlage für Rasse, Kultur und Profession eingeführt
// Toten Code aus dem Projekt entfernt (noch nicht aus dem CVS)
//
// Revision 1.3  2002/10/02 01:28:57  vrandezo
// Erweiterung von GUI und DSA.lib
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
//@-node:@file Rasse.h
//@-leo
