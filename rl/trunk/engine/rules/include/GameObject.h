#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "RulesPrerequisites.h"

namespace rl
{
    class _RlRulesExport Creature;

	static const int ACT_NORMAL = 1<<0;
	static const int ACT_INVIS_DESC = 1<<1;
	static const int ACT_DISABLED = 1<<2;
	static const int ACT_NEEDS_TALENT = 1<<8;
	
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
    public:
        GameObject(int id,
                   const CeGuiString& name,
                   const CeGuiString& description);
        virtual ~GameObject(void);

        virtual int getId() const;

        //virtual CeGuiString getNameString();
        virtual const CeGuiString& getName() const;
        virtual void setName(CeGuiString& name);
		//virtual void setName(const char* name);

        virtual const CeGuiString& getDescription() const;
        virtual void setDescription(CeGuiString& description);

        void addAction(Action* action, int option = ACT_NORMAL);
        void removeAction(Action* action);

        const ActionVector getValidActions() const;

        /** Eine Aktion des Gegenstandes wird ausgeloest.
         *  @param action der Name der auszuloesenden Aktion.
         *  @param actor der "Benutzer" des Gegenstandes,
         *         der die Aktion ausgeloest hat, kann auch
         *         <code>NULL</code> sein, falls die Aktion auf andere Weise
         *         getriggert wurde.
         *  @param target Ziel, auf das die Aktion gewirkt wird.
         */
        void doAction(const CeGuiString& action,
                      Creature* actor,
                      GameObject* target);

		void doAction(const Action* action,
                      Creature* actor,
                      GameObject* target);

		typedef std::vector<std::pair<Action*, int> > ActionOptionVector;

	private:
        ActionOptionVector mActions;
		ActionOptionVector::iterator findAction(ActionOptionVector::iterator begin, ActionOptionVector::iterator end, const CeGuiString& actionName);
		ActionOptionVector::iterator findAction(ActionOptionVector::iterator begin, ActionOptionVector::iterator end, const Action* action);
	
    protected:
        int mId;
        CeGuiString mName;
        CeGuiString mDescription;
    };
}

#endif
