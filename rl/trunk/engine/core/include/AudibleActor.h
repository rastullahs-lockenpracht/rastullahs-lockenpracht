#ifndef __AudibleActor_H__
#define __AudibleActor_H__

#include "CorePrerequisites.h"
#include "Actor.h"
#include <map>

using namespace std;

namespace rl {

    class SoundResource;

    typedef map<String, SoundResource*> SoundNameMap;

    /**
    * Diese Klasse verwaltet die Sounds, die im Spiel
    * von Aktoren hoerbar sind. Dieser Aktor wird an den
    * entsprechenden Aktor angekoppelt.
    * @author JoSch
    * @date 05-04-2004
    * @version 1.0
    */
    class _RlCoreExport AudibleActor : public virtual Actor
    {
    private:
        /// Map von Name auf SoundResource
        SoundNameMap mMap;
        /// Die Geschwindigekit des Aktors.
        Vector3 mVelocity;

    protected:
        static const String TYPENAME;

    public:
        // Konstruktor
        AudibleActor(const String& name);
        // Konstruktor
        AudibleActor(const String& name, SceneNode* pParentSceneNode);
        // Destruktor
        virtual ~AudibleActor();

        // Fuege einen Sound hinzu.
        void addSound(const String &name);
        // Loesche einen Sound.
        void deleteSound(const String &name);
        // Spiele einen Sound ab.
        void playSound(const String &name);
        // Stoppe einen Sound.
        void stopSound(const String &name);
        // Gebe den Sound mit dem entsprechenden Namen zurueck.
        const SoundResource* getSound(const String &name) const;
        // Gebe den Sound mit dem entsprechenden Namen zurueck.
        SoundResource* getSound(const String &name);
        /// Liefert die Geschwindigkeit des Listener.
        virtual const Vector3& getVelocity() const;
        /// Setzt die Geschwindigkeit des Listener.
        virtual void setVelocity(const Vector3& velocity);
        /// Sets the position of this object. 
        virtual void setPosition(Real x, Real y, Real z);
        /// Sets the orientation of this object.
        virtual void setOrientation(const Quaternion& orientation);

        virtual bool isSimpleOgreActor();

    };

}
#endif
