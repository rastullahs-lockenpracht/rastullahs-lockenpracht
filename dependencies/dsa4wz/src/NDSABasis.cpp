//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file NDSABasis.cpp
//@+at
// NDSABasis : Implementierung der Funktionen im 
// Namensraum DSA
// 
// $Revision: 1.9 $
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
#include "NDSABasis.h"
#include "Texte.h"
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

//@<< liste2string >>
//@+node:<< liste2string >>
std::string liste2string(const StringListe& Liste, const std::string& Trennzeichen, const std::string& fallsLeer) {
  if (Liste.empty()) return fallsLeer;
  StringListe::const_iterator i = Liste.begin();
  std::string r = (*i);
  ++i;
  while (i != Liste.end()) {
    r += Trennzeichen + (*i);
    ++i;
  }
  return r;
}
//@nonl
//@-node:<< liste2string >>
//@nl
//@<< schluessel >>
//@+node:<< schluessel >>
std::string Schluessel(const std::string& s, const DDict& d) {
  for (DDict::const_iterator i = d.begin(); i != d.end(); ++i)
    for (Dict::const_iterator j = i->second.begin(); j != i->second.end(); ++j)
      if (s==j->second)
        return i->first;
  return "";
}
//@nonl
//@-node:<< schluessel >>
//@nl

namespace DSA	{

	//@  << namespace DSA >>
	//@+node:<< namespace DSA >>
	bool DatenGeaendert(const bool set)
	{ static bool a = false; bool c = a; a = set; return c; }
	
	//@+others
	//@+node:Eigenschaft
	std::string EigenschaftString(DSA::Eigenschaftsart e, bool lang, bool uebersetzt) {
	  std::string s;
	  if (lang) {
	    switch (e) {
	      case Mut              : s = "Mut";              break;
	      case Klugheit         : s = "Klugheit";         break;
	      case Intuition        : s = "Intuition";        break;
	      case Charisma         : s = "Charisma";         break;
	      case Fingerfertigkeit : s = "Fingerfertigkeit"; break;
	      case Gewandtheit      : s = "Gewandtheit";      break;
	      case Konstitution     : s = "Konstitution";     break;
	      case Koerperkraft     : s = "Koerperkraft";     break;
	      default               : s = "unbekannt";         
	    }
	  } else {
	    switch (e) {
	      case Mut              : s = "MU"; break;
	      case Klugheit         : s = "KL"; break;
	      case Intuition        : s = "IN"; break;
	      case Charisma         : s = "CH"; break;
	      case Fingerfertigkeit : s = "FF"; break;
	      case Gewandtheit      : s = "GE"; break;
	      case Konstitution     : s = "KO"; break;
	      case Koerperkraft     : s = "KK"; break;
	      default               : s = "unbekannt";
	    }
	  }
	  return (uebersetzt? _t(s) : s);
	} // EigenschaftString
	
	Eigenschaftsart EigenschaftString(const std::string& s) {
	  // nein, hier ist kein switch..case..default möglich, da std::string
	  if (s==_t("MU"))        return Mut;
	  if (s==_t("KL"))        return Klugheit;
	  if (s==_t("IN"))        return Intuition;
	  if (s==_t("CH"))        return Charisma;
	  if (s==_t("FF"))        return Fingerfertigkeit;
	  if (s==_t("GE"))        return Gewandtheit;
	  if (s==_t("KO"))        return Konstitution;
	  if (s==_t("KK"))        return Koerperkraft;
	  if (s==_t("Mut"))              return Mut;
	  if (s==_t("Klugheit"))         return Klugheit;
	  if (s==_t("Intuition"))        return Intuition;
	  if (s==_t("Charisma"))         return Charisma;
	  if (s==_t("Fingerfertigkeit")) return Fingerfertigkeit;
	  if (s==_t("Gewandtheit"))      return Gewandtheit;
	  if (s==_t("Konstitution"))     return Konstitution;
	  if (s==_t("Koerperkraft"))     return Koerperkraft;
	  if (s==("MU"))        return Mut;
	  if (s==("KL"))        return Klugheit;
	  if (s==("IN"))        return Intuition;
	  if (s==("CH"))        return Charisma;
	  if (s==("FF"))        return Fingerfertigkeit;
	  if (s==("GE"))        return Gewandtheit;
	  if (s==("KO"))        return Konstitution;
	  if (s==("KK"))        return Koerperkraft;
	  if (s==("Mut"))              return Mut;
	  if (s==("Klugheit"))         return Klugheit;
	  if (s==("Intuition"))        return Intuition;
	  if (s==("Charisma"))         return Charisma;
	  if (s==("Fingerfertigkeit")) return Fingerfertigkeit;
	  if (s==("Gewandtheit"))      return Gewandtheit;
	  if (s==("Konstitution"))     return Konstitution;
	  if (s==("Koerperkraft"))     return Koerperkraft;
	  if (s==("Körperkraft"))      return Koerperkraft;
	  Log::Warning("Unbekannte Eigenschaft: " + s);
	                          return unbekannteEigenschaft;
	} // EigenschaftString
	
	bool IstEigenschaft(const std::string& s) {
	  if (s==_t("MU"))        return true;
	  if (s==_t("KL"))        return true;
	  if (s==_t("IN"))        return true;
	  if (s==_t("CH"))        return true;
	  if (s==_t("FF"))        return true;
	  if (s==_t("GE"))        return true;
	  if (s==_t("KO"))        return true;
	  if (s==_t("KK"))        return true;
	  if (s==_t("Mut"))              return true;
	  if (s==_t("Klugheit"))         return true;
	  if (s==_t("Intuition"))        return true;
	  if (s==_t("Charisma"))         return true;
	  if (s==_t("Fingerfertigkeit")) return true;
	  if (s==_t("Gewandtheit"))      return true;
	  if (s==_t("Konstitution"))     return true;
	  if (s==_t("Koerperkraft"))     return true;
	  if (s==("MU"))        return true;
	  if (s==("KL"))        return true;
	  if (s==("IN"))        return true;
	  if (s==("CH"))        return true;
	  if (s==("FF"))        return true;
	  if (s==("GE"))        return true;
	  if (s==("KO"))        return true;
	  if (s==("KK"))        return true;
	  if (s==("Mut"))              return true;
	  if (s==("Klugheit"))         return true;
	  if (s==("Intuition"))        return true;
	  if (s==("Charisma"))         return true;
	  if (s==("Fingerfertigkeit")) return true;
	  if (s==("Gewandtheit"))      return true;
	  if (s==("Konstitution"))     return true;
	  if (s==("Koerperkraft"))     return true;
	  if (s==("Körperkraft"))      return true;
	  return false;
	}
	
