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

#include "DialogManager.h"

#include <xercesc/dom/DOM.hpp>

#include "Dialog.h"
#include "DialogCondition.h"
#include "DialogElement.h"
#include "DialogImplication.h"
#include "DialogOption.h"
#include "DialogParagraph.h"
#include "DialogResponse.h"
#include "DialogVariable.h"
#include "XmlPropertyReader.h"

using namespace Ogre;
using namespace XERCES_CPP_NAMESPACE;

template<>
    rl::DialogManager* Ogre::Singleton<rl::DialogManager>::ms_Singleton = NULL;

namespace rl
{

    DialogManager::DialogManager()
        : XmlProcessor(), ScriptLoader()
    {
        mScriptPatterns.push_back("*.dialog");
    }

    DialogManager::~DialogManager()
    {
    }

    const StringVector& DialogManager::getScriptPatterns() const
    {
        return mScriptPatterns;
    }
    
	Ogre::Real DialogManager::getLoadingOrder() const
    {
        return 1000;
    }

    void DialogManager::parseScript(DataStreamPtr& stream, const Ogre::String& groupName)
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

    void DialogManager::processDialog(DOMElement* dialogElem)
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
                    DialogResponse *response = processResponseClasses(curChildChild, dialogPrototype);
                    if (response)
                    {
                        dialogPrototype->setStartResponse(response);
                    }
                }
            }
        }
        LOG_MESSAGE(Logger::AI, "Processed Dialog "+ name);
    }

    void DialogManager::processElementNodes(XERCES_CPP_NAMESPACE::DOMElement* dialogXml, const Ogre::String& nodeName, DialogPrototype* dialogPrototype)
    {
        DOMNodeList* dialogElemNodes = dialogXml->getElementsByTagName(AutoXMLCh(nodeName.c_str()).data());
        for (XMLSize_t i = 0; i < dialogElemNodes->getLength(); ++i)
        {
            DOMElement* dialogElemXml = static_cast<DOMElement*>(dialogElemNodes->item(i));
            if (!hasAttribute(dialogElemXml, "id"))
            {
                Throw(WrongFormatException, "option/switchoption/response/switchresponse node without id found");
            }
            int id = getAttributeValueAsInteger(dialogElemXml, "id");
            CeGuiString text = getValueAsString(dialogElemXml);

            if (hasNodeName(dialogElemXml, "switchoption"))
            {
                DialogOption* option = new DialogSelection<DialogOption>(id);
                option->setLabel(text);
                dialogPrototype->addOption(option);
            }
            else if (hasNodeName(dialogElemXml, "option"))
            {
                DialogOption* option = new DialogOption(id);
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

    DialogResponse* DialogManager::processResponseClasses(DOMNode *node, DialogPrototype *dialogPrototype)
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

    DialogResponse* DialogManager::processResponse(DOMElement *responseXml, DialogManager::DialogPrototype *dialogPrototype, bool subelements)
    {
        int id = getAttributeValueAsInteger(responseXml, "id");
        DialogResponse* response = dialogPrototype->getResponse(id);

        if (!response)  Throw(IllegalArgumentException, "No response with ID "+StringConverter::toString(id));

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
                else if (hasNodeName(cur, "gotoresponse"))
                {
                    int id = getAttributeValueAsInteger(static_cast<DOMElement*>(cur), "id");
                    response->addParagraph(new DialogGotoResponse(dialogPrototype->getResponse(id)));
                }
            }

            if (!paragraphsDefined)
            {
                CeGuiString responseXmlText = getValueAsString(responseXml);
                response->addParagraph(new DialogParagraph(responseXmlText));
            }
        }            

        return response;
    }

    DialogResponse* DialogManager::processSwitchResponse(XERCES_CPP_NAMESPACE::DOMElement* switchRespXml, DialogPrototype* dialogPrototype)
    {
        int id = getAttributeValueAsInteger(switchRespXml, "id");
        DialogSelection<DialogResponse>* response = dynamic_cast<DialogSelection<DialogResponse>*>(dialogPrototype->getResponse(id));

        if (!response)  Throw(IllegalArgumentException, "No switchresponse with ID "+StringConverter::toString(id));

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

    DialogOption* DialogManager::processOptionClasses(DOMNode *node, DialogPrototype *dialogPrototype)
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

    DialogOption* DialogManager::processOption(DOMElement *optionXml, DialogManager::DialogPrototype *dialogPrototype, bool subelements)
    {
        int id = getAttributeValueAsInteger(optionXml, "id");
        DialogOption* option = dialogPrototype->getOption(id);

        if (!option)    Throw(IllegalArgumentException, "No option with ID "+StringConverter::toString(id));

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
            }

            if (!paragraphsDefined)
            {
                CeGuiString optionXmlText = getValueAsString(optionXml);
                option->addParagraph(new DialogParagraph(optionXmlText));
            }
        }

        return option;
    }

    DialogOption* DialogManager::processSwitchOption(DOMElement *switchOptXml, DialogManager::DialogPrototype *dialogPrototype)
    {
        int id = getAttributeValueAsInteger(switchOptXml, "id");
        DialogOptionSelection* option = dynamic_cast<DialogOptionSelection*>(dialogPrototype->getOption(id));

        if (!option) Throw(IllegalArgumentException, "No switchoption with ID "+StringConverter::toString(id));

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
        }
            
        return option;
    }

    void DialogManager::createDialogVariable(DOMElement *variableXml, DialogManager::DialogPrototype *dialogPrototype)
    {
        XmlPropertyReader reader;
        PropertyEntry entry = reader.processProperty(variableXml);
        dialogPrototype->setProperty(entry.first, entry.second);
    }

    DialogCondition* DialogManager::processIf(DOMElement *ifXml)
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

    DialogParagraph* DialogManager::processParagraph(DOMElement* paragraphXml)
    {
        Ogre::String voicefile = "";
        if (hasAttribute(paragraphXml, "voicefile"))
        {
            voicefile = getAttributeValueAsStdString(paragraphXml, "voicefile");
        }
        return new DialogParagraph(getValueAsString(paragraphXml), voicefile);
    }

    DialogCondition* DialogManager::processCase(DOMElement *caseXml)
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

    Dialog* DialogManager::createDialog(const Ogre::String& name, Creature* pc, Creature* npc)
    {
        std::map<Ogre::String, DialogPrototype*>::iterator it = mDialogs.find(name);
        if (it == mDialogs.end())
        {
            return NULL;
        }

        Dialog* dialog = it->second->createDialog(pc, npc); ///@todo save dialogs
		dialog->initialize();
		return dialog;
    }

    void DialogManager::DialogPrototype::addOption(DialogOption* option)
    {
        if (mOptionCache.find(option->getId()) != mOptionCache.end())
        {
            Throw(IllegalArgumentException, 
                "Duplicate option/switchoption ID "+Ogre::StringConverter::toString(option->getId()));
        }
        mOptionCache[option->getId()] = option;
    }

    DialogOption* DialogManager::DialogPrototype::getOption(int id) const
    {
        std::map<int, DialogOption*>::const_iterator it = mOptionCache.find(id);
        if (it == mOptionCache.end())
        {
            return NULL;
        }
        return it->second;
    }

    void DialogManager::DialogPrototype::addResponse(DialogResponse* response)
    {
        if (mResponseCache.find(response->getId()) != mResponseCache.end())
        {
            Throw(IllegalArgumentException, 
                "Duplicate Response/switchResponse ID "+Ogre::StringConverter::toString(response->getId()));
        }
        mResponseCache[response->getId()] = response;
    }

    DialogResponse* DialogManager::DialogPrototype::getResponse(int id) const
    {
        std::map<int, DialogResponse*>::const_iterator it = mResponseCache.find(id);
        if (it == mResponseCache.end())
        {
            return NULL;
        }
        return it->second;
    }

    Dialog* DialogManager::DialogPrototype::createDialog(Creature* pc, Creature* npc)
    {
        Dialog* dialog = new Dialog(pc, npc);
        dialog->setStartResponse(mDialogStart);
        
        for (PropertyRecord::PropertyRecordMap::const_iterator it = mPropertyVariables.begin(); 
            it != mPropertyVariables.end(); ++it)
        {
            dialog->setProperty(it->first, it->second);
        }

        return dialog;
    }

    void DialogManager::DialogPrototype::setProperty(const Ogre::String &key, const rl::Property &value)
    {
        mPropertyVariables.setProperty(key, value);
    }

    void DialogManager::DialogPrototype::setStartResponse(DialogResponse* start)
    {
        mDialogStart = start;
    }

    DialogCondition* DialogManager::processConditionClasses(DOMElement* conditionXml)
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

    DialogVariable* DialogManager::processVariableClasses(DOMElement* variableXml)
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
            int modifier = 0;
            if (hasAttribute(variableXml, "modifier"))
            {
                modifier = getAttributeValueAsInteger(variableXml, "modifier");
            }
            return new EigenschaftsProbeVariable(attr, modifier);
        }
        else if (hasNodeName(variableXml, "talentcheck"))
        {
            CeGuiString attr = getAttributeValueAsString(variableXml, "talent");
            int modifier = 0;
            if (hasAttribute(variableXml, "modifier"))
            {
                modifier = getAttributeValueAsInteger(variableXml, "modifier");
            }
            return new TalentProbeVariable(attr, modifier);
        }

        return NULL;
    }


    DialogImplication* DialogManager::processImplicationClasses(DOMNode* implicationXml)
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
        }

        return NULL;
    }
}
