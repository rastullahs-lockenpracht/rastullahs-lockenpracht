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

// #include "XdimlLoader.h"
#include "DsaDataLoader.h"

#include "XmlProcessor.h"

#include "Creature.h"
#include "DsaManager.h"
#include "Kampftechnik.h"
#include "RulesPrerequisites.h"
#include "RulesSubsystem.h"
#include "Talent.h"

#include "Exception.h"

using namespace Ogre;

namespace rl
{

    XdimlLoader::XdimlLoader()
        : ScriptLoader()
    {
        mScriptPatterns.push_back("*.xdi");
        mScriptPatterns.push_back("*.xdiml");
        ResourceGroupManager::getSingleton()._registerScriptLoader(this);
    }

    void XdimlLoader::parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName)
    {
        tinyxml2::XMLDocument* doc = loadDocument(stream);
        tinyxml2::XMLElement* dataDocumentContent = getChildNamed(doc->RootElement(), "Inhalt");

        initializeTalente(getChildNamed(dataDocumentContent, "Talente"));
        initializeKampftechniken(getChildNamed(dataDocumentContent, "Kampftechniken"));
        initializeCreatures(getChildNamed(dataDocumentContent, "Personen"));
    }

    const StringVector& XdimlLoader::getScriptPatterns() const
    {
        return mScriptPatterns;
    }

    Real XdimlLoader::getLoadingOrder() const
    {
        return 1000.0;
    }

    void XdimlLoader::initializeTalente(const tinyxml2::XMLElement* rootTalente)
    {
        if (rootTalente == NULL)
            return;

        XmlElementList talentGruppen = getElementsByTagName(rootTalente, "Talentgruppe");
        for (unsigned int gruppe = 0; gruppe < talentGruppen.size(); gruppe++)
        {
            const tinyxml2::XMLElement* gruppeData = talentGruppen[gruppe];
            XmlElementList talenteXml = getElementsByTagName(gruppeData, "Talent");
            // int numTalent = 0;
            for (unsigned int talentIdx = 0; talentIdx < talenteXml.size(); talentIdx++)
            {
                Talent* t = processTalent(gruppe, talenteXml[talentIdx]);
                // numTalent++;
                LOG_MESSAGE("DsaDataLoader", "Found talent " + t->getName());
                DsaManager::getSingleton()._addTalent(t);
            }
        }
    }

    Talent* XdimlLoader::processTalent(int gruppe, const tinyxml2::XMLElement* talentXml)
    {
        CeGuiString desc = getValueAsString(getChildNamed(talentXml, "Beschreibung"));
        CeGuiString probe = getValueAsString(getChildNamed(talentXml, "Probe"));
        CeGuiString art = getValueAsString(getChildNamed(talentXml, "Art"));
        const tinyxml2::XMLElement* eBeNode = getChildNamed(talentXml, "eBE");
        const tinyxml2::XMLElement* ausweichTalenteNode = getChildNamed(talentXml, "Ausweichtalente");

        int ebe = EBE_KEINE_BE;
        if (eBeNode != NULL)
        {
            ebe = getEBeFromString(eBeNode->FirstChild()->Value());
        }

        CeGuiString name = getAttributeValueAsString(talentXml, "ID");
        EigenschaftTripel eigenschaften;
        eigenschaften.first = probe.substr(0, 2);
        eigenschaften.second = probe.substr(3, 2);
        eigenschaften.third = probe.substr(6, 2);
        probe.clear();

        // process AusweichTalente
        Talent::AusweichTalente ausweichTalente;
        if (ausweichTalenteNode)
        {
            XmlElementList ausweichTalentGruppen = getElementsByTagName(ausweichTalenteNode, "Ausweichtalent");
            for (unsigned int ausweich = 0; ausweich < ausweichTalentGruppen.size(); ausweich++)
            {
                const tinyxml2::XMLElement* ausweichData = ausweichTalentGruppen[ausweich];
                CeGuiString ausweichName = getAttributeValueAsString(ausweichData, "ID");

                ausweichTalente[ausweichName] = getValueAsInteger(getChildNamed(ausweichData, "Aufschlag"));
            }
        }

        Talent* t = new Talent(name, desc, eigenschaften, ebe, gruppe, art, ausweichTalente);

        return t;
    }

    int XdimlLoader::getEBeFromString(const Ogre::String& eBeString)
    {
        if (eBeString.length() == 0)
        {
            return EBE_KEINE_BE;
        }

        if (!(eBeString.substr(0, 2).compare("BE")) == 0)
        {
            Throw(IllegalArgumentException, "Ungueltige EBE-Angabe.");
        }

        Ogre::String ebe = eBeString.substr(2);
        if (ebe.compare("x2") == 0)
        {
            return EBE_BEx2;
        }
        if (ebe.compare("") == 0)
        {
            return 0;
        }
        return atoi(ebe.c_str());
    }

    void XdimlLoader::initializeKampftechniken(const tinyxml2::XMLElement* rootKampftechniken)
    {
        if (rootKampftechniken == NULL)
            return;

        XmlElementList kampfarten = getElementsByTagName(rootKampftechniken, "Kampfart");
        for (unsigned int art = 0; art < kampfarten.size(); art++)
        {
            const tinyxml2::XMLElement* artData = kampfarten[art];
            XmlElementList kampftechnikenXml = getElementsByTagName(artData, "Kampftechnik");
            int numKampftechnik = 0;
            for (unsigned int kampftechnikIdx = 0; kampftechnikIdx < kampftechnikenXml.size(); kampftechnikIdx++)
            {
                Kampftechnik* k = processKampftechnik(kampftechnikenXml[kampftechnikIdx]);
                numKampftechnik++;
                DsaManager::getSingleton()._addKampftechnik(k);
            }
        }
    }

    Kampftechnik* XdimlLoader::processKampftechnik(const tinyxml2::XMLElement* kampftechnikXml)
    {
        CeGuiString desc = getValueAsString(getChildNamed(kampftechnikXml, "Beschreibung"));
        CeGuiString art = getValueAsString(getChildNamed(kampftechnikXml, "Art"));
        const tinyxml2::XMLElement* eBeNode = getChildNamed(kampftechnikXml, "eBE");
        int ebe = EBE_KEINE_BE;
        if (eBeNode != NULL)
        {
            ebe = getEBeFromString(getValueAsStdString(eBeNode));
        }

        CeGuiString name = getAttributeValueAsString(kampftechnikXml, "ID");

        Kampftechnik* k = new Kampftechnik(name, desc, ebe);
        return k;
    }

    void XdimlLoader::initializeCreatures(const tinyxml2::XMLElement* rootPersons)
    {
        if (rootPersons == NULL)
            return;

        XmlElementList personenXml = getElementsByTagName(rootPersons, "Person");
        for (unsigned int idx = 0; idx < personenXml.size(); idx++)
        {
            Creature* p = processCreature(personenXml[idx]);
            DsaManager::getSingleton()._addCreature(p);
        }
    }

    Creature* XdimlLoader::processCreature(const tinyxml2::XMLElement* personXml)
    {
        CeGuiString name = getValueAsString(getChildNamed(personXml, "Name"));
        CeGuiString desc = getValueAsString(getChildNamed(personXml, "Beschreibung"));

        //@warning replace this by correct loading process
        Creature* rval = new Creature(10000);
        rval->setName(name);
        rval->setDescription(desc);

        // Eigenschaften laden
        XmlElementList eigensch = getElementsByTagName(getChildNamed(personXml, "Eigenschaften"), "Eigenschaft");
        // Die Eigenschaftsnamen mssen durch ihre Abkrzung ersetzt werden.
        for (unsigned int idx = 0; idx < eigensch.size(); idx++)
        {
            const tinyxml2::XMLElement* eigenschXml = eigensch[idx];
            CeGuiString eigName = getAttributeValueAsString(eigenschXml, "ID");
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
        XmlElementList werte = getElementsByTagName(getChildNamed(personXml, "AbgeleiteteWerte"), "AbgeleiteterWert");
        for (unsigned int idx = 0; idx < werte.size(); idx++)
        {
            const tinyxml2::XMLElement* wertXml = werte[idx];
            int basis = getValueAsInteger(getChildNamed(wertXml, "Basiswert"));
            int wert = getValueAsInteger(getChildNamed(wertXml, "Wert"));

            std::string wertId = getAttributeValueAsStdString(wertXml, "ID");
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
        XmlElementList talente = getElementsByTagName(getChildNamed(personXml, "Talente"), "Talent");
        for (unsigned int idx = 0; idx < talente.size(); idx++)
        {
            const tinyxml2::XMLElement* talentXml = talente[idx];

            CeGuiString talentName = getAttributeValueAsString(talentXml, "ID");

            Talent* tal = DsaManager::getSingleton().getTalent(talentName);

            rval->addTalent(talentName);
            rval->setTalent(talentName, getValueAsInteger(getChildNamed(talentXml, "Wert")));
        }
        return rval;
    }
}
