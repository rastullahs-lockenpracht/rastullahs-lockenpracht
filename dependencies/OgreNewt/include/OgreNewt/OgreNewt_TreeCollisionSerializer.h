/* 
OgreNewt Library

Ogre implementation of Newton Game Dynamics SDK

OgreNewt basically has no license, you may use any or all of the library however you desire... I hope it can help you in any way.

by Walaber

*/
#ifndef _INCLUDE_OGRENEWT_TREECOLLISIONSERIALIZER
#define _INCLUDE_OGRENEWT_TREECOLLISIONSERIALIZER

#include <Newton.h>
#include "OgreNewt_World.h"
#include "OgreSerializer.h"
#include "OgreString.h"

// OgreNewt namespace.  all functions and classes use this namespace.
namespace OgreNewt
{
  namespace CollisionPrimitives
  {
    class TreeCollision;
  }

  /*!
  This class can be used to (de)serialize a TreeCollision. Pre-building a TreeCollision and serializing from a tool,
  then deserializing it at runtime may be more efficient than building the TreeCollision on the fly, especially for complex objects.
  */
  class _OgreNewtExport TreeCollisionSerializer : public Ogre::Serializer
  {
  public:

    //! constructor
    TreeCollisionSerializer();

    //! destructor
    virtual ~TreeCollisionSerializer();

    /*!
    Serialize the TreeCollision to a file with the given name.
    */
    void exportTreeCollision(const CollisionPrimitives::TreeCollision* collision, const Ogre::String& filename);

    /*!
    Deserialize the TreeCollision from a DataStream.
    */
    void importTreeCollision(Ogre::DataStreamPtr& stream, CollisionPrimitives::TreeCollision* pDest);

    /*!
    Callback function for Newton. It should never be called directly, but will be called by Newton to save the TreeCollision to a stream.
    (Newton calls this function several times for each serialization, once for each chunk of its file format apparently)
    */
    static void _CDECL _newtonSerializeCallback(void* serializeHandle, const void* buffer, size_t size);

    /*!
    Callback function for Newton. It should never be called directly, but will be called by Newton to load the TreeCollision from a stream.
    (Newton calls this function several times for each deserialization, once for each chunk of its file format apparently)
    */
    static void _CDECL _newtonDeserializeCallback(void* deserializeHandle, void* buffer, size_t size);
  };


}	// end NAMESPACE OgreNewt

#endif
// _INCLUDE_OGRENEWT_TREECOLLISIONSERIALIZER

