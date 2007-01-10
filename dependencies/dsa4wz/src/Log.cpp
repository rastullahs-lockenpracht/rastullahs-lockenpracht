//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file Log.cpp
//@+at
// Log.cpp: Implementation of the class Log
// 
// $Revision: 1.4 $
// $Date: 2004/10/17 19:12:38 $
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
#include "Log.h"
#include "NDSAUtil.h"

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
//@+node:Add
void Log::Add(std::string s, severity sev) {
  switch (sev) {
	  case error   : break;
	  case info    : break;
	  case warning : break;
		case debug   : break;
		case fatal   : break;
	}
}
	
//@nonl
//@-node:Add
//@+node:Add
void Log::Add(int i, severity sev) {
	Add(strings::int2sStr(i), sev);
}
//@nonl
//@-node:Add
//@-others

//@<< Version history >>
//@+node:<< Version History >>
// $Log: Log.cpp,v $
// Revision 1.4  2004/10/17 19:12:38  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.3  2003/10/24 13:26:02  vrandezo
// DEBUG_NEW raus
//
// Revision 1.2  2003/10/19 20:22:16  vrandezo
// Eingecheckt in Leo
//
// Revision 1.1.1.1  2003/02/17 15:32:19  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.4  2003/02/14 02:01:18  vrandezo
// Log::Error f�hrt nicht mehr zu Extrafenster in Release-Version
// steigerbare VN mit Wert und Maximalwert
// einzelne VN: Gebildet, Ungebildet, Ausdauernd, Kurzatmig
//
// Revision 1.3  2002/08/30 23:28:24  vrandezo
// Die alte Log-Klasse kompatibel zu wxWindows' Log gemacht
//
// Revision 1.2  2002/08/30 15:06:17  vrandezo
// Apache Xerces ersetzt den MSXML
// Zahlreiche Speicherlecks entfernt
// Entwurf verst�rkt
//
// Revision 1.1  2002/07/17 18:26:03  Zdenko Vrandecic
// Einchecken
//
//@-node:<< Version History >>
//@nl
//@-node:@file Log.cpp
//@-leo
