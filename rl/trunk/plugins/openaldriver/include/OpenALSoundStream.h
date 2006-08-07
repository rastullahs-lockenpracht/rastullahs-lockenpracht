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
#ifndef __OpenALSoundStream_H__
#define __OpenALSoundStream_H__

#include "OpenALDriverPrerequisites.h"
#include "SoundResource.h"
#include "Sound.h"

namespace rl {

	class OpenALDriver;
	class OpenALSoundChannel;

   /** Diese Klasse dient der Interaktion mit Ogre3D
    * @author Josch
    * @date 07-03-2005
    * @version 1.0
    */
    class _RlOpenALDriverExport OpenALSoundStream : public Sound
    {
    public:
        /// Konstruktor
        OpenALSoundStream(OpenALDriver* driver, const SoundResourcePtr &soundres);
        /// Destruktor
        virtual ~OpenALSoundStream();

        /// Laedt den Sound.
        virtual void load() throw (RuntimeException);
        /// Entlaedt den Sound.
        virtual void unload() throw (RuntimeException);        

        // Wir erzeugen einen Channel für SoundChannel
        virtual SoundChannel *createChannel() throw (RuntimeException);

		float getLength() const;

	protected:
        // Sind wir gueltig?
        virtual bool isValid() const throw (RuntimeException);
   
    private:
        /// Loop?
        bool mLooping;
		OpenALDriver* mDriver;
		OpenALSoundChannel* mChannel;
    }; 

    class _RlOpenALDriverExport OpenALSoundStreamPtr :
        public Ogre::SharedPtr<OpenALSoundStream>
    {
    public:
        OpenALSoundStreamPtr() : Ogre::SharedPtr<OpenALSoundStream>() {}
        explicit OpenALSoundStreamPtr(OpenALSoundStream* rep) : Ogre::SharedPtr<OpenALSoundStream>(rep) {}
        OpenALSoundStreamPtr(const OpenALSoundStreamPtr& res) : Ogre::SharedPtr<OpenALSoundStream>(res) {}
    protected:
        void destroy();
    };

}
#endif
