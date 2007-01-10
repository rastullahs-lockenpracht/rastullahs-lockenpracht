//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file NDSAUtil.h
//@+at
// NDSAUtil.h: Schnittstelle des Namensraumes DSAUtil, der 
// nüzliche Funktionen für das ganze Projekt enthält.
// 
// $Revision: 1.11 $
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

#if !defined(NDSAUTIL_H)
#define NDSAUTIL_H

#include <string>

//! Nützliche Funktionen zu Strings
/*! Der Namensraum strings kann erweiter werden, um weitere Konvertierungen
 *  verschiedener String-typen zu ermöglichen. Die Visual C++ Version etwa
 *  erweitert diesen Namensraum in der Datei StdAfx.h für einige MFC-typischen
 *  String-typen.
 */
namespace strings
  {
    int sStr2int(const std::string&);
    std::string int2sStr(int, bool PlusVorzeichen = false);
    bool BeginntMit(const std::string& was, const std::string& womit);
    std::string OhneAnfang(const std::string& was, const std::string& anfang);
    std::string OhneAnfangExakt(const std::string& was, const std::string& anfang);
    std::string InKlammern(const std::string& ausdruck);
    std::string MacheID(const std::string& klasse, const std::string& name);
  }

int runden(float);
int max(int, int);
int max(int, int, int);
int min(int, int);
int min(int, int, int);

#endif // !defined(NDSAUTIL_H)

//@<< Version History >>
//@+node:<< Version History >>
// $Log: NDSAUtil.h,v $
// Revision 1.11  2004/10/17 19:12:39  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.10  2004/05/15 20:55:47  dertron
// Überflüssige ";" entfernt (pedantic-Warnung)
//
// Revision 1.9  2004/04/10 22:37:04  vrandezo
// InKlammern-Hilfsfunktion
//
// Revision 1.8  2004/01/11 20:22:52  twel
// mathematische Hilfsfunktionen (diesmal hoffentlich richtig)
//
// Revision 1.7  2004/01/11 19:08:56  twel
// mathematische Hilfsfunktionen
//
// Revision 1.6  2004/01/07 22:50:57  dertron
// Kommentaranfang /* innerhalb von Kommentaren entfernt
//
// Revision 1.5  2003/11/28 16:46:16  vrandezo
// Zeilenvorlauf am Dateiende (Warnung bei gcc)
//
// Revision 1.4  2003/10/25 15:06:03  vrandezo
// Leonizing
//
// Revision 1.3  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.2  2003/03/25 10:29:34  vrandezo
// Hilfsfunktionen OhneAnfang und BeginntMit hinzugefügt
//
// Revision 1.1.1.1  2003/02/17 15:33:10  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.2  2002/10/13 00:00:35  vrandezo
// Editor für Kulturen und Professionen eingeführt
//
// Revision 1.1  2002/07/17 18:26:03  Zdenko Vrandecic
// Einchecken
//
//@nonl
//@-node:<< Version History >>
//@nl
//@-node:@file NDSAUtil.h
//@-leo