	std::string NormalisiereEigenschaft(const std::string& s) {
	  return EigenschaftString(EigenschaftString(s), true, false);
	}
	//@-node:Eigenschaft
	//@+node:AbgeleiteterWert
	std::string AbgeleiteterWertString(AbgeleiteterWertArt a, bool lang, bool uebersetzt) {
	 if (uebersetzt) {
	  if (lang)
	    switch (a) {
	      case Lebensenergie    : return _t("Lebensenergie");
	      case Ausdauer         : return _t("Ausdauer");
	      case AttackeBasis     : return _t("AttackeBasis");
	      case ParadeBasis      : return _t("ParadeBasis");
	      case FernkampfBasis   : return _t("FernkampfBasis");
	      case InitiativeBasis  : return _t("InitiativeBasis");
	      case Magieresistenz   : return _t("Magieresistenz");
	      case Astralenergie    : return _t("Astralenergie");
	      case Sozialstatus     : return _t("Sozialstatus");
	      default               : return _t("unbekannt");
	    }
	  else
	    switch (a) {
	      case Lebensenergie    : return _t("LE");
	      case Ausdauer         : return _t("AU");
	      case AttackeBasis     : return _t("AT");
	      case ParadeBasis      : return _t("PA");
	      case FernkampfBasis   : return _t("FK");
	      case InitiativeBasis  : return _t("INI");
	      case Magieresistenz   : return _t("MR");
	      case Astralenergie    : return _t("AE");
	      case Sozialstatus     : return _t("SO");
	      default               : return _t("unbekannt");
	    }
	 } else {
	  if (lang)
	    switch (a) {
	      case Lebensenergie    : return ("Lebensenergie");
	      case Ausdauer         : return ("Ausdauer");
	      case AttackeBasis     : return ("AttackeBasis");
	      case ParadeBasis      : return ("ParadeBasis");
	      case FernkampfBasis   : return ("FernkampfBasis");
	      case InitiativeBasis  : return ("InitiativeBasis");
	      case Magieresistenz   : return ("Magieresistenz");
	      case Astralenergie    : return ("Astralenergie");
	      case Sozialstatus     : return ("Sozialstatus");
	      default               : return ("unbekannt");
	    }
	  else
	    switch (a) {
	      case Lebensenergie    : return ("LE");
	      case Ausdauer         : return ("AU");
	      case AttackeBasis     : return ("AT");
	      case ParadeBasis      : return ("PA");
	      case FernkampfBasis   : return ("FK");
	      case InitiativeBasis  : return ("INI");
	      case Magieresistenz   : return ("MR");
	      case Astralenergie    : return ("AE");
	      case Sozialstatus     : return ("SO");
	      default               : return ("unbekannt");
	    }
	 }
	} // AbgeleiteterWertString
	
	AbgeleiteterWertArt AbgeleiteterWertString(const std::string& s)	{
	  // nein, hier ist kein switch..case..default möglich, da std::string
	  if (s==_t("Lebensenergie"))    return Lebensenergie;
	  if (s==_t("Ausdauer"))         return Ausdauer;
	  if (s==_t("AttackeBasis"))     return AttackeBasis;
	  if (s==_t("ParadeBasis"))      return ParadeBasis;
	  if (s==_t("FernkampfBasis"))   return FernkampfBasis;
	  if (s==_t("InitiativeBasis"))  return InitiativeBasis;
	  if (s==_t("Magieresistenz"))   return Magieresistenz;
	  if (s==_t("Astralenergie"))    return Astralenergie;
	  if (s==_t("Sozialstatus"))     return Sozialstatus;
	  if (s==_t("LE"))               return Lebensenergie;
	  if (s==_t("AU"))               return Ausdauer;
	  if (s==_t("AT"))               return AttackeBasis;
	  if (s==_t("PA"))               return ParadeBasis;
	  if (s==_t("FK"))               return FernkampfBasis;
	  if (s==_t("INI"))              return InitiativeBasis;
	  if (s==_t("MR"))               return Magieresistenz;
	  if (s==_t("AE"))               return Astralenergie;
	  if (s==_t("SO"))               return Sozialstatus;
	  if (s==("Lebensenergie"))    return Lebensenergie;
	  if (s==("Ausdauer"))         return Ausdauer;
	  if (s==("AttackeBasis"))     return AttackeBasis;
	  if (s==("ParadeBasis"))      return ParadeBasis;
	  if (s==("FernkampfBasis"))   return FernkampfBasis;
	  if (s==("InitiativeBasis"))  return InitiativeBasis;
	  if (s==("Magieresistenz"))   return Magieresistenz;
	  if (s==("Astralenergie"))    return Astralenergie;
	  if (s==("Sozialstatus"))     return Sozialstatus;
	  if (s==("LE"))               return Lebensenergie;
	  if (s==("AU"))               return Ausdauer;
	  if (s==("AT"))               return AttackeBasis;
	  if (s==("PA"))               return ParadeBasis;
	  if (s==("FK"))               return FernkampfBasis;
	  if (s==("INI"))              return InitiativeBasis;
	  if (s==("MR"))               return Magieresistenz;
	  if (s==("AE"))               return Astralenergie;
	  if (s==("SO"))               return Sozialstatus;
	  Log::Warning("Unbekannter Abgeleiteter Wert: " + s);
	  return unbekannterAbgeleiteterWert;
	} // AbgeleiteterWertName
	
	bool IstAbgeleiteterWert(const std::string& s)	{
	  // nein, hier ist kein switch..case..default möglich, da std::string
	  if (s==_t("Lebensenergie"))    return true;
	  if (s==_t("Ausdauer"))         return true;
	  if (s==_t("AttackeBasis"))     return true;
	  if (s==_t("ParadeBasis"))      return true;
	  if (s==_t("FernkampfBasis"))   return true;
	  if (s==_t("InitiativeBasis"))  return true;
	  if (s==_t("Magieresistenz"))   return true;
	  if (s==_t("Astralenergie"))    return true;
	  if (s==_t("Sozialstatus"))     return true;
	  if (s==_t("LE"))               return true;
	  if (s==_t("AU"))               return true;
	  if (s==_t("AT"))               return true;
	  if (s==_t("PA"))               return true;
	  if (s==_t("FK"))               return true;
	  if (s==_t("INI"))              return true;
	  if (s==_t("MR"))               return true;
	  if (s==_t("AE"))               return true;
	  if (s==_t("SO"))               return true;
	  if (s==("Lebensenergie"))      return true;
	  if (s==("Ausdauer"))           return true;
	  if (s==("AttackeBasis"))       return true;
	  if (s==("ParadeBasis"))        return true;
	  if (s==("FernkampfBasis"))     return true;
	  if (s==("InitiativeBasis"))    return true;
	  if (s==("Magieresistenz"))     return true;
	  if (s==("Astralenergie"))      return true;
	  if (s==("Sozialstatus"))       return true;
	  if (s==("LE"))                 return true;
	  if (s==("AU"))                 return true;
	  if (s==("AT"))                 return true;
	  if (s==("PA"))                 return true;
	  if (s==("FK"))                 return true;
	  if (s==("INI"))                return true;
	  if (s==("MR"))                 return true;
	  if (s==("AE"))                 return true;
	  if (s==("SO"))                 return true;
	  return false;
	} // IstAbgeleiteterWert
	
