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
#include "stdinc.h"

#include "DialogManager.h"


#include "Creature.h"
#include "Dialog.h"
#include "DialogLoader.h"
#include "GameObjectManager.h"
#include "SaveGameManager.h"

using namespace Ogre;
using namespace XERCES_CPP_NAMESPACE;

template<>
    rl::DialogManager* Ogre::Singleton<rl::DialogManager>::ms_Singleton = NULL;

namespace rl
{
    const Ogre::String DialogManager::PROPERTY_DIALOGS = "dialogs";
    const Ogre::String DialogManager::PROPERTY_DIALOG = "dialog";
    const Ogre::String DialogManager::PROPERTY_DIALOG_NAME = "name";
    const Ogre::String DialogManager::PROPERTY_NPCS = "npc";

    DialogManager::DialogManager()
    {
		mDialogLoader = new DialogLoader();
		Ogre::ResourceGroupManager::getSingleton()._registerScriptLoader(
			mDialogLoader);
        SaveGameManager::getSingleton().registerSaveGameData(this);
    }

    DialogManager::~DialogManager()
    {
		Ogre::ResourceGroupManager::getSingleton()._unregisterScriptLoader(
			mDialogLoader);
		delete mDialogLoader;
        SaveGameManager::getSingleton().unregisterSaveGameData(this);
		std::map<DialogConfiguration, Dialog*>::iterator itr = mDialogStates.begin();
		std::map<DialogConfiguration, Dialog*>::iterator end = mDialogStates.end();
		for(; itr != end; ++itr)
		{
			delete itr->second;
		}
		mDialogStates.clear();
    }


    const Property DialogManager::getProperty(const CeGuiString& key) const
    {
        if (key == DialogManager::PROPERTY_DIALOGS)
        {
            PropertyArray vec;
            for (std::map<DialogConfiguration, Dialog*>::const_iterator it
                = mDialogStates.begin(); it != mDialogStates.end(); ++it)
            {
                PropertyMap curDialogProp;
                PropertyRecordPtr dialogProps = it->second->getAllProperties();
                curDialogProp[DialogManager::PROPERTY_DIALOG] = dialogProps->toPropertyMap();
                curDialogProp[DialogManager::PROPERTY_DIALOG_NAME] = Property(it->first.getName());

                PropertyArray npcs;

                for (std::vector<Creature*>::const_iterator itNpc = it->first.getNpcs().begin();
                    itNpc != it->first.getNpcs().end(); ++itNpc)
                {
                    npcs.push_back(GameObjectManager::getSingleton().toProperty(*itNpc));
                }

                curDialogProp[DialogManager::PROPERTY_NPCS] = Property(npcs);
            }

            return Property(vec);
        }

        Throw(IllegalArgumentException, key + " is not a property of DialogManager");
    }
	
	int DialogManager::getPriority() const
	{
		return 50;
	}

    PropertyKeys DialogManager::getAllPropertyKeys() const
    {
        PropertyKeys keys;
        keys.insert(DialogManager::PROPERTY_DIALOGS);
        return keys;
    }

    void DialogManager::setProperty(const CeGuiString& key, const Property& value)
    {
        ///@todo implement
    }

    void DialogManager::writeData(SaveGameFileWriter *writer)
    {
        LOG_MESSAGE(Logger::RULES, "Saving dialogs");

        PropertyRecordPtr set = getAllProperties();
        writer->writeEachProperty(this, set->toPropertyMap());
    }

    void DialogManager::readData(SaveGameFileReader* reader)
    {
        LOG_MESSAGE(Logger::RULES, "Loading dialogs");

        PropertyRecordPtr properties = reader->getAllPropertiesAsRecord(this);
        setProperties(properties);
    }

    CeGuiString DialogManager::getXmlNodeIdentifier() const
    {
        return "dialogs";
    }

    Dialog* DialogManager::createDialog(const Ogre::String& name, Creature* npc, Creature* pc)
    {
        std::vector<Creature*> npcs;
        npcs.push_back(npc);
        std::vector<Creature*> pcs;
        pcs.push_back(pc);

        return createDialog(name, npcs, pcs);
    }

    Dialog* DialogManager::createDialog(const Ogre::String& name, const std::vector<Creature*>& pcs, const std::vector<Creature*>& npcs)
    {
        std::map<DialogConfiguration, Dialog*>::iterator it 
            = mDialogStates.find(DialogConfiguration(name, npcs));

        Dialog* dialog;
        if (it != mDialogStates.end())
        {
            dialog = (*it).second;
        }
        else 
        {
			dialog = mDialogLoader->createDialog(name, pcs, npcs); ///@todo save dialogs
			
			if (!dialog) 
			{
				return NULL;
			}
			
            dialog->initialize();
            mDialogStates[DialogConfiguration(name, npcs)] = dialog;
        }
		return dialog;
    }
	
    DialogManager::DialogConfiguration::DialogConfiguration(const Ogre::String& name, const std::vector<Creature*>& npcs)
        : mDialogName(name), mNpcs(npcs)
    {
    }

    const Ogre::String& DialogManager::DialogConfiguration::getName() const
    {
        return mDialogName;
    }

    const std::vector<Creature*>& DialogManager::DialogConfiguration::getNpcs() const
    {
        return mNpcs;
    }

    bool DialogManager::DialogConfiguration::operator <(const rl::DialogManager::DialogConfiguration & other) const
    {
        return mDialogName < other.mDialogName;
    }

    bool DialogManager::DialogConfiguration::operator ==(const rl::DialogManager::DialogConfiguration & other) const
    {
        return (mDialogName == other.mDialogName) 
            && (mNpcs == other.mNpcs);
    }


}
