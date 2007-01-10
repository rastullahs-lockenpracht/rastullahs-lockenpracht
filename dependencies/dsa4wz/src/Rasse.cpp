//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file Rasse.cpp
//@+at
// Rasse.cpp: Implementierung der Klasse CRasse.
// 
// $Revision: 1.11 $
// $Date: 2004/10/17 19:12:40 $
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
#include "Rasse.h"
#include "Kultur.h"
#include "Log.h"
#include "PersistenzManager.h"
#include "Texte.h"
#include "Wuerfel.h"
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

namespace DSA {

	//@  @+others
	//@+node:class Farbverteilung
	//@+others
	//@+node:Farbe
		std::string Farbverteilung::Farbe() const {
			if (farben_.size()==0) return "";
			int anteile = 0;
			std::vector<std::pair<std::string, int> >::const_iterator i;
			for (i = farben_.begin(); i != farben_.end(); ++i)
				anteile += i->second;
			int wurf = W(anteile);
			for (i = farben_.begin(); i != farben_.end(); ++i)
				if (wurf > i->second)
					wurf -= i->second;
				else
					return i->first;
			Log::Error("Farbverteilung::Farbe: logischer Fehler");
			return "";
		} // Farbe
	
		void Farbverteilung::Farbe(const std::string& farbe, int anteil) {
			if (anteil<1) {
				for (std::vector<std::pair<std::string, int> >::iterator i = farben_.begin(); i != farben_.end(); ++i)
					if (i->first == farbe) { farben_.erase(i); return; }
				return;
			}
			for (std::vector<std::pair<std::string, int> >::iterator i = farben_.begin(); i != farben_.end(); ++i)
				if (i->first == farbe) { i->second = anteil; return; }
			farben_.push_back(std::pair<std::string, int>(farbe, anteil));
		} // Farbe
	
		int Farbverteilung::Farbe(const std::string& farbe) const {
			for (std::vector<std::pair<std::string, int> >::const_iterator i = farben_.begin(); i != farben_.end(); ++i)
				if (i->first == farbe) return i->second;
			return 0;
		} // Farbe
	//@nonl
	//@-node:Farbe
	//@+node:SummeAnteile
		int Farbverteilung::SummeAnteile() const {
			int summe = 0;
			for (std::vector<std::pair<std::string, int> >::const_iterator i = farben_.begin(); i != farben_.end(); ++i)
				summe += i->second;
			return summe;
		} // SummeAnteile
	//@nonl
	//@-node:SummeAnteile
	//@+node:Text
		std::string Farbverteilung::Text(const std::string& trennzeichen, const std::string& vorfarbe, const std::string& nachfarbe) const {
			std::string s;
			int anteile = 1;
			if (farben_.size() == 0) return "";
			if (farben_.size() == 1) return (farben_.begin()->first);
			for (std::vector<std::pair<std::string, int> >::const_iterator i = farben_.begin(); i != farben_.end(); ++i) {
				if (i != farben_.begin()) s += trennzeichen;
				s += i->first + vorfarbe;
				if (i->second == 1)
					s += strings::int2sStr(anteile++) + nachfarbe;
				else {
					s += strings::int2sStr(anteile) + "-" + strings::int2sStr(anteile + i->second - 1) + nachfarbe;
					anteile += i->second;
				}
			}
			return s;
		} // Text
	//@nonl
	//@-node:Text
	//@+node:Lese
	