	std::string NormalisiereAbgeleiteterWert(const std::string& s) {
	  return AbgeleiteterWertString(AbgeleiteterWertString(s), true, false);
	}
	//@-node:AbgeleiteterWert
	//@+node:Talentart
	std::string TalentartString(Talentart a, bool uebersetzt) {
	  std::string s;
	  switch (a) {
	    case Basis   : s = "Basistalent";   break;
	    case Spezial : s = "Spezialtalent"; break;
	    case Beruf   : s = "Berufstalent";  break;
	    default      : s = "unbekannt";
	  }
	  return (uebersetzt? _t(s) : s);
	} // TalentartString
	
	Talentart TalentartString(const std::string& s) {
	  if (s==_t("Basistalent"))   return Basis;
	  if (s==_t("Berufstalent"))  return Beruf;
	  if (s==_t("Spezialtalent")) return Spezial;
	  if (s==  ("Basistalent"))   return Basis;
	  if (s==  ("Berufstalent"))  return Beruf;
	  if (s==  ("Spezialtalent")) return Spezial;
	  if (s==  ("Basis"))         return Basis;
	  if (s==  ("Beruf"))         return Beruf;
	  if (s==  ("Spezial"))       return Spezial;
	  Log::Warning("Unbekannte Talentart: " + s);
	                              return unbekannteTalentart;
	} // TalentartString
	
	bool IstTalentart(const std::string& s) {
	  if (s==_t("Basistalent"))   return true;
	  if (s==_t("Berufstalent"))  return true;
	  if (s==_t("Spezialtalent")) return true;
	  if (s==  ("Basistalent"))   return true;
	  if (s==  ("Berufstalent"))  return true;
	  if (s==  ("Spezialtalent")) return true;
	  if (s==  ("Basis"))         return true;
	  if (s==  ("Beruf"))         return true;
	  if (s==  ("Spezial"))       return true;
	                              return false;
	} // IstTalentart
	
	std::string NormalisiereTalentart(const std::string& s) {
	  return TalentartString(TalentartString(s), false);
	}
	//@-node:Talentart
	//@+node:Talentgruppe
	std::string TalentgruppeString(Talentgruppe g, bool Singular, bool uebersetzt)	{
	  std::string s;
	  if (Singular)
	    switch (g) {
	      case Kampf                : s = "TalentKampf";          break;
	      case Koerperlich          : s = "TalentKoerper";        break;
	      case Gesellschaftlich     : s = "TalentGesellschaft";   break;
	      case Natur                : s = "TalentNatur";          break;
	      case Wissen               : s = "TalentWissen";         break;
	      case Handwerk             : s = "TalentHandwerk";       break;
	      case SprachenUndSchriften : s = "TalentSpracheSchrift"; break;
	      case Gabe                 : s = "TalentGabe";           break;
	      default                   : s = "Sonstiges";
	    }
	  else
	    switch (g) {
	      case Kampf                : s = "TalentgruppeKampf";          break;
	      case Koerperlich          : s = "TalentgruppeKoerper";        break;
	      case Gesellschaftlich     : s = "TalentgruppeGesellschaft";   break;
	      case Natur                : s = "TalentgruppeNatur";          break;
	      case Wissen               : s = "TalentgruppeWissen";         break;
	      case Handwerk             : s = "TalentgruppeHandwerk";       break;
	      case SprachenUndSchriften : s = "TalentgruppeSpracheSchrift"; break;
	      case Gabe                 : s = "TalentgruppeGabe";           break;
	      default                   : s = "Sonstiges";
	    }
	  return (uebersetzt? _t(s) : s);
	} // TalentgruppeString
	
	Talentgruppe TalentgruppeString(const std::string& s) {
	  if (s==_t("TalentgruppeKampf"))          return Kampf;
	  if (s==_t("TalentgruppeKoerper"))        return Koerperlich;
	  if (s==_t("TalentgruppeGesellschaft"))   return Gesellschaftlich;
	  if (s==_t("TalentgruppeNatur"))          return Natur;
	  if (s==_t("TalentgruppeWissen"))         return Wissen;
	  if (s==_t("TalentgruppeHandwerk"))       return Handwerk;
	  if (s==_t("TalentgruppeSpracheSchrift")) return SprachenUndSchriften;
	  if (s==_t("TalentgruppeGabe"))           return Gabe;
	  if (s==_t("TalentKampf"))          return Kampf;
	  if (s==_t("TalentKoerper"))        return Koerperlich;
	  if (s==_t("TalentGesellschaft"))   return Gesellschaftlich;
	  if (s==_t("TalentNatur"))          return Natur;
	  if (s==_t("TalentWissen"))         return Wissen;
	  if (s==_t("TalentHandwerk"))       return Handwerk;
	  if (s==_t("TalentSpracheSchrift")) return SprachenUndSchriften;
	  if (s==_t("TalentGabe"))           return Gabe;
	  if (s==("TalentgruppeKampf"))          return Kampf;
	  if (s==("TalentgruppeKoerper"))        return Koerperlich;
	  if (s==("TalentgruppeGesellschaft"))   return Gesellschaftlich;
	  if (s==("TalentgruppeNatur"))          return Natur;
	  if (s==("TalentgruppeWissen"))         return Wissen;
	  if (s==("TalentgruppeHandwerk"))       return Handwerk;
	  if (s==("TalentgruppeSpracheSchrift")) return SprachenUndSchriften;
	  if (s==("TalentgruppeGabe"))           return Gabe;
	  if (s==("TalentgruppeKampftechniken"))           return Kampf;
	  if (s==("TalentgruppeKoerperlicheTalente"))      return Koerperlich;
	  if (s==("TalentgruppeGesellschaftlicheTalente")) return Gesellschaftlich;
	  if (s==("TalentgruppeNaturTalente"))             return Natur;
	  if (s==("TalentgruppeWissenstalente"))           return Wissen;
	  if (s==("TalentgruppeHandwerklicheTalente"))     return Handwerk;
	  if (s==("TalentgruppeSprachenSchriften"))        return SprachenUndSchriften;
	  if (s==("TalentgruppeGaben"))                    return Gabe;
	  if (s==("TalentKampf"))          return Kampf;
	  if (s==("TalentKoerper"))        return Koerperlich;
	  if (s==("TalentGesellschaft"))   return Gesellschaftlich;
	  if (s==("TalentNatur"))          return Natur;
	  if (s==("TalentWissen"))         return Wissen;
	  if (s==("TalentHandwerk"))       return Handwerk;
	  if (s==("TalentSpracheSchrift")) return SprachenUndSchriften;
	  if (s==("TalentGabe"))           return Gabe;
	  Log::Warning("Unbekannte Talentgruppe: " + s);
	                                   return sonstige;
	} // TalentgruppeString
	
