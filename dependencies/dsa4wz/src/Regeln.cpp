//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file Regeln.cpp
//@+at
// Regeln.cpp: Implementierung für die Regeln
// 
// $Revision: 1.10 $
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
#include <vector>
#include <algorithm>

#include "NDSAUtil.h"

#include "Log.h"
#include "PersistenzManager.h"

#include "Regeln.h"

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
	//@+node:Default
	void Regeln::Default() {
		regel_.clear();
		Regel("Anzahl bei Generierung aktivierbarer Talente", 5);
		Regel("Durchschnittswert einer Eigenschaft", 10);
		Regel("Maximaler Startwert einer Eigenschaft", 14);
		Regel("Minimaler Startwert einer Eigenschaft", 8);
	  Regel("Üblicher Maximalwert einer Eigenschaft", 21);
		Regel("In Eigenschaften investierbare GP", 100);
		Regel("Aus Nachteilen gewinnbare GP", 50);
		Regel("Aus Schlechten Eigenschaften gewinnbare GP", 30);
		Regel("Durchschnittsstartwert SO", 7);
		Regel("Maximaler Startwert SO", 12);
		Regel("Minimaler Startwert SO", 1);
		Regel("Start-GP", 110);
		Regel("Startdifferenz Klugheit zur Muttersprache", -2);
		Regel("Startdifferenz Klugheit zur Zweitsprache", -4);
		Regel("Maximale Differenz AT/PA", 5);
		Regel("Maximaler Startwert einer Schlechten Eigenschaft", 12);
		Regel("Minimaler Startwert einer Schlechten Eigenschaft", 5);
	      
	  Regeloption("Begabungen und Unfähigkeiten nach Errata", true);
	  
		//@  << SKT >>
		//@+node:<< SKT >>
		// SKT              A    B    C    D    E    F    G    H  (nach MFF 46)
		int buf[AnzahlSKTZeilen][AnzahlSKTSpalten] 
			=						 {{   1,   2,   2,   3,   4,   6,   8,  16 }, //  1
										{   2,   4,   6,   7,   9,  14,  18,  35 }, //  2
										{   3,   6,   9,  12,  15,  22,  30,  60 }, //  3
										{   4,   8,  13,  17,  21,  32,  42,  85 }, //  4
										{   6,  11,  17,  22,  28,  41,  55, 110 }, //  5
										{   7,  14,  21,  27,  34,  50,  70, 140 }, //  6
										{   8,  17,  25,  33,  41,  60,  85, 165 }, //  7
										{  10,  19,  29,  39,  48,  75,  95, 195 }, //  8
										{  11,  22,  34,  45,  55,  85, 110, 220 }, //  9
										{  13,  25,  38,  50,  65,  95, 125, 250 }, // 10
										{  14,  28,  43,  55,  70, 105, 140, 280 }, // 11
										{  16,  32,  47,  65,  80, 120, 160, 320 }, // 12
										{  17,  35,  51,  70,  85, 130, 175, 350 }, // 13
										{  19,  38,  55,  75,  95, 140, 190, 380 }, // 14
										{  21,  41,  60,  85, 105, 155, 210, 410 }, // 15
										{  22,  45,  65,  90, 110, 165, 220, 450 }, // 16
										{  24,  48,  70,  95, 120, 180, 240, 480 }, // 17
										{  26,  51,  75, 105, 130, 195, 260, 510 }, // 18
										{  27,  55,  80, 110, 135, 215, 270, 550 }, // 19
										{  29,  58,  85, 115, 145, 220, 290, 580 }, // 20
										{  31,  62,  95, 125, 155, 230, 310, 620 }, // 21
										{  33,  65, 100, 130, 165, 250, 330, 650 }, // 22
										{  34,  69, 105, 140, 170, 260, 340, 690 }, // 23
										{  36,  73, 110, 145, 180, 270, 360, 720 }, // 24
										{  38,  76, 115, 150, 190, 290, 380, 760 }, // 25
										{  40,  80, 120, 160, 200, 300, 400, 800 }, // 26
										{  42,  84, 125, 165, 210, 310, 420, 830 }, // 27
										{  44,  87, 130, 170, 220, 330, 440, 870 }, // 28
										{  45,  91, 135, 180, 230, 340, 460, 910 }, // 29
										{  47,  95, 140, 190, 240, 350, 480, 950 }, // 30
										{  50, 100, 150, 200, 250, 375, 500,1000 }};// 31+
		double dbuf[AnzahlSKTSpalten]
			=							{	1.f, 2.f, 3.f, 4.f, 5.f,7.5f,10.f,20.f };
		
		    int i = 0;
		for (i=0; i<AnzahlSKTZeilen; ++i)
			for (int j=0; j<AnzahlSKTSpalten; ++j)
			  skt_[i][j] = buf[i][j];
		
		for (i=0; i<AnzahlSKTSpalten; ++i)
			aktivierungsfaktor_[i] = dbuf[i];
		//@nonl
		//@-node:<< SKT >>
		//@nl
	  
		SKTKampf = D;
		SKTKoerperlich = D;
		SKTGesellschaftlich = B;
		SKTNatur = B;
		SKTWissen = B;
		SKTSprachenUndSchriften = A;
		SKTHandwerk = B;
		SKTGaben = G;
	
	} // Default
	//@-node:Default
	//@+node:Regel
	int Regeln::Regel(const std::string& id) const {
		std::map<std::string, int>::const_iterator r = regel_.find(id);
		if (r != regel_.end()) return r->second;
		Log::Error(_t("Unbekannte Regeloption") + " " + id);
		return 0;
	} // Regel
	  
	std::string Regeln::Regel(const int i) const {
	  int it = 1;
	  for (std::map<std::string, int>::const_iterator ci = regel_.begin(); ci != regel_.end(); ++ci, ++it)
	    if (it>i) return ci->first;
	  return 0;
	} // Regel
	//@nonl
	//@-node:Regel
	//@+node:Regeloption
	bool Regeln::Regeloption(const std::string& id) const {
		std::map<std::string, bool>::const_iterator r = regeloption_.find(id);
		if (r != regeloption_.end()) return r->second;
		Log::Error(_t("Unbekannte Regeloption") + " " + id);
		return 0;
	} // Regel
	  
	std::string Regeln::Regeloption(const int i) const {
	  int it = 1;
	  for (std::map<std::string, bool>::const_iterator ci = regeloption_.begin(); ci != regeloption_.end(); ++ci, ++it)
	    if (it>i) return ci->first;
	  return 0;
	} // Regel
	//@nonl
	//@-node:Regeloption
	//@+node:SKT
	int Regeln::SKT(SKTSpalte s, int i) const {
	  if (s==TransA) return max(SKT(A, i)-2, 1);
		if (i<1) return (int) Aktivierungsfaktor(s);
		if (i>AnzahlSKTZeilen) return skt_[31][static_cast<int>(s)-1];
		return skt_[i-1][static_cast<int>(s)-1];
	} // SKT
	
	double Regeln::Aktivierungsfaktor(SKTSpalte s) const {
	  if (s==TransA) return aktivierungsfaktor_[0];
		return aktivierungsfaktor_[static_cast<int>(s)-1];
	} // Aktivierungsfaktor
	
	SKTSpalte Regeln::TalentgruppenSKT(Talentgruppe g) const {
		switch (g) {
			case Kampf								: return SKTKampf;
			case Koerperlich		: return SKTKoerperlich;
			case Gabe									: return SKTGaben;
			case Gesellschaftlich			: return SKTGesellschaftlich;
			case Handwerk					: return SKTHandwerk;
			case Natur								: return SKTNatur;
			case SprachenUndSchriften	: return SKTSprachenUndSchriften;
			case Wissen							: return SKTWissen;
			default           : return D;
		}
	}	// TalentgruppenSKT
	//@nonl
	//@-node:SKT
	//@+node:Lese
	int Regeln::Lese(PersistenzManager* pm, const std::string& id) {
		Default();
		std::string geleseneID = id;
		int rv = pm->OeffneZumLesen(Klassen(), geleseneID);
		StringListe s;
		rv += pm->IDListe(Klasse(), s);
		for (StringListe::iterator i = s.begin(); i != s.end(); ++i) {
			int wert = 0;
			int rv0 = pm->Lese(Klasse(), *i, wert);
			if (rv0==0) Regel(*i, wert);
			rv += rv0;
		}
		rv += pm->SchliesseNachLesen();
		return rv; // TODO
	} // Lese
	//@-node:Lese
	//@+node:Schreibe
	int Regeln::Schreibe(PersistenzManager* pm) const {
		int rv = pm->OeffneZumSchreiben(Klassen());
	
		for (std::map<std::string, int>::const_iterator i = regel_.begin(); i != regel_.end(); ++i)
			rv += pm->Schreibe(Klasse(), i->first, i->second);
	
		rv += pm->SchliesseNachSchreiben();
	
		return rv; // TODO
	} // Schreibe
	//@nonl
	//@-node:Schreibe
	//@+node:Prüfe
	int Regeln::Pruefe() {
		Log::Info("Regeln::Pruefe " + _t("nicht implementiert"));
		return 1; // TODO
	} // Pruefe
	//@nonl
	//@-node:Prüfe
	//@+node:IDListe
	int Regeln::IDListe(PersistenzManager* pm, StringListe* liste) {
		return pm->IDListe(_t("Regeln"), liste); // TODO Prüfen
	} // IDListe
	//@nonl
	//@-node:IDListe
	//@-others
  
	Regeln* regeln() { static Regeln* regeln_ = new Regeln(); return regeln_; }

	int regeln(const std::string& id) { return regeln()->Regel(id); }

}

