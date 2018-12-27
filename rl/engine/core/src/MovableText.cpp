//-----------------------------------------------------------
// Copyright (c) 2003 by cTh
// Use as you see fit.
// Questions : gavocanov@rambler.ru
//-----------------------------------------------------------
// Modified by Daniel Wickert, 2006

#include "stdinc.h"

#include "Exception.h"
#include "MovableText.h"

using namespace Ogre;
namespace rl
{

    const String MovableText::msType = "MovableText";

    MovableText::MovableText(
        const String& name, const String& caption, const String& fontName, int charHeight, const ColourValue& colour)
        : MovableObject(name)
        , Renderable()
        , mFontName(fontName)
        , mCaption(caption)
        , mColour(colour)
        , mRenderOp()
        , mAABB()
        , mLightList()
        , mCharHeight(charHeight)
        , mSpaceWidth(0)
        , mNeedUpdate(true)
        , mUpdateColours(true)
        , mOnTop(false)
        , mTimeUntilNextToggle(0)
        , mRadius(0)
        , mCamera(0)
        , mFont(0)
        , mMaterial()
        , mBackgroundMaterial()
        , mPositionOffset(Vector3::ZERO)
        , mScaleOffset(Vector3::UNIT_SCALE)
        , mAlignment(MovableText::ALIGN_LEFT)

    {
        if (name == Ogre::BLANKSTRING)
        {
            Throw(IllegalArgumentException, "Trying to create MovableText without name");
        }

        if (caption == Ogre::BLANKSTRING)
        {
            Throw(IllegalArgumentException, "Trying to create MovableText without caption");
        }

        this->setFontName(mFontName);
        this->_setupGeometry();
    }

    /************************************************************************/
    MovableText::~MovableText()
    {
        MaterialManager::getSingletonPtr()->remove(mMaterial->getName());
        delete mRenderOp.vertexData;
    }

    /************************************************************************/
    void MovableText::setFontName(const String& fontName)
    {
        if (mFontName != fontName || !mMaterial || !mFont)
        {
            mFontName = fontName;
            mFont = static_cast<Font*>(FontManager::getSingleton().getByName(mFontName).get());

            if (!mFont)
            {
                Throw(IllegalArgumentException, "Could not find font " + fontName);
            }

            if (!mFont->isLoaded())
            {
                mFont->load();
            }

            if (mMaterial)
            {
                MaterialManager::getSingletonPtr()->remove(mMaterial->getName());
                mMaterial.reset();
            }

            mMaterial = mFont->getMaterial()->clone(mName + "Material");
            if (!mMaterial->isLoaded())
            {
                mMaterial->load();
            }

            mMaterial->setDepthCheckEnabled(!mOnTop);
            mMaterial->setLightingEnabled(false);
            mNeedUpdate = true;
        }
    }

    /************************************************************************/
    void MovableText::showOnTop(bool show)
    {
        mOnTop = show;

        RlAssert1(mMaterial);

        mMaterial->setDepthBias(!mOnTop, !mOnTop);
        mMaterial->setDepthCheckEnabled(!mOnTop);
        mMaterial->setDepthWriteEnabled(mOnTop);
    }

    /************************************************************************/
    void MovableText::setSpaceWidth(unsigned int width)
    {
        if (width != mSpaceWidth)
        {
            mSpaceWidth = width;
            mNeedUpdate = true;
        }
    }

    /************************************************************************/
    void MovableText::setCaption(const String& caption)
    {
        if (caption != mCaption)
        {
            mCaption = caption;
            mNeedUpdate = true;
        }
    }

    /************************************************************************/
    void MovableText::setColour(const ColourValue& colour)
    {
        if (colour != mColour)
        {
            mColour = colour;
            mUpdateColours = true;
        }
    }

    /************************************************************************/
    void MovableText::setCharacterHeight(unsigned int height)
    {
        if (height != mCharHeight)
        {
            mCharHeight = height;
            mNeedUpdate = true;
        }
    }

