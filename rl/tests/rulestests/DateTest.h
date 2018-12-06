#ifndef __DATETEST_H__
#define __DATETEST_H__

#include "RulesTestPrerequisites.h"

#include "Date.h"
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace CppUnit;
using namespace std;
using namespace rl;

class DateTest : public TestFixture {
  CPPUNIT_TEST_SUITE( DateTest );
  CPPUNIT_TEST( testConstruction );
  CPPUNIT_TEST( testEquality );
  CPPUNIT_TEST( testOperators );
  CPPUNIT_TEST( testDayOfWeek );
  CPPUNIT_TEST_SUITE_END();
protected:
   Date date1, date2;

   void testConstruction()
   {
      // Timestampkonstruktor
      Date d(
         Date::ONE_YEAR * 1 +
         Date::ONE_MONTH * 1 +
         Date::ONE_DAY * 3);

      CPPUNIT_ASSERT_EQUAL(1, d.getYear());
      CPPUNIT_ASSERT_EQUAL(2, d.getMonth()); // +1, es gibt kein 0. Monat
      CPPUNIT_ASSERT_EQUAL(34, d.getDayOfYear()); // +1, es gibt kein 0. Tag
      CPPUNIT_ASSERT_EQUAL(4, d.getDayOfMonth()); // +1, es gibt kein 0. Tag

      // Timestampkonstruktor + Zuweisungsoperator
      d = Date(
         Date::ONE_YEAR * 1023 +
         Date::ONE_MONTH * 5 +
         Date::ONE_DAY * 3 + 
         Date::ONE_HOUR * 10 +
         Date::ONE_MINUTE * 15 +
         Date::ONE_SECOND * 44 +
         333);

      CPPUNIT_ASSERT_EQUAL(1023, d.getYear());
      CPPUNIT_ASSERT_EQUAL(6, d.getMonth());
      CPPUNIT_ASSERT_EQUAL(154, d.getDayOfYear());
      CPPUNIT_ASSERT_EQUAL(4, d.getDayOfMonth());
      CPPUNIT_ASSERT_EQUAL(10, d.getHour());
      CPPUNIT_ASSERT_EQUAL(15, d.getMinute());
      CPPUNIT_ASSERT_EQUAL(44, d.getSecond());
      CPPUNIT_ASSERT_EQUAL(333, d.getMilliSecond());

      // Kopierkonstruktor
      Date d1(d);

      CPPUNIT_ASSERT_EQUAL(d.getYear() , d1.getYear());
      CPPUNIT_ASSERT_EQUAL(d.getMonth(), d1.getMonth());
      CPPUNIT_ASSERT_EQUAL(d.getDayOfYear(), d1.getDayOfYear());
      CPPUNIT_ASSERT_EQUAL(d.getDayOfMonth(), d1.getDayOfMonth());
      CPPUNIT_ASSERT_EQUAL(d.getHour(), d1.getHour());
      CPPUNIT_ASSERT_EQUAL(d.getMinute(), d1.getMinute());
      CPPUNIT_ASSERT_EQUAL(d.getSecond(), d1.getSecond());
      CPPUNIT_ASSERT_EQUAL(d.getMilliSecond(), d1.getMilliSecond());
   }

   void testEquality()
   {
      CPPUNIT_ASSERT_MESSAGE("Sollten nicht gleich sein.",
         !(date1 == date2));
      date2 = date1;
      CPPUNIT_ASSERT_MESSAGE("Sollten gleich sein.",
         date1 == date2);
   }

   void testOperators()
   {
      //Additionsoperatoren
      CPPUNIT_ASSERT(Date::ONE_DAY == (date1 + Date::ONE_DAY).getTimestamp());
      CPPUNIT_ASSERT(Date::ONE_DAY == (Date::ONE_DAY + date1).getTimestamp());
   }

   void testDayOfWeek()
   {
      // Ist laut Fanpro-Tabellenwerk ein Marktag
      // 22. Ingerimm 22 Hal
      Date d = Date(
         Date::ONE_YEAR * 1015 +
         Date::ONE_MONTH * 10 +
         Date::ONE_DAY * 21);

      CPPUNIT_ASSERT_EQUAL(3, d.getDayOfWeek());      

      // Ist laut Fanpro-Tabellenwerk ein Erdstag
      // 4. Namenloser Tag 7 Hal
      d = Date(
         Date::ONE_YEAR * 1000 +
         Date::ONE_MONTH * 12 +
         Date::ONE_DAY * 3);

      CPPUNIT_ASSERT_EQUAL(2, d.getDayOfWeek());

      // Ist laut Fanpro-Tabellenwerk ein Rohalstag
      // 18. Rondra 45 v.H.
      d = Date(
         Date::ONE_YEAR * 948 +
         Date::ONE_MONTH * 1 +
         Date::ONE_DAY * 17);

      CPPUNIT_ASSERT_EQUAL(5, d.getDayOfWeek());      
   }

public:
   void setUp()
   {
      date1 = Date(0);
      date2 = Date(-1);
   }
};

#endif
