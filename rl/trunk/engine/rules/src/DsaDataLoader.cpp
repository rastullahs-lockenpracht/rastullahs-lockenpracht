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
#include "stdinc.h" //precompiled header

#include <xercesc/dom/DOM.hpp>

// #include "XdimlLoader.h"
#include "DsaDataLoader.h"


#include "XmlProcessor.h"
#include "OgreXercesInput.h"

#include "DsaManager.h"
#include "Talent.h"
#include "Creature.h"
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
        initializeXml();

		DOMDocument* doc = loadDocument(stream);
		DOMElement* dataDocumentContent = getChildNamed(
            doc->getDocumentElement(), "Inhalt");

		initializeTalente(getChildNamed(dataDocumentContent, "Talente"));
		initializeKampftechniken(getChildNamed(dataDocumentContent, "Kampftechniken"));
		initializeCreatures(getChildNamed(dataDocumentContent, "Personen"));

		doc->release();

        shutdownXml();
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
		CeGuiString desc = getValueAsString(getChildNamed(talentXml, "Beschreibung"));
        CeGuiString probe = getValueAsString(getChildNamed(talentXml, "Probe"));
        CeGuiString art = getValueAsString(getChildNamed(talentXml, "Art"));
		DOMElement* eBeNode = getChildNamed(talentXml, "eBE");
        DOMElement* ausweichTalenteNode = static_cast<DOMElement*>(getChildNamed(talentXml, "Ausweichtalente"));

		int ebe = EBE_KEINE_BE;
        if (eBeNode != NULL)
			ebe = getEBeFromString(AutoChar(eBeNode->getFirstChild()->getNodeValue()).data());

		CeGuiString name = transcodeToString(
            talentXml->getAttribute(AutoXMLCh("ID").data()));
        EigenschaftTripel eigenschaften;
		eigenschaften.first = probe.substr(0,2);
        eigenschaften.second = probe.substr(3,2);
        eigenschaften.third = probe.substr(6,2);
		probe.clear();

        // process AusweichTalente
        Talent::AusweichTalente ausweichTalente;
        if( ausweichTalenteNode )
        {
	        DOMNodeList* ausweichTalentGruppen =
                ausweichTalenteNode->getElementsByTagName(AutoXMLCh("Ausweichtalent").data());
            for( unsigned int ausweich = 0; ausweich < ausweichTalentGruppen->getLength(); ausweich++ )
            {
			    DOMElement* ausweichData = static_cast<DOMElement*>(ausweichTalentGruppen->item(ausweich));
	            CeGuiString ausweichName = transcodeToString(
                    ausweichData->getAttribute(AutoXMLCh("ID").data()));

                ausweichTalente[ausweichName] = 
                    getValueAsInteger(getChildNamed(ausweichData, "Aufschlag"));
            }
        }

        Talent* t = new Talent(
			name,
            desc,
            eigenschaften,
            ebe,
            gruppe,
			art,
            ausweichTalente);

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
		CeGuiString desc = getValueAsString(getChildNamed(kampftechnikXml, "Beschreibung"));
		CeGuiString art = getValueAsString(getChildNamed(kampftechnikXml, "Art"));
		DOMElement* eBeNode = getChildNamed(kampftechnikXml, "eBE");
		int ebe = EBE_KEINE_BE;
		if (eBeNode != NULL)
			ebe = getEBeFromString(AutoChar(eBeNode->getFirstChild()->getNodeValue()).data());

		CeGuiString name = transcodeToString(
            kampftechnikXml->getAttribute(AutoXMLCh("ID").data()));

		Kampftechnik* k = new Kampftechnik(
			name,
			desc,
			ebe);

		return k;

	}


	void XdimlLoader::initializeCreatures(DOMElement* rootPersons)
	{
		if (rootPersons == NULL)
			return;

		DOMNodeList* personenXml = rootPersons->getElementsByTagName(AutoXMLCh("Person").data());
		for (unsigned int idx = 0; idx < personenXml->getLength(); idx++)
		{
			Creature* p =
				processCreature(
					static_cast<DOMElement*>(personenXml->item(idx)));
			DsaManager::getSingleton()._addCreature(p);
		}

	}

	Creature* XdimlLoader::processCreature(DOMElement* personXml)
	{
		AutoXMLCh TALENT = "Talent";
		AutoXMLCh ID = "ID";
		AutoXMLCh ABGELEITETER_WERT = "AbgeleiteterWert";
		AutoXMLCh EIGENSCHAFT = "Eigenschaft";

		CeGuiString name =
			getValueAsString(getChildNamed(personXml, "Name"));
		CeGuiString desc =
			getValueAsString(getChildNamed(personXml, "Beschreibung"));

        //@warning replace this by correct loading process
		Creature* rval = new Creature(10000);
        rval->setName(name);
        rval->setDescription(desc);

		// Eigenschaften laden
		DOMNodeList* eigensch =
			getChildNamed(personXml, "Eigenschaften")->
				getElementsByTagName(EIGENSCHAFT.data());
		// Die Eigenschaftsnamen mssen durch ihre Abkrzung ersetzt werden.
		for (unsigned int idx = 0; idx < eigensch->getLength(); idx++)
		{
			DOMElement* eigenschXml = static_cast<DOMElement*>(eigensch->item(idx));
			CeGuiString eigName = transcodeToString(eigenschXml->getAttribute(ID.data()));
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
			int wert = getValueAsInteger(getChildNamed(eigenschXml, "Wert"));

			rval->setEigenschaft(eigName, wert);
		}

		// Abgeleitete Werte laden
		DOMNodeList* werte =
			getChildNamed(personXml, "AbgeleiteteWerte")->
				getElementsByTagName(ABGELEITETER_WERT.data());
		for (unsigned int idx = 0; idx < werte->getLength(); idx++)
		{
			DOMElement* wertXml = static_cast<DOMElement*>(werte->item(idx));
			int basis = getValueAsInteger(getChildNamed(wertXml, "Basiswert"));
			int wert = getValueAsInteger(getChildNamed(wertXml, "Wert"));

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
			getChildNamed(personXml, "Talente")->
				getElementsByTagName(TALENT.data());
		for (unsigned int idx = 0; idx < talente->getLength(); idx++)
		{
			DOMElement* talentXml = static_cast<DOMElement*>(talente->item(idx));

			CeGuiString talentName = transcodeToString(
                talentXml->getAttribute(ID.data()));

			Talent* tal =
				DsaManager::getSingleton().getTalent(talentName);

			rval->addTalent(talentName);
			rval->setTalent(
				talentName,
				getValueAsInteger(getChildNamed(talentXml, "Wert")));
		}
		return rval;
	}
}
