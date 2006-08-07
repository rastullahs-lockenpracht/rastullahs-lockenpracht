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
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax/SAXParseException.hpp>

#include <OgreSceneNode.h>

#include "DotSceneLoader.h"
#include "World.h"

#include "PhysicsManager.h"
#include "ActorManager.h"
#include "CoreSubsystem.h"
#include "ConfigurationManager.h"

#include "XmlHelper.h"


#include "Exception.h"

#include <cstdlib>
#include <ctime>

using namespace XERCES_CPP_NAMESPACE;
using namespace std;

namespace rl {
	using XERCES_CPP_NAMESPACE::DOMDocument;

    DotSceneLoader::DotSceneLoader(const string& filename, const string& resourceGroup)
        : mSceneName(filename),
          mResourceGroup(resourceGroup),
		  mStaticNodes(),
		  mRenderingDistance( ActorManager::getSingleton().getDefaultActorRenderingDistance() ),
		  mStaticgeomRenderingDistances(),
		  mStaticgeomBatchSizes(),
          mRessource(NULL),
          mParser(NULL),
          mErrorCount(0)
	{
        srand(static_cast<unsigned int>(time(NULL)));
	}

	DotSceneLoader::~DotSceneLoader()
	{      
        if( mRessource->isLoaded() )
        {
            XmlResourceManager::getSingleton().unload( mSceneName );
            XmlResourceManager::getSingleton().remove( mRessource->getHandle() );
        }
	}

	void DotSceneLoader::initializeScene(SceneManager* sceneManager)
	{
		XMLPlatformUtils::Initialize();
		XmlHelper::initializeTranscoder();

		LOG_NORMAL(Logger::CORE, "Loading Scene from " + mSceneName );
        DOMDocument* doc = openSceneFile();            

		// Durch alle Unterelemente iterieren
		DOMNode* child = doc->getDocumentElement()->getFirstChild();

		while( child != NULL )
		{
			// Ein Node
			XMLCh* nodeTag = XMLString::transcode("userData");
			if( XMLString::compareIString(child->getNodeName(), nodeTag) == 0  )
			{
				processSceneUserData( reinterpret_cast<DOMElement*>(child) );
			}
			XMLString::release(&nodeTag);
			child = child->getNextSibling();
		} 

		LOG_TRIVIAL(Logger::CORE, " Beginne parsen der Unterelemente" );
		DOMElement* nodes = XmlHelper::getChildNamed(doc->getDocumentElement(), "nodes");

		// Eine .scene wird in einem SceneNode mit dem Namen der .scene befestigt
		mSceneNode = sceneManager->getRootSceneNode()->createChildSceneNode(mSceneName);
		processNodes( nodes, sceneManager, mSceneNode );


        if( ConfigurationManager::getSingleton().shouldUseStaticGeometry() )
        {    
			for( std::map<int,Ogre::SceneNode*>::iterator it = mStaticNodes.begin();
				it != mStaticNodes.end(); ) 
			{
				Ogre::SceneNode* staticNode = it->second;
				string staticName = Ogre::StringConverter::toString(it->first);
				Ogre::Real renderDist = mRenderingDistance;
				Ogre::Real batchSize = 25.0;

				if( mStaticgeomRenderingDistances.find( it->first ) != mStaticgeomRenderingDistances.end() )
					renderDist = mStaticgeomRenderingDistances[it->first];
				if( mStaticgeomBatchSizes.find( it->first ) != mStaticgeomBatchSizes.end() )
					batchSize = mStaticgeomBatchSizes[it->first];

				StaticGeometry* staticGeom = sceneManager->createStaticGeometry( mSceneName + staticName  );
				
				staticGeom->setRenderingDistance( renderDist );
				staticGeom->addSceneNode( staticNode );
				staticGeom->setRegionDimensions( batchSize * Vector3::UNIT_SCALE );
				// Statische Geometrie bauen
				staticGeom->build();
				// Nicht mehr den Original-Knoten rendern, da dieser noch erhalten ist.			
				staticNode->setVisible( false );
				staticNode->removeAndDestroyAllChildren();
				sceneManager->destroySceneNode( staticNode->getName() );				
				staticNode = NULL;
				LOG_TRIVIAL(Logger::CORE, " Statische Geometrie "+staticName+" erstellt" );
				++it;
			}
        }
        else
            LOG_TRIVIAL(Logger::CORE, " Keine statischen Geometrien erstellt" );
        
        delete mParser;
		XMLPlatformUtils::Terminate();		
		LOG_TRIVIAL(Logger::CORE, "Szenenbeschreibung aus " + mSceneName +" fertig geparst" );
	}

