//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file Menge.h
//@+at
// Menge.h: Template zur Verwaltung von einer Menge von 
// Objekten
// 
// $Revision: 1.17 $
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

#if !defined(MENGE_H)
#define MENGE_H

//@<< includes >>
//@+node:<< includes >>
#include <map>
#include <ctype.h>
#include "PersistenzManager.h"
#include "NDSAUtil.h"
#include "Texte.h"
//@nonl
//@-node:<< includes >>
//@nl

namespace DSA {

  class Person;
  
	//@  << template class Menge >>
	//@+node:<< template class Menge >>
	template<class T>
	class Menge : public std::map<std::string, T> {
	public:
	  typedef std::map<std::string, T> Base;
	  using Base::begin;
	  using Base::clear;
	  using Base::empty;
	  using Base::end;
	  using Base::find;
	  using Base::size;
	
	  Menge(const std::string& id = "") : id_(id) {}
	
	  typedef T typ;
	  static std::string Klasse() { return T::Klasse(); }
	  static std::string Klassen() { return T::Klassen(); }
	
	  std::string ID() const { return id_; }
	  void ID(const std::string& id) { id_ = id; }
	    
			//@    @+others
			//@+node:Lese
			int Lese(PersistenzManager* pm, const std::string& id = "") {
				int rv = 0;
				std::string geleseneID = id;
				clear();
				bool zumLesenGeoeffnet = (pm->OeffneZumLesen(T::Klassen(), geleseneID) == 0);
			
			  StringListe* s = new StringListe();
			  rv += pm->IDListe(T::Klasse(), s);
				T t;
				for (StringListe::const_iterator i = s->begin(); i != s->end(); ++i) {
					int rv0 = t.Lese(pm, *i);
			  	if (rv0 == 0)	rv += FuegeEin(t) ? 0 : 1;
				  rv += rv0;
			  }
			  delete s;
			
			  if (zumLesenGeoeffnet) rv += pm->SchliesseNachLesen();
				ID(geleseneID);
				return rv; // TODO
				// TODO Auf diese Art und Weise dauert das Lesen von Mengen O(n�), es kann aber
				// auf O(n) runtergedr�ckt werden, indem nicht zuerst die Stringliste gelesen
				// wird und dann St�ck f�r St�ck jedes Objekt einzeln, sondern indem die
				// Objekte linear reingelesen werden. Dies bedarf engerer Zusammenarbeit mit
				// dem Persistenzmanager, kann aber bedeutende Geschwindigkeitserh�hung beim
				// Datenlesen bringen
			} // Lese
			//@nonl
			//@-node:Lese
			//@+node:Schreibe
			int Schreibe(PersistenzManager* pm) const { // TODO Mengen mit ID
				int rv = 0;
				if (empty()) {
			  	if (pm->Existiert(T::Klassen(), ID()))
				  	rv += pm->Loesche(T::Klassen(), ID());
					return rv;
				}
			
			  rv += pm->OeffneZumSchreiben(T::Klassen(), ID());
			  StringListe s;
			  rv += pm->IDListe(T::Klasse(), s);
			  for (typename StringListe::const_iterator i = s.begin(); i != s.end(); ++i)
				  if (!Existiert(*i))
					  rv += pm->Loesche(T::Klasse(), *i);
			  for (typename DSA::Menge<T>::const_iterator t = begin(); t != end(); ++t) {
				  rv += t->second.Schreibe(pm);
			  }
			
			  rv += pm->SchliesseNachSchreiben();
			
			  return rv; // TODO
			} // Schreibe
			//@nonl
			//@-node:Schreibe
			//@+node:Pruefe
			int Pruefe() {
			  int rv = 0;
			  for (typename DSA::Menge<T>::iterator i = begin(); i != end(); ++i)
			    rv += i->second.Pruefe();
			  return rv;
			} // Pruefe
			//@nonl
			//@-node:Pruefe
			//@+node:IDListe
			static int IDListe(PersistenzManager* pm, StringListe* liste) {
				//return pm->IDListe(Texte::?, liste); // TODO Pr�fen
				Log::Error("Menge<" + T::Klasse() + ">::IDListe " + _t("nicht implementiert"));
			  return 1; // TODO
			} // IDListe
			//@nonl
			//@-node:IDListe
			//@+node:FuegeEin
			// gibt false zur�ck, sofern die t schon existierte
			// TODO: virtual wegen Voraussetzungen. Sollte dann entfernt werden, wenn die
			// Voraussetzungen wieder richtig tun
			virtual bool FuegeEin(T t) {
				return insert(std::pair<std::string, T>(t.ID(), t)).second;
			} // FuegeEin
			//@nonl
			//@-node:FuegeEin
			//@+node:Existiert
			bool Existiert(const std::string& id) const {	return find(id)!=end();	}
			//@nonl
			//@-node:Existiert
			//@+node:Loesche
			bool Loesche(const std::string& id) {
			  typename DSA::Menge<T>::iterator i = find(id);
			  if (i==end()) return false;
			  erase(i);
			  return true;
			} // Loesche
			//@nonl
			//@-node:Loesche
			//@+node:Hole
			// gibt ein leeres T zur�ck, wenn die jeweilige nicht vorhanden ist
			T Hole(const std::string& id) const {
				T t;
				if (Existiert(id)) t = find(id)->second;
				return t;
			} // Hole(id)
			
