#include "MergeMesh.h"

#include <OgreMeshManager.h>
#include <OgreHardwareBufferManager.h>
#include <OgreSubMesh.h>
#include <OgreAxisAlignedBox.h>

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
			log( "Skipped: " + mesh->getName() + " has no skeleton" );
		    return;
        }

        if( m_BaseSkeleton.isNull() )
        {
            m_BaseSkeleton = mesh->getSkeleton();                    
            log( "Set: base skeleton (" + m_BaseSkeleton->getName()+")" );
        }

        if( mesh->getSkeleton() != m_BaseSkeleton )
        {
	        log( "Skipped: " + mesh->getName() + " has other skeleton ("+ mesh->getSkeleton()->getName() +")" );
            return;
        }

        m_Meshes.push_back( mesh );
	}

    const String MergeMesh::findSubmeshName( MeshPtr m, Ogre::ushort sid ) const
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
        log( 
             "Baking: New Mesh started" );

        MeshPtr mp = MeshManager::getSingleton().
            createManual( "mergedMesh", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
        mp->setSkeletonName( m_BaseSkeleton->getName() );

        AxisAlignedBox totalBounds = AxisAlignedBox();
        for( std::vector< Ogre::MeshPtr >::iterator it = m_Meshes.begin();
             it != m_Meshes.end(); ++it )
        {   
            log( 
                "Baking: adding submeshes for " + (*it)->getName() );

            // insert all submeshes
            for( Ogre::ushort sid = 0; sid < (*it)->getNumSubMeshes(); ++sid )
            {
                SubMesh* sub = (*it)->getSubMesh( sid );
                const String name = findSubmeshName( (*it), sid );                
                
                // create submesh with correct name                
                SubMesh* newsub;
                if( name.length() == 0 )
                    newsub = mp->createSubMesh(  );
                else 
                /// @todo check if a submesh with this name has been created before
                    newsub = mp->createSubMesh( name );   

                newsub->useSharedVertices = sub->useSharedVertices;

                // add index
                newsub->indexData = sub->indexData->clone();

                // add geometry
                if( !newsub->useSharedVertices )
                {
                    newsub->vertexData = sub->vertexData->clone();
                
                    // build bone assignments
                    SubMesh::BoneAssignmentIterator bit = sub->getBoneAssignmentIterator();
                    while (bit.hasMoreElements())
                    {
                        VertexBoneAssignment vba = bit.getNext();
                        newsub->addBoneAssignment(vba);
                    }
                }

                newsub->setMaterialName( sub->getMaterialName() );

                log("Baking: adding submesh '" + name + "'  with material " + sub->getMaterialName());
            } 

            // sharedvertices
            if ((*it)->sharedVertexData)
            {
                /// @todo merge with existing sharedVertexData
                if (!mp->sharedVertexData)
				{
					mp->sharedVertexData = (*it)->sharedVertexData->clone();
				}

                Mesh::BoneAssignmentIterator bit = (*it)->getBoneAssignmentIterator();
                while (bit.hasMoreElements())
                {
                    VertexBoneAssignment vba = bit.getNext();
                    mp->addBoneAssignment(vba);
                }
            }

            log("Baking: adding bounds for " + (*it)->getName());

            // add bounds
            totalBounds.merge((*it)->getBounds());
        }           
        mp->_setBounds( totalBounds );

        /// @todo merge submeshes with same material


        /// @todo add parameters
        mp->buildEdgeList();

        log( 
            "Baking: Finished" );

        return mp;
	}

                /*
                bool use32BitIndexes = 
                    (sub->indexData->indexBuffer->getType() == HardwareIndexBuffer::IT_32BIT);
                newsub->operationType = sub->operationType;
                newsub->indexData->indexCount = sub->indexData->indexCount;
                
                HardwareIndexBufferSharedPtr newibuf = 
                    HardwareBufferManager::getSingleton().createIndexBuffer(
                        use32BitIndexes ? HardwareIndexBuffer::IT_32BIT : HardwareIndexBuffer::IT_16BIT, 
                        newsub->indexData->indexCount, 
                        HardwareBuffer::HBU_DYNAMIC,
                        false );
                newsub->indexData->indexBuffer = newibuf;
                HardwareIndexBufferSharedPtr oldibuf = sub->indexData->indexBuffer;
                unsigned int *pNewInt, *pOldInt;
                unsigned short *pNewShort, *pOldShort;
                if( use32BitIndexes )
                {
                    pNewInt = static_cast<unsigned int*>(newibuf->lock(HardwareBuffer::HBL_DISCARD) );
                    pOldInt = static_cast<unsigned int*>(oldibuf->lock(HardwareBuffer::HBL_READ_ONLY) );
                }
                else
                {
                    pNewShort = static_cast<unsigned short*>(newibuf->lock(HardwareBuffer::HBL_DISCARD) );
                    pOldShort = static_cast<unsigned short*>(oldibuf->lock(HardwareBuffer::HBL_READ_ONLY) );
                }
                for( unsigned int iid = 0; iid < newsub->indexData->indexCount; ++iid )
                {
                    if( use32BitIndexes )
                        *pNewInt++ = *pOldInt++;
                    else
                        *pNewShort++ = *pOldShort++;
                }
                newibuf->unlock();
                oldibuf->unlock();    
                        



                    new VertexData();                                                                  
                    newsub->vertexData->vertexCount = sub->vertexData->vertexCount;

                    if( newsub->vertexData->vertexCount <= 0 )
                        break;

                    VertexDeclaration* decl = newsub->vertexData->vertexDeclaration;
                    VertexBufferBinding* bind = newsub->vertexData->vertexBufferBinding;*/


	void MergeMesh::log(const Ogre::String& message) const
	{
		if (MeshCombiner::getSingletonPtr())
		{
			MeshCombiner::getSingleton().log(message);
		}
	}
}

