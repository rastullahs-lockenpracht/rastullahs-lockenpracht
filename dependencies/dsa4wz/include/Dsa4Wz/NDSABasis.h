//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file NDSABasis.h
//@+at
// NDSABasis.h: Namensraum für die Defintionen von 
// Konstanten und ein paar ständige Deklarationen
// 
// $Revision: 1.8 $
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

#if !defined(NDSABASIS_H)
#define NDSABASIS_H

//@<< includes >>
//@+node:<< includes >>
#include <string>
#include <vector>
#include <map>
//@nonl
//@-node:<< includes >>
//@nl

//! Eine Liste von Strings
typedef std::vector<std::string> StringListe;

//! verwandelt eine StringListe in einen einzelnen String
/*! mit Trennzeichen kann angegeben werden, welches Zeichen zum trennen der einzelnen
 *  Strings der Liste verwendet werden soll. ", " "-" " " sind übliche Werte,
 *  es gehen aber auch andere Sachen.
 */
std::string liste2string(const StringListe& Liste, const std::string& Trennzeichen = ", ", const std::string& fallsLeer = "-");

typedef std::map<std::string, std::string> Dict;
typedef std::map<std::string, Dict> DDict;

std::string Schluessel(const std::string& s, const DDict& d);

//@<< Hilfsmakros >>
//@+node:<< Hilfsmakros >>
// Hilfsmakros

#define forall(typ, var, worin) for (typ::iterator var = worin.begin(); var != worin.end(); ++var)

#define forallconst(typ, var, worin) for (typ::const_iterator var = worin.begin(); var != worin.end(); ++var)

#define forallp(typ, var, worin) for (typ::iterator var = worin->begin(); var != worin->end(); ++var)

#define forallconstp(typ, var, worin) for (typ::const_iterator var = worin->begin(); var != worin->end(); ++var)
//@nonl
//@-node:<< Hilfsmakros >>
//@nl

//@<< namespace DSA >>
//@+node:<< namespace DSA >>
//! der Hauptnamensraum mit DSA-Begriffen
/*! Theoretisch hätten viele dieser Begriffe auch vollständig softe
 *  Daten sein können, welche ebenfalls veränderbar sind wie etwa die Talente.
 *  Sie befinden sich jedoch hier, weil das wahrscheinlich die Effizienz dramatisch
 *  erhöht, und zudem vergleichsweise selten verändert wird - zumindest selten
 *  genug, um eine Neukompilation in Kauf zu nehmen.
 */