			// gibt ein leeres T zur�ck, wenn die jeweilige nicht vorhanden ist
			// ansonsten alphabetisch sortiert
			T Hole(int pos) const {
				T t;
				if (pos<0) return t;
				if (size()<=pos) return t;
				typename DSA::Menge<T>::const_iterator i = begin();
				for (; pos-->0; ++i);
				return i->second;
			} // Hole(pos)
			//@-node:Hole
			//@+node:Zeige
			// zeigt auf das entsprechende T oder aber auf 0, sofern T nicht vorhanden
			T* Zeige(const std::string& id) {
				T* pt = 0;
				if (Existiert(id)) pt = &(find(id)->second);
				return pt;
			} // Zeige
			
			const T* const Zeige(const std::string& id) const {
				const T* pt = 0;
				if (Existiert(id)) pt = &(find(id)->second);
				return pt;
			} // Zeige
			
			// zeigt auf das entsprechende T oder aber auf 0, sofern T nicht vorhanden
			T* Zeige(int pos) {
				T* pt = 0;
				if (pos<0) return pt;
				if (size()<=pos) return pt;
				typename DSA::Menge<T>::iterator i = begin();
				for (; pos-->0; ++i);
				return &(i->second);
			} // Zeige
			
			// zeigt auf das entsprechende T oder aber auf 0, sofern T nicht vorhanden
			const T* Zeige(int pos) const {
				T* pt = 0;
				if (pos<0) return pt;
				if (size()<=pos) return pt;
				typename DSA::Menge<T>::const_iterator i = begin();
				for (; pos-->0; ++i);
				return &(i->second);
			} // Zeige
			//@nonl
			//@-node:Zeige
			//@+node:Finde
			std::string Finde(const std::string& bezeichnung) const {
				typename DSA::Menge<T>::const_iterator i = begin();
				while (i!=end()) {
			    if (i->second.Bezeichnung()==bezeichnung)
			      return i->first;
			//    if (i->second.Bezeichnung(DSA::StandardLang())==bezeichnung)
			//      return i->first;
			    ++i;
			  }
			  return "";
			}
			//@-node:Finde
			//@+node:Text
			std::string Text(const std::string& trennzeichen = ", ", const std::string& wennleer = "-") const {
			  if (size()==0) return wennleer;
			  std::string s = "";
			  for (typename DSA::Menge<T>::const_iterator i = begin(); i != end(); ++i) {	
					if (i != begin()) s += trennzeichen;
					s += i->second.Text();
				}
			  return s;
			} // Text
			//@nonl
			//@-node:Text
			//@+node:Ersetze Tag
			std::string ErsetzeTag(const std::string& tag) const {
			  if ("l"==tag) return Text();
			  if ("Anzahl"==tag) return strings::int2sStr(size());
			  std::string nummer = "";
			  int i;
			  for (i = 0; i != tag.size(); ++i) {
			    if (isdigit(tag[i])) nummer += tag[i]; else break;
			  }
			  int pos = strings::sStr2int(nummer)-1;
			  std::string subtag = tag.substr(i, tag.size()-i);
			  std::string rv = ""; // ein Zeichen, was in keinem Tag sein kann
			  if (size()>pos)
			    rv = Zeige(pos)->ErsetzeTag(subtag);
			  else
			    return "";
			  if (rv==subtag) return tag; else return rv;
			} // ErsetzeTag
			//@nonl
			//@-node:Ersetze Tag
			//@+node:Execute
			std::string Execute(const std::string& cmd) {
			  if (cmd[0]=='(')
			    return "";
			//    etc.
			  return _t("Unbekannter Befehl");
			}
			//@nonl
			//@-node:Execute
			//@+node:Return
			std::string Return(const std::string& request) const {
			  // beginnt mit Klammern, will also etwas von einem Element
			  if (request[0]=='(') {
			    int i;
			    std::string nummer = "";
			    for (i = 1; i != request.size(); ++i)
			      if (isdigit(request[i]))
			        nummer += request[i];
			      else
			        break;
			    int pos = strings::sStr2int(nummer)-1;
			    if (size()>pos)
			      return Zeige(pos)->Return(request.substr(i+2, request.size()-(i+2)));
			    else
			      return _t("Au�erhalb des Index");    
			  }
			  
			  // direkte Anfrage an die Menge
			  if (request=="anzahl()")
			    return strings::int2sStr(size());
			
			  return _t("Unbekannte Abfrage");
			}
			//@-node:Return
			//@-others
	    
