/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
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

    /**
    * @brief Basisklasse aller spielrelevanten Objekte in RL.
    *
    * Klasse kapselt alle Wesen, die aktiv im Spiel agieren, sei es
    * computer- oder spielergesteuert. Klasse enthält Methoden und
    * Daten für die DSA-Spielwerte (Lebensenergie, Eigenschaften)
    * und Methoden für alles was für die Interaktion mit der Spielwelt
    * noetig ist(z.B. Proben).
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
        typedef map<int, pair<int, int> > KampftechnikMap;

		///@warning Nur zu Testzwecken da. Wird spaeter entfernt.
        Creature(int id,
                 const CeGuiString& name,
                 const CeGuiString& description);
		        
        virtual ~Creature();

        virtual int getEigenschaft(int id) const;
        virtual void setEigenschaft(int id, int value);
        virtual void modifyEigenschaft(int id, int mod);

        virtual pair<int, int> getKampftechnik(int id) const;
        virtual void setKampftechnik(int id, const pair<int, int>& value);

        virtual void addTalent(int id);
		virtual int getTalent(int id) const;
        virtual void setTalent(int id, int value);
        virtual void modifyTalent(int id, int mod);
		virtual const Creature::TalentMap& getAllTalents() const;

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

		void setWert(int id, int wert);

        /** @brief Durchfuehren einer Talentprobe.
        *
        *  @param id Talent-Id
        *  @param modifier Modifikator der Probe, dabei
        *     ist ein positiver Wert eine Erschwernis,
        *     ein negativer Wert eine Erleichterung.
        *  @return uebrig gebliebene Talentpunkte, ein
        *     negativer Wert bedeutet die Probe ist nicht bestanden.
        *     100 bedeutet 2*1 gewuerfelt.
        *     1000 bedeutet 3*1 gewuerfelt.
        *     -100 bedeutet 2*20 gewuerfelt.
        *     -1000 bedeutet 3*20 gewuerfelt.
        */
        virtual int doTalentprobe(int id, int modifier);

        /** @brief Durchfuehren einer Eigenschaftsprobe.
        *
        *  @param id Eigenschaft-Id
        *  @param modifier Modifikator der Probe, dabei
        *     ist ein positiver Wert eine Erschwernis,
        *     ein negativer Wert eine Erleichterung.
        *  @return uebrig gebliebene Eigenschaftspunkte, ein
        *     negativer Wert bedeutet die Probe ist nicht bestanden.
        *     100 bedeutet 1 gewuerfelt.
        *     -100 bedeutet 20 gewuerfelt.
        */
        virtual int doEigenschaftsprobe(int id, int modifier);

	private:
        int mCurrentLe;
		int mCurrentAe;
		int mCurrentAu;		

        int mEigenschaften[EIGENSCHAFT_COUNT];
        TalentMap mTalente;
        KampftechnikMap mKampftechniken;
		WertMap mWerte;
    };
}
#endif
