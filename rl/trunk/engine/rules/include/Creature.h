/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Clarified Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Clarified Artistic License for more details.
 *
 *  You should have received a copy of the Clarified Artistic License
 *  along with this program; if not you can get it here
 *  http://www.jpaulmorrison.com/fbp/artistic2.htm.
 */



#ifndef __CREATURE_H__
#define __CREATURE_H__

#include "RulesPrerequisites.h"

#include "GameObject.h"
#include "Eigenschaft.h"
#include "Container.h"
#include "Weapon.h"

using namespace std;

namespace rl
{

	static const int WERT_MOD_AE = 1;
	static const int WERT_MOD_LE = 2;
	static const int WERT_MOD_AT = 3;
	static const int WERT_MOD_PA = 4;
	static const int WERT_MOD_FK = 5;
	static const int WERT_MOD_AU = 6;
	static const int WERT_MOD_MR = 7;
	static const int WERT_MOD_INI = 8;
	static const int WERT_SOZIALSTATUS = 9;

	static const int RESULT_AUTOERFOLG = 100;
	static const int RESULT_SPEKT_AUTOERFOLG = 1000;
	static const int RESULT_AUTOMISSERFOLG = -100;
	static const int RESULT_SPEKT_AUTOMISSERFOLG = -1000;
	static const int RESULT_GLUECKLICH = 100;
	static const int RESULT_PATZER = -100;
	static const int RESULT_ERFOLG = 1;
	static const int RESULT_MISSERFOLG = -1;

	const int SF_MIN_VALUE = 0;
	const int SF_MAX_VALUE = 2;
	static const int SF_IN_TRAINING = 0;
	static const int SF_OK = 1;
	static const int SF_PREREQ_NOT_MET = 2; /// @todo Wird das ueberhaupt gebraucht?

    /**
    * @brief Basisklasse aller spielrelevanten Objekte in RL.
    *
    * Klasse kapselt alle Wesen, die aktiv im Spiel agieren, sei es
    * computer- oder spielergesteuert. Klasse enthaelt Methoden und
    * Daten fuer die DSA-Spielwerte (Lebensenergie, Eigenschaften)
    * und Methoden fuer alles was fuer die Interaktion mit der Spielwelt
    * noetig ist (z.B. Proben).
    */
    class _RlRulesExport Creature : public GameObject
    {

    protected:
		/** @brief Liefert den Basiswert id zurueck.
		 *  @param id Bezeichnet welcher Wert zurueckgeliefert werden soll.
		 *  @return Der Wert des Basiswerts.
		 *  @exception InvalidArgumentException id konnte in mWerte nicht
		 *    gefunden werden.
		 */
        virtual int getWert(int id) const;
		virtual int getMrBasis() const;
        virtual int getLeBasis() const;
		virtual int getAuBasis() const;
		virtual int getAeBasis() const;

    public:
		typedef map<int, int> WertMap;
		/**
		 *  @brief Liste der guten Eigenschaften.
		 *  Besteht aus dem Abkuerzung der Eigenschaft (z.B. MU, KL) als 
		 *  Schluessel und ihrem Wert.
		 **/
		typedef map<CeGuiString, int> EigenschaftMap;
		/**
		 *  @brief Liste der Talente.
		 *  Besteht aus den Namen der Talente (z.B. Athletik) als
		 *  Schluessel und ihrem Wert.
		 **/
		typedef map<CeGuiString, int> TalentMap;
		/**
		 *  @brief Liste der Kampftechniken und ihrer AT/PA Werte.\n
		 *  Eine Kampftechnik in diesem Sinne ist so was wie Hiebwaffen
		 *  oder Raufen, bei Tieren sowas wie Biss oder Prankenhieb.\n
		 *  Der erste Wert entspricht der ID der Kampftechnik, das pair
		 *  den AT und PA Werten.
		 *  Die Werte werden auf den AT/PA Basiswert addiert bevor sie die fertige
		 *  AT/PA Werte ergeben. Die Summe des pairs muss also dem TaW in dem
		 *  Kampftalent entsprechen.
		 **/
        typedef map<CeGuiString, pair<int, int> > KampftechnikMap;
		/** @brief Die Sonderfertigkeiten der Kreatur
		 *  Besteht aus dem Namen der Sonderfertigkeit als Schluessel
		 *  und ihrem Status: \n
		 *  SF_LEARNING \n
		 *	SF_OK \n
		 *	SF_PREREQ_NOT_MET \n
		 */
		typedef map<CeGuiString ,int> SonderfertigkeitMap;
		/** @brief Die Container einer Kreatur.
		*  Diese Container sollen dann alles beinhalten, was direkt am Körper 
		*  getragen wird (Kleidung, Rucksäcke, Ringe etc.), sowie angeborene Waffen
		*  (Fäuste, Klauen, Zähne...).
		**/
		typedef map<CeGuiString, Container*> ContainerMap;
		/** @brief Eine Map mit Zeigern auf alle Waffen, die die Kreatur zur
		*   Zeit einsetzen kann.
		**/
		typedef map<int, Weapon*> WeaponMap;



