/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
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

namespace rl {
	
				
	 template<typename T>
   class Tripel {
   public:
      T first;
      T second;
      T third;

      Tripel();      
      Tripel(const T& t1, const T& t2, const T& t3);
      Tripel(const Tripel& rhs);

      //Tripel& operator=(const Tripel& rhs);
      bool operator==(const Tripel& rhs);
      bool operator<(const Tripel& rhs);     
   };
	
		class Date {
    public:
        static const RL_LONGLONG ONE_SECOND = 1000ULL;
        static const RL_LONGLONG ONE_MINUTE = 60000ULL;
        static const RL_LONGLONG ONE_HOUR = 3600000ULL;
        static const RL_LONGLONG ONE_DAY = 86400000ULL;
        static const RL_LONGLONG ONE_WEEK = 604800000ULL;
        static const RL_LONGLONG ONE_MONTH = 2592000000ULL;
        static const RL_LONGLONG ONE_YEAR = 31536000000ULL;
        static const RL_LONGLONG ONE_KAMPFRUNDE = 2 * ONE_SECOND;
        static const RL_LONGLONG ONE_SPIELRUNDE = 5 * ONE_MINUTE;

        Date();
        Date(RL_LONGLONG timestamp);
        Date(const Date& rhs);
      
        bool operator==(const Date& rhs);
        bool operator<(const Date& rhs);
        bool operator<=(const Date& rhs);
        bool operator>=(const Date& rhs);
        bool operator>(const Date& rhs);

        Date operator+(const RL_LONGLONG& rhs);
        Date operator-(const RL_LONGLONG& rhs);
        RL_LONGLONG operator-(const Date& rhs);
      
        RL_LONGLONG getTimestamp();

        int getTimeFraction();
        int getYear();
        int getMonth();
        CeGuiString getMonthName();
        int getDayOfYear();
        int getDayOfMonth();
        int getDayOfWeek();
        CeGuiString getDayOfWeekName();
        int getHour();
        int getMinute();
        int getSecond();
        int getMilliSecond();
      
        CeGuiString toString();
    };
   
    Date operator+(const RL_LONGLONG&, const Date&);
    Date operator-(const RL_LONGLONG&, const Date&);

  
    class DsaManager {
    public:
        RL_LONGLONG getTimestamp();
   
        int rollD20();
        Tripel<int> roll3D20();
        int rollD6();
      
	    static DsaManager& getSingleton(void);
	    rl::Talent* getTalent(const CeGuiString& name) const;
	    rl::Person* getPerson(const int id) const;
	    
    };
    
    class Talent
    {
    public:
        Talent(int id,
               const CeGuiString& name,
               const CeGuiString& description,
               const Tripel<int>& eigenschaften,
               int ebe,
			   int gruppe);

        bool operator==(const Talent& rhs) const;
        bool operator<(const Talent& rhs) const;
        int getId() const;
        CeGuiString getName() const;
        CeGuiString getDescription() const;
		int getEbe() const;

        /// Berechnet effektive Behinderung bei gegebener Behinderung;
        int calculateEbe(int be) const;
        Tripel<int> getEigenschaften() const;
    };

    class ActionGroup
	{
	public:
		ActionGroup(CeGuiString name, ActionGroup* parent = NULL);
		~ActionGroup();
	
		const CeGuiString& getName() const;
	};
	
	static const int ACT_NORMAL = 1<<0;
	static const int ACT_INVIS_DESC = 1<<1;
	static const int ACT_DISABLED = 1<<2;
	static const int ACT_NEEDS_TALENT = 1<<8;
	%feature("director") Action;   
    class Action
    {
    public:
        Action(const CeGuiString& name, const CeGuiString& description);
        virtual ~Action();

        virtual const CeGuiString& getName() const;
        virtual const CeGuiString& getClassName() const;
        virtual const CeGuiString& getDescription() const;
        virtual int getTargetClass() const;
        virtual void doAction(rl::GameObject *object, rl::Creature *actor,
            rl::GameObject *target);
            
        void setGroup(ActionGroup* group);
		ActionGroup* getGroup() const;
    };
    
    %feature("director") GameObject;
    %apply SWIGTYPE *DYNAMIC { GameObject * };
    class GameObject
    {
    public:
        GameObject(int id, const CeGuiString& name, const CeGuiString& description);
        virtual ~GameObject(void);

        virtual int getId();

        virtual const CeGuiString& getName() const;
        virtual void setName(CeGuiString& name);

        virtual const CeGuiString& getDescription() const;
        virtual void setDescription(CeGuiString& description);   
        
        void setActor(rl::GameActor* actor);
		rl::GameActor* getActor();

        void addAction(rl::Action *action, int option = ACT_NORMAL);
        //%extend {
        //    void rbAddAction(VALUE action) {
        //        void* ptr = reinterpret_cast<void*>(action);
        //        self->addAction(reinterpret_cast<Action*>(ptr));
        //    }
        //}
        void addActionInGroup(Action* action, ActionGroup* group, int option = ACT_NORMAL);
        void removeAction(rl::Action *action);

        const std::vector<Action*> getValidActions() const;        
        
        void doAction(
			const CeGuiString& actionName, 
			const CeGuiString& className, 
			rl::Creature* actor,
            rl::GameObject* target);
        void doAction(Action* action, rl::Creature* actor,
            rl::GameObject* target);
    };
    
    %feature("director") Item;
    class Item : public GameObject {
    public:
        Item(int id, const CeGuiString& name, const CeGuiString& description);
        virtual ~Item(void);
    };
    
    %feature("director") Creature;
    class Creature : public GameObject {
    public:
        Creature(int id, const CeGuiString& name, const CeGuiString& description);
        //Creature(const CeGuiString& data);
        virtual ~Creature();

        virtual int getEigenschaft(int id) const;
        virtual void setEigenschaft(int id, int value);
        virtual int getTalent(int id) const;
        virtual void setTalent(int id, int value);

        virtual void modifyEigenschaft(int id, int mod);
        virtual void modifyTalent(int id, int mod);

        virtual void modifyLe(int mod);
        virtual int getLe();

        virtual int doTalentprobe(int id, int modifier);
        virtual int doEigenschaftsprobe(int id, int modifier);
   };
   
   %feature("director") Person;
   class Person : public Creature
   {
   public:
	    Person(int id,
                 const CeGuiString& name,
                 const CeGuiString& description);
	};
} // Namespace

%{
static swig_type_info *GameObject_dynamic_cast(void **ptr)
{
    rl::GameObject **ppGameObject = reinterpret_cast<rl::GameObject**>(ptr);

    rl::Person *pPerson=dynamic_cast<rl::Person*>(*ppGameObject);
    if (pPerson) {
        *ptr=reinterpret_cast<void*>(pPerson);
        return SWIGTYPE_p_rl__Person;
    }

    rl::Creature *pCreature=dynamic_cast<rl::Creature*>(*ppGameObject);
    if (pCreature) {
        *ptr=reinterpret_cast<void*>(pCreature);
        return SWIGTYPE_p_rl__Creature;
    }
    rl::Item *pItem=dynamic_cast<rl::Item*>(*ppGameObject);
    if (pItem) {
        *ptr=reinterpret_cast<void*>(pItem);
        return SWIGTYPE_p_rl__Item;
    }
    return 0;
}
%}
DYNAMIC_CAST(SWIGTYPE_p_rl__GameObject, GameObject_dynamic_cast);

