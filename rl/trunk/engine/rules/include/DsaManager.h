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

#ifndef __DSAMANAGER_H__
#define __DSAMANAGER_H__

#include <OgreSingleton.h>
#include "RulesPrerequisites.h"

#include "Eigenschaft.h"
#include "Tripel.h"

#include <map>

using Ogre::Singleton;

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
    *  Probenwrfe.
    */
	class _RlRulesExport DsaManager : public Singleton<DsaManager>
    {
    public:
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

        Talent* getTalent(int id) const;
		Talent* getTalent(const CeGuiString& name) const;

        Kampftechnik* getKampftechnik(int id) const;

        Eigenschaft* getEigenschaft(int id) const;

		Person* getPerson(int id) const;
        
		
		int getEigenschaftIdFromString(const CeGuiString& str) const;
		int getEigenschaftIdFromLongString(const CeGuiString& str) const;
		
		int getSteigerKosten(int column, int from, int to) const;
		int getSteigerKosten(int column, int from) const;

		void _addTalent(Talent* talent);
		void _addPerson(Person* person);

    private:
		RL_LONGLONG mBaseTime;

        typedef std::map<int, Talent*> TalentMap;
		typedef std::map<int, Person*> PersonMap;
        typedef std::map<int, Kampftechnik*> KampftechnikMap;
        Eigenschaft* mEigenschaften[EIGENSCHAFT_COUNT];
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
