/* 
OgreNewt Library

Ogre implementation of Newton Game Dynamics SDK

OgreNewt basically has no license, you may use any or all of the library however you desire... I hope it can help you in any way.

by Walaber

*/
#ifndef _INCLUDE_OGRENEWT_COLLISIONSERIALIZER
#define _INCLUDE_OGRENEWT_COLLISIONSERIALIZER


#include "OgreNewt_Prerequisites.h"

// OgreNewt namespace.  all functions and classes use this namespace.
namespace OgreNewt
{
	class Collision;

  /*!
  This class can be used to (de)serialize a Collision. Pre-building a Collision and serializing from a tool,
  then deserializing it at runtime may be more efficient than building the Collision on the fly, especially for complex objects.
  */
  class _OgreNewtExport CollisionSerializer : public Ogre::Serializer
  {
  public:

    //! constructor
    CollisionSerializer();

    //! destructor
    virtual ~CollisionSerializer();

    /*!
    Serialize the Collision to a file with the given name.
    */
	void exportCollision(const Collision* collision, const Ogre::String& filename);

    /*!
    Deserialize the Collision from a DataStream.
	this will create a NEW Collision object
    */
    void importCollision(Ogre::DataStreamPtr& stream, Collision* pDest);

  private:
    /*!
    Callback function for Newton. It should never be called directly, but will be called by Newton to save the Collision to a stream.
    (Newton calls this function several times for each serialization, once for each chunk of its file format apparently)
    */
    static void _CDECL _newtonSerializeCallback(void* serializeHandle, const void* buffer, int size);

    /*!
    Callback function for Newton. It should never be called directly, but will be called by Newton to load the Collision from a stream.
    (Newton calls this function several times for each deserialization, once for each chunk of its file format apparently)
    */
    static void _CDECL _newtonDeserializeCallback(void* deserializeHandle, void* buffer, int size);
  };


}	// end NAMESPACE OgreNewt

#endif
// _INCLUDE_OGRENEWT_COLLISIONSERIALIZER

