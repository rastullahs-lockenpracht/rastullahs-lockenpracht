/* SoundResource.h - Diese Klassse kapselt einen Sound.
 * (C) 2003-2005. Team Pantheon. www.team-pantheon.de
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

#ifndef SOUNDRESOURCE_H
#define SOUNDRESOURCE_H

#include "SoundPrerequisites.h"
#include <Ogre.h>
#include <OgreDataStream.h>


namespace rl {
 
 
/** Diese Basisklasse kapselt eine OpenAl++-Source fuer
 * den ResourceManager von Ogre
 * @author Josch
 * @date 06-19-2004
 * @date 07-23-2004
 * @date 10-10-2004
 * @date 06-26-2005
 * @version 4.0
 */
class _RlSoundExport SoundResource: public Ogre::Resource
{
    private:
        /// Unsere Daten von Ogres ResourceManager.
        Ogre::DataStreamPtr mDataStream;
                       
    public:
        /// Der Standardkonstruktor
        SoundResource(Ogre::ResourceManager* creator, const Ogre::String& name, 
            Ogre::ResourceHandle handle, const Ogre::String& group, bool isManual, 
            Ogre::ManualResourceLoader* loader);
        /// Der Destruktor
        virtual ~SoundResource();
        /// Den Datenstrom zurückgeben
        Ogre::DataStreamPtr getDataStream() const;
        /// Groesse zurueckgeben.
        int getSize() const;

    protected:
		/// Laedt die Soundquelle.
		virtual void loadImpl();
		/// Entlaedt die Soundquelle.
		virtual void unloadImpl();
		/// Bestimmt die Groesse im Speicher (wird erst nach dem Laden aufgerufen)
		virtual size_t calculateSize() const;
        
        
};

class _RlSoundExport SoundResourcePtr :
	public Ogre::SharedPtr<SoundResource>
{
public:
	SoundResourcePtr() : Ogre::SharedPtr<SoundResource>() {}
	explicit SoundResourcePtr(SoundResource* rep) : Ogre::SharedPtr<SoundResource>(rep) {}
	SoundResourcePtr(const SoundResourcePtr& res) : Ogre::SharedPtr<SoundResource>(res) {}
	SoundResourcePtr(const Ogre::ResourcePtr& res);
	SoundResourcePtr& operator=(const Ogre::ResourcePtr& res);
protected:
	void destroy();
};

}
#endif
