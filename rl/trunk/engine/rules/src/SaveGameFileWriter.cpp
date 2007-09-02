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

#include "stdinc.h"

#include <XmlHelper.h>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMAttr.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <Properties.h>

#include "SaveGameFileWriter.h"
#include <GameObjectManager.h>

#include <OgreResourceManager.h>
#include <CEGUIPropertyHelper.h>

using namespace XERCES_CPP_NAMESPACE;
using namespace Ogre;

namespace rl
{
    void SaveGameFileWriter::buildSaveGameFile(SaveGameFile *file)
    {
        //@toto: build
        XMLPlatformUtils::Initialize();

        XmlHelper::initializeTranscoder();

        XMLCh tempStr[100];
        XMLString::transcode("LS", tempStr, 99);
        mImplementation = DOMImplementationRegistry::getDOMImplementation(tempStr);
        mWriter = ((DOMImplementationLS*)mImplementation)->createDOMWriter();
        mTarget = file->save();
        mDocument = ((DOMImplementation*)mImplementation)->createDocument(0, XMLString::transcode("SaveGameFile"), 0);
        mDocument->setNodeValue(XMLString::transcode("SaveGameFile"));

        if (mWriter->canSetFeature(XMLUni::fgDOMWRTDiscardDefaultContent, true))
            mWriter->setFeature(XMLUni::fgDOMWRTDiscardDefaultContent, true);

        if (mWriter->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true))
             mWriter->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);

        //Write globals
        DOMElement* globals = XmlHelper::appendChildElement(mDocument, mDocument->getDocumentElement(), "globals");

        //Write game objects
        DOMElement* gameobjects = XmlHelper::appendChildElement(mDocument, mDocument->getDocumentElement(), "gameobjects");
        
        std::list<const GameObject*>::const_iterator it;
        std::list<const GameObject*> gos;
        gos = GameObjectManager::getSingleton().getAllGameObjects();

        for(it = gos.begin(); it != gos.end(); it++)
        {
            DOMElement* gameobject = XmlHelper::appendChildElement(mDocument, gameobjects, "gameobject");

            PropertyMap map = (*it)->getAllProperties()->toPropertyMap();
            PropertyMap::iterator it2;
            for(it2 = map.begin(); it2 != map.end(); it2)
            {
                this->processProperty(gameobject, PropertyEntry(it2->first.c_str(), it2->second));
            }
        }        

        //Write scripts
        DOMElement* scripts = XmlHelper::appendChildElement(mDocument, mDocument->getDocumentElement(), "scripts");

        mWriter->writeNode(mTarget, *mDocument);

        mWriter->release();
        delete mTarget;
    }
}
