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

#include "DotSceneLoader.h"
#include "World.h"

#include "XmlHelper.h"
#include "XmlResourceManager.h"

#include "PhysicsManager.h"
#include "CoreSubsystem.h"

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
		// Usprung und Größe der Blöcke einstellen
		// staticGeom->setRegionDimensions(Vector3(1000,500,1000));
		// staticGeom->setOrigin(Vector3(0,0,0));
		/// FIXME  Diese Methode funktioniert nicht Ogre-Api-korrekt, daher Workaround
		//staticGeom->addSceneNode( staticNode );
		// Alle Entities unterhalb des Nodes einfügen
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
			if( XMLString::compareIString(child->getNodeName(), 
				XMLString::transcode("node") ) == 0  )
				processNode( reinterpret_cast<DOMElement*>(child) , parentNode );
			
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
			XMLString::transcode("name") );
		
		Ogre::SceneNode* newNode;
		// Wurde dem Node ein Name zugewiesen
		if( nodeName.length() == 0 )
			newNode = parentNode->createChildSceneNode();
		else
			newNode = parentNode->createChildSceneNode(nodeName);

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
			rootEntityXml, XMLString::transcode("entity") );
		string meshName = XmlHelper::getAttributeValueAsString( 
			rootEntityXml, XMLString::transcode("meshFile") );

		Ogre::Entity* newEnt = NULL;
		// Wurde der Entity ein Name zugewiesen
		if( entName.length() == 0 )
			entName = getNextEntityName(mSceneName+"_"+parentNode->getName());

		newEnt = mSceneManager->createEntity(entName,meshName);				
		parentNode->attachObject( newEnt );

		CoreSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, " Entity '"+meshName+"' mit dem Namen '"+entName+"' in den Knoten '"+parentNode->getName()+"' eingefügt." );

		// Zur Physik des Levels hinzufügen
		PhysicsManager::getSingleton().addLevelGeometry( newEnt );
		CoreSubsystem::getSingleton().log(Ogre::LML_TRIVIAL, " Entity '"+entName+"' als TriMesh in levelGeometry geladen");


		newEnt->setCastShadows( false );
	    // newEnt->setCastShadows( XmlHelper::getAttributeValueAsBool( rootEntityXml, XMLString::transcode("castShadows") ) );
		// XmlHelper::getAttributeValueAsBool( rootEntityXml, XMLString::transcode("static") );
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
			// Nicht gefunden, den Namen können wir nehmen
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
				XmlHelper::getAttributeValueAsReal( rootPositionXml, XMLString::transcode("x") ),
				XmlHelper::getAttributeValueAsReal( rootPositionXml, XMLString::transcode("y") ),
				XmlHelper::getAttributeValueAsReal( rootPositionXml, XMLString::transcode("z") ) );;
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
				XmlHelper::getAttributeValueAsReal( rootPositionXml, XMLString::transcode("x") ),
				XmlHelper::getAttributeValueAsReal( rootPositionXml, XMLString::transcode("y") ),
				XmlHelper::getAttributeValueAsReal( rootPositionXml, XMLString::transcode("z") ) );
		}
		catch(...) {}

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
				XmlHelper::getAttributeValueAsReal( rootQuatXml, XMLString::transcode("qw") ),
				XmlHelper::getAttributeValueAsReal( rootQuatXml, XMLString::transcode("qx") ),
				XmlHelper::getAttributeValueAsReal( rootQuatXml, XMLString::transcode("qy") ),
				XmlHelper::getAttributeValueAsReal( rootQuatXml, XMLString::transcode("qz") ) );
		}
		catch(...) {}

		// Durch axisX,axisY,axisZ,angle definiert
		try
		{
			return Ogre::Quaternion( Ogre::Radian( XmlHelper::getAttributeValueAsReal( 
				rootQuatXml, XMLString::transcode("angle") ) ),
				Ogre::Vector3(
				XmlHelper::getAttributeValueAsReal( rootQuatXml, XMLString::transcode("axisX") ),
				XmlHelper::getAttributeValueAsReal( rootQuatXml, XMLString::transcode("axisY") ),
				XmlHelper::getAttributeValueAsReal( rootQuatXml, XMLString::transcode("axisZ") )) );
		}
		catch(...) {}

		/* IMPLEMENT  ^^ / Durch angleX,angleY,angleZ definiert
		try
		{
			return Ogre::Quaternion(
				XmlHelper::getAttributeValueAsReal( rootQuatXml, XMLString::transcode("angleX") ),
				XmlHelper::getAttributeValueAsReal( rootQuatXml, XMLString::transcode("angleY") ),
				XmlHelper::getAttributeValueAsReal( rootQuatXml, XMLString::transcode("angleZ") ) );
		}
		catch(...) {} */

		return Ogre::Quaternion::IDENTITY;
	}

}
