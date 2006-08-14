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

#ifndef __DSAMANAGER_H__
#define __DSAMANAGER_H__

#include <map>
#include <OgreSingleton.h>
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


        static DsaManager& getSingleton(void);
        static DsaManager* getSingletonPtr(void);

        /** Default Constructor */
        DsaManager();
        /** Default Deconstructor */
        ~DsaManager();

        /** Liefert die aktuelle Spielzeit */
        RL_LONGLONG getTimestamp();
		Date getCurrentDate();

		/** Setzt die aktuelle Spielzeit, die Spieluhr in Core wird dabei zurückgesetzt */
		void setTimestamp(const RL_LONGLONG time);
		void setCurrentDate(const Date& date);


        /** liefert eine Zufallszahl zwischen 1 und 20.*/
        int rollD20();

        /** liefert ein Tripel von Zufallszahlen zwischen 1 und 20.*/
        Tripel<int> roll3D20();

        /** liefert eine Zufallszahl zwischen 1 und 6.*/
        int rollD6();

		int roll(int d6, int d20);

        Talent* getTalent(const CeGuiString talentName) const;
		//Talent* getTalent(const CeGuiString name) const;
        Kampftechnik* getKampftechnik(const CeGuiString kampftechnikName) const;
        Eigenschaft* getEigenschaft(const CeGuiString eigenschaftName) const;
		Person* getPerson(int id) const;        
		
		//int getEigenschaftIdFromString(const CeGuiString str) const;
		//int getEigenschaftIdFromLongString(const CeGuiString str) const;
		
		int getSteigerKosten(int column, int from, int to) const;
		int getSteigerKosten(int column, int from) const;
		bool isDkDistance(const Weapon::Distanzklasse& dk, const Ogre::Real& distance) const;
		bool isRuleActive(Rule rule) const;

		void loadDsaDataFile(const std::string& filename);	

		void _addTalent(Talent* talent);
		void _addPerson(Person* person);
		void _addKampftechnik(Kampftechnik* kampftechnik);

    private:
		RL_LONGLONG mBaseTime;

        typedef std::map<CeGuiString, Talent*> TalentMap;
		typedef std::map<int, Person*> PersonMap;
        typedef std::map<CeGuiString, Kampftechnik*> KampftechnikMap;
		typedef std::map<CeGuiString, Eigenschaft*> EigenschaftMap;
		EigenschaftMap mEigenschaften;
        TalentMap mTalente;
        KampftechnikMap mKampftechniken;
		PersonMap mPersonen;
		unsigned int mSteigerkostenTabelle[SKT_COLUMNS][SKT_ROWS];

		void initializeEigenschaften();
        void initializeTalente();
        void initializeKampftechniken();
        void initializeSkt();
    };
}
#endif
