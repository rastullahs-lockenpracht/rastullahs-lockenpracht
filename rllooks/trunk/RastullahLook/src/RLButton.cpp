/************************************************************************
    filename:   RLButton.cpp
    created:    11/5/2004
    author:     Paul D Turner
    
    purpose:    Implementation of Taharez look Button widget.
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://crayzedsgui.sourceforge.net)
    Copyright (C)2004 Paul D Turner (crayzed@users.sourceforge.net)

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
#include "RLButton.h"
#include "CEGUIImageset.h"
#include "CEGUIFont.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
    Constants
*************************************************************************/
// type name for this widget
const utf8  RLButton::WidgetTypeName[]              = "TaharezLook/Button";

const utf8  RLButton::ImagesetName[]                = "TaharezLook";
const utf8  RLButton::LeftNormalImageName[]         = "ButtonLeftNormal";
const utf8  RLButton::MiddleNormalImageName[]       = "ButtonMiddleNormal";
const utf8  RLButton::RightNormalImageName[]        = "ButtonRightNormal";
const utf8  RLButton::LeftHighlightImageName[]      = "ButtonLeftHighlight";
const utf8  RLButton::MiddleHighlightImageName[]    = "ButtonMiddleHighlight";
const utf8  RLButton::RightHighlightImageName[]     = "ButtonRightHighlight";
const utf8  RLButton::LeftPushedImageName[]         = "ButtonLeftPushed";
const utf8  RLButton::MiddlePushedImageName[]       = "ButtonMiddlePushed";
const utf8  RLButton::RightPushedImageName[]        = "ButtonRightPushed";
const utf8  RLButton::MouseCursorImageName[]        = "MouseArrow";

RLButtonProperties::NormalImage RLButton::d_normalImageProperty;
RLButtonProperties::PushedImage RLButton::d_pushedImageProperty;
RLButtonProperties::HoverImage  RLButton::d_hoverImageProperty;
RLButtonProperties::UseStandardImagery RLButton::d_useStandardImageryProperty;
RLButtonProperties::TextXOffset RLButton::d_textXOffsetProperty;


/*************************************************************************
    Constructor
*************************************************************************/
RLButton::RLButton(const String& type, const String& name) :
    PushButton(type, name)
{
    Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

    // default options
    d_autoscaleImages       = true;
    d_useStandardImagery    = true;
    d_useNormalImage        = false;
    d_useHoverImage         = false;
    d_usePushedImage        = false;
    d_useDisabledImage      = false;

    // setup cache of image pointers
    d_leftSectionNormal     = &iset->getImage(LeftNormalImageName);
    d_middleSectionNormal   = &iset->getImage(MiddleNormalImageName);
    d_rightSectionNormal    = &iset->getImage(RightNormalImageName);

    d_leftSectionHover      = &iset->getImage(LeftHighlightImageName);
    d_middleSectionHover    = &iset->getImage(MiddleHighlightImageName);
    d_rightSectionHover     = &iset->getImage(RightHighlightImageName);

    d_leftSectionPushed     = &iset->getImage(LeftPushedImageName);
    d_middleSectionPushed   = &iset->getImage(MiddlePushedImageName);
    d_rightSectionPushed    = &iset->getImage(RightPushedImageName);

    d_textXOffset = 0.0f;

    setMouseCursor(&iset->getImage(MouseCursorImageName));
    setNormalTextColour( colour( 0.33f,0.25f,0.094f ) );
    setHoverTextColour( colour( 0.43f,0.35f,0.194f ) );
    setPushedTextColour( colour( 0.27f,0.20f,0.044f ) );

    addRLButtonProperties();
}


/*************************************************************************
    Destructor
*************************************************************************/
RLButton::~RLButton(void)
{
}



/*************************************************************************
    set whether or not to render the standard imagery for the button    
*************************************************************************/
void RLButton::setStandardImageryEnabled(bool setting)
{
    if (d_useStandardImagery != setting)
    {
        d_useStandardImagery = setting;
        requestRedraw();
    }

}


/*************************************************************************
    set the image to render for the button (normal state) - use NULL to
    disable drawing of image    
*************************************************************************/
void RLButton::setNormalImage(const RenderableImage* image)
{
    if (image == NULL)
    {
        d_useNormalImage = false;
    }
    else
    {
        d_useNormalImage = true;
        d_normalImage = *image;
        d_normalImage.setRect(Rect(0, 0, d_abs_area.getWidth(), d_abs_area.getHeight()));
    }

    requestRedraw();
}


