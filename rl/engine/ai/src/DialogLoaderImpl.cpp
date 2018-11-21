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

#ifdef __APPLE__
#include <CEGUI/CEGUIPropertyHelper.h>
#else
#include <CEGUI/PropertyHelper.h>
#endif

#include "DialogLoaderImpl.h"

#include "ConfigurationManager.h"
#include "Creature.h"
#include "Dialog.h"
#include "DialogCondition.h"
#include "DialogElement.h"
#include "DialogImplication.h"
#include "DialogOption.h"
#include "DialogParagraph.h"
#include "DialogResponse.h"
#include "DialogVariable.h"
#include "PartyManager.h"
#include "XmlPropertyReader.h"

using namespace Ogre;
using namespace std;

namespace rl
{
    DialogLoaderImpl::DialogLoaderImpl()
        : XmlProcessor()
    {
    }

    DialogLoaderImpl::~DialogLoaderImpl()
    {
        std::map<Ogre::String, DialogPrototype*>::iterator itr = mDialogs.begin();
        std::map<Ogre::String, DialogPrototype*>::iterator end = mDialogs.end();
        for (; itr != end; ++itr)
        {
            DialogPrototype* p = itr->second;
            delete p;
        }
        mDialogs.clear();
    }

    void DialogLoaderImpl::parseDialog(DataStreamPtr& stream, const Ogre::String& groupName)
    {
        tinyxml2::XMLDocument* doc = loadDocument(stream);
        if (doc)
        {
            doc->Accept(this);
        }
        delete doc;
    }

    bool DialogLoaderImpl::VisitEnter(const tinyxml2::XMLElement& element, const tinyxml2::XMLAttribute* firstAttribute)
    {
        if (element.Value() == std::string("dialog"))
        {
            processDialog(&element);
            return false;
        }
        return true;
    }

    Dialog* DialogLoaderImpl::createDialog(const String& name, const CreatureList& participants) const
    {
        std::map<Ogre::String, DialogPrototype*>::const_iterator it = mDialogs.find(name);

        if (it == mDialogs.end())
        {
            return NULL;
        }

        return it->second->createDialog(participants);
    }

    void DialogLoaderImpl::processDialog(const tinyxml2::XMLElement* dialogElem)
    {
        DialogPrototype* dialogPrototype = new DialogPrototype();
        Ogre::String name = getAttributeValueAsStdString(dialogElem, "name");
        mDialogs[name] = dialogPrototype;

        // first step: process all possible references
        processElementNodes(dialogElem, "option", dialogPrototype);
        processElementNodes(dialogElem, "switchoption", dialogPrototype);
        processElementNodes(dialogElem, "response", dialogPrototype);
        processElementNodes(dialogElem, "switchresponse", dialogPrototype);

        for (const tinyxml2::XMLNode* curChild = dialogElem->FirstChild(); curChild != NULL;
             curChild = curChild->NextSibling())
        {
            if (hasNodeName(curChild, "variable"))
            {
                createDialogVariable(curChild->ToElement(), dialogPrototype);
            }
            else if (hasNodeName(curChild, "option"))
            {
                processOption(curChild->ToElement(), dialogPrototype, true);
            }
            else if (hasNodeName(curChild, "switchoption"))
            {
                processSwitchOption(curChild->ToElement(), dialogPrototype);
            }
            else if (hasNodeName(curChild, "response"))
            {
                processResponse(curChild->ToElement(), dialogPrototype, true);
            }
            else if (hasNodeName(curChild, "switchresponse"))
            {
                processSwitchResponse(curChild->ToElement(), dialogPrototype);
            }
            else if (hasNodeName(curChild, "start"))
            {
                for (const tinyxml2::XMLNode* curChildChild = curChild->FirstChild(); curChildChild != NULL;
                     curChildChild = curChildChild->NextSibling())
                {
                    DialogResponse* response = processResponseClasses(curChildChild, dialogPrototype);
                    if (response)
                    {
                        dialogPrototype->setStartResponse(response);
                    }
                }
            }
            else if (hasNodeName(curChild, "persons"))
            {
                for (const tinyxml2::XMLNode* curChildChild = curChild->FirstChild(); curChildChild != NULL;
                     curChildChild = curChildChild->NextSibling())
                {
                    if (hasNodeName(curChildChild, "person"))
                    {
                        dialogPrototype->addParticipant(processPerson(curChildChild->ToElement()));
                    }
                }
            }
        }
        LOG_MESSAGE(Logger::AI, "Processed Dialog " + name);
    }

