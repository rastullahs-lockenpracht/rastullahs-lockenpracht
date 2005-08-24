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

	const int SF_MIN_VALUE = 0;
	const int SF_MAX_VALUE = 2;

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
        virtual int getWert(int id) const;
		virtual int getAttackeBasis() const;
	    virtual int getParadeBasis() const;
		virtual int getFernkampfBasis() const;
		virtual int getInitiativeBasis() const;
		virtual int getMrBasis() const;
        virtual int getLeBasis() const;
		virtual int getAuBasis() const;
		virtual int getAeBasis() const;

    public:
		typedef map<int, int> WertMap;
		typedef map<int, int> TalentMap;
		/**
		 *  @brief Liste der Kampftechniken und ihrer AT/PA Werte.\n
		 *  Eine Kampftechnik in diesem Sinne ist so was wie Hiebwaffen
		 *  oder Raufen, bei Tieren sowas wie Biss oder Prankenhieb.\n
		 *  Der erste Wert entspricht der ID der Kampftechnik, das pair
		 *  den AT und PA Werten.
		 **/
        typedef map<int, pair<int, int> > KampftechnikMap;
		/** @brief Die Sonderfertigkeiten der Kreatur
		 *  Der erste Wert steht fuer die sfId, der zweite fuer den Status \n
		 *  0 - In Ausbildung \n
		 *	1 - gelernt \n
		 *	2 - gelernt, aber Voraussetzungen nicht erfuellt (wird das
		 *	    ueberhaupt gebraucht?) \n
		 */
		typedef map<int,int> SonderfertigkeitMap;

		///@warning Nur zu Testzwecken da. Wird spaeter entfernt.
        Creature(int id,
                 const CeGuiString& name,
                 const CeGuiString& description);
		        
        virtual ~Creature();

        virtual int getEigenschaft(int eigenschaftId) const;
        virtual void setEigenschaft(int eigenschaftId, int value);
        virtual void modifyEigenschaft(int eigenschaftId, int mod);

        virtual pair<int, int> getKampftechnik(int kampftechnikId) const;
        virtual void setKampftechnik(int kampftechnikId, const pair<int, int>& value);

        virtual void addTalent(int talentId);
		virtual int getTalent(int talentId) const;
        virtual void setTalent(int talentId, int value);
        virtual void modifyTalent(int talentId, int mod);
		virtual const Creature::TalentMap& getAllTalents() const;
		/** @brief Markiert ein Talent mit einer Speziellen Erfahrung (SE).
		 * Siehe Spezielle Erfahrungen, MFF 47
		 * @param talentId Bezeichnet das Talent un dem die SE erhalten wurde.
		 */
		virtual void addSe(int talentId);

		virtual void addSf(int sfId);
		virtual int getSf(int sfId) const;
		/** @brief Setzt den Wert der SF.
		 *  @sa KampftechnikMap
		 *  @param sfId Bezeichnet die Sonderfertigkeit deren Wert gesetzt
		 *    werden soll.
		 *  @param value Der Wert auf den die Sonderfertigkeit gesetzt werden 
		 *    soll.
		 *  @exception OutOfRangeException value ist kleiner als
		 *    SF_MIN_VALUE oder groesser als SF_MAX_VALUE.
		 *  @exception InvalidArgumentException sfId kann nicht in 
		 *    mSonderfertigkeiten gefunden werden.
		 */
		virtual void setSf(int sfId, int value);

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

		void setWert(int wertId, int wert);

        /** @brief Durchfuehren einer Talentprobe.
		*  Intern ruft sie doAlternativeTalentprobe mit den
		*  Standardeigenschaften auf. Dies ist die uebliche Weise
		*  eine Probe zu wuerfeln.
        *
        *  @param talentId Bezeichnet das Talent
        *  @param modifier Modifikator der Probe, dabei
        *     ist ein positiver Wert eine Erschwernis,
        *     ein negativer Wert eine Erleichterung.
        *  @return uebrig gebliebene Talentpunkte, ein
        *     negativer Wert bedeutet die Probe ist nicht bestanden.\n
        *  @retval   RESULT_AUTOERFOLG bedeutet 2*1 gewuerfelt.\n
        *  @retval   RESULT_SPEKT_AUTOERFOLG bedeutet 3*1 gewuerfelt.\n
        *  @retval   RESULT_AUTOMISSERFOLG bedeutet 2*20 gewuerfelt.\n
        *  @retval   RESULT_SPEKT_AUTOMISSERFOLG bedeutet 3*20 gewuerfelt.\n
        */
        virtual int doTalentprobe(int talentId, int modifier);

        /** @brief Durchfuehren einer Talentprobe.
		*  Intern ruft sie doAlternativeTalentprobe mit den
		*  Standardeigenschaften auf. Dies ist die ideale Weise
		*  eine Probe zu wuerfeln. Diese Version erlaubt auch
		*  die Angabe einer Spezialisierung
        *
        *  @param talentId Bezeichnet das Talent
        *  @param modifier Modifikator der Probe, dabei
        *     ist ein positiver Wert eine Erschwernis,
        *     ein negativer Wert eine Erleichterung.
        *  @return uebrig gebliebene Talentpunkte, ein
        *     negativer Wert bedeutet die Probe ist nicht bestanden.\n
        *  @retval   RESULT_AUTOERFOLG bedeutet 2*1 gewuerfelt.\n
        *  @retval   RESULT_SPEKT_AUTOERFOLG bedeutet 3*1 gewuerfelt.\n
        *  @retval   RESULT_AUTOMISSERFOLG bedeutet 2*20 gewuerfelt.\n
        *  @retval   RESULT_SPEKT_AUTOMISSERFOLG bedeutet 3*20 gewuerfelt.\n
        */
        virtual int doTalentprobe(int talentId, int spezialisierungId,
			int modifier);

        /** @brief Durchfuehren einer Talentprobe mit alternativen Eigenschaften.
		*  Siehe dazu auch MFF S.14. Wird nur der Korrektheit halber angeboten,
		*  sollte eher selten eingesetzt werden.
        *
        *  @param talentId Bezeichnet das Talent
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
        virtual int doAlternativeTalentprobe(int talentId, int modifier, 
			int eigenschaft1Id, int eigenschaft2Id, int eigenschaft3Id);

        /** @brief Durchfuehren einer Talentprobe mit alternativen Eigenschaften.
		*  Siehe dazu auch MFF S.14. Wird nur der Korrektheit halber angeboten,
		*  sollte eher selten eingesetzt werden. Diese Talentprobe erlaubt auch
        *  die Angabe einer Spezialisierung.
		*
        *  @param talentId Bezeichnet das Talent
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
        virtual int doAlternativeTalentprobe(int talentId, int spezialisierungId, 
			int modifier, int eigenschaft1Id, int eigenschaft2Id, 
			int eigenschaft3Id);

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
        virtual int doEigenschaftsprobe(int eigenschaftId, int modifier);

	private:
        int mCurrentLe;
		int mCurrentAe;
		int mCurrentAu;		

        int mEigenschaften[EIGENSCHAFT_COUNT];
        TalentMap mTalente;
        KampftechnikMap mKampftechniken;
		SonderfertigkeitMap mSonderfertigkeiten;
		WertMap mWerte;
    };
}
#endif