/*************************************************************************
    set the image to render for the button (hover state)  - use NULL to
    disable drawing of image
*************************************************************************/
void RLButton::setHoverImage(const RenderableImage* image)
{
    if (image == NULL)
    {
        d_useHoverImage = false;
    }
    else
    {
        d_useHoverImage = true;
        d_hoverImage = *image;
        d_hoverImage.setRect(Rect(0, 0, d_abs_area.getWidth(), d_abs_area.getHeight()));
    }

    requestRedraw();
}


/*************************************************************************
    set the image to render for the button (pushed state)  - use NULL to
    disable drawing of image    
*************************************************************************/
void RLButton::setPushedImage(const RenderableImage* image)
{
    if (image == NULL)
    {
        d_usePushedImage = false;
    }
    else
    {
        d_usePushedImage = true;
        d_pushedImage = *image;
        d_pushedImage.setRect(Rect(0, 0, d_abs_area.getWidth(), d_abs_area.getHeight()));
    }

    requestRedraw();
}


/*************************************************************************
    set the image to render for the button (disabled state)  - use NULL
    to disable drawing of image 
*************************************************************************/
void RLButton::setDisabledImage(const RenderableImage* image)
{
    if (image == NULL)
    {
        d_useDisabledImage = false;
    }
    else
    {
        d_useDisabledImage = true;
        d_disabledImage = *image;
        d_disabledImage.setRect(Rect(0, 0, d_abs_area.getWidth(), d_abs_area.getHeight()));
    }

    requestRedraw();
}

float RLButton::getTextXOffset() const
{
   return d_textXOffset;
}

void RLButton::setTextXOffset(float offset)
{
   d_textXOffset = offset;
}


/*************************************************************************
    render Widget in normal state   
*************************************************************************/
void RLButton::drawNormal(float z)
{
    Rect clipper(getPixelRect());

    // do nothing if the widget is totally clipped.
    if (clipper.getWidth() == 0)
    {
        return;
    }

    // get the destination screen rect for this window
    Rect absrect(getUnclippedPixelRect());

    // calculate colours to use.
    float alpha_comp = getEffectiveAlpha();
    ColourRect colours(colour(1, 1, 1, alpha_comp));

    // render standard button imagery if required.
    if (d_useStandardImagery)
    {
        // calculate widths for the button segments
        float leftWidth     = d_leftSectionNormal->getWidth();
        float rightWidth    = d_rightSectionNormal->getWidth();
        float midWidth      = absrect.getWidth() - leftWidth - rightWidth;

        //
        // draw the images
        //
        Vector3 pos(absrect.d_left, absrect.d_top, z);
        Size    sz(leftWidth, absrect.getHeight());
        d_leftSectionNormal->draw(pos, sz, clipper, colours);

        pos.d_x = absrect.d_right - rightWidth;
        sz.d_width = rightWidth;
        d_rightSectionNormal->draw(pos, sz, clipper, colours);

        pos.d_x = absrect.d_left + leftWidth;
        sz.d_width = midWidth;
        d_middleSectionNormal->draw(pos, sz, clipper, colours);
    }

    // render clients custom image if that is required.
    if (d_useNormalImage)
    {
        colours = d_normalImage.getColours();
        colours.setAlpha(alpha_comp);
        d_normalImage.setColours(colours);
        Vector3 imgpos(absrect.d_left, absrect.d_top, System::getSingleton().getRenderer()->getZLayer(1));
        d_normalImage.draw(imgpos, clipper);
    }

    //
    // Draw label text
    //
	//absrect.d_top += (absrect.getHeight() - getFont()->getL->getLineSpacing()) * 0.5f;
    absrect.d_top += (absrect.getHeight() - getFont()->getLineSpacing()) * 0.35f;
    absrect.d_left += d_textXOffset * absrect.getWidth();
    colours.setColours(d_normalColour);
    colours.setAlpha(alpha_comp);
    getFont()->drawText(getText(), absrect, System::getSingleton().getRenderer()->getZLayer(2), clipper, Centred, colours);
}


