/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __RL_DATE_H__
#define __RL_DATE_H__

#include "RulesPrerequisites.h"

namespace rl
{
    /** 
     * The date class for the Aventurian calendar
     * The date is managed according to the calendar described
     * in the Geographia Aventurica (see p. XXX)
     * The format is based on milliseconds since
     * 00:00:00,000 1. Praios 0 BF, this value is stored as a
     * 64 bit timestamp, all other data (day, hour etc.) is
     * calculated.
     *  @todo CReal-Umrechnung, Komfortfunktionen
     *
     * Caution: all Real parameters or return values mean seconds, NOT milliseconds
     */
    class _RlRulesExport Date
    {
        friend Date _RlRulesExport operator+(const RL_LONGLONG&, const Date&);
        friend Date _RlRulesExport operator+(const Ogre::Real&, const Date&);
        friend Date _RlRulesExport operator-(const RL_LONGLONG&, const Date&);
        friend Date _RlRulesExport operator-(const Ogre::Real&, const Date&);

    public:
        /// One second in milliseconds
        static const unsigned RL_LONGLONG ONE_SECOND;
        /// One minute in milliseconds
        static const unsigned RL_LONGLONG ONE_MINUTE;
        /// One hour in milliseconds
        static const unsigned RL_LONGLONG ONE_HOUR;
        /// One day in milliseconds
        static const unsigned RL_LONGLONG ONE_DAY;
        /// One week in milliseconds
        static const unsigned RL_LONGLONG ONE_WEEK;
        /// One (dark eye) month in milliseconds
        static const unsigned RL_LONGLONG ONE_MONTH;
        /// One (dark eye) year in milliseconds
        static const unsigned RL_LONGLONG ONE_YEAR;

        /// One combat round (Kampfrunde) in milliseconds
        static const unsigned RL_LONGLONG ONE_KAMPFRUNDE;
        /// One game round (Spielrunde) in milliseconds
        static const unsigned RL_LONGLONG ONE_SPIELRUNDE;

        /** 
         * Default constructor creates date with timestamp 0
         * (00:00:00,000 1. Praios 0 BF)
         */
        Date();

        /// Create a date with given timestamp
        Date(const RL_LONGLONG& timestamp);

        /// Copy constructor
        Date(const Date& rhs);

        Date& operator=(const Date& rhs);
        bool operator==(const Date& rhs);
        bool operator<(const Date& rhs);
        bool operator<=(const Date& rhs);
        bool operator>=(const Date& rhs);
        bool operator>(const Date& rhs);

        Date operator+(const RL_LONGLONG& rhs);
		Date operator+(const Date& rhs);
		Date operator-(const RL_LONGLONG& rhs);
        RL_LONGLONG operator-(const Date& rhs);
        Date operator*(const Ogre::Real& rhs);
        Date operator/(const Ogre::Real& rhs);

        RL_LONGLONG getTimestamp() const;

        /// Milliseconds since 00:00:00,000 of the Date's day
        int getTimeFraction() const;
        int getYear() const;
        int getMonth() const;
        /// Get the name of the Date's month
        std::string getMonthName() const;
        int getDayOfYear() const;
        int getDayOfMonth() const;
        int getDayOfWeek() const;
        std::string getDayOfWeekName() const;
        int getHour() const;
        int getMinute() const;
        int getSecond() const;
        int getMilliSecond() const;

        /** 
         * Get a nicely formated string representation of the date
         * Format is 
         * "{Day of Week}, der {Day}. {Month} im Jahre {Year}, {Hour}:{Minute}.
         *
         * @return The date as a printable text
         */
        const std::string toString() const;

        static const Date fromReal(const Ogre::Real& time);
        const Ogre::Real toReal() const;

    private:

        int mMilli;
        int mSecond;
        int mMinute;
        int mHour;
        int mDayOfMonth;
        int mDayOfYear;
        int mMonth;
        int mYear;

        RL_LONGLONG mTimestamp;

        /** Berechnet den Inhalt der Felder in Abhaengigkeit von
         *  mTimestamp neu.
         */
        void calculateFields();
    };

    Date _RlRulesExport operator+(const RL_LONGLONG&, const Date&);
    Date _RlRulesExport operator-(const RL_LONGLONG&, const Date&);
}

#endif