		int Farbverteilung::Lese(PersistenzManager* pm, const std::string& id) {
			std::vector<std::pair<std::string, int> > farben;
			std::string geleseneID = id;
			int rv = pm->OeffneZumLesen(("Farbverteilung"), geleseneID);
			StringListe farbennamen;
			rv += pm->IDListe(("Farbe"), farbennamen);
			for (StringListe::iterator i = farbennamen.begin(); i != farbennamen.end(); ++i) {
				rv += pm->OeffneZumLesen(("Farbe"), *i);
				int anteil;
				rv += pm->Lese(("Anteil"), anteil);
				farben.push_back(std::pair<std::string, int>(*i, anteil));
				rv += pm->SchliesseNachLesen();
			}
			if (rv == 0) {
				ID(geleseneID);
				farben_ = farben;
			}
			rv += pm->SchliesseNachSchreiben();
			return rv;
		} // Lese
	//@nonl
	//@-node:Lese
	//@+node:Schreibe
		int Farbverteilung::Schreibe(PersistenzManager* pm) const {
			if (farben_.size() == 0) return 0;
			if (ID()=="") return 0;
			int rv = pm->OeffneZumSchreiben(("Farbverteilung"), ID());
			for (std::vector<std::pair<std::string, int> >::const_iterator i = farben_.begin(); i != farben_.end(); ++i) {
				rv += pm->OeffneZumSchreiben(("Farbe"), i->first);
				rv += pm->Schreibe(("Anteil"), i->second);
				rv += pm->SchliesseNachLesen();
			}
			rv += pm->SchliesseNachLesen();
			return rv;
		} // Schreibe
	//@nonl
	//@-node:Schreibe
	//@+node:Pruefe
		int Farbverteilung::Pruefe() {
	    int rv = 0;
			for (std::vector<std::pair<std::string, int> >::const_iterator i = farben_.begin(); i != farben_.end(); ++i)
	      if ((i->first == "")
	        ||(i->second < 1)) {
	        Log::Info("Farbverteilung::Pruefe: " + i->first + " " + _t("fehlerhaft"));
	        ++rv;
	      }
			return rv;
		} // Pruefe
	//@nonl
	//@-node:Pruefe
	//@+node:IDListe
		int Farbverteilung::IDListe(PersistenzManager* pm, StringListe* liste) {
			return pm->IDListe(("Farbverteilung"), liste); // TODO Prüfen
		} // IDListe
	//@nonl
	//@-node:IDListe
	//@-others
	//@nonl
	//@-node:class Farbverteilung
	//@+node:class Rasse
	//@+others
	//@+node:Lese
	int Rasse::Lese(PersistenzManager* pm, const std::string& id) {
		std::string geleseneID = id;
		int rv = pm->OeffneZumLesen(("Rasse"), geleseneID);
	
	//	StringListe ueblicheKulturen;
	//	rv += pm->Lese(ueblicheKulturen, _t("Kultur"), _t("Kulturen"), _t("übliche"));
	//	StringListe moeglicheKulturen;
	//	rv += pm->Lese(moeglicheKulturen, _t("Kultur"), _t("Kulturen"), _t("mögliche"));
	
		Farbverteilung augenfarbe, hauptfarbe;
		StringListe farben;
	  if (pm->Existiert(("Farbverteilung"))) {
	  	rv += pm->IDListe(("Farbverteilung"), &farben);
	   if (farben.size() != 2) ++rv;
	  	bool augen = false;
	  	for (StringListe::iterator i = farben.begin(); i != farben.end(); ++i)
			  if (*i == ("Augenfarbe")) {
	  			  if (augen) ++rv;
	     			augen = true;
	     			augenfarbe.Lese(pm, *i);
	    		} else
	     			hauptfarbe.Lese(pm, *i);
		if (!augen) ++rv;
	  }
	  
		int groesseBasis = 0;
		int groesseW6 = 0;
		int groesseW20 = 0;
		rv += pm->OeffneZumLesen(("Größe"));
		rv += pm->Lese(("Wert"), ("Basis"), groesseBasis);
		if (pm->Existiert(("Wert"), ("W6")))
			rv += pm->Lese(("Wert"), ("W6"), groesseW6);
		if (pm->Existiert(("Wert"), ("W20")))
			rv += pm->Lese(("Wert"), ("W20"), groesseW20);
		rv += pm->SchliesseNachLesen();
	
		int gewichtDifferenz = 0;
		rv += pm->Lese(("Wert"), ("Differenz Gewicht zu Größe"), gewichtDifferenz);
	
		int alterBasis = 0;
		int alterW6 = 0;
		int alterW3 = 0;
		rv += pm->OeffneZumLesen(("Alter"));
		rv += pm->Lese(("Wert"), ("Basis"), alterBasis);
		if (pm->Existiert(("Wert"), ("W6")))
			rv += pm->Lese(("Wert"), ("W6"), alterW6);
		if (pm->Existiert(("Wert"), ("W3")))
			rv += pm->Lese(("Wert"), ("W3"), alterW3);
		rv += pm->SchliesseNachLesen();
	
		pm->SchliesseNachLesen();
		rv += PersonenKomponente::Lese(pm, ("Rasse"), geleseneID);
	
		if (!rv) {
	//    ueblicheKulturen_ = ueblicheKulturen;
	//		moeglicheKulturen_ = moeglicheKulturen;
			augenfarbe_ = augenfarbe;
			hauptfarbe_ = hauptfarbe;
			groesseBasis_ = groesseBasis;
			groesseW6_ = groesseW6;
			groesseW20_ = groesseW20;
			alterBasis_ = alterBasis;
			alterW3_ = alterW3;
			alterW6_ = alterW6;
			gewichtDifferenz_ = gewichtDifferenz;
		}
	
		return rv; // TODO
	} // Lese
	//@-node:Lese
	//@+node:Schreibe
	int Rasse::Schreibe(PersistenzManager* pm) const {
		int rv = PersonenKomponente::Schreibe(pm, ("Rasse"));
		rv += pm->OeffneZumSchreiben(("Rasse"), ID());
	
	//  rv += pm->Schreibe(ueblicheKulturen_, _t("Kultur"), _t("Kulturen"), _t("übliche"));
	//  rv += pm->Schreibe(moeglicheKulturen_, _t("Kultur"), _t("Kulturen"), _t("mögliche"));
	
		rv += augenfarbe_.Schreibe(pm);
		rv += hauptfarbe_.Schreibe(pm);
	
		rv += pm->OeffneZumSchreiben(("Größe"));
		rv += pm->Schreibe(("Wert"), ("Basis"), groesseBasis_);
		if (groesseW6_ != 0) rv += pm->Schreibe(("Wert"), ("W6"), groesseW6_);
		if (groesseW20_ != 0) rv += pm->Schreibe(("Wert"), ("W20"), groesseW20_);
		rv += pm->SchliesseNachSchreiben();
	
		rv += pm->Schreibe(("Wert"), ("Differenz Gewicht zu Größe"), gewichtDifferenz_);
	
		rv += pm->OeffneZumSchreiben(("Alter"));
		rv += pm->Schreibe(("Wert"), ("Basis"), alterBasis_);
		if (alterW3_ != 0) rv += pm->Schreibe(("Wert"), ("W3"), alterW3_);
		if (alterW6_ != 0) rv += pm->Schreibe(("Wert"), ("W6"), alterW6_);
		rv += pm->SchliesseNachSchreiben();
	
		rv += pm->SchliesseNachSchreiben();
	
		return 0; //rv; // TODO
	} // Schreibe
	//@-node:Schreibe
	//@-others
	//@nonl
	//@-node:class Rasse
	//@+node:class RasseBasis
	//@+others
	//@+node:Groesse
	int RasseBasis::Groesse() const {
		return groesseBasis_ + W(6, groesseW6_) + W(20, groesseW20_);
	} // Groesse
	
