#ifndef __Console_H__
#define __Console_H__

#include <list>

#include <OgreSingleton.h>
#include <OgreTextAreaGuiElement.h>
#include <OgreEventListeners.h>
#include <OgreLogManager.h>

#include "UiPrerequisites.h"
#include "Interpreter.h"

namespace rl {

	/** This is a Console System
	@remarks It deals with german KeyInputs, extends a KeyListener and a Framelistener
		and follow the Singleton pattern, for only one Console will exist in the game.
		Its quite hacky....
	*/
	class _RlUiExport Console : public KeyListener, public FrameListener, public Singleton<Console>
	{
		public:
			~Console();
			Console(void);
			static Console& getSingleton(void);
			static Console* getSingletonPtr(void);

			void setInterpreter(Interpreter* pInterpreter)
			{
				mInterpreter = pInterpreter;
			}

			Interpreter* getInterpreter()
			{
				return mInterpreter;
			}

			void write(String output);

			bool isOpen(void) const { if(mState==CS_OPEN) return true; return false; };
			bool isClosed(void) const { if(mState==CS_CLOSED) return true; return false; };
				
			void keyClicked(Ogre::KeyEvent* e) {};
			void keyPressed(Ogre::KeyEvent* e);
			void keyReleased(Ogre::KeyEvent* e);

			bool frameStarted(const Ogre::FrameEvent & event);
            bool frameEnded(const Ogre::FrameEvent & event);

			void open(void);
			void close(void);

		private:
			enum State { CS_OPEN, CS_CLOSED, CS_OPENING, CS_CLOSING };
			State mState;

			int getNumLines(void) const;
			void setLineText(int lineNum, String text);
			String getLineText(int lineNum) const;

			void scrollUp(void);

			enum { TEXT_HEIGHT=20 };
			enum { BORDER_PADDING=5 };
			enum { MAX_TEXT_LINES=50 };

			TextAreaGuiElement* mTextLine[MAX_TEXT_LINES];

			String mCommandLine;
			int mCursorPos;
			bool mShiftDown;
			bool mCapsDown;
			bool mAltDown;

			Interpreter* mInterpreter;

			bool mPrompt;
			std::list<String> mHistory;
			std::list<String>::iterator mHistoryIterator;
	};

}
#endif
