#include "Console.h"

#include <OgreEventQueue.h>
#include <OgreGuiElement.h>
#include <OgreGuiContainer.h>
#include <OgreGuiManager.h>
#include <OgreInput.h>
#include <OgreKeyEvent.h>
#include <OgreOverlay.h>
#include <OgreOverlayManager.h>
#include <OgrePanelGuiElement.h>
#include <OgreTextAreaGuiElement.h>
#include <OgreLogManager.h>

#include "UiSubsystem.h"


template<> rl::Console* Singleton<rl::Console>::ms_Singleton = 0;

namespace rl {

   Console& Console::getSingleton(void)
    {
        return Singleton<Console>::getSingleton();
    }
	Console* Console::getSingletonPtr(void)
    {
        return Singleton<Console>::getSingletonPtr();
    }

	Console::Console(void)
	{
		mState = CS_CLOSED;
		static_cast<Overlay*>(OverlayManager::getSingleton().getByName("console"))->setScroll(0, 1);
		static_cast<Overlay*>(OverlayManager::getSingleton().getByName("console"))->show();

		int i;
		for(i=0; i<getNumLines(); i++)
		{
			char buf[128];
			sprintf(buf, "console/textline%d", i);
			mTextLine[i] = static_cast<TextAreaGuiElement*>(GuiManager::getSingleton().createGuiElement("TextArea", buf));
			mTextLine[i]->setMaterialName("textline");
			mTextLine[i]->setVerticalAlignment(GVA_BOTTOM);
			mTextLine[i]->setFontName("TrebuchetMSBold");
			mTextLine[i]->setMetricsMode(GMM_PIXELS);
			mTextLine[i]->setLeft(BORDER_PADDING);
			mTextLine[i]->setCharHeight(TEXT_HEIGHT);
			mTextLine[i]->setColour(ColourValue::White);
			mTextLine[i]->setTop(-TEXT_HEIGHT-BORDER_PADDING - i*TEXT_HEIGHT);
			mTextLine[i]->setCaption("");

			static_cast<PanelGuiElement*>(GuiManager::getSingleton().getGuiElement("console/panel"))->addChild(static_cast<GuiElement*>(mTextLine[i]));
		}

		mTextLine[0]->setColour(ColourValue(0.5, 1, 0.3));

		mCursorPos = 0;
		mShiftDown = false;
		mCapsDown = false;
		mAltDown = false;

		setLineText(0, ">>> ");

		mInterpreter = NULL;

		mPrompt = false;

		mHistory.push_front("");
		mHistoryIterator = mHistory.begin();
	}

	Console::~Console()
	{
		int i;
		for(i=0; i<getNumLines(); i++)
		{
			char buf[128];
			sprintf(buf, "console/textline%d", i);
			static_cast<PanelGuiElement*>(GuiManager::getSingleton().getGuiElement("console/panel"))->removeChild(buf);

			GuiManager::getSingleton().destroyGuiElement(buf);
		}

		static_cast<Overlay*>(OverlayManager::getSingleton().getByName("console"))->hide();
	}

	void Console::setLineText(int lineNum, String text)
	{
		mTextLine[lineNum]->setCaption(text);
	}

	String Console::getLineText(int lineNum) const
	{
		return mTextLine[lineNum]->getCaption();
	}

	void Console::scrollUp(void)
	{
		int i;
		for(i=getNumLines()-1; i>1; i--)
			setLineText(i, getLineText(i-1));
		setLineText(1, "");
	}

	void Console::open(void)
	{
		static_cast<Overlay*>(OverlayManager::getSingleton().getByName("console"))->show();
		mState = CS_OPENING;
	}

	void Console::close(void)
	{
		mState = CS_CLOSING;
	}

    bool Console::frameEnded(const FrameEvent & event)
    {
        return !UiSubsystem::getSingleton().isRequestingExit();
    }

