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
#ifndef __DialogManager_H__
#define __DialogManager_H__

#include "AiPrerequisites.h"

#include <xercesc/dom/DOMElement.hpp>

#include "Properties.h"
#include "SaveGameManager.h"
#include "XmlProcessor.h"

namespace rl
{
    class Creature;
    class Dialog;
    class DialogCondition;
    class DialogElement;
    class DialogImplication;
    class DialogOption;
    class DialogParagraph;
    class DialogResponse;
    class DialogVariable;

    class _RlAiExport DialogManager 
        : public Ogre::Singleton<DialogManager>, 
        public Ogre::ScriptLoader, 
        public PropertyHolder,
        public SaveGameData,
        private XmlProcessor
    {
    public:
        static const Ogre::String PROPERTY_DIALOGS;
        static const Ogre::String PROPERTY_DIALOG;
        static const Ogre::String PROPERTY_DIALOG_NAME;
        static const Ogre::String PROPERTY_NPCS;

        DialogManager();
        ~DialogManager();

        virtual const Ogre::StringVector& getScriptPatterns() const;
        virtual void parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName);
		virtual Ogre::Real getLoadingOrder() const;
        Dialog* createDialog(const Ogre::String& name, rl::Creature* npc, rl::Creature* pc);
        Dialog* createDialog(const Ogre::String& name, const std::vector<Creature*>& pcs, const std::vector<Creature*>& npcs);

        virtual const Property getProperty(const Ogre::String& key) const;
        virtual void setProperty(const Ogre::String& key, const Property& value);
        virtual PropertyRecord* getAllProperties() const;
        virtual void writeData(SaveGameFileWriter *writer);
        virtual void readData(SaveGameFileReader* reader);
        virtual CeGuiString getXmlNodeIdentifier() const;
        virtual int getPriority() const;

    private:

        class DialogPrototype
        {
        public:
            void addOption(DialogOption* option);
            DialogOption* getOption(int id) const;
            void addResponse(DialogResponse* option);
            DialogResponse* getResponse(int id) const;

            void setStartResponse(DialogResponse* response);
            Dialog* createDialog(const std::vector<Creature*>& pcs, const std::vector<Creature*>& npcs);
            void setProperty(const Ogre::String& key, const Property& value);

        private:
            std::map<int, DialogOption*> mOptionCache;
            std::map<int, DialogResponse*> mResponseCache;
            DialogResponse* mDialogStart;
            PropertyRecord mPropertyVariables;
        };

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

        void processDialog(XERCES_CPP_NAMESPACE::DOMElement* dialogXml);
        DialogResponse* processResponseClasses(XERCES_CPP_NAMESPACE::DOMNode *node, DialogPrototype* dialogPrototype);
        DialogResponse* processResponse(XERCES_CPP_NAMESPACE::DOMElement* responseXml, DialogPrototype* dialogPrototype, bool subelements);
        DialogResponse* processSwitchResponse(XERCES_CPP_NAMESPACE::DOMElement* switchRespXml, DialogPrototype* dialogPrototype);
        DialogOption* processOptionClasses(XERCES_CPP_NAMESPACE::DOMNode *node, DialogPrototype *dialogPrototype);
        DialogOption* processOption(XERCES_CPP_NAMESPACE::DOMElement* optionXml, DialogPrototype* dialogPrototype, bool subelements);
        DialogOption* processSwitchOption(XERCES_CPP_NAMESPACE::DOMElement* switchOptXml, DialogPrototype* dialogPrototype);
        DialogCondition* processIf(XERCES_CPP_NAMESPACE::DOMElement *ifXml);
        DialogCondition* processCase(XERCES_CPP_NAMESPACE::DOMElement *caseXml);
        DialogVariable* processVariableClasses(XERCES_CPP_NAMESPACE::DOMElement* variableXml);
        DialogCondition* processConditionClasses(XERCES_CPP_NAMESPACE::DOMElement* conditionXml);
        DialogParagraph* processParagraph(XERCES_CPP_NAMESPACE::DOMElement* paragraphXml);
        DialogImplication* processImplicationClasses(XERCES_CPP_NAMESPACE::DOMNode* implicationXml);
        void processTranslation(DialogElement* element, XERCES_CPP_NAMESPACE::DOMNode* translationXml);
        void createDialogVariable(XERCES_CPP_NAMESPACE::DOMElement* variableXml, DialogPrototype* dialogPrototype);
        void processElementNodes(XERCES_CPP_NAMESPACE::DOMElement* dialogXml, const Ogre::String& nodeName, DialogPrototype* dialogPrototype);

        Ogre::StringVector mScriptPatterns;
        std::map<Ogre::String, DialogPrototype*> mDialogs;
        std::map<DialogConfiguration, Dialog*> mDialogStates;
    };

}

#endif // __DialogManager_H__
