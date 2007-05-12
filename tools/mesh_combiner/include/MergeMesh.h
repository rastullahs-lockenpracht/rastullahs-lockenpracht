#ifndef __MergeMesh_H__
#define __MergeMesh_H__

#include "MeshCombinerPrerequisites.h"

#include <vector>
#include <OgreMesh.h>
#include <OgreSkeleton.h>

namespace rl 
{
    class _MCExport MergeMesh
	{
		public:
			MergeMesh();
			virtual ~MergeMesh();

            void addMesh( Ogre::MeshPtr mesh );

            Ogre::MeshPtr bake();
		private: 
            const Ogre::String findSubmeshName( Ogre::MeshPtr m, Ogre::ushort sid ) const;
			void log(const Ogre::String& message) const;

            Ogre::SkeletonPtr m_BaseSkeleton;
            std::vector< Ogre::MeshPtr > m_Meshes;
	};
}

#endif

