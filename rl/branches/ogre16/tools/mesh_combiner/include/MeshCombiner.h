#ifndef __MeshCombiner_H__
#define __MeshCombiner_H__

#include <OgreSingleton.h>

#include <OgreLog.h>
#include <OgreConfigFile.h>


namespace rl {
    class MeshCombiner : protected Ogre::Singleton<MeshCombiner>
	{
		public:
			MeshCombiner();
			virtual ~MeshCombiner();

			/** Returns the Singleton */
			static MeshCombiner & getSingleton();
			static MeshCombiner * getSingletonPtr();

			void log(const Ogre::String& msg, 
				Ogre::LogMessageLevel lvl = Ogre::LML_NORMAL );
            Ogre::ConfigFile* getConfig() const;

			void initializeSettings();

            /// simple method for merging meshes via config file
            void consoleMeshCombiner();
            /// simple method for merging skeletons via config file
            void consoleSkeletonCombiner();
		private: 
			Ogre::ConfigFile* m_MeshCombinerConfig;
            Ogre::Log* m_Log;
	};
}

#endif

