//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file Regeln.h
//@+at
// Regeln.h: Implementierungsschnittstelle für die Regeln
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

#if !defined(REGELN_H)
#define REGELN_H

//@<< includes >>
//@+node:<< includes >>
#include "NDSABasis.h"
#include "Texte.h"
#include <map>
//@nonl
//@-node:<< includes >>
//@nl

namespace DSA {
	class PersistenzManager;
    
	//@  << class Regeln >>
	//@+node:<< class Regeln >>
	class Regeln {
	public:
		Regeln() { Default(); }
	
		static std::string Klassen() { return "Regeloptionen"; }
		static std::string Klasse() { return "Regeloption"; }
	
		void Regel(const std::string& id, int wert) {	regel_[id] = wert; }
		int Regel(const std::string& id) const;
		int operator[](const std::string& id) const { return Regel(id); }
		std::string Regel(const int i) const;
	  std::string operator[](const int i) const { return Regel(i); }
	  int AnzahlRegeln() const { return regel_.size(); }
	  
	  void Regeloption(const std::string& id, bool wert) { regeloption_[id] = wert; }
	  bool Regeloption(const std::string& id) const;
	  std::string Regeloption(const int i) const;
	  int AnzahlRegeloptionen() const { return regeloption_.size(); }
	
		int SKT(SKTSpalte, int) const;
		double Aktivierungsfaktor(DSA::SKTSpalte) const;
		SKTSpalte TalentgruppenSKT(Talentgruppe) const;
	
		void Default();
	
		int Lese(PersistenzManager* pm, const std::string& id = "");
		int Schreibe(PersistenzManager* pm) const;
		int Pruefe();
		static int IDListe(PersistenzManager* pm, StringListe* liste);
	
	private:
		std::map<std::string, int> regel_;
	  std::map<std::string, bool> regeloption_;
		double aktivierungsfaktor_[AnzahlSKTSpalten];
		int skt_[AnzahlSKTZeilen][AnzahlSKTSpalten]; // TODO
	
		// TODO
		DSA::SKTSpalte SKTKampf;
		DSA::SKTSpalte SKTKoerperlich;
		DSA::SKTSpalte SKTGesellschaftlich;
		DSA::SKTSpalte SKTNatur;
		DSA::SKTSpalte SKTWissen;
		DSA::SKTSpalte SKTSprachenUndSchriften;
		DSA::SKTSpalte SKTHandwerk;
		DSA::SKTSpalte SKTGaben;
	
	}; // Regeln
	//@nonl
	//@-node:<< class Regeln >>
	//@nl
  
	Regeln* regeln();
	int regeln(const std::string& id);

}

#endif // !defined(REGELN_H)

//@<< Version History >>
//@+node:<< Version History >>
// $Log: Regeln.h,v $
// Revision 1.9  2004/10/17 19:12:40  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.8  2004/04/13 21:45:45  vrandezo
// Akademische Ausbildung (Magier) implementiert
// Aufräumarbeiten (besser in Leo integriert)
//
// Revision 1.7  2004/04/06 10:42:25  vrandezo
// Regeloptionen eingeführt
//
// Revision 1.6  2003/11/28 16:46:16  vrandezo
// Zeilenvorlauf am Dateiende (Warnung bei gcc)
//
// Revision 1.5  2003/11/12 16:53:25  vrandezo
// KonstantenGUIwx
// RKP sortiert wählbar
//
// Revision 1.4  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.3  2003/02/24 17:37:42  vrandezo
// Prüfen implementiert
//
// Revision 1.2  2003/02/20 09:19:45  vrandezo
// Prüfe-Signatur geändert
//
// Revision 1.1.1.1  2003/02/17 15:34:17  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.5  2002/11/05 21:49:19  vrandezo
// Sprachen und Schriften im- und exportierbar
// bool const ueberschreiben aus Speicher-Schnittstelle entfernt
// Mengen haben IDs erhalten
// Voraussetzungen für Talente (allgemeine und meisterliche)
// MengeGUIwx beim Wechseln von Objekten mit Rückfrage
//
// Revision 1.4  2002/11/01 15:27:29  vrandezo
// Regeln neu implementiert
// ILader, MLader, IMLader, ILadbar und weitere Klassen entfernt
// XMLChar-Transcoding beschleunigt
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
//@-node:@file Regeln.h
//@-leo
