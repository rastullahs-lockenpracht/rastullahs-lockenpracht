#ifndef __COMBATTEST_H__
#define __COMBATTEST_H__

#include "RulesTestPrerequisites.h"
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "ActionManager.h"
#include "Combat.h"
#include "CombatController.h"
#include "Creature.h"
#include "Date.h"
#include "DsaManager.h"
#include "Kampftechnik.h"

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
	void reset() { mLastEvent = NULL; }

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


class rl::CombatTest : public TestFixture {
	CPPUNIT_TEST_SUITE( rl::CombatTest );
	CPPUNIT_TEST(testConstruction);
	CPPUNIT_TEST(testGetParticipant);
	CPPUNIT_TEST(testCanDefend);
	CPPUNIT_TEST(testProcessAttack);
	CPPUNIT_TEST(testInitializeKampfrunde);
	CPPUNIT_TEST(testAutoEndeEreignis);
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

	void testGetParticipant()
	{
		Combat::Participant* part1 = mCombat->getParticipant(mCreature1);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("Gruppe 1", TEST_OBJ_GROUP, part1->group);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("Creature 1", mCreature1, part1->creature);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("Paraden 1", 1, part1->pareesLeft);

		Combat::Participant* part2 = mCombat->getParticipant(mCreature2);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("Gruppe 2", TEST_OPP_GROUP, part2->group);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("Creature 2", mCreature2, part2->creature);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("Paraden 2", 1, part2->pareesLeft);
	}

	void testCanDefend()
	{
		Combat::Participant* part1 = mCombat->getParticipant(mCreature1);
		Combat::Participant* part2 = mCombat->getParticipant(mCreature2);
		
		CPPUNIT_ASSERT_MESSAGE("Creature2 kann sich gegen Creature1 verteidigen", mCombat->canDefend(part1, part2));
		CPPUNIT_ASSERT_MESSAGE("Creature1 kann sich gegen Creature2 verteidigen", mCombat->canDefend(part2, part1) == true);

		part1->pareesLeft = 0;
		part2->pareesLeft = 0;

		CPPUNIT_ASSERT_MESSAGE("Creature2 kann sich nicht mehr gegen Creature1 verteidigen", mCombat->canDefend(part1, part2) == false);
		CPPUNIT_ASSERT_MESSAGE("Creature1 kann sich nicht mehr gegen Creature2 verteidigen", mCombat->canDefend(part2, part1) == false);
	}

	void testProcessAttack()
	{
		mCombat->setNextAction(mCreature1, new CombatActionAttack(mCreature1, mCreature2, "Raufen"), mCombatTestResultGroup1->getLastEvent()->getId());

		CombatActionAttack* attackAct = dynamic_cast<CombatActionAttack*>(mCombat->getLastEvent()->action);
		CPPUNIT_ASSERT_MESSAGE("Angreifer != NULL", attackAct->getSource() != NULL);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("Angreifer ist Creature1", mCreature1, attackAct->getSource());
		CPPUNIT_ASSERT_MESSAGE("Ziel != NULL", attackAct->getTarget() != NULL);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("Ziel ist Creature2", mCreature2, attackAct->getTarget());
		
		Combat::Participant* part1 = mCombat->getParticipant(mCreature1);
		Combat::Participant* part2 = mCombat->getParticipant(mCreature2);
		CPPUNIT_ASSERT_MESSAGE("Creature2 kann sich gegen Creature1 verteidigen", mCombat->canDefend(part1, part2) == true);

		mCombatTestResultGroup1->reset();
		mCombatTestResultGroup2->reset();
		bool result = mCombat->processAttack(part1, attackAct);

		CPPUNIT_ASSERT_MESSAGE("Kein Event sollte bei Gruppe 1 empfangen worden sein", mCombatTestResultGroup1->getLastEvent() == NULL);
		CPPUNIT_ASSERT_MESSAGE(" Ein Event sollte bei Gruppe 2 empfangen worden sein", mCombatTestResultGroup2->getLastEvent() != NULL);
	}

	void testAutoEndeEreignis()
	{
		CPPUNIT_ASSERT_MESSAGE(" Ein Event sollte bei Gruppe 1 empfangen worden sein", mCombatTestResultGroup1->getLastEvent() != NULL);
		CPPUNIT_ASSERT_MESSAGE("Kein Event sollte bei Gruppe 2 empfangen worden sein", mCombatTestResultGroup2->getLastEvent() == NULL);

		mCombat->setNextAction(mCreature1, new CombatActionAttack(mCreature1, mCreature2, "Raufen"), mCombatTestResultGroup1->getLastEvent()->getId());

		CPPUNIT_ASSERT_EQUAL_MESSAGE("1 Ereignis in Liste", 1, (int)mCombat->mEventStack.size());
		CPPUNIT_ASSERT_MESSAGE("Letztes Ereignis != NULL", mCombat->getLastEvent() != NULL);
		CPPUNIT_ASSERT_EQUAL_MESSAGE("Action des Ereignisses ist Attacke", CombatAction::ATTACK, mCombat->getLastEvent()->action->getType());



		mCombatTestResultGroup1->reset();
		mCombatTestResultGroup2->reset();

		mCombat->run(0.25f * (float)Date::ONE_KAMPFRUNDE);
		CPPUNIT_ASSERT_MESSAGE(" Ein Event sollte bei Gruppe 2 empfangen worden sein", mCombatTestResultGroup2->getLastEvent() != NULL);
		CPPUNIT_ASSERT_MESSAGE("Kein Event sollte bei Gruppe 1 empfangen worden sein", mCombatTestResultGroup1->getLastEvent() == NULL);
	}


	void setUp()
	{
		new ActionManager();
		new DsaManager();

		DsaManager::getSingleton()._addKampftechnik(new Kampftechnik("Raufen", "Raufen", 0));

		mCreature1 = new Creature("Creature1", "Creature 1"); //INI-Basis = 8
		// INI_BASIS = (2 * MU + IN + GE) / 5
		mCreature1->setEigenschaft(E_MUT, 10);
		mCreature1->setEigenschaft(E_INTUITION, 10);
		mCreature1->setEigenschaft(E_GEWANDTHEIT, 10);
		mCreature1->addKampftechnik("Raufen", make_pair(10, 10));

		mCreature2 = new Creature("Creature2", "Creature 2"); //INI-Basis = 1
		mCreature2->setEigenschaft(E_MUT, 1);
		mCreature2->setEigenschaft(E_INTUITION, 1);
		mCreature2->setEigenschaft(E_GEWANDTHEIT, 1);		
		mCreature2->addKampftechnik("Raufen", make_pair(10, 10));

		mCombat = new Combat();
		mCombat->add(mCreature1, TEST_OBJ_GROUP); mCombatTestResultGroup1 = new CombatTestResult(TEST_OBJ_GROUP);
		mCombat->add(mCreature2, TEST_OPP_GROUP); mCombatTestResultGroup2 = new CombatTestResult(TEST_OPP_GROUP);

		mCombat->addCombatEventListener(mCombatTestResultGroup1);
		mCombat->addCombatEventListener(mCombatTestResultGroup2);

		mCombat->start();
		mCombat->run(0.0);
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

		delete DsaManager::getSingletonPtr();
		delete ActionManager::getSingletonPtr();
	}

private:
	Combat* mCombat;
	CombatTestResult* mCombatTestResultGroup1;
	CombatTestResult* mCombatTestResultGroup2;
	Creature* mCreature1;
	Creature* mCreature2;
};


#endif
