#ifndef __MERGESKELETON_H__
#define __MERGESKELETON_H__

#include <vector>
#include <OgreSkeleton.h>

namespace rl {
    class MergeSkeleton
	{
		public:
			MergeSkeleton();
			virtual ~MergeSkeleton();

            void addSkeleton( Ogre::SkeletonPtr skel );

            Ogre::SkeletonPtr bake();
		private: 
            Ogre::SkeletonPtr m_BaseSkeleton;
            std::vector< Ogre::SkeletonPtr > m_Skeletons;
	};
}

#endif