//@<< Version History >>
//@+node:<< Version History >>
// $Log: Regeln.cpp,v $
// Revision 1.10  2004/10/17 19:12:40  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.9  2004/04/13 21:45:45  vrandezo
// Akademische Ausbildung (Magier) implementiert
// Aufräumarbeiten (besser in Leo integriert)
//
// Revision 1.8  2004/04/10 23:01:28  vrandezo
// SKT Spalte A* eingefuehrt
//
// Revision 1.7  2004/04/06 10:42:25  vrandezo
// Regeloptionen eingeführt
//
// Revision 1.6  2003/11/12 16:53:25  vrandezo
// KonstantenGUIwx
// RKP sortiert wählbar
//
// Revision 1.5  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.4  2003/02/24 17:37:41  vrandezo
// Prüfen implementiert
//
// Revision 1.3  2003/02/20 13:53:22  vrandezo
// Vorteile: Beidhändig, Linkshänder, Blutrausch, Kampfrausch
// weitere SF
// Eigenschaften haben Maximalwerte
//
// Revision 1.2  2003/02/20 09:19:45  vrandezo
// Prüfe-Signatur geändert
//
// Revision 1.1.1.1  2003/02/17 15:34:15  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.9  2003/02/13 20:13:06  vrandezo
// gcc-Portabilität verbessert
//
// Revision 1.8  2002/11/05 21:49:18  vrandezo
// Sprachen und Schriften im- und exportierbar
// bool const ueberschreiben aus Speicher-Schnittstelle entfernt
// Mengen haben IDs erhalten
// Voraussetzungen für Talente (allgemeine und meisterliche)
// MengeGUIwx beim Wechseln von Objekten mit Rückfrage
//
// Revision 1.7  2002/11/05 13:12:43  vrandezo
// Spezies bei den Rassen einegführt
// SortierteMenge verallgemeinert
//
// Revision 1.6  2002/11/02 15:39:48  vrandezo
// Importfenster hinzugefügt
//
// Revision 1.5  2002/11/01 15:27:29  vrandezo
// Regeln neu implementiert
// ILader, MLader, IMLader, ILadbar und weitere Klassen entfernt
// XMLChar-Transcoding beschleunigt
//
// Revision 1.4  2002/10/20 00:20:47  vrandezo
// Mögliche Kulturen und Professionen implementiert
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
// Revision 1.1  2002/07/17 18:29:55  Zdenko Vrandecic
// Einchecken
//
//@nonl
//@-node:<< Version History >>
//@nl
//@nonl
//@-node:@file Regeln.cpp
//@-leo
