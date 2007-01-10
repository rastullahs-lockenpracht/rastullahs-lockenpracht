//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file Log.h
//@+at
// Log.h: Interface for the Log-Class
// 
// $Author: vrandezo $
// $Revision: 1.4 $
// $Date: 2004/10/17 19:12:39 $
// 
// Manages the log of the Application
// To use this class simply include this header-file to 
// any code that wants to write to the log. You don't need 
// to create an object (actually, you even  can't without 
// tricks). After including you may call the functions you 
// want to use with the syntax Log::function(arg);, e.g. 
// Log::Add("Ups, gone wrong");
// You can access the log per standard in the file 
// logfile.txt in the current directory.
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

#if !defined(LOG_H)
#define LOG_H

#include <string>

//@<< class Log >>
//@+node:<< Class Log >>
class Log {
public:
  enum severity { info, error, warning, debug, fatal };
				
//@+at				
// Adds the string to the log
// Opening the log, adding the string in a new line, and 
// closing the log again. This is to prevent the log from 
// being non-existent should the application crash.
//@-at
//@@c
  static void Add(std::string, severity sev = info);

	static void Info(std::string s) { Add(s, info); }
	static void Error(std::string s) { Add(s, error); }
	static void Warning(std::string s) { Add(s, warning); }
	static void Debug(std::string s) { Add(s, debug); }
	static void Fatal(std::string s) { Add(s, fatal); }

//@+at
// Adds an integer to the log
// refer to Add(string) for more information
//@-at
//@@c
	static void Add(int, severity sev = info);

	static void Info(int s) { Add(s, info); }
	static void Error(int s) { Add(s, error); }
	static void Warning(int s) { Add(s, warning); }
	static void Debug(int s) { Add(s, debug); }
	static void Fatal(int s) { Add(s, fatal); }

private:
//! hidden constructor
  Log() {};
//! hidden destructor
	virtual ~Log() {};
}; // Log
//@-node:<< Class Log >>
//@nl

#endif // !defined(LOG_H)

//@<< Version History >>
//@+node:<< Version History >>
// $Log: Log.h,v $
// Revision 1.4  2004/10/17 19:12:39  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.3  2003/11/28 16:45:03  vrandezo
// Zeilenvorlauf am Dateiende (Warnung bei gcc)
//
// Revision 1.2  2003/10/19 20:22:16  vrandezo
// Eingecheckt in Leo
//
// Revision 1.1.1.1  2003/02/17 15:32:21  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.3  2002/08/30 23:28:24  vrandezo
// Die alte Log-Klasse kompatibel zu wxWindows' Log gemacht
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
//@-node:@file Log.h
//@-leo
