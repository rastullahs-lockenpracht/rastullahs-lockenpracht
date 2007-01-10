//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file detconfig.cpp
//@+at
// Update.cpp: Implementierung der Klasse Update, welche 
// den Datenfile auf den neuesten Stand bringt
// 
// $Revision: 1.3 $
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
#include "detconfig.h"
#include "Log.h"
//#include "dsa/Texte.h"
#include "LaderXerces.h"
//@nonl
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

//@<< class detConfig >>
//@+node:<< class detConfig >>
detConfig* detConfig::singleton_ = 0;

detConfig* detConfig::Config() {
  if (singleton_==0) singleton_ = new detConfig();
  return singleton_;
} // Hole Instanz

void detConfig::Clear() {
  delete singleton_;
  singleton_ = 0;
} // Clear
  

detConfig::detConfig() {
  Reset();
} // Ctor

void detConfig::Reset() {
  boolOptionen_ = std::map<std::string, bool>();
  boolOptionen_["Warnung beim Re-Generieren"] = true;
  stringOptionen_ = std::map<std::string, std::string>();
  stringOptionen_["Sprache"] = "Deutsch";
} // Reset

bool detConfig::boolOption(const std::string& option) const {
  std::map<std::string, bool>::const_iterator i = boolOptionen_.find(option);
  if (i != boolOptionen_.end())
    return i->second; 
  Log::Add("Unbekannte Option: " + option);
  return false;
} // boolOption

void detConfig::boolOption(const std::string& option, bool b) {
  boolOptionen_[option] = b;
} // boolOption

std::string detConfig::stringOption(const std::string& option) const {
  std::map<std::string, std::string>::const_iterator i = stringOptionen_.find(option);
  if (i != stringOptionen_.end())
    return i->second; 
  Log::Add("Unbekannte Option: " + option);
  return "";
} // stringOption

void detConfig::stringOption(const std::string& option, const std::string& s) {
  stringOptionen_[option] = s;
} // stringOption

int detConfig::Lese(const std::string& dateiname) {
  Reset();
  LaderXerces* l = new LaderXerces();
  l->Lade(dateiname);
  if (l->DocLeer()) { delete l; return 0; }
  int z = l->AnzahlKnoten(l->Wurzel(), "boolOption");  
  if (z>0) {
    StringListe ids;
    l->IDListe(l->Wurzel(), "boolOption", &ids);
    for (StringListe::iterator s = ids.begin(); s != ids.end(); ++s) {
      xercesc::DOMElement* node = 0;
      StringListe sl;
      sl.push_back("detconfig");
      sl.push_back("boolOption");
      sl.push_back("ID");
      sl.push_back(*s);
      l->gehePfad(sl, &node);
      std::string w;
      l->LeseString(node, &w);
      boolOptionen_[*s] = (w=="ja");
    }    
  }
  z = l->AnzahlKnoten(l->Wurzel(), "stringOption");  
  if (z>0) {
    StringListe ids;
    l->IDListe(l->Wurzel(), "stringOption", &ids);
    for (StringListe::iterator s = ids.begin(); s != ids.end(); ++s) {
      xercesc::DOMElement* node = 0;
      StringListe sl;
      sl.push_back("detconfig");
      sl.push_back("stringOption");
      sl.push_back("ID");
      sl.push_back(*s);
      l->gehePfad(sl, &node);
      std::string w;
      l->LeseString(node, &w);
      stringOptionen_[*s] = w;
    }    
  }
  delete l;
  return 0;
} // Lese

int detConfig::Schreibe(const std::string& dateiname) const {
  LaderXerces* l = new LaderXerces();
  l->NeuesDokument(dateiname, "detconfig");
  for (std::map<std::string, bool>::const_iterator i = boolOptionen_.begin(); i != boolOptionen_.end(); ++i) {
    l->NeuerKnoten(l->Wurzel(), "boolOption", i->first);
    xercesc::DOMElement* node = 0;
    StringListe sl;
    sl.push_back("detconfig");
    sl.push_back("boolOption");
    sl.push_back("ID");
    sl.push_back(i->first);
    l->gehePfad(sl, &node);
    l->SchreibeString(node, (i->second ? "ja" : "nein"));
  }
  for (std::map<std::string, std::string>::const_iterator i = stringOptionen_.begin(); i != stringOptionen_.end(); ++i) {
    l->NeuerKnoten(l->Wurzel(), "stringOption", i->first);
    xercesc::DOMElement* node = 0;
    StringListe sl;
    sl.push_back("detconfig");
    sl.push_back("stringOption");
    sl.push_back("ID");
    sl.push_back(i->first);
    l->gehePfad(sl, &node);
    l->SchreibeString(node, i->second);
  }
  l->Speicher(dateiname);
  return 0;
} // Schreibe
//@-node:<< class detConfig >>
//@nl

//@<< Version History >>
//@+node:<< Version History >>
// $Log: detconfig.cpp,v $
// Revision 1.3  2004/10/17 19:12:39  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.2  2004/01/07 22:02:42  dertron
// Pfad in #include enthielt \, in / geändert
//
// Revision 1.1  2004/01/06 03:52:49  vrandezo
// detConfig für Applikationskonfigurationsdaten hinzugefügt
//
//@-node:<< Version History >>
//@nl
//@nonl
//@-node:@file detconfig.cpp
//@-leo
