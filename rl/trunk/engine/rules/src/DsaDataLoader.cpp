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
#include "stdinc.h" //precompiled header

#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

// #include "XdimlLoader.h"
#include "DsaDataLoader.h"

#include <OgreResourceGroupManager.h>

#include "XmlHelper.h"
#include "OgreXercesInput.h"

#include "DsaManager.h"
#include "Talent.h"
#include "Person.h"
#include "Kampftechnik.h"
#include "RulesPrerequisites.h"
#include "RulesSubsystem.h"

#include "Exception.h"

using namespace XERCES_CPP_NAMESPACE;
using namespace Ogre;

namespace rl {

	using XERCES_CPP_NAMESPACE::DOMDocument; //XXX: Warum brauche ich das unter VS 2003?

    XdimlLoader::XdimlLoader()
        : ScriptLoader()
    {
		mScriptPatterns.push_back("*.xdi");
		mScriptPatterns.push_back("*.xdiml");
		ResourceGroupManager::getSingleton()._registerScriptLoader(this);
    }

    void XdimlLoader::parseScript(Ogre::DataStreamPtr &stream, const Ogre::String &groupName)
    {
		XMLPlatformUtils::Initialize();

		XmlHelper::initializeTranscoder();
		XercesDOMParser* parser = new XercesDOMParser();

        parser->setValidationScheme(XercesDOMParser::Val_Auto);    // optional.
        parser->setDoNamespaces(true);    // optional

        OgreInputSource source(stream);
        parser->parse(source);

		DOMDocument* doc = parser->getDocument();
		DOMElement* dataDocumentContent = XmlHelper::getChildNamed(
            doc->getDocumentElement(), "Inhalt");

		initializeTalente(XmlHelper::getChildNamed(dataDocumentContent, "Talente"));
		initializeKampftechniken(XmlHelper::getChildNamed(dataDocumentContent, "Kampftechniken"));
		initializePersonen(XmlHelper::getChildNamed(dataDocumentContent, "Personen"));

		doc->release();

		XMLPlatformUtils::Terminate();
    }

    const StringVector& XdimlLoader::getScriptPatterns() const
    {
        return mScriptPatterns;
    }

    Real XdimlLoader::getLoadingOrder() const
    {
        return 1000.0;
    }

    void XdimlLoader::initializeTalente(DOMElement* rootTalente)
    {
		if (rootTalente == NULL)
			return;

	    DOMNodeList* talentGruppen =
            rootTalente->getElementsByTagName(AutoXMLCh("Talentgruppe").data());
        for (unsigned int gruppe = 0; gruppe < talentGruppen->getLength(); gruppe++)
		{
			DOMElement* gruppeData = static_cast<DOMElement*>(talentGruppen->item(gruppe));
			DOMNodeList* talenteXml =
                gruppeData->getElementsByTagName(AutoXMLCh("Talent").data());
            //int numTalent = 0;
            for (unsigned int talentIdx = 0; talentIdx < talenteXml->getLength(); talentIdx++)
            {
                Talent* t = processTalent(gruppe, static_cast<DOMElement*>(talenteXml->item(talentIdx)));
                //numTalent++;
				DsaManager::getSingleton()._addTalent(t);
            }
		}
    }

    Talent* XdimlLoader::processTalent(int gruppe, DOMElement* talentXml)
    {
		CeGuiString desc = XmlHelper::getValueAsString(XmlHelper::getChildNamed(talentXml, "Beschreibung"));
        CeGuiString probe = XmlHelper::getValueAsString(XmlHelper::getChildNamed(talentXml, "Probe"));
        CeGuiString art = XmlHelper::getValueAsString(XmlHelper::getChildNamed(talentXml, "Art"));
		DOMElement* eBeNode = XmlHelper::getChildNamed(talentXml, "eBE");
		int ebe = EBE_KEINE_BE;
        if (eBeNode != NULL)
			ebe = getEBeFromString(AutoChar(eBeNode->getFirstChild()->getNodeValue()).data());

		CeGuiString name = XmlHelper::transcodeToString(
            talentXml->getAttribute(AutoXMLCh("ID").data()));
        EigenschaftTripel eigenschaften;
		eigenschaften.first = probe.substr(0,2);
        eigenschaften.second = probe.substr(3,2);
        eigenschaften.third = probe.substr(6,2);
		probe.clear();

        Talent* t = new Talent(
			name,
            desc,
            eigenschaften,
            ebe,
            gruppe,
			art);
        // @todo DOMNodeList* ausweichTalenteNode = XmlHelper::getChildNamed(talentXml, "Ausweichtalente");

        return t;
    }

