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
#include "AiPrerequisites.h"

#include <list>
#include <tinyxml2.h>

#include "Creature.h"
#include "Properties.h"
#include "XmlProcessor.h"

namespace rl
{
    class Dialog;
    class DialogCondition;
    class DialogElement;
    class DialogImplication;
    class DialogOption;
    class DialogParagraph;
    class DialogResponse;
    class DialogVariable;
    class Property;

    class DialogLoaderImpl : private XmlProcessor, public tinyxml2::XMLVisitor
    {
    public:
        DialogLoaderImpl();
        virtual ~DialogLoaderImpl();

        Dialog* createDialog(const Ogre::String& name, const CreatureList& participants) const;
        void parseDialog(Ogre::DataStreamPtr& stream, const Ogre::String& groupName);
        virtual bool VisitEnter(const tinyxml2::XMLElement& element, const tinyxml2::XMLAttribute* firstAttribute);

    private:
        class DialogParticipant
        {
        public:
            DialogParticipant(
                const CeGuiString& personId, int goId, const CeGuiString& goClass, const CeGuiString& name);

            bool isMatching(Creature* go) const;
            const CeGuiString& getPersonId() const;

        private:
            const CeGuiString mPersonId;
            int mGoId;
            const CeGuiString mGoClass;
            const CeGuiString mName;
        };

        class DialogPrototype
        {
        public:
            DialogPrototype();
            ~DialogPrototype();
            void addOption(DialogOption* option);
            DialogOption* getOption(const CeGuiString& id) const;
            void addResponse(DialogResponse* option);
            DialogResponse* getResponse(const CeGuiString& id) const;

            void setStartResponse(DialogResponse* response);
            Dialog* createDialog(const CreatureList& participants);
            void setProperty(const CeGuiString& key, const Property& value);

            void addParticipant(DialogParticipant* participant);

        private:
            std::map<CeGuiString, DialogOption*> mOptionCache;
            std::map<CeGuiString, DialogResponse*> mResponseCache;
            std::list<DialogParticipant*> mParticipantFilter;
            DialogResponse* mDialogStart;
            PropertyRecord mPropertyVariables;
        };

        void processDialog(const tinyxml2::XMLElement* dialogXml);
        DialogResponse* processResponseClasses(const tinyxml2::XMLNode* node, DialogPrototype* dialogPrototype);
        DialogResponse* processResponse(
            const tinyxml2::XMLElement* responseXml, DialogPrototype* dialogPrototype, bool subelements);
        DialogResponse* processSwitchResponse(
            const tinyxml2::XMLElement* switchRespXml, DialogPrototype* dialogPrototype);
        DialogOption* processOptionClasses(const tinyxml2::XMLNode* node, DialogPrototype* dialogPrototype);
        DialogOption* processOption(
            const tinyxml2::XMLElement* optionXml, DialogPrototype* dialogPrototype, bool subelements);
        DialogOption* processSwitchOption(const tinyxml2::XMLElement* switchOptXml, DialogPrototype* dialogPrototype);
        DialogCondition* processIf(const tinyxml2::XMLElement* ifXml);
        DialogCondition* processCase(const tinyxml2::XMLElement* caseXml);
        DialogVariable* processVariableClasses(const tinyxml2::XMLElement* variableXml);
        DialogCondition* processConditionClasses(const tinyxml2::XMLElement* conditionXml);
        DialogParagraph* processParagraph(const tinyxml2::XMLElement* paragraphXml);
        DialogImplication* processImplicationClasses(const tinyxml2::XMLNode* implicationXml);
        DialogParticipant* processPerson(const tinyxml2::XMLElement* personXml);
        void processTranslation(DialogElement* element, const tinyxml2::XMLNode* translationXml);
        void createDialogVariable(const tinyxml2::XMLElement* variableXml, DialogPrototype* dialogPrototype);
        void processElementNodes(
            const tinyxml2::XMLElement* dialogXml, const Ogre::String& nodeName, DialogPrototype* dialogPrototype);

        std::map<Ogre::String, DialogPrototype*> mDialogs;
    };
}