	DOMDocument* DotSceneLoader::openSceneFile( )
	{
		mParser = new XercesDOMParser();
        mParser->setValidationScheme(XercesDOMParser::Val_Auto);
        mParser->setDoNamespaces(true);
        mParser->setErrorHandler(this);
		
        try
        {
		    mRessource = 
			    XmlResourceManager::getSingleton().create(
			    mSceneName, 
		   	    mResourceGroup);
        }
        catch( Ogre::Exception )
        {
            Throw( IllegalArgumentException, "RessourceGroup '"+mResourceGroup+"' does not exist." );
        }

        mRessource->load();

        if( !mRessource->isLoaded() )
        {
            Throw( FileNotFoundException, "File '"+mSceneName+"' was not found." );
        }

        mRessource.getPointer()->parseBy(mParser);

        if( mErrorCount > 0 )
            Throw( RuntimeException, "File '"+mSceneName+"' could not be parsed." );

		DOMDocument* doc = mParser->getDocument();     

        return doc;
	}

	// Iteriert durch die einzelnen Nodes
	void DotSceneLoader::processNodes(DOMElement* rootNodesXml, SceneManager* sceneManager, 
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
				processNode( reinterpret_cast<DOMElement*>(child), sceneManager, parentNode, NULL );
			}
			XMLString::release(&nodeTag);
			child = child->getNextSibling();
		} 
	}

	// Befasst sich mit einem Node
	void DotSceneLoader::processNode(DOMElement* rootNodeXml, SceneManager* sceneManager, Ogre::SceneNode* parentNode, NodeUserData* parentUserData)
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
            if( !parentNode->getCreator()->hasSceneNode( nodeName ) )
            {
                // Dann versuche einen Knoten mit dem Namen zu erstellen
                newNode = parentNode->createChildSceneNode(nodeName);
            }
            else
            {
                // Name schon vergeben
                newNode = parentNode->createChildSceneNode();
                LOG_TRIVIAL(Logger::CORE, 
                    " NodeName '"+nodeName+"' war schon vergeben! Es wurde der Name '"+newNode->getName()+"' benutzt." );
            }
        }
		else
        {
            newNode = parentNode->createChildSceneNode();
        }	

		LOG_TRIVIAL(Logger::CORE, 
            " Node '"+newNode->getName()+"' als Unterknoten von '"+parentNode->getName()+"' erstellt." );		
		
        DOMNode* child = rootNodeXml->getFirstChild();
        NodeUserData userData;
        // Defaults einstellen
		if( parentUserData == NULL || !parentUserData->is_inheriting )
		{
			userData.is_dynamic = false;
			userData.is_inheriting = false;
			userData.physical_body = "mesh";
			userData.staticgeom_group = -1;
			userData.renderingdistance = mRenderingDistance;
		}
		// Vom Vorgänger erben
		else
		{
			userData.is_dynamic = parentUserData->is_dynamic;
			userData.is_inheriting = parentUserData->is_inheriting;
			userData.physical_body = parentUserData->physical_body;
			userData.staticgeom_group = parentUserData->staticgeom_group;
			userData.renderingdistance = parentUserData->renderingdistance;
		}

        // Durch alle Unterelemente iterieren, um die userDatas zu finden
        while( child != NULL )
        {
            // Ein selbstdefinierter Bereich
            if( XMLString::compareIString(child->getNodeName(), 
                XMLString::transcode("userData") ) == 0 )
			{
                processNodeUserData( reinterpret_cast<DOMElement*>(child) , &userData );
				break;
			}

            child = child->getNextSibling();
        } 

		if( userData.staticgeom_group > -1 )
		{
			newNode->getParentSceneNode()->removeChild( newNode );

			// Existiert noch nicht
			if( mStaticNodes.find( userData.staticgeom_group ) == mStaticNodes.end() )
			{
				mStaticNodes[userData.staticgeom_group] = 
					sceneManager->getRootSceneNode()->createChildSceneNode(
						mSceneName+"_static_"+Ogre::StringConverter::toString(userData.staticgeom_group ) );
			}

			mStaticNodes[userData.staticgeom_group]->addChild( newNode );
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
				processNode( reinterpret_cast<DOMElement*>(child), sceneManager, newNode, &userData );
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
			{
				newNode->setScale( processScale( reinterpret_cast<DOMElement*>(child) ) );
				
				// Skalierung auf alle Entities übertragen
				for( unsigned short i = 0; i < newNode->numAttachedObjects(); i++ )
				{
					MovableObject* mo = newNode->getAttachedObject(i);
					if( mo->getMovableType().compare("Entity") == 0 )
						static_cast<Entity*>(mo)->setNormaliseNormals( newNode->getScale() != Vector3::UNIT_SCALE );
				}
			}
			// Eine Entity
			else if( XMLString::compareIString(child->getNodeName(), 
				XMLString::transcode("entity") ) == 0  )                
				processEntity( reinterpret_cast<DOMElement*>(child), 
					sceneManager,
					newNode,
					createMeshPhysicalBody, userData.renderingdistance );

			child = child->getNextSibling();
		} 
	}

	void DotSceneLoader::processSceneUserData( XERCES_CPP_NAMESPACE::DOMElement* rootUserDataXml )
	{
		DOMNode* child = rootUserDataXml->getFirstChild();
        LOG_TRIVIAL(Logger::CORE, " SceneUserData gefunden");

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
					if( propertyName.compare("default_renderingdistance") == 0 )
						mRenderingDistance = XmlHelper::getAttributeValueAsReal( 
                            propertyXml, "data" );
					else if( Ogre::StringUtil::startsWith(propertyName,"staticgeom_renderingdistance_") )
					{
						int groupId = Ogre::StringConverter::parseInt( propertyName.substr( 29 ) );
						
						mStaticgeomRenderingDistances[groupId] = XmlHelper::getAttributeValueAsReal( 
                            propertyXml, "data" );
					}
					else if( Ogre::StringUtil::startsWith(propertyName,"staticgeom_batchsize_") )
					{
						int groupId = Ogre::StringConverter::parseInt( propertyName.substr( 21 ) );
						
						mStaticgeomBatchSizes[groupId] = XmlHelper::getAttributeValueAsReal( 
                            propertyXml, "data" );
					}
                }
                catch(...)
                {
                    LOG_TRIVIAL(Logger::CORE, 
                        " > Parse Error beim Übernehmen der Property '"+propertyName+"'!");
                }

            }
			// Manuell definiertes LOD
			else if( XMLString::compareIString(child->getNodeName(), 
					 XMLString::transcode("manualLOD") ) == 0 )
            {
				DOMElement* lodXml = reinterpret_cast<DOMElement*>(child);
                string meshName = XmlHelper::getAttributeValueAsStdString( lodXml, 
			        "mesh" );
				
                try
                {
					Ogre::MeshPtr mesh = Ogre::MeshManager::getSingleton().getByName( meshName );
                    mesh->removeLodLevels();

					DOMNode* lodchild = child->getFirstChild();
					LOG_TRIVIAL(Logger::CORE, " LOD-Bereich für "+meshName+" gefunden");
					Ogre::Real loddist = 10.0;
					string lodmeshName = "";

					// Durch alle Unterelemente iterieren, um die LODs zu finden
					while( lodchild != NULL )
					{						
						if( XMLString::compareIString(lodchild->getNodeName(), 
							XMLString::transcode("LOD") ) == 0 )
						{
							loddist = 0.0;
							lodmeshName = "";

							try
							{
								lodXml = reinterpret_cast<DOMElement*>(lodchild);
								loddist = XmlHelper::getAttributeValueAsReal( lodXml, 
									"distance" );
								lodmeshName = XmlHelper::getAttributeValueAsStdString( lodXml, 
									"mesh" );

								if( lodmeshName.length() > 0 && loddist > 0 )
								{
									mesh->createManualLodLevel(loddist, lodmeshName );
									LOG_TRIVIAL(Logger::CORE, 
										" LOD für bei '"+Ogre::StringConverter::toString(loddist)+
										"' als '"+lodmeshName+"' gesetzt!");
								}
							}
							catch(...) { }
						}

						lodchild = lodchild->getNextSibling();
					}
				}
				catch(...)
                {
					LOG_NORMAL(Logger::CORE, 
                        " > Parse Error beim Setzen der LOD für '"+meshName+"'!");
                }
			}
            child = child->getNextSibling();
        } 
	}

    void DotSceneLoader::processNodeUserData( XERCES_CPP_NAMESPACE::DOMElement* rootUserDataXml, 
        NodeUserData* userData )
    {
        DOMNode* child = rootUserDataXml->getFirstChild();
        LOG_TRIVIAL(Logger::CORE, " NodeUserData gefunden");

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
                    else if( propertyName.compare("dynamic") == 0 )
                        userData->is_dynamic = XmlHelper::getAttributeValueAsInteger( 
                        propertyXml, "data" ) != 0;
					else if( propertyName.compare("staticgeom_group") == 0 )
						userData->staticgeom_group = XmlHelper::getAttributeValueAsInteger( 
                        propertyXml, "data" );
					else if( propertyName.compare("renderingdistance") == 0 )
						userData->renderingdistance = XmlHelper::getAttributeValueAsReal( 
                        propertyXml, "data" );
                }
                catch(...)
                {
                    LOG_NORMAL(Logger::CORE, 
                        " > Parse Error beim Übernehmen der Property '"+propertyName+"'!");
                }

            }

            child = child->getNextSibling();
        } 
    }

	// Eine Entity
	void DotSceneLoader::processEntity( DOMElement* rootEntityXml,
		SceneManager* sceneManager, Ogre::SceneNode* parentNode, 
		bool createMeshPhysicalBody, Ogre::Real renderingDistance )
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

        ResourceGroupManager& resGroupMgr = ResourceGroupManager::getSingleton();
        while( parentNode->getCreator()->hasEntity( entName ) )
        {
            entName = getRandomName(entName);
        }

        try
        {
            // if this mesh exists in our module's resource group: preload it
            if( resGroupMgr.resourceExists(mResourceGroup, meshName) )
            {
                MeshManager::getSingleton().load(meshName, mResourceGroup);
            }

            // If not, it is now loaded implicitly from the default group
            newEnt = sceneManager->createEntity(entName, meshName);
        }
        catch( ... )
        {
              LOG_ERROR(Logger::CORE, " Entity '"+meshName+"' mit dem Namen '"+entName+"' konnte nicht geladen werden." );
              return;
        }

		if( parentNode->getScale() != Vector3::UNIT_SCALE )
			newEnt->setNormaliseNormals( true );

        parentNode->attachObject( newEnt );

        LOG_TRIVIAL(Logger::CORE, " Entity '"+meshName+"' mit dem Namen '"+entName+"' in den Knoten '"+parentNode->getName()+"' eingefügt." );

        // Zur Physik des Levels hinzufügen
        if( createMeshPhysicalBody )
        {                
            PhysicsManager::getSingleton().addLevelGeometry( newEnt );
            LOG_TRIVIAL(Logger::CORE, " Entity '"+entName+"' als TriMesh in levelGeometry geladen");
        }
        
        // Renderingdistanz berechnen
        if( renderingDistance == mRenderingDistance )
        {
            Ogre::Real diameter 
                = (newEnt->getBoundingBox().getMaximum() - newEnt->getBoundingBox().getMinimum()).length();  

            // Gerade mal 10cm² => 10m
            if( diameter <= 0.5 )
                renderingDistance = 15;
            // Gerade mal 1,5m² => 25m
            else if( diameter <= 1.5 )
                renderingDistance = 30;
            // Gerade mal 2,5m² => 50m
            else if( diameter <= 2.5 )
                renderingDistance = 60;
            else if( diameter <= 10 )
                renderingDistance = 150;
            else if( diameter <= 50 )
                renderingDistance = 250;
            else if( diameter <= 100 )
                renderingDistance = 450;
            else
                renderingDistance = 1500;

            newEnt->setRenderingDistance( renderingDistance );
        }
        else
		    newEnt->setRenderingDistance( renderingDistance );
        newEnt->setCastShadows( false );
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
		LOG_TRIVIAL(Logger::CORE, " Position gefunden");

		try
		{
			if( rootPositionXml->hasAttribute( XMLString::transcode("x") ) && 
				rootPositionXml->hasAttribute( XMLString::transcode("y") ) && 
				rootPositionXml->hasAttribute( XMLString::transcode("z") ) )
			{
				return Ogre::Vector3( 
					XmlHelper::getAttributeValueAsReal( rootPositionXml, "x" ),
					XmlHelper::getAttributeValueAsReal( rootPositionXml, "y" ),
					XmlHelper::getAttributeValueAsReal( rootPositionXml, "z" ) );
			}
		}
		catch(...) { }
         
		LOG_NORMAL(Logger::CORE, " > Parse Error beim Übernehmen der Position! ");

		return Ogre::Vector3::ZERO;
	}


	Ogre::Vector3 DotSceneLoader::processScale( DOMElement* rootScaleXml )
	{
		LOG_TRIVIAL(Logger::CORE, " Skalierung gefunden");

		try
		{
			if( rootScaleXml->hasAttribute( XMLString::transcode("x") ) && 
				rootScaleXml->hasAttribute( XMLString::transcode("y") ) && 
				rootScaleXml->hasAttribute( XMLString::transcode("z") ) )
			{
				return Ogre::Vector3( 
					XmlHelper::getAttributeValueAsReal( rootScaleXml, "x" ),
					XmlHelper::getAttributeValueAsReal( rootScaleXml, "y" ),
					XmlHelper::getAttributeValueAsReal( rootScaleXml, "z" ) );
			}
		}
        catch(...) { }

		LOG_NORMAL(Logger::CORE, " > Parse Error beim Übernehmen der Skalierung! ");

		return Ogre::Vector3::UNIT_SCALE;
	}

	/// @TODO Sollten drei Möglichkeiten sein...
	Ogre::Quaternion DotSceneLoader::processRotation( DOMElement* rootQuatXml )
	{
		LOG_TRIVIAL(Logger::CORE, " Rotation gefunden");

		try
		{
			// Durch w,x,y,z definiert
			if( rootQuatXml->hasAttribute( XMLString::transcode("qw") ) && 
				rootQuatXml->hasAttribute( XMLString::transcode("qx") ) && 
				rootQuatXml->hasAttribute( XMLString::transcode("qy") ) && 
				rootQuatXml->hasAttribute( XMLString::transcode("qz") ) )
			{

				return Ogre::Quaternion( 
					XmlHelper::getAttributeValueAsReal( rootQuatXml, "qw" ),
					XmlHelper::getAttributeValueAsReal( rootQuatXml, "qx" ),
					XmlHelper::getAttributeValueAsReal( rootQuatXml, "qy" ),
					XmlHelper::getAttributeValueAsReal( rootQuatXml, "qz" ) );
			}

			// Durch axisX,axisY,axisZ,angle definiert
			if( rootQuatXml->hasAttribute( XMLString::transcode("angle") ) &&
				rootQuatXml->hasAttribute( XMLString::transcode("axisX") ) && 
				rootQuatXml->hasAttribute( XMLString::transcode("axisY") ) && 
				rootQuatXml->hasAttribute( XMLString::transcode("axisZ") )  )
			{
				return Ogre::Quaternion( 
					Ogre::Degree( XmlHelper::getAttributeValueAsReal( rootQuatXml, "angle" ) ),
					Ogre::Vector3(
					XmlHelper::getAttributeValueAsReal( rootQuatXml, "axisX" ),
					XmlHelper::getAttributeValueAsReal( rootQuatXml, "axisY" ),
					XmlHelper::getAttributeValueAsReal( rootQuatXml, "axisZ" ) ) );
			}

			// Durch angleX,angleY,angleZ definiert
			if( rootQuatXml->hasAttribute( XMLString::transcode("angleX") ) && 
				rootQuatXml->hasAttribute( XMLString::transcode("angleY") ) && 
				rootQuatXml->hasAttribute( XMLString::transcode("angleZ") )  )
			{
				Ogre::Matrix3 mat;
				mat.FromEulerAnglesXYZ(
					Degree(XmlHelper::getAttributeValueAsReal(rootQuatXml, "angleX")),
					Degree(XmlHelper::getAttributeValueAsReal(rootQuatXml, "angleY")),
					Degree(XmlHelper::getAttributeValueAsReal(rootQuatXml, "angleZ")));
				return Quaternion(mat);
			}
		}
		catch(...) {}

        LOG_NORMAL(Logger::CORE, " > Parse Error beim Übernehmen der Rotation! ");

		return Ogre::Quaternion::IDENTITY;
	}


    void DotSceneLoader::warning(const XERCES_CPP_NAMESPACE::SAXParseException& exc)
    {
        LOG_NORMAL(Logger::CORE, toString( " warning ", exc ) );
    }
    void DotSceneLoader::error(const XERCES_CPP_NAMESPACE::SAXParseException& exc)
    {
        LOG_ERROR(Logger::CORE, toString( "n error", exc ) );
        mErrorCount++;
    }
    void DotSceneLoader::fatalError(const XERCES_CPP_NAMESPACE::SAXParseException& exc)
    {
        LOG_CRITICAL(Logger::CORE, toString( " fatal error", exc ) );
        mErrorCount++;
    }
    void DotSceneLoader::resetErrors()
    {
    }

    std::string DotSceneLoader::toString( const std::string& type,
        const XERCES_CPP_NAMESPACE::SAXParseException& exc ) const
    {
        std::stringstream strs;
        strs << "A" << type << " occured while parsing " << mSceneName 
             << " at line " << exc.getLineNumber() << " column " <<  exc.getColumnNumber();

        if( exc.getSystemId() != NULL )
            strs << " with system " << XmlHelper::transcodeToStdString( exc.getSystemId() );
        if( exc.getPublicId() != NULL )
            strs << " with public " << XmlHelper::transcodeToStdString( exc.getPublicId() );
    
        return strs.str();
    }
}