	bool IstTalentgruppe(const std::string& s) {
	  if (s==_t("TalentgruppeKampf"))          return true;
	  if (s==_t("TalentgruppeKoerper"))        return true;
	  if (s==_t("TalentgruppeGesellschaft"))   return true;
	  if (s==_t("TalentgruppeNatur"))          return true;
	  if (s==_t("TalentgruppeWissen"))         return true;
	  if (s==_t("TalentgruppeHandwerk"))       return true;
	  if (s==_t("TalentgruppeSpracheSchrift")) return true;
	  if (s==_t("TalentgruppeGabe"))           return true;
	  if (s==_t("TalentKampf"))          return true;
	  if (s==_t("TalentKoerper"))        return true;
	  if (s==_t("TalentGesellschaft"))   return true;
	  if (s==_t("TalentNatur"))          return true;
	  if (s==_t("TalentWissen"))         return true;
	  if (s==_t("TalentHandwerk"))       return true;
	  if (s==_t("TalentSpracheSchrift")) return true;
	  if (s==_t("TalentGabe"))           return true;
	  if (s==("TalentgruppeKampf"))          return true;
	  if (s==("TalentgruppeKoerper"))        return true;
	  if (s==("TalentgruppeGesellschaft"))   return true;
	  if (s==("TalentgruppeNatur"))          return true;
	  if (s==("TalentgruppeWissen"))         return true;
	  if (s==("TalentgruppeHandwerk"))       return true;
	  if (s==("TalentgruppeSpracheSchrift")) return true;
	  if (s==("TalentgruppeGabe"))           return true;
	  if (s==("TalentgruppeKampftechniken"))           return true;
	  if (s==("TalentgruppeKoerperlicheTalente"))      return true;
	  if (s==("TalentgruppeGesellschaftlicheTalente")) return true;
	  if (s==("TalentgruppeNaturTalente"))             return true;
	  if (s==("TalentgruppeWissenstalente"))           return true;
	  if (s==("TalentgruppeHandwerklicheTalente"))     return true;
	  if (s==("TalentgruppeSprachenSchriften"))        return true;
	  if (s==("TalentgruppeGaben"))                    return true;
	  if (s==("TalentKampf"))          return true;
	  if (s==("TalentKoerper"))        return true;
	  if (s==("TalentGesellschaft"))   return true;
	  if (s==("TalentNatur"))          return true;
	  if (s==("TalentWissen"))         return true;
	  if (s==("TalentHandwerk"))       return true;
	  if (s==("TalentSpracheSchrift")) return true;
	  if (s==("TalentGabe"))           return true;
	                                   return false;
	} // IstTalentgruppe
	
	std::string NormalisiereTalentgruppe(const std::string& s) {
	  return TalentgruppeString(TalentgruppeString(s), true, false);
	}
	//@-node:Talentgruppe
	//@+node:Kampfart
	std::string KampfartString(Kampfart art, bool uebersetzt) {
	  std::string s;
	  switch (art) {
	    case Fernkampf           : s = "KampfartFernkampf";           break;
	    case BewaffneterKampf    : s = "KampfartBewaffneterKampf";    break;
	    case WaffenloserKampf    : s = "KampfartWaffenloserKampf";    break;
	    case BewaffneterNahkampf : s = "KampfartBewaffneterNahkampf"; break;
	    default                  : s = "Unbekannt";
	  }
	  return (uebersetzt? _t(s) : s);
	} // KampfartString
	
	Kampfart KampfartString(const std::string& s) {
	  if (s==_t("KampfartFernkampf"))           return Fernkampf;
	  if (s==_t("KampfartBewaffneterKampf"))    return BewaffneterKampf;
	  if (s==_t("KampfartWaffenloserKampf"))    return WaffenloserKampf;
	  if (s==_t("KampfartBewaffneterNahkampf")) return BewaffneterNahkampf;
	  if (s==  ("KampfartFernkampf"))           return Fernkampf;
	  if (s==  ("KampfartBewaffneterKampf"))    return BewaffneterKampf;
	  if (s==  ("KampfartWaffenloserKampf"))    return WaffenloserKampf;
	  if (s==  ("KampfartBewaffneterNahkampf")) return BewaffneterNahkampf;
	  Log::Warning("Unbekannte Kampfart: " + s);
	                                              return unbekannteKampfart;
	} // KampfartString
	
	bool IstKampfart(const std::string& s) {
	  if (s==_t("KampfartFernkampf"))           return true;
	  if (s==_t("KampfartBewaffneterKampf"))    return true;
	  if (s==_t("KampfartWaffenloserKampf"))    return true;
	  if (s==_t("KampfartBewaffneterNahkampf")) return true;
	  if (s==  ("KampfartFernkampf"))           return true;
	  if (s==  ("KampfartBewaffneterKampf"))    return true;
	  if (s==  ("KampfartWaffenloserKampf"))    return true;
	  if (s==  ("KampfartBewaffneterNahkampf")) return true;
	                                              return false;
	} // IstKampfart
	
	std::string NormalisiereKampfart(const std::string& s) {
	  return KampfartString(KampfartString(s), false);
	}
	//@-node:Kampfart
	//@+node:SKT
	std::string SKTSpalteString(SKTSpalte s) {
	  switch (s) {
	    case TransA : return "A*";
	    case A : return "A";
	    case B : return "B";
	    case C : return "C";
	    case D : return "D";
	    case E : return "E";
	    case F : return "F";
	    case G : return "G";
	    default : return "H";
	  }
	} // SKTSpalteString
	
	SKTSpalte SKTSpalteString(const std::string& s) {
	  if (s=="A*")return TransA;
	  if (s=="A") return A;
	  if (s=="B") return B;
	  if (s=="C") return C;
	  if (s=="D") return D;
	  if (s=="E") return E;
	  if (s=="F") return F;
	  if (s=="G") return G;
	              return H;
	} // SKTSpalteString
	
	SKTSpalte SpalteLeichter(SKTSpalte s) {
	  switch (s) {
	    case TransA : return TransA;
	    case A : return TransA;
	    case B : return A;
	    case C : return B;
	    case D : return C;
	    case E : return D;
	    case F : return E;
	    case G : return F;
	    case H : return G;
	    default : Log::Error("NDSABasis::SpalteLeichter: unbekannte Spalte"); return A;
	  }
	}
	