		///@warning Nur zu Testzwecken da. Wird spaeter entfernt.
        Creature(const CeGuiString& name,
                 const CeGuiString& description);
		        
        virtual ~Creature();

		/**
		 *  @brief Liefert den Wert der Eigenschaft eigenschaftName zurueck.
		 *  @param eigenschaftName Der Name al Abkuerzung (z.B. MU, FF, etc.).
		 *  @return Der Wert der Eigenschaft.
		 *  @exception InvalidArgumentException Die Eigenschaft konnte nicht
		 *  gefunden werden (Name ausgeschrieben statt abgekuerzt? 
		 *  Groß/Kleinschreibung beachtet?).
		 **/
        virtual int getEigenschaft(const CeGuiString& eigenschaftName) const;
		/**
		 *  @brief Setzt den Wert der Eigenschaft eigenschaftName auf value.
		 *  @param eigenschaftName Der Name al Abkuerzung (z.B. MU, FF, etc.).
		 *  @param value Der Wert auf den die Eigenschaft gesetzt werden soll.
		 *  @exception InvalidArgumentException Die Eigenschaft konnte nicht
		 *  gefunden werden (Name ausgeschrieben statt abgekuerzt? 
		 *  Groß/Kleinschreibung beachtet?).
		 **/
        virtual void setEigenschaft(const CeGuiString& eigenschaftName, int value);
		/** @brief Addiert mod auf den Wert der Eigenschaft eigenschaftName.
		 *  @param eigenschaftName Der Name al Abkuerzung (z.B. MU, FF, etc.).
		 *  @param mod Wird auf den Wert addiert (kann auch negativ sein).
		 *  @exception InvalidArgumentException Die Eigenschaft konnte nicht
		 *  gefunden werden (Name ausgeschrieben statt abgekuerzt? 
		 *  Groß/Kleinschreibung beachtet?).
		 **/
        virtual void modifyEigenschaft(const CeGuiString& eigenschaftName, int mod);

		/**
		 *  @brief Fuegt eine neue Kampftechnik zu mKampftechniken hinzu
		 *  @param kampftechnikName Bezeichnet die Kampftechnik
		 *  @param value Initialisiert die Kampftechnik mit value. Standard ist
		 *  (0,0).
		 *  @exception InvalidArgumentExeption Die Kampftechnik ist unbekannt.
		 */
		void addKampftechnik(const CeGuiString& kampftechnikName, const pair<int,int>& value = make_pair(0,0));
		/** @brief liefert die AT und PA Werte in einer bestimmten Kampftechnik
		 *  zurueck.
		 *  @param kampfTechnikId Beszeichnet die Kampftechnik.
		 *  @return Ein pair<AT, PA>
		 *  @exception InvalidArgumentException kampftechnikId konnte nicht in 
		 *    mKampftechniken gefunden werden.
		 */
        virtual pair<int, int> getKampftechnik(const CeGuiString& kampftechnikName) const;
		/** @brief Setzt die AT und PA Werte in einer bestimmten Kampftechnik.
		 *  @param kampftechnikId Bestimmt die zu setzende Kampftechnik.
		 *  @param value Die neuen AT/PA Werte.
		 *  @exception InvalidArgumentException Die Kampftechnik kampftechnikId
		 *    konnte nicht in mKampftechniken gefunden werden.
		 */
        virtual void setKampftechnik(const CeGuiString& kampftechnikName, const pair<int, int>& value);

