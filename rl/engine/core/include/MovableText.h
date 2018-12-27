//-----------------------------------------------------------
// Copyright (c) 2003 by cTh
// Use as you see fit.
// Questions : gavocanov@rambler.ru
//-----------------------------------------------------------
// Modified by Daniel Wickert, 2006

#ifndef __MovableText_H__
#define __MovableText_H__

#include "CorePrerequisites.h"

namespace rl
{
    /************************************************************************/
    class MovableText : public Ogre::MovableObject, public Ogre::Renderable
    {
    public:
        typedef enum
        {
            ALIGN_LEFT = 0,
            ALIGN_CENTER,
            ALIGN_RIGHT
        } Alignment;

        MovableText(const Ogre::String& name, const Ogre::String& caption,
            const Ogre::String& fontName = "VeraSerifBold-16", int charHeight = 16,
            const Ogre::ColourValue& colour = Ogre::ColourValue::White);

        virtual ~MovableText();

        // Local space offset to scene node position,
        // if you don't want to create an extra node just for the text
        void setPositionOffset(const Ogre::Vector3& offset);
        Ogre::Vector3 getPositionOffset() const;

        void setScaleOffset(const Ogre::Vector3& offset);
        Ogre::Vector3 getScaleOffset() const;

        void setAlignment(Alignment alignment);
        Alignment getAlignment() const;

        void setFontName(const Ogre::String& fontName);
        void setCaption(const Ogre::String& caption);
        void setColour(const Ogre::ColourValue& colour);
        void setCharacterHeight(Ogre::uint height);
        void setSpaceWidth(Ogre::uint width);
        void showOnTop(bool show);

        const Ogre::String& getCaption() const
        {
            return mCaption;
        }
        const Ogre::String& getFontName() const
        {
            return mFontName;
        }
        Ogre::uint getCharacterHeight() const
        {
            return mCharHeight;
        }
        Ogre::uint getSpaceWidth() const
        {
            return mSpaceWidth;
        }
        const Ogre::ColourValue& getColour() const
        {
            return mColour;
        }
        bool getShowOnTop() const
        {
            return mOnTop;
        }
        Ogre::AxisAlignedBox GetAABB(void)
        {
            return mAABB;
        }

        // from MovableObject
        void getWorldTransforms(Ogre::Matrix4* xform) const;
        const Ogre::Quaternion& getWorldOrientation(void) const;
        const Ogre::Vector3& getWorldPosition(void) const;
        void _notifyCurrentCamera(Ogre::Camera* cam);
        void _updateRenderQueue(Ogre::RenderQueue* queue);

        const Ogre::AxisAlignedBox& getBoundingBox(void) const
        {
            return mAABB;
        }
        const Ogre::String& getMovableType(void) const
        {
            return msType;
        }
        Ogre::Real getBoundingRadius(void) const
        {
            return mRadius;
        }
        Ogre::Real getSquaredViewDepth(const Ogre::Camera* cam) const
        {
            return 0;
        }
        void visitRenderables(Ogre::Renderable::Visitor* visitor, bool debugRenderables = false);

        // from renderable
        void getRenderOperation(Ogre::RenderOperation& op);
        const Ogre::MaterialPtr& getMaterial(void) const
        {
            assert(mMaterial);
            return mMaterial;
        }
        const Ogre::LightList& getLights(void) const
        {
            return mLightList;
        }

    private:
        static const int POS_TEX_BINDING = 0;
        static const int COLOUR_BINDING = 1;
        static const Ogre::String msType;

        Ogre::String mFontName;
        Ogre::String mCaption;

        Ogre::ColourValue mColour;
        Ogre::RenderOperation mRenderOp;
        Ogre::AxisAlignedBox mAABB;
        Ogre::LightList mLightList;

        Ogre::uint mCharHeight;
        Ogre::uint mSpaceWidth;

        bool mNeedUpdate;
        bool mUpdateColours;
        bool mOnTop;

        Ogre::Real mTimeUntilNextToggle;
        Ogre::Real mRadius;

        Ogre::Camera* mCamera;
        Ogre::Font* mFont;
        Ogre::MaterialPtr mMaterial;
        Ogre::MaterialPtr mBackgroundMaterial;

        Ogre::Vector3 mPositionOffset;
        Ogre::Vector3 mScaleOffset;
        Alignment mAlignment;

        void _setupGeometry();
        void _updateColours();
    };
}

#endif
