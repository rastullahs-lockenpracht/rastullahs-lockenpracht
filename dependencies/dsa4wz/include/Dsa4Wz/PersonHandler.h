//@+leo-ver=4-encoding=iso-8859-1.
//@+node:@file PersonHandler.h
//@+at
// PersonHandler.h: Schnittstelle für die PersonHandler
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

#if !defined(PERSONHANDLER_H)
#define PERSONHANDLER_H

#include <string>
#include "Person.h"

namespace DSA {

	//@  << class PersonHandler >>
	//@+node:<< class PersonHandler >>
	class PersonHandler {
	public:
	  virtual ~PersonHandler() {};
	  virtual Person* P() = 0;
	  virtual void P(std::auto_ptr<Person> person) = 0;
	  virtual std::auto_ptr<Person> HolePerson() = 0;
	
	  virtual std::string ID() { return P()->ID(); }
	  virtual void ID(const std::string& id)
	    { if (IDSetzbar()) P()->ID(id); }
	  virtual bool IDSetzbar() { return false; }
	
	  virtual std::string Name() { return P()->Name(); }
	  virtual void Name(const std::string& name)
	    { if (NameSetzbar()) P()->Name(name); }
	  virtual bool NameSetzbar() { return false; }
	
	  virtual std::string Beschreibung() { return P()->Beschreibung(); }
	  virtual void Beschreibung(const std::string& b)
	    { if (BeschreibungSetzbar()) P()->Beschreibung(b); }
	  virtual bool BeschreibungSetzbar() { return false; }
	
	  virtual Rasse Rasse_() { return P()->Rasse_(); }
	  virtual void Rasse_(Rasse rasse)
	    { if (RasseWaehlbar()) P()->Rasse_(rasse); PasseAn(); }
	  virtual bool RasseSetzbar() { return false; }
	  virtual bool RasseWaehlbar() { return false; }
	  virtual Kultur Kultur_() { return P()->Kultur_(); }
	  virtual void Kultur_(Kultur kultur)
	    { if (KulturWaehlbar()) P()->Kultur_(kultur); PasseAn(); }
	  virtual bool KulturSetzbar() { return false; }
	  virtual bool KulturWaehlbar() { return false; }
	  virtual Profession Profession_() { return P()->Profession_(); }
	  virtual void Profession_(Profession profession)
	    { if (ProfessionWaehlbar()) P()->Profession_(profession); PasseAn(); }
	  virtual bool ProfessionSetzbar() { return false; }
	  virtual bool ProfessionWaehlbar() { return false; }
	
	  virtual int Wert(Eigenschaftsart e) { return P()->Wert(e); }
	  virtual int Basiswert(Eigenschaftsart e) { return P()->Basiswert(e); }
	  virtual bool Senkbar(Eigenschaftsart eigenschaft) { return false; }
	  virtual bool Steigerbar(Eigenschaftsart eigenschaft) { return false; }
	  virtual void Senke(Eigenschaftsart eigenschaft);
	  virtual void Steiger(Eigenschaftsart eigenschaft);
	
	  virtual void BerechneEigenschaftenMaxima() {};
	
	  virtual int Wert(AbgeleiteterWertArt a) { return P()->Wert(a); }
	  virtual int Basiswert(AbgeleiteterWertArt a) { return P()->Basiswert(a); }
	  virtual bool Senkbar(AbgeleiteterWertArt a) { return false; }
	  virtual bool Steigerbar(AbgeleiteterWertArt a) { return false; }
	  virtual void Senke(AbgeleiteterWertArt a);
	  virtual void Steiger(AbgeleiteterWertArt a);
	
	  virtual bool ZauberAktivierbar() { return (AktivierbareZauber().size()>0); }
	  virtual Zauberfertigkeiten AktivierbareZauber() { return Zauberfertigkeiten(); }
	  virtual bool TalentAktivierbar(const Talentgruppe gruppe = sonstige) { StringListe sl; AktivierbareTalente(sl, gruppe); return sl.size()!=0; }
	  virtual void AktivierbareTalente(StringListe&, const Talentgruppe gruppe = sonstige) {}
	  virtual void Aktiviere(const std::string& talent) { Log::Debug("PersonHandler::Akiviere nicht implementiert"); }
	  virtual bool TalentSteigerbar(const std::string& talent) { return false; }
	  virtual bool TalentSenkbar(const std::string& talent) { return false; }
	  virtual void TalentSteigern(const std::string& talent) { Log::Debug("PersonHandler::TalentSteigern nicht implementiert"); }
	  virtual void TalentSenken(const std::string& talent) { Log::Debug("PersonHandler::TalentSenken nicht implementiert"); }
	  virtual bool AufATSteigerbar(const std::string& kampftechnik) { return false; }
	  virtual bool AufPASteigerbar(const std::string& kampftechnik) { return false; }
	  virtual void AufATSteigern(const std::string& kampftechnik) { Log::Debug("AufATSteigern::TalentSteigern nicht implementiert"); }
	  virtual void AufPASteigern(const std::string& kampftechnik) { Log::Debug("AufPASteigern::TalentSteigern nicht implementiert"); }
	  // TODO Muss nach Talentsteigerungen und -senkungen auch PasseAn durchgeführt werden?
	
