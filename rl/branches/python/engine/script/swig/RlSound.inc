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

namespace rl {
   	class SoundManager {
   	private:
        SoundManager();
        ~SoundManager();
   	public:
       static SoundManager& getSingleton();
       static SoundManager* getSingletonPtr();
       
       void addSounds();
       
       std::list<String> getSounds();
       void add(const std::string& resName);

   	};

   	class MusicManager  {
   	private:
        MusicManager();
        ~MusicManager();
   	public:
        static MusicManager& getSingleton();
        static MusicManager* getSingletonPtr();

        void playSong(unsigned int fade = 0);
        void stopSong(unsigned int fade = 0);

        const bool isPlaying() const;
        const bool isSourcePlaying() const;

        void setNextSong();
        void setGain(float newGain);
        float getGain();
        
        void clearPlayList();
        void addPlayList(std::string song);
        void addPlayList(std::list<std::string> list);
        void addSoundsIntoPlayList();
        
        bool isAuto();
        void setAuto(bool isAuto);
        bool isLooping();
        void setLooping(bool isLooping);
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