namespace DSA
  {
    // Gibt beim ersten Aufruf false zurück, sonst immer true
    bool DatenGeaendert(const bool set = true);
    
		//@    @+others
		//@+node:Eigenschaftsart
		enum Eigenschaftsart
		  { Mut              = 0,
		    Klugheit         = 1,
		    Intuition        = 2,
		    Charisma         = 3,
		    Fingerfertigkeit = 4,
		    Gewandtheit      = 5,
		    Konstitution     = 6,
		    Koerperkraft     = 7,
		    unbekannteEigenschaft
		  };
		    
		//! Gibt den Namen der mitgegebenen Eigenschaftsart als String zurück
		std::string EigenschaftString(Eigenschaftsart, bool lang=true, bool uebersetzt = true);
		
		//! Gibt die Eigenschaftsart des mitgegebenen Strings zurück
		Eigenschaftsart EigenschaftString(const std::string&);
		    
		std::string NormalisiereEigenschaft(const std::string& s);
		
		bool IstEigenschaft(const std::string&);
		//@-node:Eigenschaftsart
		//@+node:AbgeleiteterWertArt
		enum AbgeleiteterWertArt
		  { Lebensenergie,
		    Ausdauer,
		    AttackeBasis,
		    ParadeBasis,
		    FernkampfBasis,
		    InitiativeBasis,
		    Magieresistenz,
		    Astralenergie,
		    Sozialstatus,
		    unbekannterAbgeleiteterWert
		  };
		    
		//! Gibt den Namen des mitgegeben Abgeleiteten Wers als String zurück
		std::string AbgeleiteterWertString(AbgeleiteterWertArt, bool lang=true, bool uebersetzt = true);
		
		//! Gibt AbgeleiteterWert des mitgegebenen Strings zurück
		AbgeleiteterWertArt AbgeleiteterWertString(const std::string&);
		
		std::string NormalisiereAbgeleiteterWert(const std::string& s);
		
		bool IstAbgeleiteterWert(const std::string&);    
		//@nonl
		//@-node:AbgeleiteterWertArt
		//@+node:Talente
		enum Talentart {
		  Basis,
		  Beruf,
		  Spezial,
		  unbekannteTalentart
		};
		
		//! Gibt den Namen der mitgegebenen Talentart als String zurück
		std::string TalentartString(Talentart, bool uebersetzt = true);
		
		//! Gibt die Talentart des mitgegebenen Strings zurück (default: Spezial)
		Talentart TalentartString(const std::string&);
		
		std::string NormalisiereTalentart(const std::string& s);
		
		bool IstTalentart(const std::string&);
		
		enum Talentgruppe { // TODO Gabe, Kampf, SprachenUndSchriften entfernen?
		    Kampf,
		    Koerperlich,
		    Gesellschaftlich,
		    Natur,
		    Wissen,
		    Handwerk,
		    SprachenUndSchriften,
		    Gabe,
		    sonstige
		  };
		        
		//! Gibt den Namen der mitgegebenen Talentgruppe als String zurück
		std::string TalentgruppeString(Talentgruppe, bool Singular = true, bool uebersetzt = true);
		
		//! Gibt die Talentgruppe des mitgegebenen Strings zurück (default: sonstige)
		Talentgruppe TalentgruppeString(const std::string&);
		
		bool IstTalentgruppe(const std::string&);
		
		std::string NormalisiereTalentgruppe(const std::string& s);
		
		enum EBe { KeineBE, BE, BEminus1, BEminus2, BEminus3, BEminus4, BEminus5, BEminus6, BEminus7, BEminus8, BEmal2, BEmal3 };
		        
		std::string EBeString(EBe, bool uebersetzt = true);
		EBe EBeString(const std::string&);
		
		int berechneEBe(const int& behinderung, EBe);
		//@nonl
		//@-node:Talente
		//@+node:Kampf
		enum Kampfart {
		  BewaffneterNahkampf,
		  BewaffneterKampf,
		  WaffenloserKampf,
		  Fernkampf,
		  unbekannteKampfart
		};
		    
		std::string KampfartString(Kampfart, bool uebersetzt = true);
		Kampfart KampfartString(const std::string&);
		
		std::string NormalisiereKampfart(const std::string& s);
		
		bool IstKampfart(const std::string&);
		//@nonl
		//@-node:Kampf
		//@+node:SKT
		enum SKTSpalte { TransA = 0, A=1, B=2, C=3, D=4, E=5, F=6, G=7, H=8 };
		
		SKTSpalte SpalteLeichter(SKTSpalte s);
		SKTSpalte SpalteSchwerer(SKTSpalte s);
		
		//! Gibt die SKT-Spalte als String zurück
		std::string SKTSpalteString(SKTSpalte);
		
		//! Gibt die SKT-Spalte des mitgegebenen Strings zurück (default: H)
		SKTSpalte SKTSpalteString(const std::string&);
		
		//@-node:SKT
		//@+node:Zauberei
		//@+others
		//@+node:Merkmale
		enum Zaubermerkmal {
		  Antimagie,
		  Beschwoerung,
		  Daemonisch,
		  Eigenschaften,
		  Einfluss,
		  Elementar,
		  Form,
		  Geisterwesen,
		  Heilung,
		  Hellsicht,
		  Herbeirufung,
		  Herrschaft,
		  Illusion,
		  Kraft,
		  Limbus,
		  Metamagie,
		  Objekt,
		  Schaden,
		  Telekinese,
		  Temporal,
		  Umwelt,
		  Verstaendigung,
		  unbekanntesMerkmal
		};
		
		std::string ZaubermerkmalString(Zaubermerkmal, bool uebersetzt = true);
		Zaubermerkmal ZaubermerkmalString(const std::string&);
		
		std::string NormalisiereZaubermerkmal(const std::string& s);
		
		bool IstZaubermerkmal(const std::string&);
		//@nonl
		//@-node:Merkmale
		//@+node:Erzdämonen
		enum Erzdaemon {
		  GesamtDaemonisch,
		  Blakharaz,
		  Belhalhar,
		  Charyptoroth,
		  Lolgramoth,
		  Thargunitoth,
		  Amazeroth,
		  Nagrach,
		  Asfaloth,
		  Tasfarelel,
		  Mishkhara,
		  Agrimoth,
		  Belkekel,
		  unbekannteDomaene
		};
		
		std::string ErzdaemonString(Erzdaemon, bool uebersetzt = true);
		Erzdaemon ErzdaemonString(const std::string&);
		
		std::string NormalisiereErzdaemon(const std::string& s);
		
		bool IstErzdaemon(const std::string&);
		//@nonl
		//@-node:Erzdämonen
		//@+node:Elemente
		enum Element {
		  GesamtElementar,
		  Erz,
		  Feuer,
		  Eis,
		  Luft,
		  Wasser,
		  Humus,
		  unbekanntesElement
		};
		
		std::string ElementString(Element, bool uebersetzt = true);
		Element ElementString(const std::string&);
		
		std::string NormalisiereElement(const std::string& s);
		
		bool IstElement(const std::string&);
		//@nonl
		//@-node:Elemente
		//@+node:Repräsentationen
		enum Zauberrepraesentation {
		  Gildenmagier,
		  Elfen,
		  Druiden,
		  Hexen,
		  Geoden,
		  Schelme,
		  Scharlatane,
		  Borbaradianer,
		  AchazKristallomanten,
		  unbekannteRepraesentation
		};
		
		std::string ZauberrepraesentationString(Zauberrepraesentation, bool lang = true, bool uebersetzt = true);
		Zauberrepraesentation ZauberrepraesentationString(const std::string&);
		
		std::string NormalisiereZauberrepraesentation(const std::string& s);
		
		bool IstRepraesentation(const std::string&);
		inline bool IstZauberrepraesentation(const std::string& s) { return IstRepraesentation(s); }
		
		//@-node:Repräsentationen
		//@-others
		//@nonl
		//@-node:Zauberei
		//@-others
       
    enum {
        AnzahlEigenschaften = 8,
        AnzahlAbgeleiteterWerte = 9,
        EigenschaftenProTalentprobe = 3,
        AnzahlSKTSpalten = 8, // TransA zählt nicht!
        AnzahlSKTZeilen = 31,
        AnzahlTalentgruppen = 8, // TODO senken um 3?
        AnzahlTalentarten = 3,
        AnzahlKampfarten = 4,
        AnzahlEBe = 12,
        AnzahlElemente = 7, // inklusive "Alle Elemente"
        AnzahlErzdaemonen = 14, // inklusive "Alle" und "unbekannt"
        AnzahlZaubermerkmale = 22,
        AnzahlZauberrepraesentationen = 9
      };

  }