		/** @brief Fuegt das Talent talentName zu mTalente hinzu.
		 *  Das neue Talent wird mit TaW 0 initialisiert
		 *  @param talentName Bezeichnet das Talent
		 *  @exception InvalidArgumentException Das Talent konnte nicht
		 *    gefunden werden.
		 */
        void addTalent(const CeGuiString& talentName, int value = 0);
		/** @brief Liefert den Wert des Talents talentName zurueck.
		 *  @param talentName Bezeichnet das Talent.
		 *  @param value Startwert des Talents
		 *  @return TaW
		 *  @exception InvalidArgumentException Das Talent konnte in mTalente
		 *    nicht gefunden werden.
		 */
		virtual int getTalent(const CeGuiString& talentName) const;
		/** @brief Setzt den Wert des Talents talentName.
		 *  @param talentName Bezeichnet das zu veraendernde Talent.
		 *  @param value Der neue TaW.
		 *  @exception InvalidArgumentException Das Talent konnte in mTalente
		 *    nicht gefunden werden.
		 */
        virtual void setTalent(const CeGuiString& talentName, int value);
		/** @brief Erhoeht das Talent talentName um mod.
		 *  @param talentName Bezeichnet das zu steigernde Talent.
		 *  @param mod Der Wert um den das Talent gesteigert werden soll.
		 *  @exception InvalidArgumentException Das Talent konnte in mTalente
		 *    nicht gefunden werden.
		 */
        virtual void modifyTalent(const CeGuiString& talentName, int mod);
		virtual const Creature::TalentMap& getAllTalents() const;
		/** @brief Markiert ein Talent mit einer Speziellen Erfahrung (SE).
		 * Siehe Spezielle Erfahrungen, MFF 47
		 * @param talentName Bezeichnet das Talent in dem die SE erhalten wurde
		 */
		virtual void addSe(const CeGuiString& talentName);

		/** @brief Fuegt der Kreatur eine Sonderfertigkeit(SF) hinzu.
		 *  Der Wert wird auf 0 gesetzt (nicht gelernt, in Ausbildung).
		 *  @param sfId Bezeichnet die SF.
		 *  @exception InvalidArgumentException sfName kann nicht gefunden
		 *    werden.
		 */
		virtual void addSf(const CeGuiString& sfName, int value = SF_IN_TRAINING);
		/** @brief Liefert den Wert der Sonderfertigkeit(SF) zurueck.
		 *  @sa SonderfertigkeitMap
		 *  @param sfId Bezeichnet die SF
		 *  @exception InvalidArgumentException sfName kann nicht in 
		 *    mSonderfertigkeiten gefunden werden.
		 */
		virtual int getSf(const CeGuiString& sfName) const;
		/** @brief Setzt den Wert der SF.
		 *  @sa SonderfertigkeitMap
		 *  @param sfId Bezeichnet die Sonderfertigkeit deren Wert gesetzt
		 *    werden soll.
		 *  @param value Der Wert auf den die Sonderfertigkeit gesetzt werden 
		 *    soll.
		 *  @exception OutOfRangeException value ist kleiner als
		 *    SF_MIN_VALUE oder groesser als SF_MAX_VALUE.
		 *  @exception InvalidArgumentException sfName kann nicht in 
		 *    mSonderfertigkeiten gefunden werden.
		 */
		virtual void setSf(const CeGuiString& sfName, int value);

        virtual void modifyLe(int mod, bool ignoreMax = false);
        virtual int getLe();
        virtual int getLeMax();

		virtual void modifyAe(int mod,  bool ignoreMax = false);
        virtual int getAe();
        virtual int getAeMax();
		virtual bool isMagic();

		virtual void modifyAu(int mod,  bool ignoreMax = false);
        virtual int getAu();
        virtual int getAuMax();

		/**
		 *  @brief Liefert die derzeitge BE der Kreatur zurueck.
		 *    Eventuelle Ruestungsgewoehnung schon beruecksichtigt
		 **/
		virtual int getCurrentBe();
		virtual void setCurrentBe(int newBe);

		virtual int getCurrentInitiativeModifier();
		virtual void setCurrentInitiativeModifier(int newIniMod);

		void setWert(int wertId, int wert);

