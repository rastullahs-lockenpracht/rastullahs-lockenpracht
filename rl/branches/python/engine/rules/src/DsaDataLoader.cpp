/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2005 Team Pantheon. http://www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */

#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include "DsaDataLoader.h"

#include "XmlHelper.h"
#include "XmlResourceManager.h"

#include "DsaManager.h"
#include "Talent.h"
#include "Person.h"
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

		DOMDocument* doc = loadDataFile(filename);
		DOMElement* dataDocumentContent = XmlHelper::getChildNamed(doc->getDocumentElement(), "Inhalt");

		initializeTalente(XmlHelper::getChildNamed(dataDocumentContent, "Talente"));
		initializeKampftechniken(XmlHelper::getChildNamed(dataDocumentContent, "Kampftechniken"));
		initializePersonen(XmlHelper::getChildNamed(dataDocumentContent, "Personen"));
		
		doc->release();
		XMLPlatformUtils::Terminate();
		
	}

	DOMDocument* DsaDataLoader::loadDataFile(string filename)
	{

		XercesDOMParser* parser = new XercesDOMParser();
        parser->setValidationScheme(XercesDOMParser::Val_Always);    // optional.
        parser->setDoNamespaces(true);    // optional

/*        ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
        parser->setErrorHandler(errHandler);*/
		
		XmlPtr res = 
			XmlResourceManager::getSingleton().create(
			filename, 
			ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		res.getPointer()->parseBy(parser);
		return parser->getDocument();
	}

    void DsaDataLoader::initializeTalente(DOMElement* rootTalente)
    {
		if (rootTalente == NULL)
			return;

	    DOMNodeList* talentGruppen = rootTalente->getElementsByTagName(XMLString::transcode("Talentgruppe"));
        for (unsigned int gruppe = 0; gruppe < talentGruppen->getLength(); gruppe++)
		{
			DOMElement* gruppeData = reinterpret_cast<DOMElement*>(talentGruppen->item(gruppe));
			DOMNodeList* talenteXml = gruppeData->getElementsByTagName(XMLString::transcode("Talent"));
            int numTalent = 0;
            for (unsigned int talentIdx = 0; talentIdx < talenteXml->getLength(); talentIdx++)
            {
                Talent* t = processTalent(gruppe*100+numTalent, gruppe, reinterpret_cast<DOMElement*>(talenteXml->item(talentIdx)));
                numTalent++;
				DsaManager::getSingleton()._addTalent(t);
            }
		}
    }

    Talent* DsaDataLoader::processTalent(int id, int gruppe, DOMElement* talentXml)
    {
		CeGuiString desc = XmlHelper::getValueAsUtf8(XmlHelper::getChildNamed(talentXml, "Beschreibung"));
        CeGuiString probe = XmlHelper::getValueAsUtf8(XmlHelper::getChildNamed(talentXml, "Probe"));
        CeGuiString art = XmlHelper::getValueAsUtf8(XmlHelper::getChildNamed(talentXml, "Art"));
		DOMElement* eBeNode = XmlHelper::getChildNamed(talentXml, "eBE");
		int ebe = EBE_KEINE_BE;
        if (eBeNode != NULL)
			ebe = getEBeFromString(XMLString::transcode(eBeNode->getFirstChild()->getNodeValue()));

		CeGuiString name = XmlHelper::transcodeToUtf8(talentXml->getAttribute(XMLString::transcode("ID")));
        EigenschaftTripel eigenschaften;
		eigenschaften.first = DsaManager::getSingleton().getEigenschaftIdFromString(probe.substr(0,2));
        eigenschaften.second = DsaManager::getSingleton().getEigenschaftIdFromString(probe.substr(3,2));
        eigenschaften.third = DsaManager::getSingleton().getEigenschaftIdFromString(probe.substr(6,2));
		probe.clear();

        Talent* t = new Talent(
            id, 
			name,
            desc,
            eigenschaften,
            ebe,
            gruppe);

        return t;
    }

	int DsaDataLoader::getEBeFromString(const string& eBeString)
	{
		if (eBeString.length() == 0)
			return EBE_KEINE_BE;

		if (!(eBeString.substr(0, 2).compare("BE")) == 0)
			Throw(InvalidArgumentException, "Ungueltige EBE-Angabe.");

		string ebe = eBeString.substr(2);
		if (ebe.compare("x2") == 0)
			return EBE_BEx2;
		if (ebe.compare("") == 0)
			return 0;
		return atoi(ebe.c_str());
	}

    void DsaDataLoader::initializeKampftechniken(DOMElement* rootKampftechniken)
    {
	}

	void DsaDataLoader::initializePersonen(DOMElement* rootPersons)
	{
		if (rootPersons == NULL)
			return;

		DOMNodeList* personenXml = 
			rootPersons->getElementsByTagName(XMLString::transcode("Person"));
		for (unsigned int idx = 0; idx < personenXml->getLength(); idx++)
		{
			Person* p = 
				processPerson(
					idx+10000, 
					reinterpret_cast<DOMElement*>(personenXml->item(idx)));
			DsaManager::getSingleton()._addPerson(p);
		}
		
	}

	Person* DsaDataLoader::processPerson(int id, DOMElement* personXml)
	{
		XMLCh* TALENT = XMLString::transcode("Talent");
		XMLCh* ID = XMLString::transcode("ID");
		XMLCh* ABGELEITETER_WERT = XMLString::transcode("AbgeleiteterWert");
		XMLCh* EIGENSCHAFT = XMLString::transcode("Eigenschaft");
		
		CeGuiString name = 
			XmlHelper::getValueAsUtf8(XmlHelper::getChildNamed(personXml, "Name"));
		CeGuiString desc = 
			XmlHelper::getValueAsUtf8(XmlHelper::getChildNamed(personXml, "Beschreibung"));

		Person* rval = new Person(id, name, desc);

		// Eigenschaften laden
		DOMNodeList* eigensch = 
			XmlHelper::getChildNamed(personXml, "Eigenschaften")->
				getElementsByTagName(EIGENSCHAFT);
		for (unsigned int idx = 0; idx < eigensch->getLength(); idx++)
		{
			DOMElement* eigenschXml = reinterpret_cast<DOMElement*>(eigensch->item(idx));
			utf8* eigName = XmlHelper::transcodeToUtf8(eigenschXml->getAttribute(ID));
			
			int eigId = DsaManager::getSingleton().getEigenschaftIdFromLongString(eigName);
			delete[] eigName;
			int wert = XmlHelper::getValueAsInteger(XmlHelper::getChildNamed(eigenschXml, "Wert"));

			rval->setEigenschaft(eigId, wert);
		}		

		// Abgeleitete Werte laden
		DOMNodeList* werte = 
			XmlHelper::getChildNamed(personXml, "AbgeleiteteWerte")->
				getElementsByTagName(ABGELEITETER_WERT);
		for (unsigned int idx = 0; idx < werte->getLength(); idx++)
		{
			DOMElement* wertXml = reinterpret_cast<DOMElement*>(werte->item(idx));
			int basis = XmlHelper::getValueAsInteger(XmlHelper::getChildNamed(wertXml, "Basiswert"));
			int wert = XmlHelper::getValueAsInteger(XmlHelper::getChildNamed(wertXml, "Wert"));
			
			char* wertId = XMLString::transcode(wertXml->getAttribute(ID));
			if (strcmp(wertId, "Lebensenergie") == 0)
				rval->setWert(WERT_MOD_LE, wert - basis);
			else if (strcmp(wertId, "Ausdauer") == 0)
				rval->setWert(WERT_MOD_AU, wert - basis);
			else if (strcmp(wertId, "AttackeBasis") == 0)
				rval->setWert(WERT_MOD_AT, wert - basis);
			else if (strcmp(wertId, "ParadeBasis") == 0)
				rval->setWert(WERT_MOD_PA, wert - basis);
			else if (strcmp(wertId, "FernkampfBasis") == 0)
				rval->setWert(WERT_MOD_FK, wert - basis);
			else if (strcmp(wertId, "InitiativeBasis") == 0)
				rval->setWert(WERT_MOD_INI, wert - basis);
			else if (strcmp(wertId, "Magieresistenz") == 0)
				rval->setWert(WERT_MOD_MR, wert - basis);
			else if (strcmp(wertId, "Astralenergie") == 0)
				rval->setWert(WERT_MOD_AE, wert - basis);
			else if (strcmp(wertId, "Sozialstatus") == 0)
				rval->setWert(WERT_SOZIALSTATUS, wert);

			XMLString::release(&wertId);
		}

		// Talente laden
		// Talente, die direkt unter <Person> angeordnet sind, ergeben bereits die zusammengefassten Werte
		DOMNodeList* talente =			
			XmlHelper::getChildNamed(personXml, "Talente")->
				getElementsByTagName(TALENT);
		for (unsigned int idx = 0; idx < talente->getLength(); idx++)
		{
			DOMElement* talentXml = reinterpret_cast<DOMElement*>(talente->item(idx));
			
			utf8* talName = XmlHelper::transcodeToUtf8(talentXml->getAttribute(ID));
			Talent* tal = 
				DsaManager::getSingleton().getTalent(talName);
			delete[] talName;

			rval->addTalent(tal->getId());
			rval->setTalent(
				tal->getId(), 
				XmlHelper::getValueAsInteger(XmlHelper::getChildNamed(talentXml, "Wert")));
		}

		XMLString::release(&TALENT);
		XMLString::release(&ID);
		XMLString::release(&ABGELEITETER_WERT);
		XMLString::release(&EIGENSCHAFT);

		return rval;
	}
}
