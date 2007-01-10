//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file Eigenschaft.h
//@+at
// Eigenschaft.h: Implementiert Eigenschaften
// 
// $Revision: 1.9 $
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

#if !defined(EIGENSCHAFT_H)
#define EIGENSCHAFT_H

//@<< includes >>
//@+node:<< includes >>
#include "Modifikator.h"
#include "Regeln.h"
//@nonl
//@-node:<< includes >>
//@nl

namespace DSA {
  class Person;
  
	//@  << class Eigenschaft >>
	//@+node:<< class Eigenschaft >>
	class Eigenschaft {
	public:
	  Eigenschaft() : id_(EigenschaftString(Mut)), person_(0), wert_(0), maximalwert_(regeln("Üblicher Maximalwert einer Eigenschaft")) {};
		//Eigenschaft(Eigenschaftsart eigenschaft = Mut, Person* person = 0, int wert = 0, int maximalwert_ = regeln("Üblicher Maximalwert einer Eigenschaft")) :
			//wert_(wert), person_(person), eigenschaft_(eigenschaft),
			//id_(EigenschaftString(eigenschaft))	{};
	
		void ID(const std::string& eigenschaft)
			{ eigenschaft_ = EigenschaftString(eigenschaft); id_ = eigenschaft;	}
		std::string ID() const { return id_; }
		void Art(Eigenschaftsart eigenschaft)
			{ eigenschaft_ = eigenschaft; id_ = EigenschaftString(eigenschaft);	}
		Eigenschaftsart Art()	const { return eigenschaft_; }
	  void Bezugsperson(Person* person) { person_ = person; }
	
		void Basiswert(const int wert) { wert_ = wert; }
		int Basiswert() const { return wert_; }
		int Maximalwert() const { return maximalwert_; }
	  void Maximalwert(const int max) { maximalwert_ = max; }
	
		int Wert() const;
		std::string Text();
	
		bool operator==(const Eigenschaft& other) const
			{ return ((id_==other.id_) && (eigenschaft_==other.eigenschaft_) && (modifikatoren_==other.modifikatoren_) && (wert_==other.wert_)); }
		bool operator!=(const Eigenschaft& other) const { return !operator==(other); }
	
		int Lese(PersistenzManager* pm, const std::string& id = "");
		int Schreibe(PersistenzManager* pm) const;
		int Pruefe();
		static int IDListe(PersistenzManager* pm, StringListe* liste);
	
		// sollte versteckt werden (Demeter)
		Modifikatoren modifikatoren_;
	private:
		std::string id_;
		Eigenschaftsart eigenschaft_;
		int wert_;
	  int maximalwert_;
	  Person* person_;
	};
	//@nonl
	//@-node:<< class Eigenschaft >>
	//@nl
  
}					

#endif // !defined(EIGENSCHAFT_H)

//@<< Version History >>
//@+node:<< Version History >>
// $Log: Eigenschaft.h,v $
// Revision 1.9  2004/10/17 19:12:39  vrandezo
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
// Revision 1.7  2003/11/28 16:46:16  vrandezo
// Zeilenvorlauf am Dateiende (Warnung bei gcc)
//
// Revision 1.6  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.5  2003/03/24 18:56:18  vrandezo
// Neue Vorteile: Herausragende Eigenschaften (Forumswunsch Sebastian)
//
// Revision 1.4  2003/02/24 17:37:25  vrandezo
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
// Revision 1.1.1.1  2003/02/17 15:32:45  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.12  2002/11/05 21:49:18  vrandezo
// Sprachen und Schriften im- und exportierbar
// bool const ueberschreiben aus Speicher-Schnittstelle entfernt
// Mengen haben IDs erhalten
// Voraussetzungen für Talente (allgemeine und meisterliche)
// MengeGUIwx beim Wechseln von Objekten mit Rückfrage
//
// Revision 1.11  2002/11/02 23:36:57  vrandezo
// Exportier und Importierfunktionen implementiert
// Vergleichsoperationen implementiert
// Kampftechnikfenstter implementiert
//
// Revision 1.10  2002/11/01 15:27:29  vrandezo
// Regeln neu implementiert
// ILader, MLader, IMLader, ILadbar und weitere Klassen entfernt
// XMLChar-Transcoding beschleunigt
//
// Revision 1.9  2002/10/25 14:21:11  vrandezo
// Hauptmenü erweitert
// Charakteristika hinzugefügt
// eigenes Mengen-GUI-Element erzeugt
//
// Revision 1.8  2002/10/02 01:28:57  vrandezo
// Erweiterung von GUI und DSA.lib
//
// Revision 1.7  2002/09/28 00:28:37  vrandezo
// Beginn des Generators mit DSA.lib und in wxWindows
//
// Revision 1.6  2002/09/25 23:50:51  vrandezo
// Beginn der Aufteilung in DSA.lib und Anwendung
//
// Revision 1.5  2002/08/30 15:06:17  vrandezo
// Apache Xerces ersetzt den MSXML
// Zahlreiche Speicherlecks entfernt
// Entwurf verstärkt
//
// Revision 1.4  2002/08/08 18:51:08  vrandezo
// da Xerces und andere XML-Parser XPath nicht
// unterstützen, wurde die Navigation im DOM-Baum
// von XPath auf eine generische Variante umgestellt.
// Dazu mussten allen Schnittstellen verändert werden.
//
// Revision 1.3  2002/05/30 00:46:52  Zdenko Vrandecic
// MS-spezifischer Code entfernt
//
// Revision 1.2  2002/05/22 19:32:54  Zdenko Vrandecic
// Log hinzugefügt
//@nonl
//@-node:<< Version History >>
//@nl
//@-node:@file Eigenschaft.h
//@-leo
