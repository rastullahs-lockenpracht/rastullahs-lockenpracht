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

#include "XmlHelper.h"
#include "XmlResourceManager.h"

#include "Exception.h"

using namespace XERCES_CPP_NAMESPACE;
using namespace std;

namespace rl {
	using XERCES_CPP_NAMESPACE::DOMDocument; //XXX: Warum brauche ich das unter VS 2003?

	DotSceneLoader::DotSceneLoader( const std::string &filename )
	{
		mSceneName = filename;
		mSceneManager = CoreSubsystem::getSingleton().getWorld()->getSceneManager();

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


		StaticGeometry* staticGeom = mSceneManager->createStaticGeometry( mSceneName );
		// Usprung und Gr��e der Bl�cke einstellen
		// staticGeom->setRegionDimensions(Vector3(1000,500,1000));
		// staticGeom->setOrigin(Vector3(0,0,0));
		/// FIXME  Diese Methode funktioniert nicht Ogre-Api-korrekt, daher Workaround
		//staticGeom->addSceneNode( staticNode );
		// Alle Entities unterhalb des Nodes einf�gen
		DotSceneLoader::staticGeometryAddSceneNodeWorkaround(
			staticGeom, staticNode);
		// Statische Geometrie bauen
		staticGeom->build();
		// Nicht mehr den Original-Knoten rendern, da dieser erhalten bleibt.
		staticNode->setVisible( false );
		CoreSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, " Statische Geometrie erstellt" );

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
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
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

		string nodeName = XmlHelper::getAttributeValueAsString( rootNodeXml, 
			"name" ).c_str();
		
		Ogre::SceneNode* newNode;
		// Wurde dem Node ein Name zugewiesen
		if( nodeName.length() > 0 )
        {
            try
            {
                mSceneManager->getSceneNode( nodeName );
                // Name war schon vergeben!
                newNode = parentNode->createChildSceneNode();
                CoreSubsystem::getSingleton().log(Ogre::LML_NORMAL, 
                    " NodeName '"+nodeName+"' war schon vergeben! Es wurde der Name '"+newNode->getName()+"' benutzt." );                
            }
            catch( Ogre::Exception )
            {
                // Name war noch nicht vergeben!
                newNode = parentNode->createChildSceneNode(nodeName);
            }
        }
		else
            newNode = parentNode->createChildSceneNode();
			

		CoreSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, " Node '"+newNode->getName()+"' als Unterknoten von '"+parentNode->getName()+"' erstellt." );

		// Durch alle Unterelemente iterieren
		DOMNode* child = rootNodeXml->getFirstChild();

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
				processEntity( reinterpret_cast<DOMElement*>(child) , newNode );

			child = child->getNextSibling();
		} 
	}

	// Eine Entity
	void DotSceneLoader::processEntity( DOMElement* rootEntityXml, Ogre::SceneNode* parentNode )
	{
		string entName = XmlHelper::getAttributeValueAsString( 
			rootEntityXml, "name" ).c_str();
		string meshName = XmlHelper::getAttributeValueAsString( 
			rootEntityXml, "meshFile" ).c_str();

		Ogre::Entity* newEnt = NULL;
		// Wurde der Entity bisher kein Name zugewiesen
		if( entName.length() == 0 )              
			entName = getNextEntityName(mSceneName+"_"+parentNode->getName());
        // �berpr�fung auf Korrektheit des Namens
        else
        {
            try
            {
                mSceneManager->getEntity( entName );
                // Bereits vergebener Name
                string oldName = entName;
                entName = getNextEntityName(mSceneName+"_"+parentNode->getName());
                CoreSubsystem::getSingleton().log(Ogre::LML_NORMAL, 
                    " EntityName '"+oldName+"' war schon vergeben! Es wurde der Name '"+entName+"' benutzt." );
            }
            // Noch nicht gefunden
            catch( Ogre::Exception ) {}
        }

        // Erschaffen versuchen
        try
        {        
		    newEnt = mSceneManager->createEntity(entName, meshName);				
		    parentNode->attachObject( newEnt );

		    CoreSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, " Entity '"+meshName+"' mit dem Namen '"+entName+"' in den Knoten '"+parentNode->getName()+"' eingef�gt." );

		    // Zur Physik des Levels hinzuf�gen
		    PhysicsManager::getSingleton().addLevelGeometry( newEnt );
		    CoreSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, " Entity '"+entName+"' als TriMesh in levelGeometry geladen");

		    newEnt->setCastShadows( false );
	        // newEnt->setCastShadows( XmlHelper::getAttributeValueAsBool( rootEntityXml, XMLString::transcode("castShadows") ) );
		    // XmlHelper::getAttributeValueAsBool( rootEntityXml, XMLString::transcode("static") );
        }
        catch( Ogre::Exception ) 
        {
            CoreSubsystem::getSingleton().log(Ogre::LML_CRITICAL, 
                " Laden der Entity '"+meshName+"' gescheitert!" );
        }
	}

	string DotSceneLoader::getNextEntityName( const string& baseName )
	{
		bool found = true;
		int number = 0;
		string name;
		Ogre::SceneManager* sceneManager = CoreSubsystem::getSingleton().getWorld()->getSceneManager();

		while( found )
		{
			name = baseName + "_" + Ogre::StringConverter::toString(number);

			try
			{
				sceneManager->getEntity(name);
				number++;
			}
			// Nicht gefunden, den Namen k�nnen wir nehmen
			catch( Ogre::Exception ex )
			{
				found = false;
			}
		}

		return name;
	}

	void DotSceneLoader::staticGeometryAddSceneNodeWorkaround( 
		Ogre::StaticGeometry* staticGeom, Ogre::SceneNode* baseNode )
	{
		// Das hier sollte eigentlich reichen
		staticGeom->addSceneNode(baseNode);

		// Aber das hier muss wohl erstmal sein ^^
		Ogre::SceneNode::ChildNodeIterator it = baseNode->getChildIterator();

		while (it.hasMoreElements())
		{
			SceneNode* node = reinterpret_cast<SceneNode*>(it.getNext());
			
			staticGeometryAddSceneNodeWorkaround(staticGeom,node);
		}
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
		catch(...) {}

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
		catch(...) {}

		return Ogre::Vector3::UNIT_SCALE;
	}

	/// @TODO Sollten drei M�glichkeiten sein...
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

		return Ogre::Quaternion::IDENTITY;
	}

}
