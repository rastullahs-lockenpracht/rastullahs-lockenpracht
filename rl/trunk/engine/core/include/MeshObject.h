/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __MeshObject_H__
#define __MeshObject_H__

#include "CorePrerequisites.h"
#include "PhysicalObject.h"

#include <OgreEntity.h>

namespace rl {

	class MeshAnimation;

	class _RlCoreExport MeshObject : public PhysicalObject
    {
    public:
        MeshObject(const Ogre::String& name, const Ogre::String& meshname);
        virtual ~MeshObject();

        /// Wie ActorControlledObject::getMovableObject()
        /// Nur schon gebrauchsfertig gecastet.
        Ogre::Entity* getEntity() const;
        
        /// Groesse der Boundingbox
        Ogre::AxisAlignedBox getDefaultSize() const;
        Ogre::AxisAlignedBox getPoseSize(const Ogre::String& animationName);
        
		/// Returns whether a mesh has an animation called animName or not.
        bool hasAnimation(const Ogre::String& animName) const;

		/** Returns a MeshAnimation for the given animation name.
         *  Either returns an already created MeshAnimation or creates it ad-hoc in a paused state.
         *  @throw IllegalArgumentException, if animName is not an animation of the Mesh.
         */
        MeshAnimation* getAnimation(const Ogre::String& animName);

		/** Creates and returns a MeshAnimation for the given animation name.
         *  This animation is returned unpaused.
         *  @param animName name of the animation to be started.
         *  @param speed scale factor for animation speed. 1.0 is normal time as defined.
         *  @param timesToPlay 0 means infinitly looped.
         *
         *  @throw IllegalArgumentException, if animName is not an animation of the Mesh.
         */
		MeshAnimation* startAnimation(const Ogre::String& animName, 
		    Ogre::Real speed=1.0, unsigned int timesToPlay=0);

		/** First stops oldAnimName, then starts newAnimName
         *  @see rl::MeshObject::startAnimation
         *  @see rl::MeshObject::stopAnimation
         *  @throw IllegalArgumentException, if either anim name is not an animation of the Mesh.
         */
		MeshAnimation* replaceAnimation(const Ogre::String& oldAnimName,
            const Ogre::String& newAnimName, 
		    Ogre::Real speed=1.0, unsigned int timesToPlay=0);

		/** Stops an animation.
         *  @param animName name of the animation to be stopped.
         *
         *  @throw IllegalArgumentException, if animName is not an animation of the Mesh.
         */
        void stopAnimation(const Ogre::String& animName);

        void stopAllAnimations();
        void stopAllAnimationsExcept(const Ogre::String& animName);

        void setCastShadows (bool enabled);
        bool getCastShadows (void) const;

		/** creates a mesh animated to the specified pose.
		 * @param animName name of the animation to be posed at.
		 */
		MeshObject* createPosedCopy(const Ogre::String& animName);

		const Ogre::String &getMeshName() const;

		virtual bool isMeshObject() const;

        /// Per Skeleton-Blend-Setting
        bool getBlendCumulative() const;
        void setBlendCumulative(bool cumulative);

        void setHighlighted( bool highlight );
        
        virtual Ogre::String getObjectType() const;

        /** Changes the Material of a submesh
         * if the submesh is not specified (or the parameter is set to "") 
         * the first submesh's material is changed
         * @param material the name of the new material
         * @param submesh (optional) the name of the submesh
         */
        void setMaterial(const Ogre::String& material, const Ogre::String& submesh = "");
    private:
		Ogre::String mMeshName;
        Ogre::AxisAlignedBox mSize;
        typedef std::map<Ogre::String, Ogre::AxisAlignedBox> PoseMap;
		PoseMap mPoseSizes;

		/** calculates the default size stored in mSize.
		 * This is the size of the mesh in it's default position.
		 */
		Ogre::AxisAlignedBox calculateDefaultSize(void);
		/** calculates the size from vertices of the mesh.
		 * This takes animation into account and therefore calculates
		 * a size Box for the currently active animation.
		 */
		Ogre::AxisAlignedBox calculateSize();
		Ogre::AxisAlignedBox getAabbFromVertexData(Ogre::VertexData*);
    };
}
#endif
