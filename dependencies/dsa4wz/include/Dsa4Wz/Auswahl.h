//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file Auswahl.h
//@+at
// Auswahl.h: Template für die Auswahlen
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

#if !defined(AUSWAHL_H)
#define AUSWAHL_H

//@<< includes >>
//@+node:<< includes >>
#include "Texte.h"
#include "Menge.h"
#include "NDSAUtil.h"

#include <algorithm>
//@nonl
//@-node:<< includes >>
//@nl

namespace DSA {

  class PersistenzManager;
  
	//@  << template class Auswahl >>
	//@+node:<< template class Auswahl >>
	template<class T>
	class Auswahl {
	public:
	  Auswahl() : art_(("einfach")) {}
	  static std::string Klasse() { return T::Klasse(); }
	  static std::string Klassen() { return T::Klassen(); }
	
	  std::string Art() const { return art_; }
	  void Art(const std::string art) {
	    if (art=="einfach") art_=art;
	    if (art=="Liste") art_=art;
	  } // Art
	  
		//@  << IstOK >>
		//@+node:<< IstOK >>
		bool IstOK() const {
		  if ("einfach"==art_) {
		    if (id_.size() != 1) return false;
		    if ((wert_.size() != 1)&&(wert_.size() != 0)) return false;
		    return true;
		  }
		  if ("Liste"==art_) {
		    if ((wert_.size()>1) && (extra_.size()>0)) return false;
		    if (id_.size() < wert_.size()) return false;
		    return true;
		  }
		  return false;
		} // IstOK
		//@nonl
		//@-node:<< IstOK >>
		//@nl
	  
		//@  << ID >>
		//@+node:<< ID >>
		void IDHinzu(const std::string& id) {
		  id_.push_back(id);
		}
		
		std::string ID(int n) const {
		  if ((n<id_.size())&&(n>-1))
		    return id_[n];
		  return "";
		}
		
		int IDAnzahl() const { return id_.size(); }
		
		
		//@-node:<< ID >>
		//@nl
		//@  << Wert >>
		//@+node:<< Wert >>
		void WertHinzu(int wert) {
		  wert_.push_back(wert);
		}
		
		int HoleWert(int n) const {
		  if ((n<wert_.size())&&(n>-1))
		    return wert_[n];
		  return 0;
		}
		
		void EntferneLetztenWert() {
		  if (wert_.size()>0)
		    wert_.pop_back();
		}
		
		int WertAnzahl() const { return wert_.size(); }
		
		//@-node:<< Wert >>
		//@nl
		//@  << Extra >>
		//@+node:<< Extra >>
		void ExtraHinzu(const std::string& extra) {
		  extra_.push_back(extra);
		}
		
		void ExtraWeg(const std::string& extra) {
		  StringListe::iterator x = std::find(extra_.begin(), extra_.end(), extra);
		  if (x != extra_.end())
		    extra_.erase(x);
		}
		
		bool HatExtra(const std::string& extra) const {
		  return (std::find(extra_.begin(), extra_.end(), extra)!=extra_.end());
		}
		
		std::string Extra(int n) const {
		  if ((n<extra_.size())&&(n>-1))
		    return extra_[n];
		  return "";
		}
		
		const StringListe& Extra() const {
		  return extra_;
		}
		
		int ExtraAnzahl() const {
		  return extra_.size();
		}
		
		//@-node:<< Extra >>
		//@nl
	  
	  std::string Kommentar() const { return kommentar_; }
	  void Kommentar(const std::string& kommentar) { kommentar_ = kommentar; }
	  
	  bool Existiert(const std::string& x) const {
	    return (std::find(id_.begin(), id_.end(), x)!=id_.end());
	  } // Existiert
	
