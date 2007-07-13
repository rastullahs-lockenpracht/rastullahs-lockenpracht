/************************************************************************
	filename: 	ListboxWrappedTextItem.cpp
	created:	01/8/2006
	author:		Paul D Turner & The CEGUI Development Team, Philipp Walser

	purpose:	Implementation of List box text items with WordWrapping
*************************************************************************/
/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the Perl Artistic License.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  Perl Artistic License for more details.
*
*  You should have received a copy of the Perl Artistic License
*  along with this program; if not you can get it here
*  http://www.perldoc.com/perl5.6/Artistic.html.
*/
#include "stdinc.h" //precompiled header

#include "ListboxWrappedTextItem.h"
#include "CEGUIFontManager.h"
#include "CEGUIFont.h"
#include "CEGUIWindow.h"
#include "CEGUIImage.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    /*************************************************************************
    Constants
    *************************************************************************/
    const colour    ListboxWrappedTextItem::DefaultTextColour   = 0xFFFFFFFF;
    const String    ListboxWrappedTextItem::ds_bullet           = " - ";


    /*************************************************************************
    Constructor
    *************************************************************************/
    ListboxWrappedTextItem::ListboxWrappedTextItem(const String& text, int padding,
        bool draw_bullet, uint item_id,
        void* item_data, bool disabled, bool auto_delete) :
    ListboxTextItem(text, item_id, item_data, disabled, auto_delete),
        d_textCols(DefaultTextColour, DefaultTextColour, DefaultTextColour, DefaultTextColour),
        d_padding(padding), d_draw_bullet(draw_bullet),
        d_font(0), d_textFormatting(WordWrapLeftAligned)
    {
    }

    /*************************************************************************
    Destructor
    *************************************************************************/
    ListboxWrappedTextItem::~ListboxWrappedTextItem()
    {
    }


    /*************************************************************************
    Return a pointer to the font being used by this ListboxWrappedTextItem
    *************************************************************************/
    const Font* ListboxWrappedTextItem::getFont(void) const
    {
        // prefer out own font
        if (d_font)
        {
            return d_font;
        }
        // try our owner window's font setting (may be null if owner uses no existant default font)
        else if (d_owner)
        {
            return d_owner->getFont();
        }
        // no owner, just use the default (which may be NULL anyway)
        else
        {
            return System::getSingleton().getDefaultFont();
        }

    }


    /*************************************************************************
    Set the font to be used by this ListboxWrappedTextItem
    *************************************************************************/
    void ListboxWrappedTextItem::setFont(const String& font_name)
    {
        setFont(FontManager::getSingleton().getFont(font_name));
    }

    /*************************************************************************
    Return the rendered pixel size of this list box item.
    *************************************************************************/
    Size ListboxWrappedTextItem::getPixelSize(void) const
    {
        Size size(0,0);

        if (d_owner != NULL)
        {
            size.d_width = d_owner->getWidth().asAbsolute(0.0f) - 25.0f;
        }

        Font* font = const_cast<Font*>(getFont());

        if (font != NULL)
        {
            size.d_height = PixelAligned(font->getLineSpacing());
            if(size.d_width == 0)
            {
                size.d_width = PixelAligned(font->getTextExtent(d_itemText));
            }

            Rect formatRect;

            formatRect.setPosition(CEGUI::Point(0,0));
            if (d_draw_bullet)
            {
                formatRect.d_left = font->getTextExtent(ds_bullet);
            }
            formatRect.setSize(size);
            uint lines = font->getFormattedLineCount(d_itemText, formatRect, d_textFormatting);
            size.d_height *= lines;
            size.d_height += d_padding;
        }

        return size;
    }


    /*************************************************************************
    Draw the list box item in its current state.
    *************************************************************************/
    void ListboxWrappedTextItem::draw(const Vector3& position, float alpha, const Rect& clipper) const
    {
        if (d_selected && (d_selectBrush != 0))
        {
            d_selectBrush->draw(clipper, position.d_z, clipper,
                getModulateAlphaColourRect(d_selectCols, alpha));
        }

        Font* font = const_cast<Font*>(getFont());

        if (font)
        {
            float left_offset = 0;
            if (d_draw_bullet)
            {
                left_offset = font->getTextExtent(ds_bullet);
            }

            Vector3 finalPos = position;
            finalPos.d_y -= PixelAligned((font->getLineSpacing() - font->getBaseline()) * 0.5f);
            Rect draw_area = Rect(finalPos.d_x, finalPos.d_y,
                clipper.d_right, finalPos.d_y);
            font->drawText(ds_bullet, draw_area, finalPos.d_z, clipper, d_textFormatting,
                getModulateAlphaColourRect(d_textCols, alpha));
            draw_area.d_left += left_offset;
            font->drawText(d_itemText, draw_area, finalPos.d_z, clipper, d_textFormatting,
                getModulateAlphaColourRect(d_textCols, alpha));
        }

    }

    void ListboxWrappedTextItem::draw(RenderCache& cache,const Rect& targetRect,
        float zBase, float alpha, const Rect* clipper) const
    {
        if (d_selected && d_selectBrush != 0)
        {
            cache.cacheImage(*d_selectBrush, targetRect, zBase,
                getModulateAlphaColourRect(d_selectCols, alpha), clipper);
        }

        Font* font = const_cast<Font*>(getFont());

        if (font)
        {
            float left_offset = 0;
            if (d_draw_bullet)
            {
                left_offset = font->getTextExtent(ds_bullet);
            }

            Rect finalPos = targetRect;
            finalPos.d_top -= (font->getLineSpacing() - font->getBaseline()) * 0.5f;
            cache.cacheText(ds_bullet, font, d_textFormatting, finalPos, zBase,
                getModulateAlphaColourRect(d_textCols, alpha), clipper);
            finalPos.d_left += left_offset;
            cache.cacheText(d_itemText, font, d_textFormatting, finalPos, zBase,
                getModulateAlphaColourRect(d_textCols, alpha), clipper);
        }
    }


    /*************************************************************************
    Set the colours used for text rendering.
    *************************************************************************/
    void ListboxWrappedTextItem::setTextColours(colour top_left_colour,
        colour top_right_colour,
        colour bottom_left_colour,
        colour bottom_right_colour)
    {
        d_textCols.d_top_left		= top_left_colour;
        d_textCols.d_top_right		= top_right_colour;
        d_textCols.d_bottom_left	= bottom_left_colour;
        d_textCols.d_bottom_right	= bottom_right_colour;
    }

    void ListboxWrappedTextItem::setTextFormatting(TextFormatting fmt)
    {
        d_textFormatting = fmt;
    }

    const TextFormatting& ListboxWrappedTextItem::getTextFormatting() const
    {
        return d_textFormatting;
    }


} // End of  CEGUI namespace section