		/**
		 *  @brief Heftet Container an die Kreatur.
		 *  @param id ID des Containers
		 *  @param containerName Name des Containers (z.B. "Linke Hand")
		 *  @param desc Beschreibung des Containers
		 *  @exception InvalidArgumentException Nullpointer uebergeben.
		 *  @exception InvalidArgumentException Container bereits in
		 *  mContainer enthalten.
		 **/
		void addContainer(Container* container);
		/**
		 *  @brief Liefert den Container containerName zurueck.
		 *  @param containerName Bezeichnet den Container in mContainer.
		 *  @return Liefert einen Zeiger auf den Container.
		 *  @exception InvalidArgumentException Der Container wurde nicht
		 *  in mContainer gefunden.
		 **/
		Container* getContainer(const CeGuiString& containterName) const;
		/**
		 *  @brief Entfernt den Container containerName.
		 *   Der Container wird aus mContainer geloescht und ein Zeiger darauf
		 *   zurueckgegeben.
		 *  @param containerName Bezeichnet den Container in mContainer.
		 *  @exception InvalidArgumentException Container nicht in
		 *  mContainer gefunden.
		 **/
		Container* removeContainer(const CeGuiString& containerName);
		/**
		 *  @brief Fuegt eine Waffer der Liste der derzeit einsetzbaren Waffen
		 *   hinzu
		 *  @param Weapon* newWeapon Ein Zeiger auf die hinzuzufuegende Waffe.
		 *  @exception InvalidArgumentException newWeapon darf nicht NULL sein.
		 **/
		void addWeapon(Weapon* weapon);
		/**
		 *  @brief Liefert einen Zeiger auf eine Waffe aus der Liste der derzeit
		 *   verfuegbaren Waffen zurueck.
		 *  @param weaponId Bezeichnet zurueckzugebende die Waffe.
		 *  @return Ein Zeiger auf die Waffe weaponId.
		 *  @exception InvalidArgumentException weaponId kann nicht in mWeapons
		 *   gefunden werden.
		 **/
		Weapon* getWeapon(int weaponId) const;
		/** 
		 *  @brief Entfernt die Waffe weaponId aus der Liste der derzeit 
		 *   verfuegbaren Waffen.
		 *  @param weaponId Bezeichnet die zu entfernende Waffe.
		 *  @return Ein Zeiger auf die Entfernte Waffe.
		 *  @exception InvalidArgumentException weaponId kann nicht in mWeapons
		 *   gefunden werden.
		 **/
		Weapon* removeWeapon(int weaponId);

		void switchToWeapon(int weaponId);
		Weapon* getActiveWeapon();

        /** 
		*  @brief Durchfuehren einer Talentprobe.
		*   Intern ruft sie doAlternativeTalentprobe mit den
		*   Standardeigenschaften auf. Dies ist die uebliche Weise
		*   eine Probe zu wuerfeln.
		*  @see doAlternativeTalentprobe(CeGuiString* talentName, int spezialisierungId, int modifier, int eigenschaft1Id, int eigenschaft2Id, int eigenschaft3Id) 
		*   fuer Parameter und Rueckgabewerte
        */
        virtual int doTalentprobe(const CeGuiString& talentName, int modifier);

        /** @brief Durchfuehren einer Talentprobe.
		*  Intern ruft sie doAlternativeTalentprobe mit den
		*  Standardeigenschaften auf. Dies ist die ideale Weise
		*  eine Probe zu wuerfeln. Diese Version erlaubt auch
		*  die Angabe einer Spezialisierung
		*  @see doAlternativeTalentprobe(CeGuiString* talentName, int spezialisierungId, int modifier, int eigenschaft1Id, int eigenschaft2Id, int eigenschaft3Id) 
		*  fuer Parameter und Rueckgabewerte
		*/
        virtual int doTalentprobe(const CeGuiString& talentName, int spezialisierungId,
			int modifier);

        /** @brief Durchfuehren einer Talentprobe mit alternativen Eigenschaften.
		*  Siehe dazu auch MFF S.14. Wird nur der Korrektheit halber angeboten,
		*  sollte eher selten eingesetzt werden.
		*  @see doAlternativeTalentprobe(CeGuiString* talentName, int spezialisierungId, int modifier, int eigenschaft1Id, int eigenschaft2Id, int eigenschaft3Id) 
		*  fuer Parameter und Rueckgabewerte
        */
        virtual int doAlternativeTalentprobe(const CeGuiString& talentName, int modifier, 
			CeGuiString eigenschaft1Name, CeGuiString eigenschaft2Name, CeGuiString eigenschaft3Name);

        /** @brief Durchfuehren einer Talentprobe mit alternativen Eigenschaften.
		*  Siehe dazu auch MFF S.14. Wird nur der Korrektheit halber angeboten,
		*  sollte eher selten eingesetzt werden. Diese Talentprobe erlaubt auch
        *  die Angabe einer Spezialisierung.
		*
        *  @param talentName Bezeichnet das Talent
		*  @param spezialisierungId Bezeichnet die Spezialisierung
        *  @param modifier Modifikator der Probe, dabei
        *     ist ein positiver Wert eine Erschwernis,
        *     ein negativer Wert eine Erleichterung.
		*  @param eigenschaft1Id Bezeichnet die erste Eigenschaft auf die 
		*     gewuerfelt werden soll
		*  @param eigenschaft2Id Bezeichnet die zweite Eigenschaft
		*  @param eigenschaft3Id Bezeichnet die dritte Eigenschaft
        *  @return uebrig gebliebene Talentpunkte, ein
        *     negativer Wert bedeutet die Probe ist nicht bestanden.
        *  @retval   RESULT_AUTOERFOLG bedeutet 2*1 gewuerfelt.
        *  @retval   RESULT_SPEKT_AUTOERFOLG bedeutet 3*1 gewuerfelt.
        *  @retval   RESULT_AUTOMISSERFOLG bedeutet 2*20 gewuerfelt.
        *  @retval   RESULT_SPEKT_AUTOMISSERFOLG bedeutet 3*20 gewuerfelt.
        */
        virtual int doAlternativeTalentprobe(const CeGuiString& talentName, int spezialisierungId, 
			int modifier, CeGuiString eigenschaft1Name, CeGuiString eigenschaft2Name, 
			CeGuiString eigenschaft3Name);