	std::string RasseBasis::GroesseText(bool mitRahmen) const {
		std::string s = strings::int2sStr(groesseBasis_);
		if (groesseW6_ != 0) s += " + " + strings::int2sStr(groesseW6_) + _t("W6");
		if (groesseW20_ != 0) s += " + " + strings::int2sStr(groesseW20_) + _t("W20");
		if (!mitRahmen) return s;
		int min = groesseBasis_ + groesseW6_ + groesseW20_;
		int max = groesseBasis_ + groesseW6_*6 + groesseW20_*20;
		s += " (" + strings::int2sStr(min) + "-" + strings::int2sStr(max) + ")";
		return s;
	} // GroesseText
	//@-node:Groesse
	//@+node:Alter
	int RasseBasis::Alter() const {
		return alterBasis_ + W(3, alterW3_) + W(6, alterW6_);
	} // Alter
	
	std::string RasseBasis::AlterText(bool mitRahmen) const {
		std::string s = strings::int2sStr(alterBasis_);
		if (alterW3_ != 0) s += " + " + strings::int2sStr(alterW3_) + _t("W") + "3";
		if (alterW6_ != 0) s += " + " + strings::int2sStr(alterW6_) + _t("W6");
		if (!mitRahmen) return s;
		int min = alterBasis_ + alterW3_ + alterW6_;
		int max = alterBasis_ + alterW3_*3 + alterW6_*6;
		s += " (" + strings::int2sStr(min) + "-" + strings::int2sStr(max) + ")";
		return s;
	} // AlterText
	//@nonl
	//@-node:Alter
	//@+node:Pruefe
	int RasseBasis::Pruefe() {
	  int rv = augenfarbe_.Pruefe();
	  rv += hauptfarbe_.Pruefe();
	  if (gewichtDifferenz_ >= (groesseBasis_+groesseW6_+groesseW20_)) {
	    Log::Info("RasseBasis::Pruefe: Rasse kann zu leicht werden");
	    ++rv;
	  }
	  return rv;    
	}
	//@-node:Pruefe
	//@-others
	//@nonl
	//@-node:class RasseBasis
	//@+node:class RasseDefinition
	//@+others
	//@+node:Text
	std::string RasseDefinition::Text(const std::string& trennzeichenDerEinzelne, const std::string& zwischenEinzelneTitelUndSachen, const std::string& trennzeichenImEinzelnen, const std::string& wennEinzelnesLeer) const {
	  std::string s = _t("Spezies") + zwischenEinzelneTitelUndSachen + spezies_ + trennzeichenDerEinzelne;
	  s += PersonenKomponenteDefinition::Text(trennzeichenDerEinzelne, zwischenEinzelneTitelUndSachen, trennzeichenImEinzelnen, wennEinzelnesLeer);
	  s += trennzeichenDerEinzelne + _t("Übliche Kulturen") + zwischenEinzelneTitelUndSachen + liste2string(ueblicheKulturen_, trennzeichenImEinzelnen, wennEinzelnesLeer);
	  s += trennzeichenDerEinzelne + _t("Mögliche Kulturen") + zwischenEinzelneTitelUndSachen + liste2string(moeglicheKulturen_, trennzeichenImEinzelnen, wennEinzelnesLeer);
	  s += trennzeichenDerEinzelne + augenfarbe_.ID() + " " + _t("auf W") + strings::int2sStr(augenfarbe_.SummeAnteile()) + zwischenEinzelneTitelUndSachen + augenfarbe_.Text(trennzeichenImEinzelnen);
	  s += trennzeichenDerEinzelne + hauptfarbe_.ID() + " " + _t("auf W") + strings::int2sStr(hauptfarbe_.SummeAnteile()) + zwischenEinzelneTitelUndSachen + hauptfarbe_.Text(trennzeichenImEinzelnen);
	  s += trennzeichenDerEinzelne + _t("Körpergröße") + zwischenEinzelneTitelUndSachen + GroesseText();
	  s += trennzeichenDerEinzelne + _t("Gewicht") + zwischenEinzelneTitelUndSachen + _t("Größe") + " - " + strings::int2sStr(gewichtDifferenz_);
	  s += trennzeichenDerEinzelne + _t("Alter") + zwischenEinzelneTitelUndSachen + AlterText();
	  return s;
	} // Text
	//@nonl
	//@-node:Text
	//@+node:Lese
	int RasseDefinition::Lese(PersistenzManager* pm, const std::string& id) {
		std::string geleseneID = id;
		int rv = pm->OeffneZumLesen(("Rasse"), geleseneID);
	
		StringListe ueblicheKulturen;
		rv += pm->Lese(ueblicheKulturen, ("Kultur"), ("Kulturen"), ("übliche"));
		StringListe moeglicheKulturen;
		rv += pm->Lese(moeglicheKulturen, ("Kultur"), ("Kulturen"), _t("mögliche"));
	
		Farbverteilung augenfarbe, hauptfarbe;
		StringListe farben;
		rv += pm->IDListe(("Farbverteilung"), &farben);
		if (farben.size() != 2) ++rv;
		bool augen = false;
		for (StringListe::iterator i = farben.begin(); i != farben.end(); ++i)
			if (*i == ("Augenfarbe")) {
				if (augen) ++rv;
				augen = true;
				augenfarbe.Lese(pm, *i);
			} else
				hauptfarbe.Lese(pm, *i);
		if (!augen) ++rv;
	
		int groesseBasis = 0;
		int groesseW6 = 0;
		int groesseW20 = 0;
		rv += pm->OeffneZumLesen(("Größe"));
		rv += pm->Lese(("Wert"), ("Basis"), groesseBasis);
		if (pm->Existiert(("Wert"), ("W6")))
			rv += pm->Lese(("Wert"), ("W6"), groesseW6);
		if (pm->Existiert(("Wert"), ("W20")))
			rv += pm->Lese(("Wert"), ("W20"), groesseW20);
		rv += pm->SchliesseNachLesen();
	
		int gewichtDifferenz = 0;
		rv += pm->Lese(("Wert"), ("Differenz Gewicht zu Größe"), gewichtDifferenz);
	
		int alterBasis = 0;
		int alterW6 = 0;
		int alterW3 = 0;
		rv += pm->OeffneZumLesen(("Alter"));
		rv += pm->Lese(("Wert"), ("Basis"), alterBasis);
		if (pm->Existiert(("Wert"), ("W6")))
			rv += pm->Lese(("Wert"), ("W6"), alterW6);
		if (pm->Existiert(("Wert"), ("W3")))
			rv += pm->Lese(("Wert"), ("W3"), alterW3);
		rv += pm->SchliesseNachLesen();
	
		pm->SchliesseNachLesen();
		rv += PersonenKomponenteDefinition::Lese(pm, geleseneID);
	
		if (!rv) {
	    ueblicheKulturen_ = ueblicheKulturen;
			moeglicheKulturen_ = moeglicheKulturen;
			augenfarbe_ = augenfarbe;
			hauptfarbe_ = hauptfarbe;
			groesseBasis_ = groesseBasis;
			groesseW6_ = groesseW6;
			groesseW20_ = groesseW20;
			alterBasis_ = alterBasis;
			alterW3_ = alterW3;
			alterW6_ = alterW6;
			gewichtDifferenz_ = gewichtDifferenz;
		}
		return rv; // TODO
	} // Lese
	//@nonl
	//@-node:Lese
	//@+node:Schreibe
	int RasseDefinition::Schreibe(PersistenzManager* pm) const {
		int rv = PersonenKomponenteDefinition::Schreibe(pm);
		rv += pm->OeffneZumSchreiben(Klasse(), ID());
	
	  rv += pm->Schreibe(ueblicheKulturen_, "Kultur", "Kulturen", ("übliche"));
	  rv += pm->Schreibe(moeglicheKulturen_, "Kultur", "Kulturen", ("mögliche"));
	
		rv += augenfarbe_.Schreibe(pm);
		rv += hauptfarbe_.Schreibe(pm);
	
		rv += pm->OeffneZumSchreiben(("Größe"));
		rv += pm->Schreibe(("Wert"), ("Basis"), groesseBasis_);
		if (groesseW6_ != 0) rv += pm->Schreibe(("Wert"), ("W6"), groesseW6_);
		if (groesseW20_ != 0) rv += pm->Schreibe(("Wert"), ("W20"), groesseW20_);
		rv += pm->SchliesseNachSchreiben();
	
		rv += pm->Schreibe("Wert", "Differenz Gewicht zu Größe", gewichtDifferenz_);
	
		rv += pm->OeffneZumSchreiben(("Alter"));
		rv += pm->Schreibe(("Wert"), ("Basis"), alterBasis_);
		if (alterW3_ != 0) rv += pm->Schreibe(("Wert"), ("W3"), alterW3_);
		if (alterW6_ != 0) rv += pm->Schreibe(("Wert"), ("W6"), alterW6_);
		rv += pm->SchliesseNachSchreiben();
	
		rv += pm->SchliesseNachSchreiben();
		return rv; // TODO
	} // Schreibe
	//@nonl
	//@-node:Schreibe
	//@+node:Pruefe
	int RasseDefinition::Pruefe() {
	  int rv = PersonenKomponenteDefinition::Pruefe();
	  rv += RasseBasis::Pruefe();
	  
	  StringListe sl;
	  forallconst (StringListe, si, moeglicheKulturen_) {
	   std::string s = kulturen()->Finde(*si);
	    sl.push_back((s=="")?*si:s);
	  }
	  moeglicheKulturen_ = sl;
	  
	  
	  sl.clear();
	  forallconst (StringListe, si, ueblicheKulturen_) {
	    std::string s = kulturen()->Finde(*si);
	    sl.push_back((s=="")?*si:s);
	  }
	  ueblicheKulturen_ = sl;
	  
	  if (IstVariante())
	    VarianteVon((rassen()->Finde(VarianteVon())=="")? VarianteVon() : rassen()->Finde(VarianteVon()));
	  
	  return rv;
	}
	
	
	//@-node:Pruefe
	//@-others
	
