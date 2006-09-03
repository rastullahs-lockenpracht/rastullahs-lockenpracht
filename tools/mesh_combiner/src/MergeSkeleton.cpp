#include "MergeSkeleton.h"

#include <OgreSkeletonManager.h>
#include <OgreHardwareBufferManager.h>
#include <OgreBone.h>
#include <OgreAnimation.h>

#include "MeshCombiner.h"


using namespace Ogre;

namespace rl {

    MergeSkeleton::MergeSkeleton() :
        m_BaseSkeleton(),
        m_Skeletons()
	{
	}

	MergeSkeleton::~MergeSkeleton() 
	{    
        m_Skeletons.clear();
        m_BaseSkeleton.setNull();
	}

    void MergeSkeleton::addSkeleton( Ogre::SkeletonPtr skel )
	{    
        m_Skeletons.push_back( skel );
	}

    SkeletonPtr MergeSkeleton::bake()
    {    
        MeshCombiner::getSingleton().log( 
             "Baking: New Skeleton started" );

        SkeletonPtr sp = SkeletonManager::getSingleton().create( "mergeSkeleton", 
             ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true );
        
        for( std::vector< Ogre::SkeletonPtr >::iterator it = m_Skeletons.begin();
             it != m_Skeletons.end(); ++it )
        {   
            if(  it == m_Skeletons.begin() )
            {
                MeshCombiner::getSingleton().log( 
                    "Baking: using " + (*it)->getName() + " as the base skeleton"   );

                MeshCombiner::getSingleton().log( 
                    "Baking: adding bones"   );
                Skeleton::BoneIterator bit = (*it)->getBoneIterator();
                while( bit.hasMoreElements() )
                {
                    Bone* bone = bit.getNext();
                    Bone* newbone = sp->createBone( bone->getName(), bone->getHandle() );
                    newbone->setScale( bone->getScale() );
                    newbone->setOrientation( bone->getOrientation() );
                    newbone->setPosition( bone->getPosition() );
                }
                MeshCombiner::getSingleton().log( 
                    "Baking: building bone hierarchy"   );
                // bone hierarchy
                bit = (*it)->getBoneIterator();
                while( bit.hasMoreElements() )
                {
                    Bone* bone = bit.getNext();
                    Node* pnode = bone->getParent();
                    if( pnode != NULL )
                    {
                        Bone* pbone = static_cast<Bone*>( pnode );
                        sp->getBone( pbone->getHandle() )->addChild( sp->getBone( bone->getHandle() ) );
                    }
                }
            }   

            MeshCombiner::getSingleton().log( 
                "Baking: adding animations for " + (*it)->getName() );

            // insert all animations
            for (unsigned short a=0; a < (*it)->getNumAnimations(); ++a )
            {
                Animation* anim = (*it)->getAnimation( a );
                Animation* newanim = sp->createAnimation( anim->getName(), anim->getLength() );

                if( anim->getNumNodeTracks() > 0 )
                    MeshCombiner::getSingleton().log( 
                        "Baking: adding node tracks" );
                for( unsigned short na=0; na < anim->getNumNodeTracks(); ++na )
                {
                    if( anim->hasNodeTrack( na ) )
                    {
                        NodeAnimationTrack* nat = anim->getNodeTrack( na );
                        NodeAnimationTrack* newnat = newanim->createNodeTrack( na );
                        // all key frames
                        for( unsigned short nf=0; nf < nat->getNumKeyFrames(); ++nf )
                        {
                            TransformKeyFrame* tkf = nat->getNodeKeyFrame( nf );
                            TransformKeyFrame* newtkf = newnat->createNodeKeyFrame( tkf->getTime() );
                            newtkf->setRotation( tkf->getRotation() );
                            newtkf->setTranslate( tkf->getTranslate() );
                            newtkf->setScale( tkf->getScale() );
                        }

                        newnat->setAssociatedNode( sp->getBone( nat->getHandle() ) );
                    }
                }

                if( anim->getNumNumericTracks() > 0 )
                    MeshCombiner::getSingleton().log( 
                        "Baking: adding numeric tracks" );
                for( unsigned short na=0; na < anim->getNumNumericTracks(); ++na )
                {
                    if( anim->hasNumericTrack( na ) )
                    {
                        NumericAnimationTrack* nat = anim->getNumericTrack( na );
                        NumericAnimationTrack* newnat = newanim->createNumericTrack( na );

                        // all key frames
                        for( unsigned short nf=0; nf < nat->getNumKeyFrames(); ++nf )
                        {
                            NumericKeyFrame* nkf = nat->getNumericKeyFrame( nf );
                            NumericKeyFrame* newnkf = newnat->createNumericKeyFrame( nkf->getTime() );
                            newnkf->setValue( nkf->getValue() );
                        }
                    }
                }

                if( anim->getNumVertexTracks() > 0 )
                    MeshCombiner::getSingleton().log( 
                        "Baking: adding vertex tracks" );
                for( unsigned short va=0; va < anim->getNumVertexTracks(); ++va )
                {
                    if( anim->hasVertexTrack( va ) )
                    {
                        VertexAnimationTrack* vat = anim->getVertexTrack( va );
                        VertexAnimationTrack* newvat = newanim->createVertexTrack( va, vat->getAnimationType() );

                        // all key frames
                        for( unsigned short nf=0; nf < vat->getNumKeyFrames(); ++nf )
                        {
                            // all morphs
                            VertexMorphKeyFrame* vmkf = vat->getVertexMorphKeyFrame( nf );
                            if( vmkf != NULL )
                            {
                                VertexMorphKeyFrame* newvmkf = newvat->createVertexMorphKeyFrame( vmkf->getTime() );
                                // @todo vertex buffer copying correct??
                                HardwareVertexBufferSharedPtr buf = vmkf->getVertexBuffer();
                                HardwareVertexBufferSharedPtr newbuf = HardwareBufferManager::getSingleton().createVertexBuffer(
                                    buf->getVertexSize(), buf->getNumVertices(), buf->getUsage(), buf->hasShadowBuffer() );
                                newbuf->copyData( *buf.getPointer(), 0, 0, buf->getSizeInBytes() );
                            }

                            // all poses
                            VertexPoseKeyFrame* vpkf = vat->getVertexPoseKeyFrame( nf );
                            if( vpkf != NULL )
                            {
                                VertexPoseKeyFrame* newvpkf = newvat->createVertexPoseKeyFrame( vpkf->getTime() );
                                
                                VertexPoseKeyFrame::PoseRefIterator pit = vpkf->getPoseReferenceIterator();
                                while( pit.hasMoreElements() )
                                {
                                    VertexPoseKeyFrame::PoseRef pr = pit.getNext();
                                    newvpkf->addPoseReference( pr.poseIndex, pr.influence );
                                }
                            }

                        }
                    }
                }
            }
        }           

        return sp;
	}
}