	  virtual bool CharakteristikaHinzufuegbar(int modus0beide1vor2nach = 0) { StringListe sl; HinzufuegbareCharakteristika(sl, modus0beide1vor2nach); return sl.size()!=0; }
	  virtual void HinzufuegbareCharakteristika(StringListe&, int modus0beide1vor2nach = 0) {}
	  virtual void FuegeHinzu(const std::string& c) { Log::Debug("PersonHandler::FuegeHinzu nicht implementiert"); }
	  virtual bool CharakteristikumSteigerbar(const std::string& c) { return false; }
	  virtual void CharakteristikumSteigern(const std::string& c) { Log::Debug("PersonHandler::CharakteristikumSteigern nicht implementiert"); }
	  virtual bool CharakteristikumSenkbar(const std::string& c) { return false; }
	  virtual void CharakteristikumSenken(const std::string& c) { Log::Debug("PersonHandler::CharakteristikumSenken nicht implementiert"); }
	
	  virtual bool SonderfertigkeitenHinzufuegbar() { StringListe sl; HinzufuegbareSonderfertigkeiten(sl); return sl.size()!=0; }
	  virtual void HinzufuegbareSonderfertigkeiten(StringListe&) {}
	  virtual void FuegeHinzuSF(const std::string& sf) { Log::Debug("PersonHandler::FuegeHinzuSF nicht implementiert"); }
	  virtual bool EntfernbarSF(const std::string& sf) { return false; }
	  virtual void EntferneSF(const std::string& sf) { Log::Debug("PersonHandler::EntferneSF nicht implementiert"); }
	  virtual bool SteigerbarSF(const std::string& sf) { return false; }
	  virtual void SteigerSF(const std::string& sf) { Log::Debug("PersonHandler::SteigerSF nicht implementiert"); }
	  virtual bool SenkbarSF(const std::string& sf) { return false; }
	  virtual void SenkeSF(const std::string& sf) { Log::Debug("PersonHandler::SenkeSF nicht implementiert"); }
	  
	
	  virtual bool AlterSetzbar() { return false; }
	  virtual void Alter(int alter) { if (AlterSetzbar()) P()->Alter(alter); }
	  virtual int Alter() { return P()->Alter(); }
	  virtual bool GroesseSetzbar() { return false; }
	  virtual void Groesse(int groesse) { if (GroesseSetzbar()) P()->Groesse(groesse); }
	  virtual int Groesse() { return P()->Groesse(); }
	  virtual bool GewichtSetzbar() { return false; }
	  virtual void Gewicht(int gewicht) { if (GewichtSetzbar()) P()->Gewicht(gewicht); }
	  virtual int Gewicht() { return P()->Gewicht(); }
	  virtual bool HauptfarbeSetzbar() { return false; }
	  virtual void Hauptfarbe(const std::string& farbe) { if (HauptfarbeSetzbar()) P()->Hauptfarbe(farbe); }
	  virtual std::string Hauptfarbe() { return P()->Hauptfarbe(); }
	  virtual bool AugenfarbeSetzbar() { return false; }
	  virtual void Augenfarbe(const std::string& farbe) { if (AugenfarbeSetzbar()) P()->Augenfarbe(farbe); }
	  virtual std::string Augenfarbe() { return P()->Augenfarbe(); }
	
	  virtual bool GeschlechtSetzbar() { return false; }
	  virtual bool Maennlich() { return P()->Maennlich(); }
	  virtual void Maennlich(bool m) { if (GeschlechtSetzbar()) P()->Maennlich(m); }
	  virtual bool Weiblich() { return P()->Weiblich(); }
	  virtual void Weiblich(bool w) { if (GeschlechtSetzbar()) P()->Weiblich(w); }
	
	  virtual void PasseAn() {}
	
	  virtual void Abschliessen() {};
	
	  virtual std::string HandlerArt() const = 0;
	};
	//@nonl
	//@-node:<< class PersonHandler >>
	//@nl
  
}

#endif // !defined(PERSONHANDLER_H)

