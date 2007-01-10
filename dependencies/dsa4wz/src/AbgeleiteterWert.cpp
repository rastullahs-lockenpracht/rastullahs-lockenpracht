//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file AbgeleiteterWert.cpp
//@+at
// AbgeleiteterWert.cpp: Implementierung für abgeleitete 
// Werte
// 
// $Revision: 1.14 $
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
#include "AbgeleiteterWert.h"

//#include "IPerson.h"

//#include "IMModifikator.h"
//#include "IMLader.h"

#include "Log.h"
#include "Texte.h"
#include "NDSAUtil.h"

#include "Person.h"
#include "PersistenzManager.h"
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

namespace DSA {

	//@	<< class AbgeleiteterWert >>
	//@+node:<< class AbgeleiteterWert >>
	//@+others
	//@+node:Basiswert
	int AbgeleiteterWert::Basiswert() const {
		if (!person_) return 0;
		switch (Art()) {
			case DSA::Lebensenergie     : return Lebensenergiebasis();
			case DSA::Ausdauer          : return Ausdauerbasis();
			case DSA::AttackeBasis      : return Attackebasis();
			case DSA::ParadeBasis       : return Paradebasis();
			case DSA::FernkampfBasis    : return Fernkampfbasis();
			case DSA::InitiativeBasis   : return Initiativebasis();
			case DSA::Magieresistenz    : return Magieresistenzbasis();
			case DSA::Astralenergie     : return Astralenergiebasis();
			default											: return 0; 
		}
		return 0;
	} // Basiswert
	//@nonl
	//@-node:Basiswert
	//@+node:Wert
	int AbgeleiteterWert::Wert() const {
		switch (Art()) {
	 case DSA::Lebensenergie     : BeachteCharakteristika("CharakteristikumHoheLebenskraft", "CharakteristikumNiedrigeLebenskraft"); break;
			case DSA::Ausdauer          : BeachteCharakteristika("CharakteristikumAusdauernd", "CharakteristikumKurzatmig"); break;
			case DSA::AttackeBasis      : break;
			case DSA::ParadeBasis       : break;
			case DSA::FernkampfBasis    : break;
			case DSA::InitiativeBasis   : break;
			case DSA::Magieresistenz    : BeachteCharakteristika("CharakteristikumHoheMagieresistenz", "CharakteristikumNiedrigeMagieresistenz"); break;
			case DSA::Astralenergie     : BeachteCharakteristika("CharakteristikumAstralmacht", "CharakteristikumNiedrigeAstralkraft"); break;
			case DSA::Sozialstatus      : BeachteCharakteristikaSO(); break;
	          default	            		: break;
		}
	return modifikatoren_.Verrechne(Basiswert());
	} // Wert
	//@nonl
	//@-node:Wert
	//@+node:BeachteCharakteristika
	void AbgeleiteterWert::BeachteCharakteristika
	  (const std::string& pro, const std::string& contra) const {    
	AbgeleiteterWert* ab = const_cast<AbgeleiteterWert*>(this);
	
	if (ab->modifikatoren_.Existiert(pro))
	  if (!person_->charakteristika_.Existiert(pro))
	    ab->modifikatoren_.Loesche(pro);
	if (person_->charakteristika_.Existiert(pro))
	  if (ab->modifikatoren_.Existiert(pro)) {
	    Modifikator* mod = ab->modifikatoren_.Zeige(pro);
	    if (mod->Wert()!=person_->charakteristika_.Hole(pro).Stufe())
	      mod->Wert(person_->charakteristika_.Hole(pro).Stufe());
	  } else
	    ab->modifikatoren_.FuegeEin(
	      Modifikator(person_->charakteristika_.Hole(pro).Stufe(), pro));
	      
	if (ab->modifikatoren_.Existiert(contra))
	  if (!person_->charakteristika_.Existiert(contra))
	    ab->modifikatoren_.Loesche(contra);
	if (person_->charakteristika_.Existiert(contra))
	  if (ab->modifikatoren_.Existiert(contra)) {
	    Modifikator* mod = ab->modifikatoren_.Zeige(contra);
	    if (mod->Wert()!= -person_->charakteristika_.Hole(contra).Stufe())
	      mod->Wert(-person_->charakteristika_.Hole(contra).Stufe());
	  } else
	    ab->modifikatoren_.FuegeEin(
	      Modifikator(-person_->charakteristika_.Hole(contra).Stufe(), contra));
	   
		//@  @+others
		//@+node:Fettleibig
		if (Art()==Ausdauer) {
		  if (person_->charakteristika_.Existiert(("CharakteristikumFettleibig"))) {
		    if (!modifikatoren_.Existiert(("CharakteristikumFettleibig")))
		      ab->modifikatoren_.FuegeEin(Modifikator(-Ausdauerbasis()/2, ("CharakteristikumFettleibig")));
		    else 
		      if (modifikatoren_.Zeige(("CharakteristikumFettleibig"))->Wert() != -Ausdauerbasis()/2)
		        ab->modifikatoren_.Zeige
		          (("CharakteristikumFettleibig"))->Wert(-Ausdauerbasis()/2);
		  } else if (modifikatoren_.Existiert(("CharakteristikumFettleibig")))
		    ab->modifikatoren_.Loesche(("CharakteristikumFettleibig"));
		}
		//@nonl
		//@-node:Fettleibig
		//@+node:Vollzauberer
		if (Art()==Astralenergie) {
		  if (person_->charakteristika_.Existiert(("CharakteristikumVollzauberer"))) {
		    if (!modifikatoren_.Existiert(("CharakteristikumVollzauberer")))
		      ab->modifikatoren_.FuegeEin(Modifikator(12, ("CharakteristikumVollzauberer")));
		      // TODO Magische Konstante
		  } else if (modifikatoren_.Existiert(("CharakteristikumVollzauberer")))
		    ab->modifikatoren_.Loesche(("CharakteristikumVollzauberer"));
		}
		
		if (Art()==Magieresistenz) {
		  if (person_->charakteristika_.Existiert(("CharakteristikumVollzauberer"))) {
		    if (!modifikatoren_.Existiert(("CharakteristikumVollzauberer")))
		      ab->modifikatoren_.FuegeEin(Modifikator(2, ("CharakteristikumVollzauberer")));
		      // TODO Magische Konstante
		  } else if (modifikatoren_.Existiert(("CharakteristikumVollzauberer")))
		    ab->modifikatoren_.Loesche(("CharakteristikumVollzauberer"));
		}
		//@-node:Vollzauberer
		//@+node:Halbzauberer
		if (Art()==Astralenergie) {
		  if (person_->charakteristika_.Existiert(("CharakteristikumHalbzauberer"))) {
		    if (!modifikatoren_.Existiert(("CharakteristikumHalbzauberer")))
		      ab->modifikatoren_.FuegeEin(Modifikator(6, ("CharakteristikumHalbzauberer")));
		      // TODO Magische Konstante
		  } else if (modifikatoren_.Existiert(("CharakteristikumHalbzauberer")))
		    ab->modifikatoren_.Loesche(("CharakteristikumHalbzauberer"));
		}
		
		if (Art()==Magieresistenz) {
		  if (person_->charakteristika_.Existiert(("CharakteristikumHalbzauberer"))) {
		    if (!modifikatoren_.Existiert("CharakteristikumHalbzauberer"))
		      ab->modifikatoren_.FuegeEin(Modifikator(1, ("CharakteristikumHalbzauberer")));
		      // TODO Magische Konstante
		  } else if (modifikatoren_.Existiert(("CharakteristikumHalbzauberer")))
		    ab->modifikatoren_.Loesche(("CharakteristikumHalbzauberer"));
		}
		//@-node:Halbzauberer
		//@+node:Viertelzauberer
		if (Art()==Astralenergie) {
		  if (person_->charakteristika_.Existiert(("CharakteristikumViertelzauberer"))) {
		    if (!modifikatoren_.Existiert(("CharakteristikumViertelzauberer")))
		      ab->modifikatoren_.FuegeEin(Modifikator(-6, ("CharakteristikumViertelzauberer")));
		      // TODO Magische Konstante
		  } else if (modifikatoren_.Existiert(("CharakteristikumViertelzauberer")))
		    ab->modifikatoren_.Loesche(("CharakteristikumViertelzauberer"));
		}
		//@-node:Viertelzauberer
		//@+node:Akademische Ausbildung
		if (Art()==Astralenergie) {
		  if (person_->charakteristika_.Existiert(("CharakteristikumAkademischeAusbildungMagier"))) {
		    ab->modifikatoren_.FuegeEin(Modifikator(6, ("CharakteristikumAkademischeAusbildungMagier")));
		      // TODO Magische Konstante
		  } else if (modifikatoren_.Existiert(("CharakteristikumAkademischeAusbildungMagier")))
		    ab->modifikatoren_.Loesche(("CharakteristikumAkademischeAusbildungMagier"));
		}
		//@-node:Akademische Ausbildung
		//@-others
		}  // BeachteCharakteristika		
	//@nonl
	//@-node:BeachteCharakteristika
	//@+node:Basisberechnungen
	int AbgeleiteterWert::Lebensenergiebasis() const {
		return ((person_->Wert(DSA::Konstitution)
			+person_->Wert(DSA::Konstitution)
			+person_->Wert(DSA::Koerperkraft)+1)/2);
	} // Lebensenergiebasis
	
