//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file Definition.h
//@+at
// Definition.h: Virtuelle Oberklasse für alle 
// Definitionen
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

#if !defined(DEFINITION_H)
#define DEFINITION_H

//@<< includes >>
//@+node:<< includes >>
#include "Texte.h"
//@nonl
//@-node:<< includes >>
//@nl

namespace DSA {
  class PersistenzManager;
    
	//@  << Definition >>
	//@+node:<< Definition >>
	class Definition {
	public:
	  Definition(const std::string& id) : id_(id) {}
	  virtual ~Definition() {};
	  
	  virtual std::string VirtuelleKlasse() const = 0;
	
	  void ID(const std::string& id);
	  std::string ID() const { return id_; }
	  void Bezeichnung(const std::string& bez, const std::string& sprache = DSA::Lang());
	  std::string Bezeichnung(const std::string& sprache = DSA::Lang()) const;
	
	  void Beschreibung(const std::string& beschreibung, const std::string& sprache = DSA::Lang());
	   std::string Beschreibung(const std::string& sprache = DSA::Lang()) const;
	
	  bool operator==(const Definition& other) const
	  { return ((id_==other.id_)
	         && (beschreibung_==other.beschreibung_)
	         && (bezeichnung_==other.bezeichnung_)); }
	  bool operator!=(const Definition& other) const { return !operator==(other); }
	
	  int LeseBezeichnung(PersistenzManager* pm, std::string& geleseneID, Dict& bezeichnung, Dict& beschreibung);
	  int SchreibeBezeichnung(PersistenzManager* pm) const; 
	
	protected:
	  std::string id_;
	  Dict bezeichnung_;
	  Dict beschreibung_;
	};
	//@-node:<< Definition >>
	//@nl
  
}

#endif // !defined(DEFINITION_H)

//@<< Version History >>
//@+node:<< Version History >>
// $Log: Definition.h,v $
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
//@-node:@file Definition.h
//@-leo