	int XdimlLoader::getEBeFromString(const Ogre::String& eBeString)
	{
		if (eBeString.length() == 0)
			return EBE_KEINE_BE;

		if (!(eBeString.substr(0, 2).compare("BE")) == 0)
			Throw(IllegalArgumentException, "Ungueltige EBE-Angabe.");

		Ogre::String ebe = eBeString.substr(2);
		if (ebe.compare("x2") == 0)
			return EBE_BEx2;
		if (ebe.compare("") == 0)
			return 0;
		return atoi(ebe.c_str());
	}

    void XdimlLoader::initializeKampftechniken(DOMElement* rootKampftechniken)
    {
		if (rootKampftechniken == NULL)
			return;

		DOMNodeList* kampfarten =
            rootKampftechniken->getElementsByTagName(AutoXMLCh("Kampfart").data());
		for (unsigned int art = 0; art < kampfarten->getLength(); art++)
		{
			DOMElement* artData = static_cast<DOMElement*>(kampfarten->item(art));
			DOMNodeList* kampftechnikenXml =
                artData->getElementsByTagName(AutoXMLCh("Kampftechnik").data());
			int numKampftechnik = 0;
			for (unsigned int kampftechnikIdx = 0; kampftechnikIdx < kampftechnikenXml->getLength(); kampftechnikIdx++)
			{
				Kampftechnik* k = processKampftechnik(static_cast<DOMElement*>(kampftechnikenXml->item(kampftechnikIdx)));
				numKampftechnik++;
				DsaManager::getSingleton()._addKampftechnik(k);
			}
		}

	}

	Kampftechnik* XdimlLoader::processKampftechnik(DOMElement* kampftechnikXml)
	{
		CeGuiString desc = XmlHelper::getValueAsString(XmlHelper::getChildNamed(kampftechnikXml, "Beschreibung"));
		CeGuiString art = XmlHelper::getValueAsString(XmlHelper::getChildNamed(kampftechnikXml, "Art"));
		DOMElement* eBeNode = XmlHelper::getChildNamed(kampftechnikXml, "eBE");
		int ebe = EBE_KEINE_BE;
		if (eBeNode != NULL)
			ebe = getEBeFromString(AutoChar(eBeNode->getFirstChild()->getNodeValue()).data());

		CeGuiString name = XmlHelper::transcodeToString(
            kampftechnikXml->getAttribute(AutoXMLCh("ID").data()));

		Kampftechnik* k = new Kampftechnik(
			name,
			desc,
			ebe);

		return k;

	}


	void XdimlLoader::initializePersonen(DOMElement* rootPersons)
	{
		if (rootPersons == NULL)
			return;

		DOMNodeList* personenXml = rootPersons->getElementsByTagName(AutoXMLCh("Person").data());
		for (unsigned int idx = 0; idx < personenXml->getLength(); idx++)
		{
			Person* p =
				processPerson(
					static_cast<DOMElement*>(personenXml->item(idx)));
			DsaManager::getSingleton()._addPerson(p);
		}

	}

