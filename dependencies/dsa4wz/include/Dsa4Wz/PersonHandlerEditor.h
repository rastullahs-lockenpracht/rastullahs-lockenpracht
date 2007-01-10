//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file PersonHandlerEditor.h
//@+at
// PersonHandlerEditor.h: Implementation des 
// Personeneditors
// 
// $Revision: 1.4 $
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

#if !defined(PERSONHANDLEREDITOR_H)
#define PERSONHANDLEREDITOR_H

//@<< includes >>
//@+node:<< includes >>
#include "PersonHandler.h"
#include "Log.h"
//@nonl
//@-node:<< includes >>
//@nl

namespace DSA {

	//@  << class PersonHandlerEditor >>
	//@+node:<< class PersonHandlerEditor >>
	class PersonHandlerEditor : public PersonHandler {
	public:
	  PersonHandlerEditor() { Log::Error("PersonHandlerEditor nicht implementiert"); }
	  virtual ~PersonHandlerEditor() {}
	  virtual Person* P() { return person_.get(); }
	  virtual void P(std::auto_ptr<Person> person) { person_ = person; }
	  virtual std::auto_ptr<Person> HolePerson() { return person_; }
	
	  virtual std::string HandlerArt() const { return "Editor"; }
	
	  //virtual bool Steigerbar(Eigenschaftsart eigenschaft) { return true; }
	  //virtual bool Senkbar(Eigenschaftsart eigenschaft) { return true; }
	
	private:
	  std::auto_ptr<Person> person_;
	
	};
	//@nonl
	//@-node:<< class PersonHandlerEditor >>
	//@nl
  
}

#endif // !defined(PERSONHANDLEREDITOR_H)

//@<< Version History >>
//@+node:<< Version History >>
// $Log: PersonHandlerEditor.h,v $
// Revision 1.4  2004/10/17 19:12:39  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.3  2004/04/13 21:45:45  vrandezo
// Akademische Ausbildung (Magier) implementiert
// Aufräumarbeiten (besser in Leo integriert)
//
// Revision 1.2  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.1.1.1  2003/02/17 15:33:33  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.3  2002/10/19 20:35:32  vrandezo
// feste Stringliterale entfernt
//
// Revision 1.2  2002/10/10 21:41:38  vrandezo
// PersonenKomponente als Grundlage für Rasse, Kultur und Profession eingeführt
// Toten Code aus dem Projekt entfernt (noch nicht aus dem CVS)
//
// Revision 1.1  2002/09/28 00:28:37  vrandezo
// Beginn des Generators mit DSA.lib und in wxWindows
//
//@nonl
//@-node:<< Version History >>
//@nl
//@nonl
//@-node:@file PersonHandlerEditor.h
//@-leo
