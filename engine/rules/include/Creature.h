#ifndef __CREATURE_H__
#define __CREATURE_H__

#include "RulesPrerequisites.h"
#include "GameObject.h"
#include "Eigenschaft.h"

using namespace std;

namespace rl
{
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
    private:
        typedef map<int, int> TalentMap;
        typedef map<int, pair<int, int> > KampftechnikMap;

        int mCurrentLe;

        int mEigenschaften[EIGENSCHAFT_COUNT];
        TalentMap mTalente;
        KampftechnikMap mKampftechniken;

    protected:
        virtual int getAttackeBasis();
        virtual int getParadeBasis();
        virtual int getFernkampfBasis();
        virtual int getInitiativeBasis();
        virtual int getMrBasis();
        virtual int getLeBasis();
        virtual int getAuBasis();

    public:
        ///@warning Nur zu Testzwecken da. Wird spaeter entfernt.
        Creature(int id,
                 const std::string& name,
                 const std::string& description);
        virtual ~Creature();

        virtual int getEigenschaft(int id) const;
        virtual void setEigenschaft(int id, int value);
        virtual void modifyEigenschaft(int id, int mod);

        virtual pair<int, int> getKampftechnik(int id) const;
        virtual void setKampftechnik(int id, const pair<int, int>& value);

        virtual int getTalent(int id) const;
        virtual void setTalent(int id, int value);
        virtual void modifyTalent(int id, int mod);

        virtual void modifyLe(int mod);
        virtual int getLe();

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
    };
}
#endif
