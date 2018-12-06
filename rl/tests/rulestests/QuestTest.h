#ifndef __QUESTTEST_H__
#define __QUESTTEST_H__

#include "RulesTestPrerequisites.h"

#include "Quest.h"
#include "QuestChangeEvent.h"
#include "QuestBook.h"
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <vector>

using namespace CppUnit;
using namespace std;
using namespace rl;

class QuestTestListener : public QuestChangeListener {
public:
	void questKnownChanged(QuestChangeEvent* evt) {};
	void questPartsDoneChanged(QuestChangeEvent* evt) {};
	void questStateChanged(QuestChangeEvent* evt) {};
	void questSubquestAdded(QuestChangeEvent* evt) {};
};

class QuestTest : public TestFixture {
  CPPUNIT_TEST_SUITE( QuestTest );
  CPPUNIT_TEST( testGetQuestStateFromName );
  CPPUNIT_TEST( testAddSubQuest );
  CPPUNIT_TEST( testAutoComplete );
  CPPUNIT_TEST( testAutoCompleteParent1 );
  CPPUNIT_TEST( testAutoCompleteParent2 );
  CPPUNIT_TEST_SUITE_END();
private:
	QuestBook* mQuestBook;
	QuestTestListener* mQuestListener;

protected:
   void testGetQuestStateFromName()
   {
	   CPPUNIT_ASSERT_EQUAL_MESSAGE("OPEN", Quest::OPEN, Quest::getStateFromName("OPEN"));
	   CPPUNIT_ASSERT_EQUAL_MESSAGE("CLOSED", Quest::CLOSED, Quest::getStateFromName("CLOSED"));
	   CPPUNIT_ASSERT_EQUAL_MESSAGE("FAILED", Quest::FAILED, Quest::getStateFromName("FAILED"));
	   CPPUNIT_ASSERT_EQUAL_MESSAGE("COMPLETED", Quest::COMPLETED, Quest::getStateFromName("COMPLETED"));
	   CPPUNIT_ASSERT_EQUAL_MESSAGE("UNKNOWN", Quest::UNKNOWN, Quest::getStateFromName("UNKNOWN"));
   }

   void testAddSubQuest()
   {
	   Quest* testObj_Parent = new Quest("id1", "Name", "Description");
	   testObj_Parent->setQuestBook(mQuestBook);
	   Quest* testObj_Sub1 = new Quest("id2", "Name", "Description");
	   Quest* testObj_Sub2 = new Quest("id3", "Name", "Description");
	   testObj_Parent->addSubquest(testObj_Sub1);
	   testObj_Parent->addSubquest(testObj_Sub2);

	   CPPUNIT_ASSERT_EQUAL_MESSAGE("2 Kinder beim Parent-Quest", 2, testObj_Parent->getPartsToDo());
	   CPPUNIT_ASSERT_EQUAL_MESSAGE("Parent von Subquest1", testObj_Parent, testObj_Sub1->getParent());
	   CPPUNIT_ASSERT_EQUAL_MESSAGE("Parent von Subquest2", testObj_Parent, testObj_Sub2->getParent());
   }

   void testAutoComplete()
   {
	   Quest* testObj = new Quest("id1", "Name", "Description");
	   testObj->setQuestBook(mQuestBook);	   
	   testObj->setState(Quest::OPEN);
	   testObj->setPartsToDo(5);
	   testObj->setPartsDone(4);
	   CPPUNIT_ASSERT_EQUAL_MESSAGE("Quest sollte noch auf OPEN stehen", Quest::OPEN, testObj->getState());
	   testObj->setPartsDone(5);
	   CPPUNIT_ASSERT_EQUAL_MESSAGE("Quest sollte noch auf COMPLETED stehen", Quest::COMPLETED, testObj->getState());
   }

   void testAutoCompleteParent1()
   {
	   Quest* testObj_Parent = new Quest("id1", "Name", "Description");
	   testObj_Parent->setQuestBook(mQuestBook);
	   Quest* testObj_Sub1 = new Quest("id2", "Name", "Description");
	   Quest* testObj_Sub2 = new Quest("id3", "Name", "Description");
	   testObj_Parent->addSubquest(testObj_Sub1);
	   testObj_Parent->addSubquest(testObj_Sub2);

	   CPPUNIT_ASSERT_EQUAL_MESSAGE("Quest sollte noch auf UNKNOWN stehen", Quest::UNKNOWN, testObj_Parent->getState());

	   testObj_Sub1->setState(Quest::COMPLETED);
	   testObj_Sub2->setState(Quest::COMPLETED);
	   CPPUNIT_ASSERT_EQUAL_MESSAGE("Parent-Quest sollte auf COMPLETED stehen", Quest::COMPLETED, testObj_Parent->getState());

	   delete testObj_Parent;	   
	   delete testObj_Sub1;	   
	   delete testObj_Sub2;
   }

   void testAutoCompleteParent2()
   {
	   Quest* testObj_Parent = new Quest("id1", "Name", "Description");
	   testObj_Parent->setQuestBook(mQuestBook);
	   Quest* testObj_Sub1 = new Quest("id2", "Name", "Description");
	   Quest* testObj_Sub2 = new Quest("id3", "Name", "Description");
	   Quest* testObj_Sub2Sub1 = new Quest("id4", "Name", "Description");
	   Quest* testObj_Sub2Sub2 = new Quest("id5", "Name", "Description");
	   Quest* testObj_Sub2Sub3 = new Quest("id6", "Name", "Description");
	   testObj_Parent->addSubquest(testObj_Sub1);
	   testObj_Parent->addSubquest(testObj_Sub2);
	   testObj_Sub2->addSubquest(testObj_Sub2Sub1);
	   testObj_Sub2->addSubquest(testObj_Sub2Sub2);
	   testObj_Sub2->addSubquest(testObj_Sub2Sub3);

	   CPPUNIT_ASSERT_EQUAL_MESSAGE("2 Kinder beim Parent-Quest", 2, testObj_Parent->getPartsToDo());
	   CPPUNIT_ASSERT_EQUAL_MESSAGE("Quest sollte noch auf UNKNOWN stehen", Quest::UNKNOWN, testObj_Parent->getState());

	   testObj_Sub1->setState(Quest::COMPLETED);
	   testObj_Sub2Sub1->setState(Quest::COMPLETED);
	   testObj_Sub2Sub2->setState(Quest::COMPLETED);
	   testObj_Sub2Sub3->setState(Quest::COMPLETED);
	   CPPUNIT_ASSERT_EQUAL_MESSAGE("Sub2-Quest sollte auf COMPLETED stehen", Quest::COMPLETED, testObj_Sub2->getState());
	   CPPUNIT_ASSERT_EQUAL_MESSAGE("Parent-Quest sollte auf COMPLETED stehen", Quest::COMPLETED, testObj_Parent->getState());

	   delete testObj_Parent;	   
	   delete testObj_Sub1;	   
	   delete testObj_Sub2;
   }

public:
   void setUp()
   {
	   mQuestBook = new QuestBook();
	   mQuestListener = new QuestTestListener();
	   mQuestBook->addQuestChangeListener(mQuestListener);
   }

   void tearDown()
   {
	   mQuestBook->removeQuestChangeListener(mQuestListener);
	   delete mQuestListener;
	   delete mQuestBook;
   }
};

#endif
