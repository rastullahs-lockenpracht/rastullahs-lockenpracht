//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file Modifikator.cpp
//@+at
// Modifikator.cpp: Implementierung von Modifikator.
// 
// $Revision: 1.6 $
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
#include "Texte.h"
#include "Log.h"
#include "NDSAUtil.h"

#include "PersistenzManager.h"
#include <numeric>

#include "Modifikator.h"
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

	//@  << Modifikator >>
	//@+node:<< Modifikator >>
	//@+others
	//@+node:Text
	std::string Modifikator::Text() const {
	  std::string s = "";
	  if (IstEigenschaft(ID()))
	    s = EigenschaftString(EigenschaftString(ID()));
	  if (IstAbgeleiteterWert(ID()))
	    s = AbgeleiteterWertString(AbgeleiteterWertString(ID()));
		return s + " " + strings::int2sStr(Wert(), true);
	} // Text
	
	//@-node:Text
	//@+node:Lese
	int Modifikator::Lese(PersistenzManager* pm, const std::string& id) {
		std::string geleseneID = id;
		int rv = pm->OeffneZumLesen(("Modifikator"), geleseneID);
		int wert;
		rv += pm->Lese(("Wert"), wert);
		if (!rv) {
			ID(geleseneID);
			Wert(wert);
		}
		pm->SchliesseNachLesen();
		return rv; // TODO
	} // Lese
	//@nonl
	//@-node:Lese
	//@+node:Schreibe
	int Modifikator::Schreibe(PersistenzManager* pm) const {
	  if (wert_==0) return 0;
		int rv = pm->OeffneZumSchreiben(Klasse(), ID());
		rv += pm->Schreibe(("Wert"), Wert());
		rv += pm->SchliesseNachSchreiben();
		return rv;
	} // Schreibe
	//@nonl
	//@-node:Schreibe
	//@+node:Pruefe
	int Modifikator::Pruefe() {
	  int rv = 0; // kann nichts schiefgehen, was wir hier testen könnten
	  bool gefunden = false;
	  if (IstEigenschaft(ID()))
	    gefunden = true;
	  if (IstAbgeleiteterWert(ID()))
	    gefunden = true;
	  if (!gefunden) {
	    Log::Info("Modifikator::Pruefe: unbekannter Modifikator auf " + ID());
	    ++rv;
	  }    
	  if (rv!=0) Log::Info("in " + Klasse() + " " + ID());
		return rv;
	} // Pruefe
	
	
	
	//@-node:Pruefe
	//@+node:IDListe
	int Modifikator::IDListe(PersistenzManager* pm, StringListe* liste) {
		return pm->IDListe(_t("Modifikator"), liste); // TODO Prüfen
	} // IDListe
	//@nonl
	//@-node:IDListe
	//@-others
	//@nonl
	//@-node:<< Modifikator >>
	//@nl
  
	int VerrechneModifikator(int wert, const std::pair<std::string, Modifikator> pair) {
		return pair.second.Verrechne(wert);
	} // Hilfsfunktion

	int Modifikatoren::Verrechne(int wert) const {
		return std::accumulate(begin(), end(), wert, VerrechneModifikator);
	} // Verrechne

/*	bool Modifikatoren::FuegeEin(Modifikator modifikator) {
		if (modifikator.Wert()==0)
			if (Existiert(modifikator.ID()))
				return Loesche(modifikator.ID());
			else
				return true;
		return Menge<Modifikator>::FuegeEin(modifikator);
	} // FuegeEin
*/
}

//@<< Version History >>
//@+node:<< Version History >>
// $Log: Modifikator.cpp,v $
// Revision 1.6  2004/10/17 19:12:39  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.5  2004/04/13 21:45:45  vrandezo
// Akademische Ausbildung (Magier) implementiert
// Aufräumarbeiten (besser in Leo integriert)
//
// Revision 1.4  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.3  2003/02/24 17:37:29  vrandezo
// Prüfen implementiert
//
// Revision 1.2  2003/02/20 09:19:45  vrandezo
// Prüfe-Signatur geändert
//
// Revision 1.1.1.1  2003/02/17 15:33:01  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.17  2003/01/15 20:02:30  vrandezo
// Voraussetzungen jetzt auch für Rassen und Professionen
// GUI für Modifikatoren bei Personenkomponenten verbessert
// Lesen von Modifikatoren erweitert
//
// Revision 1.16  2002/11/05 21:49:18  vrandezo
// Sprachen und Schriften im- und exportierbar
// bool const ueberschreiben aus Speicher-Schnittstelle entfernt
// Mengen haben IDs erhalten
// Voraussetzungen für Talente (allgemeine und meisterliche)
// MengeGUIwx beim Wechseln von Objekten mit Rückfrage
//
// Revision 1.15  2002/11/02 23:36:57  vrandezo
// Exportier und Importierfunktionen implementiert
// Vergleichsoperationen implementiert
// Kampftechnikfenstter implementiert
//
// Revision 1.14  2002/10/27 17:29:21  vrandezo
// Die meisten Teile sind speicherbar
//
// Revision 1.13  2002/10/19 20:35:32  vrandezo
// feste Stringliterale entfernt
//
// Revision 1.12  2002/10/19 18:58:32  vrandezo
// Texte in eigene, externe Datei untergebracht
//
// Revision 1.11  2002/10/17 10:54:43  vrandezo
// Talent und Talente eingefügt
//
// Revision 1.10  2002/10/13 15:18:42  vrandezo
// Voraussetzungen für Professionen implementiert
// CVS aufgeräumt für Modifkatoren und Voraussetzungen
//
// Revision 1.9  2002/10/13 00:00:35  vrandezo
// Editor für Kulturen und Professionen eingeführt
//
// Revision 1.8  2002/10/12 11:46:47  vrandezo
// Rasseneditor eingeführt
//
// Revision 1.7  2002/10/11 14:13:20  vrandezo
// Modifikatoren für Eigenschaften und Abgeleitete Werte eingeführt
//
// Revision 1.6  2002/10/10 21:41:38  vrandezo
// PersonenKomponente als Grundlage für Rasse, Kultur und Profession eingeführt
// Toten Code aus dem Projekt entfernt (noch nicht aus dem CVS)
//
// Revision 1.5  2002/09/25 23:50:51  vrandezo
// Beginn der Aufteilung in DSA.lib und Anwendung
//
// Revision 1.4  2002/08/30 15:06:17  vrandezo
// Apache Xerces ersetzt den MSXML
// Zahlreiche Speicherlecks entfernt
// Entwurf verstärkt
//
// Revision 1.3  2002/08/08 18:51:09  vrandezo
// da Xerces und andere XML-Parser XPath nicht
// unterstützen, wurde die Navigation im DOM-Baum
// von XPath auf eine generische Variante umgestellt.
// Dazu mussten allen Schnittstellen verändert werden.
//
// Revision 1.2  2002/05/30 00:46:52  Zdenko Vrandecic
// MS-spezifischer Code entfernt
//
// Revision 1.1  2002/05/22 19:38:30  Zdenko Vrandecic
// Startversion
//@nonl
//@-node:<< Version History >>
//@nl
//@nonl
//@-node:@file Modifikator.cpp
//@-leo
