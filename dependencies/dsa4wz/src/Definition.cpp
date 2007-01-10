//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file Definition.cpp
//@+at
// Definition.cpp: Implementierung der virtuellen 
// Basisklasse Definition
// 
// $Revision: 1.1 $
// $Date: 2004/10/17 19:25:08 $
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
#include "Definition.h"
#include "NDSAUtil.h"
#include "PersistenzManager.h"
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

	//@  << Definition >>
	//@+node:<< Definition >>
	void Definition::ID(const std::string& id) {
	  if (strings::BeginntMit(id, VirtuelleKlasse()))
	    id_ = id;
	  else {
	   id_ = strings::MacheID(VirtuelleKlasse(), id);
	  }
	}
	
	void Definition::Beschreibung(const std::string& beschreibung, const std::string& sprache) {
	  beschreibung_[sprache] = beschreibung;
	}
	
	std::string Definition::Beschreibung(const std::string& sprache) const {
	  Dict::const_iterator i = beschreibung_.find(sprache);
	  if (i != beschreibung_.end())
	    return i->second;
	  return "";
	}
	
	//@+others
	//@+node:Bezeichnung
	void Definition::Bezeichnung(const std::string& bez, const std::string& sprache) {
	  bezeichnung_[sprache] = bez;
	}
	
	std::string Definition::Bezeichnung(const std::string& sprache) const {
	  Dict::const_iterator i = bezeichnung_.find(sprache);
	  if (i != bezeichnung_.end())
	    return i->second;
	  i = bezeichnung_.find(DSA::StandardLang());
	  if (i != bezeichnung_.end())
	    return i->second;
	  if (bezeichnung_.size() > 0)
	    return bezeichnung_.begin()->second;
	  if (strings::BeginntMit(ID(), VirtuelleKlasse()))
	    return strings::OhneAnfangExakt(ID(), VirtuelleKlasse());
	  return ID();
	}
	//@-node:Bezeichnung
	//@+node:LeseBezeichnung
	int Definition::LeseBezeichnung(PersistenzManager* pm, std::string& geleseneID, Dict& bezeichnung, Dict& beschreibung) {
	  int rv = 0;
	  std::string sBuf = geleseneID;
	  if (!strings::BeginntMit(geleseneID, VirtuelleKlasse()))
	   geleseneID = strings::MacheID(VirtuelleKlasse(), geleseneID);
	  else
	    if (islower(strings::OhneAnfangExakt(geleseneID, VirtuelleKlasse())[0]))
	     geleseneID = strings::MacheID(VirtuelleKlasse(), geleseneID);
	  if (pm->Existiert("Bezeichnung")) {
	    StringListe langs;
	    rv += pm->IDListe("Bezeichnung", langs);
	    for (StringListe::iterator si = langs.begin(); si != langs.end(); ++si) {
	      rv += pm->Lese("Bezeichnung", *si, sBuf);
	      bezeichnung[*si] = sBuf;
	    }      
	  } else
	    bezeichnung[DSA::StandardLang()] = sBuf; 
	
	  if (pm->Existiert("Beschreibung")) {
	    StringListe langs;
	    rv += pm->IDListe("Beschreibung", langs);
	   if (langs.size()>0) {
	      for (StringListe::iterator si = langs.begin(); si != langs.end(); ++si) {
	        rv += pm->Lese("Beschreibung", *si, sBuf);
	        beschreibung[*si] = sBuf;
	     }
	   } else {
	     rv += pm->Lese("Beschreibung", sBuf);
	     beschreibung[DSA::StandardLang()] = sBuf;
	    }      
	  }
	  
	  return rv;
	}
	//@nonl
	//@-node:LeseBezeichnung
	//@+node:SchreibeBezeichnung
	int Definition::SchreibeBezeichnung(PersistenzManager* pm) const {
	  int rv = 0;
	  if (bezeichnung_.size() > 0)
	    for (Dict::const_iterator i = bezeichnung_.begin(); i != bezeichnung_.end(); ++i)
	      rv += pm->Schreibe("Bezeichnung", i->first, i->second);  
	  if (beschreibung_.size() > 0)
	    for (Dict::const_iterator i = beschreibung_.begin(); i != beschreibung_.end(); ++i)
	      rv += pm->Schreibe("Beschreibung", i->first, i->second);  
	  return rv;
	}
	//@nonl
	//@-node:SchreibeBezeichnung
	//@-others
	//@-node:<< Definition >>
	//@nl
  
}

//@<< Version History >>
//@+node:<< Version History >>
// $Log: Definition.cpp,v $
// Revision 1.1  2004/10/17 19:25:08  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//

//@-node:<< Version History >>
//@nl

//@-node:@file Definition.cpp
//@-leo
