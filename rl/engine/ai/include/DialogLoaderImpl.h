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
#include <tinyxml.h>

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

	class DialogLoaderImpl : private XmlProcessor, public TiXmlVisitor
    {
    public:
        DialogLoaderImpl();
        virtual ~DialogLoaderImpl();

        Dialog* createDialog(const Ogre::String& name, const CreatureList& participants) const;
        void parseDialog(Ogre::DataStreamPtr& stream, const Ogre::String& groupName);
        virtual bool VisitEnter(const TiXmlElement &element, const TiXmlAttribute *firstAttribute);

    private:
        class DialogParticipant
        {
        public:
            DialogParticipant(const CeGuiString& personId,
                    int goId, const CeGuiString& goClass, const CeGuiString& name);

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

        void processDialog(const TiXmlElement* dialogXml);
        DialogResponse* processResponseClasses(const TiXmlNode *node, DialogPrototype* dialogPrototype);
        DialogResponse* processResponse(const TiXmlElement* responseXml, DialogPrototype* dialogPrototype, bool subelements);
        DialogResponse* processSwitchResponse(const TiXmlElement* switchRespXml, DialogPrototype* dialogPrototype);
        DialogOption* processOptionClasses(const TiXmlNode *node, DialogPrototype *dialogPrototype);
        DialogOption* processOption(const TiXmlElement* optionXml, DialogPrototype* dialogPrototype, bool subelements);
        DialogOption* processSwitchOption(const TiXmlElement* switchOptXml, DialogPrototype* dialogPrototype);
        DialogCondition* processIf(const TiXmlElement *ifXml);
        DialogCondition* processCase(const TiXmlElement *caseXml);
        DialogVariable* processVariableClasses(const TiXmlElement* variableXml);
        DialogCondition* processConditionClasses(const TiXmlElement* conditionXml);
        DialogParagraph* processParagraph(const TiXmlElement* paragraphXml);
        DialogImplication* processImplicationClasses(const TiXmlNode* implicationXml);
        DialogParticipant* processPerson(const TiXmlElement* personXml);
        void processTranslation(DialogElement* element, const TiXmlNode* translationXml);
        void createDialogVariable(const TiXmlElement* variableXml, DialogPrototype* dialogPrototype);
        void processElementNodes(const TiXmlElement* dialogXml, const Ogre::String& nodeName, DialogPrototype* dialogPrototype);

        std::map<Ogre::String, DialogPrototype*> mDialogs;

    };
}
