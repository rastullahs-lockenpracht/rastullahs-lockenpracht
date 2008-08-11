#include <OgreNewt_CollisionSerializer.h>
#include <OgreNewt_Collision.h>

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
    mpfFile=fopen(filename.c_str(),"wb");
    if (!mpfFile)
    {
      OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Unable to open file " + filename + " for writing","CollisionSerializer::exportTreeCollision");
    }
    NewtonCollisionSerialize(collision->getNewtonCollision(), &CollisionSerializer::_newtonSerializeCallback, this);
  }


  void CollisionSerializer::importCollision(Ogre::DataStreamPtr& stream, Collision* pDest)
  {
    NewtonCollision* col = NewtonCreateCollisionFromSerialization(pDest->getWorld()->getNewtonWorld(), &CollisionSerializer::_newtonDeserializeCallback, &stream);
    pDest->m_col=col;
  }


  void CollisionSerializer::_newtonSerializeCallback(void* serializeHandle, const void* buffer, int size)
  {
    CollisionSerializer* me=(static_cast<CollisionSerializer*>(serializeHandle));
    me->writeData(buffer,1,size);
  }


  void CollisionSerializer::_newtonDeserializeCallback(void* deserializeHandle, void* buffer, int size)
  {
    Ogre::DataStreamPtr ptr=*(static_cast<Ogre::DataStreamPtr*>(deserializeHandle));
    ptr->read(buffer,size);
  }
}	// end namespace OgreNewt


