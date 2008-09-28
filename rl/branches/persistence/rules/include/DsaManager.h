/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __DSAMANAGER_H__
#define __DSAMANAGER_H__

#include <map>
#include "RulesPrerequisites.h"

#include "Eigenschaft.h"
#include "Tripel.h"
#include "Weapon.h"

namespace rl
{
    class Talent;
    class Kampftechnik;
	class Person;
	class Date;

	static const int SKT_ROWS = 20;
	static const int SKT_COLUMNS = 8;

	static const int EBE_KEINE_BE = -100;
	static const int EBE_BEx2 = -99;
    /** 
    *  Klasse kapselt alles was mit den DSA-Grundregeln zu tun hat.
    *  Namentlich sind das Eigenschaften, Talente, Kampftechniken und
    *  Probenwuerfe.
    */
	class _RlRulesExport DsaManager : public Ogre::Singleton<DsaManager>
    {
    public:
		
		enum Rule
		{
			RULE_DISTANZKLASSEN
		};

        /** Default Constructor */
        DsaManager();
        /** Default Deconstructor */
        ~DsaManager();

        /** Liefert die aktuelle Spielzeit */
        RL_LONGLONG getTimestamp() const;
		Date getCurrentDate() const;

		/// Setzt die aktuelle Spielzeit
		void setTimestamp(const RL_LONGLONG time);
		void setCurrentDate(const Date& date);

        /** liefert eine Zufallszahl zwischen 1 und 20.*/
        int rollD20() const;

        /** liefert ein Tripel von Zufallszahlen zwischen 1 und 20.*/
        Tripel<int> roll3D20() const;

        /** liefert eine Zufallszahl zwischen 1 und 6.*/
        int rollD6() const;

		int roll(int d6, int d20) const;

        Talent* getTalent(const CeGuiString talentName) const;
        Kampftechnik* getKampftechnik(const CeGuiString kampftechnikName) const;
        Eigenschaft* getEigenschaft(const CeGuiString eigenschaftName) const;
		Creature* getCreature(const CeGuiString &id) const;        
				
		int getSteigerKosten(int column, int from, int to) const;
		int getSteigerKosten(int column, int from) const;
		bool isDkDistance(const Weapon::Distanzklasse& dk, const Ogre::Real& distance) const;
		bool isRuleActive(Rule rule) const;

		void _addTalent(Talent* talent);
		void _addCreature(Creature* creature);
		void _addKampftechnik(Kampftechnik* kampftechnik);

    private:
		mutable RL_LONGLONG mLastGameTime;
        mutable unsigned long mLastClock;
        Ogre::Real mTimeScale;

        typedef std::map<CeGuiString, Talent*> TalentMap;
		typedef std::map<CeGuiString, Creature*> CreatureMap;
        typedef std::map<CeGuiString, Kampftechnik*> KampftechnikMap;
		typedef std::map<CeGuiString, Eigenschaft*> EigenschaftMap;
		EigenschaftMap mEigenschaften;
        TalentMap mTalente;
        KampftechnikMap mKampftechniken;
		CreatureMap mCreatures;
		unsigned int mSteigerkostenTabelle[SKT_COLUMNS][SKT_ROWS];

		void initializeEigenschaften();
        void initializeTalente();
        void initializeKampftechniken();
        void initializeSkt();
    };
}
#endif