    /************************************************************************/
    void MovableText::_setupGeometry()
    {
        RlAssert1(mFont);
        RlAssert1(mMaterial);

        unsigned int vertexCount = mCaption.size() * 6;

        if (mRenderOp.vertexData)
        {
            if (mRenderOp.vertexData->vertexCount != vertexCount)
            {
                delete mRenderOp.vertexData;
                mRenderOp.vertexData = NULL;
                mUpdateColours = true;
            }
        }

        if (!mRenderOp.vertexData)
        {
            mRenderOp.vertexData = new VertexData();
        }

        mRenderOp.indexData = 0;
        mRenderOp.vertexData->vertexStart = 0;
        mRenderOp.vertexData->vertexCount = vertexCount;
        mRenderOp.operationType = RenderOperation::OT_TRIANGLE_LIST;
        mRenderOp.useIndexes = false;

        VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
        VertexBufferBinding* bind = mRenderOp.vertexData->vertexBufferBinding;
        size_t offset = 0;

        // create/bind positions/tex.ccord. buffer
        if (!decl->findElementBySemantic(VES_POSITION))
        {
            decl->addElement(POS_TEX_BINDING, offset, VET_FLOAT3, VES_POSITION);
        }

        offset += VertexElement::getTypeSize(VET_FLOAT3);

        if (!decl->findElementBySemantic(VES_TEXTURE_COORDINATES))
        {
            decl->addElement(POS_TEX_BINDING, offset, VET_FLOAT2, VES_TEXTURE_COORDINATES, 0);
        }

        HardwareVertexBufferSharedPtr ptbuf
            = HardwareBufferManager::getSingleton().createVertexBuffer(decl->getVertexSize(POS_TEX_BINDING),
                mRenderOp.vertexData->vertexCount, HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
        bind->setBinding(POS_TEX_BINDING, ptbuf);

        // Colours - store these in a separate buffer because they change less often
        if (!decl->findElementBySemantic(VES_DIFFUSE))
        {
            decl->addElement(COLOUR_BINDING, 0, VET_COLOUR, VES_DIFFUSE);
        }

        HardwareVertexBufferSharedPtr cbuf
            = HardwareBufferManager::getSingleton().createVertexBuffer(decl->getVertexSize(COLOUR_BINDING),
                mRenderOp.vertexData->vertexCount, HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
        bind->setBinding(COLOUR_BINDING, cbuf);

        size_t charlen = mCaption.size();
        Real* pPCBuff = static_cast<Real*>(ptbuf->lock(HardwareBuffer::HBL_DISCARD));

        float largestWidth = 0;
        float left = 0 * 2.0 - 1.0;
        float top = -((0 * 2.0) - 1.0);

        // Derive space width from a capital A
        if (mSpaceWidth == 0)
        {
            mSpaceWidth = mFont->getGlyphAspectRatio('A') * mCharHeight * 2.0;
        }

        // for calculation of AABB
        Ogre::Vector3 min, max, currPos;
        Ogre::Real maxSquaredRadius;
        bool first = true;

        // Use iterator
        String::iterator i, iend;
        iend = mCaption.end();
        bool newLine = true;
        for (i = mCaption.begin(); i != iend; ++i)
        {
            if (newLine)
            {
                Real len = 0.0f;
                for (String::iterator j = i; j != iend && *j != '\n'; j++)
                {
                    if (*j == ' ')
                        len += mSpaceWidth;
                    else
                        len += mFont->getGlyphAspectRatio(*j) * mCharHeight * 2.0;
                }
                newLine = false;
            }

            if (*i == '\n')
            {
                left = 0 * 2.0 - 1.0;
                top -= mCharHeight * 2.0;
                newLine = true;
                continue;
            }

            if (*i == ' ')
            {
                // Just leave a gap, no tris
                left += mSpaceWidth;
                // Also reduce tri count
                mRenderOp.vertexData->vertexCount -= 6;
                continue;
            }

            Real horiz_height = mFont->getGlyphAspectRatio(*i);
            Real u1, u2, v1, v2;
            Font::UVRect rect = mFont->getGlyphTexCoords(*i);
            u1 = rect.left;
            v1 = rect.top;
            u2 = rect.right;
            v2 = rect.bottom;

            // each vert is (x, y, z, u, v)
            //---------------------------------------------------------------
            // First tri
            //
            // Upper left
            *pPCBuff++ = left;
            *pPCBuff++ = top;
            *pPCBuff++ = -1.0;
            *pPCBuff++ = u1;
            *pPCBuff++ = v1;

            // Deal with bounds
            currPos = Ogre::Vector3(left, top, -1.0);
            if (first)
            {
                min = max = currPos;
                maxSquaredRadius = currPos.squaredLength();
                first = false;
            }
            else
            {
                min.makeFloor(currPos);
                max.makeCeil(currPos);
                maxSquaredRadius = std::max(maxSquaredRadius, currPos.squaredLength());
            }

            top -= mCharHeight * 2.0;

            // Bottom left
            *pPCBuff++ = left;
            *pPCBuff++ = top;
            *pPCBuff++ = -1.0;
            *pPCBuff++ = u1;
            *pPCBuff++ = v2;

            // Deal with bounds
            currPos = Ogre::Vector3(left, top, -1.0);
            min.makeFloor(currPos);
            max.makeCeil(currPos);
            maxSquaredRadius = std::max(maxSquaredRadius, currPos.squaredLength());

            top += mCharHeight * 2.0;
            left += horiz_height * mCharHeight * 2.0;

            // Top right
            *pPCBuff++ = left;
            *pPCBuff++ = top;
            *pPCBuff++ = -1.0;
            *pPCBuff++ = u2;
            *pPCBuff++ = v1;
            //---------------------------------------------------------------

            // Deal with bounds
            currPos = Ogre::Vector3(left, top, -1.0);
            min.makeFloor(currPos);
            max.makeCeil(currPos);
            maxSquaredRadius = std::max(maxSquaredRadius, currPos.squaredLength());

            //---------------------------------------------------------------
            // Second tri
            //
            // Top right (again)
            *pPCBuff++ = left;
            *pPCBuff++ = top;
            *pPCBuff++ = -1.0;
            *pPCBuff++ = u2;
            *pPCBuff++ = v1;

            currPos = Ogre::Vector3(left, top, -1.0);
            min.makeFloor(currPos);
            max.makeCeil(currPos);
            maxSquaredRadius = std::max(maxSquaredRadius, currPos.squaredLength());

            top -= mCharHeight * 2.0;
            left -= horiz_height * mCharHeight * 2.0;

            // Bottom left (again)
            *pPCBuff++ = left;
            *pPCBuff++ = top;
            *pPCBuff++ = -1.0;
            *pPCBuff++ = u1;
            *pPCBuff++ = v2;

            currPos = Ogre::Vector3(left, top, -1.0);
            min.makeFloor(currPos);
            max.makeCeil(currPos);
            maxSquaredRadius = std::max(maxSquaredRadius, currPos.squaredLength());

            left += horiz_height * mCharHeight * 2.0;

            // Bottom right
            *pPCBuff++ = left;
            *pPCBuff++ = top;
            *pPCBuff++ = -1.0;
            *pPCBuff++ = u2;
            *pPCBuff++ = v2;
            //---------------------------------------------------------------

            currPos = Ogre::Vector3(left, top, -1.0);
            min.makeFloor(currPos);
            max.makeCeil(currPos);
            maxSquaredRadius = std::max(maxSquaredRadius, currPos.squaredLength());

            // Go back up with top
            top += mCharHeight * 2.0;

            float currentWidth = (left + 1) / 2 - 0;
            if (currentWidth > largestWidth)
            {
                largestWidth = currentWidth;
            }
        }

        // Unlock vertex buffer
        ptbuf->unlock();

        // update AABB/Sphere radius
        mAABB = Ogre::AxisAlignedBox(min / (300 * currPos.x), max / (300 * currPos.x));
        // mAABB = Ogre::AxisAlignedBox(Vector3(0.001,0.001,0.001),Vector3(0.01, 0.01, 0.01));

        mRadius = Ogre::Math::Sqrt(maxSquaredRadius) / (300 * currPos.x);

        if (mUpdateColours)
        {
            this->_updateColours();
        }

        mNeedUpdate = false;
    }

    /************************************************************************/
    void MovableText::_updateColours(void)
    {
        RlAssert1(mFont);
        RlAssert1(mMaterial);

        // Convert to system-specific
        RGBA colour;
        Root::getSingleton().convertColourValue(mColour, &colour);
        HardwareVertexBufferSharedPtr vbuf = mRenderOp.vertexData->vertexBufferBinding->getBuffer(COLOUR_BINDING);
        RGBA* pDest = static_cast<RGBA*>(vbuf->lock(HardwareBuffer::HBL_DISCARD));
        for (unsigned int i = 0; i < mRenderOp.vertexData->vertexCount; ++i)
            *pDest++ = colour;
        vbuf->unlock();
        mUpdateColours = false;
    }

    /************************************************************************/
    const Quaternion& MovableText::getWorldOrientation(void) const
    {
        RlAssert1(mCamera != 0);
        return mCamera->getDerivedOrientation();
    }

    /************************************************************************/
    const Vector3& MovableText::getWorldPosition(void) const
    {
        RlAssert1(mParentNode != 0);
        return mParentNode->_getDerivedPosition();
    }

    /************************************************************************/
    void MovableText::getWorldTransforms(Matrix4* xform) const
    {
        if (this->isVisible() && mCamera != 0)
        {
            Matrix3 rot3x3, scale3x3 = Matrix3::IDENTITY;
            // store rotation in a matrix
            mCamera->getDerivedOrientation().ToRotationMatrix(rot3x3);

            // parent node position
            Vector3 ppos = mParentNode->_getDerivedPosition();
            Quaternion pori = mParentNode->_getDerivedOrientation();
            Vector3 alignmentOffset = Vector3::ZERO;
            // Vector3 center = mAABB.getCenter();
            // if (mAlignment == ALIGN_CENTER)
            //{
            //    alignmentOffset = Vector3(-center.x / 2.0, 0, 0);
            //}
            // else if (mAlignment == ALIGN_RIGHT)
            //{
            //    alignmentOffset = Vector3(-center.x, 0, 0);
            //}
            ppos += pori * (mPositionOffset + alignmentOffset * 0.01);

            // apply scale
            scale3x3[0][0] = 0.01 * mScaleOffset.x * mParentNode->_getDerivedScale().x / 2;
            scale3x3[1][1] = 0.01 * mScaleOffset.y * mParentNode->_getDerivedScale().y / 2;
            scale3x3[2][2] = 0.01 * mScaleOffset.z * mParentNode->_getDerivedScale().z / 2;
            // apply all transforms to xform
            *xform = (rot3x3 * scale3x3);
            xform->setTrans(ppos);
        }
    }

    /************************************************************************/
    void MovableText::getRenderOperation(RenderOperation& op)
    {
        if (this->isVisible())
        {
            if (mNeedUpdate)
            {
                this->_setupGeometry();
            }
            if (mUpdateColours)
            {
                this->_updateColours();
            }
            op = mRenderOp;
        }
    }

    /************************************************************************/
    void MovableText::_notifyCurrentCamera(Camera* camera)
    {
        mCamera = camera;
    }

    /************************************************************************/
    void MovableText::_updateRenderQueue(RenderQueue* queue)
    {
        if (this->isVisible())
        {
            if (mNeedUpdate)
            {
                this->_setupGeometry();
            }
            if (mUpdateColours)
            {
                this->_updateColours();
            }

            // queue->addRenderable(this, mRenderQueueID, OGRE_RENDERABLE_DEFAULT_PRIORITY);
            queue->addRenderable(this);
        }
    }

    /************************************************************************/
    void MovableText::setPositionOffset(const Ogre::Vector3& offset)
    {
        mPositionOffset = offset;
    }

    /************************************************************************/
    Ogre::Vector3 MovableText::getPositionOffset() const
    {
        return mPositionOffset;
    }

    /************************************************************************/
    void MovableText::setScaleOffset(const Ogre::Vector3& offset)
    {
        mScaleOffset = offset;
    }

    /************************************************************************/
    Ogre::Vector3 MovableText::getScaleOffset() const
    {
        return mScaleOffset;
    }

    /************************************************************************/
    void MovableText::setAlignment(MovableText::Alignment alignment)
    {
        mAlignment = alignment;
    }

    /************************************************************************/
    MovableText::Alignment MovableText::getAlignment() const
    {
        return mAlignment;
    }

    /************************************************************************/
    void MovableText::visitRenderables(Renderable::Visitor* visitor, bool debugRenderables)
    {
        visitor->visit(this, 0, debugRenderables);
    }
}