	//@-node:class RasseDefinition
	//@-others
  
	RasseDefinitionen* rassen() 
		{ static RasseDefinitionen* rassen_ = new RasseDefinitionen(); return rassen_; }

}

//@<< Version History >>
//@+node:<< Version History >>
// $Log: Rasse.cpp,v $
// Revision 1.11  2004/10/17 19:12:40  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.10  2003/12/02 11:55:03  vrandezo
// Standard-C++ verlangt "int" (Tron via Twelwan)
//
// Revision 1.9  2003/11/28 23:22:49  vrandezo
// speicherbare konkrete RKP
//
// Revision 1.7  2003/10/27 18:24:50  vrandezo
// unsichtbare Vor- und Nachteile
// Tänze fremder Repräsentationen teurer
//
// Revision 1.6  2003/10/24 13:30:07  vrandezo
// DEBUG_NEW raus
// Varianten jetzt von Hand eintragbar
//
// Revision 1.5  2003/10/24 00:51:09  vrandezo
// Leonizing
// hoffentlich RKP-Verknüpfungsbug gelöst
//
// Revision 1.4  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.3  2003/02/24 17:37:36  vrandezo
// Prüfen implementiert
//
// Revision 1.2  2003/02/20 09:19:45  vrandezo
// Prüfe-Signatur geändert
//
// Revision 1.1.1.1  2003/02/17 15:34:10  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.14  2003/02/13 20:13:06  vrandezo
// gcc-Portabilität verbessert
//
// Revision 1.13  2003/01/18 18:10:42  vrandezo
// Personen können Sprachen und Schriften
// übliche Kulturen bei Rassen hinzugefügt
// GUI für mögliche und übliche Kulturen und für mögliche Professionen geändert
// Varianten ausblendbar bei der Eingabe möglicher Kulturen und Professionen
//
// Revision 1.12  2003/01/16 16:32:17  vrandezo
// Mutter- und Zweitsprache bei Kultur einstellbar
//
// Revision 1.11  2002/11/05 21:49:18  vrandezo
// Sprachen und Schriften im- und exportierbar
// bool const ueberschreiben aus Speicher-Schnittstelle entfernt
// Mengen haben IDs erhalten
// Voraussetzungen für Talente (allgemeine und meisterliche)
// MengeGUIwx beim Wechseln von Objekten mit Rückfrage
//
// Revision 1.10  2002/11/03 19:48:00  vrandezo
// Größe, Gewicht, Alter eingefügt
//
// Revision 1.9  2002/11/03 17:48:36  vrandezo
// Haarfarbe und Augenfarbe jetzt auch bei der Heldenerschaffung
//
// Revision 1.8  2002/11/03 14:25:33  vrandezo
// Wuerfel angepasst
// Haar- und Augenfarbe hinzugefügt bei Rassen
//
// Revision 1.7  2002/10/27 17:29:21  vrandezo
// Die meisten Teile sind speicherbar
//
// Revision 1.6  2002/10/20 00:20:47  vrandezo
// Mögliche Kulturen und Professionen implementiert
//
// Revision 1.5  2002/10/11 14:13:23  vrandezo
// Modifikatoren für Eigenschaften und Abgeleitete Werte eingeführt
//
// Revision 1.4  2002/10/10 21:41:38  vrandezo
// PersonenKomponente als Grundlage für Rasse, Kultur und Profession eingeführt
// Toten Code aus dem Projekt entfernt (noch nicht aus dem CVS)
//
// Revision 1.3  2002/10/02 01:28:57  vrandezo
// Erweiterung von GUI und DSA.lib
//
// Revision 1.2  2002/08/08 18:51:09  vrandezo
// da Xerces und andere XML-Parser XPath nicht
// unterstützen, wurde die Navigation im DOM-Baum
// von XPath auf eine generische Variante umgestellt.
// Dazu mussten allen Schnittstellen verändert werden.
//
// Revision 1.1  2002/07/17 18:26:03  Zdenko Vrandecic
// Einchecken
//
//@nonl
//@-node:<< Version History >>
//@nl
//@-node:@file Rasse.cpp
//@-leo
