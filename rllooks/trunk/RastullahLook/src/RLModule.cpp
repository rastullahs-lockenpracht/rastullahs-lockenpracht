/************************************************************************
	filename: 	RLModule.cpp
	created:	13/4/2004
	author:		Paul D Turner
	
	purpose:	Implements the System <-> GUI Module interface.
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
#include "RLVUMeter.h"
#include "RLSpinner.h"
#include "RLScrollablePane.h"
#include "RLTooltip.h"
#include "RLPopupMenu.h"
#include "RLMenubar.h"
#include "RLMenuItem.h"


/*************************************************************************
	Static factory objects
*************************************************************************/
static CEGUI::RLFrameWindowFactory		s_FrameWindowFactory;
static CEGUI::RLTitlebarFactory			s_TitlebarFactory;
static CEGUI::RLCloseButtonFactory		s_CloseButtonFactory;
static CEGUI::RLButtonFactory			s_ButtonFactory;
static CEGUI::RLCheckboxFactory			s_CheckboxFactory;
static CEGUI::RLRadioButtonFactory		s_RadioButtonFactory;
static CEGUI::RLSliderThumbFactory		s_SliderThumbFactory;
static CEGUI::RLSliderFactory			s_SliderFactory;
static CEGUI::RLProgressBarFactory		s_ProgressBarFactory;
static CEGUI::RLAlternateProgressBarFactory s_AltProgressBarFactory;
static CEGUI::RLEditboxFactory			s_EditboxFactory;
static CEGUI::RLVertScrollbarFactory	s_VertScrollbarFactory;
static CEGUI::RLVertScrollbarThumbFactory	s_VertScrollbarThumbFactory;
static CEGUI::RLMiniVertScrollbarFactory	s_MiniVertScrollbarFactory;
static CEGUI::RLMiniVertScrollbarThumbFactory	s_MiniVertScrollbarThumbFactory;
static CEGUI::RLMiniHorzScrollbarFactory		s_MiniHorzScrollbarFactory;
static CEGUI::RLMiniHorzScrollbarThumbFactory	s_MiniHorzScrollbarThumbFactory;
static CEGUI::RLStaticImageFactory		s_StaticImageFactory;
static CEGUI::RLStaticTextFactory		s_StaticTextFactory;
static CEGUI::RLListboxFactory			s_ListboxFactory;
static CEGUI::RLComboboxFactory			s_ComboboxFactory;
static CEGUI::RLComboDropListFactory	s_ComboDropListFactory;
static CEGUI::RLComboEditboxFactory		s_ComboEditboxFactory;
static CEGUI::RLListHeaderSegmentFactory	s_ListHeaderSegmentFactory;
static CEGUI::RLListHeaderFactory		s_ListHeaderFactory;
static CEGUI::RLMultiColumnListFactory	s_MultiColumnListFactory;
static CEGUI::RLMultiLineEditboxFactory	s_MultiLineEditboxFactory;
static CEGUI::RLTabControlFactory	    s_TabControlFactory;
static CEGUI::RLTabButtonFactory	    s_TabButtonFactory;
static CEGUI::RLTabPaneFactory	        s_TabPaneFactory;
static CEGUI::RLVUMeterFactory			s_VUMeterFactory;
static CEGUI::RLSpinnerFactory          s_SpinnerFactory;
static CEGUI::RLScrollablePaneFactory   s_ScrollablePaneFactory;
static CEGUI::RLTooltipFactory          s_TooltipFactory;
static CEGUI::RLPopupMenuFactory		s_PopupMenuFactory;
static CEGUI::RLPopupMenuItemFactory	s_PopupMenuItemFactory;
static CEGUI::RLMenubarFactory			s_MenubarFactory;
static CEGUI::RLMenubarItemFactory		s_MenubarItemFactory;


using namespace CEGUI;

struct mapEntry
{
    const CEGUI::utf8* d_name;
    CEGUI::WindowFactory* d_factory;
};

