//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file Voraussetzung.h
//@+at
// Voraussetzung.h: Implementation für die Voraussetzung
// 
// $Revision: 1.8 $
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

#if !defined(VORAUSSETZUNG_H)
#define VORAUSSETZUNG_H

//@<< includes >>
//@+node:<< includes >>
#include "NDSABasis.h"
#include "Texte.h"
#include "Menge.h"
//@nonl
//@-node:<< includes >>
//@nl

namespace DSA {
	class PersistenzManager;
  class Person;
  class Voraussetzungen;
  
	//@  @+others
	//@+node:class Voraussetzung
	class Voraussetzung {
	public:
		Voraussetzung(const std::string& id= "")
			: id_(id), minimal_(false), maximal_(false), kein_(false), minWert_(0), maxWert_(0), alternativ_(0) {}
	  Voraussetzung(const Voraussetzung&);
	  Voraussetzung& operator=(const Voraussetzung&);
	  
	  ~Voraussetzung();
	  
		static std::string Klasse() { return ("Voraussetzung"); }
		static std::string Klassen() { return ("Voraussetzungen"); }
	
		void ID(const std::string& id) { id_ = id; }
		std::string ID() const { return id_; }
		void MinimalWert(int wert)	{ minWert_ = wert; }
		int MinimalWert() const { return minWert_; }
		void MaximalWert(int wert)	{ maxWert_ = wert; }
		int MaximalWert() const { return maxWert_; }
		void Maximal(bool maximal) { maximal_ = maximal; if (maximal) kein_ = false;}
		bool Maximal() const { return maximal_; }
		void Minimal(bool minimal) { minimal_ = minimal; if (minimal) kein_ = false; }
		bool Minimal() const { return minimal_; }
	  void Kein(bool kein) { kein_ = kein; if (kein) minimal_ = maximal_ = false; }
	  bool Kein() const { return kein_; }
	  bool Einfach() const { return !(minimal_ || maximal_ || kein_ ); }
	  
	  void Oder(Voraussetzung vor);
	  Voraussetzung Oder(int i) const;
	  int OderAnzahl() const;
	  void OderLoeschen();
	  void OderLoeschen(int i);
	  bool Oder() const;
	
	  bool Erfuellt(const Person& person) const;
	  
		//@  << operator== >>
		//@+node:<< operator== >>
		bool operator==(const Voraussetzung& other) const;
		bool operator!=(const Voraussetzung& other) const { return !operator==(other); }
		//@nonl
		//@-node:<< operator== >>
		//@nl
	  
		std::string Text() const;
	
		//@  << Lese/Schreibe >>
		//@+node:<< Lese/Schreibe >>
		int Lese(PersistenzManager* pm, const std::string& id = "");
		int Schreibe(PersistenzManager* pm) const;
		int Pruefe();
		static int IDListe(PersistenzManager* pm, StringListe* liste);
		//@nonl
		//@-node:<< Lese/Schreibe >>
		//@nl
	
	private:
		//@  << private >>
		//@+node:<< private >>
		std::string id_;
		int minWert_;
		int maxWert_;
		bool minimal_;
		bool maximal_;
		bool kein_;
		Voraussetzungen* alternativ_;
		
		bool ErfuelltPrivat(const Person& person) const;
		std::string TextID(const std::string&) const;
		//@nonl
		//@-node:<< private >>
		//@nl
	}; // Voraussetzung
	//@-node:class Voraussetzung
	//@+node:class Voraussetzungen
	class Voraussetzungen : public Menge<Voraussetzung> {
	public:
		Voraussetzungen(const std::string& id = "") :  Menge<Voraussetzung>(id) {}
		bool FuegeEin(Voraussetzung voraussetzung);
	  bool Erfuellt(const Person& person) const;
	}; // Voraussetzungen
	//@nonl
	//@-node:class Voraussetzungen
	//@-others
  
}

#endif // !defined(VORAUSSETZUNG_H)

//@<< Version History >>
//@+node:<< Version History >>
// $Log: Voraussetzung.h,v $
// Revision 1.8  2004/10/17 19:12:40  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.7  2004/04/11 21:13:06  vrandezo
// Voraussetzungen koennen jetzt mit "oder" verknuepft werden
//
// Revision 1.6  2003/11/28 16:46:16  vrandezo
// Zeilenvorlauf am Dateiende (Warnung bei gcc)
//
// Revision 1.5  2003/10/24 23:50:41  vrandezo
// Leonizing
// Vor-/Nachteile als Voraussetzung ermöglicht
// Astralmacht, ZVH-Zauberer
//
// Revision 1.4  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.3  2003/02/24 17:37:47  vrandezo
// Prüfen implementiert
//
// Revision 1.2  2003/02/20 09:19:45  vrandezo
// Prüfe-Signatur geändert
//
// Revision 1.1.1.1  2003/02/17 15:34:40  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.13  2003/02/16 17:49:32  vrandezo
// Bug entfernt: Button Anteil- bei der Augenfarbe tat nicht
// SF ex- und importierbar
// einfache Voraussetzung (weder "kein X" noch "X min. 7") hinzugefügt
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
// Revision 1.10  2003/01/26 20:10:53  vrandezo
// operator== für Voraussetzungen verbessert
// Datensätze umbenennen jetzt möglich
// gelöschte Daten werden aus der Sicht entfernt
//
// Revision 1.9  2002/12/06 15:28:01  vrandezo
// Spezies bei den Rassen einstellbar
// Kampftechniken bei PK und bei Person hinzugefügt
//
// Revision 1.8  2002/11/05 21:49:19  vrandezo
// Sprachen und Schriften im- und exportierbar
// bool const ueberschreiben aus Speicher-Schnittstelle entfernt
// Mengen haben IDs erhalten
// Voraussetzungen für Talente (allgemeine und meisterliche)
// MengeGUIwx beim Wechseln von Objekten mit Rückfrage
//
// Revision 1.7  2002/11/02 23:36:57  vrandezo
// Exportier und Importierfunktionen implementiert
// Vergleichsoperationen implementiert
// Kampftechnikfenstter implementiert
//
// Revision 1.6  2002/10/25 14:21:12  vrandezo
// Hauptmenü erweitert
// Charakteristika hinzugefügt
// eigenes Mengen-GUI-Element erzeugt
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

//@-node:@file Voraussetzung.h
//@-leo