	protected:
	  std::string id_;
	
	}; // Template Class Menge
	//@-node:<< template class Menge >>
	//@nl
  
	//@  << template class SortierteMenge >>
	//@+node:<< template class SortierteMenge >>
	template<class T>
	class SortierteMenge : public Menge<T> {
	
	public:		
		typedef Menge<T> Base;
		using Base::Existiert;
		using Base::begin;
		using Base::clear;
		using Base::empty;
		using Base::end;
	
	  typedef typename T::Kriterium Kriterium;
	
	  static DDict kriteriumsbezeichnung_;;
	  
		//@  @+others
		//@+node:Schreibe
		int Schreibe(PersistenzManager *pm) const { // TODO Mengen mit ID
			int rv = 0;
		  if (empty()) {
				if (pm->Existiert(T::Klassen()))
					rv += pm->Loesche(T::Klassen());
		  	return rv;
			}
		  rv += pm->OeffneZumSchreiben(T::Klassen());
		
			std::map<Kriterium, StringListe> gruppe;
			for (typename DSA::Menge<T>::const_iterator t = begin(); t != end(); ++t)
		  	gruppe[t->second.ZuKriterium()].push_back(t->second.ID());
		
			for (typename std::map<Kriterium, StringListe>::iterator g = gruppe.begin(); g != gruppe.end(); ++g) {
			  if (g->second.empty()) {
					if (pm->Existiert(T::Kriteriumsart(), T::Kriteriumstring(g->first)))
						rv += pm->Loesche(T::Kriteriumsart(), T::Kriteriumstring(g->first));
		  		continue;
				}
			  rv += pm->OeffneZumSchreiben(T::Kriteriumsart(), T::Kriteriumstring(g->first));
		    if (kriteriumsbezeichnung_[T::Kriteriumstring(g->first)].size() > 0)
		      for (Dict::const_iterator i = kriteriumsbezeichnung_[T::Kriteriumstring(g->first)].begin(); i != kriteriumsbezeichnung_[T::Kriteriumstring(g->first)].end(); ++i)
		        rv += pm->Schreibe("Bezeichnung", i->first, i->second);  
		
				StringListe s;
				rv += pm->IDListe(T::Klasse(), s);
		  	for (StringListe::const_iterator i = s.begin(); i != s.end(); ++i)
			  	if ((!Existiert(*i)) || (Zeige(*i)->IstKriterium(g->first)))
				  	rv += pm->Loesche(T::Klasse(), *i);
			  for (StringListe::const_iterator t = g->second.begin(); t != g->second.end(); ++t)
				  rv += Zeige(*t)->Schreibe(pm);
			  rv += pm->SchliesseNachSchreiben();
			}
		  rv += pm->SchliesseNachSchreiben();
		  return rv; // TODO
		} // Schreibe
		//@nonl
		//@-node:Schreibe
		//@+node:Lese
		int Lese(PersistenzManager* pm, const std::string& id = "") {
			int rv = 0;
			clear();
			bool zumLesenGeoeffnet = (pm->OeffneZumLesen(T::Klassen()) == 0);
			StringListe gruppen;
			rv += pm->IDListe(T::Kriteriumsart(), gruppen);
			if (gruppen.empty()) return 0;
			for (StringListe::iterator g = gruppen.begin(); g != gruppen.end(); ++g) {
				rv += pm->OeffneZumLesen(T::Kriteriumsart(), *g);
		  
		    std::string geleseneGruppenID = *g;      
		    if (!strings::BeginntMit(geleseneGruppenID, T::Kriteriumsart()))
		      geleseneGruppenID = strings::MacheID(T::Kriteriumsart(), *g);
		    if (pm->Existiert("Bezeichnung")) {
		      StringListe langs;
		      rv += pm->IDListe("Bezeichnung", langs);
		      for (StringListe::iterator si = langs.begin(); si != langs.end(); ++si) {
		        std::string sBuf;
		        rv += pm->Lese("Bezeichnung", *si, sBuf);
		        kriteriumsbezeichnung_[*g][*si] = sBuf;
		      }      
		    } else
		      kriteriumsbezeichnung_[*g][DSA::StandardLang()] = *g; 
		            
				StringListe s;
				rv += pm->IDListe(T::Klasse(), s);
				T t;
				for (StringListe::const_iterator i = s.begin(); i != s.end(); ++i) {
					int rv0 = t.Lese(pm, *i);
					t.SetzeKriterium(T::Kriteriumstring(geleseneGruppenID));
					if (rv0 == 0)	rv += FuegeEin(t) ? 0 : 1;
					rv += rv0;
				}
				rv += pm->SchliesseNachLesen();
			}
			if (zumLesenGeoeffnet) rv += pm->SchliesseNachLesen();
			return rv; // TODO
			// TODO Auf diese Art und Weise dauert das Lesen von Mengen O(n�), es kann aber
			// auf O(n) runtergedr�ckt werden, indem nicht zuerst die Stringliste gelesen
			// wird und dann St�ck f�r St�ck jedes Objekt einzeln, sondern indem die
			// Objekte linear reingelesen werden. Dies bedarf engerer Zusammenarbeit mit
			// dem Persistenzmanager, kann aber bedeutende Geschwindigkeitserh�hung beim
			// Datenlesen bringen
		} // Lese
		//@-node:Lese
		//@+node:Kriteriumsbezeichnung
		static std::string Kriterium2Bezeichnung(const std::string& s, const std::string& sprache = DSA::Lang()) {
		  DDict::const_iterator i = kriteriumsbezeichnung_.find(s);
		  if (i == kriteriumsbezeichnung_.end())
		    return s;
		  Dict::const_iterator j = i->second.find(sprache);
		  if (j != i->second.end())
		    return j->second;
		  j = i->second.find(DSA::StandardLang());
		  if (j != i->second.end())
		    return j->second;
		  if (i->second.size() > 0)
		    return i->second.begin()->second;
		  if (strings::BeginntMit(s, T::Kriteriumsart()))
		    return strings::OhneAnfangExakt(s, T::Kriteriumsart());
		  return s;
		}
		
