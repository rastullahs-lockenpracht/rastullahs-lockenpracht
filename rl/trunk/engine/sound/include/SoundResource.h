/* Copyright 2004 Jochen Sch?fer */
#ifndef SOUNDRESOURCE_H
#define SOUNDRESOURCE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "SoundPrerequisites.h"

#include "Ogre.h"
#include "SndResource.h"

namespace rl {
/** Diese Klasse kapselt eine OpenAl++-Source (Sound) fuer
 * den ResourceManager von Ogre
 * @author Josch
 * @date 06-17-2004
 * @date 07-23-2004
 * @version 2.0
 */
class _RlSoundExport SoundResource: public SndResource {

    public:
        /// Der Standardkonstruktor
        SoundResource(const Ogre::String& name);
        /// Der Destruktor
        ~SoundResource();
        /// Laedt die Soundquelle.
        virtual void load(); 
         
};

}
#endif
