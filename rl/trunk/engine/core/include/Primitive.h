#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

#include <OgreSimpleRenderable.h>
#include "CorePrerequisites.h"

namespace rl {

class _RlCoreExport Primitive : public Ogre::SimpleRenderable
{
public:
	Primitive(const Ogre::String& materialName);
    ~Primitive();

    //void addLine(const Vector3 &start,const Vector3 &end);
    void setBox(const Ogre::Vector3& minVertex, const Ogre::Vector3& maxVertex);
    
    /// creates a Frustum (or even a more generic volume) made up by two
    /// recangles. points of one rectangle are connected to the
    /// coresponding points of the other rectangle.
    /// constraints: z-values of plane corners have to be equal for each plane
    /// (but may(must) differ from plane to plane)
    void setFrustum(const Ogre::Vector3& frontPlaneMin, const Ogre::Vector3& frontPlaneMax,
        const Ogre::Vector3& backPlaneMin, const Ogre::Vector3& backPlaneMax);
        
    void setCylinder(const Ogre::Vector3& baseCenter, Ogre::Real radius, Ogre::Real height,
        unsigned int numSegments = 16);
    
    void setCone(const Ogre::Vector3& baseCenter, Ogre::Real radius, Ogre::Real height,
        unsigned int numSegments = 16);

	void setSphereFrame(const Ogre::Vector3& center, Ogre::Real radius, Ogre::Real width, 
		unsigned int numSegments = 10);
        
    void clear();

    Ogre::Real getSquaredViewDepth(const Ogre::Camera *cam) const;
    Ogre::Real getBoundingRadius() const;

    void getRenderOperation(Ogre::RenderOperation& op);
    void _notifyCurrentCamera(Ogre::Camera* cam);
    void _notifyAttached(Ogre::Node* parent, bool isTagPoint = false);
    
protected:
    /// contains vertices for the lines. two adjacent points are a line
    std::vector<Ogre::Vector3> mPoints;
    
    /// Flag signs if renderOp is uptodate
    bool mIsDirty;
    
    /// recreates the vertices in the RenderOp to content of mPoints
    void updateRenderOp();
    
    void createBoxFromCorners(const std::vector<Ogre::Vector3>& corners);
};

}

#endif