	SKTSpalte SpalteSchwerer(SKTSpalte s) {
	  switch (s) {
	    case TransA : return A;
	    case A : return B;
	    case B : return C;
	    case C : return D;
	    case D : return E;
	    case E : return F;
	    case F : return G;
	    case G : return H;
	    case H : return H;
	    default : Log::Error("NDSABasis::SpalteLeichter: unbekannte Spalte"); return A;
	  }
	}
	//@-node:SKT
	//@+node:EBe
	std::string EBeString(EBe e, bool uebersetzt) {
	  std::string be = (uebersetzt ? _t("BE") : "BE");
	  switch (e) {
	    case BE       : return be;
	    case BEminus1 : return be + "-1";
	    case BEminus2 : return be + "-2";
	    case BEminus3 : return be + "-3";
	    case BEminus4 : return be + "-4";
	    case BEminus5 : return be + "-5";
	    case BEminus6 : return be + "-6";
	    case BEminus7 : return be + "-7";
	    case BEminus8 : return be + "-8";
	    case BEmal2   : return be + "x2";
	    case BEmal3   : return be + "x3";
	    default       : return "-";
	  }
	} 
	
	EBe EBeString(const std::string& s) {
	  std::string be = _t("BE");
	  if (s=="-")       return KeineBE;
	  if (s== be)       return BE;
	  if (s==(be+"-1")) return BEminus1;
	  if (s==(be+"-2")) return BEminus2;
	  if (s==(be+"-3")) return BEminus3;
	  if (s==(be+"-4")) return BEminus4;
	  if (s==(be+"-5")) return BEminus5;
	  if (s==(be+"-6")) return BEminus6;
	  if (s==(be+"-7")) return BEminus7;
	  if (s==(be+"-8")) return BEminus8;
	  if (s==(be+"x2")) return BEmal2;
	  if (s==(be+"x3")) return BEmal3;
	  be = "BE";
	  if (s== be)       return BE;
	  if (s==(be+"-1")) return BEminus1;
	  if (s==(be+"-2")) return BEminus2;
	  if (s==(be+"-3")) return BEminus3;
	  if (s==(be+"-4")) return BEminus4;
	  if (s==(be+"-5")) return BEminus5;
	  if (s==(be+"-6")) return BEminus6;
	  if (s==(be+"-7")) return BEminus7;
	  if (s==(be+"-8")) return BEminus8;
	  if (s==(be+"x2")) return BEmal2;
	  if (s==(be+"x3")) return BEmal3;
	                          return KeineBE;
	}
	
	int BerechneEBe(const int& be, EBe eBe) {
	  switch (eBe) {
	    case BE       : return be;
	    case BEminus1 : return be-1;
	    case BEminus2 : return be-2;
	    case BEminus3 : return be-3;
	    case BEminus4 : return be-4;
	    case BEminus5 : return be-5;
	    case BEminus6 : return be-6;
	    case BEminus7 : return be-7;
	    case BEminus8 : return be-8;
	    case BEmal2   : return be*2;
	    case BEmal3   : return be*3;
	    default       : return 0;
	  }
	}
	//@nonl
	//@-node:EBe
	//@+node:Zauberei
	//@+others
	//@+node:Merkmal
	std::string ZaubermerkmalString(Zaubermerkmal m, bool uebersetzt) {
	  std::string s;
	  switch(m) {
	    case Antimagie      : s = "Antimagie";     break;
	    case Beschwoerung   : s = "Beschwörung";   break;
	    case Daemonisch     : s = "Dämonisch";     break;
	    case Eigenschaften  : s = "Eigenschaften"; break;
	    case Einfluss       : s = "Einfluss";      break;
	    case Elementar      : s = "Elementar";     break;
	    case Form           : s = "Form";          break;
	    case Geisterwesen   : s = "Geisterwesen";  break;
	    case Heilung        : s = "Heilung";       break;
	    case Hellsicht      : s = "Hellsicht";     break;
	    case Herbeirufung   : s = "Herbeirufung";  break;
	    case Herrschaft     : s = "Herrschaft";    break;
	    case Illusion       : s = "Illusion";      break;
	    case Kraft          : s = "Kraft";         break;
	    case Limbus         : s = "Limbus";        break;
	    case Metamagie      : s = "Metamagie";     break;
	    case Objekt         : s = "Objekt";        break;
	    case Schaden        : s = "Schaden";       break;
	    case Telekinese     : s = "Telekinese";    break;
	    case Temporal       : s = "Temporal";      break;
	    case Umwelt         : s = "Umwelt";        break;
	    case Verstaendigung : s = "Verständigung"; break;
	    default             : s = "unbekannt";
	  }
	  return (uebersetzt? _t(s) : s);
	}
	
	Zaubermerkmal ZaubermerkmalString(const std::string& s) {
	  if (_t("Antimagie") == s)      return Antimagie;
	  if (_t("Beschwörung") == s)    return Beschwoerung;
	  if (_t("Dämonisch") == s)      return Daemonisch;
	  if (_t("Eigenschaften") == s)  return Eigenschaften;
	  if (_t("Einfluss") == s)       return Einfluss;
	  if (_t("Elementar") == s)      return Elementar;
	  if (_t("Form") == s)           return Form;
	  if (_t("Geisterwesen") == s)   return Geisterwesen;
	  if (_t("Heilung") == s)        return Heilung;
	  if (_t("Hellsicht") == s)      return Hellsicht;
	  if (_t("Herbeirufung") == s)   return Herbeirufung;
	  if (_t("Herrschaft") == s)     return Herrschaft;
	  if (_t("Illusion") == s)       return Illusion;
	  if (_t("Kraft") == s)          return Kraft;
	  if (_t("Limbus") == s)         return Limbus;
	  if (_t("Metamagie") == s)      return Metamagie;
	  if (_t("Objekt") == s)         return Objekt;
	  if (_t("Schaden") == s)        return Schaden;
	  if (_t("Telekinese") == s)     return Telekinese;
	  if (_t("Temporal") == s)       return Temporal;
	  if (_t("Umwelt") == s)         return Umwelt;
	  if (_t("Verständigung") == s)  return Verstaendigung;
	  if (("Antimagie") == s)      return Antimagie;
	  if (("Beschwörung") == s)    return Beschwoerung;
	  if (("Dämonisch") == s)      return Daemonisch;
	  if (("Eigenschaften") == s)  return Eigenschaften;
	  if (("Einfluss") == s)       return Einfluss;
	  if (("Elementar") == s)      return Elementar;
	  if (("Form") == s)           return Form;
	  if (("Geisterwesen") == s)   return Geisterwesen;
	  if (("Heilung") == s)        return Heilung;
	  if (("Hellsicht") == s)      return Hellsicht;
	  if (("Herbeirufung") == s)   return Herbeirufung;
	  if (("Herrschaft") == s)     return Herrschaft;
	  if (("Illusion") == s)       return Illusion;
	  if (("Kraft") == s)          return Kraft;
	  if (("Limbus") == s)         return Limbus;
	  if (("Metamagie") == s)      return Metamagie;
	  if (("Objekt") == s)         return Objekt;
	  if (("Schaden") == s)        return Schaden;
	  if (("Telekinese") == s)     return Telekinese;
	  if (("Temporal") == s)       return Temporal;
	  if (("Umwelt") == s)         return Umwelt;
	  if (("Verständigung") == s)  return Verstaendigung;
	  if (("Beschwoerung") == s)    return Beschwoerung;
	  if (("Daemonisch") == s)      return Daemonisch;
	  if (strings::BeginntMit(s, _t("Elementar")))  return Elementar;
	  if (strings::BeginntMit(s,   ("Elementar")))  return Elementar;
	  if (strings::BeginntMit(s, _t("Daemonisch"))) return Daemonisch;
	  if (strings::BeginntMit(s,   ("Daemonisch"))) return Daemonisch;
	  if (strings::BeginntMit(s, _t("Dämonisch")))  return Daemonisch;
	  if (strings::BeginntMit(s,   ("Dämonisch")))  return Daemonisch;
	  Log::Warning("Unbekanntes Merkmal: " + s);
	  return unbekanntesMerkmal;
	}
	