/*************************************************************************
    render Widget in hover / highlight state    
*************************************************************************/
void RLButton::drawHover(float z)
{
    Rect clipper(getPixelRect());

    // do nothing if the widget is totally clipped.
    if (clipper.getWidth() == 0)
    {
        return;
    }

    // get the destination screen rect for this window
    Rect absrect(getUnclippedPixelRect());

    // calculate colours to use.
    float alpha_comp = getEffectiveAlpha();
    ColourRect colours(colour(1, 1, 1, alpha_comp));

    // render standard button imagery if required.
    if (d_useStandardImagery)
    {
        // calculate widths for the button segments
        float leftWidth     = d_leftSectionHover->getWidth();
        float rightWidth    = d_rightSectionHover->getWidth();
        float midWidth      = absrect.getWidth() - leftWidth - rightWidth;

        //
        // draw the images
        //
        Vector3 pos(absrect.d_left, absrect.d_top, z);
        Size    sz(leftWidth, absrect.getHeight());
        d_leftSectionHover->draw(pos, sz, clipper, colours);

        pos.d_x = absrect.d_right - rightWidth;
        sz.d_width = rightWidth;
        d_rightSectionHover->draw(pos, sz, clipper, colours);

        pos.d_x = absrect.d_left + leftWidth;
        sz.d_width = midWidth;
        d_middleSectionHover->draw(pos, sz, clipper, colours);
    }

    // render clients custom image if that is required.
    if (d_useHoverImage)
    {
        colours = d_hoverImage.getColours();
        colours.setAlpha(alpha_comp);
        d_hoverImage.setColours(colours);
        Vector3 imgpos(absrect.d_left, absrect.d_top, System::getSingleton().getRenderer()->getZLayer(1));
        d_hoverImage.draw(imgpos, clipper);
    }

    //
    // Draw label text
    //
	//absrect.d_top += (absrect.getHeight() - getFont()->getLineSpacing()) * 0.5f;
    absrect.d_top += (absrect.getHeight() - getFont()->getLineSpacing()) * 0.35f;
    absrect.d_left += d_textXOffset * absrect.getWidth();
    colours.setColours(d_hoverColour);
    colours.setAlpha(alpha_comp);
    getFont()->drawText(getText(), absrect, System::getSingleton().getRenderer()->getZLayer(2), clipper, Centred, colours);
}


/*************************************************************************
    render Widget in Pushed state   
*************************************************************************/
void RLButton::drawPushed(float z)
{
    Rect clipper(getPixelRect());

    // do nothing if the widget is totally clipped.
    if (clipper.getWidth() == 0)
    {
        return;
    }

    // get the destination screen rect for this window
    Rect absrect(getUnclippedPixelRect());

    // calculate colours to use.
    float alpha_comp = getEffectiveAlpha();
    ColourRect colours(colour(1, 1, 1, alpha_comp));

    // render standard button imagery if required.
    if (d_useStandardImagery)
    {
        // calculate widths for the button segments
        float leftWidth     = d_leftSectionPushed->getWidth();
        float rightWidth    = d_rightSectionPushed->getWidth();
        float midWidth      = absrect.getWidth() - leftWidth - rightWidth;

        //
        // draw the images
        //
        Vector3 pos(absrect.d_left, absrect.d_top, z);
        Size    sz(leftWidth, absrect.getHeight());
        d_leftSectionPushed->draw(pos, sz, clipper, colours);

        pos.d_x = absrect.d_right - rightWidth;
        sz.d_width = rightWidth;
        d_rightSectionPushed->draw(pos, sz, clipper, colours);

        pos.d_x = absrect.d_left + leftWidth;
        sz.d_width = midWidth;
        d_middleSectionPushed->draw(pos, sz, clipper, colours);
    }

    // render clients custom image if that is required.
    if (d_usePushedImage)
    {
        colours = d_pushedImage.getColours();
        colours.setAlpha(alpha_comp);
        d_pushedImage.setColours(colours);
        Vector3 imgpos(absrect.d_left, absrect.d_top, System::getSingleton().getRenderer()->getZLayer(1));
        d_pushedImage.draw(imgpos, clipper);
    }

    //
    // Draw label text
    //
	//absrect.d_top += (absrect.getHeight() - getFont()->getLineSpacing()) * 0.5f;
    absrect.d_top += (absrect.getHeight() - getFont()->getLineSpacing()) * 0.35f;
    absrect.d_left += d_textXOffset * absrect.getWidth();
    colours.setColours(d_pushedColour);
    colours.setAlpha(alpha_comp);
    getFont()->drawText(getText(), absrect, System::getSingleton().getRenderer()->getZLayer(2), clipper, Centred, colours);
}