		static std::string Bezeichnung2Kriterium(const std::string& s, const std::string& sprache = DSA::Lang()) {
		  for (DDict::const_iterator i = kriteriumsbezeichnung_.begin(); i != kriteriumsbezeichnung_.end(); ++i) {
		    Dict::const_iterator j = i->second.find(sprache);
		    if (j != i->second.end())
		      if (j->second == s)
		        return i->first;
		    j = i->second.find(DSA::StandardLang());
		    if (j != i->second.end())
		      if (j->second == s)
		        return i->first;
		    if (i->second.size() > 0)
		      if (i->second.begin()->second == s)
		        return i->first;
		  }
		  if (!strings::BeginntMit(s, T::Kriteriumsart()))
		    return T::Kriteriumsart()+s;
		  return s;
		}
		
		
		
		//@-node:Kriteriumsbezeichnung
		//@-others
	  
		using Menge<T>::Zeige;
	
		T* ZeigeSortiert(int pos, typename T::Kriterium k) {
			T* pt = 0;
			if (pos<0) return pt;
			for (typename DSA::Menge<T>::iterator i = begin(); pos>=0; ++i) 
				if (i == end()) 
					return pt;
				else
					if (i->second.IstKriterium(k))
						if (--pos<0) return &(i->second);
			return pt;
		} // Zeige
	
	}; // SortierteMenge
	