    void DialogLoaderImpl::processElementNodes(
        const tinyxml2::XMLElement* dialogXml, const Ogre::String& nodeName, DialogPrototype* dialogPrototype)
    {
        XmlElementList dialogElemNodes = getElementsByTagName(dialogXml, nodeName.c_str());
        for (XmlElementList::iterator it = dialogElemNodes.begin(); it != dialogElemNodes.end(); ++it)
        {
            const tinyxml2::XMLElement* dialogElemXml = *it;
            if (!hasAttribute(dialogElemXml, "id"))
            {
                Throw(WrongFormatException, "option/switchoption/response/switchresponse node without id found");
            }
            CeGuiString id = getAttributeValueAsString(dialogElemXml, "id");
            CeGuiString text = getValueAsString(dialogElemXml);
            CeGuiString person = "";
            if (hasAttribute(dialogElemXml, "person"))
            {
                person = getAttributeValueAsString(dialogElemXml, "person");
            }

            if (hasNodeName(dialogElemXml, "switchoption"))
            {
                DialogOption* option = new DialogSelection<DialogOption>(id, person);
                option->setLabel(text);
                dialogPrototype->addOption(option);
            }
            else if (hasNodeName(dialogElemXml, "option"))
            {
                bool isAutoSelected = getAttributeValueAsBool(dialogElemXml, "autoSelect");
                DialogOption* option = new DialogOption(id, person, isAutoSelected);
                if (hasAttribute(dialogElemXml, "label"))
                {
                    option->setLabel(getAttributeValueAsString(dialogElemXml, "label"));
                }
                else
                {
                    option->setLabel(text);
                }
                dialogPrototype->addOption(option);
            }
            else if (hasNodeName(dialogElemXml, "response"))
            {
                dialogPrototype->addResponse(new DialogResponse(id, person));
            }
            else if (hasNodeName(dialogElemXml, "switchresponse"))
            {
                dialogPrototype->addResponse(new DialogResponseSelection(id, person));
            }
        }
    }

    DialogResponse* DialogLoaderImpl::processResponseClasses(
        const tinyxml2::XMLNode* node, DialogPrototype* dialogPrototype)
    {
        DialogResponse* response = NULL;
        if (hasNodeName(node, "response"))
        {
            response = processResponse(node->ToElement(), dialogPrototype, true);
        }
        else if (hasNodeName(node, "gotoresponse"))
        {
            response = processResponse(node->ToElement(), dialogPrototype, false);
        }
        else if (hasNodeName(node, "switchresponse"))
        {
            response = processSwitchResponse(node->ToElement(), dialogPrototype);
        }
        return response;
    }

