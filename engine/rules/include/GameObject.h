#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "RulesPrerequisites.h"

namespace rl
{
    class _RlRulesExport Creature;

    /**
    * \brief Basisklasse aller spielrelevanten Objekte in RL.
    * Stellt im Wesentlichen Methoden zur Identifikation von
    * Objekten innerhalb der Spielwelt bereit.
    * Hierbei handelt es sich um konkrete Objekte.
    * Abstrakte Konzepte, wie Date und Action erben nicht von dieser Klasse.
    *
    * @todo Ueberlegen, wie man Aktionen situativ aktivierbar macht.
    */
    class _RlRulesExport GameObject
    {
    private:
        ActionMap mActionMap;
    protected:
        int mId;
        std::string mName;
        std::string mDescription;
    public:
        GameObject(int id,
                   const std::string& name,
                   const std::string& description);
        virtual ~GameObject(void);

        virtual int getId() const;

        virtual std::string getName() const;
        virtual void setName(const std::string& name);

        virtual std::string getDescription() const;
        virtual void setDescription(const std::string& description);

        void addAction(Action* action);
        void removeAction(const std::string& name);

        StringVector getValidActions() const;

        /** Eine Aktion des Gegenstandes wird ausgeloest.
         *  @param action der Name der auszuloesenden Aktion.
         *  @param actor der "Benutzer" des Gegenstandes,
         *         der die Aktion ausgeloest hat, kann auch
         *         <code>NULL</code> sein, falls die Aktion auf andere Weise
         *         getriggert wurde.
         *  @param target Ziel, auf das die Aktion gewirkt wird.
         */
        void doAction(const std::string& action,
                      Creature* actor,
                      GameObject* target);
    };
}

#endif
