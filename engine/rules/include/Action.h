#ifndef __RL_ACTION_H__
#define __RL_ACTION_H__

#include "RulesPrerequisites.h"

namespace rl
{
    class _RlRulesExport Creature;

    static const int TC_NO_TARGET = 0;
    static const int TC_GAMEOBJECT = 1;
    static const int TC_CREATURE = 2;
    static const int TC_ITEM = 3;

    /**
    * \brief Abstrakte Basisklasse für Aktionen an Spielobjekten.
    * Spielobjekte (GameObject) besitzen einen Satz von Aktionen, die man auf
    * ihnen anwenden kann. Diese werden von dieser Klasse gekapselt.
    * Konkrete Aktionen erben von dieser Klasse und muessen
    * doAction() ueberschreiben. Diese Klasse wird in Ruby ueberschrieben.
    */
    class _RlRulesExport Action
    {
    private:
        std::string mName;
        std::string mDescription;
    public:
        /**
        * @param name Name, mit der die Aktion dem Benutzer
        *             gegenüber dargestellt wird.
        *             Die gleiche, die intern verwendet wird.
        * @param descritpion Eine naehere Beschreibung.
        */
        Action(const std::string& name, const std::string& description);
        virtual ~Action();

        virtual std::string getName();
        virtual std::string getDescription();

        /**
         * @return Die Art des auszuwaehlenden Zieles für die Aktion.
         * @todo rval sollte eigentlich ein enum sein.
         *       SWIG mag das so aber lieber.
         */
        virtual int getTargetClass();

        /**
         * Die Aktion wird ausgeführt. Diese Methode wird in Ruby
         * überschrieben. Ableitende Klassen sollten dabei nicht neue
         * öffentliche Methoden einführen.
         * @param object Das benutzte Object, für diese Aktion.
         *        Das kann auch ein Zauberspruch, etc sein.
         * @param actor Der Benutzer des Objektes.
         * @param target Das (erste) Ziele der Aktion.
         *
         */
        virtual void doAction(GameObject* object,
                              Creature* actor,
                              GameObject* target);
    };
}

#endif
