/************************************************************************
	filename: 	RLVUMeter.h
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
#ifndef _RLVUMeter_h_
#define _RLVUMeter_h_

#include "RLModule.h"
#include "CEGUIWindowFactory.h"
#include "elements/CEGUIProgressBar.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Vertical "VU Meter" style progress bar for the Taharez Gui Scheme.
*/
class RASTULLAHLOOK_API RLVUMeter : public ProgressBar
{
	// simple struct used to hold info about a light.
	struct LightInfo
	{
		Rect	area;
		colour	col;
	};

public:
	/*************************************************************************
		Constants
	*************************************************************************/
	// type name for this widget
	static const utf8	WidgetTypeName[];				//!< The unique typename of this widget

	// Progress bar image names
	static const utf8	ImagesetName[];					//!< Name of the imageset to use for rendering.
	static const utf8	LightImageName[];				//!< Name of image for lights.

	// Colours
	static const colour	redLitColour;			//!< Base colour to use for red segments when lit.
	static const colour	redUnlitColour;			//!< Base colour to use for red segments when not lit.
	static const colour	yellowLitColour;		//!< Base colour to use for yellow segments when lit.
	static const colour	yellowUnlitColour;		//!< Base colour to use for yellow segments when not lit.
	static const colour	greenLitColour;			//!< Base colour to use for green segments when lit.
	static const colour	greenUnlitColour;		//!< Base colour to use for green segments when not lit.

	/*************************************************************************
		Construction / Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for Taharez RLVUMeter objects
	*/
	RLVUMeter(const String& type, const String& name);


	/*!
	\brief
		Destructor for Taharez RLVUMeter objects
	*/
	virtual ~RLVUMeter(void);


protected:
	/*************************************************************************
		Overridden Implementation Rendering Functions
	*************************************************************************/
	/*!
	\brief
		Perform rendering for this widget
	*/
	virtual void	drawSelf(float z);


	/*************************************************************************
		Overridden Event Handling Functions
	*************************************************************************/
	/*!
	\brief
		handler called when widget is sized
	*/
	virtual void	onSized(WindowEventArgs& e);

	virtual void	onAlphaChanged(WindowEventArgs& e);
	virtual void	onProgressChanged(WindowEventArgs& e);


	/*************************************************************************
		Implementation Functions
	*************************************************************************/
	/*!
	\brief
		function to initialise data relating to lights that should be rendered.
	*/
	void	setupLightsInfo(void);


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	// images
	const Image*	d_lightImage;	//!< Image to use for lights.

	// lights information
	int			d_lightCount;	//!< number of LED segments to draw from d_lightInfos.
	int			d_lightInfoCount;	//!< Size of the d_lightInfos array.
	LightInfo*	d_lightInfos;	//!< Array of LightInfo structs.
};


/*!
\brief
	Factory class for producing RLVUMeter objects
*/
class RASTULLAHLOOK_API RLVUMeterFactory : public WindowFactory
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	RLVUMeterFactory(void) : WindowFactory(RLVUMeter::WidgetTypeName) { }
	~RLVUMeterFactory(void){}


	/*!
	\brief
		Create a new Window object of whatever type this WindowFactory produces.

	\param name
		A unique name that is to be assigned to the newly created Window object

	\return
		Pointer to the new Window object.
	*/
	Window*	createWindow(const String& name);


	/*!
	\brief
		Destroys the given Window object.

	\param window
		Pointer to the Window object to be destroyed.

	\return
		Nothing.
	*/
	virtual void	destroyWindow(Window* window)	 { if (window->getType() == d_type) delete window; }
};


} // End of  CEGUI namespace section


#endif	// end of guard _RLVUMeter_h_