	bool IstZaubermerkmal(const std::string& s) {
	  if (_t("Antimagie") == s)      return true;
	  if (_t("Beschwörung") == s)    return true;
	  if (_t("Dämonisch") == s)      return true;
	  if (_t("Eigenschaften") == s)  return true;
	  if (_t("Einfluss") == s)       return true;
	  if (_t("Elementar") == s)      return true;
	  if (_t("Form") == s)           return true;
	  if (_t("Geisterwesen") == s)   return true;
	  if (_t("Heilung") == s)        return true;
	  if (_t("Hellsicht") == s)      return true;
	  if (_t("Herbeirufung") == s)   return true;
	  if (_t("Herrschaft") == s)     return true;
	  if (_t("Illusion") == s)       return true;
	  if (_t("Kraft") == s)          return true;
	  if (_t("Limbus") == s)         return true;
	  if (_t("Metamagie") == s)      return true;
	  if (_t("Objekt") == s)         return true;
	  if (_t("Schaden") == s)        return true;
	  if (_t("Telekinese") == s)     return true;
	  if (_t("Temporal") == s)       return true;
	  if (_t("Umwelt") == s)         return true;
	  if (_t("Verständigung") == s)  return true;
	  if (("Antimagie") == s)      return true;
	  if (("Beschwörung") == s)    return true;
	  if (("Dämonisch") == s)      return true;
	  if (("Eigenschaften") == s)  return true;
	  if (("Einfluss") == s)       return true;
	  if (("Elementar") == s)      return true;
	  if (("Form") == s)           return true;
	  if (("Geisterwesen") == s)   return true;
	  if (("Heilung") == s)        return true;
	  if (("Hellsicht") == s)      return true;
	  if (("Herbeirufung") == s)   return true;
	  if (("Herrschaft") == s)     return true;
	  if (("Illusion") == s)       return true;
	  if (("Kraft") == s)          return true;
	  if (("Limbus") == s)         return true;
	  if (("Metamagie") == s)      return true;
	  if (("Objekt") == s)         return true;
	  if (("Schaden") == s)        return true;
	  if (("Telekinese") == s)     return true;
	  if (("Temporal") == s)       return true;
	  if (("Umwelt") == s)         return true;
	  if (("Verständigung") == s)  return true;
	  if (("Beschwoerung") == s)   return true;
	  if (("Daemonisch") == s)     return true;
	  if (strings::BeginntMit(s, _t("Elementar")))  return true;
	  if (strings::BeginntMit(s,   ("Elementar")))  return true;
	  if (strings::BeginntMit(s, _t("Daemonisch"))) return true;
	  if (strings::BeginntMit(s,   ("Daemonisch"))) return true;
	  if (strings::BeginntMit(s, _t("Dämonisch")))  return true;
	  if (strings::BeginntMit(s,   ("Dämonisch")))  return true;
	  return false;
	}
	
	std::string NormalisiereZaubermerkmal(const std::string& s) {
	  return ZaubermerkmalString(ZaubermerkmalString(s), false);
	}
	//@-node:Merkmal
	//@+node:Elemente
	std::string ElementString(Element m, bool uebersetzt) {
	  std::string s;
	  switch(m) {
	    case GesamtElementar:s="Gesamt";  break;
	    case Erz   : s = "Erz";   break;
	    case Feuer : s = "Feuer"; break;
	    case Eis   : s = "Eis";   break;
	    case Luft  : s = "Luft";  break;
	    case Wasser: s = "Wasser";break;
	    case Humus : s = "Humus"; break;
	    default    : s = "Unbekannt";
	  }
	  return (uebersetzt? _t(s) : s);
	}
	
	Element ElementString(const std::string& s) {
	  if (_t("Gesamt") == s) return GesamtElementar;
	  if (_t("Erz") == s)    return Erz;
	  if (_t("Feuer") == s)  return Feuer;
	  if (_t("Eis") == s)    return Eis;
	  if (_t("Luft") == s)   return Luft;
	  if (_t("Wasser") == s) return Wasser;
	  if (_t("Humus") == s)  return Humus;
	  if (("Gesamt") == s) return GesamtElementar;
	  if (("Erz") == s)    return Erz;
	  if (("Feuer") == s)  return Feuer;
	  if (("Eis") == s)    return Eis;
	  if (("Luft") == s)   return Luft;
	  if (("Wasser") == s) return Wasser;
	  if (("Humus") == s)  return Humus;
	  Log::Warning("Unbekanntes Element: " + s);
	  return unbekanntesElement;
	}
	
	bool IstElement(const std::string& s) {
	  if (_t("Gesamt") == s) return true;
	  if (_t("Erz") == s)    return true;
	  if (_t("Feuer") == s)  return true;
	  if (_t("Eis") == s)    return true;
	  if (_t("Luft") == s)   return true;
	  if (_t("Wasser") == s) return true;
	  if (_t("Humus") == s)  return true;
	  if (("Gesamt") == s) return true;
	  if (("Erz") == s)    return true;
	  if (("Feuer") == s)  return true;
	  if (("Eis") == s)    return true;
	  if (("Luft") == s)   return true;
	  if (("Wasser") == s) return true;
	  if (("Humus") == s)  return true;
	  return false;
	}
	