mapEntry factoriesMap[] =
{
    {RLAlternateProgressBar::WidgetTypeName, &s_AltProgressBarFactory},
    {RLButton::WidgetTypeName, &s_ButtonFactory},
    {RLCheckbox::WidgetTypeName, &s_CheckboxFactory},
    {RLCloseButton::WidgetTypeName, &s_CloseButtonFactory},
    {RLCombobox::WidgetTypeName, &s_ComboboxFactory},
    {RLComboDropList::WidgetTypeName, &s_ComboDropListFactory},
    {RLComboEditbox::WidgetTypeName, &s_ComboEditboxFactory},    
    {RLEditbox::WidgetTypeName, &s_EditboxFactory},
    {RLFrameWindow::WidgetTypeName, &s_FrameWindowFactory},
    {RLListbox::WidgetTypeName, &s_ListboxFactory},
    {RLListHeader::WidgetTypeName, &s_ListHeaderFactory},
    {RLListHeaderSegment::WidgetTypeName, &s_ListHeaderSegmentFactory},
    {RLMiniHorzScrollbar::WidgetTypeName, &s_MiniHorzScrollbarFactory},
    {RLMiniHorzScrollbarThumb::WidgetTypeName, &s_MiniHorzScrollbarThumbFactory},
    {RLMiniVertScrollbar::WidgetTypeName, &s_MiniVertScrollbarFactory},
    {RLMiniVertScrollbarThumb::WidgetTypeName, &s_MiniVertScrollbarThumbFactory},
    {RLMultiColumnList::WidgetTypeName, &s_MultiColumnListFactory},
    {RLMultiLineEditbox::WidgetTypeName, &s_MultiLineEditboxFactory},
    {RLProgressBar::WidgetTypeName, &s_ProgressBarFactory},
    {RLRadioButton::WidgetTypeName, &s_RadioButtonFactory},
    {RLScrollablePane::WidgetTypeName, &s_ScrollablePaneFactory},
    {RLSlider::WidgetTypeName, &s_SliderFactory},
    {RLSliderThumb::WidgetTypeName, &s_SliderThumbFactory},
    {RLSpinner::WidgetTypeName, &s_SpinnerFactory},
    {RLStaticImage::WidgetTypeName, &s_StaticImageFactory},
    {RLStaticText::WidgetTypeName, &s_StaticTextFactory},
    {RLTabButton::WidgetTypeName, &s_TabButtonFactory},
    {RLTabControl::WidgetTypeName, &s_TabControlFactory},
    {RLTabPane::WidgetTypeName, &s_TabPaneFactory},
    {RLTitlebar::WidgetTypeName, &s_TitlebarFactory},
    {RLTooltip::WidgetTypeName, &s_TooltipFactory},
    {RLVertScrollbar::WidgetTypeName, &s_VertScrollbarFactory},
    {RLVertScrollbarThumb::WidgetTypeName, &s_VertScrollbarThumbFactory},
    {RLVUMeter::WidgetTypeName, &s_VUMeterFactory},
	{RLPopupMenu::WidgetTypeName, &s_PopupMenuFactory},
	{RLPopupMenuItem::WidgetTypeName, &s_PopupMenuItemFactory},
	{RLMenubar::WidgetTypeName, &s_MenubarFactory},
	{RLMenubarItem::WidgetTypeName, &s_MenubarItemFactory},
    {0,0}
};

/*************************************************************************
	Plugin access interface
*************************************************************************/
extern "C" void registerFactory(const CEGUI::String& type_name)
{
    mapEntry* entry = factoriesMap;

    while (entry->d_name)
    {
        if (entry->d_name == type_name)
        {
            WindowFactoryManager::getSingleton().addFactory(entry->d_factory);
            return;
        }

        ++entry;
    }

    throw UnknownObjectException((utf8*)"::registerFactory - The window factory for type '" + type_name + "' is not known in this module.");
}

extern "C" CEGUI::uint registerAllFactories(void)
{
    CEGUI::uint count = 0;
    mapEntry* entry = factoriesMap;

    while (entry->d_name)
    {
        WindowFactoryManager::getSingleton().addFactory(entry->d_factory);

        ++entry;
        ++count;
    }

    return count;
}
