/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
* 
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the Clarified Artistic License.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  Clarified Artistic License for more details.
*
*  You should have received a copy of the Clarified Artistic License
*  along with this program; if not you can get it here
*  http://www.jpaulmorrison.com/fbp/artistic2.htm.
*/

/*
 * Based on work by monster (TODO substitue with real name)
 * Copyright 2005 Daniel Wickert
 */

#include "Primitive.h"
#include <OgreCamera.h>
#include <OgreHardwareBufferManager.h>

using namespace std;
using namespace Ogre;

namespace rl {

Primitive::Primitive(const String& materialName)
{
    setMaterial(materialName);
}

Primitive::~Primitive()
{
}

void Primitive::setFrustum(const Vector3& frontPlaneMin,
    const Vector3& frontPlaneMax,
    const Vector3& backPlaneMin, const Vector3& backPlaneMax)
{
    // compute AABB
    Vector3 vmin, vmax;
    vmin.x = frontPlaneMin.x < backPlaneMin.x ? frontPlaneMin.x : backPlaneMin.x;
    vmin.y = frontPlaneMin.y < backPlaneMin.y ? frontPlaneMin.y : backPlaneMin.y;
    vmin.z = frontPlaneMin.z < backPlaneMin.z ? frontPlaneMin.z : backPlaneMin.z;
    
    vmax.x = frontPlaneMax.x > backPlaneMax.x ? frontPlaneMax.x : backPlaneMax.x;
    vmax.y = frontPlaneMax.y > backPlaneMax.y ? frontPlaneMax.y : backPlaneMax.y;
    vmax.z = frontPlaneMax.z > backPlaneMax.z ? frontPlaneMax.z : backPlaneMax.z;
    
    mBox.setExtents(vmax, vmin);
    clear();

    vector<Vector3> corners;
    corners.reserve(8);
    
    corners.push_back(frontPlaneMin);
    // Create helper vertices. One vertex for each cube vertex
    // first the three on the xy-plane through vmin
    corners.push_back(Vector3(frontPlaneMax.x, frontPlaneMin.y, frontPlaneMin.z));
    corners.push_back(frontPlaneMax);
    corners.push_back(Vector3(frontPlaneMin.x, frontPlaneMax.y, frontPlaneMin.z));
    // next the second three on the xy-plane through vmax
    corners.push_back(Vector3(backPlaneMax.x, backPlaneMin.y, backPlaneMax.z));
    corners.push_back(backPlaneMin);
    corners.push_back(Vector3(backPlaneMin.x, backPlaneMax.y, backPlaneMax.z));
    corners.push_back(backPlaneMax);

    createBoxFromCorners(corners);
}

void Primitive::setCylinder(const Vector3& baseCenter, Real radius, Real height,
    unsigned int numSegments)
{
    Vector3 vmin(baseCenter.x - radius, baseCenter.y, baseCenter.z - radius);
    Vector3 vmax(baseCenter.x + radius, baseCenter.y + height,
        baseCenter.z + radius);
    mBox.setExtents(vmax, vmin);
    clear();
    
    vector<Vector3> floor_verts;
    floor_verts.reserve(numSegments);
    vector<Vector3> ceiling_verts;
    ceiling_verts.reserve(numSegments);
    
    for (unsigned int i = 0; i < numSegments; ++i)
    {
        Real phi = (360.0 / numSegments) * i;
        Real x = baseCenter.x + radius * Math::Sin(Degree(phi));
        Real z = baseCenter.z + radius * Math::Cos(Degree(phi));
        floor_verts.push_back(Vector3(x, baseCenter.y, z));
        ceiling_verts.push_back(Vector3(x, baseCenter.y + height, z));
    }
    
    for (unsigned int i = 1; i <= numSegments; ++i)
    {
        mPoints.push_back(baseCenter);
        mPoints.push_back(floor_verts[i % numSegments]);
        mPoints.push_back(floor_verts[i - 1]);
        
        mPoints.push_back(baseCenter + Vector3(0, height, 0));
        mPoints.push_back(ceiling_verts[i % numSegments]);
        mPoints.push_back(ceiling_verts[i - 1]);
        
        mPoints.push_back(floor_verts[i % numSegments]);
        mPoints.push_back(floor_verts[i - 1]);
        mPoints.push_back(ceiling_verts[i % numSegments]);
        
        mPoints.push_back(ceiling_verts[i % numSegments]);
        mPoints.push_back(ceiling_verts[i - 1]);
        mPoints.push_back(floor_verts[i - 1]);
    }
    
}

void Primitive::setCone(const Vector3& baseCenter, Real radius, Real height,
                            unsigned int numSegments)
{
    Vector3 vmin(baseCenter.x - radius, baseCenter.y, baseCenter.z - radius);
    Vector3 vmax(baseCenter.x + radius, baseCenter.y + height,
        baseCenter.z + radius);
    mBox.setExtents(vmax, vmin);
    clear();

    vector<Vector3> floor_verts;
    floor_verts.reserve(numSegments);

    for (unsigned int i = 0; i < numSegments; ++i)
    {
        Real phi = (360.0 / numSegments) * i;
        Real x = baseCenter.x + radius * Math::Sin(Degree(phi));
        Real z = baseCenter.z + radius * Math::Cos(Degree(phi));
        floor_verts.push_back(Vector3(x, baseCenter.y, z));
    }

    for (unsigned int i = 1; i <= numSegments; ++i)
    {
        mPoints.push_back(baseCenter);
        mPoints.push_back(floor_verts[i % numSegments]);
        mPoints.push_back(floor_verts[i - 1]);

        mPoints.push_back(floor_verts[i % numSegments]);
        mPoints.push_back(floor_verts[i - 1]);
        mPoints.push_back(baseCenter + Vector3(0, height, 0));
    }

}

Vector3 getKarthesianFromPolar(const Vector3& center, Real radius, Degree phi, Degree theta)
{
	return Vector3(
		radius * Math::Sin(theta) * Math::Cos(phi),
		radius * Math::Sin(theta) * Math::Sin(phi),
		radius * Math::Cos(theta));
}

void Primitive::setSphereFrame(const Vector3& center, Real radius, Real width, unsigned int numSegments)
{
	mBox.setExtents(
		Vector3(center.x - radius, center.y - radius, center.z - radius),
		Vector3(center.x + radius, center.y + radius, center.z + radius));
    clear();

	// Falls jemand will, kann er hier mal einen richtigen Algorithmus reinpacken
	for (unsigned int phiIdx = 0; phiIdx < numSegments; phiIdx++)
	{
		Degree anglePhiLast, anglePhiCurr;

		anglePhiCurr = Degree(360 * phiIdx/numSegments);

		if (phiIdx == 0)
			anglePhiLast = Degree(360 * (numSegments-1)/numSegments);
		else
			anglePhiLast = Degree(360 * (phiIdx-1)/numSegments);

		{
        Vector3 point0 = center+Vector3(0, radius*Math::Sin(anglePhiLast), radius*Math::Cos(anglePhiLast));
		Vector3 point1 = center+Vector3(0, radius*Math::Sin(anglePhiCurr), radius*Math::Cos(anglePhiCurr));
										
		mPoints.push_back(point0 + Vector3(-width, 0, 0));
		mPoints.push_back(point0 + Vector3(width, 0, 0));
		mPoints.push_back(point1 + Vector3(-width, 0, 0));

		mPoints.push_back(point0 + Vector3(width, 0, 0));
		mPoints.push_back(point1 + Vector3(width, 0, 0));
		mPoints.push_back(point1 + Vector3(-width, 0, 0));
		}

		{
        Vector3 point0 = center+Vector3(radius*Math::Sin(anglePhiLast), 0, radius*Math::Cos(anglePhiLast));
		Vector3 point1 = center+Vector3(radius*Math::Sin(anglePhiCurr), 0, radius*Math::Cos(anglePhiCurr));
										
		mPoints.push_back(point0 + Vector3(0, -width, 0));
		mPoints.push_back(point0 + Vector3(0, width, 0));
		mPoints.push_back(point1 + Vector3(0, -width, 0));

		mPoints.push_back(point0 + Vector3(0, width, 0));
		mPoints.push_back(point1 + Vector3(0, width, 0));
		mPoints.push_back(point1 + Vector3(0, -width, 0));
		}

		{
        Vector3 point0 = center+Vector3(radius*Math::Sin(anglePhiLast), radius*Math::Cos(anglePhiLast), 0);
		Vector3 point1 = center+Vector3(radius*Math::Sin(anglePhiCurr), radius*Math::Cos(anglePhiCurr), 0);
										
		mPoints.push_back(point0 + Vector3(0, 0, -width));
		mPoints.push_back(point0 + Vector3(0, 0, width));
		mPoints.push_back(point1 + Vector3(0, 0, -width));

		mPoints.push_back(point0 + Vector3(0, 0, width));
		mPoints.push_back(point1 + Vector3(0, 0, width));
		mPoints.push_back(point1 + Vector3(0, 0, -width));
		}
	}
}

void Primitive::setBox(const Vector3& vmin, const Vector3& vmax)
{
    mBox.setExtents(vmax, vmin);
    clear();
    
    vector<Vector3> corners;
    corners.reserve(8);
    
    corners.push_back(vmin);
    // Create helper vertices. One vertex for each cube vertex
    // first the three on the xy-plane through vmin
    corners.push_back(Vector3(vmax.x, vmin.y, vmin.z));
    corners.push_back(Vector3(vmax.x, vmax.y, vmin.z));
    corners.push_back(Vector3(vmin.x, vmax.y, vmin.z));
    // next the second three on the xy-plane through vmax
    corners.push_back(Vector3(vmax.x, vmin.y, vmax.z));
    corners.push_back(Vector3(vmin.x, vmin.y, vmax.z));
    corners.push_back(Vector3(vmin.x, vmax.y, vmax.z));
    corners.push_back(vmax);
    
    createBoxFromCorners(corners);
}

Ogre::ManualObject* createBox(const Ogre::AxisAlignedBox& aabb, bool wireFrame = false,
    const Ogre::String& materialName = "DebugPrimitiveDefaultMaterial")
{
    ManualObject* obj = CoreSubsystem::getSingleton().getWorld().getSceneManager();
}

void Primitive::createBoxFromCorners(const vector<Vector3>& corners)
{
    // First side
    mPoints.push_back(corners[0]);
    mPoints.push_back(corners[1]);
    mPoints.push_back(corners[2]);

    mPoints.push_back(corners[0]);
    mPoints.push_back(corners[2]);
    mPoints.push_back(corners[3]);

    // Second side
    mPoints.push_back(corners[0]);
    mPoints.push_back(corners[3]);
    mPoints.push_back(corners[6]);

    mPoints.push_back(corners[0]);
    mPoints.push_back(corners[6]);
    mPoints.push_back(corners[5]);

    // Third side
    mPoints.push_back(corners[0]);
    mPoints.push_back(corners[4]);
    mPoints.push_back(corners[1]);

    mPoints.push_back(corners[0]);
    mPoints.push_back(corners[5]);
    mPoints.push_back(corners[4]);

    // Forth side
    mPoints.push_back(corners[7]);
    mPoints.push_back(corners[3]);
    mPoints.push_back(corners[2]);

    mPoints.push_back(corners[7]);
    mPoints.push_back(corners[6]);
    mPoints.push_back(corners[3]);

    // Fifth side
    mPoints.push_back(corners[7]);
    mPoints.push_back(corners[2]);
    mPoints.push_back(corners[1]);

    mPoints.push_back(corners[7]);
    mPoints.push_back(corners[1]);
    mPoints.push_back(corners[4]);

    // Sixth side
    mPoints.push_back(corners[7]);
    mPoints.push_back(corners[4]);
    mPoints.push_back(corners[5]);

    mPoints.push_back(corners[7]);
    mPoints.push_back(corners[5]);
    mPoints.push_back(corners[6]);

    mIsDirty = true;
}

Real Primitive::getBoundingRadius() const
{
    return (mBox.getMaximum() - mBox.getMinimum()).length() / 2.0;
}

Real Primitive::getSquaredViewDepth(const Camera* cam) const
{
    Vector3 vmin, vmax, vmid, vdist;
    vmin = mBox.getMinimum();
    vmax = mBox.getMaximum();
    vmid = ((vmin - vmax) * 0.5) + vmin;
    vdist = cam->getDerivedPosition() - vmid;

    return vdist.squaredLength();
}

void Primitive::clear()
{
    mIsDirty = true;
    
    mPoints.clear();
}

void Primitive::updateRenderOp()
{
    if (!mIsDirty) return;
    
    mRenderOp.indexData = 0;
    delete mRenderOp.vertexData;
    mRenderOp.vertexData = new Ogre::VertexData();
    mRenderOp.vertexData->vertexCount = mPoints.size();
    mRenderOp.vertexData->vertexStart = 0;
    mRenderOp.operationType = RenderOperation::OT_TRIANGLE_LIST;
    mRenderOp.useIndexes = false;

    Ogre::VertexDeclaration *decl = mRenderOp.vertexData->vertexDeclaration;
    Ogre::VertexBufferBinding *bind = mRenderOp.vertexData->vertexBufferBinding;

    decl->addElement(0, 0, VET_FLOAT3, VES_POSITION);

    HardwareVertexBufferSharedPtr vbuf =
        HardwareBufferManager::getSingleton().createVertexBuffer(
        decl->getVertexSize(0),
        mRenderOp.vertexData->vertexCount,
        HardwareBuffer::HBU_STATIC_WRITE_ONLY);

    bind->setBinding(0, vbuf);

    // Drawing stuff
    size_t size = mPoints.size();
    Vector3 vaabMin = mPoints[0];
    Vector3 vaabMax = mPoints[0];

    Real *prPos = static_cast<Real*>(vbuf->lock(HardwareBuffer::HBL_DISCARD));

    for(size_t i = 0; i < size; i++)
    {
        *prPos++ = mPoints[i].x;
        *prPos++ = mPoints[i].y;
        *prPos++ = mPoints[i].z;
    }

    vbuf->unlock();
    
    mIsDirty = false;
}

void Primitive::getRenderOperation(RenderOperation& op)
{
    if(mIsDirty) updateRenderOp();
    SimpleRenderable::getRenderOperation(op);
}
void Primitive::_notifyCurrentCamera(Camera* cam)
{
    if(mIsDirty) updateRenderOp();
    SimpleRenderable::_notifyCurrentCamera(cam);
}

void Primitive::_notifyAttached(Node* parent, bool isTagPoint)
{
    if(mIsDirty) updateRenderOp();
    SimpleRenderable::_notifyAttached(parent, isTagPoint);
}

}
