#include "MergeMesh.h"

#include <OgreMeshManager.h>
#include <OgreSubmesh.h>

#include "MeshCombiner.h"


using namespace Ogre;

namespace rl {

    MergeMesh::MergeMesh() :
        m_BaseSkeleton(),
        m_Meshes()
	{
	}

	MergeMesh::~MergeMesh() 
	{    
        m_Meshes.clear();
        m_BaseSkeleton.setNull();
	}

    void MergeMesh::addMesh( Ogre::MeshPtr mesh )
	{    
        if( mesh->getSkeleton().isNull() )
        {
            MeshCombiner::getSingleton().log( "Skipped: " + mesh->getName() + " has no skeleton" );
            return;
        }

        if( m_BaseSkeleton.isNull() )
        {
            m_BaseSkeleton = mesh->getSkeleton();                    
            MeshCombiner::getSingleton().log( "Set: base skeleton (" + m_BaseSkeleton->getName()+")" );
        }

        if( mesh->getSkeleton() != m_BaseSkeleton )
        {
            MeshCombiner::getSingleton().log( "Skipped: " + mesh->getName() + " has other skeleton ("+ mesh->getSkeleton()->getName() +")" );
            return;
        }

        m_Meshes.push_back( mesh );
	}

    const String MergeMesh::findSubmeshName( MeshPtr m, ushort sid ) const
    {
        Mesh::SubMeshNameMap map = m->getSubMeshNameMap();
        for( Mesh::SubMeshNameMap::const_iterator it = map.begin();
             it != map.end(); ++it )
        {
            if( it->second == sid )
                return it->first;
        }

        return "";
    }   

    MeshPtr MergeMesh::bake()
    {    
         MeshCombiner::getSingleton().log( 
             "Baking: New Mesh started" );

        MeshPtr mp = MeshManager::getSingleton().
            createManual( "bakeMono", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
        mp->setSkeletonName( m_BaseSkeleton->getName() );

        AxisAlignedBox totalBounds = AxisAlignedBox();
        for( std::vector< Ogre::MeshPtr >::iterator it = m_Meshes.begin();
             it != m_Meshes.end(); ++it )
        {   
            MeshCombiner::getSingleton().log( 
                "Baking: adding submeshes for " + (*it)->getName() );

            // insert all submeshes
            for( ushort sid = 0; sid < (*it)->getNumSubMeshes(); ++sid )
            {
                SubMesh* sub = (*it)->getSubMesh( sid );
                const String name = findSubmeshName( (*it), sid );                
                
                // create submesh with correct name                
                SubMesh* newsub;
                if( name.length() == 0 )
                    newsub = mp->createSubMesh(  );
                else /// @todo check if a submesh with this name has been created before
                    newsub = mp->createSubMesh( name );



                newsub->useSharedVertices = sub->useSharedVertices;
                // add geometry
                newsub->indexData = sub->indexData->clone();
                newsub->vertexData = sub->vertexData->clone();

                // build bone assignments
                SubMesh::BoneAssignmentIterator bit = sub->getBoneAssignmentIterator();
                while( bit.hasMoreElements() )
                {
                    VertexBoneAssignment vba = bit.getNext();

                    newsub->addBoneAssignment( vba );
                }

                newsub->_compileBoneAssignments();
                newsub->setMaterialName( sub->getMaterialName() );

                MeshCombiner::getSingleton().log(  
                    "Baking: adding submesh '" + name + "'  with material " + sub->getMaterialName() );
            } 

            /// @todo sharedvertices and stuff...

            MeshCombiner::getSingleton().log( 
                "Baking: adding bounds for " + (*it)->getName() );

            // add bounds
            Vector3 oldmin = totalBounds.getMinimum();
            Vector3 newmin = (*it)->getBounds().getMinimum();
            Vector3 oldmax = totalBounds.getMaximum();
            Vector3 newmax = (*it)->getBounds().getMaximum();
            totalBounds = AxisAlignedBox( 
                std::min( oldmin.x, newmin.x ), std::min( oldmin.y, newmin.y  ), std::min( oldmin.z, newmin.z  ),
                std::max( oldmax.x, newmax.x ), std::max( oldmax.y, newmax.y  ), std::max( oldmax.z, newmax.z  )
                );
        }

        
        mp->_setBounds( totalBounds );

        MeshCombiner::getSingleton().log( 
            "Baking: Finished" );

        return mp;
	}


}