    DialogResponse* DialogLoaderImpl::processResponse(
        const tinyxml2::XMLElement* responseXml, DialogLoaderImpl::DialogPrototype* dialogPrototype, bool subelements)
    {
        CeGuiString id = getAttributeValueAsString(responseXml, "id");

        DialogResponse* response = dialogPrototype->getResponse(id);

        if (!response)
            Throw(IllegalArgumentException, CeGuiString("No response with ID " + id).c_str());

        bool languageDefined = false;
        const tinyxml2::XMLElement* defaultLanguage = NULL;
        if (subelements)
        {
            bool paragraphsDefined = false;

            for (const tinyxml2::XMLNode* cur = responseXml->FirstChild(); cur != NULL; cur = cur->NextSibling())
            {
                DialogOption* option = processOptionClasses(cur, dialogPrototype);
                if (option)
                {
                    response->addOption(option);
                    continue;
                }

                DialogImplication* implication = processImplicationClasses(cur);
                if (implication)
                {
                    response->addImplication(implication);
                    continue;
                }

                if (hasNodeName(cur, "p"))
                {
                    response->addParagraph(processParagraph(cur->ToElement()));
                    paragraphsDefined = true;
                }
                // process translations
                else if (hasNodeName(cur, "t"))
                {
                    const tinyxml2::XMLElement* translation = cur->ToElement();
                    // check loca
                    if (getAttributeValueAsStdString(translation, "language")
                        == ConfigurationManager::getSingleton().getStringSetting("Localization", "language"))
                    {
                        processTranslation(response, translation);
                        languageDefined = true;
                    }
                    // set german as default language
                    if (getAttributeValueAsStdString(translation, "language") == "de")
                    {
                        defaultLanguage = translation;
                    }
                    paragraphsDefined = true;
                }
                else if (hasNodeName(cur, "gotoresponse"))
                {
                    CeGuiString id = getAttributeValueAsString(cur->ToElement(), "id");
                    response->addParagraph(new DialogGotoResponse(dialogPrototype->getResponse(id)));
                }
            }

            if (!paragraphsDefined)
            {
                CeGuiString responseXmlText = getValueAsString(responseXml);
                response->addParagraph(new DialogParagraph(responseXmlText));
            }
        }
        // use german as the default language if german is not set as
        // default language but no other language was found!
        if (!languageDefined && defaultLanguage != NULL
            && ConfigurationManager::getSingleton().getStringSetting("Localization", "language") != "de")
        {
            processTranslation(response, defaultLanguage);
            languageDefined = true;
        }

        return response;
    }

    DialogResponse* DialogLoaderImpl::processSwitchResponse(
        const tinyxml2::XMLElement* switchRespXml, DialogPrototype* dialogPrototype)
    {
        CeGuiString id = getAttributeValueAsString(switchRespXml, "id");
        DialogSelection<DialogResponse>* response
            = dynamic_cast<DialogSelection<DialogResponse>*>(dialogPrototype->getResponse(id));

        if (!response)
            Throw(IllegalArgumentException, CeGuiString("No switchresponse with ID " + id).c_str());

        for (const tinyxml2::XMLNode* cur = switchRespXml->FirstChild(); cur != NULL; cur = cur->NextSibling())
        {
            DialogVariable* variable = processVariableClasses(cur->ToElement());
            if (variable != NULL)
            {
                response->setVariable(variable);
            }
            else if (hasNodeName(cur, "case"))
            {
                DialogCondition* condition = processCase(cur->ToElement());
                for (const tinyxml2::XMLNode* caseChild = cur->FirstChild(); caseChild != NULL;
                     caseChild = caseChild->NextSibling())
                {
                    DialogResponse* responseCase = processResponseClasses(caseChild, dialogPrototype);
                    if (responseCase)
                    {
                        response->addElement(condition, responseCase);
                        break;
                    }
                }
            }
        }

        return response;
    }

    DialogOption* DialogLoaderImpl::processOptionClasses(
        const tinyxml2::XMLNode* node, DialogPrototype* dialogPrototype)
    {
        DialogOption* option = NULL;
        if (hasNodeName(node, "option"))
        {
            option = processOption(node->ToElement(), dialogPrototype, true);
        }
        else if (hasNodeName(node, "optionref"))
        {
            option = processOption(node->ToElement(), dialogPrototype, false);
        }
        else if (hasNodeName(node, "switchoption"))
        {
            option = processSwitchOption(node->ToElement(), dialogPrototype);
        }

        return option;
    }

