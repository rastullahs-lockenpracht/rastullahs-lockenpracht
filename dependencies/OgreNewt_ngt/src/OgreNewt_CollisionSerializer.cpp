#include "OgreNewt_CollisionSerializer.h"
#include "OgreNewt_Collision.h"

#include "Ogre.h"

namespace OgreNewt
{
  CollisionSerializer::CollisionSerializer()
  {
  }


  CollisionSerializer::~CollisionSerializer()
  {
  }


  void CollisionSerializer::exportCollision(const Collision* collision, const Ogre::String& filename)
  {
    if( collision )
    {
        mpfFile=fopen(filename.c_str(),"wb");
    
        if (!mpfFile)
        {
          OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Unable to open file " + filename + " for writing","CollisionSerializer::exportCollision");
        }
    
        NewtonCollisionSerialize(collision->getWorld()->getNewtonWorld(), collision->m_col, &CollisionSerializer::_newtonSerializeCallback, this);

        fclose(mpfFile);
    }
  }


  void CollisionSerializer::importCollision(Ogre::DataStreamPtr& stream, Collision* pDest)
  {
    if( pDest )
    {
        if( pDest->m_col )
        {
            // we need an error here!!
            NewtonReleaseCollision(pDest->m_world->getNewtonWorld(), pDest->m_col);
        }

        NewtonCollision* col = NewtonCreateCollisionFromSerialization(pDest->getWorld()->getNewtonWorld(), &CollisionSerializer::_newtonDeserializeCallback, &stream);

        pDest->m_col = col;
        // this is a bit nasty, but we need to add a reference for the collision, there should be no problems,
        // if the OgreNewt-functions are used and the newton-collisions are not accessed directly in any way
        NewtonAddCollisionReference(pDest->m_col);
    }
  }


  void CollisionSerializer::_newtonSerializeCallback(void* serializeHandle, const void* buffer, int size)
  {
    CollisionSerializer* me = (static_cast<CollisionSerializer*>(serializeHandle));
    me->writeData(buffer, 1, size);
  }


  void CollisionSerializer::_newtonDeserializeCallback(void* deserializeHandle, void* buffer, int size)
  {
    Ogre::DataStreamPtr ptr =* (static_cast<Ogre::DataStreamPtr*>(deserializeHandle));
    ptr->read(buffer, size);
  }
}	// end namespace OgreNewt


