//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file detconfig.h
//@+at
// detconfig.h: detconfig bietet Zugriff zu nicht 
// DSA-spezifischen, sondern Applikatonsspezifischen 
// Daten.
// 
// $Revision: 1.2 $
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
#include <string>
#include <map>
//@nonl
//@-node:<< includes >>
//@nl

#if !defined(DETCONFIG_H)
#define DETCONFIG_H

namespace DSA { class PersistenzManager; }

//@<< class detConfig >>
//@+node:<< class detConfig >>
class detConfig {
public:
  static detConfig* Config();
  static void Clear();
  
  void Reset();
  
  bool boolOption(const std::string& option) const;
  void boolOption(const std::string& option, bool b);
  std::string stringOption(const std::string& option) const;
  void stringOption(const std::string& option, const std::string& s);
  
  int Lese(const std::string& datei);
  int Schreibe(const std::string& datei) const;
  
  static std::string Sprache() { return detConfig::Config()->stringOption("Sprache"); }
  
private:
  detConfig();
  virtual ~detConfig() {};
  
  static detConfig* singleton_;

  std::map<std::string, bool> boolOptionen_;
  std::map<std::string, std::string> stringOptionen_;
}; // detConfig
//@nonl
//@-node:<< class detConfig >>
//@nl

#endif // !defined(DETCONFIG_H)

//@<< Version History >>
//@+node:<< Version History >>
// $Log: detconfig.h,v $
// Revision 1.2  2004/10/17 19:12:39  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.1  2004/01/06 03:52:49  vrandezo
// detConfig für Applikationskonfigurationsdaten hinzugefügt
//
//@nonl
//@-node:<< Version History >>
//@nl
//@nonl
//@-node:@file detconfig.h
//@-leo
