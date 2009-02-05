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
#ifndef __DialogManager_H__
#define __DialogManager_H__

#include "AiPrerequisites.h"

#include "Properties.h"
#include "SaveGameData.h"
#include "XmlProcessor.h"

namespace rl
{
    class Creature;
	class Dialog;
	class DialogLoader;

    class _RlAiExport DialogManager 
        : public Ogre::Singleton<DialogManager>, 
        public PropertyHolder,
        public SaveGameData
    {
    public:
        static const Ogre::String PROPERTY_DIALOGS;
        static const Ogre::String PROPERTY_DIALOG;
        static const Ogre::String PROPERTY_DIALOG_NAME;
        static const Ogre::String PROPERTY_NPCS;

        DialogManager();
        ~DialogManager();

        Dialog* createDialog(const Ogre::String& name, rl::Creature* npc, rl::Creature* pc);
        Dialog* createDialog(const Ogre::String& name, const std::vector<Creature*>& pcs, const std::vector<Creature*>& npcs);

        virtual const Property getProperty(const CeGuiString& key) const;
        virtual void setProperty(const CeGuiString& key, const Property& value);
        virtual PropertyKeys getAllPropertyKeys() const;
        virtual void writeData(SaveGameFileWriter *writer);
        virtual void readData(SaveGameFileReader* reader);
        virtual CeGuiString getXmlNodeIdentifier() const;
        virtual int getPriority() const;

    private:
 
        class DialogConfiguration
        {
        public:
            DialogConfiguration(const Ogre::String& name, const std::vector<Creature*>& npcs);

            const Ogre::String& getName() const;
            const std::vector<Creature*>& getNpcs() const;
            
            bool operator==(const DialogConfiguration&) const;
            bool operator<(const DialogConfiguration&) const;
        private:
            Ogre::String mDialogName;
            std::vector<Creature*> mNpcs;
        };


        std::map<DialogConfiguration, Dialog*> mDialogStates;
		DialogLoader* mDialogLoader;
    };

}

#endif // __DialogManager_H__
