//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file NDSAUtil.cpp
//@+at
// NDSAUtil.cpp: Implementierung der Hilfsfunktionen für 
// Strings
// 
// $Revision: 1.16 $
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
#include <sstream>
#include "NDSAUtil.h"
#include "Log.h"

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

//@+others
//@+node:sStr2int

int strings::sStr2int(const std::string& s)
  {
    return atoi(s.c_str());
  }
//@-node:sStr2int
//@+node:int2sStr

std::string strings::int2sStr(int i, bool PlusVorzeichen)
{
  std::ostringstream s;
  if (PlusVorzeichen) s << std::showpos;
  s << i;
  return s.str();
}
//@-node:int2sStr
//@+node:BeginntMit

bool strings::BeginntMit(const std::string& was, const std::string& womit) {
  return was.substr(0, womit.size())==womit;
}
//@-node:BeginntMit
//@+node:MacheID
std::string strings::MacheID(const std::string& klasse, const std::string& name) {
 std::string id = klasse;
  for (std::string::const_iterator ci = name.begin(); ci != name.end(); ++ci)
   switch (*ci) {
     case 'ä' : id += "ae"; break;
     case 'ö' : id += "oe"; break;
     case 'ü' : id += "ue"; break;
     case 'Ä' : id += "Ae"; break;
     case 'Ö' : id += "Oe"; break;
     case 'Ü' : id += "Ue"; break;
     case 'é' : id += "e"; break;
     case 'ê' : id += "e"; break;
     case 'è' : id += "e"; break;
     case 'á' : id += "a"; break;
     case 'â' : id += "a"; break;
     case 'à' : id += "a"; break;
     case 'ó' : id += "o"; break;
     case 'ô' : id += "o"; break;
     case 'ò' : id += "o"; break;
     case 'í' : id += "i"; break;
     case 'î' : id += "i"; break;
     case 'ì' : id += "i"; break;
     case 'ú' : id += "u"; break;
     case 'û' : id += "u"; break;
     case 'ù' : id += "u"; break;
     case 'ß' : id += "ss"; break;
     default  : 
       if (*ci < 0)
         id += *ci; 
       else
         if (isalpha(*ci)) id += *ci;
   }
  return id;
}
//@nonl
//@-node:MacheID
//@+node:OhneAnfang
std::string strings::OhneAnfang(const std::string& was, const std::string& anfang) {
  if (was.size()<(anfang.size()+2)) { Log::Error("strings::OhneAnfang: Nur Anfang"); return was; }
  return was.substr(anfang.size() + 1, was.size()-(anfang.size()+1));
}

std::string strings::OhneAnfangExakt(const std::string& was, const std::string& anfang) {
  if (was.size()<(anfang.size()+1)) { Log::Error("strings::OhneAnfang: Nur Anfang"); return was; }
  return was.substr(anfang.size(), was.size()-(anfang.size()));
}
//@-node:OhneAnfang
//@+node:InKlammern
std::string strings::InKlammern(const std::string& ausdruck) {
  if (BeginntMit(ausdruck, "Merkmalskenntnis")) // TODO Hardcoded
    if ((ausdruck.find("(")<ausdruck.size())&&(ausdruck.find(")")<ausdruck.size()))
      return ausdruck.substr(ausdruck.find_first_of("(")+1, ausdruck.find_last_of(")")-(ausdruck.find_first_of("(")+1));
  
  if ((ausdruck.find("(")<ausdruck.size())&&(ausdruck.find(")")<ausdruck.size()))
    return ausdruck.substr(ausdruck.find_last_of("(")+1, ausdruck.find_last_of(")")-(ausdruck.find_last_of("(")+1));
  Log::Error("Kein geklammerter Ausruck: " + ausdruck);
  return "";
}
//@nonl
//@-node:InKlammern
//@+node:Runden
int runden(float kommazahl) {
  return (int) (kommazahl+.500001f);
}
//@nonl
//@-node:Runden
//@+node:Max und Min
int max(int a, int b) {
  if (a > b)
    return a;
  else
    return b;
}

int max(int a, int b, int c) {
  return max(a, max(b, c));
}

int min(int a, int b) {
  if (a < b)
    return a;
  else
    return b;
}

int min(int a, int b, int c) {
  return min(a, min(b, c));
}
//@nonl
//@-node:Max und Min
//@-others

//@<< Version History >>
//@+node:<< Version History >>
// $Log: NDSAUtil.cpp,v $
// Revision 1.16  2004/10/17 19:12:39  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.15  2004/09/29 00:54:01  josch1710
// *** empty log message ***
//
// Revision 1.14  2004/09/28 23:33:39  josch1710
// Habe ostream eingef?gt, damit showpos richtig referenziert wird.
//
// Revision 1.13  2004/05/01 08:26:19  dertron
// "sprintf()" ersetzt durch einen "ostringstream"
//
// Revision 1.12  2004/04/12 01:43:03  vrandezo
// Anpassungen fuer Merkmalskenntnis (wegen Doppelklammer)
//
// Revision 1.11  2004/04/10 22:37:04  vrandezo
// InKlammern-Hilfsfunktion
//
// Revision 1.10  2004/04/06 13:57:58  vrandezo
// Begabung und Unfaehigkeit von Talenten und Talentgruppen jetzt
// nach Errata implementiert (Twel)
//
// Revision 1.9  2004/04/04 17:29:27  vrandezo
// Re-Generierungs-bug gefixt
//
// Revision 1.8  2004/01/11 20:22:53  twel
// mathematische Hilfsfunktionen (diesmal hoffentlich richtig)
//
// Revision 1.7  2004/01/11 19:08:56  twel
// mathematische Hilfsfunktionen
//
// Revision 1.6  2003/11/28 16:46:16  vrandezo
// Zeilenvorlauf am Dateiende (Warnung bei gcc)
//
// Revision 1.5  2003/11/20 00:35:07  vrandezo
// compilierbarkeit mit gcc verbessert
//
// Revision 1.4  2003/10/25 15:06:03  vrandezo
// Leonizing
//
// Revision 1.3  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.2  2003/03/25 10:29:33  vrandezo
// Hilfsfunktionen OhneAnfang und BeginntMit hinzugefügt
//
// Revision 1.1.1.1  2003/02/17 15:33:09  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.3  2002/10/13 00:00:35  vrandezo
// Editor für Kulturen und Professionen eingeführt
//
// Revision 1.2  2002/08/30 15:06:17  vrandezo
// Apache Xerces ersetzt den MSXML
// Zahlreiche Speicherlecks entfernt
// Entwurf verstärkt
//
// Revision 1.1  2002/07/17 18:26:03  Zdenko Vrandecic
// Einchecken
//
//@nonl
//@-node:<< Version History >>
//@nl
//@-node:@file NDSAUtil.cpp
//@-leo
