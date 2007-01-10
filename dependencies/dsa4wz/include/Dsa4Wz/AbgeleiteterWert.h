//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file AbgeleiteterWert.h
//@+at
// AbgeleiteterWert.h: Implementierung für abgeleitete 
// Werte
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

#if !defined(ABGELEITETERWERT_H)
#define ABGELEITETERWERT_H

//@<< includes >>
//@+node:<< includes >>
#include "Modifikator.h"
//@nonl
//@-node:<< includes >>
//@nl

namespace DSA {

	class Person;
	//@  << class AbgeleiteterWert >>
	//@+node:<< class AbgeleiteterWert >>
	class AbgeleiteterWert {
	public:
		AbgeleiteterWert(AbgeleiteterWertArt art = Lebensenergie, Person* person = 0) :
			person_(person), art_(art),
			id_(AbgeleiteterWertString(art))	{};
	
		void ID(const std::string& art)
			{ art_ = AbgeleiteterWertString(art); id_ = art;	}
		std::string ID() const { return id_; }
	
		void Art(AbgeleiteterWertArt art)
			{ art_ = art; id_ = AbgeleiteterWertString(art);	}
		AbgeleiteterWertArt Art() const	{ return art_; }
		int Basiswert() const;
		void Bezugsperson(Person* person) { person_ = person; }
	
		int Wert() const;
		std::string Text() const;
	
		bool operator==(const AbgeleiteterWert& other) const
			{ return ((id_==other.id_) && (art_==other.art_) && (modifikatoren_==other.modifikatoren_)); }
		bool operator!=(const AbgeleiteterWert& other) const { return !operator==(other); }
	
		//@  << Lese/Schreibe >>
		//@+node:<< Lese/Schreibe >>
		int Lese(PersistenzManager* pm, const std::string& id = "");
		int Schreibe(PersistenzManager* pm) const;
		int Pruefe();
		static int IDListe(PersistenzManager* pm, StringListe* liste);
		//@nonl
		//@-node:<< Lese/Schreibe >>
		//@nl
	
	  // sollte versteckt werden (Demeter)
	  Modifikatoren modifikatoren_;
	
	//@<<private>>
	//@+node:<<private>>
	private:
		std::string id_;
		AbgeleiteterWertArt art_;
		Person* person_;    
	
		int Astralenergiebasis() const;
		int Magieresistenzbasis() const;
		int Initiativebasis() const;
		int Fernkampfbasis() const;
		int Paradebasis() const;
		int Attackebasis() const;
		int Ausdauerbasis() const;
		int Lebensenergiebasis() const;
	
	  void BeachteCharakteristika(const std::string& pro, const std::string& contra) const;
	  void BeachteCharakteristikaSO() const;
	//@nonl
	//@-node:<<private>>
	//@nl
	};
	//@nonl
	//@-node:<< class AbgeleiteterWert >>
	//@nl

}

#endif // !defined(ABGELEITETERWERT_H)

//@<< Version History >>
//@+node:<< Version History >>
// $Log: AbgeleiteterWert.h,v $
// Revision 1.8  2004/10/17 19:12:39  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.7  2004/06/23 22:24:19  twel
// Magier und Krieger bekommen SO +1
//
// Revision 1.6  2003/11/28 16:46:16  vrandezo
// Zeilenvorlauf am Dateiende (Warnung bei gcc)
//
// Revision 1.5  2003/10/20 02:12:30  vrandezo
// Gibt AE für magisch begabte Helden
//
// Revision 1.4  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.3  2003/02/24 17:37:21  vrandezo
// Prüfen implementiert
//
// Revision 1.2  2003/02/20 09:19:45  vrandezo
// Prüfe-Signatur geändert
//
// Revision 1.1.1.1  2003/02/17 15:32:34  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.13  2003/02/17 14:10:47  vrandezo
// Beschreibung eintragbar für Helden während der Generierung
// Entsprechende Vor- und Nachteile machen Modifikatoren statt im Basiswert mitberechnet zu werden
//
// Revision 1.12  2002/11/05 21:49:17  vrandezo
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
// Revision 1.10  2002/11/01 15:27:28  vrandezo
// Regeln neu implementiert
// ILader, MLader, IMLader, ILadbar und weitere Klassen entfernt
// XMLChar-Transcoding beschleunigt
//
// Revision 1.9  2002/10/25 14:21:11  vrandezo
// Hauptmenü erweitert
// Charakteristika hinzugefügt
// eigenes Mengen-GUI-Element erzeugt
//
// Revision 1.8  2002/09/28 00:28:37  vrandezo
// Beginn des Generators mit DSA.lib und in wxWindows
//
// Revision 1.7  2002/09/27 19:06:33  vrandezo
// Erweiterung von GUI und DSA.lib
//
// Revision 1.6  2002/08/30 15:06:16  vrandezo
// Apache Xerces ersetzt den MSXML
// Zahlreiche Speicherlecks entfernt
// Entwurf verstärkt
//
// Revision 1.5  2002/08/08 18:51:08  vrandezo
// da Xerces und andere XML-Parser XPath nicht
// unterstützen, wurde die Navigation im DOM-Baum
// von XPath auf eine generische Variante umgestellt.
// Dazu mussten allen Schnittstellen verändert werden.
//
// Revision 1.4  2002/05/30 00:46:52  Zdenko Vrandecic
// MS-spezifischer Code entfernt
//
// Revision 1.3  2002/05/29 12:33:06  Zdenko Vrandecic
// Schreibfehler verbessert
//
// Revision 1.2  2002/05/29 12:30:14  Zdenko Vrandecic
// Removed MS-specific code, replaced license with a pointer to the license
//
// Revision 1.1  2002/05/22 19:38:30  Zdenko Vrandecic
// Startversion
//@nonl
//@-node:<< Version History >>
//@nl
//@-node:@file AbgeleiteterWert.h
//@-leo