	std::string NormalisiereElement(const std::string& s) {
	  return ElementString(ElementString(s), false);
	}
	//@-node:Elemente
	//@+node:Erzdämonen
	std::string ErzdaemonString(Erzdaemon m, bool uebersetzt) {
	  std::string s;
	  switch(m) {
	    case GesamtDaemonisch : s = "Gesamt";       break;
	    case Blakharaz        : s = "Blakharaz";    break;
	    case Belhalhar        : s = "Belhalhar";    break;
	    case Charyptoroth     : s = "Charyptoroth"; break;
	    case Lolgramoth       : s = "Lolgramoth";   break;
	    case Thargunitoth     : s = "Thargunitoth"; break;
	    case Amazeroth        : s = "Amazeroth";    break;
	    case Nagrach          : s = "Nagrach";      break;
	    case Asfaloth         : s = "Asfaloth";     break;
	    case Tasfarelel       : s = "Tasfarelel";   break;
	    case Mishkhara        : s = "Mishkhara";    break;
	    case Agrimoth         : s = "Agrimoth";     break;
	    case Belkekel         : s = "Belkekel";     break;
	    case unbekannteDomaene: s = "unbekannt";    break;
	    default               : s = "Namenlos";
	  }
	  return (uebersetzt? _t(s) : s);
	}
	
	Erzdaemon ErzdaemonString(const std::string& s) {
	  if (_t("Gesamt") == s)       return GesamtDaemonisch;
	  if (_t("Blakharaz") == s)    return Blakharaz;
	  if (_t("Belhalhar") == s)    return Belhalhar;
	  if (_t("Charyptoroth") == s) return Charyptoroth;
	  if (_t("Lolgramoth") == s)   return Lolgramoth;
	  if (_t("Thargunitoth") == s) return Thargunitoth;
	  if (_t("Amazeroth") == s)    return Amazeroth;
	  if (_t("Nagrach") == s)      return Nagrach;
	  if (_t("Asfaloth") == s)     return Asfaloth;
	  if (_t("Tasfarelel") == s)   return Tasfarelel;
	  if (_t("Mishkhara") == s)    return Mishkhara;
	  if (_t("Agrimoth") == s)     return Agrimoth;
	  if (_t("Belkekel") == s)     return Belkekel;
	  if (("Gesamt") == s)       return GesamtDaemonisch;
	  if (("Blakharaz") == s)    return Blakharaz;
	  if (("Belhalhar") == s)    return Belhalhar;
	  if (("Charyptoroth") == s) return Charyptoroth;
	  if (("Lolgramoth") == s)   return Lolgramoth;
	  if (("Thargunitoth") == s) return Thargunitoth;
	  if (("Amazeroth") == s)    return Amazeroth;
	  if (("Nagrach") == s)      return Nagrach;
	  if (("Asfaloth") == s)     return Asfaloth;
	  if (("Tasfarelel") == s)   return Tasfarelel;
	  if (("Mishkhara") == s)    return Mishkhara;
	  if (("Agrimoth") == s)     return Agrimoth;
	  if (("Belkekel") == s)     return Belkekel;
	  Log::Warning("Unbekannte Domäne: " + s);
	  return unbekannteDomaene;
	}
	
	bool IstErzdaemon(const std::string& s) {
	  if (_t("Gesamt") == s)       return true;
	  if (_t("Blakharaz") == s)    return true;
	  if (_t("Belhalhar") == s)    return true;
	  if (_t("Charyptoroth") == s) return true;
	  if (_t("Lolgramoth") == s)   return true;
	  if (_t("Thargunitoth") == s) return true;
	  if (_t("Amazeroth") == s)    return true;
	  if (_t("Nagrach") == s)      return true;
	  if (_t("Asfaloth") == s)     return true;
	  if (_t("Tasfarelel") == s)   return true;
	  if (_t("Mishkhara") == s)    return true;
	  if (_t("Agrimoth") == s)     return true;
	  if (_t("Belkekel") == s)     return true;
	  if (("Gesamt") == s)       return true;
	  if (("Blakharaz") == s)    return true;
	  if (("Belhalhar") == s)    return true;
	  if (("Charyptoroth") == s) return true;
	  if (("Lolgramoth") == s)   return true;
	  if (("Thargunitoth") == s) return true;
	  if (("Amazeroth") == s)    return true;
	  if (("Nagrach") == s)      return true;
	  if (("Asfaloth") == s)     return true;
	  if (("Tasfarelel") == s)   return true;
	  if (("Mishkhara") == s)    return true;
	  if (("Agrimoth") == s)     return true;
	  if (("Belkekel") == s)     return true;
	  return false;
	}
	
	std::string NormalisiereErzdaemon(const std::string& s) {
	  return ErzdaemonString(ErzdaemonString(s), false);
	}
	//@-node:Erzdämonen
	//@+node:Repräsentationen
	std::string ZauberrepraesentationString(Zauberrepraesentation m, bool lang, bool uebersetzt) {
	  std::string s ="Unbekannt";
	  if (lang)
	    switch(m) {
	      case Gildenmagier         : s = "Gildenmagier";         break;
	      case Elfen                : s = "Elfen";                break;
	      case Druiden              : s = "Druiden";              break;
	      case Hexen                : s = "Hexen";                break;
	      case Geoden               : s = "Geoden";               break;
	      case Schelme              : s = "Schelme";              break;
	      case Scharlatane          : s = "Scharlatane";          break;
	      case Borbaradianer        : s = "Borbaradianer";        break;
	      case AchazKristallomanten : s = "Achaz-Kristallomanten";break;
	      default                   : s = "Unbekannt";
	    }
	  else
	    switch(m) {
	      case Gildenmagier         : s = "Mag"; break;
	      case Elfen                : s = "Elf"; break;
	      case Druiden              : s = "Dru"; break;
	      case Hexen                : s = "Hex"; break;
	      case Geoden               : s = "Geo"; break;
	      case Schelme              : s = "Sch"; break;
	      case Scharlatane          : s = "Srl"; break;
	      case Borbaradianer        : s = "Bor"; break;
	      case AchazKristallomanten : s = "Ach"; break;
	      default                   : s = "???";
	    }
	  return (uebersetzt? _t(s) : s);
	}
	