	int AbgeleiteterWert::Ausdauerbasis() const {
		return ((person_->Wert(DSA::Mut)
			+person_->Wert(DSA::Konstitution)
			+person_->Wert(DSA::Gewandtheit)+1)/2);
	} // Ausdauerbasis
	
	int AbgeleiteterWert::Attackebasis() const {
		return ((person_->Wert(DSA::Mut)
			+person_->Wert(DSA::Gewandtheit)
			+person_->Wert(DSA::Koerperkraft)+2)/5);
	} // Attackebasis
	
	int AbgeleiteterWert::Paradebasis() const {
		return ((person_->Wert(DSA::Intuition)
			+person_->Wert(DSA::Gewandtheit)
			+person_->Wert(DSA::Koerperkraft)+2)/5);
	} // Paradebasis
	
	int AbgeleiteterWert::Fernkampfbasis() const {
		return ((person_->Wert(DSA::Intuition)
			+person_->Wert(DSA::Fingerfertigkeit)
			+person_->Wert(DSA::Koerperkraft)+2)/5);
	} // Fernkampfbasis
	
	int AbgeleiteterWert::Initiativebasis() const {
		return ((person_->Wert(DSA::Mut)
			+person_->Wert(DSA::Mut)
			+person_->Wert(DSA::Intuition)
			+person_->Wert(DSA::Gewandtheit)+2)/5);
	} // Initiativebasis
	