	Person* XdimlLoader::processPerson(DOMElement* personXml)
	{
		AutoXMLCh TALENT = "Talent";
		AutoXMLCh ID = "ID";
		AutoXMLCh ABGELEITETER_WERT = "AbgeleiteterWert";
		AutoXMLCh EIGENSCHAFT = "Eigenschaft";

		CeGuiString name =
			XmlHelper::getValueAsString(XmlHelper::getChildNamed(personXml, "Name"));
		CeGuiString desc =
			XmlHelper::getValueAsString(XmlHelper::getChildNamed(personXml, "Beschreibung"));

        //@warning replace this by correct loading process
		Person* rval = new Person(10000);
        rval->setName(name);
        rval->setDescription(desc);

		// Eigenschaften laden
		DOMNodeList* eigensch =
			XmlHelper::getChildNamed(personXml, "Eigenschaften")->
				getElementsByTagName(EIGENSCHAFT.data());
		// Die Eigenschaftsnamen mssen durch ihre Abkrzung ersetzt werden.
		for (unsigned int idx = 0; idx < eigensch->getLength(); idx++)
		{
			DOMElement* eigenschXml = static_cast<DOMElement*>(eigensch->item(idx));
			CeGuiString eigName = XmlHelper::transcodeToString(eigenschXml->getAttribute(ID.data()));
			if (eigName == DsaManager::getSingleton().getEigenschaft(E_MUT)->getName())
				eigName = DsaManager::getSingleton().getEigenschaft(E_MUT)->getNameAbbreviation();
			if (eigName == DsaManager::getSingleton().getEigenschaft(E_KLUGHEIT)->getName())
				eigName = DsaManager::getSingleton().getEigenschaft(E_KLUGHEIT)->getNameAbbreviation();
			if (eigName == DsaManager::getSingleton().getEigenschaft(E_INTUITION)->getName())
				eigName = DsaManager::getSingleton().getEigenschaft(E_INTUITION)->getNameAbbreviation();
			if (eigName == DsaManager::getSingleton().getEigenschaft(E_CHARISMA)->getName())
				eigName = DsaManager::getSingleton().getEigenschaft(E_CHARISMA)->getNameAbbreviation();
			if (eigName == DsaManager::getSingleton().getEigenschaft(E_FINGERFERTIGKEIT)->getName())
				eigName = DsaManager::getSingleton().getEigenschaft(E_FINGERFERTIGKEIT)->getNameAbbreviation();
			if (eigName == DsaManager::getSingleton().getEigenschaft(E_GEWANDTHEIT)->getName())
				eigName = DsaManager::getSingleton().getEigenschaft(E_GEWANDTHEIT)->getNameAbbreviation();
			if (eigName == DsaManager::getSingleton().getEigenschaft(E_KONSTITUTION)->getName())
				eigName = DsaManager::getSingleton().getEigenschaft(E_KONSTITUTION)->getNameAbbreviation();
			if (eigName == DsaManager::getSingleton().getEigenschaft(E_KOERPERKRAFT)->getName())
				eigName = DsaManager::getSingleton().getEigenschaft(E_KOERPERKRAFT)->getNameAbbreviation();
			int wert = XmlHelper::getValueAsInteger(XmlHelper::getChildNamed(eigenschXml, "Wert"));

			rval->setEigenschaft(eigName, wert);
		}

		// Abgeleitete Werte laden
		DOMNodeList* werte =
			XmlHelper::getChildNamed(personXml, "AbgeleiteteWerte")->
				getElementsByTagName(ABGELEITETER_WERT.data());
		for (unsigned int idx = 0; idx < werte->getLength(); idx++)
		{
			DOMElement* wertXml = static_cast<DOMElement*>(werte->item(idx));
			int basis = XmlHelper::getValueAsInteger(XmlHelper::getChildNamed(wertXml, "Basiswert"));
			int wert = XmlHelper::getValueAsInteger(XmlHelper::getChildNamed(wertXml, "Wert"));

			AutoChar wertId = wertXml->getAttribute(ID.data());
			if (strcmp(wertId.data(), "Lebensenergie") == 0)
				rval->setWert(rl::Creature::WERT_MOD_LE, wert - basis);
			else if (strcmp(wertId.data(), "Ausdauer") == 0)
                rval->setWert(rl::Creature::WERT_MOD_AU, wert - basis);
			else if (strcmp(wertId.data(), "AttackeBasis") == 0)
                rval->setWert(rl::Creature::WERT_MOD_AT, wert - basis);
			else if (strcmp(wertId.data(), "ParadeBasis") == 0)
                rval->setWert(rl::Creature::WERT_MOD_PA, wert - basis);
			else if (strcmp(wertId.data(), "FernkampfBasis") == 0)
                rval->setWert(rl::Creature::WERT_MOD_FK, wert - basis);
			else if (strcmp(wertId.data(), "InitiativeBasis") == 0)
                rval->setWert(rl::Creature::WERT_INI, wert - basis);
			else if (strcmp(wertId.data(), "Magieresistenz") == 0)
                rval->setWert(rl::Creature::WERT_MOD_MR, wert - basis);
			else if (strcmp(wertId.data(), "Astralenergie") == 0)
                rval->setWert(rl::Creature::WERT_MOD_AE, wert - basis);
			else if (strcmp(wertId.data(), "Sozialstatus") == 0)
                rval->setWert(rl::Creature::WERT_SOZIALSTATUS, wert);
		}

		// Talente laden
		// Talente, die direkt unter <Person> angeordnet sind,
        // ergeben bereits die zusammengefassten Werte
		DOMNodeList* talente =
			XmlHelper::getChildNamed(personXml, "Talente")->
				getElementsByTagName(TALENT.data());
		for (unsigned int idx = 0; idx < talente->getLength(); idx++)
		{
			DOMElement* talentXml = static_cast<DOMElement*>(talente->item(idx));

			CeGuiString talentName = XmlHelper::transcodeToString(
                talentXml->getAttribute(ID.data()));

			Talent* tal =
				DsaManager::getSingleton().getTalent(talentName);

			rval->addTalent(talentName);
			rval->setTalent(
				talentName,
				XmlHelper::getValueAsInteger(XmlHelper::getChildNamed(talentXml, "Wert")));
		}
		return rval;
	}
}
