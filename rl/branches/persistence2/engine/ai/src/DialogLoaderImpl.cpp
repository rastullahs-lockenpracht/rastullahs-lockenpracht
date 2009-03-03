/*
 *  DialogLoaderImpl.cpp
 *  Rastullah
 *
 *  Created by Sascha Kolewa on 04.12.08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#include "stdinc.h"

#include <xercesc/dom/DOM.hpp>

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
using namespace XERCES_CPP_NAMESPACE;

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
        initializeXml();

        DOMDocument* doc = loadDocument(stream);
        if (doc)
        {
            DOMNodeList* dialogNodes = doc->getElementsByTagName(AutoXMLCh("dialog").data());

            for (XMLSize_t i = 0; i < dialogNodes->getLength(); ++i)
            {
                DOMNode* cur = dialogNodes->item(i);
                processDialog(static_cast<DOMElement*>(cur));
            }
        }

        shutdownXml();
    }

    Dialog* DialogLoaderImpl::createDialog(const String& name, const std::list<Creature*>& participants) const
    {
        std::map<Ogre::String, DialogPrototype*>::const_iterator it =
        mDialogs.find(name);

        if (it == mDialogs.end())
        {
            return NULL;
        }

        return it->second->createDialog(participants);
    }

    void DialogLoaderImpl::processDialog(DOMElement* dialogElem)
    {
        DialogPrototype* dialogPrototype = new DialogPrototype();
        Ogre::String name = getAttributeValueAsStdString(dialogElem, "name");
        mDialogs[name] = dialogPrototype;

        // first step: process all possible references
        processElementNodes(dialogElem, "option", dialogPrototype);
        processElementNodes(dialogElem, "switchoption", dialogPrototype);
        processElementNodes(dialogElem, "response", dialogPrototype);
        processElementNodes(dialogElem, "switchresponse", dialogPrototype);

        for (DOMNode* curChild = dialogElem->getFirstChild(); curChild != NULL; curChild = curChild->getNextSibling())
        {
            if (hasNodeName(curChild, "variable"))
            {
                createDialogVariable(static_cast<DOMElement*>(curChild), dialogPrototype);
            }
            else if (hasNodeName(curChild, "option"))
            {
                processOption(static_cast<DOMElement*>(curChild), dialogPrototype, true);
            }
            else if (hasNodeName(curChild, "switchoption"))
            {
                processSwitchOption(static_cast<DOMElement*>(curChild), dialogPrototype);
            }
            else if (hasNodeName(curChild, "response"))
            {
                processResponse(static_cast<DOMElement*>(curChild), dialogPrototype, true);
            }
            else if (hasNodeName(curChild, "switchresponse"))
            {
                processSwitchResponse(static_cast<DOMElement*>(curChild), dialogPrototype);
            }
            else if (hasNodeName(curChild, "start"))
            {
                for (DOMNode* curChildChild = curChild->getFirstChild(); curChildChild != NULL;
                     curChildChild = curChildChild->getNextSibling())
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
                for (DOMNode* curChildChild = curChild->getFirstChild(); curChildChild != NULL;
                                     curChildChild = curChildChild->getNextSibling())
                {
                    if (hasNodeName(curChildChild, "person"))
                    {
                        dialogPrototype->addParticipant(processPerson(static_cast<DOMElement*>(curChildChild)));
                    }
                }
            }
        }
        LOG_MESSAGE(Logger::AI, "Processed Dialog "+ name);
    }

    void DialogLoaderImpl::processElementNodes(XERCES_CPP_NAMESPACE::DOMElement* dialogXml, const Ogre::String& nodeName, DialogPrototype* dialogPrototype)
    {
        DOMNodeList* dialogElemNodes = dialogXml->getElementsByTagName(AutoXMLCh(nodeName.c_str()).data());
        for (XMLSize_t i = 0; i < dialogElemNodes->getLength(); ++i)
        {
            DOMElement* dialogElemXml = static_cast<DOMElement*>(dialogElemNodes->item(i));
            if (!hasAttribute(dialogElemXml, "id"))
            {
                Throw(WrongFormatException, "option/switchoption/response/switchresponse node without id found");
            }
            CeGuiString id = getAttributeValueAsString(dialogElemXml, "id");
            CeGuiString text = getValueAsString(dialogElemXml);

            if (hasNodeName(dialogElemXml, "switchoption"))
            {
                DialogOption* option = new DialogSelection<DialogOption>(id);
                option->setLabel(text);
                dialogPrototype->addOption(option);
            }
            else if (hasNodeName(dialogElemXml, "option"))
            {
                bool isAutoSelected = getAttributeValueAsBool(dialogElemXml, "autoSelect");
                DialogOption* option = new DialogOption(id, isAutoSelected);
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
                dialogPrototype->addResponse(new DialogResponse(id));
            }
            else if (hasNodeName(dialogElemXml, "switchresponse"))
            {
                dialogPrototype->addResponse(new DialogResponseSelection(id));
            }
        }

    }

    DialogResponse* DialogLoaderImpl::processResponseClasses(DOMNode *node, DialogPrototype *dialogPrototype)
    {
        DialogResponse* response = NULL;
        if (hasNodeName(node, "response"))
        {
            response = processResponse(static_cast<DOMElement*>(node), dialogPrototype, true);
        }
        else if (hasNodeName(node, "gotoresponse"))
        {
            response = processResponse(static_cast<DOMElement*>(node), dialogPrototype, false);
        }
        else if (hasNodeName(node, "switchresponse"))
        {
            response = processSwitchResponse(static_cast<DOMElement*>(node), dialogPrototype);
        }
        return response;
    }

    DialogResponse* DialogLoaderImpl::processResponse(DOMElement *responseXml, DialogLoaderImpl::DialogPrototype *dialogPrototype, bool subelements)
    {
        CeGuiString id = getAttributeValueAsString(responseXml, "id");

        DialogResponse* response = dialogPrototype->getResponse(id);

        if (!response)  Throw(IllegalArgumentException, CeGuiString("No response with ID "+ id).c_str());

        bool languageDefined = false;
        DOMElement* defaultLanguage = NULL;
        if (subelements)
        {
            bool paragraphsDefined = false;

            for (DOMNode* cur = responseXml->getFirstChild(); cur != NULL; cur = cur->getNextSibling())
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
                    response->addParagraph(processParagraph(static_cast<DOMElement*>(cur)));
                    paragraphsDefined = true;
                }
                // process translations
                else if (hasNodeName(cur, "t"))
                {
                    DOMElement* translation = static_cast<DOMElement*>(cur);
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
                    CeGuiString id = getAttributeValueAsString(static_cast<DOMElement*>(cur), "id");
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

    DialogResponse* DialogLoaderImpl::processSwitchResponse(XERCES_CPP_NAMESPACE::DOMElement* switchRespXml, DialogPrototype* dialogPrototype)
    {
        CeGuiString id = getAttributeValueAsString(switchRespXml, "id");
        DialogSelection<DialogResponse>* response = dynamic_cast<DialogSelection<DialogResponse>*>(dialogPrototype->getResponse(id));

        if (!response)  Throw(IllegalArgumentException, CeGuiString("No switchresponse with ID "+ id).c_str());

        for (DOMNode* cur = switchRespXml->getFirstChild(); cur != NULL; cur = cur->getNextSibling())
        {
            DialogVariable* variable = processVariableClasses(static_cast<DOMElement*>(cur));
            if (variable != NULL)
            {
                response->setVariable(variable);
            }
            else if (hasNodeName(cur, "case"))
            {
                DialogCondition* condition = processCase(static_cast<DOMElement*>(cur));
                for (DOMNode* caseChild = cur->getFirstChild(); caseChild != NULL; caseChild = caseChild->getNextSibling())
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

    DialogOption* DialogLoaderImpl::processOptionClasses(DOMNode *node, DialogPrototype *dialogPrototype)
    {
        DialogOption* option = NULL;
        if (hasNodeName(node, "option"))
        {
            option = processOption(static_cast<DOMElement*>(node), dialogPrototype, true);
        }
        else if (hasNodeName(node, "optionref"))
        {
            option = processOption(static_cast<DOMElement*>(node), dialogPrototype, false);
        }
        else if (hasNodeName(node, "switchoption"))
        {
            option = processSwitchOption(static_cast<DOMElement*>(node), dialogPrototype);
        }
        return option;
    }

    DialogOption* DialogLoaderImpl::processOption(DOMElement *optionXml, DialogLoaderImpl::DialogPrototype *dialogPrototype, bool subelements)
    {
        CeGuiString id = getAttributeValueAsString(optionXml, "id");

        DialogOption* option = dialogPrototype->getOption(id);

        if (!option)    Throw(IllegalArgumentException, CeGuiString("No option with ID "+ id).c_str());

        bool languageDefined = false;
        DOMElement* defaultLanguage = NULL;

        if (subelements)
        {
            bool paragraphsDefined = false;

            for (DOMNode* cur = optionXml->getFirstChild(); cur != NULL; cur = cur->getNextSibling())
            {
                DialogResponse* response = processResponseClasses(cur, dialogPrototype);
                if (response)
                {
                    option->setResponse(response);
                }
                else if (hasNodeName(cur, "if"))
                {
                    option->setPrecondition(processIf(static_cast<DOMElement*>(cur)));
                }
                else if (hasNodeName(cur, "p"))
                {
                    option->addParagraph(processParagraph(static_cast<DOMElement*>(cur)));
                    paragraphsDefined = true;
                }
                // process translations
                else if (hasNodeName(cur, "t"))
                {
                    DOMElement* translation = static_cast<DOMElement*>(cur);
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

    DialogOption* DialogLoaderImpl::processSwitchOption(DOMElement *switchOptXml, DialogLoaderImpl::DialogPrototype *dialogPrototype)
    {
        CeGuiString id = getAttributeValueAsString(switchOptXml, "id");
        DialogOptionSelection* option = dynamic_cast<DialogOptionSelection*>(dialogPrototype->getOption(id));

        if (!option) Throw(IllegalArgumentException, CeGuiString("No switchoption with ID "+ id).c_str());

        for (DOMNode* cur = switchOptXml->getFirstChild(); cur != NULL; cur = cur->getNextSibling())
        {
            DialogVariable* variable = processVariableClasses(static_cast<DOMElement*>(cur));
            if (variable != NULL)
            {
                option->setVariable(variable);
            }
            else if (hasNodeName(cur, "case"))
            {
                DialogCondition* condition = processCase(static_cast<DOMElement*>(cur));
                for (DOMNode* caseChild = cur->getFirstChild(); caseChild != NULL; caseChild = caseChild->getNextSibling())
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
                DOMElement* translation = static_cast<DOMElement*>(cur);
                // check loca
                if (getAttributeValueAsStdString(translation, "language") ==
                   ConfigurationManager::getSingleton().getStringSetting("Localization", "language"))
                {
                    std::string label = getAttributeValueAsStdString(translation, "label");
                    if (!label.empty())
                    {
                        option->setLabel(label);
                    }
                }
            }
        }

        return option;
    }

    void DialogLoaderImpl::createDialogVariable(DOMElement *variableXml, DialogLoaderImpl::DialogPrototype *dialogPrototype)
    {
        XmlPropertyReader reader;
        PropertyEntry entry = reader.processProperty(variableXml);
        dialogPrototype->setProperty(entry.first, entry.second);
    }

    DialogCondition* DialogLoaderImpl::processIf(DOMElement *ifXml)
    {
        DialogCondition* cond = NULL;
        DialogVariable* var = NULL;

        for (DOMNode* cur = ifXml->getFirstChild(); cur != NULL; cur = cur->getNextSibling())
        {
            if (cur->getNodeType() == DOMNode::ELEMENT_NODE)
            {
                DOMElement* curElem = static_cast<DOMElement*>(cur);

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

    DialogParagraph* DialogLoaderImpl::processParagraph(DOMElement* paragraphXml)
    {
        Ogre::String voicefile = "";
        if (hasAttribute(paragraphXml, "voicefile"))
        {
            voicefile = getAttributeValueAsStdString(paragraphXml, "voicefile");
        }
        return new DialogParagraph(getValueAsString(paragraphXml), voicefile);
    }

    DialogCondition* DialogLoaderImpl::processCase(DOMElement *caseXml)
    {
        DialogCondition* cond = NULL;

        for (DOMNode* cur = caseXml->getFirstChild(); cur != NULL; cur = cur->getNextSibling())
        {
            if (cur->getNodeType() == DOMNode::ELEMENT_NODE)
            {
                DialogCondition* curCond = processConditionClasses(static_cast<DOMElement*>(cur));
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
            Throw(IllegalArgumentException,
                  CeGuiString("Duplicate option/switchoption ID "+ option->getId()).c_str());
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
                  CeGuiString("Duplicate Response/switchResponse ID "+ response->getId()).c_str());
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

    Dialog* DialogLoaderImpl::DialogPrototype::createDialog(const list<Creature*>& participants)
    {
        Dialog* dialog = new Dialog();

        // for easy 1pc-1nsc dialogs (active player is "player", one NSC is "nsc")
        Creature* player = PartyManager::getSingleton().getActiveCharacter();
        dialog->addParticipant("player", player);

        bool found1stNpc = false;
        Party playerChars = PartyManager::getSingleton().getCharacters();

        for (list<Creature*>::const_iterator itPart = participants.begin(); itPart != participants.end(); ++itPart)
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
                    dialog->addParticipant("nsc", curCr);
                    found1stNpc = true;
                }
            }


            for (list<DialogLoaderImpl::DialogParticipant*>::iterator it = mParticipantFilter.begin();
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

    DialogLoaderImpl::DialogParticipant::DialogParticipant(const CeGuiString& personId, int goId,
            const CeGuiString& goClass, const CeGuiString& name)
        : mPersonId(personId), mGoId(goId), mGoClass(goClass), mName(name)
    {
    }

    const CeGuiString& DialogLoaderImpl::DialogParticipant::getPersonId() const
    {
        return mPersonId;
    }

    bool DialogLoaderImpl::DialogParticipant::isMatching(Creature* creature) const
    {
        return (mGoId == -1 || creature->getId() == mGoId)
            && (mGoClass.empty() || creature->getClassId() == mGoClass)
            && (mName.empty() || creature->getName() == mName);
    }

    DialogCondition* DialogLoaderImpl::processConditionClasses(DOMElement* conditionXml)
    {
        if (hasNodeName(conditionXml, "equals"))
        {
            return new DialogConditionEquals(
                                             getAttributeValueAsString(conditionXml, "value"));
        }
        else if (hasNodeName(conditionXml, "inrange"))
        {
            return new DialogConditionInRange(
                                              getAttributeValueAsReal(conditionXml, "from"),
                                              getAttributeValueAsReal(conditionXml, "to"));
        }
        else if (hasNodeName(conditionXml, "lower"))
        {
            return new DialogConditionLowerThan(
                                                getAttributeValueAsReal(conditionXml, "value"));
        }
        else if (hasNodeName(conditionXml, "lowereq"))
        {
            return new DialogConditionLowerOrEquals(
                                                    getAttributeValueAsReal(conditionXml, "value"));
        }
        else if (hasNodeName(conditionXml, "greater"))
        {
            return new DialogConditionGreaterThan(
                                                  getAttributeValueAsReal(conditionXml, "value"));
        }
        else if (hasNodeName(conditionXml, "greatereq"))
        {
            return new DialogConditionGreaterOrEquals(
                                                      getAttributeValueAsReal(conditionXml, "value"));
        }

        return NULL;
    }

    DialogVariable* DialogLoaderImpl::processVariableClasses(DOMElement* variableXml)
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


    DialogImplication* DialogLoaderImpl::processImplicationClasses(DOMNode* implicationXml)
    {
        if (implicationXml->getNodeType() == DOMNode::ELEMENT_NODE)
        {
            DOMElement* implicationElem = static_cast<DOMElement*>(implicationXml);

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

    void DialogLoaderImpl::processTranslation(DialogElement* element, DOMNode* translationXml)
    {
        for (DOMNode* cur = translationXml->getFirstChild(); cur != NULL; cur = cur->getNextSibling())
        {
            if (hasNodeName(cur, "p"))
            {
                element->addParagraph(processParagraph(static_cast<DOMElement*>(cur)));
            }
        }
    }

    DialogLoaderImpl::DialogParticipant* DialogLoaderImpl::processPerson(DOMElement* personXml)
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

        if (hasAttribute(personXml, "goId"))
        {
            goId = getAttributeValueAsInteger(personXml, "goId");
        }
        if (hasAttribute(personXml, "goClass"))
        {
            goClass = getAttributeValueAsString(personXml, "goClass");
        }
        if (hasAttribute(personXml, "name"))
        {
            name = getAttributeValueAsString(personXml, "name");
        }

        return new DialogParticipant(personId, goId, goClass, name);
    }

}