	int AbgeleiteterWert::Magieresistenzbasis() const {
		return ((person_->Wert(DSA::Mut)
			+person_->Wert(DSA::Klugheit)
			+person_->Wert(DSA::Konstitution)+2)/5);
	} // Magieresistenzbasis
	
	int AbgeleiteterWert::Astralenergiebasis() const {
		if (person_->charakteristika_.Verfuegt(("CharakteristikumMagiebegabt")))
			return ((person_->Wert(DSA::Mut)
							+person_->Wert(DSA::Intuition)
							+person_->Wert(DSA::Charisma)+1)/2);
		else
			return 0;
	} // Astralenergiebasis
	//@nonl
	//@-node:Basisberechnungen
	//@+node:Text
	std::string AbgeleiteterWert::Text() const {
		return DSA::AbgeleiteterWertString(art_) + 
			" " + strings::int2sStr(Wert());
	} // Text
	//@nonl
	//@-node:Text
	//@+node:Lese
	int AbgeleiteterWert::Lese(PersistenzManager* pm, const std::string& id) {
		std::string geleseneID = id;
		int rv = pm->OeffneZumLesen(("AbgeleiteterWert"), geleseneID);
		Modifikatoren modifikatoren;
		rv += modifikatoren.Lese(pm); // TODO kein zweiter Parameter?
		if (!rv) {
	    person_=0;
			ID(geleseneID);
			modifikatoren_ = modifikatoren; // TODO nach Demeter
		}
		pm->SchliesseNachLesen();
		return rv; // TODO
	} // Lese
	//@nonl
	//@-node:Lese
	//@+node:Schreibe
	int AbgeleiteterWert::Schreibe(PersistenzManager* pm) const {
		int rv = pm->OeffneZumSchreiben(("AbgeleiteterWert"), ID());
		rv += pm->Schreibe(("Wert"), Wert());
		rv += pm->Schreibe(("Basiswert"), Basiswert());
	
		rv += modifikatoren_.Schreibe(pm);
	
		rv += pm->SchliesseNachSchreiben();
	
		return rv; // TODO
	} // Schreibe
	//@nonl
	//@-node:Schreibe
	//@+node:Pruefe
	int AbgeleiteterWert::Pruefe() {
	int rv = 0;
	if (person_ == 0) {
	  Log::Info(("hängender Abgeleiteter Wert"));
	  ++rv;
	}
	if (rv!=0) Log::Info("in " + ID());
		return rv;
	} // Pruefe
	//@nonl
	//@-node:Pruefe
	//@+node:IDListe
	int AbgeleiteterWert::IDListe(PersistenzManager* pm, StringListe* liste) {
		Log::Error("AbgeleiteterWert::IDListe nicht implementiert");
		return 1; // TODO
	} // IDListe
	//@nonl
	//@-node:IDListe
	//@-others
	
