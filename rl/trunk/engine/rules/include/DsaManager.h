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

        Kampftechnik* getKampftechnik(int id) const;

        Eigenschaft* getEigenschaft(int id) const;

        int getEigenschaftIdFromString(const std::string& str) const;
		
		int getSteigerKosten(int column, int from, int to) const;
		int getSteigerKosten(int column, int from) const;

		void _addTalent(Talent* talent);

    private:
        typedef std::map<int, Talent*> TalentMap;
        typedef std::map<int, Kampftechnik*> KampftechnikMap;
        Eigenschaft* mEigenschaften[EIGENSCHAFT_COUNT];
        TalentMap mTalente;
        KampftechnikMap mKampftechniken;
		unsigned int mSteigerkostenTabelle[SKT_COLUMNS][SKT_ROWS];

		void initializeEigenschaften();
        void initializeTalente();
        void initializeKampftechniken();
        void initializeSkt();
    };
}
#endif
