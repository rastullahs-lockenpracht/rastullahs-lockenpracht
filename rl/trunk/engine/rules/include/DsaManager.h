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

        /** liefert eine Zufallszahl zwischen 1 und 20.*/
        int rollD20();

        /** liefert ein Tripel von Zufallszahlen zwischen 1 und 20.*/
        Tripel<int> roll3D20();

        /** liefert eine Zufallszahl zwischen 1 und 6.*/
        int rollD6();

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