    DialogOption* DialogLoaderImpl::processOption(
        const tinyxml2::XMLElement* optionXml, DialogLoaderImpl::DialogPrototype* dialogPrototype, bool subelements)
    {
        CeGuiString id = getAttributeValueAsString(optionXml, "id");

        DialogOption* option = dialogPrototype->getOption(id);

        if (!option)
            Throw(IllegalArgumentException, CeGuiString("No option with ID " + id).c_str());

        bool languageDefined = false;
        const tinyxml2::XMLElement* defaultLanguage = NULL;

        if (subelements)
        {
            bool paragraphsDefined = false;

            for (const tinyxml2::XMLNode* cur = optionXml->FirstChild(); cur != NULL; cur = cur->NextSibling())
            {
                DialogResponse* response = processResponseClasses(cur, dialogPrototype);
                if (response)
                {
                    option->setResponse(response);
                }
                else if (hasNodeName(cur, "if"))
                {
                    option->setPrecondition(processIf(cur->ToElement()));
                }
                else if (hasNodeName(cur, "p"))
                {
                    option->addParagraph(processParagraph(cur->ToElement()));
                    paragraphsDefined = true;
                }
                // process translations
                else if (hasNodeName(cur, "t"))
                {
                    const tinyxml2::XMLElement* translation = cur->ToElement();
                    // check locale
                    if (getAttributeValueAsStdString(translation, "language")
                        == ConfigurationManager::getSingleton().getStringSetting("Localization", "language"))
                    {
                        defaultLanguage = translation;
                        processTranslation(option, translation);
                        std::string label = getAttributeValueAsStdString(translation, "label");
                        if (!label.empty())
                        {
                            option->setLabel(label);
                        }
                        languageDefined = true;
                    }
                    // set german as default language
                    if (getAttributeValueAsStdString(translation, "language") == "de")
                    {
                        defaultLanguage = translation;
                    }
                    paragraphsDefined = true;
                }
            }

            if (!paragraphsDefined)
            {
                CeGuiString optionXmlText = getValueAsString(optionXml);
                option->addParagraph(new DialogParagraph(optionXmlText));
            }
        }
        // use german as the default language if german is not set as
        // default language but no other language was found!
        if (!languageDefined && defaultLanguage != NULL
            && ConfigurationManager::getSingleton().getStringSetting("Localization", "language") != "de")
        {
            processTranslation(option, defaultLanguage);
            std::string label = getAttributeValueAsStdString(defaultLanguage, "label");
            if (!label.empty())
            {
                option->setLabel(label);
            }
            languageDefined = true;
        }

        return option;
    }

    DialogOption* DialogLoaderImpl::processSwitchOption(
        const tinyxml2::XMLElement* switchOptXml, DialogLoaderImpl::DialogPrototype* dialogPrototype)
    {
        CeGuiString id = getAttributeValueAsString(switchOptXml, "id");
        DialogOptionSelection* option = dynamic_cast<DialogOptionSelection*>(dialogPrototype->getOption(id));

        if (!option)
        {
            Throw(IllegalArgumentException, CeGuiString("No switchoption with ID " + id).c_str());
        }

        for (const tinyxml2::XMLNode* cur = switchOptXml->FirstChild(); cur; cur = cur->NextSibling())
        {
            const tinyxml2::XMLElement* curElem = cur->ToElement();
            if (curElem)
            {
                DialogVariable* variable = processVariableClasses(curElem);
                if (variable != NULL)
                {
                    option->setVariable(variable);
                }
                else if (hasNodeName(cur, "case"))
                {
                    DialogCondition* condition = processCase(cur->ToElement());
                    for (const tinyxml2::XMLNode* caseChild = cur->FirstChild(); caseChild != NULL;
                         caseChild = caseChild->NextSibling())
                    {
                        DialogOption* optionCase = processOptionClasses(caseChild, dialogPrototype);
                        if (optionCase)
                        {
                            option->addElement(condition, optionCase);
                            break;
                        }
                    }
                }
                // process translations
                else if (hasNodeName(cur, "t"))
                {
                    const tinyxml2::XMLElement* translation = cur->ToElement();
                    // check loca
                    if (getAttributeValueAsStdString(translation, "language")
                        == ConfigurationManager::getSingleton().getStringSetting("Localization", "language"))
                    {
                        std::string label = getAttributeValueAsStdString(translation, "label");
                        if (!label.empty())
                        {
                            option->setLabel(label);
                        }
                    }
                }
            }
        }

        return option;
    }

    void DialogLoaderImpl::createDialogVariable(
        const tinyxml2::XMLElement* variableXml, DialogLoaderImpl::DialogPrototype* dialogPrototype)
    {
        XmlPropertyReader reader;
        PropertyEntry entry = reader.processProperty(variableXml);
        dialogPrototype->setProperty(entry.first, entry.second);
    }

