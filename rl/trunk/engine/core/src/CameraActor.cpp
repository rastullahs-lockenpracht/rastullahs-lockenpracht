/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2004 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */

#include "CameraActor.h"

namespace rl {

const String CameraActor::TYPENAME="CameraActor";

CameraActor::CameraActor( const String& name, Camera* pCamera) : Actor(name)
{
	mCamera = pCamera;
	mSceneNode->attachObject(mCamera);
	mCamera->setUserObject(this);
}

CameraActor::CameraActor( const String& name, SceneNode* pParentSceneNode, Camera* pCamera) 
	: Actor(name,pParentSceneNode)
{
	mCamera = pCamera;
	mSceneNode->attachObject(mCamera);
	mCamera->setUserObject(this);
}

CameraActor::~CameraActor()
{
	mSceneNode->detachObject(mCamera->getName()); 
    mSceneNode->getCreator()->removeCamera(mCamera);
}

void CameraActor::setOrientation(const Quaternion& orientation)
{
	// Set on camera
	mCamera->setOrientation(orientation);
}
//-----------------------------------------------------------------------
const Quaternion& CameraActor::getOrientation(void)
{
	return mCamera->getOrientation();
}
//-----------------------------------------------------------------------
void CameraActor::roll(Real angle)
{
	mCamera->roll(Degree(angle));
}
//-----------------------------------------------------------------------
void CameraActor::pitch(Real angle)
{
	mCamera->pitch(Degree(angle));
}
//-----------------------------------------------------------------------
void CameraActor::yaw(Real angle)
{
	mCamera->yaw(Degree(angle));
}
//-----------------------------------------------------------------------
void CameraActor::rotate(const Vector3& axis, Real angle)
{
	mCamera->rotate(axis, Degree(angle));
}
//-----------------------------------------------------------------------
void CameraActor::rotate(const Quaternion& q)
{
	mCamera->rotate(q);
}
//-----------------------------------------------------------------------
void CameraActor::translate(const Vector3& d)
{
	// Adjust position by rotation
	Vector3 newTrans = mCamera->getOrientation() * d;
	translateWorldSpace(newTrans);

}
//-----------------------------------------------------------------------
void CameraActor::setProjectionType(ProjectionType pt)
{
	mCamera->setProjectionType(pt);
}
//-----------------------------------------------------------------------
ProjectionType CameraActor::getProjectionType(void) const
{
	return mCamera->getProjectionType();
}
//-----------------------------------------------------------------------
void CameraActor::setDetailLevel(SceneDetailLevel sd)
{
	mCamera->setDetailLevel(sd);
}
//-----------------------------------------------------------------------
SceneDetailLevel CameraActor::getDetailLevel(void) const
{
	return mCamera->getDetailLevel();
}
//-----------------------------------------------------------------------
void CameraActor::setDirection(Real x, Real y, Real z)
{
	mCamera->setDirection(x, y, z);

}
//-----------------------------------------------------------------------
void CameraActor::setDirection(const Vector3& vec)
{
	mCamera->setDirection(vec);
}
//-----------------------------------------------------------------------
Vector3 CameraActor::getDirection(void) const
{
	return mCamera->getDirection();
}
//-----------------------------------------------------------------------
void CameraActor::lookAt( const Vector3& targetPoint )
{
	mCamera->lookAt(targetPoint);
}
//-----------------------------------------------------------------------
void CameraActor::lookAt(Real x, Real y, Real z)
{
	mCamera->lookAt(x, y, z);
}
//-----------------------------------------------------------------------
void CameraActor::setFixedYawAxis( bool useFixed, const Vector3& fixedAxis)
{
	mCamera->setFixedYawAxis(useFixed, fixedAxis);
}
//-----------------------------------------------------------------------
void CameraActor::setFOVy(Real fovy)
{
	mCamera->setFOVy(Degree(fovy));
}
//-----------------------------------------------------------------------
Real CameraActor::getFOVy(void) const
{
	return mCamera->getFOVy().valueDegrees();
}
//-----------------------------------------------------------------------
void CameraActor::setNearClipDistance(Real nearDist)
{
	mCamera->setNearClipDistance(nearDist);
}
//-----------------------------------------------------------------------
Real CameraActor::getNearClipDistance(void) const
{
	return mCamera->getNearClipDistance();
}
//-----------------------------------------------------------------------
void CameraActor::setFarClipDistance(Real farDist)
{
	mCamera->setFarClipDistance(farDist);
}
//-----------------------------------------------------------------------
Real CameraActor::getFarClipDistance(void) const
{
	return mCamera->getFarClipDistance();
}
//-----------------------------------------------------------------------
void CameraActor::setAspectRatio(Real ratio)
{
	mCamera->setAspectRatio(ratio);
}
//-----------------------------------------------------------------------
Real CameraActor::getAspectRatio(void) const
{
	return mCamera->getAspectRatio();
}
//-----------------------------------------------------------------------
const Plane& CameraActor::getFrustumPlane( FrustumPlane plane )
{
	return mCamera->getFrustumPlane(plane);
}
//-----------------------------------------------------------------------
bool CameraActor::isVisible(const AxisAlignedBox& bound, FrustumPlane* culledBy)
{
	return mCamera->isVisible(bound, culledBy);
}
//-----------------------------------------------------------------------
bool CameraActor::isVisible(const Sphere& bound, FrustumPlane* culledBy)
{
	return mCamera->isVisible(bound, culledBy);
}
//-----------------------------------------------------------------------
bool CameraActor::isVisible(const Vector3& vert, FrustumPlane* culledBy)
{
	return mCamera->isVisible(vert, culledBy);
}

const String& CameraActor::getTypeName(){
	return TYPENAME;
}

}