//@<< Version History >>
//@+node:<< Version History >>
// $Log: PersonHandler.h,v $
// Revision 1.10  2004/10/17 19:12:39  vrandezo
// IDs und Bezeichner getrennt - I18N vorbereitet
// ACHTUNG! Daten sichern. Das Programm ist noch reichlich fehlerhaft.
// Aber Entwicklung ohne CVS wird mir zu doof.
// Vorsicht vor allem mit Helden und RKP.
// Tausend Kleinigkeiten geaendert.
// Neue Datensaetze in basis.xdi.
//
// Revision 1.9  2004/04/09 22:25:50  vrandezo
// Zauberfertigkeiten fuer Personen eintragbar
//
// Revision 1.8  2003/12/31 03:51:58  vrandezo
// Re-Generation erweitert
//
// Revision 1.7  2003/10/30 00:02:44  vrandezo
// Steigerbare Ritualkenntnisse
//
// Revision 1.6  2003/10/19 21:59:07  vrandezo
// In Leo eingecheckt
//
// Revision 1.5  2003/02/23 12:36:27  vrandezo
// Charakteristika-Reiter umbenannt zu Vor-/Nachteile & SF (Tim von Umbscheiden)
// Generierung abschließbar, ohne alle TGP und GP verbraucht zu haben (Stefan Holzmüller)
// das Schließen des Fensters bei der Auswahl von Talenten bei der Vergabe einer
//   Profession, Rasse oder Kultur während der Heldengenerierung führt nicht mehr
//   zum Absturz (Stefan Holzmüller)
// Neuer Vorteil: Fettleibig (Volker Strunk)
//
// Revision 1.4  2003/02/20 13:53:22  vrandezo
// Vorteile: Beidhändig, Linkshänder, Blutrausch, Kampfrausch
// weitere SF
// Eigenschaften haben Maximalwerte
//
// Revision 1.3  2003/02/18 17:42:21  vrandezo
// verbesserte GUI für SF und für Charakteristika in Generierung
//
// Revision 1.2  2003/02/18 01:19:14  vrandezo
// (erste) GUI für SF und für Charakteristika in Generierung
//
// Revision 1.1.1.1  2003/02/17 15:33:32  vrandezo
// Neue Verzeichnisstruktur
//
// Revision 1.15  2003/02/07 19:37:25  vrandezo
// Magische Professionen als Professionskategorie eingefügt
// Voraussetzungen können sich selber überprüfen (Implementierung fehlt)
// Charakteristika mit Beschreibung
// Charakteristika GP, Beschreibung editierbar
// KompaktPanel-GUI für Charakteristika angelegt
// VN bei Genererierung hinzufügen
// VN Liste erstellt
// VN-GUI bei RKP erstellt
// einfache VN möglich
//
// Revision 1.14  2003/01/26 16:49:04  vrandezo
// AT/PA jetzt bei Kampftechniken aufteilbar
//
// Revision 1.13  2003/01/26 14:27:29  vrandezo
// Astralenergie auf 0 gesetzt
// Helden haben jetzt Raum für Notizen
//
// Revision 1.12  2003/01/24 20:33:34  vrandezo
// übliche Kulturen auswählbar bei Heldenerschaffung
// Wert der Mutter- und Zweitsprache auch bei Modifikatoren korrekt
// Geschlecht wechselbar
//
// Revision 1.11  2003/01/19 01:05:22  vrandezo
// Mutter- und Zweitsprachen bei Heldenerschaffung vollständig integriert
// Werte passen sich ständig der aktuellen Klugheit an
// Sprachen und Schriften in Heldenerschaffung integriert
// Bei möglicher Talentaktivierung werden nur passende Talente angezeigt
//
// Revision 1.10  2002/11/03 17:48:36  vrandezo
// Haarfarbe und Augenfarbe jetzt auch bei der Heldenerschaffung
//
// Revision 1.9  2002/11/03 14:25:33  vrandezo
// Wuerfel angepasst
// Haar- und Augenfarbe hinzugefügt bei Rassen
//
// Revision 1.8  2002/10/19 20:35:32  vrandezo
// feste Stringliterale entfernt
//
// Revision 1.7  2002/10/18 00:50:16  vrandezo
// Talente bei der Heldenerschaffung einsetzbar
//
// Revision 1.6  2002/10/09 19:58:34  vrandezo
// Auswahl von Rasse, Kultur, Profession ermöglicht
//
// Revision 1.5  2002/10/02 01:28:57  vrandezo
// Erweiterung von GUI und DSA.lib
//
// Revision 1.4  2002/09/28 15:59:11  vrandezo
// Modifikatoren in GUI eingebunden
//
// Revision 1.3  2002/09/27 19:06:36  vrandezo
// Erweiterung von GUI und DSA.lib
//
// Revision 1.2  2002/09/26 14:52:20  vrandezo
// DSA.lib-Anbindung der GUI geschrieben
//
// Revision 1.1  2002/09/25 23:52:47  vrandezo
// Beginn der Aufteilung in DSA.lib und Anwendung
//
//@nonl
//@-node:<< Version History >>
//@nl
//@-node:@file PersonHandler.h
//@-leo
