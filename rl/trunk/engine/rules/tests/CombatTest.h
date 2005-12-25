#ifndef __COMBATTEST_H__
#define __COMBATTEST_H__

#include "RulesTestPrerequisites.h"
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "ActionManager.h"
#include "Combat.h"
#include "CombatEvents.h"
#include "Creature.h"
#include "Date.h"
#include "RubyInterpreter.h"

using namespace CppUnit;
using namespace std;
using namespace rl;

const int TEST_OBJ_GROUP = 0;
const int TEST_OPP_GROUP = 1;

class CombatTestResult : public CombatEventListener {
public:

	CombatTestResult(int group)
		: CombatEventListener(group),
		mLastEvent(NULL)
	{
	}

	bool eventRaised(rl::CombatFinishEvent * evt)
	{
		mLastEvent = evt;
		return true;
	}

    bool eventRaised(rl::AskForActionEvent * evt)
	{
		mLastEvent = evt;
		return true;
	}

	bool eventRaised(rl::AskForReactionEvent * evt)
	{
		mLastEvent = evt;
		return true;
	}

	CombatEvent* getLastEvent() { return mLastEvent; }

private:
	CombatEvent* mLastEvent;
};

class MockupDefaultObjectAction : public Action {
public:
	MockupDefaultObjectAction()
		: Action("viewobject", "viewobject")
	{
	}
};

class CombatTest;

class FriendCombat : public Combat {
	friend class CombatTest;
};

class CombatTest : public TestFixture {
  CPPUNIT_TEST_SUITE( CombatTest );
  CPPUNIT_TEST(testConstruction);
  CPPUNIT_TEST(testInitializeKampfrunde);
  CPPUNIT_TEST_SUITE_END();
protected:
public:

	void testConstruction()
	{
		CPPUNIT_ASSERT_MESSAGE("Listener sollten fuer Gruppe 1 vorhanden sein", mCombat->mEventCasters[TEST_OBJ_GROUP].hasEventListeners());
		CPPUNIT_ASSERT_MESSAGE("Listener sollten fuer Gruppe 2 vorhanden sein", mCombat->mEventCasters[TEST_OPP_GROUP].hasEventListeners());
		CPPUNIT_ASSERT_EQUAL_MESSAGE("Creature 1 sollte INI-Basis 8 haben", 8, mCreature1->getInitiativeBasis());
		CPPUNIT_ASSERT_EQUAL_MESSAGE("Creature 2 sollte INI-Basis 1 haben", 1, mCreature2->getInitiativeBasis());
	}

	void testInitializeKampfrunde()
	{
		mCombat->start();
		mCombat->run(0.0);

		CPPUNIT_ASSERT_EQUAL_MESSAGE("Zeit fuer eine Aktion is 1/4 KR", 0.25f * (float)Date::ONE_KAMPFRUNDE, mCombat->mTimeOfAction);
		CPPUNIT_ASSERT_MESSAGE("Creature 1 sollte INI > 8 haben", mCombat->getParticipant(mCreature1)->initiative > 8);
		CPPUNIT_ASSERT_MESSAGE("Creature 2 sollte 1 < INI < 8 haben", mCombat->getParticipant(mCreature2)->initiative > 1);


		CPPUNIT_ASSERT_MESSAGE("INI sollte nicht mehr auf Start stehen", mCombat->mCurrentInitiative != mCombat->INI_START);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("1 Event auf dem Stack", 1, (int)mCombat->mEventStack.size());
		CPPUNIT_ASSERT_MESSAGE("SlowMo sollte aktiviert sein", mCombat->mIsSlowMotion == true);

		CPPUNIT_ASSERT_MESSAGE("Kein Event sollte bei Gruppe 2 empfangen worden sein", mCombatTestResultGroup2->getLastEvent() == NULL);
		CPPUNIT_ASSERT_MESSAGE("Ein Event sollte bei Gruppe 1 empfangen worden sein", mCombatTestResultGroup1->getLastEvent() != NULL);
		
		CPPUNIT_ASSERT_EQUAL_MESSAGE("Event sollte AskForAction sein", CombatEvent::CEV_ASK_FOR_ACTION, mCombatTestResultGroup1->getLastEvent()->getEventType());
		CPPUNIT_ASSERT_MESSAGE("SlowMo sollte aktiviert sein", mCombatTestResultGroup1->getLastEvent()->isGroupRelevant(TEST_OBJ_GROUP) == true);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("Creature 1 sollte das Event erhalten haben", CombatEvent::CEV_ASK_FOR_ACTION, mCombatTestResultGroup1->getLastEvent()->getEventType());
	}



	void setUp()
	{
		new ActionManager();

		mCreature1 = new Creature("Creature1", "Creature 1"); //INI-Basis = 8
		// INI_BASIS = (2 * MU + IN + GE) / 5
		mCreature1->setEigenschaft(E_MUT, 10);
		mCreature1->setEigenschaft(E_INTUITION, 10);
		mCreature1->setEigenschaft(E_GEWANDTHEIT, 10);

		mCreature2 = new Creature("Creature2", "Creature 2"); //INI-Basis = 1
		mCreature2->setEigenschaft(E_MUT, 1);
		mCreature2->setEigenschaft(E_INTUITION, 1);
		mCreature2->setEigenschaft(E_GEWANDTHEIT, 1);		
		
		mCombat = new FriendCombat();
		mCombat->add(mCreature1, TEST_OBJ_GROUP); mCombatTestResultGroup1 = new CombatTestResult(TEST_OBJ_GROUP);
		mCombat->add(mCreature2, TEST_OPP_GROUP); mCombatTestResultGroup2 = new CombatTestResult(TEST_OPP_GROUP);

		mCombat->addCombatEventListener(mCombatTestResultGroup1);
		mCombat->addCombatEventListener(mCombatTestResultGroup2);
	}

	void tearDown()
	{
		mCombat->removeCombatEventListener(mCombatTestResultGroup1);
		mCombat->removeCombatEventListener(mCombatTestResultGroup2);
		delete mCombatTestResultGroup1;
		delete mCombatTestResultGroup2;

		delete mCombat;

		delete mCreature1;
		delete mCreature2;
	}

private:
	FriendCombat* mCombat;
	CombatTestResult* mCombatTestResultGroup1;
	CombatTestResult* mCombatTestResultGroup2;
	Creature* mCreature1;
	Creature* mCreature2;
};


#endif