	void AbgeleiteterWert::BeachteCharakteristikaSO() const
	  {    
	    AbgeleiteterWert* ab = const_cast<AbgeleiteterWert*>(this);
	    if (Art()==Sozialstatus) {
		if (person_->charakteristika_.Existiert(("CharakteristikumAkademischeAusbildungMagier"))) {
		  ab->modifikatoren_.FuegeEin(Modifikator(1, ("CharakteristikumAkademischeAusbildungMagier")));
		    // TODO Magische Konstante
		} else if (modifikatoren_.Existiert(("CharakteristikumAkademischeAusbildungMagier")))
		  ab->modifikatoren_.Loesche(("CharakteristikumAkademischeAusbildungMagier"));
	
		if (person_->charakteristika_.Existiert(("CharakteristikumAkademischeAusbildungKrieger"))) {
		  ab->modifikatoren_.FuegeEin(Modifikator(1, ("CharakteristikumAkademischeAusbildungKrieger")));
		    // TODO Magische Konstante
		} else if (modifikatoren_.Existiert(("CharakteristikumAkademischeAusbildungKrieger")))
		  ab->modifikatoren_.Loesche(("CharakteristikumAkademischeAusbildungKrieger"));
	  }
	  }
	//@-node:<< class AbgeleiteterWert >>
	//@nl

}

