/* This source file is part of Rastullahs Lockenpracht.
 * Copyright (C) 2003-2007 Team Pantheon. http://www.team-pantheon.de
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

#include "Property.h"
#include "PropertyReader.h"
#include "XmlHelper.h"


#include "Exception.h"

#include <cstdlib>
#include <ctime>

using namespace XERCES_CPP_NAMESPACE;
using namespace std;
using namespace Ogre;

namespace rl {
    using XERCES_CPP_NAMESPACE::DOMDocument;

    DotSceneLoader::DotSceneLoader(const string& filename, const string& resourceGroup)
        : mSceneName(filename),
          mResourceGroup(resourceGroup),
          mStaticNodes(),
          mRenderingDistance(ActorManager::getSingleton().getDefaultActorRenderingDistance()),
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
        if (mRessource->isLoaded())
        {
            XmlResourceManager::getSingleton().unload(mSceneName);
            XmlResourceManager::getSingleton().remove(mRessource->getHandle());
        }
    }

    void DotSceneLoader::initializeScene(SceneManager* sceneManager)
    {
        XMLPlatformUtils::Initialize();
        XmlHelper::initializeTranscoder();

        LOG_MESSAGE(Logger::CORE, "Loading Scene from " + mSceneName);
        DOMDocument* doc = openSceneFile();

        // Durch alle Unterelemente iterieren
        DOMNode* child = doc->getDocumentElement()->getFirstChild();

        while(child != NULL)
        {
            // Ein Node
            if (XMLString::compareIString(child->getNodeName(), AutoXMLCh("userData").data()) == 0 )
            {
                processSceneUserData(static_cast<DOMElement*>(child));
            }
            child = child->getNextSibling();
        }

        LOG_DEBUG(Logger::CORE, " Beginne parsen der Unterelemente");
        DOMElement* nodes = XmlHelper::getChildNamed(doc->getDocumentElement(), "nodes");

        // Eine .scene wird in einem SceneNode mit dem Namen der .scene befestigt
        mSceneNode = sceneManager->getRootSceneNode()->createChildSceneNode(mSceneName);
        processNodes(nodes, sceneManager, mSceneNode);

        // Find out, if static geometry should be used
        Ogre::String temp = ConfigurationManager::getSingleton().getStringSetting(ConfigurationManager::CS_GRAPHICS, "Use Static Geometry");
        bool useStaticGeometry;

        if (temp == "yes")
        {
            useStaticGeometry = true;
        }
        else if (temp == "no")
        {
            useStaticGeometry = false;
        }
        else
        {
            // Check, if Renderer supports vertex buffer
            useStaticGeometry = Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_VBO);
        }

        if (useStaticGeometry)
        {
            for(std::map<int,Ogre::SceneNode*>::iterator it = mStaticNodes.begin();
                it != mStaticNodes.end();)
            {
                Ogre::SceneNode* staticNode = it->second;
                string staticName = Ogre::StringConverter::toString(it->first);
                Ogre::Real renderDist = mRenderingDistance;
                Ogre::Real batchSize = 25.0;

                if (mStaticgeomRenderingDistances.find(it->first) != mStaticgeomRenderingDistances.end())
                    renderDist = mStaticgeomRenderingDistances[it->first];
                if (mStaticgeomBatchSizes.find(it->first) != mStaticgeomBatchSizes.end())
                    batchSize = mStaticgeomBatchSizes[it->first];

                StaticGeometry* staticGeom = sceneManager->createStaticGeometry(mSceneName + staticName );

                staticGeom->setRenderingDistance(renderDist);
                staticGeom->addSceneNode(staticNode);
                staticGeom->setRegionDimensions(batchSize * Vector3::UNIT_SCALE);
                // Statische Geometrie bauen
                staticGeom->build();
                // Nicht mehr den Original-Knoten rendern, da dieser noch erhalten ist.
                staticNode->setVisible(false);
                staticNode->removeAndDestroyAllChildren();
                sceneManager->destroySceneNode(staticNode->getName());
                staticNode = NULL;
                LOG_DEBUG(Logger::CORE, " Statische Geometrie "+staticName+" erstellt");
                ++it;
            }
        }
        else
            LOG_DEBUG(Logger::CORE, " Keine statischen Geometrien erstellt");

        delete mParser;
        XMLPlatformUtils::Terminate();
        LOG_DEBUG(Logger::CORE, "Szenenbeschreibung aus " + mSceneName +" fertig geparst");
    }

    DOMDocument* DotSceneLoader::openSceneFile()
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
        catch(Ogre::Exception)
        {
            Throw(IllegalArgumentException, "RessourceGroup '"+mResourceGroup+"' does not exist.");
        }

        mRessource->load();

        if (!mRessource->isLoaded())
        {
            Throw(FileNotFoundException, "File '"+mSceneName+"' was not found.");
        }

        mRessource.getPointer()->parseBy(mParser);

        if (mErrorCount > 0)
            Throw(RuntimeException, "File '"+mSceneName+"' could not be parsed.");

        DOMDocument* doc = mParser->getDocument();

        return doc;
    }

    // Iteriert durch die einzelnen Nodes
    void DotSceneLoader::processNodes(DOMElement* rootNodesXml, SceneManager* sceneManager,
        Ogre::SceneNode* parentNode)
    {
        if (rootNodesXml == NULL)
            return;
        if (parentNode == NULL)
            Throw(NullPointerException, "parentNode darf nicht null sein");


        // Durch alle Unterelemente iterieren
        DOMNode* child = rootNodesXml->getFirstChild();

        while(child != NULL)
        {
            // Ein Node
            if (XMLString::compareIString(child->getNodeName(), AutoXMLCh("node").data()) == 0 )
            {
                processNode(static_cast<DOMElement*>(child), sceneManager, parentNode, NULL);
            }
            child = child->getNextSibling();
        }
    }

    // Befasst sich mit einem Node
    void DotSceneLoader::processNode(DOMElement* rootNodeXml, SceneManager* sceneManager, Ogre::SceneNode* parentNode, NodeUserData* parentUserData)
    {
        if (rootNodeXml == NULL)
            return;
        if (parentNode == NULL)
            Throw(NullPointerException, "parentNode darf nicht null sein");

        string nodeName = XmlHelper::getAttributeValueAsStdString(rootNodeXml,
                        "name");

        Ogre::SceneNode* newNode;
        // Wurde dem Node ein Name zugewiesen?
        if (nodeName.length() > 0)
        {
            if (!parentNode->getCreator()->hasSceneNode(nodeName))
            {
                // Dann versuche einen Knoten mit dem Namen zu erstellen
                newNode = parentNode->createChildSceneNode(nodeName);
            }
            else
            {
                // Name schon vergeben
                newNode = parentNode->createChildSceneNode();
                LOG_DEBUG(Logger::CORE,
                    " NodeName '"+nodeName+"' war schon vergeben! Es wurde der Name '"+newNode->getName()+"' benutzt.");
            }
        }
        else
        {
            newNode = parentNode->createChildSceneNode();
        }

        LOG_DEBUG(Logger::CORE,
                    " Node '"+newNode->getName()+"' als Unterknoten von '"+parentNode->getName()+"' erstellt.");


        NodeUserData userData;
        // Defaults einstellen
        if (parentUserData == NULL || !parentUserData->is_inheriting)
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


        mCollisions.clear();
        DOMNode* child = rootNodeXml->getFirstChild();
        DOMNode *childScale = NULL, *childPosition = NULL,
        *childRotation = NULL;
        // Durch alle Unterelemente iterieren und gefundenes speichern
        while(child != NULL)
        {
            // Ein selbstdefinierter Bereich
            if (XMLString::compareIString(child->getNodeName(),
                AutoXMLCh("userData").data()) == 0)
            {
                // UserData direkt auslesen
                processNodeUserData(static_cast<DOMElement*>(child) , &userData);
            }
            else if (XMLString::compareIString(child->getNodeName(),
                     AutoXMLCh("position").data()) == 0)
                childPosition = child;//rootNodeXml->removeChild(child);
            else if (XMLString::compareIString(child->getNodeName(),
                     AutoXMLCh("rotation").data()) == 0)
                childRotation = child;//rootNodeXml->removeChild(child);
            else if (XMLString::compareIString(child->getNodeName(),
                     AutoXMLCh("scale").data()) == 0)
                childScale = child;//rootNodeXml->removeChild(child);

            child = child->getNextSibling();
        }
        // so jetzt stehen nur noch entities und nodes in der liste





        if (userData.staticgeom_group > -1)
        {
            newNode->getParentSceneNode()->removeChild(newNode);

            // Existiert noch nicht
            if (mStaticNodes.find(userData.staticgeom_group) == mStaticNodes.end())
            {
                mStaticNodes[userData.staticgeom_group] =
                sceneManager->getRootSceneNode()->createChildSceneNode(
                mSceneName+"_static_"+Ogre::StringConverter::toString(userData.staticgeom_group));
            }

            mStaticNodes[userData.staticgeom_group]->addChild(newNode);
        }


        // Einzelne weitere childs auslesen, falls nötig, in der richtigen Reihenfolge

        // Position des Nodes
        if (childPosition != NULL)
            newNode->setPosition(processPosition(static_cast<DOMElement*>(childPosition)));

        // Rotation des Nodes
        if (childRotation != NULL)
            newNode->setOrientation(processRotation(static_cast<DOMElement*>(childRotation)));

        // Skalierung des Nodes
        if (childScale != NULL)
            newNode->setScale(processScale(static_cast<DOMElement*>(childScale)));

/*      // nun nicht mehr nötig!
        {
            // Skalierung auf alle Entities übertragen
            for(unsigned short i = 0; i < newNode->numAttachedObjects(); i++)
            {
                MovableObject* mo = newNode->getAttachedObject(i);
                if (mo->getMovableType().compare("Entity") == 0)
                    static_cast<Entity*>(mo)->setNormaliseNormals(newNode->getScale() != Vector3::UNIT_SCALE);
            }
        }
*/



        // Alle Entities und Subnodes auslesen
        child = rootNodeXml->getFirstChild();
        while(child != NULL)
        {
            // geschachteltes weiteres Node
            if (XMLString::compareIString(child->getNodeName(),
                AutoXMLCh("node").data()) == 0)
                processNode(static_cast<DOMElement*>(child), sceneManager, newNode, &userData);
            // Eine Entity
            else if (XMLString::compareIString(child->getNodeName(),
                     AutoXMLCh("entity").data()) == 0)
                processEntity(static_cast<DOMElement*>(child),
                                sceneManager,
                                newNode,
                                userData.renderingdistance,
                                userData.physical_body);

            child = child->getNextSibling();
        }
    }

    void DotSceneLoader::processSceneUserData(XERCES_CPP_NAMESPACE::DOMElement* rootUserDataXml)
    {
        DOMNode* child = rootUserDataXml->getFirstChild();
        LOG_DEBUG(Logger::CORE, " SceneUserData gefunden");
        XmlPropertyReader* propertyReader = new XmlPropertyReader();

        // Durch alle Unterelemente iterieren, um die properties zu finden
        while(child != NULL)
        {
            // Ein selbstdefinierter Bereich
            if (XMLString::compareIString(child->getNodeName(),
                AutoXMLCh("property").data()) == 0)
            {
                DOMElement* propertyXml = static_cast<DOMElement*>(child);

                PropertyEntry entry = propertyReader->processProperty(propertyXml);

                try
                {
                    if (entry.first == "default_renderingdistance")
                    {
                        mRenderingDistance = entry.second.toReal();
                    }
                    else if (Ogre::StringUtil::startsWith(entry.first, "staticgeom_renderingdistance_"))
                    {
                        int groupId = Ogre::StringConverter::parseInt(entry.first.substr(29));
                        mStaticgeomRenderingDistances[groupId] = entry.second.toReal();
                    }
                    else if (Ogre::StringUtil::startsWith(entry.first, "staticgeom_batchsize_"))
                    {
                        int groupId = Ogre::StringConverter::parseInt(entry.first.substr(21));
                        mStaticgeomBatchSizes[groupId] = entry.second.toReal();
                    }
                }
                catch(...)
                {
                    LOG_DEBUG(Logger::CORE,
                        " > Parse Error beim Übernehmen der Property '"+entry.first+"'!");
                }

            }
            // Manuell definiertes LOD
            else if (XMLString::compareIString(child->getNodeName(),
                AutoXMLCh("manualLOD").data()) == 0)
            {
                DOMElement* lodXml = static_cast<DOMElement*>(child);
                string meshName = XmlHelper::getAttributeValueAsStdString(lodXml,
                    "mesh");

                try
                {
                    Ogre::MeshPtr mesh = Ogre::MeshManager::getSingleton().getByName(meshName);
                    mesh->removeLodLevels();

                    DOMNode* lodchild = child->getFirstChild();
                    LOG_DEBUG(Logger::CORE, " LOD-Bereich für "+meshName+" gefunden");
                    Ogre::Real loddist = 10.0;
                    string lodmeshName = "";

                    // Durch alle Unterelemente iterieren, um die LODs zu finden
                    while(lodchild != NULL)
                    {
                        if (XMLString::compareIString(lodchild->getNodeName(),
                            AutoXMLCh("LOD").data()) == 0)
                        {
                            loddist = 0.0;
                            lodmeshName = "";

                            try
                            {
                                lodXml = static_cast<DOMElement*>(lodchild);
                                loddist = XmlHelper::getAttributeValueAsReal(lodXml,
                                    "distance");
                                lodmeshName = XmlHelper::getAttributeValueAsStdString(lodXml,
                                    "mesh");

                                if (lodmeshName.length() > 0 && loddist > 0)
                                {
                                    mesh->createManualLodLevel(loddist, lodmeshName);
                                    LOG_DEBUG(Logger::CORE,
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
                    LOG_MESSAGE(Logger::CORE,
                        " > Parse Error beim Setzen der LOD für '"+meshName+"'!");
                }
            }
            child = child->getNextSibling();
        }

        delete propertyReader;
    }

    void DotSceneLoader::processNodeUserData(XERCES_CPP_NAMESPACE::DOMElement* rootUserDataXml,
        NodeUserData* userData)
    {
        DOMNode* child = rootUserDataXml->getFirstChild();
        LOG_DEBUG(Logger::CORE, " NodeUserData gefunden");
        XmlPropertyReader* propertyReader = new XmlPropertyReader();

        // Durch alle Unterelemente iterieren, um die properties zu finden
        while(child != NULL)
        {
            // Ein selbstdefinierter Bereich
            if (XMLString::compareIString(child->getNodeName(),
                AutoXMLCh("property").data()) == 0)
            {
                DOMElement* propertyXml = static_cast<DOMElement*>(child);
                PropertyEntry entry = propertyReader->processProperty(propertyXml);

                try
                {
                    if (entry.first == "physical_body")
                        userData->physical_body = entry.second.toString().c_str();
                    else if (entry.first == "dynamic")
                        userData->is_dynamic = entry.second.toInt() != 0;
                    else if (entry.first == "staticgeom_group")
                        userData->staticgeom_group = entry.second.toInt();
                    else if (entry.first == "renderingdistance")
                        userData->renderingdistance = entry.second.toReal();
                }
                catch(...)
                {
                    LOG_MESSAGE(Logger::CORE,
                        " > Parse Error beim Übernehmen der Property '"+entry.first+"'!");
                }

            }
            else if (XMLString::compareIString(child->getNodeName(),
                        AutoXMLCh("collisions").data()) == 0)
                    processCollisions(static_cast<DOMElement*> (child));


            child = child->getNextSibling();
        }

        delete propertyReader;
    }

    // eine benutzerdefinierte Collision
    void DotSceneLoader::processCollisions(XERCES_CPP_NAMESPACE::DOMElement *rootCollisionXml)
    {
        OgreNewt::CollisionPtr collision = OgreNewt::CollisionPtr();
        DOMNode* child = rootCollisionXml->getFirstChild();
        OgreNewt::World *thisWorld = PhysicsManager::getSingleton()._getNewtonWorld();

        std::vector<Ogre::Vector3> vec3Vector;
        Ogre::Vector3 scale, offset;
        Ogre::Quaternion rotation;


        LOG_DEBUG(Logger::CORE, " collisions in NodeUserData gefunden");


        while(child != NULL)
        {
            if (child->getNodeType() != DOMNode::ELEMENT_NODE)
            {
                child = child->getNextSibling();
                continue;
            }
            collision.setNull();
            // am Anfang steht ein Node mit dem Typ
            std::string typeAsString = XmlHelper::transcodeToStdString(child->getNodeName());

            scale = Ogre::Vector3::UNIT_SCALE;
            offset = Ogre::Vector3::ZERO;
            rotation = Ogre::Quaternion::IDENTITY;
            vec3Vector.clear();

            DOMNode *childChild = static_cast<DOMElement*>(child)->getFirstChild();
            while(childChild != NULL)
            {
                if (childChild->getNodeType() == DOMNode::ELEMENT_NODE)
                {
                    if (XMLString::compareIString(childChild->getNodeName(),
                        AutoXMLCh("scale").data()) == 0)
                        scale = processScale(static_cast<DOMElement*>(childChild));
                    else if (XMLString::compareIString(childChild->getNodeName(),
                             AutoXMLCh("offset").data()) == 0)
                        offset = processPosition(static_cast<DOMElement*>(childChild));
                    else if (XMLString::compareIString(childChild->getNodeName(),
                             AutoXMLCh("rotation").data()) == 0)
                        rotation = processRotation(static_cast<DOMElement*>(childChild));

                    else if (XMLString::compareIString(childChild->getNodeName(),
                             AutoXMLCh("vertices").data()) == 0)
                    {
                        DOMNode *childChildChild = static_cast<DOMElement*>(childChild)->getFirstChild();
                        while(childChildChild != NULL)
                        {
                            if (childChild->getNodeType() == DOMNode::ELEMENT_NODE)
                            {
                                if (XMLString::compareIString(childChildChild->getNodeName(),
                                    AutoXMLCh("vertex").data()) == 0)
                                {
                                    bool error = false;
                                    Ogre::Vector3 vec3 = processVector(static_cast<DOMElement*>(childChildChild), error);
                                    if (!error)
                                        vec3Vector.push_back(vec3);
                                }
                            }
                            childChildChild = childChildChild->getNextSibling();
                        }
                    }
                }
                childChild = childChild->getNextSibling();
            }

            // typangabe aus String extrahieren
            if (typeAsString == "convexhull")
            {
                int vertcount = vec3Vector.size();
                Ogre::Vector3 *vertices = new Ogre::Vector3[ vertcount ];
                Ogre::Vector3 vec3Min(0,0,0), vec3Max(0,0,0);
                for(int i = 0; i < vertcount; i++)
                {
                    vertices[i] = vec3Vector[i] * scale; // in array übertragen


                    if (vertices[i].x < vec3Min.x)   // und größe des körpers bestimmen
                        vec3Min.x = vertices[i].x;
                    if (vertices[i].y < vec3Min.y)
                        vec3Min.y = vertices[i].y;
                    if (vertices[i].z < vec3Min.z)
                        vec3Min.z = vertices[i].z;

                    if (vertices[i].x > vec3Max.x)
                        vec3Max.x = vertices[i].x;
                    if (vertices[i].y > vec3Max.y)
                        vec3Max.y = vertices[i].y;
                    if (vertices[i].z > vec3Max.z)
                        vec3Max.z = vertices[i].z;
                }

                Ogre::Vector3 size = vec3Max - vec3Min;
                // Größe überprüfen
                if (size.x < PhysicsManager::NEWTON_GRID_WIDTH ||
                    size.y < PhysicsManager::NEWTON_GRID_WIDTH ||
                    size.z < PhysicsManager::NEWTON_GRID_WIDTH ||
                    vertcount < 4)
                {
                    LOG_MESSAGE(Logger::CORE,
                        " physical_body 'convexhull' in <collisions> konnte nicht erstellt werden; der Körper ist zu klein!");

                    // Minimale Größe verwenden
                    if (size.x < PhysicsManager::NEWTON_GRID_WIDTH)
                        size.x = PhysicsManager::NEWTON_GRID_WIDTH;
                    if (size.y < PhysicsManager::NEWTON_GRID_WIDTH)
                        size.y = PhysicsManager::NEWTON_GRID_WIDTH;
                    if (size.z < PhysicsManager::NEWTON_GRID_WIDTH)
                        size.z = PhysicsManager::NEWTON_GRID_WIDTH;

                    collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::Box(
                                     thisWorld, size, rotation, offset));

                    LOG_MESSAGE(Logger::CORE, " stattdessen physical_body 'box' erstellt. ");
                }
                else
                {
                    collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(
                                    thisWorld,
                                    vertices,
                                    vertcount,
                                    rotation, offset));

                    LOG_DEBUG(Logger::CORE, " physical_body 'convexhull' in <collisions> erstellt. ");
                }

                delete [] vertices;
            }
            else
            {
                if (!vec3Vector.empty())   // fehler!
                {
                    LOG_MESSAGE(Logger::CORE, " > Parse Error beim Einlesen einer Collision; Es könne nur für Convexhull Vektoren definiert werden!");
                }

                if (typeAsString.compare("box") == 0)
                {
                    collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::Box(
                                    thisWorld, scale, rotation, offset));
                    LOG_DEBUG(Logger::CORE, " physical_body 'box' in <collisions> erstellt. ");
                }
                else if (typeAsString.compare("sphere") == 0)
                {
                    collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::Ellipsoid(
                                    thisWorld, scale, rotation, offset));
                    LOG_DEBUG(Logger::CORE, " physical_body 'sphere' in <collisions> erstellt. ");
                }
                else if (typeAsString.compare("ellipsoid") == 0)
                {
                    collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::Ellipsoid(
                    thisWorld, scale, rotation, offset));
                    LOG_DEBUG(Logger::CORE, " physical_body 'ellipsoid' in <collisions> erstellt. ");
                }
                else if (typeAsString.compare("pyramid") == 0)
                {
                    collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::Pyramid(
                    thisWorld, scale, rotation, offset));
                    LOG_DEBUG(Logger::CORE, " physical_body 'pyramid' in <collisions> erstellt. ");
                }
                else if (typeAsString.compare("capsule") == 0)
                {
                    double radius = std::max(scale.x, scale.z) / 2.0;
                    double height = scale.y;
                    offset.x -= scale.y/2;
                    Ogre::Quaternion tempRot;
                    tempRot.FromAngleAxis(Degree(90), Ogre::Vector3::UNIT_Z);
                    rotation = (rotation * tempRot);

                    collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::Capsule(
                                    thisWorld,
                                    radius,
                                    height,
                                    rotation, offset));
                    LOG_DEBUG(Logger::CORE, " physical_body 'capsule' in <collisions> erstellt. ");
                }
                else if (typeAsString.compare("cone") == 0)
                {
                    double radius = std::max(scale.x, scale.z) / 2.0;
                    double height = scale.y;
                    offset.x -= scale.y/2;
                    Ogre::Quaternion tempRot;
                    tempRot.FromAngleAxis(Degree(90), Ogre::Vector3::UNIT_Z);
                    rotation = (rotation * tempRot);

                    collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::Cone(
                                    thisWorld,
                                    radius,
                                    height,
                                    rotation, offset));
                    LOG_DEBUG(Logger::CORE, " physical_body 'cone' in <collisions> erstellt. ");
                }
                else if (typeAsString.compare("cylinder") == 0)
                {
                    double radius = std::max(scale.x, scale.z) / 2.0;
                    double height = scale.y;
                    offset.x -= scale.y/2;
                    Ogre::Quaternion tempRot;
                    tempRot.FromAngleAxis(Degree(90), Ogre::Vector3::UNIT_Z);
                    rotation = (rotation * tempRot);

                    collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::Cylinder(
                                    thisWorld,
                                    radius,
                                    height,
                                    rotation, offset));
                    LOG_DEBUG(Logger::CORE, " physical_body 'cylinder' in <collisions> erstellt. ");
                }
                else if (typeAsString.compare("chamfer_cylinder") == 0)
                {
                    double radius = std::max(scale.x, scale.z) / 2.0;
                    double height = scale.y;
                    offset.x -= scale.y/2;
                    Ogre::Quaternion tempRot;
                    tempRot.FromAngleAxis(Degree(90), Ogre::Vector3::UNIT_Z);
                    rotation = (rotation * tempRot);

                    collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::ChamferCylinder(
                                    thisWorld,
                                    radius,
                                    height,
                                    rotation, offset));
                    LOG_DEBUG(Logger::CORE, " physical_body 'chamfer_cylinder' in <collisions> erstellt. ");
                }
                else if (typeAsString.compare("mesh") == 0)
                {
                    LOG_MESSAGE(Logger::CORE,
                        " Mesh-Collisions in <collisions> werden momentan noch nicht unterstützt (Eintrag wird ignoriert).");
                    collision.setNull();
                }
