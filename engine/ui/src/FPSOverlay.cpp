#include "FPSOverlay.h"

#include <OgreGuiManager.h>
#include <OgreGuiElement.h>
#include <OgreOverlay.h>
#include <OgreOverlayManager.h>
#include <OgreRenderWindow.h>
#include <OgreStringConverter.h>
#include <OgreTextAreaGuiElement.h>

namespace rl {

void FPSOverlay::updateStats(void)
{
        static String currFps = "Current FPS: ";
        static String avgFps = "Average FPS: ";
        static String bestFps = "Best FPS: ";
        static String worstFps = "Worst FPS: ";
        static String tris = "Triangle Count: ";

        // update stats when necessary
        GuiElement* guiAvg = GuiManager::getSingleton().getGuiElement("Core/AverageFps");
        GuiElement* guiCurr = GuiManager::getSingleton().getGuiElement("Core/CurrFps");
        GuiElement* guiBest = GuiManager::getSingleton().getGuiElement("Core/BestFps");
        GuiElement* guiWorst = GuiManager::getSingleton().getGuiElement("Core/WorstFps");
        
        guiAvg->setCaption(avgFps + StringConverter::toString(mWindow->getAverageFPS()));
        guiCurr->setCaption(currFps + StringConverter::toString(mWindow->getLastFPS()));
        guiBest->setCaption(bestFps + StringConverter::toString(mWindow->getBestFPS())
            +" "+StringConverter::toString(mWindow->getBestFrameTime())+" ms");
        guiWorst->setCaption(worstFps + StringConverter::toString(mWindow->getWorstFPS())
            +" "+StringConverter::toString(mWindow->getWorstFrameTime())+" ms");
            
        GuiElement* guiTris = GuiManager::getSingleton().getGuiElement("Core/NumTris");
        guiTris->setCaption(tris + StringConverter::toString(mWindow->getTriangleCount()));

        GuiElement* guiDbg = GuiManager::getSingleton().getGuiElement("Core/DebugText");
        guiDbg->setCaption(mWindow->getDebugText());
 }
    
FPSOverlay::FPSOverlay(RenderWindow* win)
{
    mWindow = win;
    mStatsOn = true;

    Ogre::Root::getSingleton().addFrameListener(this);
	showDebugOverlay(true);
}

FPSOverlay::~FPSOverlay()
{
    Ogre::Root::getSingleton().removeFrameListener(this);
}


void FPSOverlay::showDebugOverlay(bool show)
{   
        Overlay* o = (Overlay*)OverlayManager::getSingleton().getByName("Core/DebugOverlay");

        if (!o)
            Except( Exception::ERR_ITEM_NOT_FOUND, "Could not find overlay Core/DebugOverlay",
                "showDebugOverlay" );

        if (show)
        {
            o->show();
        }
        else
        {
            o->hide();
        }
}

// Override frameEnded event 
bool FPSOverlay::frameEnded(const FrameEvent& evt)
{
        updateStats();
		return true;
}

}