//@<< Version History >>
//@+node:<< Version History >>
// $Log: AbgeleiteterWert.cpp,v $
// Revision 1.14  2004/10/17 19:12:39  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.13  2004/06/23 22:24:08  twel
// Magier und Krieger bekommen SO +1
//
// Revision 1.12  2004/04/13 21:45:45  vrandezo
// Akademische Ausbildung (Magier) implementiert
// Aufräumarbeiten (besser in Leo integriert)
//
// Revision 1.11  2004/04/12 21:08:08  vrandezo
// Lehrsprachen implementiert
// Mutter-/Zweitsprachensystem umgebaut
//
// Revision 1.10  2003/11/28 16:46:16  vrandezo
// Zeilenvorlauf am Dateiende (Warnung bei gcc)
//
// Revision 1.9  2003/10/25 02:15:35  vrandezo
// Astralmacht kostet 3 Punkte für Viertelzauberer
//
// Revision 1.8  2003/10/24 23:50:41  vrandezo
// Leonizing
// Vor-/Nachteile als Voraussetzung ermöglicht
// Astralmacht, ZVH-Zauberer
//
// Revision 1.7  2003/10/24 13:27:00  vrandezo
// DEBUG_NEW raus
//
// Revision 1.6  2003/10/20 02:12:30  vrandezo
// Gibt AE für magisch begabte Helden
//
// Revision 1.5  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.4  2003/02/24 17:37:20  vrandezo
// Prüfen implementiert
//
// Revision 1.3  2003/02/23 12:36:27  vrandezo
// Charakteristika-Reiter umbenannt zu Vor-/Nachteile & SF (Tim von Umbscheiden)
// Generierung abschließbar, ohne alle TGP und GP verbraucht zu haben (Stefan Holzmüller)
// das Schließen des Fensters bei der Auswahl von Talenten bei der Vergabe einer
//   Profession, Rasse oder Kultur während der Heldengenerierung führt nicht mehr
//   zum Absturz (Stefan Holzmüller)
// Neuer Vorteil: Fettleibig (Volker Strunk)
//
// Revision 1.2  2003/02/20 09:19:45  vrandezo
// Prüfe-Signatur geändert
//
// Revision 1.1.1.1  2003/02/17 15:32:32  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.16  2003/02/17 14:10:46  vrandezo
// Beschreibung eintragbar für Helden während der Generierung
// Entsprechende Vor- und Nachteile machen Modifikatoren statt im Basiswert mitberechnet zu werden
//
// Revision 1.15  2003/02/14 17:14:17  vrandezo
// einzelne VN: Hohe und Niedrige Lebenskraft und Magieresistenz
// Export: Button, um alle auf einmal zu exportieren
// Geweihte Profession möglich (kann sich durch G&D wieder ändern)
//
// Revision 1.14  2003/02/14 02:00:56  vrandezo
// Log::Error führt nicht mehr zu Extrafenster in Release-Version
// steigerbare VN mit Wert und Maximalwert
// einzelne VN: Gebildet, Ungebildet, Ausdauernd, Kurzatmig
//
// Revision 1.13  2003/01/26 14:27:28  vrandezo
// Astralenergie auf 0 gesetzt
// Helden haben jetzt Raum für Notizen
//
// Revision 1.12  2002/11/05 21:49:16  vrandezo
// Sprachen und Schriften im- und exportierbar
// bool const ueberschreiben aus Speicher-Schnittstelle entfernt
// Mengen haben IDs erhalten
// Voraussetzungen für Talente (allgemeine und meisterliche)
// MengeGUIwx beim Wechseln von Objekten mit Rückfrage
//
// Revision 1.11  2002/11/02 23:36:57  vrandezo
// Exportier und Importierfunktionen implementiert
// Vergleichsoperationen implementiert
// Kampftechnikfenstter implementiert
//
// Revision 1.10  2002/11/01 15:27:28  vrandezo
// Regeln neu implementiert
// ILader, MLader, IMLader, ILadbar und weitere Klassen entfernt
// XMLChar-Transcoding beschleunigt
//
// Revision 1.9  2002/10/19 20:35:32  vrandezo
// feste Stringliterale entfernt
//
// Revision 1.8  2002/10/19 18:58:32  vrandezo
// Texte in eigene, externe Datei untergebracht
//
// Revision 1.7  2002/10/10 21:41:38  vrandezo
// PersonenKomponente als Grundlage für Rasse, Kultur und Profession eingeführt
// Toten Code aus dem Projekt entfernt (noch nicht aus dem CVS)
//
// Revision 1.6  2002/09/28 00:28:37  vrandezo
// Beginn des Generators mit DSA.lib und in wxWindows
//
// Revision 1.5  2002/09/27 19:06:32  vrandezo
// Erweiterung von GUI und DSA.lib
//
// Revision 1.4  2002/08/30 15:06:16  vrandezo
// Apache Xerces ersetzt den MSXML
// Zahlreiche Speicherlecks entfernt
// Entwurf verstärkt
//
// Revision 1.3  2002/08/08 18:51:09  vrandezo
// da Xerces und andere XML-Parser XPath nicht
// unterstützen, wurde die Navigation im DOM-Baum
// von XPath auf eine generische Variante umgestellt.
// Dazu mussten allen Schnittstellen verändert werden.
//
// Revision 1.2  2002/05/30 00:46:52  Zdenko Vrandecic
// MS-spezifischer Code entfernt
//
// Revision 1.1  2002/05/22 19:38:30  Zdenko Vrandecic
// Startversion
//@nonl
//@-node:<< Version History >>
//@nl
//@-node:@file AbgeleiteterWert.cpp
//@-leo
