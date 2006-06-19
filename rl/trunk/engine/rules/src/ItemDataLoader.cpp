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

#include "ItemDataLoader.h"

#include "XmlHelper.h"
#include "XmlResourceManager.h"

#include "ItemManager.h"
#include "Item.h"
#include "Armor.h"
#include "Weapon.h"

#include "Exception.h"

using namespace XERCES_CPP_NAMESPACE;
using namespace std;

namespace rl {

	using XERCES_CPP_NAMESPACE::DOMDocument; //XXX: Warum brauche ich das unter VS 2003?

	void ItemDataLoader::loadData(string filename)
	{
		XMLPlatformUtils::Initialize();

		XmlHelper::initializeTranscoder();
		XercesDOMParser* parser = new XercesDOMParser();

        parser->setValidationScheme(XercesDOMParser::Val_Always);    // optional.
        parser->setDoNamespaces(true);    // optional
		
		XmlPtr res = XmlResourceManager::getSingleton().create(filename, 
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		res->parseBy(parser);

		DOMDocument* doc = parser->getDocument();
		DOMElement* dataDocumentContent = XmlHelper::getChildNamed(
            doc->getDocumentElement(), "Inhalt");

		initializeWeapons(XmlHelper::getChildNamed(dataDocumentContent, "Waffen"));
		//initializeKampftechniken(XmlHelper::getChildNamed(dataDocumentContent, "Kampftechniken"));
		//initializePersonen(XmlHelper::getChildNamed(dataDocumentContent, "Personen"));
		
		doc->release();

        res.setNull();
        XmlResourceManager::getSingleton().remove(filename);

		XMLPlatformUtils::Terminate();
	}

    void ItemDataLoader::initializeWeapons(DOMElement* rootWeapons)
    {
		if (rootWeapons == NULL)
			return;

	    DOMNodeList* weaponGroups =
            rootWeapons->getElementsByTagName(AutoXMLCh("Waffengruppe").data());
        for (unsigned int group = 0; group < weaponGroups->getLength(); group++)
		{
			DOMElement* groupData = reinterpret_cast<DOMElement*>(weaponGroups->item(group));
			DOMNodeList* weaponsXml =
                groupData->getElementsByTagName(AutoXMLCh("Waffe").data());
            //int numTalent = 0;
            for (unsigned int weaponIdx = 0; weaponIdx < weaponsXml->getLength(); weaponIdx++)
            {
                Weapon* w = processWeapon(group, reinterpret_cast<DOMElement*>(weaponsXml->item(weaponIdx)));
                //numTalent++;
				ItemManager::getSingleton()._addItem(w);
            }
		}
    }

    Weapon* ItemDataLoader::processWeapon(int group, DOMElement* weaponXml)
    {
		// Itemname
		CeGuiString name = XmlHelper::getAttributeValueAsString(weaponXml,"Name");
		// Beschreibung
		CeGuiString desc = XmlHelper::getAttributeValueAsString(weaponXml,"Beschreibung");
		// Eindeutiger Zuordner
		CeGuiString id = XmlHelper::getAttributeValueAsString(weaponXml,"ID");

		// Image fürs Inventar
		CeGuiString imageName = XmlHelper::getValueAsUtf8(XmlHelper::getChildNamed(weaponXml, "Bildname"));

		// Größe im Inventar
		int size_x = XmlHelper::getAttributeValueAsInteger(XmlHelper::getChildNamed(weaponXml, "Größe"),"X");
		int size_y = XmlHelper::getAttributeValueAsInteger(XmlHelper::getChildNamed(weaponXml, "Größe"),"Y");

		// Schaden der Waffe in Anzahl W6, W20 und Modifikator
		DOMElement* tpNode = XmlHelper::getChildNamed(weaponXml, "TP");
		int w6 = XmlHelper::getAttributeValueAsInteger(tpNode, "W6");
		int w20 = XmlHelper::getAttributeValueAsInteger(tpNode, "W20");
		int boni = XmlHelper::getAttributeValueAsInteger(tpNode, "Boni");

		// TP / KK
		int tp = XmlHelper::getAttributeValueAsInteger(XmlHelper::getChildNamed(weaponXml, "TpKk"), "Tp");
		int kk = XmlHelper::getAttributeValueAsInteger(XmlHelper::getChildNamed(weaponXml, "TpKk"), "Kk");

		// Bruchfaktor
		int bf = XmlHelper::getValueAsInteger(XmlHelper::getChildNamed(weaponXml, "Bf"));
		//Initiative bonus
		int ini = XmlHelper::getValueAsInteger(XmlHelper::getChildNamed(weaponXml, "Ini"));

		// Distanzklasse
		int dK = Weapon::DK_N;
		DOMElement* dkNode = XmlHelper::getChildNamed(weaponXml, "Dk");
		if (dkNode != NULL)
			dK = getDKFromString(AutoChar(dkNode->getFirstChild()->getNodeValue()).data());
		
		CeGuiString talent = XmlHelper::getValueAsString(XmlHelper::getChildNamed(weaponXml, "Talent"));

		CeGuiString mesh = XmlHelper::getValueAsString(XmlHelper::getChildNamed(weaponXml, "Mesh"));

		int length = XmlHelper::getValueAsInteger(XmlHelper::getChildNamed(weaponXml, "Länge"));

		int attackMod = XmlHelper::getAttributeValueAsInteger(XmlHelper::getChildNamed(weaponXml, "WM"), "Attacke");
		int paradeMod = XmlHelper::getAttributeValueAsInteger(XmlHelper::getChildNamed(weaponXml, "WM"), "Parade");

		Weapon* w = new Weapon(
			name,
			desc
			);
		w->setImageName(imageName);
		w->setMeshName(mesh);
		w->setItemType(Item::ITEMTYPE_WEAPON);
		w->setSize(size_x,size_y);
		w->setTp(w6, w20, boni);
		w->setTpKk(tp, kk);
		w->setBf(bf);
		w->setIni(ini);
		w->setDk(static_cast<Weapon::Distanzklasse>(dK));
		w->setKampftechnik(talent);
        return w;
    }

	int ItemDataLoader::getDKFromString(const string& dkString)
	{
		if (dkString.length() == 0)
			return Weapon::DK_N;

		if (dkString.compare("DK_N") == 0)
			return Weapon::DK_N;
		if (dkString.compare("DK_H") == 0)
			return Weapon::DK_P;
		if (dkString.compare("DK_P") == 0)
			return Weapon::DK_P;
		if (dkString.compare("DK_S") == 0)
			return Weapon::DK_S;

		Throw(IllegalArgumentException, "Ungueltige Distanzklassenangabe");
	}

/*    void DsaDataLoader::initializeKampftechniken(DOMElement* rootKampftechniken)
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
		CeGuiString desc = XmlHelper::getValueAsUtf8(XmlHelper::getChildNamed(kampftechnikXml, "Beschreibung"));
		CeGuiString art = XmlHelper::getValueAsUtf8(XmlHelper::getChildNamed(kampftechnikXml, "Art"));
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
			XmlHelper::getValueAsUtf8(XmlHelper::getChildNamed(personXml, "Name"));
		CeGuiString desc = 
			XmlHelper::getValueAsUtf8(XmlHelper::getChildNamed(personXml, "Beschreibung"));

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
				rval->setWert(WERT_MOD_LE, wert - basis);
			else if (strcmp(wertId.data(), "Ausdauer") == 0)
				rval->setWert(WERT_MOD_AU, wert - basis);
			else if (strcmp(wertId.data(), "AttackeBasis") == 0)
				rval->setWert(WERT_MOD_AT, wert - basis);
			else if (strcmp(wertId.data(), "ParadeBasis") == 0)
				rval->setWert(WERT_MOD_PA, wert - basis);
			else if (strcmp(wertId.data(), "FernkampfBasis") == 0)
				rval->setWert(WERT_MOD_FK, wert - basis);
			else if (strcmp(wertId.data(), "InitiativeBasis") == 0)
				rval->setWert(WERT_MOD_INI, wert - basis);
			else if (strcmp(wertId.data(), "Magieresistenz") == 0)
				rval->setWert(WERT_MOD_MR, wert - basis);
			else if (strcmp(wertId.data(), "Astralenergie") == 0)
				rval->setWert(WERT_MOD_AE, wert - basis);
			else if (strcmp(wertId.data(), "Sozialstatus") == 0)
				rval->setWert(WERT_SOZIALSTATUS, wert);
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

	*/
}
