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
        Talent(const rl::CeGuiString& name,
               const rl::CeGuiString& description,
               const Tripel<rl::CeGuiString>& eigenschaften,
               int ebe,
			   int gruppe);

        bool operator==(const Talent& rhs) const;
        bool operator<(const Talent& rhs) const;
        rl::CeGuiString getName() const;
        rl::CeGuiString getDescription() const;
		int getEbe() const;

        /// Berechnet effektive Behinderung bei gegebener Behinderung;
        int calculateEbe(int be) const;
        Tripel<rl::CeGuiString> getEigenschaften() const;
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
	
	%feature("director") Action;   
    class Action
    {
    public:
    	static const int ACT_NORMAL = 1<<0;
		static const int ACT_INVIS_DESC = 1<<1;
		static const int ACT_DISABLED = 1<<2;
		static const int ACT_NEEDS_TALENT = 1<<8;

		static const int TC_NO_TARGET = 0;
		static const int TC_GAMEOBJECT = 1;
		static const int TC_CREATURE = 2;
		static const int TC_ITEM = 3;
		static const int TC_POINT = 4;
		static const int TC_RADIUS = 5;
		static const int TC_POINT_AND_RADIUS = 6;
    
        Action(const rl::CeGuiString& name, const rl::CeGuiString& description);
        virtual ~Action();

        virtual const rl::CeGuiString& getName() const;
        //virtual const rl::CeGuiString& getClassName() const;
        virtual const rl::CeGuiString& getDescription() const;
        virtual int getTargetClass() const;
        virtual void doAction(rl::GameObject *object, rl::Creature *actor,
            rl::GameObject *target);
        virtual bool canDo(rl::GameObject* object, rl::Creature* actor) const;
            
        void setGroup(rl::ActionGroup* group);
		rl::ActionGroup* getGroup() const;
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
			rl::GameObject* getObject() const;
			void fireObjectStateChangeEvent(); 	 
		}; 	 
		  	 
		class ObjectStateChangeEvent 	 
		{ 	 
		public: 	 
			ObjectStateChangeEvent( rl::ObjectStateChangeEventSource* src ); 	 
			rl::GameObject* getProvokingObject() const; 	 
			rl::ObjectStateChangeEventSource* getSource() const; 	 
		};
		    
    
    %feature("director") GameObject;
    class GameObject: public ObjectStateChangeEventSource
    {
    public:
        GameObject(const rl::CeGuiString& name, const rl::CeGuiString& description);
        virtual ~GameObject(void);

        virtual int getId();

        virtual const rl::CeGuiString& getName() const;
        virtual void setName(rl::CeGuiString& name);

        const rl::CeGuiString& getDescription() const;
        virtual void setDescription(rl::CeGuiString& description);   
        
        void setActor(rl::Actor* actor);
		rl::Actor* getActor();

        void addAction(rl::Action *action, int option = ACT_NORMAL);
        void addActionInGroup(Action* action, ActionGroup* group, int option = ACT_NORMAL);
        void removeAction(rl::Action *action);

        const std::vector<Action*> getValidActions(rl::Creature *actor) const;        
        
        virtual Action* getDefaultAction(rl::Creature* actor) const;
        
        void doAction(
			const rl::CeGuiString& actionName, 
			rl::Creature* actor,
            rl::GameObject* target);
        void doAction(
			const rl::CeGuiString& actionName);
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
		
		static const rl::CeGuiString DEFAULT_VIEW_OBJECT_ACTION;
    };
    
    %feature("director") Item;
    class Item : public GameObject {
    public:
        Item(const rl::CeGuiString& name, const rl::CeGuiString& description);
        virtual ~Item(void);
        
        enum ItemType;
        void setItemType(ItemType itemType);
        ItemType getItemType() const;
    };
    
    %feature("director") Weapon;
    class Weapon : public Item 
    {
    public:
        Weapon(const rl::CeGuiString& name, const rl::CeGuiString& description);
        virtual ~Weapon(void);
        
        
		void setTp(int d6, int d20, int mod);
		const Tripel<int>& getTp() const;
		void setTpKk(int base, int step);
		const pair<int, int>& getTpKk() const;
		void setBf(int newBf);
		int getBf();
		void setIni(int newIni);
		int getIni();
		void setWm(pair<int, int>& newWm);
		const pair<int,int>& getWm() const;
		void setDk(Weapon::Distanzklasse newDk);
		Weapon::Distanzklasse getDk();
		void setKampftechnik(const rl::CeGuiString& newKampftechnik);
		const rl::CeGuiString& getKampftechnik() const;
     };
    
    %feature("director") Container;
    class Container : public GameObject
    {
    public:
        Container(const rl::CeGuiString& name, const rl::CeGuiString& description);
        virtual ~Container(void);

        int getCapacity() const;
        void setCapacity(int capacity);
        int getContentWeight() const;
        void addItem(rl::Item* item);
        rl::Item* removeItem(int itemId);
        rl::ItemSet getItems() const;
        int getItemCount() const;
        rl::Item* getItem(int itemId);
        
        virtual void close() = 0;
	};
    
    %feature("director") Creature;
    class Creature : public GameObject {
    public:
        Creature(const rl::CeGuiString& name, const rl::CeGuiString& description);
        //Creature(const rl::CeGuiString& data);
        virtual ~Creature();

        virtual int getEigenschaft(const rl::CeGuiString& eigenschaftName) const;
        virtual void setEigenschaft(const rl::CeGuiString& eigenschaftName, int value);
        void addTalent(const rl::CeGuiString& talentName, int value=0);
        virtual int getTalent(const rl::CeGuiString& talentName) const;
        virtual void setTalent(const rl::CeGuiString& talentName, int value);
        void addKampftechnik(const rl::CeGuiString& kampftechnikName, pair<int,int> value = make_pair(0,0));
		void addContainer(Container* container);
		Container* getContainer(const rl::CeGuiString& containterName) const;
		Container* removeContainer(rl::CeGuiString& containerName);
		void addWeapon(Weapon* weapon);
		Weapon* getWeapon(int weaponId) const;
		Weapon* removeWeapon(int waponId);

        virtual void modifyEigenschaft(const rl::CeGuiString& eigenschaftName, int mod);
        virtual void modifyTalent(const rl::CeGuiString& talentName, int mod);

        virtual void modifyLe(int mod);
        virtual void modifyAe(int mod);
        virtual void modifyAu(int mod);
        virtual int getLe();
        

        virtual int doTalentprobe(const rl::CeGuiString& talentName, int modifier);
        virtual int doEigenschaftsprobe(const rl::CeGuiString& eigenschaftName, int modifier);
        
        void setWert(int id, int value);
   };
   
   %feature("director") Person;
   class Person : public Creature
   {
   public:
	    Person(  const rl::CeGuiString& name,
                 const rl::CeGuiString& description);
	};
	%feature("director") ActionMananger;
    class ActionManager
    {
    public:
        void registerAction(rl::Action* action);
        void unregisterAction(const rl::CeGuiString& actionName);
		rl::Action* getAction(const rl::CeGuiString& actionName) const;

		void registerInGameGlobalAction(rl::Action* action, rl::ActionGroup* group);
	
        static rl::ActionManager& getSingleton();
	};
	
	%feature("director") TimerListener;
	class TimerListener
	{
	public:
		virtual ~TimerLister();
	  
  		virtual void timerFired(rl::TimerEvent* anEvent) = 0;
	};
	
	class TimerEventSource
	{
	public:
		static const RL_LONGLONG NO_REPEAT;

		TimerEventSource( const RL_LONGLONG gameTime, const RL_LONGLONG interval = rl::TimerEventSource::NO_REPEAT );
		TimerEventSource( const rl::Date dsaTime, const rl::Date interval = rl::TimerEventSource::NO_REPEAT );
	
		void addTimerListener( rl::TimerListener* ist );

	};
	
	class TimerEvent
	{
	public:
		TimerEvent( rl::TimerEventSource* src );
	
		Ogre::Real getTime();
		rl::Date getGameTime();

		rl::TimerEventSource* getSource() const;
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