	  template<class T> DDict SortierteMenge<T>::kriteriumsbezeichnung_ = DDict();
	//@-node:<< template class SortierteMenge >>
	//@nl
  
	//@  << template class MengeOhneID >>
	//@+node:<< template class MengeOhneID >>
	template<class T>
	class MengeOhneID : public std::vector<T> {
	public:
		typedef std::vector<T> Base;
		using Base::begin;
		using Base::clear;
		using Base::empty;
		using Base::end;
		using Base::size;
	
		MengeOhneID(const std::string& id = "") : id_(id) {}
	
		typedef T typ;
		static std::string Klasse() { return T::Klasse(); }
		static std::string Klassen() { return T::Klassen(); }
	
		std::string ID() const { return id_; }
		void ID(const std::string& id) { id_ = id; }
	    
		//@  << Lese >>
		//@+node:<< Lese >>
		int Lese(PersistenzManager* pm, const std::string& id = "") {
			int rv = 0;
			std::string geleseneID = id;
			clear();
			bool zumLesenGeoeffnet = (pm->OeffneZumLesen(T::Klassen(), geleseneID) == 0);
		
		  int Anzahl = pm->Anzahl(T::Klasse());
		  T t;
		  for (int i = 0; i < Anzahl; ++i) {
		    int rv0 = t.Lese(pm, i);
		    if (rv0 == 0) rv += FuegeEin(t) ? 0 : 1;
		    rv += rv0;
		  }
		
			if (zumLesenGeoeffnet) rv += pm->SchliesseNachLesen();
			ID(geleseneID);
			return rv; // TODO
			// TODO Auf diese Art und Weise dauert das Lesen von Mengen O(n�), es kann aber
			// auf O(n) runtergedr�ckt werden, indem nicht zuerst die Stringliste gelesen
			// wird und dann St�ck f�r St�ck jedes Objekt einzeln, sondern indem die
			// Objekte linear reingelesen werden. Dies bedarf engerer Zusammenarbeit mit
			// dem Persistenzmanager, kann aber bedeutende Geschwindigkeitserh�hung beim
			// Datenlesen bringen
		} // Lese
		//@nonl
		//@-node:<< Lese >>
		//@nl
		//@  << Schreibe >>
		//@+node:<< Schreibe >>
		int Schreibe(PersistenzManager* pm) const { // TODO Mengen mit ID
			int rv = 0;
			if (empty()) {
		  	if (pm->Existiert(T::Klassen(), ID()))
			  	rv += pm->Loesche(T::Klassen(), ID());
				return rv;
		  }
		
		  if (pm->Existiert(T::Klassen(), ID()))
		    rv += pm->Loesche(T::Klassen(), ID());
		  rv += pm->OeffneZumSchreiben(T::Klassen(), ID());
		  for (typename DSA::MengeOhneID<T>::const_iterator t = begin(); t != end(); ++t)
		    rv += t->Schreibe(pm);
		  rv += pm->SchliesseNachSchreiben();
		
		  return rv; // TODO
		} // Schreibe
		//@nonl
		//@-node:<< Schreibe >>
		//@nl
		//@  << Pruefe >>
		//@+node:<< Pruefe >>
		int Pruefe() {
		  int rv = 0;
		  for (typename DSA::MengeOhneID<T>::iterator i = begin(); i != end(); ++i)
		    rv += i->Pruefe();
		  return rv;
		} // Pruefe		
		
