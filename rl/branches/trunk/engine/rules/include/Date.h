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

#ifndef __RL_DATE_H__
#define __RL_DATE_H__

#include "RulesPrerequisites.h"

namespace rl
{
    /** Die Datumsklasse fuer den Aventurischen Kalender.
     *  Das Datum wird auf Basis des in der Geographia Aventurica 
     *  beschriebenen Kalenders verwaltet. Das Format basiert auf
     *  die Millisekunden seit 00:00:00,000 1. Praios 0 BF.
     *  Dieser Wert wird als 64Bit Timestamp gespeichert, alle
     *  anderen Komponenten(Tag, Stunde etc.) werden daraus
     *  berechnet.
     *  @todo CReal-Umrechnung, Komfortfunktionen
     */
    class _RlRulesExport Date
    {
        friend Date _RlRulesExport operator+(const RL_LONGLONG&, const Date&);
        friend Date _RlRulesExport operator-(const RL_LONGLONG&, const Date&);
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
    public:
        /// Eine Sekunde in Millisekunden
        static const unsigned RL_LONGLONG ONE_SECOND;
        /// Eine Minute in Millisekunden
        static const unsigned RL_LONGLONG ONE_MINUTE;
        /// Eine Stunde in Millisekunden
        static const unsigned RL_LONGLONG ONE_HOUR;
        /// Ein Tag in Millisekunden
        static const unsigned RL_LONGLONG ONE_DAY;
        /// Eine Woche in Millisekunden
        static const unsigned RL_LONGLONG ONE_WEEK;
        /// Ein Monat in Millisekunden
        static const unsigned RL_LONGLONG ONE_MONTH;
        /// Ein Jahr in Millisekunden
        static const unsigned RL_LONGLONG ONE_YEAR;

        /// Ein Kampfrunde in Millisekunden
        static const unsigned RL_LONGLONG ONE_KAMPFRUNDE;
        /// Ein Spielrunde in Millisekunden
        static const unsigned RL_LONGLONG ONE_SPIELRUNDE;

        /// Standardkonstruktor erzeugt Datum mit timestamp 0
        Date();

        /// Konstruktor erzeugt Datum an uebergebener Timestamp
        Date(const RL_LONGLONG& timestamp);

        /// Kopierkonstruktor
        Date(const Date& rhs);

        Date& operator=(const Date& rhs);
        bool operator==(const Date& rhs);
        bool operator<(const Date& rhs);
        bool operator<=(const Date& rhs);
        bool operator>=(const Date& rhs);
        bool operator>(const Date& rhs);

        Date operator+(const RL_LONGLONG& rhs);
        Date operator-(const RL_LONGLONG& rhs);
        RL_LONGLONG operator-(const Date& rhs);

        RL_LONGLONG getTimestamp() const;

        /// Millisekunden seit 00:00:00,000 des instantiierten Tages.
        int getTimeFraction() const;
        int getYear() const;
        int getMonth() const;
        /// Name des instantiierten Monats
        std::string getMonthName() const;
        int getDayOfYear() const;
        int getDayOfMonth() const;
        int getDayOfWeek() const;
        std::string getDayOfWeekName() const;
        int getHour() const;
        int getMinute() const;
        int getSecond() const;
        int getMilliSecond() const;

        /** Gibt ein Datum in der Form {Wochentag},
         *  der {Tag}. {Monat} im Jahre {Jahr}, {Stunde}:{Minute}.
         *  zurueck.
         * @return Datum in ausgabefaehiger Textform.
         */
        std::string toString() const;
    };

    Date _RlRulesExport operator+(const RL_LONGLONG&, const Date&);
    Date _RlRulesExport operator-(const RL_LONGLONG&, const Date&);
}

#endif
