/* This source file is part of Rastullahs Lockenpracht.
* Copyright (C) 2003-2008 Team Pantheon. http://www.team-pantheon.de
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

#ifndef __RlWriteableDataStreamFormatTarget_H_
#define __RlWriteableDataStreamFormatTarget_H_

#include <xercesc/framework/XMLFormatter.hpp>
#include "WriteableDataStream.h"

namespace rl
{
    class _RlCommonExport WriteableDataStreamFormatTarget : public XERCES_CPP_NAMESPACE::XMLFormatTarget
    {
    public:
        WriteableDataStreamFormatTarget(WriteableDataStream* stream);
        virtual void writeChars (const XMLByte *const toWrite, const unsigned int count, XERCES_CPP_NAMESPACE::XMLFormatter *const formatter);
        virtual void flush ();
    protected:
        WriteableDataStream* mStream;
    };
}

#endif