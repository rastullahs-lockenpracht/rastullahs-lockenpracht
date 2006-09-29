/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2006 Team Pantheon. http://www.team-pantheon.de
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
#include <sstream>
#include <xercesc/sax/SAXParseException.hpp>
#include "XmlErrorHandler.h"
#include "XmlHelper.h"


using namespace rl;

XmlErrorHandler::XmlErrorHandler(void)
    : mFileName("")
{
}

XmlErrorHandler::~XmlErrorHandler(void)
{
}

void XmlErrorHandler::setFileName(const std::string& fileName)
{
    mFileName = fileName;
}

std::string XmlErrorHandler::toString( const std::string& type,
        const XERCES_CPP_NAMESPACE::SAXParseException& exc ) const
{
    std::stringstream strs;
    strs << "A" << type << " occured while parsing " << mFileName 
         << " at line " << exc.getLineNumber() << " column " <<  exc.getColumnNumber();

    if( exc.getSystemId() != NULL )
        strs << " with system " << XmlHelper::transcodeToStdString( exc.getSystemId() );
    if( exc.getPublicId() != NULL )
        strs << " with public " << XmlHelper::transcodeToStdString( exc.getPublicId() );
    return strs.str();
}

void XmlErrorHandler::warning(const XERCES_CPP_NAMESPACE::SAXParseException& exc)
{
    LOG_MESSAGE(Logger::CORE, toString( " warning ", exc ) );
}

void XmlErrorHandler::error(const XERCES_CPP_NAMESPACE::SAXParseException& exc)
{
    LOG_ERROR(Logger::CORE, toString( "n error", exc ) );
}

void XmlErrorHandler::fatalError(const XERCES_CPP_NAMESPACE::SAXParseException& exc)
{
    LOG_CRITICAL(Logger::CORE, toString( " fatal error", exc ) );
}

void XmlErrorHandler::resetErrors()
{
}