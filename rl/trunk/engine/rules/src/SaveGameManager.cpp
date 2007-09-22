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

#include <XmlProcessor.h>

#include <xercesc/dom/DOM.hpp>
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

    //SaveGameIndexWriter::SaveGameIndexWriter()
    //{
    //}

    //void SaveGameIndexWriter::buildIndexFile(const SaveGameEntryMap &map)
    //{
    //    initializeXml();

    //    XMLCh tempStr[256];
    //    XMLString::transcode("LS", tempStr, 255);
    //    mImplementation = DOMImplementationRegistry::getDOMImplementation(tempStr);
    //    mWriter = static_cast<DOMImplementationLS*>(mImplementation)->createDOMWriter();
    //    XMLString::transcode(((Ogre::String)(ConfigurationManager::getSingleton().getModulesRootDirectory() + "/saves/index.saves")).c_str(), tempStr, 255);
    //    mTarget = new LocalFileFormatTarget(tempStr);
    //    mDocument = static_cast<DOMImplementation*>(mImplementation)->createDocument(0, XMLString::transcode("SaveGameIndexFile"), 0);
    //    
    //    if (mWriter->canSetFeature(XMLUni::fgDOMWRTDiscardDefaultContent, true))
    //        mWriter->setFeature(XMLUni::fgDOMWRTDiscardDefaultContent, true);

    //    if (mWriter->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true))
    //         mWriter->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);

    //    mWriter->setEncoding(XMLUni::fgISO88591EncodingString);

    //    mDocument->setNodeValue(XMLString::transcode("SaveGameIndexFile")); //Set name of document root node

    //     //Write SaveGameIndexVersion
    //    setAttributeValueAsString(mDocument->getDocumentElement(), "Version", "0.1");


    //    //write save games
    //    SaveGameEntryMap::const_iterator it_saves;

    //    for(it_saves = map.begin(); it_saves != map.end(); it_saves++)
    //    {
    //        DOMElement* saveGame = appendChildElement(mDocument, mDocument->getDocumentElement(), "savegame");
    //        setAttributeValueAsString(saveGame, "Name", it_saves->second->getName());

    //        PropertyMap map = it_saves->second->getAllProperties()->toPropertyMap();
    //        PropertyMap::iterator it_properties;
    //        for(it_properties = map.begin(); it_properties != map.end(); it_properties++)
    //        {
    //            this->processProperty(saveGame, PropertyEntry(it_properties->first.c_str(), it_properties->second));
    //        }
    //    }        


    //    mWriter->writeNode(mTarget, *mDocument);


    //    mWriter->release();

    //    delete mDocument;
    //    delete mTarget;

    //    shutdownXml();
    //}

    //SaveGameIndexReader::SaveGameIndexReader()
    //{
    //}

    //SaveGameEntryMap SaveGameIndexReader::parseIndexFile(Ogre::DataStreamPtr &stream, const Ogre::String &groupName)
    //{
    //    SaveGameEntryMap entries;

    //    initializeXml();

    //    /*Ogre::String fileName = ConfigurationManager::getSingleton().getModulesRootDirectory() + "/saves/index.saves";

    //    FILE* fileHandle = fopen(fileName.c_str(), "r");

    //    if(fileHandle)
    //    {
    //        Ogre::DataStreamPtr stream = Ogre::DataStreamPtr(new Ogre::FileHandleDataStream(fileHandle));*/

    //        /*char* temp = new char[1024];

    //        stream->read(temp, 1022);

    //        LOG_MESSAGE(Logger::RULES, temp);

    //        delete[] temp;*/

    //        if(stream->size())
    //        {
    //            DOMDocument* doc = loadDocument( stream);

    //            ///@todo: Right version of save game file index?

    //            DOMNodeList* saveGameDefsXml = doc->getDocumentElement()->getElementsByTagName(AutoXMLCh("savegame").data());
    //            for(unsigned int i = 0; i < saveGameDefsXml->getLength(); i++)
    //            {
    //                DOMElement* curNode = static_cast<DOMElement*>(saveGameDefsXml->item(i));
    //                SaveGameFile * file = new SaveGameFile(this->getAttributeValueAsString(curNode, "Name"));
    //                
    //                DOMNodeList* saveGameDefChildren = curNode->getChildNodes();
    //                for (XMLSize_t childIdx = 0; childIdx < saveGameDefChildren->getLength(); childIdx++)
    //                {
    //                    DOMNode* curChild = saveGameDefChildren->item(childIdx);
    //                    if (curChild->getNodeType() == DOMNode::ELEMENT_NODE)
    //                    {
    //                        PropertyEntry entry = processProperty(static_cast<DOMElement*>(curChild));
    //                        if (entry.first != "")
    //                        {
    //                            file->setProperty(entry.first, entry.second);
    //                        }
    //                    }
    //                }
    //                entries[this->getAttributeValueAsString(curNode, "Name").c_str()] = file;
    //            }

    //            doc->release();
    //        }
    //    /*}*/

    //    shutdownXml();

    //    return entries;
    //}

    SaveGameHeaderReader::SaveGameHeaderReader()
    {
    }

    void SaveGameHeaderReader::parseHeader(Ogre::DataStreamPtr &stream, const Ogre::String &groupName, SaveGameFile* file)
    {
        initializeXml();

        if(stream->size())
        {
            DOMDocument* doc = loadDocument(stream);

             DOMNodeList* headerDefsXml = doc->getDocumentElement()->getElementsByTagName(AutoXMLCh("header").data());
             if(headerDefsXml->getLength())
             {
                 DOMElement* elem = static_cast<DOMElement*>(headerDefsXml->item(0));
                 DOMNodeList* headerDefChildren = elem->getChildNodes();
                 for(XMLSize_t childIdx = 0; childIdx < headerDefChildren->getLength(); childIdx++)
                 {
                     DOMNode* curChild = headerDefChildren->item(childIdx);
                     if (curChild->getNodeType() == DOMNode::ELEMENT_NODE)
                     {
                         PropertyEntry entry = processProperty(static_cast<DOMElement*>(curChild));
                         if(entry.first != "")
                         {
                            file->setProperty(entry.first, entry.second);
                         }
                     }
                 }
             }
        }

        shutdownXml();
    }

    SaveGameManager::SaveGameManager()
    { 
        Ogre::ResourceGroupManager::getSingleton().createResourceGroup("SaveGames");

        mScriptPatterns.push_back("*.save");
        Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(this);

        Ogre::ResourceGroupManager::getSingleton().addResourceLocation(ConfigurationManager::getSingleton().getModulesRootDirectory() 
            + "/saves", "FileSystem", "SaveGames");
        Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("SaveGames");
    }

    SaveGameManager::~SaveGameManager()
    {
    }

    SaveGameEntryMap SaveGameManager::listSaveGames()
    {
        return mSaveGames;
    }

    void SaveGameManager::saveSaveGameFile(const CeGuiString &name)
    {
        time_t rawTime;
        tm* localTime; 
        time(&rawTime);
        localTime = localtime(&rawTime); 
        SaveGameFile* file = new SaveGameFile(name);
        file->setProperty("Time", Property(printTimeAsString(localTime)));
        file->setProperty("ModuleID", Property(CoreSubsystem::getSingleton().getActiveAdventureModule()->getName()));

        mSaveGames[name] = file;

        SaveGameFileWriter writer;
        writer.buildSaveGameFile(file);
    }

    void SaveGameManager::loadSaveGameFile(const CeGuiString &name)
    {
        if(SaveGameFileExists(name))
        {
            SaveGameFile file(name);
            ///@todo: SaveGameReader
        }
    }

    void SaveGameManager::deleteSaveGameFile(const CeGuiString &name)
    {
        if(SaveGameFileExists(name))
        {
            static_cast<SaveGameFile*>(mSaveGames[name])->deleteFileFromStorage();
            delete mSaveGames[name];
            mSaveGames.erase(name);
        }
    }

    bool SaveGameManager::SaveGameFileExists(const CeGuiString &name)
    {
        if(mSaveGames.find(name) != mSaveGames.end())
            return true;
        return false;
    }
    
    const Ogre::StringVector& SaveGameManager::getScriptPatterns() const
    {
        return mScriptPatterns;
    }

    Ogre::Real SaveGameManager::getLoadingOrder() const
    {
        return 2000.0f;
    }

    void SaveGameManager::parseScript(Ogre::DataStreamPtr &stream, const Ogre::String &groupName)
    {
        Ogre::String name = stream->getName();
        name = name.substr(0, name.length()-5); //delete ".save" at the and of the name

        SaveGameFile* file = new SaveGameFile(name);
        file->setDataStream(stream);
        LOG_MESSAGE(Logger::RULES, "Parsing header of save game: " + name);
        SaveGameHeaderReader reader;
        reader.parseHeader(stream, groupName, file);
        
        mSaveGames[name] = file;
    }
}
