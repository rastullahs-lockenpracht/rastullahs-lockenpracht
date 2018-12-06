/************************************************************************
         filename:       ListboxWrappedTextItem.h
         created:        2/6/2004
         author:         Paul D Turner & The CEGUI Development Team, Philipp Walser

         purpose:        Interface for list box text items with line wrapping
*************************************************************************/
/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Clarified Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Clarified Artistic License for more details.
 *
 *  You should have received a copy of the Clarified Artistic License
 *  along with this program; if not you can get it here
 *  http://www.jpaulmorrison.com/fbp/artistic2.htm.
 */
#ifndef _ListboxWrappedTextItem_h_
#define _ListboxWrappedTextItem_h_

#include "CEGUIFont.h"
#include "elements/CEGUIListboxTextItem.h"

namespace CEGUI
{

    class ListboxWrappedTextItem : public ListboxTextItem
    {
    public:
        /*************************************************************************
            Constants
        *************************************************************************/
        static const colour DefaultTextColour; //!< Default text colour.

        /*************************************************************************
            Construction and Destruction
        *************************************************************************/
        /*!
        \brief
            base class constructor
        */
        ListboxWrappedTextItem(const String& text, int padding = 0, bool draw_bullet = false, uint item_id = 0,
            void* item_data = NULL, bool disabled = false, bool auto_delete = true);

        /*!
        \brief
            base class destructor
        */
        virtual ~ListboxWrappedTextItem();

        /*************************************************************************
            Accessor methods
        *************************************************************************/
        /*!
        \brief
            Return a pointer to the font being used by this ListboxTextItem

            This method will try a number of places to find a font to be used.  If no font can be
            found, NULL is returned.

        \return
            Font to be used for rendering this item
        */
        const Font* getFont(void) const;

        /*!
        \brief
            Return the current colours used for text rendering.

        \return
            ColourRect object describing the currently set colours
        */
        ColourRect getTextColours(void) const
        {
            return d_textCols;
        }

        /*!
        \brief
            Return the text format used for text rendering.

        \return
            TextFormatting value describing the currently set text format
        */
        const TextFormatting& getTextFormatting() const;

        /*************************************************************************
            Manipulator methods
        *************************************************************************/
        /*!
        \brief
            Set the font to be used by this ListboxTextItem

        \param font
            Font to be used for rendering this item

        \return
            Nothing
        */
        void setFont(const Font* font)
        {
            d_font = font;
        }

        /*!
        \brief
            Set the font to be used by this ListboxTextItem

        \param font_name
            String object containing the name of the Font to be used for rendering this item

        \return
            Nothing
        */
        void setFont(const String& font_name);

        /*!
        \brief
            Set the colours used for text rendering.

        \param cols
            ColourRect object describing the colours to be used.

        \return
            Nothing.
        */
        void setTextColours(const ColourRect& cols)
        {
            d_textCols = cols;
        }

        /*!
        \brief
            Set the colours used for text rendering.

        \param top_left_colour
            Colour (as ARGB value) to be applied to the top-left corner of each text glyph rendered.

        \param top_right_colour
            Colour (as ARGB value) to be applied to the top-right corner of each text glyph rendered.

        \param bottom_left_colour
            Colour (as ARGB value) to be applied to the bottom-left corner of each text glyph rendered.

        \param bottom_right_colour
            Colour (as ARGB value) to be applied to the bottom-right corner of each text glyph rendered.

        \return
            Nothing.
        */
        void setTextColours(
            colour top_left_colour, colour top_right_colour, colour bottom_left_colour, colour bottom_right_colour);

        /*!
        \brief
            Set the colours used for text rendering.

        \param col
            colour value to be used when rendering.

        \return
            Nothing.
        */
        void setTextColours(colour col)
        {
            setTextColours(col, col, col, col);
        }

        /*!
        \brief
            Set the text format used for text rendering.

        \param fmt
            TextFormat to be used when rendering.

        \return
            Nothing.
        */
        void setTextFormatting(TextFormatting fmt);

        /*************************************************************************
            Required implementations of pure virtuals from the base class.
        *************************************************************************/
        Size getPixelSize(void) const;
        void draw(const Vector3& position, float alpha, const Rect& clipper) const;
        void draw(RenderCache& cache, const Rect& targetRect, float zBase, float alpha, const Rect* clipper) const;

    protected:
        /*************************************************************************
            Implementation Data
        *************************************************************************/
        static const String ds_bullet;
        ColourRect d_textCols; //!< Colours used for rendering the text.
        const Font* d_font; //!< Font used for rendering text.
        const int d_padding; //!< Padding in pixels to the bottom side of cliprect
        const bool d_draw_bullet; //!< Start an item with a bullet sign.

    private:
        TextFormatting d_textFormatting;
    };
}

#endif
