//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file PersistenzManager.h
//@+at
// PersistenzManager.h: Schnittstelle der 
// PersistenzManager
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

#if !defined(PERSISTENZMANAGER_H)
#define PERSISTENZMANAGER_H

//@<< includes >>
//@+node:<< includes >>
#include <string>

#include "Log.h"
#include "NDSABasis.h"
//#include "Texte.h"
#include "Update.h"
//@nonl
//@-node:<< includes >>
//@nl

namespace DSA {
	class PersistenzManager;

	class PersistenzManagerFabrik {
	public:
		virtual ~PersistenzManagerFabrik() {};
		virtual PersistenzManager* Erzeuge() = 0;
	};
    
	//@  << class PersistenzManager >>
	//@+node:<< class PersistenzManager >>
	class PersistenzManager {
	private:
		static PersistenzManagerFabrik* fabrik_;
	public:
		virtual ~PersistenzManager() {};
		static PersistenzManager* Erzeuge() {
			if (!fabrik_) { Log::Fatal("Keine PMFabrik initialisiert"); return 0; }
			return fabrik_->Erzeuge();
		}
		static void SetzeFabrik(PersistenzManagerFabrik* pm) {	delete fabrik_;	fabrik_ = pm;	}
	
		// Schnittstelle des PersistenzManagers	
	  virtual const std::string LaderVersion() const = 0;
		virtual int Lade (const std::string& dateiname) = 0;
		virtual int Speicher (const std::string& dateiname) = 0;
		virtual std::string Version () const = 0;
	  virtual void Version (const std::string& version) = 0;
	
		virtual int OeffneZumLesen (const std::string& tag, std::string& id) = 0;
		virtual int OeffneZumLesen (const std::string& tag) = 0;
		virtual int OeffneZumLesen (const std::string& tag, int nr) = 0;
	  virtual int OeffneZumLesen (const std::string& tag, int nr, std::string& id) = 0;
		virtual int Lese (const std::string& tag, std::string& wert) = 0;
		virtual int Lese (const std::string& tag, int& wert) = 0;
		virtual int Lese (const std::string& tag, const std::string& id, std::string& wert) = 0;
		virtual int Lese (const std::string& tag, const std::string& id, int& wert) = 0;
	  virtual int LeseMehrfach (const std::string& tag, std::string& wert, int nr) = 0;
	  virtual int LeseMehrfach (const std::string& tag, int& wert, int nr) = 0;
	  virtual int Lese (StringListe& liste, const std::string& einzeltag, const std::string& tag, std::string id="") = 0;
	  virtual int Lese (DDict& dict, const std::string& einzeltag, const std::string& tag, std::string id="", const std::string& oberid = "") = 0;
	  virtual int Anzahl (const std::string& tag, const std::string& id = "") = 0;
		virtual int SchliesseNachLesen() = 0;
	
		virtual int OeffneZumSchreiben (const std::string& tag, const std::string& id = "") = 0;
		virtual int OeffneZumSchreiben (const std::string& tag, int nr) = 0;
		virtual int Schreibe (const std::string& tag, const std::string& wert) = 0;
		virtual int Schreibe (const std::string& tag, const int wert) = 0;
		virtual int Schreibe (const std::string& tag, const std::string& id, const std::string& wert) = 0;
		virtual int Schreibe (const std::string& tag, const std::string& id, const int wert) = 0;
	  virtual int SchreibeMehrfach (const std::string& tag, const std::string& wert) = 0;
	  virtual int SchreibeMehrfach (const std::string& tag, int wert) = 0;
	  virtual int Schreibe (const StringListe& liste, const std::string& einzeltag, const std::string& tag, const std::string& id="") = 0;
	  virtual int Schreibe (const DDict& dict, const std::string& einzeltag, const std::string& tag, const std::string& id="") = 0;
		virtual int SchliesseNachSchreiben() = 0;
	
	  virtual int SchreibePI (const std::string& target, const std::string& data = "") = 0;
	
		virtual bool Existiert(const std::string& tag, const std::string& id = "") = 0;
		virtual int Loesche(const std::string& tag, const std::string& id = "") = 0;
		virtual int IDListe(const std::string& tag, StringListe* liste) const = 0;
		virtual int IDListe(const std::string& tag, StringListe& liste) const = 0;
	};
	//@nonl
	//@-node:<< class PersistenzManager >>
	//@nl
  
}

#endif // !defined(PERSISTENZMANAGER_H)

//@<< Version History >>
//@+node:<< Version History >>
// $Log: PersistenzManager.h,v $
// Revision 1.7  2004/10/17 19:12:39  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.6  2004/04/13 21:45:45  vrandezo
// Akademische Ausbildung (Magier) implementiert
// Aufräumarbeiten (besser in Leo integriert)
//
// Revision 1.5  2004/04/04 17:29:27  vrandezo
// Re-Generierungs-bug gefixt
//
// Revision 1.4  2004/01/05 02:45:29  vrandezo
// Charakteristika bei RPK-Definition von Mengen auf Auswahlmengen umgestellt
//
// Revision 1.3  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.2  2003/03/03 21:53:17  vrandezo
// Testdruck möglich
// XML-Export, der mit Gerhard Mayers XSL arbeitet
//
// Revision 1.1.1.1  2003/02/17 15:33:12  vrandezo
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
// Korrektur der Verrechnungspunkteberechnung für negative Talentmodifikatoren
// Talentauswahl implementiert
//
// Revision 1.6  2002/11/01 15:27:29  vrandezo
// Regeln neu implementiert
// ILader, MLader, IMLader, ILadbar und weitere Klassen entfernt
// XMLChar-Transcoding beschleunigt
//
// Revision 1.5  2002/10/27 17:29:21  vrandezo
// Die meisten Teile sind speicherbar
//
// Revision 1.4  2002/10/25 23:06:33  vrandezo
// Speichern erweitert
//
// Revision 1.3  2002/10/19 20:35:32  vrandezo
// feste Stringliterale entfernt
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
//@nonl
//@-node:@file PersistenzManager.h
//@-leo