/*
                else if (typeAsString.compare("meshhull") == 0)  // automatische convexhull mit daten aus mesh
                {
                    LOG_MESSAGE(Logger::CORE,
                        " Entityhull-Collisions werden momentan noch nicht unterstützt (Eintrag wird ignoriert).");
                    collision.setNull();
                }
*/
                else
                {
                    LOG_MESSAGE(Logger::CORE,
                        " > Parse Error beim Erstellen einer Collision; ungültiger typ: '"+typeAsString+"' !");
                    collision.setNull();
                    //return OgreNewt::CollisionPtr(NULL);
                }

            }


            // Collision dem vektor hinzufügen
            if (!collision.isNull())
                mCollisions.push_back(collision);

            child = child->getNextSibling();
        }
    }

    // Eine Entity
    void DotSceneLoader::processEntity(DOMElement* rootEntityXml,
        SceneManager* sceneManager, Ogre::SceneNode* parentNode,
        Ogre::Real renderingDistance, const std::string &physical_body)
    {
        string entName = XmlHelper::getAttributeValueAsStdString(
            rootEntityXml, "name");
        string meshName = XmlHelper::getAttributeValueAsStdString(
            rootEntityXml, "meshFile");

        Ogre::Entity* newEnt = NULL;

        // Wurde der Entity bisher kein Name zugewiesen
        if (entName.length() == 0)
        {
            entName = getRandomName(mSceneName+"_"+parentNode->getName());
        }

        ResourceGroupManager& resGroupMgr = ResourceGroupManager::getSingleton();
        while(parentNode->getCreator()->hasEntity(entName))
        {
            entName = getRandomName(entName);
        }

        try
        {
            // if this mesh exists in our module's resource group: preload it
            if (resGroupMgr.resourceExists(mResourceGroup, meshName))
            {
                MeshManager::getSingleton().load(meshName, mResourceGroup);
            }

            // If not, it is now loaded implicitly from the default group
            newEnt = sceneManager->createEntity(entName, meshName);
        }
        catch(...)
        {
              LOG_ERROR(Logger::CORE, " Entity '"+meshName+"' mit dem Namen '"+entName+"' konnte nicht geladen werden.");
              return;
        }

        if (parentNode->getScale() != Vector3::UNIT_SCALE)
            newEnt->setNormaliseNormals(true);

        parentNode->attachObject(newEnt);

        LOG_DEBUG(Logger::CORE, " Entity '"+meshName+"' mit dem Namen '"+entName+"' in den Knoten '"+parentNode->getName()+"' eingefügt.");


        // ------- Falls nötig automatisch bodyproxy erstellen -------------
        // (wenn physical_body gesetzt wurde)
        OgreNewt::CollisionPtr collision = OgreNewt::CollisionPtr();
        OgreNewt::World *thisWorld = PhysicsManager::getSingleton()._getNewtonWorld();

        if (physical_body.compare("none") != 0)
        {
            const AxisAlignedBox &aab = newEnt->getMesh()->getBounds();
            Ogre::Vector3 size = (aab.getMaximum() - aab.getMinimum()) * parentNode->getScale();
            bool forceBox = false;

            if ((size.x < PhysicsManager::NEWTON_GRID_WIDTH ||
                 size.y < PhysicsManager::NEWTON_GRID_WIDTH ||
                 size.z < PhysicsManager::NEWTON_GRID_WIDTH) &&
                 physical_body.compare("convexhull") == 0)
            {
                if (size.x < PhysicsManager::NEWTON_GRID_WIDTH)
                    size.x = PhysicsManager::NEWTON_GRID_WIDTH;
                if (size.y < PhysicsManager::NEWTON_GRID_WIDTH)
                    size.y = PhysicsManager::NEWTON_GRID_WIDTH;
                if (size.z < PhysicsManager::NEWTON_GRID_WIDTH)
                    size.z = PhysicsManager::NEWTON_GRID_WIDTH;

                LOG_MESSAGE(Logger::CORE, " Die Entity '"+entName+"' liegt in einer Ebene, verwende 'box' für physical_body '"+physical_body+"' ");
                forceBox = true;
            }
            const Quaternion orientation(0,0,0,0);// = parentNode->getOrientation();
            const Ogre::Vector3 pos = aab.getMinimum()* parentNode->getScale() + (size/2.0);


            // Prüfen, ob schon ein identischer Proxy erstellt wurde um diesen erneut zu verwenden
            AlreadyUsedCollision &aucol (mAutoCreatedCollisions[meshName]);
            if (aucol.Type.compare(physical_body) == 0  &&
                aucol.Scale == parentNode->getScale() &&
                (!forceBox)) // sicherheitshalber
            {
                collision = aucol.ColPtr;
                LOG_DEBUG(Logger::CORE, " Schon früher erstellten physical_body für Entity '"+entName+"' wieder verwendet. ");
            }
            else
            {

                if (physical_body.compare("box") == 0 || forceBox)
                {
                    collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::Box(
                                     thisWorld, size, orientation, pos));
                    LOG_DEBUG(Logger::CORE, " physical_body 'box' für Entity '"+entName+"' erstellt. ");
                }
                else if (physical_body.compare("pyramid") == 0)
                {
                    collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::Pyramid(
                                    thisWorld, size, orientation, pos));
                    LOG_DEBUG(Logger::CORE, " physical_body 'pyramid' für Entity '"+entName+"' erstellt. ");
                }
                else if (physical_body.compare("sphere") == 0)
                {
                    double radius = std::max(size.x, std::max(size.y, size.z)) / 2.0;
                    collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::Ellipsoid(
                                    thisWorld, Vector3(radius, radius, radius),
                                    orientation, pos));
                    LOG_DEBUG(Logger::CORE, " physical_body 'sphere' für Entity '"+entName+"' erstellt. ");
                }
                else if (physical_body.compare("ellipsoid") == 0)
                {
                    // set the size x/z values to the maximum
                    Ogre::Vector3 s(size/2.0);
                    s.x = std::max(s.x, s.z);
                    s.z = s.x;
                    collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::Ellipsoid(
                                    thisWorld, s,
                                    orientation, pos));
                    LOG_DEBUG(Logger::CORE, " physical_body 'ellipsoid' für Entity '"+entName+"' erstellt. ");
                }
                else if (physical_body.compare("capsule") == 0)
                {
                    double radius = std::max(size.x, size.z) / 2.0;
                    double height = size.y;

                    collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::Capsule(
                                    thisWorld,
                                    radius,
                                    height,
                                    orientation, pos));
                    LOG_DEBUG(Logger::CORE, " physical_body 'capsule' für Entity '"+entName+"' erstellt. ");
                }
                else if (physical_body.compare("convexhull") == 0)
                {
                    collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::ConvexHull(
                                    thisWorld,
                                    newEnt,
                                    false));
                    //orientation, pos));
                    LOG_DEBUG(Logger::CORE, " physical_body 'convexhull' für Entity '"+entName+"' erstellt. ");
                }
                else if (physical_body.compare("mesh") == 0 || physical_body.compare("auto"))
                {
                    collision = OgreNewt::CollisionPtr(new OgreNewt::CollisionPrimitives::TreeCollision(
                                    thisWorld, newEnt, false));
                    LOG_DEBUG(Logger::CORE, " physical_body 'mesh' für Entity '"+entName+"' erstellt. ");
                }
                else
                    LOG_MESSAGE(Logger::CORE,
                        " Der bodyproxy_type '"+physical_body+"'(aus userData) der Entity '"+meshName+"' ist ungültig.");

                // proxy in die liste der schon erstellten proxies hinzufügen
                aucol.ColPtr = collision;
                aucol.Scale = parentNode->getScale();
                aucol.Type = physical_body;
            }
        }

        // zur liste hinzufügen
        if (!collision.isNull())
        {
            mCollisions.push_back(collision);
        }


        // Zur Physik des Levels hinzufügen
        if (mCollisions.size() > 0)
        {
            PhysicsManager::getSingleton().addLevelGeometry(newEnt, mCollisions);
            LOG_DEBUG(Logger::CORE, " Entity '"+entName+"' in levelGeometry geladen");
        }


        // wieder aus der liste entfernen, falls mehrere entities hier definiert werden
        if (!collision.isNull())
        {
            mCollisions.pop_back();
        }

        // Renderingdistanz berechnen
        if (renderingDistance == mRenderingDistance)
        {
            Ogre::Real diameter
                = (newEnt->getBoundingBox().getMaximum() - newEnt->getBoundingBox().getMinimum()).length();

            // Gerade mal 10cm² => 10m
            if (diameter <= 0.5)
                renderingDistance = 15;
            // Gerade mal 1,5m² => 25m
            else if (diameter <= 1.5)
                renderingDistance = 30;
            // Gerade mal 2,5m² => 50m
            else if (diameter <= 2.5)
                renderingDistance = 60;
            else if (diameter <= 10)
                renderingDistance = 150;
            else if (diameter <= 50)
                renderingDistance = 250;
            else if (diameter <= 100)
                renderingDistance = 450;
            else
                renderingDistance = 1500;

            newEnt->setRenderingDistance(renderingDistance);
        }
        else
            newEnt->setRenderingDistance(renderingDistance);
        newEnt->setCastShadows(false);
    }

    string DotSceneLoader::getRandomName(const string& baseName)
    {
        int rnd = rand();
        stringstream rval;
        rval << baseName << "_" << rnd;
        return rval.str();
    }

    Ogre::Vector3 DotSceneLoader::processPosition(DOMElement* rootPositionXml)
    {
        LOG_DEBUG(Logger::CORE, " Position gefunden");

        try
        {
            if (XmlHelper::hasAttribute(rootPositionXml, "x") &&
                XmlHelper::hasAttribute(rootPositionXml, "y") &&
                XmlHelper::hasAttribute(rootPositionXml, "z"))
            {
                return Ogre::Vector3(
                    XmlHelper::getAttributeValueAsReal(rootPositionXml, "x"),
                    XmlHelper::getAttributeValueAsReal(rootPositionXml, "y"),
                    XmlHelper::getAttributeValueAsReal(rootPositionXml, "z"));
            }
        }
        catch(...) { }

        LOG_MESSAGE(Logger::CORE, " > Parse Error beim Übernehmen der Position! ");

        return Ogre::Vector3::ZERO;
    }


    Ogre::Vector3 DotSceneLoader::processScale(DOMElement* rootScaleXml)
    {
        LOG_DEBUG(Logger::CORE, " Skalierung gefunden");

        try
        {
            if (XmlHelper::hasAttribute(rootScaleXml, "x") &&
                XmlHelper::hasAttribute(rootScaleXml, "y") &&
                XmlHelper::hasAttribute(rootScaleXml, "z"))
            {
                return Ogre::Vector3(
                    XmlHelper::getAttributeValueAsReal(rootScaleXml, "x"),
                    XmlHelper::getAttributeValueAsReal(rootScaleXml, "y"),
                    XmlHelper::getAttributeValueAsReal(rootScaleXml, "z"));
            }
        }
        catch(...) { }

        LOG_MESSAGE(Logger::CORE, " > Parse Error beim Übernehmen der Skalierung! ");

        return Ogre::Vector3::UNIT_SCALE;
    }

    Ogre::Vector3 DotSceneLoader::processVector(DOMElement* rootScaleXml, bool &error)
    {
        LOG_DEBUG(Logger::CORE, " Vector gefunden");

        try
        {
            if (XmlHelper::hasAttribute(rootScaleXml, "x") &&
                XmlHelper::hasAttribute(rootScaleXml, "y") &&
                XmlHelper::hasAttribute(rootScaleXml, "z"))
            {
                error = false;
                return Ogre::Vector3(
                    XmlHelper::getAttributeValueAsReal(rootScaleXml, "x"),
                    XmlHelper::getAttributeValueAsReal(rootScaleXml, "y"),
                    XmlHelper::getAttributeValueAsReal(rootScaleXml, "z"));
            }
        }
        catch(...) { }

        LOG_MESSAGE(Logger::CORE, " > Parse Error beim Lesen eines Vectors! ");
        error = true;

        return Ogre::Vector3::UNIT_SCALE;
    }

    /// @TODO Sollten drei Möglichkeiten sein...
    Ogre::Quaternion DotSceneLoader::processRotation(DOMElement* rootQuatXml)
    {
        LOG_DEBUG(Logger::CORE, " Rotation gefunden");

        try
        {
            // Durch w,x,y,z definiert
            if (XmlHelper::hasAttribute(rootQuatXml, "qw") &&
                XmlHelper::hasAttribute(rootQuatXml, "qx") &&
                XmlHelper::hasAttribute(rootQuatXml, "qy") &&
                XmlHelper::hasAttribute(rootQuatXml, "qz"))
            {

                return Ogre::Quaternion(
                    XmlHelper::getAttributeValueAsReal(rootQuatXml, "qw"),
                    XmlHelper::getAttributeValueAsReal(rootQuatXml, "qx"),
                    XmlHelper::getAttributeValueAsReal(rootQuatXml, "qy"),
                    XmlHelper::getAttributeValueAsReal(rootQuatXml, "qz"));
            }

            // Durch axisX,axisY,axisZ,angle definiert
            if (XmlHelper::hasAttribute(rootQuatXml, "angle") &&
                XmlHelper::hasAttribute(rootQuatXml, "axisX") &&
                XmlHelper::hasAttribute(rootQuatXml, "axisY") &&
                XmlHelper::hasAttribute(rootQuatXml, "axisZ"))
            {
                return Ogre::Quaternion(
                    Ogre::Degree(XmlHelper::getAttributeValueAsReal(rootQuatXml, "angle")),
                    Ogre::Vector3(
                    XmlHelper::getAttributeValueAsReal(rootQuatXml, "axisX"),
                    XmlHelper::getAttributeValueAsReal(rootQuatXml, "axisY"),
                    XmlHelper::getAttributeValueAsReal(rootQuatXml, "axisZ")));
            }

            // Durch angleX,angleY,angleZ definiert
            if (XmlHelper::hasAttribute(rootQuatXml, "angleX") &&
                XmlHelper::hasAttribute(rootQuatXml, "angleY") &&
                XmlHelper::hasAttribute(rootQuatXml, "angleZ") )
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

        LOG_MESSAGE(Logger::CORE, " > Parse Error beim Übernehmen der Rotation! ");

        return Ogre::Quaternion::IDENTITY;
    }


    void DotSceneLoader::warning(const XERCES_CPP_NAMESPACE::SAXParseException& exc)
    {
        LOG_MESSAGE(Logger::CORE, toString(" warning ", exc));
    }
    void DotSceneLoader::error(const XERCES_CPP_NAMESPACE::SAXParseException& exc)
    {
        LOG_ERROR(Logger::CORE, toString("n error", exc));
        mErrorCount++;
    }
    void DotSceneLoader::fatalError(const XERCES_CPP_NAMESPACE::SAXParseException& exc)
    {
        LOG_CRITICAL(Logger::CORE, toString(" fatal error", exc));
        mErrorCount++;
    }
    void DotSceneLoader::resetErrors()
    {
    }

    std::string DotSceneLoader::toString(const std::string& type,
        const XERCES_CPP_NAMESPACE::SAXParseException& exc) const
    {
        std::stringstream strs;
        strs << "A" << type << " occured while parsing " << mSceneName
             << " at line " << exc.getLineNumber() << " column " <<  exc.getColumnNumber();

        if (exc.getSystemId() != NULL)
            strs << " with system " << XmlHelper::transcodeToStdString(exc.getSystemId());
        if (exc.getPublicId() != NULL)
            strs << " with public " << XmlHelper::transcodeToStdString(exc.getPublicId());

        return strs.str();
    }
}

