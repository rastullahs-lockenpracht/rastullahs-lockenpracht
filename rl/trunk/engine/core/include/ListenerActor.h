#ifndef __LISTENERACTOR_H
#define __LISTENERACTOR_H

#include "Actor.h"

#include "CorePrerequisites.h"

namespace rl {
/**
 * Kapselt den Zuhoerer des Soundsystem (entspricht Kamera
 * bei Ogre).
 * @author JoSch
 * @version 2.0
 * @date 04-27-2004
 * @date 07-23-2004
 */
class ListenerActor : public virtual Actor, public Singleton<ListenerActor> {
private:
    
public:
    /// Konstruktor
	ListenerActor(const String& name);
    /// Konstruktor
    ListenerActor(const String& name, SceneNode *pParentSceneNode);
    /// Standarddestruktor
	~ListenerActor();

    /// Liefert die Geschwindigkeit des Listener.
    virtual const Vector3 getVelocity() const;
    /// Setzt die Geschwindigkeit des Listener.
    virtual void setVelocity(const Vector3& velocity);
    /// Sets the position of this object. 
    virtual void setPosition(Real x, Real y, Real z);
    /// Sets the orientation of this object.
    virtual void setOrientation(const Quaternion& orientation);

    virtual bool isSimpleOgreActor();

    static ListenerActor & getSingleton(void);
    static ListenerActor * getSingletonPtr(void);

};

}
#endif // LISTENERACTOR_H
