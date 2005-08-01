#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

#include <OgreSimpleRenderable.h>

using namespace Ogre;

class Primitive : public Ogre::SimpleRenderable
{
public:
    Primitive(const String& materialName);
    ~Primitive();

    //void addLine(const Vector3 &start,const Vector3 &end);
    void setBox(const Vector3& minVertex, const Vector3& maxVertex);
    
    /// creates a Frustum (or even a more generic volume) made up by two
    /// recangles. points of one rectangle are connected to the
    /// coresponding points of the other rectangle.
    /// constraints: z-values of plane corners have to be equal for each plane
    /// (but may(must) differ from plane to plane)
    void setFrustum(const Vector3& frontPlaneMin, const Vector3& frontPlaneMax,
        const Vector3& backPlaneMin, const Vector3& backPlaneMax);
        
    void setCylinder(const Vector3& baseCenter, Real radius, Real height,
        unsigned int numSegments = 16);
    
    void setCone(const Vector3& baseCenter, Real radius, Real height,
        unsigned int numSegments = 16);
        
    void clear();

    Real getSquaredViewDepth(const Camera *cam) const;
    Real getBoundingRadius(void) const;

    void getRenderOperation(RenderOperation& op);
    void _notifyCurrentCamera(Camera* cam);
    void _notifyAttached(Node* parent, bool isTagPoint = false);
    
protected:
    /// contains vertices for the lines. two adjacent points are a line
    std::vector<Vector3> mPoints;
    
    /// Flag signs if renderOp is uptodate
    bool mIsDirty;
    
    /// recreates the vertices in the RenderOp to content of mPoints
    void updateRenderOp();
    
    void createBoxFromCorners(const std::vector<Vector3>& corners);
};

#endif