	bool Console::frameStarted(const FrameEvent & event)
	{
		if(mState == CS_OPENING)
		{
			static_cast<Overlay*>(OverlayManager::getSingleton().getByName("console"))->scroll(0, -2*event.timeSinceLastFrame);
			if(static_cast<Overlay*>(OverlayManager::getSingleton().getByName("console"))->getScrollY() <= 0)
			{
				static_cast<Overlay*>(OverlayManager::getSingleton().getByName("console"))->setScroll(0, 0);
				mState = CS_OPEN;
			}
		}
		else if(mState == CS_CLOSING)
		{
			static_cast<Overlay*>(OverlayManager::getSingleton().getByName("console"))->scroll(0, 2*event.timeSinceLastFrame);
			if(static_cast<Overlay*>(OverlayManager::getSingleton().getByName("console"))->getScrollY() >= 1)
			{
				static_cast<Overlay*>(OverlayManager::getSingleton().getByName("console"))->setScroll(0, 1);
				mState = CS_CLOSED;
				static_cast<Overlay*>(OverlayManager::getSingleton().getByName("console"))->hide();
			}
		}

		return true;
	}

	int Console::getNumLines(void) const
	{
		return ((Root::getSingleton().getAutoCreatedWindow()->getHeight() / 2) - BORDER_PADDING) / TEXT_HEIGHT;
	}