/*************************************************************************
    render Widget in disabled state 
*************************************************************************/
void RLButton::drawDisabled(float z)
{
    Rect clipper(getPixelRect());

    // do nothing if the widget is totally clipped.
    if (clipper.getWidth() == 0)
    {
        return;
    }

    // get the destination screen rect for this window
    Rect absrect(getUnclippedPixelRect());

    // calculate colours to use.
    float alpha_comp = getEffectiveAlpha();
    ColourRect colours(colour(0.5f, 0.5f, 0.5f, alpha_comp));

    // render standard button imagery if required.
    if (d_useStandardImagery)
    {
        // calculate widths for the button segments
        float leftWidth     = d_leftSectionNormal->getWidth();
        float rightWidth    = d_rightSectionNormal->getWidth();
        float midWidth      = absrect.getWidth() - leftWidth - rightWidth;

        //
        // draw the images
        //
        Vector3 pos(absrect.d_left, absrect.d_top, z);
        Size    sz(leftWidth, absrect.getHeight());
        d_leftSectionNormal->draw(pos, sz, clipper, colours);

        pos.d_x = absrect.d_right - rightWidth;
        sz.d_width = rightWidth;
        d_rightSectionNormal->draw(pos, sz, clipper, colours);

        pos.d_x = absrect.d_left + leftWidth;
        sz.d_width = midWidth;
        d_middleSectionNormal->draw(pos, sz, clipper, colours);
    }

    // render clients custom image if that is required.
    if (d_useDisabledImage)
    {
        colours = d_disabledImage.getColours();
        colours.setAlpha(alpha_comp);
        d_disabledImage.setColours(colours);
        Vector3 imgpos(absrect.d_left, absrect.d_top, System::getSingleton().getRenderer()->getZLayer(1));
        d_disabledImage.draw(imgpos, clipper);
    }

    //
    // Draw label text
    //
	absrect.d_top += (absrect.getHeight() - getFont()->getLineSpacing()) * 0.35f;
    absrect.d_left += d_textXOffset * absrect.getWidth();
    colours.setColours(d_disabledColour);
    colours.setAlpha(alpha_comp);
    getFont()->drawText(getText(), absrect, System::getSingleton().getRenderer()->getZLayer(2), clipper, Centred, colours);
}


/*************************************************************************
    Set whether to auto re-size custom image areas when the button is
    sized.  
*************************************************************************/
void RLButton::setCustomImageryAutoSized(bool setting)
{
    // if we are enabling auto-sizing, scale images for current size
    if ((setting == true) && (setting != d_autoscaleImages))
    {
        Rect area(0, 0, d_abs_area.getWidth(), d_abs_area.getHeight());

        d_normalImage.setRect(area);
        d_hoverImage.setRect(area);
        d_pushedImage.setRect(area);
        d_disabledImage.setRect(area);

        requestRedraw();
    }

    d_autoscaleImages = setting;
}


/*************************************************************************
    Handler for when button size is changed
*************************************************************************/
void RLButton::onSized(WindowEventArgs& e)
{
    // default processing
    PushButton::onSized(e);

    // scale images if required.
    if (d_autoscaleImages)
    {
        Rect area(0, 0, d_abs_area.getWidth(), d_abs_area.getHeight());

        d_normalImage.setRect(area);
        d_hoverImage.setRect(area);
        d_pushedImage.setRect(area);
        d_disabledImage.setRect(area);

        e.handled = true;
    }

}

void RLButton::addRLButtonProperties(void)
{
   addProperty(&d_normalImageProperty);
   addProperty(&d_pushedImageProperty);
   addProperty(&d_hoverImageProperty);
   addProperty(&d_useStandardImageryProperty);
   addProperty(&d_textXOffsetProperty);
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

    Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
    Create, initialise and return a RLButton
*************************************************************************/
Window* RLButtonFactory::createWindow(const String& name)
{
    RLButton* wnd = new RLButton(d_type, name);
    wnd->initialise();

    return wnd;
}

} // End of  CEGUI namespace section
