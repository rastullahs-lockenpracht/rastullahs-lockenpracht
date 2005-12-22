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
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include "DotSceneLoader.h"
#include "World.h"

#include "PhysicsManager.h"
#include "CoreSubsystem.h"
#include "ConfigurationManager.h"

#include "XmlHelper.h"
#include "XmlResourceManager.h"

#include "Exception.h"

#include <cstdlib>
#include <ctime>

using namespace XERCES_CPP_NAMESPACE;
using namespace std;

namespace rl {
	using XERCES_CPP_NAMESPACE::DOMDocument; //XXX: Warum brauche ich das unter VS 2003?

    DotSceneLoader::DotSceneLoader(const string& filename, const string& resourceGroup)
        : mSceneName(filename),
          mResourceGroup(resourceGroup),
          mSceneManager(CoreSubsystem::getSingleton().getWorld()->getSceneManager())

	{
        srand(static_cast<unsigned int>(time(NULL)));
		initializeScene();
	}

	void DotSceneLoader::initializeScene()
	{
		XMLPlatformUtils::Initialize();

		XmlHelper::initializeTranscoder();

		CoreSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, "Lade Szenenbeschreibung aus " + mSceneName );
		DOMDocument* doc = openSceneFile();
		CoreSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, " Beginne parsen der Unterelemente" );
		DOMElement* nodes = XmlHelper::getChildNamed(doc->getDocumentElement(), "nodes");

		// Eine .scene wird in einem SceneNode mit dem Namen der .scene befestigt
		SceneNode* staticNode = mSceneManager->getRootSceneNode()->createChildSceneNode(mSceneName);
		processNodes( nodes, staticNode );


        if( ConfigurationManager::getSingleton().shouldUseStaticGeometry() )
        {        
		    StaticGeometry* staticGeom = mSceneManager->createStaticGeometry( mSceneName );

		    staticGeom->addSceneNode( staticNode );

		    // Statische Geometrie bauen
		    staticGeom->build();
		    // Nicht mehr den Original-Knoten rendern, da dieser noch erhalten ist.
		    staticNode->setVisible( false );
            CoreSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, " Statische Geometrie erstellt" );
        }
        else
            CoreSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, " Keine statische Geometrie erstellt" );
		

		doc->release();
		XMLPlatformUtils::Terminate();		
		CoreSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, "Szenenbeschreibung aus " + mSceneName +" fertig geparst" );
	}

	DOMDocument* DotSceneLoader::openSceneFile( )
	{

		XercesDOMParser* parser = new XercesDOMParser();
        parser->setValidationScheme(XercesDOMParser::Val_Always);
        parser->setDoNamespaces(true);
		
		XmlPtr res = 
			XmlResourceManager::getSingleton().create(
			mSceneName, 
			mResourceGroup);
		res.getPointer()->parseBy(parser);
		return parser->getDocument();
	}

	// Iteriert durch die einzelnen Nodes
	void DotSceneLoader::processNodes(DOMElement* rootNodesXml, 
		Ogre::SceneNode* parentNode )
	{
		if ( rootNodesXml == NULL )
			return;
		if ( parentNode == NULL )
			Throw( NullPointerException, "parentNode darf nicht null sein" );
	

		// Durch alle Unterelemente iterieren
		DOMNode* child = rootNodesXml->getFirstChild();

		while( child != NULL )
		{
			// Ein Node
			XMLCh* nodeTag = XMLString::transcode("node");
			if( XMLString::compareIString(child->getNodeName(), nodeTag) == 0  )
			{
				processNode( reinterpret_cast<DOMElement*>(child) , parentNode );
			}
			XMLString::release(&nodeTag);
			child = child->getNextSibling();
		} 
	}

	// Befasst sich mit einem Node
	void DotSceneLoader::processNode(DOMElement* rootNodeXml, Ogre::SceneNode* parentNode)
	{
		if ( rootNodeXml == NULL )
			return;
		if ( parentNode == NULL )
			Throw( NullPointerException, "parentNode darf nicht null sein" );

		string nodeName = XmlHelper::getAttributeValueAsStdString( rootNodeXml, 
			"name" );
		
		Ogre::SceneNode* newNode;
		// Wurde dem Node ein Name zugewiesen?
		if( nodeName.length() > 0 )
        {
            try
            {
                // Dann versuche einen Knoten mit dem Namen zu erstellen
                newNode = parentNode->createChildSceneNode(nodeName);
            }
            catch( Ogre::Exception& e )
            {
                // Name schon vergeben?
                if (e.getNumber() == Ogre::Exception::ERR_DUPLICATE_ITEM)
                {
                    newNode = parentNode->createChildSceneNode();
                    CoreSubsystem::getSingleton().log(Ogre::LML_NORMAL, 
                        " NodeName '"+nodeName+"' war schon vergeben! Es wurde der Name '"+newNode->getName()+"' benutzt." );
                }
                else
                {
                    // Andere Exception-Ursache - weiterwerfen.
                    throw e;
                }
            }
        }
		else
        {
            newNode = parentNode->createChildSceneNode();
        }	

		CoreSubsystem::getSingleton().log( Ogre::LML_TRIVIAL, 
            " Node '"+newNode->getName()+"' als Unterknoten von '"+parentNode->getName()+"' erstellt." );		
		
        DOMNode* child = rootNodeXml->getFirstChild();
        NodeUserData userData;
        // Defaults einstellen
        userData.is_static = true;
        userData.physical_body = "mesh";

        // Durch alle Unterelemente iterieren, um die userDatas zu finden
        while( child != NULL )
        {
            // Ein selbstdefinierter Bereich
            if( XMLString::compareIString(child->getNodeName(), 
                XMLString::transcode("userData") ) == 0 )
                processNodeUserData( reinterpret_cast<DOMElement*>(child) , &userData );

            child = child->getNextSibling();
        } 

        // Muss für Meshes in diesem Node ein TriMeshBody erstellt werden?
        bool createMeshPhysicalBody = userData.physical_body.compare("mesh") == 0;

        child = rootNodeXml->getFirstChild();
        // Durch alle Unterelemente iterieren
		while( child != NULL )
		{
			// Ein Node
			if( XMLString::compareIString(child->getNodeName(), 
				XMLString::transcode("node") ) == 0 )
				processNode( reinterpret_cast<DOMElement*>(child) , newNode );
			// Position des Nodes
			else if( XMLString::compareIString(child->getNodeName(), 
				XMLString::transcode("position") ) == 0  )
				newNode->setPosition( processPosition( reinterpret_cast<DOMElement*>(child) ) );
			// Rotation des Nodes
			else if( XMLString::compareIString(child->getNodeName(), 
				XMLString::transcode("rotation") ) == 0  )
				newNode->setOrientation( processRotation( reinterpret_cast<DOMElement*>(child) ) );
			// Skalierung des Nodes
			else if( XMLString::compareIString(child->getNodeName(), 
				XMLString::transcode("scale") ) == 0  )
				newNode->setScale( processScale( reinterpret_cast<DOMElement*>(child) ) );
			// Eine Entity
			else if( XMLString::compareIString(child->getNodeName(), 
				XMLString::transcode("entity") ) == 0  )                
				processEntity( reinterpret_cast<DOMElement*>(child) , newNode,  createMeshPhysicalBody );

			child = child->getNextSibling();
		} 
	}

    void DotSceneLoader::processNodeUserData( XERCES_CPP_NAMESPACE::DOMElement* rootUserDataXml, 
        NodeUserData* userData )
    {
        DOMNode* child = rootUserDataXml->getFirstChild();
        CoreSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, " NodeUserData gefunden");

        // Durch alle Unterelemente iterieren, um die properties zu finden
        while( child != NULL )
        {
            // Ein selbstdefinierter Bereich
            if( XMLString::compareIString(child->getNodeName(), 
                XMLString::transcode("property") ) == 0 )
            {
                DOMElement* propertyXml = reinterpret_cast<DOMElement*>(child);
                string propertyName = XmlHelper::getAttributeValueAsStdString( propertyXml, 
			        "name" );

                try
                {
                    if( propertyName.compare("physical_body") == 0 )
                        userData->physical_body = XmlHelper::getAttributeValueAsStdString( 
                            propertyXml, "data" );
                    else if( propertyName.compare("static") == 0 )
                        userData->is_static = XmlHelper::getAttributeValueAsInteger( 
                        propertyXml, "data" ) != 0;
                }
                catch(...)
                {
                    CoreSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, 
                        " > Parse Error beim Übernehmen der Property '"+propertyName+"'!");
                }

            }

            child = child->getNextSibling();
        } 
    }

	// Eine Entity
	void DotSceneLoader::processEntity( DOMElement* rootEntityXml,
        Ogre::SceneNode* parentNode, bool createMeshPhysicalBody )
	{
		string entName = XmlHelper::getAttributeValueAsStdString( 
			rootEntityXml, "name" );
		string meshName = XmlHelper::getAttributeValueAsStdString( 
			rootEntityXml, "meshFile" );

		Ogre::Entity* newEnt = NULL;

        // Wurde der Entity bisher kein Name zugewiesen
		if( entName.length() == 0 )
        {
            entName = getRandomName(mSceneName+"_"+parentNode->getName());
        }

        bool isEntityCreated = false;
        ResourceGroupManager& resGroupMgr = ResourceGroupManager::getSingleton();
        while(!isEntityCreated)
        {
            // Erschaffen versuchen
            try
            {
                // if this mesh exists in our module's resource group: preload it
                if (resGroupMgr.resourceExists(mResourceGroup, meshName))
                {
                    MeshManager::getSingleton().load(meshName, mResourceGroup);
                }
                // if not, it is now loaded implicitly from the default group
                newEnt = mSceneManager->createEntity(entName, meshName);
                parentNode->attachObject( newEnt );
                isEntityCreated = true;

                CoreSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, " Entity '"+meshName+"' mit dem Namen '"+entName+"' in den Knoten '"+parentNode->getName()+"' eingefügt." );

                // Zur Physik des Levels hinzufügen
                if( createMeshPhysicalBody )
                {                
                    PhysicsManager::getSingleton().addLevelGeometry( newEnt );
                    CoreSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, " Entity '"+entName+"' als TriMesh in levelGeometry geladen");
                }

                
                newEnt->setCastShadows( false );
            }
            catch (Ogre::Exception& e) 
            {
                if (e.getNumber() == Ogre::Exception::ERR_DUPLICATE_ITEM)
                {
                    // Ok, gab es schon. Neuen Namen probieren.
                    entName = getRandomName(entName);
                }
                else
                {
                    CoreSubsystem::getSingleton().log(Ogre::LML_CRITICAL, 
                        " Laden der Entity '"+meshName+"' gescheitert!" );
                    // Nicht weiter versuchen
                    break;
                }
            }
        }
	}

    string DotSceneLoader::getRandomName(const string& baseName)
    {
        int rnd = rand();
        stringstream rval;
        rval << baseName << "_" << rnd;
        return rval.str();
    }

	Ogre::Vector3 DotSceneLoader::processPosition( DOMElement* rootPositionXml )
	{
		CoreSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, " Position gefunden");

		try
		{
			return Ogre::Vector3( 
				XmlHelper::getAttributeValueAsReal( rootPositionXml, "x" ),
				XmlHelper::getAttributeValueAsReal( rootPositionXml, "y" ),
				XmlHelper::getAttributeValueAsReal( rootPositionXml, "z" ) );;
		}
		catch(...) 
        {
            CoreSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, " > Parse Error beim Übernehmen der Position! ");
        }

		return Ogre::Vector3::ZERO;
	}


	Ogre::Vector3 DotSceneLoader::processScale( DOMElement* rootPositionXml )
	{
		CoreSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, " Skalierung gefunden");

		try
		{
			return Ogre::Vector3( 
				XmlHelper::getAttributeValueAsReal( rootPositionXml, "x" ),
				XmlHelper::getAttributeValueAsReal( rootPositionXml, "y" ),
				XmlHelper::getAttributeValueAsReal( rootPositionXml, "z" ) );
		}
        catch(...) 
        {
            CoreSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, " > Parse Error beim Übernehmen der Skalierung! ");
        }

		return Ogre::Vector3::UNIT_SCALE;
	}

	/// @TODO Sollten drei Möglichkeiten sein...
	Ogre::Quaternion DotSceneLoader::processRotation( DOMElement* rootQuatXml )
	{
		CoreSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, " Rotation gefunden");

		// Durch w,x,y,z definiert
		try
		{
			return Ogre::Quaternion( 
				XmlHelper::getAttributeValueAsReal( rootQuatXml, "qw" ),
				XmlHelper::getAttributeValueAsReal( rootQuatXml, "qx" ),
				XmlHelper::getAttributeValueAsReal( rootQuatXml, "qy" ),
				XmlHelper::getAttributeValueAsReal( rootQuatXml, "qz" ) );
		}
		catch(...) {}

		// Durch axisX,axisY,axisZ,angle definiert
		try
		{
			return Ogre::Quaternion( Ogre::Radian( XmlHelper::getAttributeValueAsReal( 
				rootQuatXml, "angle" ) ),
				Ogre::Vector3(
				XmlHelper::getAttributeValueAsReal( rootQuatXml, "axisX" ),
				XmlHelper::getAttributeValueAsReal( rootQuatXml, "axisY" ),
				XmlHelper::getAttributeValueAsReal( rootQuatXml, "axisZ" )) );
		}
		catch(...) {}

		// Durch angleX,angleY,angleZ definiert
		try
		{
            Ogre::Matrix3 mat;
			mat.FromEulerAnglesXYZ(
				Degree(XmlHelper::getAttributeValueAsReal(rootQuatXml, "angleX")),
				Degree(XmlHelper::getAttributeValueAsReal(rootQuatXml, "angleY")),
				Degree(XmlHelper::getAttributeValueAsReal(rootQuatXml, "angleZ")));
            return Quaternion(mat);
		}
		catch(...) {}

        CoreSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, " > Parse Error beim Übernehmen der Rotation! ");

		return Ogre::Quaternion::IDENTITY;
	}

}
