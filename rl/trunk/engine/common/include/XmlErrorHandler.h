/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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
#ifndef __XmlErrorHandler_H__
#define __XmlErrorHandler_H__
#include <string>
#include <xercesc/sax/ErrorHandler.hpp>
#include "CommonPrerequisites.h"
#include "XmlHelper.h"



namespace rl
{
    /*class _RlCommonExport XmlErrorHandler 
        :   public XERCES_CPP_NAMESPACE::ErrorHandler,
            private XmlProcessor
    {
    public:
        XmlErrorHandler(void);
        virtual~XmlErrorHandler(void);

        void setFileName(const std::string& fileName);

        virtual void warning(const XERCES_CPP_NAMESPACE::SAXParseException& exc);
        virtual void error(const XERCES_CPP_NAMESPACE::SAXParseException& exc);
        virtual void fatalError(const XERCES_CPP_NAMESPACE::SAXParseException& exc);
        virtual void resetErrors();
    private:
        std::string toString( const std::string& type,
                              const XERCES_CPP_NAMESPACE::SAXParseException& exc ) const;
        
        std::string mFileName;
    };*/
}
#endif