		//@  << operator== >>
		//@+node:<< operator== >>
		bool operator==(const Auswahl<T>& other) const {
		  return ((id_==other.id_)
		       && (wert_==other.wert_)
		       && (art_==other.art_)
		       && (extra_==other.extra_)
		       && (kommentar_==other.kommentar_));
		}
		bool operator!=(const Auswahl<T>& other) const { return !operator==(other); }
		//@nonl
		//@-node:<< operator== >>
		//@nl
		//@  << Text >>
		//@+node:<< Text >>
		template <class Ts>
		std::string Text(Ts* tmenge, Dict* ersatz = 0) const {
		  StringListe bezeichner;
		  forallconst (StringListe, id, id_) {
		    typename Ts::typ* t = tmenge->Zeige(*id);
		    std::string s = "";
		    if (t != 0)
		      s = t->Bezeichnung();
		    if (s!="") {
		      bezeichner.push_back(s);
		      continue;
		    }
		    if (ersatz!=0)
		      s = ersatz->operator[](*id);
		    if (s!="")
		      bezeichner.push_back(s);
		    else
		      bezeichner.push_back(_t(*id));
		  }
		  return TextPrivate(bezeichner);
		}
		
		std::string Text() const {
		  return TextPrivate(id_);
		}
		
		
		//@-node:<< Text >>
		//@nl
	  
		//@  << Lese >>
		//@+node:<< Lese >>
		int Lese(PersistenzManager* pm, const int nr) {
		  std::string einzelid = "";
		  int rv = pm->OeffneZumLesen(T::Klasse(), nr, einzelid);
		  bool geoeffnet = (rv==0);
		  std::string art = "einfach";
		  if (pm->Existiert(("Auswahlart"))) rv += pm->Lese(("Auswahlart"), art);
		  StringListe id;
		  if (einzelid!="") id.push_back(einzelid);  // um Charakteristika zu uebernehmen
		  int zahl = pm->Anzahl(("ID"));
		  int i = 0;
		  for (; i < zahl; ++i) {
		    rv += pm->LeseMehrfach(("ID"), einzelid, i);
		    id.push_back(einzelid);
		  }
		  std::vector<int> wert;
		  int einzelwert;
		  zahl = pm->Anzahl(("Wert"));
		  for (i = 0; i < zahl; ++i) {
		    rv += pm->LeseMehrfach(("Wert"), einzelwert, i);
		    wert.push_back(einzelwert);
		  }
		  zahl = pm->Anzahl(("Stufe")); // um Charakteristika zu uebernehmen
		  for (i = 0; i < zahl; ++i) {
		    rv += pm->LeseMehrfach(("Stufe"), einzelwert, i);
		    wert.push_back(einzelwert);
		  }
		  StringListe extra;
		  rv += pm->Lese(extra, ("Extra"), ("Extras"));
		  
		  std::string kommentar;
		  if (pm->Existiert(("Spezifizierung"))) rv += pm->Lese(("Spezifizierung"), kommentar); // um Charakteristika zu uebernehmen
		  if (pm->Existiert(("Kommentar"))) rv += pm->Lese(("Kommentar"), kommentar);
		
		  if (rv==0) {
		    Art(art);
		    id_ = id;
		    wert_ = wert;
		    extra_ = extra;
		    Kommentar(kommentar);
		  }
		  if (geoeffnet) pm->SchliesseNachLesen();
		
		  return rv;
		} // Lese
		//@nonl
		//@-node:<< Lese >>
		//@nl
		//@  << Schreibe >>
		//@+node:<< Schreibe >>
		int Schreibe(PersistenzManager* pm) const {
		  if (!IstOK()) return 1;
		  int rv = pm->OeffneZumSchreiben(T::Klasse(), Anzahl(pm)+1);
		  if (art_!="einfach") rv += pm->Schreibe(("Auswahlart"), art_);
		  int x = IDAnzahl();
		  int i = 0;
		  for (; i < x; ++i)
		    rv += pm->SchreibeMehrfach(("ID"), ID(i));
		  x = WertAnzahl();
		  for (i = 0; i < x; ++i)
		    rv += pm->SchreibeMehrfach(("Wert"), HoleWert(i));
		  rv += pm->Schreibe(extra_, ("Extra"), ("Extras"));
		  if (Kommentar()!="") rv += pm->Schreibe(("Kommentar"), Kommentar());
		  rv += pm->SchliesseNachSchreiben();
		  return rv;
		} // Schreibe
		//@nonl
		//@-node:<< Schreibe >>
		//@nl
		//@  << Pruefe >>
		//@+node:<< Pruefe >>
		int Pruefe() {
		int rv = 0;
		if (!IstOK()) {
		  Log::Info(("Interner Auswahl-Fehler"));
		  ++rv;
		}
		Log::Info(("Auswahlkonsistenz nicht testbar"));
		++rv; // TODO Zeile entfernen
		
		if (rv!=0) Log::Info("in Auswahl mit " + Klasse() + " " + ID(0));
		return rv;
		} // Pruefe
		