        /** @brief Durchfuehren einer Eigenschaftsprobe.
        *
        *  @param eigenschaftId Bezeichnet die Eigenschaft
        *  @param modifier Modifikator der Probe, dabei
        *     ist ein positiver Wert eine Erschwernis,
        *     ein negativer Wert eine Erleichterung.
        *  @return uebrig gebliebene Eigenschaftspunkte, ein
        *     negativer Wert bedeutet die Probe ist nicht bestanden.
        *  @retval RESULT_GLUECKLICH bedeutet 1 gewuerfelt.
        *  @retval RESULT_PATZER bedeutet 20 gewuerfelt.
        */
        virtual int doEigenschaftsprobe(const CeGuiString& eigenschaftName, int modifier);

		/** 
		*  @brief Fuehrt eine Attacke aus.
		*   Die Funktion wird von einem Angriffsmanoever aufgerufen.
		*  @param kampftechnikName Bezeichnet die Kampftechnik in dem die 
		*   Attacke ausgefuehrt wird.
		*  @param mod Erschwert oder erleichtert die Attacke.
		*  @return Der Rueckgabewert kann folgende Werte annehmen:\n
		*   RESULT_ERFOLG\n
		*   RESULT_MISSERFOLG\n
		*   RESULT_GLUECKLICH\n
		*   RESULT_PATZER\n
		*  @exception InvalidArgumentException Kampftechnik nicht in 
		*   mKampftechniken gefunden.
		**/
		int doAttacke(const CeGuiString& kampftechnikName, int modifier);
		/** 
		*  @brief Fuehrt eine Parade aus.
		*   Die Funktion wird von einem Parademanoever aufgerufen.
		*  @param kampftechnikName Bezeichnet die Kampftechnik in dem die 
		*   Parade ausgefuehrt wird.
		*  @param mod Erschwert oder erleichtert die Parade.
		*  @param gluecklicheParade Versucht eine gute Parade (bei einem gluecklichen Treffer)
		*  @return Der Rueckgabewert kann folgende Werte annehmen:\n
		*   RESULT_ERFOLG\n
		*   RESULT_MISSERFOLG\n
		*   RESULT_GLUECKLICH\n
		*   RESULT_PATZER\n
		*  @exception InvalidArgumentException Kampftechnik nicht in 
		*   mKampftechniken gefunden.
		**/
		int doParade(const CeGuiString& kampftechnikName, int modifier, bool guteParade = false);

		/**
		 *  @brief Fuehrt einen Initiativewurf durch.
		 *  @param getMaxInitiative Wenn true wird ein Wert zurueckgeliefert,
		 *    als ob eine 6 gewuerfelt worden waere, z.B. fuer das Manoever
		 *    'Orientieren'.
		 **/
		int doInitiativeWurf(bool getMaxInitiative = false);

		/**
		 *  @brief Fuegt der Kreatur Schaden zu
		 *  @param sp die Schadenspunkte
		 *  @param weapon die Waffe, mit der der Schaden zugefuegt wurde (fuer
		 *    Schadenstypen), bei NULL sind die SP als TP zu werten
		 **/
		void applyDamage(int sp, Weapon* weapon);

		virtual int getAttackeBasis() const;
	    virtual int getParadeBasis() const;
		virtual int getFernkampfBasis() const;
		virtual int getInitiativeBasis() const;

	private:
        int mCurrentLe;
		int mCurrentAe;
		int mCurrentAu;		

		Weapon* mActiveWeapon;
        EigenschaftMap mEigenschaften;
        TalentMap mTalente;
        KampftechnikMap mKampftechniken;
		SonderfertigkeitMap mSonderfertigkeiten;
		WertMap mWerte;
		ContainerMap mContainer;
		WeaponMap mWeapons;
		int mCurrentBe;
		int mCurrentInitiativeModifier;
    };
}
#endif
