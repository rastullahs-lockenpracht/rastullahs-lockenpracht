
/* File : RlSound.i */
%module RlSound

%{
#include "FixRubyHeaders.h"
#include "SoundPrerequisites.h"
#include "MusicManager.h"
#include "SoundResource.h"
#include "SoundManager.h"
#include "ResManager.h"
#include "MusicResource.h"
#include "AL/al.h"
%}

%include "../../common/swig/TypeMaps.i"


namespace rl {

%apply SWIGTYPE *DYNAMIC { Ogre::Resource * };

   	class ResManager {
   	public:
       void addSounds();
       void setMemoryBudget(size_t bytes);
       Ogre::Resource* getByName(const String &name);
       void addSearchPath(const String &path);
       std::list<String> getSounds();
       virtual Resource* create(const String &resName) = 0;
   	};
   
   	class SoundManager : public ResManager {
   	public:
       static SoundManager& getSingleton();
       static SoundManager* getSingletonPtr();
       virtual Resource* create(const String &resName);
   	};
      
   	class MusicManager : public ResManager {
   	public:
        virtual Resource* create(const String &resName);
        static MusicManager& getSingleton();
        static MusicManager* getSingletonPtr();
        MusicManager();
        ~MusicManager();

        void playSong();
        void stopSong();

        const bool isPlaying() const;
        const bool isSourcePlaying() const;

        void setNextSong();
        void setGain(float newGain);
        float getGain();
        
        bool isAuto();
        void setAuto(bool isAuto);
        bool isLooping();
        void setLooping(bool isLooping);
   	};
   
    class SndResource {
   	public:
        SoundResource(const String& name);
        const Vector3& getDirection() const throw (rl::RuntimeException);
        void setDirection(const Vector3& direction) throw (rl::RuntimeException);
        const Vector3& getVelocity() const throw (rl::RuntimeException);
        void setVelocity(const Vector3& velocity) throw (rl::RuntimeException);
        const Vector3& getPosition() const throw (rl::RuntimeException);
        void setPosition(const Vector3& direction) throw (rl::RuntimeException);
        const float getGain() const throw (rl::RuntimeException);
        void setGain(const float gain) throw (rl::RuntimeException);
        
        virtual void load();
        virtual void unload() = 0;
        
        void play(int msec) throw (rl::RuntimeException);
        void pause() throw (rl::RuntimeException);
        void stop(int msec) throw (rl::RuntimeException);
        void rewind() throw (rl::RuntimeException);
        const ALenum getState() const throw (rl::RuntimeException);
    };

   	class MusicResource : public SndResource {
   	public:
        MusicResource(const String& name);
        ~MusicResource();
        
        virtual void load();
        virtual void unload();
  	};

   	class SoundResource : public SndResource {
   	public:
        SoundResource(const String& name);
        ~SoundResource();
        
        virtual void load();
        virtual void unload();
  	};
}

%{
static swig_type_info *Resource_dynamic_cast(void **ptr) {
    Ogre::Resource **ppResource = reinterpret_cast<Ogre::Resource **>(ptr);
    rl::SoundResource *pSoundResource = dynamic_cast<rl::SoundResource*>(*ppResource);
    if(pSoundResource){
      *ptr=reinterpret_cast<void*>(pSoundResource);
      return SWIGTYPE_p_rl__SoundResource;
    }
    rl::MusicResource *pMusicResource = dynamic_cast<rl::MusicResource*>(*ppResource);
    if(pMusicResource){
      *ptr=reinterpret_cast<void*>(pMusicResource);
      return SWIGTYPE_p_rl__MusicResource;
    }
    return 0;
}
%}
DYNAMIC_CAST(SWIGTYPE_p_Ogre__Resource, Resource_dynamic_cast);

