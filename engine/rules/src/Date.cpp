#include "Date.h"

namespace rl
{
    static const char* const MONTHS[] =
    {
      "Praios",
      "Rondra",
      "Efferd",
      "Travia",
      "Boron",
      "Hesinde",
      "Firun",
      "Tsa",
      "Phex",
      "Peraine",
      "Ingerimm",
      "Rahja",
      "???"
    };

    static const int MONTH_MODIFIER[] =
    {
      0, 2, 4, 6, 1, 3, 5, 0, 2, 4, 6, 1, 3
    };

    static const int WEEKDAY_OFFSET[] =
    {
      2, 3, 4, 5, 6, 7, 1
    };

    static const char* const DAYS_OF_WEEK[] =
    {
      "Windstag",
      "Erdtag",
      "Markttag",
      "Praiostag",
      "Rohalstag",
      "Feuertag",
      "Wassertag"
    };

    const unsigned RL_LONGLONG Date::ONE_SECOND = 1000UL;
    const unsigned RL_LONGLONG Date::ONE_MINUTE = 60000UL;
    const unsigned RL_LONGLONG Date::ONE_HOUR = 3600000UL;
    const unsigned RL_LONGLONG Date::ONE_DAY = 86400000UL;
    const unsigned RL_LONGLONG Date::ONE_WEEK = 604800000UL;
    const unsigned RL_LONGLONG Date::ONE_MONTH = 2592000000UL;

#if (_MSC_VER >= 1200) && (_MSC_VER < 1300)
    // VC6 kennt kein ULL-Suffix 
    const unsigned RL_LONGLONG Date::ONE_YEAR = 31536000000UL;
#else
    // Und GCC will unbedingt ULL
    const unsigned RL_LONGLONG Date::ONE_YEAR = 31536000000ULL;
#endif
    const unsigned RL_LONGLONG Date::ONE_KAMPFRUNDE = 2 * Date::ONE_SECOND;
    const unsigned RL_LONGLONG Date::ONE_SPIELRUNDE = 5 * Date::ONE_MINUTE;

    Date::Date()
    {
        mTimestamp = 0;
        calculateFields();
    }

    Date::Date(const RL_LONGLONG& timestamp)
    {
        mTimestamp = timestamp;
        calculateFields();
    }

    Date::Date(const Date& rhs)
    {
        mTimestamp = rhs.mTimestamp;
        calculateFields();
    }

    Date& Date::operator=(const Date& rhs)
    {
        if (this != &rhs)
        {
            mTimestamp = rhs.mTimestamp;
            calculateFields();
        }
        return *this;
    }

    bool Date::operator==(const Date& rhs)
    {
        return mTimestamp == rhs.mTimestamp;
    }

    bool Date::operator<(const Date& rhs)
    {
        return mTimestamp < rhs.mTimestamp;
    }

    bool Date::operator<=(const Date& rhs)
    {
        return mTimestamp <= rhs.mTimestamp;
    }

    bool Date::operator>=(const Date& rhs)
    {
        return mTimestamp >= rhs.mTimestamp;
    }

    bool Date::operator>(const Date& rhs)
    {
        return mTimestamp > rhs.mTimestamp;
    }

    Date Date::operator+(const RL_LONGLONG& rhs)
    {
        return Date(mTimestamp + rhs);
    }

    Date Date::operator-(const RL_LONGLONG& rhs)
    {
        return Date(mTimestamp - rhs);
    }

    RL_LONGLONG Date::operator-(const Date& rhs)
    {
        return mTimestamp - rhs.mTimestamp;
    }

    void Date::calculateFields()
    {
        //!\todo aufr?umen!
        mYear = mTimestamp / ONE_YEAR;
        RL_LONGLONG timeInYear = mTimestamp % ONE_YEAR;
        mMonth = timeInYear / ONE_MONTH;
        int timeInMonth = timeInYear % ONE_MONTH;
        mDayOfYear = timeInYear / ONE_DAY;
        mDayOfMonth = timeInMonth / ONE_DAY;
        int timeInDay = timeInMonth % ONE_DAY;
        mHour = timeInDay / ONE_HOUR;
        int timeInHour = timeInDay % ONE_HOUR;
        mMinute = timeInHour / ONE_MINUTE;
        int timeInMinute = timeInHour % ONE_MINUTE;
        mSecond = timeInMinute / ONE_SECOND;
        mMilli = timeInMinute % ONE_SECOND;
    }

    RL_LONGLONG Date::getTimestamp()
    {
        return mTimestamp;
    }

    int Date::getTimeFraction()
    {
        return mTimestamp % ONE_DAY;
    }

    int Date::getYear()
    {
        return mYear;
    }

    int Date::getMonth()
    {
        return mMonth + 1;
    }

    std::string Date::getMonthName()
    {
        return std::string(MONTHS[mMonth]);
    }

    int Date::getDayOfYear()
    {
        return mDayOfYear + 1;
    }

    int Date::getDayOfMonth()
    {
        return mDayOfMonth + 1;
    }

    int Date::getDayOfWeek()
    {
        int wd = ((getYear() - 993) + getDayOfMonth() + MONTH_MODIFIER[mMonth]) %
                 7;

        if (wd < 0)
        {
            wd += 7;
        }

        return WEEKDAY_OFFSET[wd];
    }

    std::string Date::getDayOfWeekName()
    {
        return std::string(DAYS_OF_WEEK[getDayOfWeek() - 1]);
    }

    int Date::getHour()
    {
        return mHour;
    }

    int Date::getMinute()
    {
        return mMinute;
    }

    int Date::getSecond()
    {
        return mSecond;
    }

    int Date::getMilliSecond()
    {
        return mMilli;
    }

    std::string Date::toString()
    {
        char date[256];
        _snprintf(date,
                  255,
                  "%s, der %d. %s im Jahre %dBF, %02d:%02d.",
                  getDayOfWeekName().c_str(),
                  getDayOfMonth(),
                  getMonthName().c_str(),
                  mYear,
                  mHour,
                  mMinute);
        return std::string(date);
    }

    Date operator+(const RL_LONGLONG& lhs, const Date& rhs)
    {
        return Date(rhs.mTimestamp + lhs);
    }

    Date operator-(const RL_LONGLONG& lhs, const Date& rhs)
    {
        return Date(rhs.mTimestamp - lhs);
    }
}
