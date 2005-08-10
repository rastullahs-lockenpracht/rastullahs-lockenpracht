/* Buffer.cpp created 10.08.2005 by josch
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
#include "Buffer.h"
#include "SoundSubsystem.h"
extern "C" {
    #include "AL/al.h"
}

namespace rl
{

/**
 * Konstruktor
 * @author josch
 * @date 10.08.2005
 * @version 1.0
 * @exception RuntimeException
 */
Buffer::Buffer()
{
    alGenBuffers(1, &mBuffer);
    SoundSubsystem::check();
}
/**
 * Destruktor
 * @author josch
 * @date 10.08.2005
 * @version 1.0
 */
Buffer::~Buffer()
{
    alDeleteBuffers(1, &mBuffer);
}

/**
 * Ist der Buffer noch gueltig?
 * @return TRUE, wenn der Buffer gueltig ist, sonst FALSE.
 * @author josch
 * @date 10.08.2005
 * @version 1.0
 */
const ALboolean Buffer::isValid() const
{
    return alIsBuffer(mBuffer);
}

/**
 * Buffer befuellen
 * @param format. Das Soundformat der Daten.
 * @param data. Die Sounddaten.
 * @param size. Die Groesse der Sounddaten.
 * @param freq. Die Frequenz der Daten.
 * @exception RuntimeException.
 * @author josch
 * @date 10.08.2005
 * @version 1.0
 */
void Buffer::setBuffer(const ALenum format,
    const ALvoid *data,
    const ALsizei size,
    const ALsizei freq)
{
    if (isValid())
    {
        alBufferData(mBuffer, format, data, size, freq);
        SoundSubsystem::check();
    }
}

/**
 * Frequenz
 * @return Die Frequenz der Bufferdaten.
 * @exception RuntimeException.
 * @author josch
 * @date 10.08.2005
 * @version 1.0
 */
const ALint Buffer::getFrequency() const
{
    if (isValid())
    {
        ALint value;
        alGetBufferi(mBuffer, AL_FREQUENCY, &value);
        SoundSubsystem::check();
        return value;
    }
}

/**
 * Bits
 * @return Die Bitzahl der Bufferdaten.
 * @exception RuntimeException.
 * @author josch
 * @date 10.08.2005
 * @version 1.0
 */
const ALint Buffer::getBits() const
{
    if (isValid())
    {
        ALint value;
        alGetBufferi(mBuffer, AL_BITS, &value);
        SoundSubsystem::check();
        return value;
    }
}

/**
 * Kanaele
 * @return Die Kanalanzahl der Bufferdaten.
 * @exception RuntimeException.
 * @author josch
 * @date 10.08.2005
 * @version 1.0
 */
const ALint Buffer::getChannels() const
{
    if (isValid())
    {
        ALint value;
        alGetBufferi(mBuffer, AL_CHANNELS, &value);
        SoundSubsystem::check();
        return value;
    }
}

/**
 * Groesse
 * @return Die Groesse der Bufferdaten.
 * @exception RuntimeException.
 * @author josch
 * @date 10.08.2005
 * @version 1.0
 */
const ALint Buffer::getSize() const
{
    if (isValid())
    {
        ALint value;
        alGetBufferi(mBuffer, AL_SIZE, &value);
        SoundSubsystem::check();
        return value;
    }
}

/**
 * Datenposition
 * @return Die Speicherposition der Bufferdaten.
 * @exception RuntimeException.
 * @author josch
 * @date 10.08.2005
 * @version 1.0
 */
const ALint Buffer::getData() const
{
    if (isValid())
    {
        ALint value;
        alGetBufferi(mBuffer, AL_DATA, &value);
        SoundSubsystem::check();
        return value;
    }
}

};
