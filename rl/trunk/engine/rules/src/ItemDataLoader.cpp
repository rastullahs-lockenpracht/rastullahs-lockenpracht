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

#include <CEGUIString.h>

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
		initializeArmors(XmlHelper::getChildNamed(dataDocumentContent, "Rüstungen"));
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
		int weight = XmlHelper::getValueAsInteger(XmlHelper::getChildNamed(weaponXml, "Gewicht"));

		int attackMod = XmlHelper::getAttributeValueAsInteger(XmlHelper::getChildNamed(weaponXml, "WM"), "Attacke");
		int paradeMod = XmlHelper::getAttributeValueAsInteger(XmlHelper::getChildNamed(weaponXml, "WM"), "Parade");

		// Neuen Waffenprototyp erzeugen und zurückgeben
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
		w->setWeight(weight);
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

    void ItemDataLoader::initializeArmors(DOMElement* rootArmors)
    {
		if (rootArmors == NULL)
			return;

		DOMNodeList* armorGroups =
            rootArmors->getElementsByTagName(AutoXMLCh("Rüstungsgruppe").data());
		for (unsigned int group = 0; group < armorGroups->getLength(); group++)
		{
			DOMElement* groupData = reinterpret_cast<DOMElement*>(armorGroups->item(group));
			DOMNodeList* armorsXml =
                groupData->getElementsByTagName(AutoXMLCh("Rüstung").data());
			//int numKampftechnik = 0;
			for (unsigned int armorIdx = 0; armorIdx < armorsXml->getLength(); armorIdx++)
			{
				Armor* a = processArmor(reinterpret_cast<DOMElement*>(armorsXml->item(armorIdx)));
				//numKampftechnik++;
				ItemManager::getSingleton()._addItem(a);
			}
		}
	}

	Armor* ItemDataLoader::processArmor(DOMElement* armorXml)
	{
		// Itemname
		CeGuiString name = XmlHelper::getAttributeValueAsString(armorXml,"Name");
		// Beschreibung
		CeGuiString desc = XmlHelper::getAttributeValueAsString(armorXml,"Beschreibung");
		// Eindeutiger Zuordner
		CeGuiString id = XmlHelper::getAttributeValueAsString(armorXml,"ID");

		// Image fürs Inventar
		CeGuiString imageName = XmlHelper::getValueAsUtf8(XmlHelper::getChildNamed(armorXml, "Bildname"));
		// Mesh für das Spiel
		DOMElement* meshNode = XmlHelper::getChildNamed(armorXml, "Mesh");
		CeGuiString mesh = "";
		if (meshNode->hasChildNodes())
		{
			mesh = XmlHelper::getValueAsString(meshNode);
		}
		CeGuiString typeString = XmlHelper::getValueAsUtf8(XmlHelper::getChildNamed(armorXml, "Klasse"));
		Item::ItemType type = static_cast<Item::ItemType>(getItemTypeFromString(typeString));

		// Größe im Inventar
		int size_x = XmlHelper::getAttributeValueAsInteger(XmlHelper::getChildNamed(armorXml, "Größe"),"X");
		int size_y = XmlHelper::getAttributeValueAsInteger(XmlHelper::getChildNamed(armorXml, "Größe"),"Y");

		// Rüstungsschutz für bestimmte Zonen
		int ko = XmlHelper::getValueAsInteger(XmlHelper::getChildNamed(armorXml, "Ko"));
		int br = XmlHelper::getValueAsInteger(XmlHelper::getChildNamed(armorXml, "Br"));
		int rue = XmlHelper::getValueAsInteger(XmlHelper::getChildNamed(armorXml, "Rü"));
		int ba = XmlHelper::getValueAsInteger(XmlHelper::getChildNamed(armorXml, "Ba"));
		int la = XmlHelper::getValueAsInteger(XmlHelper::getChildNamed(armorXml, "LA"));
		int ra = XmlHelper::getValueAsInteger(XmlHelper::getChildNamed(armorXml, "RA"));
		int lb = XmlHelper::getValueAsInteger(XmlHelper::getChildNamed(armorXml, "LB"));
		int rb = XmlHelper::getValueAsInteger(XmlHelper::getChildNamed(armorXml, "RB"));
		int ges = XmlHelper::getValueAsInteger(XmlHelper::getChildNamed(armorXml, "Ges."));
		int grs = XmlHelper::getValueAsInteger(XmlHelper::getChildNamed(armorXml, "gRs"));
		int gbe = XmlHelper::getValueAsInteger(XmlHelper::getChildNamed(armorXml, "gBe"));

		int gewicht = XmlHelper::getValueAsInteger(XmlHelper::getChildNamed(armorXml, "Gewicht"));

		int preis = XmlHelper::getValueAsInteger(XmlHelper::getChildNamed(armorXml, "Preis"));

		// Neuen Rüstungsprototyp erzeugen und zurückgeben
		Armor* a = new Armor(
			name,
			desc
			);
		a->setImageName(imageName);
		a->setMeshName(mesh);
		a->setItemType(type);
		a->setSize(size_x,size_y);
		a->setKo(ko);
		a->setBr(br);
		a->setRue(rue);
		a->setBa(ba);
		a->setLA(la);
		a->setRA(ra);
		a->setLB(lb);
		a->setRB(rb);
		a->setGes(ges);
		a->setGRS(grs);
		a->setGBE(gbe);
		// Umrechnung Stein->Unzen = Mal 40
		a->setWeight(gewicht * 40);
		a->setPrice(preis);
        return a;
    }


	int ItemDataLoader::getItemTypeFromString(const CeGuiString& typeString)
	{
		if (typeString.length() == 0)
			return Item::ITEMTYPE_OTHER;

		if (typeString.compare("Rüstung") == 0)
			return Item::ITEMTYPE_ARMOR;
		if (typeString.compare("Waffe") == 0)
			return Item::ITEMTYPE_WEAPON;
		if (typeString.compare("Schild") == 0)
			return Item::ITEMTYPE_SHIELD;
		if (typeString.compare("Ring") == 0)
			return Item::ITEMTYPE_RING;
		if (typeString.compare("Handschuhe") == 0)
			return Item::ITEMTYPE_GLOVES;
		if (typeString.compare("Armreif") == 0)
			return Item::ITEMTYPE_BRACELET;
		if (typeString.compare("Umhang") == 0)
			return Item::ITEMTYPE_CAPE;
		if (typeString.compare("Armschienen") == 0)
			return Item::ITEMTYPE_BRACERS;
		if (typeString.compare("Rucksack") == 0)
			return Item::ITEMTYPE_BACKPACK;
		if (typeString.compare("Gürtel") == 0)
			return Item::ITEMTYPE_BELT;
		if (typeString.compare("Amulett") == 0)
			return Item::ITEMTYPE_NECKLACE;
		if (typeString.compare("Helm") == 0)
			return Item::ITEMTYPE_HELMET;
		if (typeString.compare("Hose") == 0)
			return Item::ITEMTYPE_TROUSERS;
		if (typeString.compare("Beinschienen") == 0)
			return Item::ITEMTYPE_SHINBONE;
		if (typeString.compare("Stiefel") == 0)
			return Item::ITEMTYPE_BOOTS;
		if (typeString.compare("Anderes") == 0)
			return Item::ITEMTYPE_OTHER;
		
		Throw(IllegalArgumentException, "Ungueltige ItemTypangabe: "+string(typeString.c_str()));
	}

/*
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
