//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file Eigenschaft.cpp
//@+at
// Eigenschaft.cpp: Implementiert Eigenschaft
// 
// $Revision: 1.10 $
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
#include "NDSAUtil.h"
#include "Log.h"
#include "Texte.h"

#include "PersistenzManager.h"

#include "Person.h"

#include "Eigenschaft.h"

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

	//@  << Eigenschaft >>
	//@+node:<< Eigenschaft >>
	//@+others
	//@+node:Wert
	int Eigenschaft::Wert() const {
	  if (person_!=0) {
	    Eigenschaft* ei = const_cast<Eigenschaft*>(this);
	    
	    // Herausragende Eigenschaft
	    bool hatHerausragendeEigenschaft = false;
	    forallconst (Charakteristika, ch, person_->charakteristika_)
	      if (ch->second.Definition()->IstHerausragendeEigenschaft())
	        if (ch->second.Definition()->HerausragendeEigenschaft() == Art()) {
	          if (!modifikatoren_.Existiert(ch->second.ID()))
	            ei->modifikatoren_.FuegeEin(Modifikator(1, ch->second.ID()));
	          ei->modifikatoren_.Zeige(ch->second.ID())->Wert(ch->second.Stufe());
	          hatHerausragendeEigenschaft = true;
	        } 
	    if (!hatHerausragendeEigenschaft)
	        ei->modifikatoren_.Loesche(strings::MacheID("CharakteristikumHerausragendeEigenschaft", ID()));
	
	    // Miserable Eigenschaft
	    bool hatMiserableEigenschaft = false;
	    forallconst (Charakteristika, ch, person_->charakteristika_)
	      if (ch->second.Definition()->IstMiserableEigenschaft())
	        if (ch->second.Definition()->MiserableEigenschaft() == Art()) {
	          if (!modifikatoren_.Existiert(ch->second.ID()))
	            ei->modifikatoren_.FuegeEin(Modifikator(1, ch->second.ID()));
	          ei->modifikatoren_.Zeige(ch->second.ID())->Wert(ch->second.Stufe());
	          hatMiserableEigenschaft = true;
	        } 
	    if (!hatMiserableEigenschaft)
	        ei->modifikatoren_.Loesche(strings::MacheID("CharakteristikumMiserableEigenschaft", ID()));
	  }
	  return modifikatoren_.Verrechne(wert_);
	} // Wert
	//@-node:Wert
	//@+node:Text
	std::string Eigenschaft::Text() {
		return DSA::EigenschaftString(eigenschaft_) + " " + strings::int2sStr(Wert());
	} // Text
	//@nonl
	//@-node:Text
	//@+node:Lese
	int Eigenschaft::Lese(PersistenzManager* pm, const std::string& id) {
		std::string geleseneID = id;
		int rv = pm->OeffneZumLesen(("Eigenschaft"), geleseneID);
		int wert;
		rv += pm->Lese(("Basiswert"), wert);
	  int max;
		rv += pm->Lese(("Maximalwert"), max);
		Modifikatoren modifikatoren;
		rv += modifikatoren.Lese(pm); // TODO kein zweiter Parameter?
		if (!rv) {
	    person_ = 0;
			ID(geleseneID);
			Basiswert(wert);
	    Maximalwert(max);
			modifikatoren_ = modifikatoren; // TODO nach Demeter
		}
		pm->SchliesseNachLesen();
		return rv; // TODO
	} // Lese
	//@nonl
	//@-node:Lese
	//@+node:Schreibe
	int Eigenschaft::Schreibe(PersistenzManager* pm) const {
		int rv = pm->OeffneZumSchreiben(("Eigenschaft"), ID());
		rv += pm->Schreibe(("Wert"), Wert());
		rv += pm->Schreibe(("Basiswert"), Basiswert());
		rv += pm->Schreibe(("Maximalwert"), Maximalwert());
	
		rv += modifikatoren_.Schreibe(pm);
	
		rv += pm->SchliesseNachSchreiben();
	
		return rv; // TODO
	} // Schreibe
	//@nonl
	//@-node:Schreibe
	//@+node:Pruefe
	int Eigenschaft::Pruefe() {
	  int rv = 0;
	  if (Wert()>Maximalwert()) {
	    Log::Info(("Wert größer als Maximalwert"));
	    ++rv;
	  }
	  if (rv!=0) Log::Info("in " + ID());
		return rv;
	} // Pruefe
	//@nonl
	//@-node:Pruefe
	//@+node:IDListe
	int Eigenschaft::IDListe(PersistenzManager* pm, StringListe* liste) {
		Log::Error("Modifikatoren::IDListe " + _t("nicht implementiert"));
		return 0; // TODO
	} // IDListe
	//@nonl
	//@-node:IDListe
	//@-others
	//@nonl
	//@-node:<< Eigenschaft >>
	//@nl

}