    DialogCondition* DialogLoaderImpl::processIf(const tinyxml2::XMLElement* ifXml)
    {
        DialogCondition* cond = NULL;
        DialogVariable* var = NULL;

        for (const tinyxml2::XMLNode* cur = ifXml->FirstChild(); cur != NULL; cur = cur->NextSibling())
        {
            const tinyxml2::XMLElement* curElem = cur->ToElement();

            if (curElem)
            {
                DialogCondition* curCond = processConditionClasses(curElem);
                if (curCond)
                {
                    cond = curCond;
                }

                DialogVariable* curVar = processVariableClasses(curElem);
                if (curVar)
                {
                    var = curVar;
                }
            }
        }

        cond->setVariable(var);
        return cond;
    }

    DialogParagraph* DialogLoaderImpl::processParagraph(const tinyxml2::XMLElement* paragraphXml)
    {
        Ogre::String voicefile = "";
        CeGuiString person = "";
        if (hasAttribute(paragraphXml, "voicefile"))
        {
            voicefile = getAttributeValueAsStdString(paragraphXml, "voicefile");
        }
        if (hasAttribute(paragraphXml, "person"))
        {
            person = getAttributeValueAsString(paragraphXml, "person");
        }
        return new DialogParagraph(getValueAsString(paragraphXml), person, voicefile);
    }

    DialogCondition* DialogLoaderImpl::processCase(const tinyxml2::XMLElement* caseXml)
    {
        DialogCondition* cond = NULL;

        for (const tinyxml2::XMLNode* cur = caseXml->FirstChild(); cur != NULL; cur = cur->NextSibling())
        {
            const tinyxml2::XMLElement* curElem = cur->ToElement();
            if (curElem)
            {
                DialogCondition* curCond = processConditionClasses(curElem);
                if (curCond)
                {
                    cond = curCond;
                }
            }
        }

        return cond;
    }

    DialogLoaderImpl::DialogPrototype::DialogPrototype()
    {
    }

    DialogLoaderImpl::DialogPrototype::~DialogPrototype()
    {
        std::map<CeGuiString, DialogOption*>::iterator it;
        for (it = mOptionCache.begin(); it != mOptionCache.end(); it++)
            if (it->second != NULL)
                delete it->second;

        std::map<CeGuiString, DialogResponse*>::iterator it1;
        for (it1 = mResponseCache.begin(); it1 != mResponseCache.end(); it1++)
            if (it1->second != NULL)
                delete it1->second;
    }

    void DialogLoaderImpl::DialogPrototype::addOption(DialogOption* option)
    {
        if (mOptionCache.find(option->getId()) != mOptionCache.end())
        {
            Throw(IllegalArgumentException, CeGuiString("Duplicate option/switchoption ID " + option->getId()).c_str());
        }
        mOptionCache[option->getId()] = option;
    }

    DialogOption* DialogLoaderImpl::DialogPrototype::getOption(const CeGuiString& id) const
    {
        std::map<CeGuiString, DialogOption*>::const_iterator it = mOptionCache.find(id);
        if (it == mOptionCache.end())
        {
            return NULL;
        }
        return it->second;
    }

    void DialogLoaderImpl::DialogPrototype::addResponse(DialogResponse* response)
    {
        if (mResponseCache.find(response->getId()) != mResponseCache.end())
        {
            Throw(IllegalArgumentException,
                CeGuiString("Duplicate Response/switchResponse ID " + response->getId()).c_str());
        }
        mResponseCache[response->getId()] = response;
    }

    DialogResponse* DialogLoaderImpl::DialogPrototype::getResponse(const CeGuiString& id) const
    {
        std::map<CeGuiString, DialogResponse*>::const_iterator it = mResponseCache.find(id);
        if (it == mResponseCache.end())
        {
            return NULL;
        }
        return it->second;
    }

