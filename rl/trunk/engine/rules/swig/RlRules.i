%module(directors="1") RlRules

%{
#include "FixRubyHeaders.h"
#include "RulesPrerequisites.h"
#include "Date.h"
#include "Tripel.h"
#include "DsaManager.h"
#include "GameObject.h"
#include "Creature.h"
#include "Action.h"
#include "Item.h"

#pragma warning( disable : 4312 )
using namespace rl;
%}
%include "../../common/swig/TypeMaps.i"
%include "stl.i"

// Kopie. Falls das nochmal irgendwohin kopiert werden muss,
// In separate Datei auslagen.
#if !defined(RL_LONGLONG)
#   if (_MSC_VER >= 1200) && (_MSC_VER < 1300)
#       define RL_LONGLONG _int64
#   else
#       define RL_LONGLONG long long
#   endif
#endif

namespace rl {

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
        std::string getMonthName();
        int getDayOfYear();
        int getDayOfMonth();
        int getDayOfWeek();
        std::string getDayOfWeekName();
        int getHour();
        int getMinute();
        int getSecond();
        int getMilliSecond();
      
        std::string toString();
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
    };

    %feature("director") GameObject;
    %apply SWIGTYPE *DYNAMIC { GameObject * };
    class GameObject
    {
    public:
        GameObject(int id, const std::string& name, const std::string& description);
        virtual ~GameObject(void);

        virtual int getId();

        virtual std::string getName();
        virtual void setName(const std::string& name);

        virtual std::string getDescription();
        virtual void setDescription(const std::string& description);   

        void addAction(rl::Action *action);
        //%extend {
        //    void rbAddAction(VALUE action) {
        //        void* ptr = reinterpret_cast<void*>(action);
        //        self->addAction(reinterpret_cast<Action*>(ptr));
        //    }
        //}
        void removeAction(const std::string& name);

        std::vector<std::string> getValidActions() const;
        
        void doAction(const std::string& action, rl::Creature* actor,
            rl::GameObject* target);
    };
    
    %feature("director") Item;
    class Item : public GameObject {
    public:
        Item(int id, const std::string& name, const std::string& description);
        virtual ~Item(void);
    };
    
    %feature("director") Creature;
    class Creature : public GameObject {
    public:
        Creature(int id, const std::string& name, const std::string& description);
        //Creature(const std::string& data);
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
   
    %feature("director") Action;   
    class Action
    {
    public:
        Action(const std::string& name, const std::string& description);
        virtual ~Action();

        virtual std::string getName();
        virtual std::string getDescription();
        virtual int getTargetClass();
        virtual void doAction(rl::GameObject *object, rl::Creature *actor,
            rl::GameObject *target);
    };
    

} // Namespace

%{
static swig_type_info *GameObject_dynamic_cast(void **ptr)
{
    rl::GameObject **ppGameObject = reinterpret_cast<rl::GameObject**>(ptr);
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