		template <class Ts>
		int Pruefe(Ts* tmenge, StringListe* ersatz = 0) {
		  int rv = 0;
		  if (!IstOK()) {
		    Log::Info(("Interner Auswahl-Fehler"));
		    ++rv;
		  }
		  for (int i = 0; i != IDAnzahl(); ++i)
		    if (!tmenge->Existiert(ID(i))) {
		      if (ersatz != 0) 
		        if (std::find(ersatz->begin(), ersatz->end(), ID(i))!=ersatz->end())
		          continue;
		      Log::Info(Klasse() + " unbekannt " + ID(i));
		      std::string s = tmenge->Finde(ID(i));
		      if (s != "") {
		        Log::Info(" wurde geändert zu " + s);
		        id_[i] = s;
		      }        
		      ++rv;
		    }
		
		  if (rv!=0) Log::Info("in Auswahl mit " + ID(0));
		  return rv;
		} // Pruefe
		
		template <class Ts>
		int Pruefe(Ts* tmenge, Ts* alternativ, StringListe* ersatz = 0) {
		  int rv = 0;
		  if (!IstOK()) {
		    Log::Info(("Interner Auswahl-Fehler"));
		    ++rv;
		  }
		  for (int i = 0; i != IDAnzahl(); ++i)
		    if (!tmenge->Existiert(ID(i))) 
		      if (!alternativ->Existiert(ID(i))) {
		        if (ersatz != 0) 
		          if (std::find(ersatz->begin(), ersatz->end(), ID(i))!=ersatz->end())
		            continue;
		        Log::Info(Klasse() + " unbekannt " + ID(i));
		        std::string s = tmenge->Finde(ID(i));
		        if (s=="") s = alternativ->Finde(ID(i));
		        if (s != "") {
		          Log::Info(" wurde geändert zu " + s);
		          id_[i] = s;
		        }        
		        ++rv;
		    }
		
		  if (rv!=0) Log::Info("in Auswahl mit " + ID(0));
		  return rv;
		} // Pruefe
		
		
		//@-node:<< Pruefe >>
		//@nl
	
	  static int Anzahl(PersistenzManager* pm) {
	    return pm->Anzahl(T::Klasse());
	  } // Anzahl
	      
	private:
		//@  << private >>
		//@+node:<< private >>
		std::string art_;
		StringListe id_;
		std::vector<int> wert_;
		std::string kommentar_;
		StringListe extra_;
		
		//@<< TextPrivat >>
		//@+node:<< TextPrivat >>
		std::string TextPrivate(const StringListe& ids) const {
		  if (!IstOK()) return "";
		  
		  std::string x = "";
		  if (ExtraAnzahl()>0)
		    x += " (" + liste2string(extra_) + ")";
		  
		  if (("einfach"==art_) || (ids.size()==1))
		    if (wert_.size()>0)
		      return ids[0] + " " + strings::int2sStr(HoleWert(0), true) + x;
		    else
		      return ids[0] + x;
		    
		  if (("Liste"==art_) && (wert_.size()==0)) {
		    std::string s = liste2string(ids, " " + _t("oder") + " ");
		    return s + x;
		  }
		
		  if (("Liste"==art_) && (wert_.size()==1)) {
		    std::string s = liste2string(ids, " " + _t("oder") + " ");
		    s += " " + strings::int2sStr(*(wert_.begin()), true);
		    return s + x;
		  }
		  
		  if (("Liste"==art_) && (wert_.size()==2)) {
		    std::string s = liste2string(ids, " " + _t("oder") + " ");
		    s += " " + strings::int2sStr(*(wert_.begin()), true);
		    if (ids.size()==2)
		      s += ", " + _t("das andere") + " " + strings::int2sStr(HoleWert(1), true);
		    else
		      s += ", " + _t("ein anderes") + " " + strings::int2sStr(HoleWert(1), true);
		    return s;
		  }
		  
		  if (("Liste"==art_) && (wert_.size()==3)) {
		    std::string s = liste2string(ids, " " + _t("oder") + " ");
		    s += " " + strings::int2sStr(*(wert_.begin()), true);
		    s += ", " + _t("ein weiteres") + " " + strings::int2sStr(HoleWert(1), true);
		    if (ids.size()==3)
		      s += ", " + _t("und das dritte") + " " + strings::int2sStr(HoleWert(2), true);
		    else
		      s += ", " + _t("ein drittes") + " " + strings::int2sStr(HoleWert(2), true);
		    return s;
		  }
		  
		  if ("Liste"==art_) {
		    std::string s;
		    for (int i = 0; i < wert_.size(); ++i) {
		      if (i!=0) s += "/";
		      s += strings::int2sStr(HoleWert(i), true);
		    }
		    s += " " + _t("verteilen auf") + ": ";
		    s += liste2string(ids, ", ");
		    return s;
		  }
		
		  return "Fehler";
		} // TextPrivate
		//@nonl
		//@-node:<< TextPrivat >>
		//@nl
		//@nonl
		//@-node:<< private >>
		//@nl
	}; // Auswahl
	//@-node:<< template class Auswahl >>
	//@nl
  