		template<class Ts>
		int Pruefe(Ts* tmenge, StringListe* sl = 0) {
		  int rv = 0;
		  for (typename DSA::MengeOhneID<T>::iterator i = begin(); i != end(); ++i)
		    rv += i->Pruefe(tmenge, sl);
		  return rv;
		} // Pruefe
		
		template<class Ts>
		int Pruefe(Ts* tmenge, Ts* altmenge, StringListe* sl = 0) {
		  int rv = 0;
		  for (typename DSA::MengeOhneID<T>::iterator i = begin(); i != end(); ++i)
		    rv += i->Pruefe(tmenge, altmenge, sl);
		  return rv;
		} // Pruefe
		
		//@-node:<< Pruefe >>
		//@nl
	  
		//@  << Fuege Ein >>
		//@+node:<< Fuege Ein >>
		bool FuegeEin(T t) {
			push_back(t);
		  return true;
		} // FuegeEin
		//@nonl
		//@-node:<< Fuege Ein >>
		//@nl
		//@  << Loesche >>
		//@+node:<< Loesche >>
		void Loesche(T t) {
		  for (typename DSA::MengeOhneID<T>::iterator i = begin(); i != end(); ++i)
		    if (t==*i) { erase(i); return; }
		} // Loesche
		//@nonl
		//@-node:<< Loesche >>
		//@nl
	  
		//@  << Zeige >>
		//@+node:<< Zeige >>
		// zeigt auf das entsprechende T oder aber auf 0, sofern T nicht vorhanden
		T* Zeige(int pos) {
			T* pt = 0;
			if (pos<0) return pt;
			if (size()<=pos) return pt;
			typename DSA::MengeOhneID<T>::iterator i = begin();
			for (; pos-->0; ++i);
			return &(i->second);
		} // Zeige
		//@nonl
		//@-node:<< Zeige >>
		//@nl
		//@  << Hole >>
		//@+node:<< Hole >>
		// gibt ein leeres T zur�ck, wenn die jeweilige nicht vorhanden ist
		// ansonsten alphabetisch sortiert
		T Hole(int pos) const {
			T t;
			if (pos<0) return t;
			if (size()<=pos) return t;
			typename DSA::MengeOhneID<T>::iterator i = begin();
			for (; pos-->0; ++i);
			return i->second;
		} // Hole(pos)
		//@nonl
		//@-node:<< Hole >>
		//@nl
	  
		//@  << Text >>
		//@+node:<< Text >>
		std::string Text(const std::string& trennzeichen = ", ", const std::string& wennleer = "-") const {
		  if (size()==0) return wennleer;
		  std::string s = "";
		  for (typename DSA::MengeOhneID<T>::const_iterator i = begin(); i != end(); ++i) {	
				if (i != begin()) s += trennzeichen;
				s += i->Text();
			}
		  return s;
		} // Text
		//@nonl
		//@-node:<< Text >>
		//@nl
	  
	protected:
		std::string id_;
	
	}; // Template Class MengeOhneID
	//@-node:<< template class MengeOhneID >>
	//@nl
  
}

#endif // !defined(MENGE_H)

