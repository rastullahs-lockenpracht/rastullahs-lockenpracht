#ifndef __FPSOverlay_H__
#define __FPSOverlay_H__

#include <OgreFrameListener.h>
#include <OgreOverlay.h>

#include "UiPrerequisites.h"

namespace rl {

class _RlUiExport FPSOverlay: public FrameListener
{
private:
    void updateStats(void);
    
public:
    // Constructor takes a RenderWindow because it uses that to determine input context
    FPSOverlay(RenderWindow* win);
    virtual ~FPSOverlay();

    void showDebugOverlay(bool show);

    // Override frameEnded event 
    bool frameEnded(const FrameEvent& evt);

protected:
    RenderWindow* mWindow;
    bool mStatsOn;
};

}
#endif
