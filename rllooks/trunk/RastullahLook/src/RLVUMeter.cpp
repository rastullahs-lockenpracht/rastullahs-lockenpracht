/************************************************************************
	filename: 	RLVUMeter.cpp
	created:	20/1/2005
	author:		Paul D Turner
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#include "RLVUMeter.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"


// Start of CEGUI namespace section
namespace CEGUI
{
	/*************************************************************************
		Constants
	*************************************************************************/
	// type name for this widget
	const utf8	RLVUMeter::WidgetTypeName[] = "RastullahLook/VUMeter";

	// Progress bar image names
	const utf8	RLVUMeter::ImagesetName[]	= "RastullahLook";
	const utf8	RLVUMeter::LightImageName[] = "MultiLineEditboxSelectionBrush";

	// Colours
	const colour RLVUMeter::redLitColour(0.918f, 0.0f, 0.0f);
	const colour RLVUMeter::redUnlitColour(0.453f, 0.0f, 0.0f);
	const colour RLVUMeter::yellowLitColour(0.918f, 0.918f, 0.0f);
	const colour RLVUMeter::yellowUnlitColour(0.453f, 0.453f, 0.0f);
	const colour RLVUMeter::greenLitColour(0.0f, 0.918f, 0.0f);
	const colour RLVUMeter::greenUnlitColour(0.0f, 0.453f, 0.0f);


	/*************************************************************************
		Constructor for Rastullah RLVUMeter objects
	*************************************************************************/
	RLVUMeter::RLVUMeter(const String& type, const String& name) :
		ProgressBar(type, name),
		d_lightCount(0),
		d_lightInfoCount(0),
		d_lightInfos(0)
	{
		// cache images
		Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);
		d_lightImage = &iset->getImage(LightImageName);

		setupLightsInfo();
	}


	/*************************************************************************
		Destructor for Rastullah RLVUMeter objects
	*************************************************************************/
	RLVUMeter::~RLVUMeter(void)
	{
		if (d_lightInfos != 0)
		{
			delete[] d_lightInfos;
		}
	}


	/*************************************************************************
		function to initialise data relating to lights that should be rendered.	
	*************************************************************************/
	void RLVUMeter::setupLightsInfo(void)
	{
		Rect absrect(getUnclippedPixelRect());

		//
		// calculate number of lights to have
		//
		d_lightCount = static_cast<int>(absrect.getHeight() / 3.0f);

		// re-allocate buffer if needed
		if (d_lightCount > d_lightInfoCount)
		{
			if (d_lightInfos != 0)
			{
				delete[] d_lightInfos;
			}

			d_lightInfoCount = d_lightCount;
			d_lightInfos = new LightInfo[d_lightInfoCount];
		}

		//
		// Update LightInfo structures for current widget state
		//
		Rect lightArea(0, 0, getUnclippedPixelRect().getWidth(), 2);

		for (int currLight = 0; currLight < d_lightCount; ++currLight)
		{
			d_lightInfos[currLight].area = lightArea;
			lightArea.offset(Point(0.0f, 3.0f));

			float lpos = 1.0f - static_cast<float>(currLight) / static_cast<float>(d_lightCount);

			// red
			if (lpos >= 0.8f)
			{
				if (d_progress >= lpos)
				{
					d_lightInfos[currLight].col = redLitColour;
				}
				else
				{
					d_lightInfos[currLight].col = redUnlitColour;
				}

			}
			// yellow
			else if (lpos >= 0.66f)
			{
				if (d_progress >= lpos)
				{
					d_lightInfos[currLight].col = yellowLitColour;
				}
				else
				{
					d_lightInfos[currLight].col = yellowUnlitColour;
				}

			}
			// green
			else
			{
				if (d_progress >= lpos)
				{
					d_lightInfos[currLight].col = greenLitColour;
				}
				else
				{
					d_lightInfos[currLight].col = greenUnlitColour;
				}

			}

			// set alpha
			d_lightInfos[currLight].col.setAlpha(getEffectiveAlpha());
		}

	}


	/*************************************************************************
		Perform rendering for this widget
	*************************************************************************/
	void RLVUMeter::drawSelf(float z)
	{
		Rect clipper(getPixelRect());

		// do nothing if the widget is totally clipped.
		if (clipper.getWidth() == 0)
		{
			return;
		}

		Point wndPos(getUnclippedPixelRect().getPosition());
		Rect screenArea;

		// render the lights
		for (int currLight = 0; currLight < d_lightCount; ++currLight)
		{
			screenArea = d_lightInfos[currLight].area;
			screenArea.offset(wndPos);
			d_lightImage->draw(screenArea, z, clipper, ColourRect(d_lightInfos[currLight].col));
		}

	}


	/*************************************************************************
		handler called when widget is sized
	*************************************************************************/
	void RLVUMeter::onSized(WindowEventArgs& e)
	{
		// default handling
		ProgressBar::onSized(e);

		// layout lights again
		setupLightsInfo();
	}

	void RLVUMeter::onAlphaChanged(WindowEventArgs& e)
	{
		// default handling
		ProgressBar::onAlphaChanged(e);

		setupLightsInfo();
	}

	void RLVUMeter::onProgressChanged(WindowEventArgs& e)
	{
		// default handling
		ProgressBar::onProgressChanged(e);

		setupLightsInfo();
	}


	//////////////////////////////////////////////////////////////////////////
	/*************************************************************************

		Factory Methods

	*************************************************************************/
	//////////////////////////////////////////////////////////////////////////
	/*************************************************************************
		Create, initialise and return a RLVUMeter
	*************************************************************************/
	Window* RLVUMeterFactory::createWindow(const String& name)
	{
		RLVUMeter* wnd = new RLVUMeter(d_type, name);
		wnd->initialise();

		return wnd;
	}


} // End of  CEGUI namespace section