	Zauberrepraesentation ZauberrepraesentationString(const std::string& s) {
	  if (_t("Mag") == s) return Gildenmagier;
	  if (_t("Elf") == s) return Elfen;
	  if (_t("Dru") == s) return Druiden;
	  if (_t("Hex") == s) return Hexen;
	  if (_t("Geo") == s) return Geoden;
	  if (_t("Sch") == s) return Schelme;
	  if (_t("Srl") == s) return Scharlatane;
	  if (_t("Bor") == s) return Borbaradianer;
	  if (_t("Ach") == s) return AchazKristallomanten;
	  if (_t("Gildenmagier") == s)          return Gildenmagier;
	  if (_t("Elfen") == s)                 return Elfen;
	  if (_t("Druiden") == s)               return Druiden;
	  if (_t("Hexen") == s)                 return Hexen;
	  if (_t("Geoden") == s)                return Geoden;
	  if (_t("Schelme") == s)               return Schelme;
	  if (_t("Scharlatane") == s)           return Scharlatane;
	  if (_t("Borbaradianer") == s)         return Borbaradianer;
	  if (_t("Achaz-Kristallomanten") == s) return AchazKristallomanten;
	  if (("Mag") == s) return Gildenmagier;
	  if (("Elf") == s) return Elfen;
	  if (("Dru") == s) return Druiden;
	  if (("Hex") == s) return Hexen;
	  if (("Geo") == s) return Geoden;
	  if (("Sch") == s) return Schelme;
	  if (("Srl") == s) return Scharlatane;
	  if (("Bor") == s) return Borbaradianer;
	  if (("Ach") == s) return AchazKristallomanten;
	  if (("Gildenmagier") == s)          return Gildenmagier;
	  if (("Elfen") == s)                 return Elfen;
	  if (("Druiden") == s)               return Druiden;
	  if (("Hexen") == s)                 return Hexen;
	  if (("Geoden") == s)                return Geoden;
	  if (("Schelme") == s)               return Schelme;
	  if (("Scharlatane") == s)           return Scharlatane;
	  if (("Borbaradianer") == s)         return Borbaradianer;
	  if (("Achaz-Kristallomanten") == s) return AchazKristallomanten;
	  if (("AchazKristallomanten") == s) return AchazKristallomanten;
	  Log::Warning("Unbekannte Repräsentation: " + s);
	  return unbekannteRepraesentation;
	}
	
	bool IstRepraesentation(const std::string& s) {
	  if (_t("Mag") == s) return true;
	  if (_t("Elf") == s) return true;
	  if (_t("Dru") == s) return true;
	  if (_t("Hex") == s) return true;
	  if (_t("Geo") == s) return true;
	  if (_t("Sch") == s) return true;
	  if (_t("Srl") == s) return true;
	  if (_t("Bor") == s) return true;
	  if (_t("Ach") == s) return true;
	  if (_t("Gildenmagier") == s)          return true;
	  if (_t("Elfen") == s)                 return true;
	  if (_t("Druiden") == s)               return true;
	  if (_t("Hexen") == s)                 return true;
	  if (_t("Geoden") == s)                return true;
	  if (_t("Schelme") == s)               return true;
	  if (_t("Scharlatane") == s)           return true;
	  if (_t("Borbaradianer") == s)         return true;
	  if (_t("Achaz-Kristallomanten") == s) return true;
	  if (("Mag") == s) return true;
	  if (("Elf") == s) return true;
	  if (("Dru") == s) return true;
	  if (("Hex") == s) return true;
	  if (("Geo") == s) return true;
	  if (("Sch") == s) return true;
	  if (("Srl") == s) return true;
	  if (("Bor") == s) return true;
	  if (("Ach") == s) return true;
	  if (("Gildenmagier") == s)          return true;
	  if (("Elfen") == s)                 return true;
	  if (("Druiden") == s)               return true;
	  if (("Hexen") == s)                 return true;
	  if (("Geoden") == s)                return true;
	  if (("Schelme") == s)               return true;
	  if (("Scharlatane") == s)           return true;
	  if (("Borbaradianer") == s)         return true;
	  if (("Achaz-Kristallomanten") == s) return true;
	  if (("AchazKristallomanten") == s)  return true;
	  return false;
	}
	
	std::string NormalisiereZauberrepraesentation(const std::string& s) {
	  return ZauberrepraesentationString(ZauberrepraesentationString(s), true, false);
	}
	
	//@-node:Repräsentationen
	//@-others
	//@-node:Zauberei
	//@-others
	//@-node:<< namespace DSA >>
	//@nl
  
}

//@<< Version History >>
//@+node:<< Version History >>
// $Log: NDSABasis.cpp,v $
// Revision 1.9  2004/10/17 19:12:39  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.8  2004/05/15 20:55:47  dertron
// Überflüssige ";" entfernt (pedantic-Warnung)
//
// Revision 1.7  2004/04/24 12:26:59  vrandezo
// TransA leichter ist TransA
//
// Revision 1.6  2004/04/10 23:01:28  vrandezo
// SKT Spalte A* eingefuehrt
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
// Revision 1.2  2003/03/25 09:37:40  vrandezo
// Begabung für [Talentgruppe]
//
// Revision 1.1.1.1  2003/02/17 15:33:07  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.15  2003/02/07 10:49:34  vrandezo
// Bug: [eine Sprache] und [eine Schrift] führt zu Absturz
//
// Revision 1.14  2003/01/17 01:35:11  vrandezo
// Speichern stark verkürzt, von der Dauer her
// Erinnert beim Beenden des Programms an das Speichern veränderter Daten
//
// Revision 1.13  2002/11/05 21:49:18  vrandezo
// Sprachen und Schriften im- und exportierbar
// bool const ueberschreiben aus Speicher-Schnittstelle entfernt
// Mengen haben IDs erhalten
// Voraussetzungen für Talente (allgemeine und meisterliche)
// MengeGUIwx beim Wechseln von Objekten mit Rückfrage
//
// Revision 1.12  2002/11/02 23:36:57  vrandezo
// Exportier und Importierfunktionen implementiert
// Vergleichsoperationen implementiert
// Kampftechnikfenstter implementiert
//
// Revision 1.11  2002/11/02 15:39:48  vrandezo
// Importfenster hinzugefügt
//
// Revision 1.10  2002/11/02 11:44:41  vrandezo
// Kampftechnik hinzugefügt
//
// Revision 1.9  2002/10/19 20:35:32  vrandezo
// feste Stringliterale entfernt
//
// Revision 1.8  2002/10/19 18:58:32  vrandezo
// Texte in eigene, externe Datei untergebracht
//
// Revision 1.7  2002/09/27 19:06:34  vrandezo
// Erweiterung von GUI und DSA.lib
//
// Revision 1.6  2002/09/26 14:52:20  vrandezo
// DSA.lib-Anbindung der GUI geschrieben
//
// Revision 1.5  2002/09/25 23:50:51  vrandezo
// Beginn der Aufteilung in DSA.lib und Anwendung
//
// Revision 1.4  2002/08/30 15:06:17  vrandezo
// Apache Xerces ersetzt den MSXML
// Zahlreiche Speicherlecks entfernt
// Entwurf verstärkt
//
// Revision 1.3  2002/08/09 11:33:56  vrandezo
// Der Xerces Parser hinzugefügt. Lesen kann er schon.
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
//@-node:@file NDSABasis.cpp
//@-leo
