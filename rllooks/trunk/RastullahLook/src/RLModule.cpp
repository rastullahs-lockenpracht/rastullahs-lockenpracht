/************************************************************************
    filename:   RLModule.cpp
    created:    13/4/2004
    author:     Paul D Turner
    
    purpose:    Implements the System <-> GUI Module interface.
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
#include "RLModule.h"
#include "CEGUIExceptions.h"
#include "CEGUIWindowFactoryManager.h"
#include "RLFrameWindow.h"
#include "RLTitlebar.h"
#include "RLButton.h"
#include "RLCloseButton.h"
#include "RLCheckbox.h"
#include "RLRadioButton.h"
#include "RLSliderThumb.h"
#include "RLSlider.h"
#include "RLProgressBar.h"
#include "RLAlternateProgressBar.h"
#include "RLEditbox.h"
#include "RLVertScrollbarThumb.h"
#include "RLVertScrollbar.h"
#include "RLMiniVertScrollbarThumb.h"
#include "RLMiniVertScrollbar.h"
#include "RLMiniHorzScrollbarThumb.h"
#include "RLMiniHorzScrollbar.h"
#include "RLStatic.h"
#include "RLListbox.h"
#include "RLCombobox.h"
#include "RLComboEditbox.h"
#include "RLComboDropList.h"
#include "RLListHeader.h"
#include "RLListHeaderSegment.h"
#include "RLMultiColumnList.h"
#include "RLMultiLineEditbox.h"
#include "RLTabControl.h"
#include "RLTabButton.h"
#include "RLTabPane.h"


/*************************************************************************
    Static factory objects
*************************************************************************/
static CEGUI::RLFrameWindowFactory      s_FrameWindowFactory;
static CEGUI::RLTitlebarFactory         s_TitlebarFactory;
static CEGUI::RLCloseButtonFactory      s_CloseButtonFactory;
static CEGUI::RLButtonFactory           s_ButtonFactory;
static CEGUI::RLCheckboxFactory         s_CheckboxFactory;
static CEGUI::RLRadioButtonFactory      s_RadioButtonFactory;
static CEGUI::RLSliderThumbFactory      s_SliderThumbFactory;
static CEGUI::RLSliderFactory           s_SliderFactory;
static CEGUI::RLProgressBarFactory      s_ProgressBarFactory;
static CEGUI::RLAlternateProgressBarFactory s_AltProgressBarFactory;
static CEGUI::RLEditboxFactory          s_EditboxFactory;
static CEGUI::RLVertScrollbarFactory    s_VertScrollbarFactory;
static CEGUI::RLVertScrollbarThumbFactory   s_VertScrollbarThumbFactory;
static CEGUI::RLMiniVertScrollbarFactory    s_MiniVertScrollbarFactory;
static CEGUI::RLMiniVertScrollbarThumbFactory   s_MiniVertScrollbarThumbFactory;
static CEGUI::RLMiniHorzScrollbarFactory        s_MiniHorzScrollbarFactory;
static CEGUI::RLMiniHorzScrollbarThumbFactory   s_MiniHorzScrollbarThumbFactory;
static CEGUI::RLStaticImageFactory      s_StaticImageFactory;
static CEGUI::RLStaticTextFactory       s_StaticTextFactory;
static CEGUI::RLListboxFactory          s_ListboxFactory;
static CEGUI::RLComboboxFactory         s_ComboboxFactory;
static CEGUI::RLComboDropListFactory    s_ComboDropListFactory;
static CEGUI::RLComboEditboxFactory     s_ComboEditboxFactory;
static CEGUI::RLListHeaderSegmentFactory    s_ListHeaderSegmentFactory;
static CEGUI::RLListHeaderFactory       s_ListHeaderFactory;
static CEGUI::RLMultiColumnListFactory  s_MultiColumnListFactory;
static CEGUI::RLMultiLineEditboxFactory s_MultiLineEditboxFactory;
static CEGUI::RLTabControlFactory       s_TabControlFactory;
static CEGUI::RLTabButtonFactory        s_TabButtonFactory;
static CEGUI::RLTabPaneFactory          s_TabPaneFactory;