	void Console::keyPressed(KeyEvent* e)
	{
		if(e->getKey() == KC_LSHIFT || e->getKey() == KC_RSHIFT  )
			mShiftDown = true;
		
		if(e->getKey() == KC_CAPITAL)
			mCapsDown = !mCapsDown;

		if(e->getKey() == KC_LMENU || e->getKey() == KC_RMENU  )
			mAltDown = true;

		if(mState != CS_CLOSED)
		{
			char keyChar = e->getKeyChar();
			char c[2];
			c[1] = 0;

			if( ( keyChar >= 'a' && keyChar <= 'z' ) )
			{
				c[0] = keyChar;

				if( keyChar == 'z' )
					c[0] = 'y';
				else if( keyChar == 'y' )
					c[0] = 'z';

				if(mShiftDown ^ mCapsDown)
					c[0] -= 'a' - 'A';

				if( keyChar = 'q' && (!mShiftDown && mAltDown))
					c[0] = '@';
				else if( keyChar = 'e' && (!mShiftDown && mAltDown))
					c[0] = '?';
				else if( keyChar = 'm' && (!mShiftDown && mAltDown))
					c[0] = '?';

				mCommandLine.insert(mCursorPos, c);
				mCursorPos++;
			}
			//Kleiner Gr??er
			else if (   e->getKey() == 86   )
			{
				if(!mShiftDown && mAltDown)
					c[0] = '|';	
				else if(mShiftDown && !mAltDown)
					c[0] = '>';
				else
					c[0] = '<';

				mCommandLine.insert(mCursorPos, c);
				mCursorPos++;
			}
			else if(keyChar != 0 )
			{
				//Normal
				switch( e->getKey() ) 
				{
					case KC_SLASH :
						c[0] = '-';break;
					case KC_MINUS :
						c[0] = '?';break;
					case KC_EQUALS :
						c[0] = '?';break;
					case KC_SEMICOLON :
						c[0] = '?';break; 
					case KC_APOSTROPHE :
						c[0] = '?';break;
					case KC_GRAVE :
						c[0] = '^';break;
					case KC_COMMA  :
						c[0] = ',';break;	
					case KC_PERIOD  :
						c[0] = '.';break;	
					case KC_MULTIPLY :
						c[0] = '*';break;
					case KC_LBRACKET  :
						c[0] = '?';break;	
					case KC_RBRACKET :
						c[0] = '+';break;
					case KC_BACKSLASH :
						c[0] = '#';break;
					default :
						c[0] = keyChar;
				}


				//Mit Shift
				if(mShiftDown && !mAltDown)
				{
					switch( e->getKey() ) 
					{
						case KC_1 :
							c[0] = '!';break;
						case KC_2 :
							c[0] = '"';break;
						case KC_3 :
							c[0] = '?';break;
						case KC_4 :
							c[0] = '$';break;
						case KC_5 :
							c[0] = '%';break;
						case KC_6 :
							c[0] = '&';break;
						case KC_7 :
							c[0] = '/';break;
						case KC_8 :
							c[0] = '(';break;
						case KC_9 :
							c[0] = ')';break;
						case KC_0 :
							c[0] = '=';break;
						case KC_SLASH :
							c[0] = '_';break;
						case KC_MINUS :
							c[0] = '?';break;
						case KC_EQUALS :
							c[0] = '`';break;
						case KC_SEMICOLON :
							c[0] = '?';break; 
						case KC_APOSTROPHE :
							c[0] = '?';break;
						case KC_GRAVE :
							c[0] = '?';break;
						case KC_COMMA  :
							c[0] = ';';break;	
						case KC_PERIOD  :
							c[0] = ':';break;
						case KC_LBRACKET  :
							c[0] = '?';break;	
						case KC_RBRACKET :
							c[0] = '*';break;
						case KC_BACKSLASH :
							c[0] = '\'';break;
					}
				}

				if(!mShiftDown && mAltDown)
				{
					switch( e->getKey() ) 
					{
						case KC_2 :
							c[0] = '?';break;
						case KC_3 :
							c[0] = '?';break;
						case KC_7 :
							c[0] = '{';break;
						case KC_8 :
							c[0] = '[';break;
						case KC_9 :
							c[0] = ']';break;
						case KC_0 :
							c[0] = '}';break;
						case KC_MINUS :
							c[0] = '\\';break;
						case KC_RBRACKET :
							c[0] = '~';break;
					}
				}

				mCommandLine.insert(mCursorPos, c);
				mCursorPos++;
			}
			else if(e->getKey() == KC_BACK)
			{
				if(mCursorPos > 0)
				{
					mCursorPos--;
					mCommandLine.erase(mCursorPos, 1);
				}
			}
			else if(e->getKey() == KC_UP)
			{
				if (mHistoryIterator == mHistory.end()){
					mHistoryIterator=mHistory.begin();
				}
				mCommandLine=(*mHistoryIterator);
				mCursorPos=mCommandLine.length();
				mHistoryIterator++;
			}
			else if(e->getKey() == KC_DOWN)
			{
				if (mHistoryIterator == mHistory.begin()){
					mHistoryIterator=mHistory.end();
				}
				mHistoryIterator--;
				mCommandLine=(*mHistoryIterator);
				mCursorPos=mCommandLine.length();
			}

			if(e->getKey() == KC_RETURN)
			{
				if(!mPrompt)
					write(">>> " + mCommandLine + "\n");
				else
					write("... " + mCommandLine + "\n");

				if (mCommandLine.length()>0){
					mHistory.push_front(mCommandLine);
					mHistoryIterator=mHistory.begin();
				}

				if(mInterpreter != 0)
					mPrompt = mInterpreter->execute(mCommandLine);

				if(!mPrompt)
					setLineText(0, ">>> _");
				else
					setLineText(0, "... _");
				mCommandLine = "";
				mCursorPos = 0;
			}
			else
			{
				if(!mPrompt)
					setLineText(0, ">>> " + mCommandLine + "_");
				else
					setLineText(0, "... " + mCommandLine + "_");
			}
		}

	}

	void Console::keyReleased(KeyEvent* e)
	{
		if(e->getKey() == KC_LSHIFT || e->getKey() == KC_RSHIFT)
			mShiftDown = false;
		
		if(e->getKey() == KC_LMENU || e->getKey() == KC_RMENU  )
			mAltDown = false;

		if(e->getKey() == KC_F12 || e->getKey() == KC_TAB )
		{
			if(mState == CS_OPEN || mState == CS_OPENING)
				close();
			else
				open();
		}
        
        // Beende das Programm.
        if (e->getKey() == KC_ESCAPE)
        {
            UiSubsystem::getSingleton().requestExit();
        }
	}

	void Console::write(String output)
	{
		static bool lineStart = true;
		unsigned int i;

		for(i=0; i<output.size(); i++)
		{
			if(output.at(i) == '\n')
			{
				if(lineStart == true)
					scrollUp();
				else
					lineStart=true;
			}
			else
			{
				if(lineStart == true)
				{
					scrollUp();
				}
				char c[2];
				c[0] = output.at(i);
				c[1] = 0;
				setLineText(1, getLineText(1) + String(c));
				lineStart=false;
			}
		}

		//std::cout << output;
		LogManager::getSingleton().logMessage(output);
	}



}