//@<< Version History >>
//@+node:<< Version History >>
// $Log: Eigenschaft.cpp,v $
// Revision 1.10  2004/10/17 19:12:39  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.9  2004/04/13 21:45:45  vrandezo
// Akademische Ausbildung (Magier) implementiert
// Aufräumarbeiten (besser in Leo integriert)
//
// Revision 1.8  2004/04/12 21:08:08  vrandezo
// Lehrsprachen implementiert
// Mutter-/Zweitsprachensystem umgebaut
//
// Revision 1.7  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.6  2003/03/24 20:22:56  vrandezo
// Layoutänderungen (viele Anregungen von Tim, Achim, Wolfgang, King Lui, Twelwan...)
// Neu: Miserable Eigenschaften
//
// Revision 1.5  2003/03/24 18:56:18  vrandezo
// Neue Vorteile: Herausragende Eigenschaften (Forumswunsch Sebastian)
//
// Revision 1.4  2003/02/24 17:37:23  vrandezo
// Prüfen implementiert
//
// Revision 1.3  2003/02/20 13:53:22  vrandezo
// Vorteile: Beidhändig, Linkshänder, Blutrausch, Kampfrausch
// weitere SF
// Eigenschaften haben Maximalwerte
//
// Revision 1.2  2003/02/20 09:19:45  vrandezo
// Prüfe-Signatur geändert
//
// Revision 1.1.1.1  2003/02/17 15:32:44  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.13  2002/11/05 21:49:18  vrandezo
// Sprachen und Schriften im- und exportierbar
// bool const ueberschreiben aus Speicher-Schnittstelle entfernt
// Mengen haben IDs erhalten
// Voraussetzungen für Talente (allgemeine und meisterliche)
// MengeGUIwx beim Wechseln von Objekten mit Rückfrage
//
// Revision 1.12  2002/11/01 15:27:28  vrandezo
// Regeln neu implementiert
// ILader, MLader, IMLader, ILadbar und weitere Klassen entfernt
// XMLChar-Transcoding beschleunigt
//
// Revision 1.11  2002/10/19 20:35:32  vrandezo
// feste Stringliterale entfernt
//
// Revision 1.10  2002/10/19 18:58:32  vrandezo
// Texte in eigene, externe Datei untergebracht
//
// Revision 1.9  2002/09/28 00:28:37  vrandezo
// Beginn des Generators mit DSA.lib und in wxWindows
//
// Revision 1.8  2002/09/25 23:50:50  vrandezo
// Beginn der Aufteilung in DSA.lib und Anwendung
//
// Revision 1.7  2002/08/30 15:06:16  vrandezo
// Apache Xerces ersetzt den MSXML
// Zahlreiche Speicherlecks entfernt
// Entwurf verstärkt
//
// Revision 1.6  2002/08/08 18:51:08  vrandezo
// da Xerces und andere XML-Parser XPath nicht
// unterstützen, wurde die Navigation im DOM-Baum
// von XPath auf eine generische Variante umgestellt.
// Dazu mussten allen Schnittstellen verändert werden.
//
// Revision 1.5  2002/06/05 22:38:11  Zdenko Vrandecic
// Auf neues Format für die Regeln angepasst
//
// Revision 1.4  2002/05/30 00:46:52  Zdenko Vrandecic
// MS-spezifischer Code entfernt
//
// Revision 1.3  2002/05/23 02:41:26  Zdenko Vrandecic
// Header aufgeräumt
//
// Revision 1.2  2002/05/22 19:32:54  Zdenko Vrandecic
// Log hinzugefügt
//@nonl
//@-node:<< Version History >>
//@nl
//@nonl
//@-node:@file Eigenschaft.cpp
//@-leo