//@nonl
//@-node:<< namespace DSA >>
//@nl

#endif // !defined(NDSABASIS_H)

//@<< Version History >>
//@+node:<< Version History >>
// $Log: NDSABasis.h,v $
// Revision 1.8  2004/10/17 19:12:39  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.7  2004/04/10 23:01:28  vrandezo
// SKT Spalte A* eingefuehrt
//
// Revision 1.6  2004/01/07 22:50:57  dertron
// Kommentaranfang /* innerhalb von Kommentaren entfernt
//
// Revision 1.5  2003/11/28 16:46:16  vrandezo
// Zeilenvorlauf am Dateiende (Warnung bei gcc)
//
// Revision 1.4  2003/10/25 15:06:03  vrandezo
// Leonizing
//
// Revision 1.3  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.2  2003/03/25 09:37:40  vrandezo
// Begabung für [Talentgruppe]
//
// Revision 1.1.1.1  2003/02/17 15:33:09  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.13  2003/02/07 10:49:34  vrandezo
// Bug: [eine Sprache] und [eine Schrift] führt zu Absturz
//
// Revision 1.12  2003/01/17 01:35:11  vrandezo
// Speichern stark verkürzt, von der Dauer her
// Erinnert beim Beenden des Programms an das Speichern veränderter Daten
//
// Revision 1.11  2002/11/05 21:49:18  vrandezo
// Sprachen und Schriften im- und exportierbar
// bool const ueberschreiben aus Speicher-Schnittstelle entfernt
// Mengen haben IDs erhalten
// Voraussetzungen für Talente (allgemeine und meisterliche)
// MengeGUIwx beim Wechseln von Objekten mit Rückfrage
//
// Revision 1.10  2002/11/02 23:36:57  vrandezo
// Exportier und Importierfunktionen implementiert
// Vergleichsoperationen implementiert
// Kampftechnikfenstter implementiert
//
// Revision 1.9  2002/11/02 11:44:41  vrandezo
// Kampftechnik hinzugefügt
//
// Revision 1.8  2002/11/01 15:27:29  vrandezo
// Regeln neu implementiert
// ILader, MLader, IMLader, ILadbar und weitere Klassen entfernt
// XMLChar-Transcoding beschleunigt
//
// Revision 1.7  2002/10/18 00:50:16  vrandezo
// Talente bei der Heldenerschaffung einsetzbar
//
// Revision 1.6  2002/10/17 15:35:58  vrandezo
// TalentGUI hinzugefügt
//
// Revision 1.5  2002/09/27 19:06:35  vrandezo
// Erweiterung von GUI und DSA.lib
//
// Revision 1.4  2002/09/26 14:52:20  vrandezo
// DSA.lib-Anbindung der GUI geschrieben
//
// Revision 1.3  2002/09/25 23:50:51  vrandezo
// Beginn der Aufteilung in DSA.lib und Anwendung
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
//@-node:@file NDSABasis.h
//@-leo
