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
#include "CEGUIImage.h"
#include "CEGUIImageset.h"
#include "CEGUIImagesetManager.h"

#include "CEGUIExceptions.h"

#include "RLButton.h"
#include "RLButtonProperties.h"
#include "CEGUIPropertyHelper.h"

// Start of CEGUI namespace section
namespace CEGUI
{

// Start of RLButtonProperties namespace section
namespace RLButtonProperties
{

String NormalImage::get(const PropertyReceiver *receiver) const
{
   //RLButton currently has no getter for this...
   return String("");//PropertyHelper::imageToString(static_cast<const RLButton*>(receiver)->getNormal());
}

void NormalImage::set(PropertyReceiver *receiver, const String &value)
{
   RenderableImage * image = new RenderableImage();
   image->setImage(PropertyHelper::stringToImage(value));
   image->setHorzFormatting(RenderableImage::HorzStretched);
   image->setVertFormatting(RenderableImage::VertStretched);
   static_cast<RLButton*>(receiver)->setNormalImage(image);
}

String PushedImage::get(const PropertyReceiver *receiver) const
{
   //RLButton currently has no getter for this...
   return String("");//PropertyHelper::imageToString(static_cast<const RLButton*>(receiver)->getNormal());
}

void PushedImage::set(PropertyReceiver *receiver, const String &value)
{
   RenderableImage * image = new RenderableImage();
   image->setImage(PropertyHelper::stringToImage(value));
   image->setHorzFormatting(RenderableImage::HorzStretched);
   image->setVertFormatting(RenderableImage::VertStretched);
   static_cast<RLButton*>(receiver)->setPushedImage(image);
}

String HoverImage::get(const PropertyReceiver *receiver) const
{
   //RLButton currently has no getter for this...
   return String("");//PropertyHelper::imageToString(static_cast<const RLButton*>(receiver)->getNormal());
}

void HoverImage::set(PropertyReceiver *receiver, const String &value)
{
   RenderableImage * image = new RenderableImage();
   image->setImage(PropertyHelper::stringToImage(value));
   image->setHorzFormatting(RenderableImage::HorzStretched);
   image->setVertFormatting(RenderableImage::VertStretched);
   static_cast<RLButton*>(receiver)->setHoverImage(image);
}

String UseStandardImagery::get(const PropertyReceiver *receiver) const
{
    return PropertyHelper::boolToString(static_cast<const RLButton*>(receiver)->isStandardImageryEnabled());
}

void UseStandardImagery::set(PropertyReceiver *receiver, const String &value)
{
   static_cast<RLButton*>(receiver)->setStandardImageryEnabled(PropertyHelper::stringToBool(value));
}

String TextXOffset::get(const PropertyReceiver *receiver) const
{
   return PropertyHelper::floatToString(static_cast<const RLButton*>(receiver)->getTextXOffset());
}

void TextXOffset::set(PropertyReceiver *receiver, const String &value)
{
   static_cast<RLButton*>(receiver)->setTextXOffset(PropertyHelper::stringToFloat(value));
}

}

};