    Dialog* DialogLoaderImpl::DialogPrototype::createDialog(const CreatureList& participants)
    {
        Dialog* dialog = new Dialog();

        // for easy 1pc-1nsc dialogs (active player is "player", one NSC is "nsc")
        Creature* player = PartyManager::getSingleton().getActiveCharacter();
        dialog->addParticipant("player", player);

        bool found1stNpc = false;
        Party playerChars = PartyManager::getSingleton().getCharacters();

        for (CreatureList::const_iterator itPart = participants.begin(); itPart != participants.end(); ++itPart)
        {
            Creature* curCr = *itPart;

            if (!found1stNpc)
            {
                bool isInParty = false;
                for (Party::iterator itParty = playerChars.begin(); itParty != playerChars.end(); ++itParty)
                {
                    if (*itParty == curCr)
                    {
                        isInParty = true;
                        break;
                    }
                }

                if (!isInParty)
                {
                    dialog->addParticipant("npc", curCr);
                    found1stNpc = true;
                }
            }

            for (std::list<DialogLoaderImpl::DialogParticipant*>::iterator it = mParticipantFilter.begin();
                 it != mParticipantFilter.end(); ++it)
            {
                if ((*it)->isMatching(curCr))
                {
                    dialog->addParticipant((*it)->getPersonId(), curCr);
                }
            }
        }
        dialog->setStartResponse(mDialogStart);

        for (PropertyRecord::PropertyRecordMap::const_iterator it = mPropertyVariables.begin();
             it != mPropertyVariables.end(); ++it)
        {
            dialog->setProperty(it->first, it->second);
        }

        return dialog;
    }

    void DialogLoaderImpl::DialogPrototype::setProperty(const CeGuiString& key, const Property& value)
    {
        mPropertyVariables.setProperty(key, value);
    }

    void DialogLoaderImpl::DialogPrototype::setStartResponse(DialogResponse* start)
    {
        mDialogStart = start;
    }

    void DialogLoaderImpl::DialogPrototype::addParticipant(DialogLoaderImpl::DialogParticipant* participant)
    {
        mParticipantFilter.push_back(participant);
    }

    DialogLoaderImpl::DialogParticipant::DialogParticipant(
        const CeGuiString& personId, int goId, const CeGuiString& goClass, const CeGuiString& name)
        : mPersonId(personId)
        , mGoId(goId)
        , mGoClass(goClass)
        , mName(name)
    {
    }

    const CeGuiString& DialogLoaderImpl::DialogParticipant::getPersonId() const
    {
        return mPersonId;
    }

    bool DialogLoaderImpl::DialogParticipant::isMatching(Creature* creature) const
    {
        return (mGoId == -1 || creature->getId() == mGoId) && (mGoClass.empty() || creature->getClassId() == mGoClass)
            && (mName.empty() || creature->getName() == mName);
    }

    DialogCondition* DialogLoaderImpl::processConditionClasses(const tinyxml2::XMLElement* conditionXml)
    {
        if (hasNodeName(conditionXml, "equals"))
        {
            return new DialogConditionEquals(getAttributeValueAsString(conditionXml, "value"));
        }
        else if (hasNodeName(conditionXml, "inrange"))
        {
            return new DialogConditionInRange(
                getAttributeValueAsReal(conditionXml, "from"), getAttributeValueAsReal(conditionXml, "to"));
        }
        else if (hasNodeName(conditionXml, "lower"))
        {
            return new DialogConditionLowerThan(getAttributeValueAsReal(conditionXml, "value"));
        }
        else if (hasNodeName(conditionXml, "lowereq"))
        {
            return new DialogConditionLowerOrEquals(getAttributeValueAsReal(conditionXml, "value"));
        }
        else if (hasNodeName(conditionXml, "greater"))
        {
            return new DialogConditionGreaterThan(getAttributeValueAsReal(conditionXml, "value"));
        }
        else if (hasNodeName(conditionXml, "greatereq"))
        {
            return new DialogConditionGreaterOrEquals(getAttributeValueAsReal(conditionXml, "value"));
        }

        return NULL;
    }

    DialogVariable* DialogLoaderImpl::processVariableClasses(const tinyxml2::XMLElement* variableXml)
    {
        if (hasNodeName(variableXml, "dialogvariable"))
        {
            return new DialogPropertyVariable(getAttributeValueAsStdString(variableXml, "name"));
        }
        else if (hasNodeName(variableXml, "queststate"))
        {
            Ogre::String questId = getAttributeValueAsStdString(variableXml, "quest");
            Ogre::String prop = getAttributeValueAsStdString(variableXml, "property");
            return new QuestStateVariable(questId, prop);
        }
        else if (hasNodeName(variableXml, "attributecheck"))
        {
            CeGuiString attr = getAttributeValueAsString(variableXml, "attribute");
            CeGuiString target = getAttributeValueAsString(variableXml, "target");
            int modifier = 0;
            if (hasAttribute(variableXml, "modifier"))
            {
                modifier = getAttributeValueAsInteger(variableXml, "modifier");
            }
            return new EigenschaftsProbeVariable(attr, modifier, target);
        }
        else if (hasNodeName(variableXml, "talentcheck"))
        {
            CeGuiString attr = getAttributeValueAsString(variableXml, "talent");
            CeGuiString target = getAttributeValueAsString(variableXml, "target");
            int modifier = 0;
            if (hasAttribute(variableXml, "modifier"))
            {
                modifier = getAttributeValueAsInteger(variableXml, "modifier");
            }
            return new TalentProbeVariable(attr, modifier, target);
        }
        else if (hasNodeName(variableXml, "random"))
        {
            int maximum = getAttributeValueAsInteger(variableXml, "maximum");
            return new RandomVariable(maximum);
        }

        return NULL;
    }

