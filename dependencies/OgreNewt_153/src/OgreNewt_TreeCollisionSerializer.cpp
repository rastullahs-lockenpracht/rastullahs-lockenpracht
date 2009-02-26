#include <OgreNewt_Prerequisites.h>
#include <OgreNewt_TreeCollisionSerializer.h>
#include <OgreNewt_CollisionPrimitives.h>

namespace OgreNewt
{
  TreeCollisionSerializer::TreeCollisionSerializer()
  {
  }


  TreeCollisionSerializer::~TreeCollisionSerializer()
  {
  }


  void TreeCollisionSerializer::exportTreeCollision(const CollisionPrimitives::TreeCollision* collision, const Ogre::String& filename)
  {
    mpfFile=fopen(filename.c_str(),"wb");
    if (!mpfFile)
    {
      OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS, "Unable to open file " + filename + " for writing","TreeCollisionSerializer::exportTreeCollision");
    }
    NewtonTreeCollisionSerialize(collision->m_col,&TreeCollisionSerializer::_newtonSerializeCallback,this);
  }


  void TreeCollisionSerializer::importTreeCollision(Ogre::DataStreamPtr& stream, CollisionPrimitives::TreeCollision* pDest)
  {
    NewtonCollision* col=NewtonCreateTreeCollisionFromSerialization(pDest->getWorld()->getNewtonWorld(), NULL, &TreeCollisionSerializer::_newtonDeserializeCallback, &stream);
    pDest->m_col=col;
  }


  void TreeCollisionSerializer::_newtonSerializeCallback(void* serializeHandle, const void* buffer, size_t size)
  {
    TreeCollisionSerializer* me=(static_cast<TreeCollisionSerializer*>(serializeHandle));
    me->writeData(buffer,1,size);
  }


  void TreeCollisionSerializer::_newtonDeserializeCallback(void* deserializeHandle, void* buffer, size_t size)
  {
    Ogre::DataStreamPtr ptr=*(static_cast<Ogre::DataStreamPtr*>(deserializeHandle));
    ptr->read(buffer,size);
  }
}	// end namespace OgreNewt