/*************************************************************************
    Plugin access interface
*************************************************************************/
extern "C" void registerFactory(const CEGUI::String& type_name)
{
    using namespace CEGUI;

    if (type_name == RLFrameWindow::WidgetTypeName)
    {
        WindowFactoryManager::getSingleton().addFactory(&s_FrameWindowFactory);
        return;
    }
    else if (type_name == RLTitlebar::WidgetTypeName)
    {
        WindowFactoryManager::getSingleton().addFactory(&s_TitlebarFactory);
        return;
    }
    else if (type_name == RLButton::WidgetTypeName)
    {
        WindowFactoryManager::getSingleton().addFactory(&s_ButtonFactory);
        return;
    }
    else if (type_name == RLCloseButton::WidgetTypeName)
    {
        WindowFactoryManager::getSingleton().addFactory(&s_CloseButtonFactory);
        return;
    }
    else if (type_name == RLCheckbox::WidgetTypeName)
    {
        WindowFactoryManager::getSingleton().addFactory(&s_CheckboxFactory);
        return;
    }
    else if (type_name == RLRadioButton::WidgetTypeName)
    {
        WindowFactoryManager::getSingleton().addFactory(&s_RadioButtonFactory);
        return;
    }
    else if (type_name == RLSliderThumb::WidgetTypeName)
    {
        WindowFactoryManager::getSingleton().addFactory(&s_SliderThumbFactory);
        return;
    }
    else if (type_name == RLSlider::WidgetTypeName)
    {
        WindowFactoryManager::getSingleton().addFactory(&s_SliderFactory);
        return;
    }
    else if (type_name == RLProgressBar::WidgetTypeName)
    {
        WindowFactoryManager::getSingleton().addFactory(&s_ProgressBarFactory);
        return;
    }
    else if (type_name == RLAlternateProgressBar::WidgetTypeName)
    {
        WindowFactoryManager::getSingleton().addFactory(&s_AltProgressBarFactory);
        return;
    }
    else if (type_name == RLEditbox::WidgetTypeName)
    {
        WindowFactoryManager::getSingleton().addFactory(&s_EditboxFactory);
        return;
    }
    else if (type_name == RLVertScrollbar::WidgetTypeName)
    {
        WindowFactoryManager::getSingleton().addFactory(&s_VertScrollbarFactory);
        return;
    }
    else if (type_name == RLVertScrollbarThumb::WidgetTypeName)
    {
        WindowFactoryManager::getSingleton().addFactory(&s_VertScrollbarThumbFactory);
        return;
    }
    else if (type_name == RLMiniVertScrollbar::WidgetTypeName)
    {
        WindowFactoryManager::getSingleton().addFactory(&s_MiniVertScrollbarFactory);
        return;
    }
    else if (type_name == RLMiniVertScrollbarThumb::WidgetTypeName)
    {
        WindowFactoryManager::getSingleton().addFactory(&s_MiniVertScrollbarThumbFactory);
        return;
    }
    else if (type_name == RLMiniHorzScrollbar::WidgetTypeName)
    {
        WindowFactoryManager::getSingleton().addFactory(&s_MiniHorzScrollbarFactory);
        return;
    }
    else if (type_name == RLMiniHorzScrollbarThumb::WidgetTypeName)
    {
        WindowFactoryManager::getSingleton().addFactory(&s_MiniHorzScrollbarThumbFactory);
        return;
    }
    else if (type_name == RLStaticImage::WidgetTypeName)
    {
        WindowFactoryManager::getSingleton().addFactory(&s_StaticImageFactory);
        return;
    }
    else if (type_name == RLStaticText::WidgetTypeName)
    {
        WindowFactoryManager::getSingleton().addFactory(&s_StaticTextFactory);
        return;
    }
    else if (type_name == RLListbox::WidgetTypeName)
    {
        WindowFactoryManager::getSingleton().addFactory(&s_ListboxFactory);
        return;
    }
    else if (type_name == RLCombobox::WidgetTypeName)
    {
        WindowFactoryManager::getSingleton().addFactory(&s_ComboboxFactory);
        return;
    }
    else if (type_name == RLComboDropList::WidgetTypeName)
    {
        WindowFactoryManager::getSingleton().addFactory(&s_ComboDropListFactory);
        return;
    }
    else if (type_name == RLComboEditbox::WidgetTypeName)
    {
        WindowFactoryManager::getSingleton().addFactory(&s_ComboEditboxFactory);
        return;
    }
    else if (type_name == RLListHeaderSegment::WidgetTypeName)
    {
        WindowFactoryManager::getSingleton().addFactory(&s_ListHeaderSegmentFactory);
        return;
    }
    else if (type_name == RLListHeader::WidgetTypeName)
    {
        WindowFactoryManager::getSingleton().addFactory(&s_ListHeaderFactory);
        return;
    }
    else if (type_name == RLMultiColumnList::WidgetTypeName)
    {
        WindowFactoryManager::getSingleton().addFactory(&s_MultiColumnListFactory);
        return;
    }
    else if (type_name == RLMultiLineEditbox::WidgetTypeName)
    {
        WindowFactoryManager::getSingleton().addFactory(&s_MultiLineEditboxFactory);
        return;
    }
    else if (type_name == RLTabControl::WidgetTypeName)
    {
        WindowFactoryManager::getSingleton().addFactory(&s_TabControlFactory);
        return;
    }
    else if (type_name == RLTabButton::WidgetTypeName)
    {
        WindowFactoryManager::getSingleton().addFactory(&s_TabButtonFactory);
        return;
    }
    else if (type_name == RLTabPane::WidgetTypeName)
    {
        WindowFactoryManager::getSingleton().addFactory(&s_TabPaneFactory);
        return;
    }

    throw UnknownObjectException((utf8*)"::registerFactory - The window factory for type '" + type_name + "' is not known in this module.");

    return;
}
