/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2006 Team Pantheon. http://www.team-pantheon.de
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

	class RulesSubsystem 
    {
		public:
 			static rl::RulesSubsystem& getSingleton();
 			rl::QuestBook* getQuestBook();
 			void resetQuestBook();
    };
  
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
	    rl::Talent* getTalent(const rl::CeGuiString name) const;
	    rl::Person* getPerson(const int id) const;

        void loadDsaDataFile(const std::string& filename);	
    };

	%feature("director") QuestListener; 
	class QuestListener
	{
	public:
		virtual ~QuestListener();
		
		virtual void questStateChanged(rl::QuestEvent* anEvent) = 0;
		virtual void questPartsDoneChanged(rl::QuestEvent* anEvent) = 0;
		virtual void questKnownChanged(rl::QuestEvent* anEvent) = 0;
		virtual void questSubquestAdded(rl::QuestEvent* anEvent) = 0;
		virtual void journalEntryAdded(rl::JournalEvent* anEvent) = 0;
	};

    class JournalEntry
    {
    public:
        JournalEntry(const rl::CeGuiString caption, const rl::CeGuiString text);
        virtual ~JournalEntry();

        rl::CeGuiString getCaption() const;

        rl::CeGuiString getText() const;
    };
    
	class Quest
	{
	public:
		enum State
		{
			OPEN = 0,
			ASSIGNED,
			FAILED,
			SUCCEEDED,
			COMPLETED
		};

		Quest(const rl::CeGuiString id, const rl::CeGuiString name, const rl::CeGuiString description);
		virtual ~Quest();

		const rl::CeGuiString getId();
		const rl::CeGuiString getName();
		const rl::CeGuiString getDescription();
		int getPartsToDo();
		void setPartsToDo(int partsToDo);
		int getPartsDone();
		void setPartsDone(int partsDone);
		void increasePartsDone(int parts = 1);
		void decreasePartsDone(int parts = 1);

		rl::Quest::State getState();
		void setState(rl::Quest::State state);
		bool isKnown();
		void setKnown(bool known);

		// QuestVector getSubquests();
		void addSubquest(rl::Quest* quest); 
	};   
	
	class QuestEvent
	{
	public:
		rl::Quest* getQuest();
	private:
		QuestEvent( rl::QuestBook* src, int reason );
	};
	
	class QuestBook
	{
	public:
		rl::Quest* getQuest(const rl::CeGuiString id);
		void addQuest(rl::Quest* quest);
		
		void addJournalEntry(rl::JournalEntry* entry);
		void addJournalEntry(rl::CeGuiString caption, rl::CeGuiString text);
		
		void addQuestListener(rl::QuestListener* listener);
		void removeQuestListener(rl::QuestListener* listener);
	};
	
    class Talent
    {
    public:
        Talent(const rl::CeGuiString name,
               const rl::CeGuiString description,
               const Tripel<rl::CeGuiString>& eigenschaften,
               int ebe,
			   int gruppe,
			   const rl::CeGuiString art);

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
	
		const rl::CeGuiString getName() const;
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
    
        Action(const rl::CeGuiString name, const rl::CeGuiString description);
        virtual ~Action();

        virtual const rl::CeGuiString getName() const;
        //virtual const rl::CeGuiString getClassName() const;
        virtual const rl::CeGuiString getDescription() const;
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
        GameObject(const rl::CeGuiString name, const rl::CeGuiString description);
        virtual ~GameObject(void);

        virtual int getId();

        virtual const rl::CeGuiString getName() const;
        virtual void setName(rl::CeGuiString name);

        const rl::CeGuiString getDescription() const;
        virtual void setDescription(rl::CeGuiString description);   
        
        void setActor(rl::Actor* actor);
		rl::Actor* getActor();

        void addAction(rl::Action *action, int option = ACT_NORMAL);
        void addActionInGroup(Action* action, ActionGroup* group, int option = ACT_NORMAL);
        void removeAction(rl::Action *action);

        const std::vector<Action*> getValidActions(rl::Creature *actor) const;        
        
        virtual Action* getDefaultAction(rl::Creature* actor) const;
        
        void doAction(
			const rl::CeGuiString actionName, 
			rl::Creature* actor,
            rl::GameObject* target);
        void doAction(
			const rl::CeGuiString actionName);
        void doAction(rl::Action* action, rl::Creature* actor,
            rl::GameObject* target);
            
        void doDefaultAction(rl::Creature* actor, rl::GameObject* target);
            
		static const rl::CeGuiString DEFAULT_VIEW_OBJECT_ACTION;
		
		bool isHighlightingEnabled();
        void setHighlightingEnabled( bool highlightenabled );
    };
    
    %feature("director") Item;
    class Item : public GameObject {
    public:
        Item(const rl::CeGuiString name, const rl::CeGuiString description);
        virtual ~Item(void);
        
        enum ItemType;
        void setItemType(ItemType itemType);
        ItemType getItemType() const;
    };
    
    %feature("director") Weapon;
    class Weapon : public Item 
    {
    public:
		enum Distanzklasse { DK_H=0, DK_N, DK_S, DK_P };

        Weapon(const rl::CeGuiString name, const rl::CeGuiString description);
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
		void setKampftechnik(const rl::CeGuiString newKampftechnik);
		const rl::CeGuiString getKampftechnik() const;
     };
    
    %feature("director") Container;
    class Container : public GameObject
    {
    public:
        Container(const rl::CeGuiString name, const rl::CeGuiString description);
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
	
	class StateSet
	{
	public:
		StateSet();
		virtual ~StateSet();

		virtual int getValue(bool getUnmodfiedValue = false) const;
		virtual int getOriginalValue() const;
		virtual void setOriginalValue(int newValue);
		virtual int getModifier() const;
		virtual void setModifier(int newModifier);
		virtual void modifyModifier(int modifier);
		virtual int getProbenModifier() const;
		virtual void setProbenModifier(int newProbenModifier);
		virtual void modifyProbenModifier(int modifier);
		virtual int getMultiplier() const;
		virtual void setMultiplier(int newMultiplier);
	};

	class EigenschaftenStateSet : public StateSet
	{
	public:
		EigenschaftenStateSet();
		~EigenschaftenStateSet();

		int getStartValue();
		void setStartValue( int newStartValue );
		int getPermanentModifier();
		void setPermanentModifier( int newPermanentModifier );
		int getModifierWithoutRecalculation();
		void setModifierWithoutRecalculation( int newModifierWithoutRecalculation );

		int getValue( bool getUnmodifiedValue = false );
		int getValueForBasiswertCalculation();
	};
	
	class SonderfertigkeitenStateSet : public StateSet
	{
	public:
		SonderfertigkeitenStateSet();
		~SonderfertigkeitenStateSet();

		void setModifier( int newModifier );
		int getLehrmeisterZeiteinheiten();
		void setLehrmeisterZeiteinheiten( int newLehrmeisterZeiteinheiten );
	};
	
	class TalentStateSet : public StateSet
	{
	public:
		TalentStateSet();
		~TalentStateSet();

		bool getSe();
		void setSe(bool newSe);
		int getLehrmeisterZeiteinheiten();
		void setLehrmeisterZeiteinheiten( int newLehrmeisterZeiteinheiten );
		int getSelbststudiumZeiteinheiten();
		void setSelbststudiumZeiteinheiten( int newSelbststudiumZeiteinheiten );
	};
	
	%feature("director") Effect;
	class Effect
	{
	public:
        enum Quantifier
        {
            QUANTIFIER_MULTIPLE,
            QUANTIFIER_UNIQUE,
            QUANTIFIER_UNIQUE_BUT_PROLONGABLE,
            QUANTIFIER_UNIQUE_BUT_CUMULATIVE
        };

        enum EffectTag
        {
            EFFECTTAG_NONE,
            EFFECTTAG_KRANKHEIT
        };

		Effect(int stufe = 1);
		virtual ~Effect();

        const rl::CeGuiString getName() const;
        void setName(rl::CeGuiString name);
        const rl::CeGuiString getDescription() const;
        void setDescription(rl::CeGuiString description);
		rl::Creature* getOwner() const;
		void setOwner(rl::Creature* owner);
		virtual int getDuration();
		virtual void setDuration(int newDuration);
		Quantifier getQuantifier();
		void setQuantifier(Quantifier quantifier);
		virtual void addTag(EffectTag tagId);
		virtual void removeTag(EffectTag tagId);
		virtual bool queryTag(EffectTag tagId);
		virtual const int getStufe();
		virtual void increaseStufe();
		virtual void decreaseStufe();
		virtual void enable();
		virtual void disable();
		virtual void check() = 0;

	protected:
		virtual void apply() = 0;
		virtual void remove() = 0;
		virtual RL_LONGLONG getTimePassed();
	};

    %feature("director") Creature;
    class Creature : public GameObject {
    public:

    enum Wert
    {
        WERT_MOD_AE = 1, // Astralenergie
        WERT_MOD_LE, // Lebensenergie
        WERT_MOD_AT, // Attacke
        WERT_MOD_PA, // Parade
        WERT_MOD_FK, // Fernkampf
        WERT_MOD_AU, // Ausdauer
        WERT_MOD_MR, // Magieresistenz
        WERT_MOD_INI, // Initiative
        WERT_MOD_REGENERATION_LE, // Naechtliche Regeneration. modifier modifiziert den W6, ProbenModifier modifiziert die KO-Probe.
        WERT_MOD_REGENERATION_AE, // Astrale Regeneration. modifier modifiziert den W6, ProbenModifier modifiziert die IN-Probe.
        WERT_MOD_ERSCHOEPFUNGSSCHWELLE, // Die Modifkitoren von KO bezueglich der Erschoepfungsschwelle.
        WERT_MOD_ALL_EIGENSCHAFTSPROBEN, // Modifiziert alle Proben, die mit 1W20 gewuerfelt werden.
        WERT_MOD_ALL_TALENTPROBEN, // Modifiziert alle Proben, die mit 3W20 gewuerfelt werden.
        WERT_GS, // Geschwindigkeit
        WERT_SOZIALSTATUS, // Sozialstatus
        WERT_BE // Behinderung
    };

        Creature(const rl::CeGuiString name, const rl::CeGuiString description);
        //Creature(const rl::CeGuiString data);
        virtual ~Creature();

        virtual int getLe();
        virtual void modifyLe(int mod, bool ignoreMax);
        virtual void modifyAe(int mod);
        virtual void modifyAu(int mod);
        void setWert(Wert wertId, int value);
        virtual StateSet* getWertStateSet(Wert wertId);
        
        virtual int getEigenschaft(const rl::CeGuiString eigenschaftName);
        virtual void setEigenschaft(const rl::CeGuiString eigenschaftName, int value);
        virtual void modifyEigenschaft(const rl::CeGuiString eigenschaftName, int mod);
		virtual EigenschaftenStateSet* getEigenschaftenStateSet(const rl::CeGuiString eigenschaftName);
		
        void addTalent(const rl::CeGuiString talentName, int value=0);
        virtual int getTalent(const rl::CeGuiString talentName);
        virtual void setTalent(const rl::CeGuiString talentName, int value);
        virtual void modifyTalent(const rl::CeGuiString talentName, int mod);
		virtual void addSe(const rl::CeGuiString talentName);
		virtual TalentStateSet* getTalentStateSet(const rl::CeGuiString talentName);
		
		void addVorteil(const rl::CeGuiString vorteilName, int value = 0);
		void addNachteil(const rl::CeGuiString nachteilName, int value = 0);		
		
        void addKampftechnik(const rl::CeGuiString kampftechnikName, std::pair<int,int> value = make_pair(0,0));
        
		virtual SonderfertigkeitenStateSet* getSonderfertigkeitenStateSet(const rl::CeGuiString sfName);

		void addContainer(rl::Container* container);
		rl::Container* getContainer(const rl::CeGuiString containterName) const;
		rl::Container* removeContainer(rl::CeGuiString containerName);
		void addWeapon(Weapon* weapon);
		Weapon* getWeapon(int weaponId) const;
		Weapon* removeWeapon(int waponId);
		void switchToWeapon(int weaponId);
		void rl::Creature::detachWeapon();
		rl::Weapon* rl::Creature::getActiveWeapon();
		
        virtual int doTalentprobe(const rl::CeGuiString talentName, int modifier);
        virtual int doEigenschaftsprobe(const rl::CeGuiString eigenschaftName, int modifier);
        
		void addEffect(Effect* effect);
		void damageLe(int tp, int damageType = LEDAMAGE_NORMAL);
        static const int LEDAMAGE_NORMAL = 0;
        static const int LEDAMAGE_FIRE = 1;
        static const int LEDAMAGE_WATER = 2;
        static const int LEDAMAGE_DEMONIC = 4;
        void damageAu(int aup, int damageType = AUDAMAGE_NORMAL);
        static const int AUDAMAGE_NORMAL = 0;
        static const int AUDAMAGE_HEAT = 1;
        static const int AUDAMAGE_COLD = 2;
        static const int AUDAMAGE_CHOKE = 4;
        static const int AUDAMAGE_DROWN = 8;
   };
   
   %feature("director") Person;
   class Person : public Creature
   {
   public:
	    Person(  const rl::CeGuiString name,
                 const rl::CeGuiString description);
	};
		
	%feature("director") CompositeEffect;	
	class CompositeEffect : public Effect
	{
	public:
		CompositeEffect();
		virtual ~CompositeEffect();

		void addEffect(Effect* effect);
		virtual void check();
	protected:
		void apply();
		void remove();
	};

	%feature("director") ActionMananger;
    class ActionManager
    {
    public:
        void registerAction(rl::Action* action);
        void unregisterAction(const rl::CeGuiString actionName);
		rl::Action* getAction(const rl::CeGuiString actionName) const;

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
        void removeTimerListener( rl::TimerListener* ist );
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