  template<class T>
  class AuswahlMenge : public MengeOhneID<Auswahl<T> > {
  public:
    typedef MengeOhneID<Auswahl<T> > Base;
    using Base::begin;
    using Base::end;

    bool Existiert(const std::string& x) const {
      for (typename MengeOhneID<Auswahl<T> >::const_iterator i = begin(); i != end(); ++i)
        if (i->Existiert(x)) return true;
      return false;
    } // Existiert
  }; // AuswahlMenge

}

#endif // !defined(AUSWAHL_H)

//@<< Version History >>
//@+node:<< Version History >>
// $Log: Auswahl.h,v $
// Revision 1.14  2004/10/17 19:12:39  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.13  2004/05/13 07:52:43  dertron
// Korrekturen für gcc 3.4, der einiges ISO-C++-konformer ist.
//
// Revision 1.12  2004/04/13 14:06:52  vrandezo
// Hauszauber implementiert
//
// Revision 1.11  2004/01/05 05:32:54  vrandezo
// Template Parameter nach C++-Standard (auf Twels Hinweis)
//
// Revision 1.10  2004/01/05 03:35:19  vrandezo
// SF bei RKP-Defs sind jetzt auch Auswahlmengen statt einfacher Mengen
//
// Revision 1.9  2004/01/05 02:45:29  vrandezo
// Charakteristika bei RPK-Definition von Mengen auf Auswahlmengen umgestellt
//
// Revision 1.8  2003/11/28 16:46:16  vrandezo
// Zeilenvorlauf am Dateiende (Warnung bei gcc)
//
// Revision 1.7  2003/10/25 15:06:03  vrandezo
// Leonizing
//
// Revision 1.6  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.5  2003/03/09 15:29:12  vrandezo
// Bug: falsche Darstellung dreier Talentboni bei der Auswahl (King Lui)
//
// Revision 1.4  2003/03/07 15:06:40  vrandezo
// AT/PA-Werte bei Kampftechniken (Heldenbrief Gerhard Mayer)
//
// Revision 1.3  2003/02/24 17:37:22  vrandezo
// Prüfen implementiert
//
// Revision 1.2  2003/02/20 09:19:45  vrandezo
// Prüfe-Signatur geändert
//
// Revision 1.1.1.1  2003/02/17 15:32:36  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.4  2003/02/13 20:13:01  vrandezo
// gcc-Portabilität verbessert
//
// Revision 1.3  2003/01/17 17:45:29  vrandezo
// Notebook-Bug  korrigiert (kam nur in der Release-Version vor)
// Varianten automatisch zugelassen, wenn Basis möglich
// Auswahl von Rasse, Profession, Kultur vereinfacht
// Talentauswahlauflösung implementiert (einfach und Liste)
//
// Revision 1.2  2003/01/16 13:42:14  vrandezo
// Kommentare für Auswahlen möglich
// Ein Talent aus einer Talentgruppe auswählbar
//
// Revision 1.1  2003/01/13 00:30:48  vrandezo
// Release Version 1.12 dev
// Korrektur der Verrechnungspunkteberechnung für negative Talentmodifikatoren
// Talentauswahl implementiert
//
//@nonl
//@-node:<< Version History >>
//@nl
//@-node:@file Auswahl.h
//@-leo
