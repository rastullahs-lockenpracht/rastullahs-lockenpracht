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
        rl::CeGuiString getMonthName();
        int getDayOfYear();
        int getDayOfMonth();
        int getDayOfWeek();
        rl::CeGuiString getDayOfWeekName();
        int getHour();
        int getMinute();
        int getSecond();
        int getMilliSecond();
      
        rl::CeGuiString toString();
    };
   
    Date operator+(const RL_LONGLONG&, const Date&);
    Date operator-(const RL_LONGLONG&, const Date&);

  
    class DsaManager {
    public:
        RL_LONGLONG getTimestamp();
        Date getCurrentDate();
		void setTimestamp(const RL_LONGLONG time);
		void setCurrentDate(const Date& date);
		
        int rollD20();
        Tripel<int> roll3D20();
        int rollD6();
        int roll(int d6, int d20);
      
	    static DsaManager& getSingleton(void);
	    rl::Talent* getTalent(const rl::CeGuiString& name) const;
	    rl::Person* getPerson(const int id) const;

        void loadDsaDataFile(const std::string& filename);	
    };
    
    class RulesSubsystem 
    {
    public:
		static rl::RulesSubsystem& getSingleton();
    };

    
    class Talent
    {
    public:
        Talent(int id,
               const rl::CeGuiString& name,
               const rl::CeGuiString& description,
               const Tripel<int>& eigenschaften,
               int ebe,
			   int gruppe);

        bool operator==(const Talent& rhs) const;
        bool operator<(const Talent& rhs) const;
        int getId() const;
        rl::CeGuiString getName() const;
        rl::CeGuiString getDescription() const;
		int getEbe() const;

        /// Berechnet effektive Behinderung bei gegebener Behinderung;
        int calculateEbe(int be) const;
        Tripel<int> getEigenschaften() const;
    };

    class ActionGroup
	{
	public:
		ActionGroup(rl::CeGuiString name, ActionGroup* parent = NULL);
		~ActionGroup();
	
		const rl::CeGuiString& getName() const;
	};
	
	// Vor Action stehen!
	%apply SWIGTYPE *DYNAMIC { rl::GameObject * };
	
	static const int ACT_NORMAL = 1<<0;
	static const int ACT_INVIS_DESC = 1<<1;
	static const int ACT_DISABLED = 1<<2;
	static const int ACT_NEEDS_TALENT = 1<<8;
	
	%feature("director") Action;   
    class Action
    {
    public:
        Action(const rl::CeGuiString& name, const rl::CeGuiString& description);
        virtual ~Action();

        virtual const rl::CeGuiString& getName() const;
        virtual const rl::CeGuiString& getClassName() const;
        virtual const rl::CeGuiString& getDescription() const;
        virtual int getTargetClass() const;
        virtual void doAction(rl::GameObject *object, rl::Creature *actor,
            rl::GameObject *target);
            
        void setGroup(ActionGroup* group);
		ActionGroup* getGroup() const;
    };
    
	 %feature("director") ObjectStateChangeListener; 	 
	 class ObjectStateChangeListener 	 
	 { 	 
		public: 	 
				virtual ~ObjectStateChangeListener(); 	 
		  	 
				virtual void objectStateChanged(rl::ObjectStateChangeEvent *anEvent) = 0; 	 
		}; 	 
		  	 
		class ObjectStateChangeEventSource 	 
		{ 	 
		public: 	 
				ObjectStateChangeEventSource( ); 	 
		  	 
			void addObjectStateChangeListener( rl::ObjectStateChangeListener*  list ); 	 
			void removeObjectStateChangeListener( rl::ObjectStateChangeListener* list ); 	 
			bool hasListeners( ) const; 	 
			Ogre::UserDefinedObject* getObject() const { return mObject; } 	 
				void fireObjectStateChangeEvent(); 	 
		}; 	 
		  	 
		class ObjectStateChangeEvent 	 
		{ 	 
		public: 	 
			ObjectStateChangeEvent( rl::ObjectStateChangeEventSource* src ); 	 
			Ogre::UserDefinedObject* getProvokingObject() const { return mObject; }; 	 
			rl::ObjectStateChangeEventSource* getSource() const; 	 
		};
		    
    
    %feature("director") GameObject;
    class GameObject: public ObjectStateChangeEventSource
    {
    public:
        GameObject(int id, const rl::CeGuiString& name, const rl::CeGuiString& description);
        virtual ~GameObject(void);

        virtual int getId();

        virtual const rl::CeGuiString& getName() const;
        virtual void setName(rl::CeGuiString& name);

        virtual const rl::CeGuiString& getDescription() const;
        virtual void setDescription(rl::CeGuiString& description);   
        
        void setActor(rl::Actor* actor);
		rl::Actor* getActor();

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
			const rl::CeGuiString& actionName, 
			const rl::CeGuiString& className, 
			rl::Creature* actor,
            rl::GameObject* target);
        void doAction(
			const rl::CeGuiString& actionName, 
			const rl::CeGuiString& className);
        void doAction(rl::Action* action, rl::Creature* actor,
            rl::GameObject* target);
            
        void setString(const char* key, rl::CeGuiString value);
		void setBool(const char* key, bool value);
		void setInt(const char* key, int value);
		void setReal(const char* key, Ogre::Real real);

		rl::CeGuiString getString(const char* key);
		bool getBool(const char* key);
		int getInt(const char* key);
		Ogre::Real getReal(const char* key);
    };
    
    %feature("director") Item;
    class Item : public GameObject {
    public:
        Item(int id, const rl::CeGuiString& name, const rl::CeGuiString& description);
        virtual ~Item(void);
    };
    
    %feature("director") Container;
    class Container : public GameObject
    {
    public:
        Container(int id, const rl::CeGuiString& name, const rl::CeGuiString& description);
        virtual ~Container(void);

        int getCapacity() const;
        void setCapacity(int capacity);
        int getContentWeight() const;
        void addItem(rl::Item* item);
        rl::Item* removeItem(int itemId);
        rl::ItemSet getItems() const;
	};
    
    %feature("director") Creature;
    class Creature : public GameObject {
    public:
        Creature(int id, const rl::CeGuiString& name, const rl::CeGuiString& description);
        //Creature(const rl::CeGuiString& data);
        virtual ~Creature();

        virtual int getEigenschaft(int id) const;
        virtual void setEigenschaft(int id, int value);
        virtual int getTalent(int id) const;
        virtual void setTalent(int id, int value);

        virtual void modifyEigenschaft(int id, int mod);
        virtual void modifyTalent(int id, int mod);

        virtual void modifyLe(int mod);
        virtual void modifyAe(int mod);
        virtual void modifyAu(int mod);
        virtual int getLe();

        virtual int doTalentprobe(int id, int modifier);
        virtual int doEigenschaftsprobe(int id, int modifier);
        
        void setWert(int id, int value);
   };
   
   %feature("director") Person;
   class Person : public Creature
   {
   public:
	    Person(int id,
                 const rl::CeGuiString& name,
                 const rl::CeGuiString& description);
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
    rl::Container *pContainer=dynamic_cast<rl::Container*>(*ppGameObject);
    if (pContainer) {
        *ptr=reinterpret_cast<void*>(pContainer);
        return SWIGTYPE_p_rl__Container;
    }
    return 0;
}
%}
DYNAMIC_CAST(SWIGTYPE_p_rl__GameObject, GameObject_dynamic_cast);

