#include "MeshCombiner.h"


#include <OgreRoot.h>
#include <OgreTextureManager.h>
#include <OgreMaterialManager.h>
#include <OgreMeshManager.h>
#include <OgreLogManager.h>
#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>
#include <OgreParticleSystemManager.h>
#include <OgreDefaultHardwareBufferManager.h>
#include <OgreBillboardParticleRenderer.h>
#include <OgreMeshSerializer.h>
#include <OgreSkeletonSerializer.h>
#include <OgreSkeletonManager.h>
#include <OgreEntity.h>
#include <OgreSceneNode.h>

#include "MergeMesh.h"
#include "MergeSkeleton.h"

using namespace Ogre;

template<> rl::MeshCombiner* Singleton<rl::MeshCombiner>::msSingleton = nullptr;

namespace rl {

	MeshCombiner& MeshCombiner::getSingleton()
	{
		return Singleton<MeshCombiner>::getSingleton();
	}

	MeshCombiner* MeshCombiner::getSingletonPtr()
	{
		return Singleton<MeshCombiner>::getSingletonPtr();
	}

	MeshCombiner::MeshCombiner()
	{
        m_MeshCombinerConfig = new ConfigFile();
		// Configuration einladen
		m_MeshCombinerConfig->loadDirect("media/cfg/meshcombiner.cfg","=");

		initializeSettings();
        consoleMeshCombiner();
        consoleSkeletonCombiner();
	}

	MeshCombiner::~MeshCombiner() 
	{    
        delete Root::getSingletonPtr();
	}

    ConfigFile* MeshCombiner::getConfig() const
    {
        return m_MeshCombinerConfig;
    }

    void MeshCombiner::consoleMeshCombiner()
    {
        StringVector vec = m_MeshCombinerConfig->getMultiSetting( "Mesh" );
        if( vec.empty() )
            return;

        MergeMesh* mm = new MergeMesh();
        SkeletonPtr skel = SkeletonPtr(); 

        for( StringVector::iterator it = vec.begin();
             it != vec.end(); ++it )
        {
            log( "Loading: " + *it );
            try
            {
                MeshPtr mesh = MeshManager::getSingleton().load(
                    *it, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );

                if( !mesh.isNull() )
                    mm->addMesh( mesh );
            }
            catch( ... )
            {
            }
        }

        // save
        MeshPtr mesh = mm->bake();
        
        MeshSerializer* meshSerializer = new MeshSerializer();
        meshSerializer->exportMesh( mesh.getPointer(), "./media/merged.mesh" );

        MeshManager::getSingleton().remove( mesh->getHandle() );

        // try to load...
        mesh = MeshManager::getSingleton().load(
                    "merged.mesh", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
        
        SceneManager* sm = Root::getSingleton().createSceneManager( ST_GENERIC );       

        // try to place...
        sm->getRootSceneNode()->attachObject( sm->createEntity( "test", "merged.mesh" ) );

        delete meshSerializer;
        delete mm;
    }

    void MeshCombiner::consoleSkeletonCombiner()
    {
        StringVector vec = m_MeshCombinerConfig->getMultiSetting( "Skeleton" );
        if( vec.empty() )
            return;

        MergeSkeleton* ms = new MergeSkeleton();

        for( StringVector::iterator it = vec.begin();
             it != vec.end(); ++it )
        {
            log( "Loading: " + *it );
            try
            {
                SkeletonPtr skel = SkeletonManager::getSingleton().getByName(
                    *it, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );

                if( !skel.isNull() )
                    ms->addSkeleton( skel );
            }
            catch( ... )
            {
            }
        }

        // save
        SkeletonPtr skel = ms->bake();
        
        SkeletonSerializer* skelSerializer = new SkeletonSerializer();
        skelSerializer->exportSkeleton( skel.getPointer(), "./media/merged.skeleton" );

        SkeletonManager::getSingleton().remove( skel->getHandle() );

        delete skelSerializer;
        delete ms;
    }

	void MeshCombiner::initializeSettings()
	{	
		// Ogre Root erzeugen
		new Root(
			"media/cfg/ogre_plugins.cfg", 
			"media/cfg/ogre_meshcombiner.cfg", 
			"logs/ogre.log"
		);


		// Konfigurationsdialog anzeigen/oder alte Konfiguration laden
		if( Root::getSingleton().restoreConfig()
            || Root::getSingleton().showConfigDialog(nullptr) )
		{
			Root::getSingleton().initialise( false, "MeshCombiner" );            
		}
		else
		{
			exit(1);
		}

		// Logdatei erzeugen
		m_Log = LogManager::getSingleton().createLog( "logs/meshcombiner.log"	);
		m_Log->setLogDetail( LL_BOREME );


		// Verzeichnisse
		ResourceGroupManager::getSingleton().addResourceLocation( "media/", "FileSystem" );

        new DefaultHardwareBufferManager(); // needed because we don't have a rendersystem

      
		ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
        ResourceGroupManager::getSingleton().loadResourceGroup( ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
	}

	void MeshCombiner::log(const String& msg, LogMessageLevel lvl )
	{
		m_Log->logMessage(msg,lvl);
	}
}

