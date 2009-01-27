#include "OgreNewt_RayCast.h"
#include "OgreNewt_Tools.h"
#include "OgreNewt_Debugger.h"


namespace OgreNewt
{


	Raycast::Raycast()	{}
	Raycast::~Raycast()	{}


	void Raycast::go(const OgreNewt::World* world, const Ogre::Vector3& startpt, const Ogre::Vector3& endpt )
	{
        if( Debugger::getSingleton().isRaycastRecording() )
        {
            Debugger::getSingleton().addRay(startpt, endpt);
        }
		// perform the raycast!
		NewtonWorldRayCast( world->getNewtonWorld(), (float*)&startpt, (float*)&endpt, OgreNewt::Raycast::newtonRaycastFilter, this, OgreNewt::Raycast::newtonRaycastPreFilter );
	}

	float _CDECL Raycast::newtonRaycastFilter(const NewtonBody* body, const float* hitNormal, int collisionID, void* userData, float intersectParam)
	{
		// get our object!
		Raycast* me = (Raycast*)userData;

		Body* bod = (Body*)NewtonBodyGetUserData( body );
		Ogre::Vector3 normal = Ogre::Vector3( hitNormal[0], hitNormal[1], hitNormal[2] );


        if( Debugger::getSingleton().isRaycastRecording() && Debugger::getSingleton().isRaycastRecordingHitBodies() )
        {
            Debugger::getSingleton().addHitBody(bod);
        }


		if (me->userCallback( bod, intersectParam, normal, collisionID ))
			return intersectParam;
		else
			return 1.1;

	}

	unsigned _CDECL Raycast::newtonRaycastPreFilter(const NewtonBody *body, const NewtonCollision *collision, void* userData)
	{
		// get our object!
		Raycast* me = (Raycast*)userData;

		Body* bod = (Body*)NewtonBodyGetUserData( body );

		if (me->userPreFilterCallback( bod ))
			return 1;
		else
        {

            if( Debugger::getSingleton().isRaycastRecording() && Debugger::getSingleton().isRaycastRecordingHitBodies() )
            {
                Debugger::getSingleton().addDiscardedBody(bod);
            }

			return 0;
        }
	}



	//--------------------------------
	BasicRaycast::BasicRaycastInfo::BasicRaycastInfo()
	{
		mBody = NULL;
		mDistance = -1.0;
		mNormal = Ogre::Vector3::ZERO;
	}

	BasicRaycast::BasicRaycastInfo::~BasicRaycastInfo() {}


	BasicRaycast::BasicRaycast(const OgreNewt::World* world, const Ogre::Vector3& startpt, const Ogre::Vector3& endpt, bool sorted)
        : Raycast()
	{
		go( world, startpt, endpt );
                if( sorted )
                {
                    std::sort(mRayList.begin(), mRayList.end());
                }
	}

	BasicRaycast::~BasicRaycast()	{}


	int BasicRaycast::getHitCount() const { return (int)mRayList.size(); }


	BasicRaycast::BasicRaycastInfo BasicRaycast::getFirstHit() const
	{
		//return the closest hit...
		BasicRaycast::BasicRaycastInfo ret;

		Ogre::Real dist = Ogre::Math::POS_INFINITY;

		RaycastInfoList::const_iterator it;
		for (it = mRayList.begin(); it != mRayList.end(); it++)
		{
			if (it->mDistance < dist)
			{
				dist = it->mDistance;
				ret = (*it);
			}
		}


		return ret;
	}


	BasicRaycast::BasicRaycastInfo BasicRaycast::getInfoAt( int hitnum ) const
	{
		BasicRaycast::BasicRaycastInfo ret;

		if ((hitnum < 0) || (hitnum >= mRayList.size()))
			return ret;

		ret = mRayList.at(hitnum);

		return ret;
	}

	bool BasicRaycast::userCallback( OgreNewt::Body* body, Ogre::Real distance, const Ogre::Vector3& normal, int collisionID )
	{
		// create a new infor object.
		BasicRaycast::BasicRaycastInfo newinfo;

		newinfo.mBody = body;
		newinfo.mDistance = distance;
		newinfo.mNormal = normal;
		newinfo.mCollisionID = collisionID;

		mRayList.push_back( newinfo );

		return false;
	}




//-------------------------------------------------------------------------------------
        Convexcast::Convexcast() :
            mReturnInfoList(NULL),
            mReturnInfoListLength(0),
            mReturnInfoListSize(0)
        {
        }

	Convexcast::~Convexcast()
        {
            if( mReturnInfoList )
            {
                delete[] mReturnInfoList;
            }
        }


