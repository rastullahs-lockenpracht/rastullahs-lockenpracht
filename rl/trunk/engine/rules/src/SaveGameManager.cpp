/* 
 * (C) 2003-2007. Team Pantheon. www.team-pantheon.de
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
 *  along with this program; if not you can get it here:
 *  http://www.jpaulmorrison.com/fbp/artistic2.htm
 */

#include "stdinc.h"

#include "SaveGameManager.h"

#include <CoreSubsystem.h>
#include <ContentModule.h>
#include <ConfigurationManager.h>

#include <xercesc/framework/LocalFileFormatTarget.hpp>

template<> rl::SaveGameManager* Ogre::Singleton<rl::SaveGameManager>::ms_Singleton = 0;

using namespace XERCES_CPP_NAMESPACE;

namespace rl
{
    CeGuiString printTimeAsString(tm* time)
    {
        CeGuiString string = ((Ogre::String)(Ogre::StringConverter::toString(time->tm_mday) + "." 
            + Ogre::StringConverter::toString(time->tm_mon) + "."
            + Ogre::StringConverter::toString(time->tm_year + 1900) + " - " 
            + Ogre::StringConverter::toString(time->tm_hour) + ":"
            + Ogre::StringConverter::toString(time->tm_min) + ":" 
            + Ogre::StringConverter::toString(time->tm_sec))).c_str();
        return string;
    }

    SaveGameIndexWriter::SaveGameIndexWriter()
    {
    }

    void SaveGameIndexWriter::buildIndexFile(const SaveGameEntryMap &map)
    {
        initializeXml();

        XMLCh tempStr[256];
        XMLString::transcode("LS", tempStr, 255);
        mImplementation = DOMImplementationRegistry::getDOMImplementation(tempStr);
        mWriter = static_cast<DOMImplementationLS*>(mImplementation)->createDOMWriter();
        XMLString::transcode(((Ogre::String)(ConfigurationManager::getSingleton().getModulesRootDirectory() + "/saves/index.saves")).c_str(), tempStr, 255);
        mTarget = new LocalFileFormatTarget(tempStr);
        mDocument = static_cast<DOMImplementation*>(mImplementation)->createDocument(0, XMLString::transcode("SaveGameIndexFile"), 0);
        
        if (mWriter->canSetFeature(XMLUni::fgDOMWRTDiscardDefaultContent, true))
            mWriter->setFeature(XMLUni::fgDOMWRTDiscardDefaultContent, true);

        if (mWriter->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true))
             mWriter->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);

        mDocument->setNodeValue(XMLString::transcode("SaveGameIndexFile")); //Set name of document root node

         //Write SaveGameIndexVersion
        setAttributeValueAsString(mDocument->getDocumentElement(), "Version", "0.1");


        //write save games
        SaveGameEntryMap::const_iterator it_saves;

        for(it_saves = map.begin(); it_saves != map.end(); it_saves++)
        {
            DOMElement* saveGame = appendChildElement(mDocument, mDocument->getDocumentElement(), "savegame");
            setAttributeValueAsString(saveGame, "Name", it_saves->second->getName());

            PropertyMap map = it_saves->second->getAllProperties()->toPropertyMap();
            PropertyMap::iterator it_properties;
            for(it_properties = map.begin(); it_properties != map.end(); it_properties++)
            {
                this->processProperty(saveGame, PropertyEntry(it_properties->first.c_str(), it_properties->second));
            }
        }        


        mWriter->writeNode(mTarget, *mDocument);


        mWriter->release();

        delete mDocument;
        delete mTarget;

        shutdownXml();
    }

    SaveGameIndexReader::SaveGameIndexReader()
    {
    }

    SaveGameEntryMap SaveGameIndexReader::parseIndexFile() const
    {
        SaveGameEntryMap entries;
        return entries;
    }

    SaveGameManager::SaveGameManager()
    {
        mReader = new SaveGameIndexReader();
        mWriter = new SaveGameIndexWriter();
    }

    SaveGameManager::~SaveGameManager()
    {
        delete mReader;
        delete mWriter;
    }

    SaveGameEntryMap SaveGameManager::listSaveGames()
    {
        return mReader->parseIndexFile();
    }

    void SaveGameManager::saveSaveGameFile(const CeGuiString &name)
    {
        time_t rawTime;
        tm* localTime; 
        time(&rawTime);
        localTime = localtime(&rawTime); 
        SaveGameFile file(name);
        file.setProperty("Time", Property(printTimeAsString(localTime)));
        file.setProperty("ModuleID", Property(CoreSubsystem::getSingleton().getActiveAdventureModule()->getName()));

        SaveGameEntryMap map = listSaveGames();
        map[name] = &file;

        mWriter->buildIndexFile(map);

        SaveGameFileWriter writer;
        writer.buildSaveGameFile(&file);
    }

    void SaveGameManager::loadSaveGameFile(const CeGuiString &name)
    {
        ///@todo: SaveGameReader
    }
}
