/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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

namespace Ogre {
%apply SWIGTYPE *DYNAMIC { Resource * };
	class Resource {
		virtual void load() = 0;
	};
}
 

namespace rl {


   	class ResourceManager {
   	public:
       void addSounds();
       void setMemoryBudget(size_t bytes);
       Ogre::Resource* getByName(const String &name);
       void addSearchPath(const String &path);
       std::list<String> getSounds();
       virtual Resource* create(const String &resName) = 0;
       virtual void add(Resource *song);
   	};
   
   	class SoundManager : public ResourceManager {
   	public:
       static SoundManager& getSingleton();
       static SoundManager* getSingletonPtr();
       virtual Resource* create(const String &resName);
   	};
      
   	class MusicManager : public ResourceManager {
   	public:
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
        
        virtual Resource* create(const String &resName);
        
        void clearPlayList();
        void addPlayList(std::string song);
        void addPlayList(std::list<std::string> list);
        void addSoundsIntoPlayList();
        
        bool isAuto();
        void setAuto(bool isAuto);
        bool isLooping();
        void setLooping(bool isLooping);
   	};
   	
    class SoundResource : public Ogre::Resource {
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
        const bool isPlaying() const;
    };
    
    class SoundEvent {
    	SoundEvent();
    	virtual ~SoundEvent();
    };
    
    class SoundPlayEvent : public SoundEvent {
    	SoundPlayEvent();
    	virtual ~SoundPlayEvent();
     	static const unsigned int STARTEVENT = 10;
    	static const unsigned int STOPEVENT = 11;
    };
    
    class SoundFadeEvent : public SoundEvent {
    	SoundFadeEvent();
    	~SoundFadeEvent();
    	static const unsigned int STARTEVENT = 12;
    	static const unsigned int STOPEVENT = 13;
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
    return 0;
}
%}
DYNAMIC_CAST(SWIGTYPE_p_Ogre__Resource, Resource_dynamic_cast);

