#ifndef __CameraActor_H__
#define __CameraActor_H__

#include <OgreCamera.h>

#include "CorePrerequisites.h"
#include "Actor.h"

namespace rl {

/** A specifc Actor containing a Camera
@remark
	This Actor wraps all the functinality given by a 
	Ogre Camera
@par
	The Scenenode in which the Camera resides and the Camera
@see 
	Camera
	FlyingCameraController
*/
class _RlCoreExport CameraActor : public virtual Actor
{
public:

	/** Constructor
		@param name The Actors name
		@param pSceneNode The SceneNode of the Camera
		@param pCamera The Camera
	*/
	CameraActor( const String& namme, Camera* pCamera );

	/** Constructor
		@param name The Actors name
		@param pParentSceneNode The Parent SceneNode for the Camera
		@param pCamera The Camera
	*/
	CameraActor( const String& name, SceneNode* pParentSceneNode, Camera* pCamera );

	/** Default Deconstructor */
	virtual ~CameraActor();

	/** Returns the Typename */
	virtual const String& getTypeName();

	/** Gets the internal Camera object. */
	Camera* getOgreCamera(void)
	{
		return mCamera;
	}

	/** Attaches the Camera to a scene graph node */
	void attachTo(SceneNode* node);
	
	/** Sets the orientation of this object. */
	void setOrientation(const Quaternion& orientation);
	/** Gets the current orientation of this object. */
	const Quaternion& getOrientation(void);

	/** Moves the object along it's local  axes.
	    @par
	        This method moves the object by the supplied vector along the
	        local axes of the obect.
	    @param 
	        d Vector with x,y,z values representing the translation.
	*/
	void translate(const Vector3& d);

	/** Rotate the object around the local Z-axis.
	*/
	void roll(Real angleunits);

	/** Rotate the object around the local X-axis.
	*/
	void pitch(Real angleunits);

	/** Rotate the object around the local Y-axis.
	*/
	void yaw(Real angleunits);

	/** Rotate the object around an arbitrary axis.
	*/
	void rotate(const Vector3& axis, Real angleunits);

	/** Rotate the object around an aritrary axis using a Quarternion.
	*/
	void rotate(const Quaternion& q);

	// ----------------------------------------------
	// The following are methods delegated to Camera
	// ----------------------------------------------

	/** Sets the type of projection to use (orthographic or perspective). Default is perspective.
	*/
	void setProjectionType(ProjectionType pt);

	/** Retrieves info on the type of projection used (orthographic or perspective).
	*/
	ProjectionType getProjectionType(void) const;

	/** Sets the level of rendering detail required from this camera.
	    @remarks
	        Each camera is set to render at full detail by default, that is
	        with full texturing, lighting etc. This method lets you change
	        that behaviour, allowing you to make the camera just render a
	        wireframe view, for example.
	*/
	void setDetailLevel(SceneDetailLevel sd);

	/** Retrieves the level of detail that the camera will render.
	*/
	SceneDetailLevel getDetailLevel(void) const;

	/** Sets the camera's direction vector.
	    @remarks
	        Note that the 'up' vector for the camera will automatically be recalculated based on the
	        current 'up' vector (i.e. the roll will remain the same).
	*/
	void setDirection(Real x, Real y, Real z);

	/** Sets the camera's direction vector.
	*/
	void setDirection(const Vector3& vec);

	/* Gets the camera's direction.
	*/
	Vector3 getDirection(void) const;


	/** Points the camera at a location in worldspace.
	    @remarks
	        This is a helper method to automatically generate the
	        direction vector for the camera, based on it's current position
	        and the supplied look-at point.
	    @param
	        targetPoint A vector specifying the look at point.
	*/
	void lookAt( const Vector3& targetPoint );
	/** Points the camera at a location in worldspace.
	    @remarks
	        This is a helper method to automatically generate the
	        direction vector for the camera, based on it's current position
	        and the supplied look-at point.
	    @param
	        x
	    @param
	        y
	    @param
	        z Co-ordinates of the point to look at.
	*/
	void lookAt(Real x, Real y, Real z);

	/** Tells the camera whether to yaw around it's own local Y axis or a fixed axis of choice.
	    @remarks
	        This method allows you to change the yaw behaviour of the camera - by default, the camera
	        yaws around it's own local Y axis. This is often what you want - for example a flying camera
	        - but sometimes this produces unwanted effects. For example, if you're making a first-person
	        shooter, you really don't want the yaw axis to reflect the local camera Y, because this would
	        mean a different yaw axis if the player is looking upwards rather than when they are looking
	        straight ahead. You can change this behaviour by setting the yaw to a fixed axis (say, the world Y).
	    @param
	        useFixed If true, the axis passed in the second parameter will always be the yaw axis no
	        matter what the camera orientation. If false, the camera returns to it's default behaviour.
	    @param
	        fixedAxis The axis to use if the first parameter is true.
	*/
	void setFixedYawAxis( bool useFixed, const Vector3& fixedAxis = Vector3::UNIT_Y );

