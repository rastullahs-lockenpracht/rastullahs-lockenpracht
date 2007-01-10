//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file PersistenzManagerXerces.h
//@+at
// PersistenzManagerXerces.h: Implemenetation des PM mit 
// Xerces
// 
// $Revision: 1.7 $
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

#if !defined(PERSISTENZMANAGERXERCES_H)
#define PERSISTENZMANAGERXERCES_H

//@<< includes >>
//@+node:<< includes >>
#include "PersistenzManager.h"
#include "LaderXerces.h"
//@nonl
//@-node:<< includes >>
//@nl

namespace DSA {
//@<< class PersistenzManagerXerces >>
//@+node:<< class PersistenzManagerXerces >>
	class PersistenzManagerXerces : public PersistenzManager {
	public:
		PersistenzManagerXerces();
		virtual ~PersistenzManagerXerces();

		// Schnittstelle des PersistenzManagers
    virtual const std::string LaderVersion() const;
		virtual int Lade (const std::string& dateiname);
		virtual int Speicher (const std::string& dateiname);
		virtual std::string Version () const;
    virtual void Version(const std::string& version);

		virtual int OeffneZumLesen (const std::string& tag, std::string& id);
		virtual int OeffneZumLesen (const std::string& tag);
		virtual int OeffneZumLesen (const std::string& tag, int nr);
    virtual int OeffneZumLesen (const std::string& tag, int nr, std::string& id);
    virtual int Lese (const std::string& tag, std::string& wert);
		virtual int Lese (const std::string& tag, int& wert);
		virtual int Lese (const std::string& tag, const std::string& id, std::string& wert);
		virtual int Lese (const std::string& tag, const std::string& id, int& wert);
    virtual int LeseMehrfach (const std::string& tag, std::string& wert, int nr);
    virtual int LeseMehrfach (const std::string& tag, int& wert, int nr);
    virtual int Lese (StringListe& liste, const std::string& einzeltag, const std::string& tag, std::string id);
    virtual int Lese (DDict& dict, const std::string& einzeltag, const std::string& tag, std::string id, const std::string& oberid = "");
    virtual int Anzahl (const std::string& tag, const std::string& id = "");
		virtual int SchliesseNachLesen();

		virtual int OeffneZumSchreiben (const std::string& tag, const std::string& id = "");
		virtual int OeffneZumSchreiben (const std::string& tag, int nr);
		virtual int Schreibe (const std::string& tag, const std::string& wert);
		virtual int Schreibe (const std::string& tag, const int wert);
		virtual int Schreibe (const std::string& tag, const std::string& id, const std::string& wert);
		virtual int Schreibe (const std::string& tag, const std::string& id, const int wert);
    virtual int SchreibeMehrfach (const std::string& tag, const std::string& wert);
    virtual int SchreibeMehrfach (const std::string& tag, int wert);
    virtual int Schreibe (const StringListe& liste, const std::string& einzeltag, const std::string& tag, const std::string& id="");
    virtual int Schreibe (const DDict& dict, const std::string& einzeltag, const std::string& tag, const std::string& id="");
		virtual int SchliesseNachSchreiben();

    virtual int SchreibePI (const std::string& target, const std::string& data = "");

		virtual bool Existiert(const std::string& tag, const std::string& id = "");
		virtual int Loesche(const std::string& tag, const std::string& id = "");
		virtual int IDListe(const std::string& tag, StringListe* liste) const;
		virtual int IDListe(const std::string& tag, StringListe& liste) const;

	private:
		xercesc::DOMElement* knoten_;
		std::auto_ptr<LaderXerces> lader_;
	};
//@nonl
//@-node:<< class PersistenzManagerXerces >>
//@nl
//@<< class PersistenzManagerFabrikXerces >>
//@+node:<< class PersistenzManagerFabrikXerces >>
	class PersistenzManagerFabrikXerces : public PersistenzManagerFabrik {
	public:
		virtual ~PersistenzManagerFabrikXerces() {};
		virtual PersistenzManager* Erzeuge()
			{ return new PersistenzManagerXerces(); }
	};
//@-node:<< class PersistenzManagerFabrikXerces >>
//@nl
}

#endif // !defined(PERSISTENZMANAGERXERCES_H)

//@<< Version History >>
//@+node:<< Version History >>
// $Log: PersistenzManagerXerces.h,v $
// Revision 1.7  2004/10/17 19:12:39  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.6  2004/04/04 17:29:04  vrandezo
// Re-Generierungs-bug gefixt
//
// Revision 1.5  2004/01/05 02:45:49  vrandezo
// Charakteristika bei RPK-Definition von Mengen auf Auswahlmengen umgestellt
//
// Revision 1.4  2003/11/28 16:45:03  vrandezo
// Zeilenvorlauf am Dateiende (Warnung bei gcc)
//
// Revision 1.3  2003/10/19 20:22:16  vrandezo
// Eingecheckt in Leo
//
// Revision 1.2  2003/03/03 21:39:06  vrandezo
// Testdruck m�glich
// XML-Export, der mit Gerhard Mayers XSL arbeitet
//
// Revision 1.1.1.1  2003/02/17 15:32:26  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.9  2003/02/13 22:29:30  vrandezo
// Auf Xerces 2.2.0 geupdatet
//
// Revision 1.8  2003/01/16 16:32:17  vrandezo
// Mutter- und Zweitsprache bei Kultur einstellbar
//
// Revision 1.7  2003/01/13 00:30:49  vrandezo
// Release Version 1.12 dev
// Korrektur der Verrechnungspunkteberechnung f�r negative Talentmodifikatoren
// Talentauswahl implementiert
//
// Revision 1.6  2002/11/02 23:36:57  vrandezo
// Exportier und Importierfunktionen implementiert
// Vergleichsoperationen implementiert
// Kampftechnikfenstter implementiert
//
// Revision 1.5  2002/11/01 15:27:29  vrandezo
// Regeln neu implementiert
// ILader, MLader, IMLader, ILadbar und weitere Klassen entfernt
// XMLChar-Transcoding beschleunigt
//
// Revision 1.4  2002/10/27 17:29:21  vrandezo
// Die meisten Teile sind speicherbar
//
// Revision 1.3  2002/10/25 23:06:33  vrandezo
// Speichern erweitert
//
// Revision 1.2  2002/09/28 00:28:37  vrandezo
// Beginn des Generators mit DSA.lib und in wxWindows
//
// Revision 1.1  2002/09/25 23:52:47  vrandezo
// Beginn der Aufteilung in DSA.lib und Anwendung
//
//@nonl
//@-node:<< Version History >>
//@nl
//@-node:@file PersistenzManagerXerces.h
//@-leo
