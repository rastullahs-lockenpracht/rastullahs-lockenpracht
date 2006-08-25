/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
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

#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include "DsaDataLoader.h"

#include "XmlHelper.h"
#include "XmlResourceManager.h"

#include "DsaManager.h"
#include "Talent.h"
#include "Person.h"
#include "Kampftechnik.h"
#include "RulesSubsystem.h"

#include "Exception.h"

using namespace XERCES_CPP_NAMESPACE;
using namespace std;

namespace rl {

	using XERCES_CPP_NAMESPACE::DOMDocument; //XXX: Warum brauche ich das unter VS 2003?

	void DsaDataLoader::loadData(string filename)
	{
		XMLPlatformUtils::Initialize();

		XmlHelper::initializeTranscoder();
		XercesDOMParser* parser = new XercesDOMParser();

        parser->setValidationScheme(XercesDOMParser::Val_Auto);    // optional.
        parser->setDoNamespaces(true);    // optional
		
		XmlPtr res = XmlResourceManager::getSingleton().create(filename, 
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		res->parseBy(parser);

		DOMDocument* doc = parser->getDocument();
		DOMElement* dataDocumentContent = XmlHelper::getChildNamed(
            doc->getDocumentElement(), "Inhalt");

		initializeTalente(XmlHelper::getChildNamed(dataDocumentContent, "Talente"));
		initializeKampftechniken(XmlHelper::getChildNamed(dataDocumentContent, "Kampftechniken"));
		initializePersonen(XmlHelper::getChildNamed(dataDocumentContent, "Personen"));
		
		doc->release();

        res.setNull();
        XmlResourceManager::getSingleton().remove(filename);

		XMLPlatformUtils::Terminate();
	}

    void DsaDataLoader::initializeTalente(DOMElement* rootTalente)
    {
		if (rootTalente == NULL)
			return;

	    DOMNodeList* talentGruppen =
            rootTalente->getElementsByTagName(AutoXMLCh("Talentgruppe").data());
        for (unsigned int gruppe = 0; gruppe < talentGruppen->getLength(); gruppe++)
		{
			DOMElement* gruppeData = reinterpret_cast<DOMElement*>(talentGruppen->item(gruppe));
			DOMNodeList* talenteXml =
                gruppeData->getElementsByTagName(AutoXMLCh("Talent").data());
            //int numTalent = 0;
            for (unsigned int talentIdx = 0; talentIdx < talenteXml->getLength(); talentIdx++)
            {
                Talent* t = processTalent(gruppe, reinterpret_cast<DOMElement*>(talenteXml->item(talentIdx)));
                //numTalent++;
				DsaManager::getSingleton()._addTalent(t);
            }
		}
    }

    Talent* DsaDataLoader::processTalent(int gruppe, DOMElement* talentXml)
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

	int DsaDataLoader::getEBeFromString(const string& eBeString)
	{
		if (eBeString.length() == 0)
			return EBE_KEINE_BE;

		if (!(eBeString.substr(0, 2).compare("BE")) == 0)
			Throw(IllegalArgumentException, "Ungueltige EBE-Angabe.");

		string ebe = eBeString.substr(2);
		if (ebe.compare("x2") == 0)
			return EBE_BEx2;
		if (ebe.compare("") == 0)
			return 0;
		return atoi(ebe.c_str());
	}

    void DsaDataLoader::initializeKampftechniken(DOMElement* rootKampftechniken)
    {
		if (rootKampftechniken == NULL)
			return;

		DOMNodeList* kampfarten =
            rootKampftechniken->getElementsByTagName(AutoXMLCh("Kampfart").data());
		for (unsigned int art = 0; art < kampfarten->getLength(); art++)
		{
			DOMElement* artData = reinterpret_cast<DOMElement*>(kampfarten->item(art));
			DOMNodeList* kampftechnikenXml =
                artData->getElementsByTagName(AutoXMLCh("Kampftechnik").data());
			int numKampftechnik = 0;
			for (unsigned int kampftechnikIdx = 0; kampftechnikIdx < kampftechnikenXml->getLength(); kampftechnikIdx++)
			{
				Kampftechnik* k = processKampftechnik(reinterpret_cast<DOMElement*>(kampftechnikenXml->item(kampftechnikIdx)));
				numKampftechnik++;
				DsaManager::getSingleton()._addKampftechnik(k);
			}
		}

	}

	Kampftechnik* DsaDataLoader::processKampftechnik(DOMElement* kampftechnikXml)
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


	void DsaDataLoader::initializePersonen(DOMElement* rootPersons)
	{
		if (rootPersons == NULL)
			return;

		DOMNodeList* personenXml = rootPersons->getElementsByTagName(AutoXMLCh("Person").data());
		for (unsigned int idx = 0; idx < personenXml->getLength(); idx++)
		{
			Person* p = 
				processPerson(
					reinterpret_cast<DOMElement*>(personenXml->item(idx)));
			DsaManager::getSingleton()._addPerson(p);
		}
		
	}

	Person* DsaDataLoader::processPerson(DOMElement* personXml)
	{
		AutoXMLCh TALENT = "Talent";
		AutoXMLCh ID = "ID";
		AutoXMLCh ABGELEITETER_WERT = "AbgeleiteterWert";
		AutoXMLCh EIGENSCHAFT = "Eigenschaft";
		
		CeGuiString name = 
			XmlHelper::getValueAsString(XmlHelper::getChildNamed(personXml, "Name"));
		CeGuiString desc = 
			XmlHelper::getValueAsString(XmlHelper::getChildNamed(personXml, "Beschreibung"));

		Person* rval = new Person(name, desc);

		// Eigenschaften laden
		DOMNodeList* eigensch = 
			XmlHelper::getChildNamed(personXml, "Eigenschaften")->
				getElementsByTagName(EIGENSCHAFT.data());
		// Die Eigenschaftsnamen müssen durch ihre Abkürzung ersetzt werden.
		for (unsigned int idx = 0; idx < eigensch->getLength(); idx++)
		{
			DOMElement* eigenschXml = reinterpret_cast<DOMElement*>(eigensch->item(idx));
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
			DOMElement* wertXml = reinterpret_cast<DOMElement*>(werte->item(idx));
			int basis = XmlHelper::getValueAsInteger(XmlHelper::getChildNamed(wertXml, "Basiswert"));
			int wert = XmlHelper::getValueAsInteger(XmlHelper::getChildNamed(wertXml, "Wert"));
			
			AutoChar wertId = wertXml->getAttribute(ID.data());
			if (strcmp(wertId.data(), "Lebensenergie") == 0)
                rval->setWert(Creature::WERT_MOD_LE, wert - basis);
			else if (strcmp(wertId.data(), "Ausdauer") == 0)
                rval->setWert(Creature::WERT_MOD_AU, wert - basis);
			else if (strcmp(wertId.data(), "AttackeBasis") == 0)
                rval->setWert(Creature::WERT_MOD_AT, wert - basis);
			else if (strcmp(wertId.data(), "ParadeBasis") == 0)
                rval->setWert(Creature::WERT_MOD_PA, wert - basis);
			else if (strcmp(wertId.data(), "FernkampfBasis") == 0)
                rval->setWert(Creature::WERT_MOD_FK, wert - basis);
			else if (strcmp(wertId.data(), "InitiativeBasis") == 0)
                rval->setWert(Creature::WERT_MOD_INI, wert - basis);
			else if (strcmp(wertId.data(), "Magieresistenz") == 0)
                rval->setWert(Creature::WERT_MOD_MR, wert - basis);
			else if (strcmp(wertId.data(), "Astralenergie") == 0)
                rval->setWert(Creature::WERT_MOD_AE, wert - basis);
			else if (strcmp(wertId.data(), "Sozialstatus") == 0)
                rval->setWert(Creature::WERT_SOZIALSTATUS, wert);
		}

		// Talente laden
		// Talente, die direkt unter <Person> angeordnet sind,
        // ergeben bereits die zusammengefassten Werte
		DOMNodeList* talente =			
			XmlHelper::getChildNamed(personXml, "Talente")->
				getElementsByTagName(TALENT.data());
		for (unsigned int idx = 0; idx < talente->getLength(); idx++)
		{
			DOMElement* talentXml = reinterpret_cast<DOMElement*>(talente->item(idx));
			
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
