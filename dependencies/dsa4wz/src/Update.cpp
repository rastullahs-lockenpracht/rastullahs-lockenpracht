//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file Update.cpp
//@+at
// Update.cpp: Implementierung der Klasse Update, welche 
// den Datenfile auf den neuesten Stand bringt
// 
// $Revision: 1.8 $
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
#include "Update.h"
#include "PersistenzManager.h"

#include "Log.h"
#include "Texte.h"

#include "Talent.h"
#include "Kampftechnik.h"
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

void Update101(DSA::PersistenzManager* pm);

//@+others
//@+node:Commit
int Update::Commit(DSA::PersistenzManager* pm) {
  Log::Info(_t("Datenversion wird �berpr�ft."));
  std::string ver = pm->Version();
  if (ver=="1.00") {
    Log::Info("Update XDI 1.00 --> 1.01");
    pm->OeffneZumLesen(_t("Inhalt"));
    bool geoeffnet = (pm->OeffneZumLesen(_t("Rassen")) == 0);
    StringListe speziesliste;
    pm->IDListe(_t("Spezies"), speziesliste);
    for (StringListe::iterator sp = speziesliste.begin(); sp != speziesliste.end(); ++sp) {
      pm->OeffneZumLesen(_t("Spezies"), *sp);
      StringListe rassenliste;
      pm->IDListe(_t("Rasse"), rassenliste);
      for (StringListe::iterator ra = rassenliste.begin(); ra != rassenliste.end(); ++ra) {
        pm->OeffneZumLesen(_t("Rasse"), *ra);
        Update101(pm);
        pm->SchliesseNachLesen();
      }
      pm->SchliesseNachLesen();
    }
    if (geoeffnet) pm->SchliesseNachLesen();


    geoeffnet = (pm->OeffneZumLesen(_t("Kulturen")) == 0);
    StringListe kulturenliste;
    pm->IDListe(_t("Kultur"), kulturenliste);
    StringListe::iterator ku;
    for (ku = kulturenliste.begin(); ku != kulturenliste.end(); ++ku) {
      pm->OeffneZumLesen(_t("Kultur"), *ku);
      Update101(pm);
      pm->SchliesseNachLesen();
    }
    if (geoeffnet) pm->SchliesseNachLesen();

    geoeffnet = (pm->OeffneZumLesen(_t("Professionen")) == 0);
    pm->IDListe(_t("Profession"), kulturenliste);
    for (ku = kulturenliste.begin(); ku != kulturenliste.end(); ++ku) {
      pm->OeffneZumLesen(_t("Profession"), *ku);
      Update101(pm);
      pm->SchliesseNachLesen();
    }
    if (geoeffnet) pm->SchliesseNachLesen();

    pm->SchliesseNachLesen();

    pm->Version("1.01");
  }
	Log::Info(_t("Datenversion ist aktuell."));
	return 0;		
}
//@nonl
//@-node:Commit
//@+node:Update101
void Update101(DSA::PersistenzManager* pm) {
  DSA::Talente t;
  t.Lese(pm);
  DSA::AuswahlMenge<DSA::Talent> at;
  for (DSA::Talente::iterator ta = t.begin(); ta != t.end(); ++ta) {
    DSA::Auswahl<DSA::Talent> a;
    a.WertHinzu(ta->second.Wert());
    a.IDHinzu(ta->second.ID());
    at.FuegeEin(a);
  }
  //if (pm->Existiert(_t("Talente")))
    //pm->Loesche(_t("Talente"));
  at.Schreibe(pm);

  DSA::Kampftechniken k;
  k.Lese(pm);
  DSA::AuswahlMenge<DSA::Kampftechnik> kt;
  for (DSA::Kampftechniken::iterator ka = k.begin(); ka != k.end(); ++ka) {
    DSA::Auswahl<DSA::Kampftechnik> a;
    a.WertHinzu(ka->second.Wert());
    a.IDHinzu(ka->second.ID());
    kt.FuegeEin(a);
  }
  //pm->Loesche(_t("Kampftechniken"));
  kt.Schreibe(pm);
}
//@nonl
//@-node:Update101
//@-others

//@<< Version History >>
//@+node:<< Version History >>
// $Log: Update.cpp,v $
// Revision 1.8  2004/10/17 19:12:39  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.7  2004/07/13 18:59:52  twel
// nur ein Test
//
// Revision 1.6  2003/11/28 16:45:03  vrandezo
// Zeilenvorlauf am Dateiende (Warnung bei gcc)
//
// Revision 1.5  2003/10/24 13:26:02  vrandezo
// DEBUG_NEW raus
//
// Revision 1.4  2003/10/19 21:54:19  vrandezo
// In Leo eingecheckt oder letzte Anpassungen daf�r
//
// Revision 1.2  2003/10/19 20:22:16  vrandezo
// Eingecheckt in Leo
//
// Revision 1.1.1.1  2003/02/17 15:32:28  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.10  2003/02/13 20:13:07  vrandezo
// gcc-Portabilit�t verbessert
//
// Revision 1.9  2003/01/13 00:30:49  vrandezo
// Release Version 1.12 dev
// Korrektur der Verrechnungspunkteberechnung f�r negative Talentmodifikatoren
// Talentauswahl implementiert
//
// Revision 1.8  2002/11/02 23:36:57  vrandezo
// Exportier und Importierfunktionen implementiert
// Vergleichsoperationen implementiert
// Kampftechnikfenstter implementiert
//
// Revision 1.7  2002/11/01 15:27:29  vrandezo
// Regeln neu implementiert
// ILader, MLader, IMLader, ILadbar und weitere Klassen entfernt
// XMLChar-Transcoding beschleunigt
//
// Revision 1.6  2002/10/19 20:35:32  vrandezo
// feste Stringliterale entfernt
//
// Revision 1.5  2002/10/10 21:41:38  vrandezo
// PersonenKomponente als Grundlage f�r Rasse, Kultur und Profession eingef�hrt
// Toten Code aus dem Projekt entfernt (noch nicht aus dem CVS)
//
// Revision 1.4  2002/08/30 23:28:24  vrandezo
// Die alte Log-Klasse kompatibel zu wxWindows' Log gemacht
//
// Revision 1.3  2002/08/30 15:06:18  vrandezo
// Apache Xerces ersetzt den MSXML
// Zahlreiche Speicherlecks entfernt
// Entwurf verst�rkt
//
// Revision 1.2  2002/08/08 18:51:09  vrandezo
// da Xerces und andere XML-Parser XPath nicht
// unterst�tzen, wurde die Navigation im DOM-Baum
// von XPath auf eine generische Variante umgestellt.
// Dazu mussten allen Schnittstellen ver�ndert werden.
//
// Revision 1.1  2002/07/28 21:20:15  vrandezo
// Datenformats-Update in eigene Klasse verschoben
//
//@nonl
//@-node:<< Version History >>
//@nl
//@-node:@file Update.cpp
//@-leo
