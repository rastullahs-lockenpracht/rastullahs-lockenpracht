/*
-----------------------------------------------------------
Copyright © 2003 by cTh
Use as you see fit.
Questions : gavocanov@rambler.ru
-----------------------------------------------------------
*/
/***************************************************************************************************/
#ifndef INC_MovableText_H
#define INC_MovableText_H

#include <OgreMovableObject.h>
#include <OgreRenderable.h>

#include "CorePrerequisites.h"
#include "ActorControlledObject.h"

/***************************************************************************************************/
namespace rl
{
	/***************************************************************************************************/
	class _RlCoreExport MovableText : public ActorControlledObject, public Ogre::MovableObject, public Ogre::Renderable
	{
	public:
		MovableText(
			const Ogre::String &name, 
			const Ogre::String &caption, 
			const Ogre::String &fontName = "VeraSerifBold-12", 
			int charHeight = 12, 
			const Ogre::ColourValue &color = Ogre::ColourValue::White);
		virtual ~MovableText();
	
		void setFontName(const Ogre::String &fontName);
		void setCaption(const Ogre::String &caption);
		void setColor(const Ogre::ColourValue &color);
		void setCharacterHeight(Ogre::uint height);
		void setSpaceWidth(Ogre::uint width);
		void showOnTop(bool show);

		const Ogre::String &getCaption() const {return mCaption;}
		const Ogre::String &getFontName() const {return mFontName;}
		Ogre::uint getCharacterHeight() const {return mCharHeight;}
		Ogre::uint getSpaceWidth() const {return mSpaceWidth;}
		const Ogre::ColourValue &getColor() const {return mColor;}
		bool getShowOnTop() const {return mOnTop;}
        Ogre::AxisAlignedBox	GetAABB(void) { return mAABB; }

		Ogre::String getObjectType() { static Ogre::String TYPE = "MovableText"; return TYPE; }

	private:
		Ogre::String			mFontName;
		Ogre::String			mType;
		Ogre::String			mName;
		Ogre::String			mCaption;

		Ogre::ColourValue		mColor;
		Ogre::RenderOperation	mRenderOp;
		Ogre::AxisAlignedBox	mAABB;
		Ogre::LightList		mLList;

		Ogre::uint			mCharHeight;
		Ogre::uint			mSpaceWidth;

		bool			mNeedUpdate;
		bool			mUpdateColors;
		bool			mOnTop;

		Ogre::Real			mTimeUntilNextToggle;
		Ogre::Real			mRadius;

		Ogre::Camera			*mpCam;
		Ogre::RenderWindow	*mpWin;
		Ogre::Font			   *mpFont;
		Ogre::MaterialPtr		mpMaterial;
        Ogre::MaterialPtr		mpBackgroundMaterial;

		Ogre::Vector3 _translate3Dto2D(Ogre::Camera *pCam, const Ogre::Vector3 vertex);
		void	_translate3Dto2DPixels(Ogre::Camera *pCam, const Ogre::Vector3 vertex, int *x, int *y);
		void	_setupGeometry();
		void	_updateColors();

		// from MovableObject
		void getWorldTransforms(Ogre::Matrix4 *xform) const;
        const Ogre::Quaternion& getWorldOrientation(void) const;
        const Ogre::Vector3& getWorldPosition(void) const;
		void _notifyCurrentCamera(Ogre::Camera *cam);
		void _updateRenderQueue(Ogre::RenderQueue* queue);

		const Ogre::AxisAlignedBox &getBoundingBox(void) const {return mAABB;};
		const Ogre::String &getName(void) const {return mName;};
		const Ogre::String &getMovableType(void) const {static Ogre::String movType = "MovableText"; return movType;};
		Ogre::Real getBoundingRadius(void) const {return mRadius;};
		Ogre::Real getSquaredViewDepth(const Ogre::Camera *cam) const {return 0;};

		// from renderable
		void getRenderOperation(Ogre::RenderOperation &op);
		const Ogre::MaterialPtr& getMaterial(void) const {assert(!mpMaterial.isNull());return mpMaterial;};
		const Ogre::LightList& getLights(void) const {return mLList;};
	};
	/***************************************************************************************************/
}
/***************************************************************************************************/
#endif
/***************************************************************************************************/
