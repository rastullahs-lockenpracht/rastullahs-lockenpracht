/* Buffer.h created 10.08.2005 by josch
 * This source file is part of Rastullahs Lockenpracht.
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
#ifndef _BUFFER_H_
#define _BUFFER_H_
extern "C" {
    #include "AL/altypes.h"
}


namespace rl
{

class Buffer
{
private:
    /// OpenAL Buffer ID
    ALuint mBuffer;
    
public:
    /// Konstruktor
	Buffer();
    /// Destruktor;
	~Buffer();
    
    /// Getter fuer Buffer ID
    const ALuint getBuffer() const;
    /// Ist die Buffer ID noch gueltig?
    const ALboolean isValid() const;
    /// Frequenz zurueckgeben
    const ALint getFrequency() const;
    /// Wieviel Bits
    const ALint getBits() const;
    /// Wieviele Kanaele
    const ALint getChannels() const;
    /// Groesse des Buffers
    const ALint getSize() const;
    /// Die Daten im Buffer
    const ALint getData() const;
    /// Den Buffer fuellen
    void setBuffer(const ALenum format, const ALvoid *data,
        const ALsizei size, const ALsizei freq);
};

};

#endif //_BUFFER_H_