	void Convexcast::go(const OgreNewt::World* world, const OgreNewt::Collision *col, const Ogre::Vector3& startpt, const Ogre::Quaternion &colori, const Ogre::Vector3& endpt, int maxcontactscount)
	{

        if( Debugger::getSingleton().isRaycastRecording() )
        {
            Debugger::getSingleton().addConvexRay(col, startpt, colori, endpt);
        }
                // reserve memory
                if( mReturnInfoListSize < maxcontactscount )
                {
                    mReturnInfoListSize = 0;
                    if( mReturnInfoList )
                        delete[] mReturnInfoList;
                    mReturnInfoList = new NewtonWorldConvexCastReturnInfo[maxcontactscount];
                    mReturnInfoListSize = maxcontactscount;
                }

		// perform the cast
                float matrix[16];
                OgreNewt::Converters::QuatPosToMatrix(colori, startpt, &matrix[0] );
                mFirstContactDistance = -1;
                mReturnInfoListLength = 
                        NewtonWorldConvexCast( world->getNewtonWorld(), &matrix[0], (float*)&endpt, col->getNewtonCollision(),
                                               &mFirstContactDistance, this, OgreNewt::Convexcast::newtonConvexcastPreFilter,
                                               mReturnInfoList, mReturnInfoListSize );

            if( Debugger::getSingleton().isRaycastRecording() && Debugger::getSingleton().isRaycastRecordingHitBodies() )
            {
                for(int i = 0; i < mReturnInfoListLength; i++)
                {
                    Body* body = (OgreNewt::Body*) NewtonBodyGetUserData(mReturnInfoList[i].m_hitBody);
                    Debugger::getSingleton().addHitBody(body);
                }
            }
	}

	unsigned _CDECL Convexcast::newtonConvexcastPreFilter(const NewtonBody *body, const NewtonCollision *collision, void* userData)
	{
		// get our object!
		Convexcast* me = (Convexcast*)userData;

		Body* bod = (Body*)NewtonBodyGetUserData( body );

		if (me->userPreFilterCallback( bod ))
			return 1;
		else
        {

            if( Debugger::getSingleton().isRaycastRecording() && Debugger::getSingleton().isRaycastRecordingHitBodies() )
            {
                Debugger::getSingleton().addDiscardedBody(bod);
            }

			return 0;
        }
	}



//-------------------------------------------------------------------------------------
	BasicConvexcast::ConvexcastContactInfo::ConvexcastContactInfo()
	{
		mBody = NULL;
		mDistance = -1.0;
	}

	BasicConvexcast::ConvexcastContactInfo::~ConvexcastContactInfo() {}

        BasicConvexcast::BasicConvexcast()
        {
        }

	BasicConvexcast::BasicConvexcast(const OgreNewt::World* world, const OgreNewt::Collision *col, const Ogre::Vector3& startpt, const Ogre::Quaternion &colori, const Ogre::Vector3& endpt, int maxcontactscount)
	{
		go( world, col, startpt, colori, endpt, maxcontactscount);
	}

	BasicConvexcast::~BasicConvexcast()	{}


	int BasicConvexcast::calculateBodyHitCount() const
        {
            int count = 0;
            const NewtonBody * bod = NULL;
            for( int i = 0; i < mReturnInfoListLength; i++ )
            {
                if( bod != mReturnInfoList[i].m_hitBody )
                {
                    count++;
                    bod = mReturnInfoList[i].m_hitBody;
                }
            }

            return count;
        }


        int BasicConvexcast::getContactsCount() const
        {
            return mReturnInfoListLength;
        }

	BasicConvexcast::ConvexcastContactInfo BasicConvexcast::getInfoAt( int hitnum ) const
	{
            ConvexcastContactInfo info;

            if( hitnum < 0 || hitnum >= mReturnInfoListLength )
                return info;


            info.mDistance = mReturnInfoList[hitnum].m_intersectionParam;
            info.mBody = (OgreNewt::Body*) NewtonBodyGetUserData(mReturnInfoList[hitnum].m_hitBody);
            info.mCollisionID = mReturnInfoList[hitnum].m_contactID;
            info.mContactNormal.x = mReturnInfoList[hitnum].m_normal[0];
            info.mContactNormal.y = mReturnInfoList[hitnum].m_normal[1];
            info.mContactNormal.z = mReturnInfoList[hitnum].m_normal[2];
            info.mContactPoint.x = mReturnInfoList[hitnum].m_point[0];
            info.mContactPoint.y = mReturnInfoList[hitnum].m_point[1];
            info.mContactPoint.z = mReturnInfoList[hitnum].m_point[2];
            info.mContactPenetration = mReturnInfoList[hitnum].m_penetration;

	    return info;
	}

        Ogre::Real BasicConvexcast::getDistanceToFirstHit() const
        {
            return mFirstContactDistance;
        }

}	// end NAMESPACE OgreNewt