//@<< Version History >>
//@+node:<< Version History >>
// $Log: Menge.h,v $
// Revision 1.17  2004/10/17 19:12:39  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.16  2004/05/13 07:52:44  dertron
// Korrekturen f�r gcc 3.4, der einiges ISO-C++-konformer ist.
//
// Revision 1.15  2004/04/23 13:19:07  vrandezo
// Erweiterung von return / execute
//
// Revision 1.13  2004/04/13 21:45:45  vrandezo
// Akademische Ausbildung (Magier) implementiert
// Aufr�umarbeiten (besser in Leo integriert)
//
// Revision 1.12  2003/11/28 16:46:16  vrandezo
// Zeilenvorlauf am Dateiende (Warnung bei gcc)
//
// Revision 1.11  2003/11/20 00:35:07  vrandezo
// compilierbarkeit mit gcc verbessert
//
// Revision 1.10  2003/10/25 15:06:03  vrandezo
// Leonizing
//
// Revision 1.9  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.8  2003/10/17 23:22:02  vrandezo
// sehr langsames Kulturenlesen
//
// Revision 1.7  2003/03/24 18:57:16  vrandezo
// Neue Vorteile: Herausragende Eigenschaften (Forumswunsch Sebastian)
//
// Revision 1.6  2003/03/07 15:06:41  vrandezo
// AT/PA-Werte bei Kampftechniken (Heldenbrief Gerhard Mayer)
//
// Revision 1.5  2003/03/04 18:30:31  vrandezo
// schon wieder m�glichen Bug beim Lesen von Personen (SF) korrigiert
// Korrekturen Standardtags
//
// Revision 1.4  2003/03/04 15:23:17  vrandezo
// weitere Tags implementiert f�r die Druckvorlagen
//
// Revision 1.3  2003/02/24 17:37:28  vrandezo
// Pr�fen implementiert
//
// Revision 1.2  2003/02/20 09:19:45  vrandezo
// Pr�fe-Signatur ge�ndert
//
// Revision 1.1.1.1  2003/02/17 15:32:59  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.17  2003/02/13 20:54:00  vrandezo
// typedef/typename-Probleme mit MSVC gel�st
//
// Revision 1.16  2003/02/13 20:13:02  vrandezo
// gcc-Portabilit�t verbessert
//
// Revision 1.15  2003/02/08 00:22:54  vrandezo
// VN von RKP bei Generierung �bernommen
// mehrfache VN bei RKP geben GP
// erweitere Voraussetzungen: nicht VN
// entfernbare VN
// ausw�hlbare VN
// VN mit Voraussetzungen (nicht VN)
// Geschlecht neuerschaffener Helden ist zuf�llig
//
// Revision 1.14  2003/01/13 00:30:49  vrandezo
// Release Version 1.12 dev
// Korrektur der Verrechnungspunkteberechnung f�r negative Talentmodifikatoren
// Talentauswahl implementiert
//
// Revision 1.13  2002/12/06 15:28:00  vrandezo
// Spezies bei den Rassen einstellbar
// Kampftechniken bei PK und bei Person hinzugef�gt
//
// Revision 1.12  2002/11/05 21:49:18  vrandezo
// Sprachen und Schriften im- und exportierbar
// bool const ueberschreiben aus Speicher-Schnittstelle entfernt
// Mengen haben IDs erhalten
// Voraussetzungen f�r Talente (allgemeine und meisterliche)
// MengeGUIwx beim Wechseln von Objekten mit R�ckfrage
//
// Revision 1.11  2002/11/05 16:24:37  vrandezo
// MengeGUIwx vereinfacht (nur ein Template-Parameter)
// SpracheGUIwx hinzugef�gt
// SortierteMenge erweitert um Zeige
//
// Revision 1.10  2002/11/05 13:12:43  vrandezo
// Spezies bei den Rassen einegf�hrt
// SortierteMenge verallgemeinert
//
// Revision 1.9  2002/11/02 23:36:57  vrandezo
// Exportier und Importierfunktionen implementiert
// Vergleichsoperationen implementiert
// Kampftechnikfenstter implementiert
//
// Revision 1.8  2002/11/02 15:39:48  vrandezo
// Importfenster hinzugef�gt
//
// Revision 1.7  2002/10/27 17:29:20  vrandezo
// Die meisten Teile sind speicherbar
//
// Revision 1.6  2002/10/25 23:06:33  vrandezo
// Speichern erweitert
//
// Revision 1.5  2002/10/25 14:21:12  vrandezo
// Hauptmen� erweitert
// Charakteristika hinzugef�gt
// eigenes Mengen-GUI-Element erzeugt
//
// Revision 1.4  2002/10/20 14:25:11  vrandezo
// Eigenes Talentfenster eingef�hrt
//
// Revision 1.3  2002/10/19 20:35:32  vrandezo
// feste Stringliterale entfernt
//
// Revision 1.2  2002/10/18 00:50:16  vrandezo
// Talente bei der Heldenerschaffung einsetzbar
//
// Revision 1.1  2002/10/17 10:54:43  vrandezo
// Talent und Talente eingef�gt
//
//@nonl
//@-node:<< Version History >>
//@nl
//@-node:@file Menge.h
//@-leo