	/** Sets the Y-dimension Field Of View (FOV) of the camera.
	    @remarks
	        Field Of View (FOV) is the angle made between the camera's position, and the left & right edges
	        of the 'screen' onto which the scene is projected. High values (90+) result in a wide-angle,
	        fish-eye kind of view, low values (30-) in a stretched, telescopic kind of view. Typical values
	        are between 45 and 60.
	    @par
	        This value represents the HORIZONTAL field-of-view. The vertical field of view is calculated from
	        this depending on the dimensions of the viewport (they will only be the same if the viewport is square).
	    @note
	        Setting the FOV overrides the value supplied for Camera::setNearClipPlane.
	 */
	void setFOVy(Real fovy);

	/** Retrieves the cameras Y-dimension Field Of View (FOV).
	*/
	Real getFOVy(void) const;

	/** Sets the position of the near clipping plane.
	    @remarks
	        The position of the near clipping plane is the distance from the cameras position to the screen
	        on which the world is projected. The near plane distance, combined with the field-of-view and the
	        aspect ratio, determines the size of the viewport through which the world is viewed (in world
	        co-ordinates). Note that this world viewport is different to a screen viewport, which has it's
	        dimensions expressed in pixels. The cameras viewport should have the same aspect ratio as the
	        screen viewport it renders into to avoid distortion.
	    @param
	        near The distance to the near clipping plane from the camera in world coordinates.
	 */
	void setNearClipDistance(Real nearDist);

	/** Sets the position of the near clipping plane.
	*/
	Real getNearClipDistance(void) const;

	/** Sets the distance to the far clipping plane.
	    @remarks
	        The view frustrum is a pyramid created from the camera position and the edges of the viewport.
	        This frustrum does not extend to infinity - it is cropped near to the camera and there is a far
	        plane beyond which nothing is displayed. This method sets the distance for the far plane. Different
	        applications need different values: e.g. a flight sim needs a much further far clipping plane than
	        a first-person shooter. An important point here is that the larger the gap between near and far
	        clipping planes, the lower the accuracy of the Z-buffer used to depth-cue pixels. This is because the
	        Z-range is limited to the size of the Z buffer (16 or 32-bit) and the max values must be spread over
	        the gap between near and far clip planes. The bigger the range, the more the Z values will
	        be approximated which can cause artifacts when lots of objects are close together in the Z-plane. So
	        make sure you clip as close to the camera as you can - don't set a huge value for the sake of
	        it.
	    @param
	        far The distance to the far clipping plane from the camera in world coordinates.
	*/
	void setFarClipDistance(Real farDist);

	/** Retrieves the distance from the camera to the far clipping plane.
	*/
	Real getFarClipDistance(void) const;

	/** Sets the aspect ratio for the camera viewport.
	    @remarks
	        The ratio between the x and y dimensions of the rectangular area visible through the camera
	        is known as aspect ratio: aspect = width / height .
	    @par
	        The default for most fullscreen windows is 1.3333 - this is also assumed by Ogre unless you
	        use this method to state otherwise.
	*/
	void setAspectRatio(Real ratio);

	/** Retreives the current aspect ratio.
	*/
	Real getAspectRatio(void) const;

	/** Retrieves a specified plane of the frustum.
	    @remarks
	        Gets a reference to one of the planes which make up the camera frustum, e.g. for clipping purposes.
	*/
	const Plane& getFrustumPlane( FrustumPlane plane );

	/** Tests whether the given container is visible in the Frustum.
	    @param
	        bound Bounding box to be checked
	    @param
	        culledBy Optional pointer to an int which will be filled by the plane number which culled
	        the box if the result was false;
	    @returns
	        If the box was visible, true is returned.
	    @par
	        Otherwise, false is returned.
	*/
	bool isVisible(const AxisAlignedBox& bound, FrustumPlane* culledBy = 0);

	/** Tests whether the given container is visible in the Frustum.
	    @param
	        bound Bounding sphere to be checked
	    @param
	        culledBy Optional pointer to an int which will be filled by the plane number which culled
	        the box if the result was false;
	    @returns
	        If the sphere was visible, true is returned.
	    @par
	        Otherwise, false is returned.
	*/
	bool isVisible(const Sphere& bound, FrustumPlane* culledBy = 0);

	/** Tests whether the given vertex is visible in the Frustum.
	    @param
	        vert Vertex to be checked
	    @param
	        culledBy Optional pointer to an int which will be filled by the plane number which culled
	        the box if the result was false;
	    @returns
	        If the box was visible, true is returned.
	    @par
	        Otherwise, false is returned.
	*/
	bool isVisible(const Vector3& vert, FrustumPlane* culledBy = 0);
protected:
	static const String TYPENAME;
	Camera *mCamera;
};

}
#endif
