#ifndef __TALENTTEST_H__
#define __TALENTTEST_H__

#include "DsaManager.h"
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace CppUnit;
using namespace std;
using namespace rl;

class TalentTest : public TestFixture {
  CPPUNIT_TEST_SUITE( TalentTest );
  CPPUNIT_TEST( testConstruction );
  CPPUNIT_TEST_SUITE_END();
protected:
   void testConstruction()
   {
       new DsaManager();
   }
};

#endif