    DialogImplication* DialogLoaderImpl::processImplicationClasses(const tinyxml2::XMLNode* implicationXml)
    {
        const tinyxml2::XMLElement* implicationElem = implicationXml->ToElement();

        if (implicationElem)
        {
            if (hasNodeName(implicationElem, "setvariable"))
            {
                Ogre::String variableName = getAttributeValueAsStdString(implicationElem, "name");
                CeGuiString variableValue = getAttributeValueAsString(implicationElem, "value");
                return new DialogVariableAssignment(variableName, variableValue);
            }
            else if (hasNodeName(implicationElem, "incvariable"))
            {
                Ogre::String variableName = getAttributeValueAsStdString(implicationElem, "name");
                CeGuiString variableValue = getAttributeValueAsString(implicationElem, "value");
                return new DialogVariableIncrease(variableName, variableValue);
            }
            else if (hasNodeName(implicationElem, "decvariable"))
            {
                Ogre::String variableName = getAttributeValueAsStdString(implicationElem, "name");
                CeGuiString variableValue = getAttributeValueAsString(implicationElem, "value");
                return new DialogVariableDecrease(variableName, variableValue);
            }
            else if (hasNodeName(implicationElem, "setoptionactive"))
            {
                CeGuiString id = getAttributeValueAsString(implicationElem, "id");
                bool value = getAttributeValueAsBool(implicationElem, "value");
                return new DialogElementActivation(id, value, true);
            }
            else if (hasNodeName(implicationElem, "exit"))
            {
                return new DialogExit();
            }
            else if (hasNodeName(implicationElem, "changequest"))
            {
                Ogre::String questId = getAttributeValueAsStdString(implicationElem, "quest");
                Ogre::String prop = getAttributeValueAsStdString(implicationElem, "property");
                CeGuiString newvalue = getAttributeValueAsString(implicationElem, "newvalue");
                return new QuestPropertyAssignment(questId, prop, newvalue);
            }
            else if (hasNodeName(implicationElem, "startcombat"))
            {
                return new CombatStart();
            }
        }

        return NULL;
    }

    void DialogLoaderImpl::processTranslation(DialogElement* element, const tinyxml2::XMLNode* translationXml)
    {
        for (const tinyxml2::XMLNode* cur = translationXml->FirstChild(); cur != NULL; cur = cur->NextSibling())
        {
            if (hasNodeName(cur, "p"))
            {
                element->addParagraph(processParagraph(cur->ToElement()));
            }
        }
    }

    DialogLoaderImpl::DialogParticipant* DialogLoaderImpl::processPerson(const tinyxml2::XMLElement* personXml)
    {
        CeGuiString personId(""), goClass(""), name("");
        int goId = -1;

        if (hasAttribute(personXml, "id"))
        {
            personId = getAttributeValueAsString(personXml, "id");
        }
        else
        {
            LOG_ERROR("DialogLoader", "person node without id found");
        }

        if (hasAttribute(personXml, "goid"))
        {
            goId = getAttributeValueAsInteger(personXml, "goid");
        }
        if (hasAttribute(personXml, "goclass"))
        {
            goClass = getAttributeValueAsString(personXml, "goclass");
        }
        if (hasAttribute(personXml, "name"))
        {
            name = getAttributeValueAsString(personXml, "name");
        }

        return new DialogParticipant(personId, goId, goClass, name);
    }
}
