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

#ifndef __DotSceneLoader_H__
#define __DotSceneLoader_H__

#include "CorePrerequisites.h"

#ifdef __APPLE__
#   include <OgreNewt/OgreNewt.h>
#else
#   include <OgreNewt.h>
#endif

#include <string>
#include <map>

#include "XmlResourceManager.h"
#include "XmlPropertyReader.h"

namespace Ogre {
	class SceneNode;
}

namespace XERCES_CPP_NAMESPACE {
	class DOMElement;
}

namespace rl {

	/** Diese Klasse parst eine .scene.xml Datei.
	 *  
	 *  Zur Zeit sind folgenende Dinge (partiell) unterstützt 
	 *   - Nodes ( Name, Hierarchie + Platzierung + Skalierung + Rotation )
	 *   - Entities ( Name, TriMeshPhysik )
	 */
    class DotSceneLoader : private XmlPropertyReader
	{
	public:
		/// Erstellt einen Dotscene Loader, der das gewünschte File einliest
        DotSceneLoader(const std::string& filename, const std::string& resourceGroup);
		/// Standard Destruktor
		virtual ~DotSceneLoader();

		/// Laden der Szene
		void initializeScene(Ogre::SceneManager* sceneManager);

	private:
        struct NodeUserData
        {
            std::string physical_body;
			Ogre::Real renderingdistance;
			int staticgeom_group;
            bool is_dynamic;
			bool is_inheriting;
			OgreNewt::CollisionPtr collision;
        };

		// wird verwendet um einen Zeiger auf eine vorherige identische
        // Collision zu erhalten
        struct AlreadyUsedCollision
        {
        public:
            std::string Type;
            Ogre::Vector3 Scale;
            OgreNewt::CollisionPtr ColPtr;
        };


		/// Geht alle Nodes in der .scene durch
		void processNodes(XERCES_CPP_NAMESPACE::DOMElement* rootNodesXml, 
			Ogre::SceneManager* sceneManager,
			Ogre::SceneNode* parentNode );
		/// Node und alle Unterelemente
		void processNode(XERCES_CPP_NAMESPACE::DOMElement* rootNodeXml, 
			Ogre::SceneManager* sceneManager,
			Ogre::SceneNode* parentNode, NodeUserData* parentUserData );
		/// Eine Entity+Attribute
		void processEntity( XERCES_CPP_NAMESPACE::DOMElement* rootEntityXml, 
			Ogre::SceneManager* sceneManager, Ogre::SceneNode* parentNode, 
			Ogre::Real renderingDistance, const std::string &bodyproxy_type );
        /// Ein benutzerdefinierter Bereich im Node
        void processNodeUserData( XERCES_CPP_NAMESPACE::DOMElement* rootUserDataXml, 
			NodeUserData* userData );
		/// Ein benutzerdefinierter Bereich in der Szene
		void processSceneUserData( XERCES_CPP_NAMESPACE::DOMElement* rootUserDataXml );

		/** Liest einen Vector aus einem XML Element, über die Attribute x, y, z
		 *  Sollten die Attribute nicht korrekt definiert sein, gibt es Vector::ZERO zurück (0,0,0)
		 */
		Ogre::Vector3 processPosition( XERCES_CPP_NAMESPACE::DOMElement* rootPositionXml );

		/** Liest einen Vector aus einem XML Element, über die Attribute x, y, z
		*  Sollten die Attribute nicht korrekt definiert sein, gibt es Vector::UNIT_SCALE zurück (1,1,1)
		*/
		Ogre::Vector3 processScale( XERCES_CPP_NAMESPACE::DOMElement* rootPositionXml );
		/** Liest einen Vector aus einem XML Element, über die Attribute x, y, z
		*  Sollten die Attribute nicht korrekt definiert sein, wird error auf true gesetzt.
		*/
		Ogre::Vector3 processVector( XERCES_CPP_NAMESPACE::DOMElement* rootPositionXml, bool &error );
		/** Liest ein Quaternion aus einem XML Element, 
		*  über die Attribute qw, qx, qy, qz  ODER angle, axisX, axisY, axisZ
		*  Sollten die Attribute nicht korrekt definiert sein, gibt es Quaternion::IDENTITY zurück (1,0,0,0)
		*/
		Ogre::Quaternion processRotation( XERCES_CPP_NAMESPACE::DOMElement* rootQuatXml );
		/** Liest eine Liste von ein bis mehreren Bodyproxies (Collisions)
		* SceneUserData ein.
		* Sollten die Attribute falsch gesetzt sein, gibt es NULL zurück; Dadurch wird keine Collision verwendet
		*/
		void processCollisions( XERCES_CPP_NAMESPACE::DOMElement* rootCollisionXml );

        std::string getRandomName(const std::string& baseName);
        /// Builds a string from a xerces exception
        std::string toString( const std::string& type, const XERCES_CPP_NAMESPACE::SAXParseException& exc ) const;

		/// Der Node der Scene
		Ogre::SceneNode* mSceneNode;
		/// Alle statischen GeometrieNodes
		std::map<int,Ogre::SceneNode*> mStaticNodes;

		/// Alle bodyproxies, die schon automatisch erstellt worden sind
		typedef std::map<const std::string,AlreadyUsedCollision>
			USEDCOLLISIONSMAP;
		USEDCOLLISIONSMAP mAutoCreatedCollisions;
        std::vector<OgreNewt::CollisionPtr> mCollisions;

		Ogre::Real mRenderingDistance;
		std::map<int,Ogre::Real> mStaticgeomRenderingDistances;
		std::map<int,Ogre::Real> mStaticgeomBatchSizes;
		
		/// Der Name der Scene
		const std::string mSceneName;
        /// ResourceGroup der dotscene-Resource
        const std::string mResourceGroup;

        /// Have any errors occured
        int mErrorCount;

        /// The Ressource
        XmlPtr mRessource;
        // The Parser
        XERCES_CPP_NAMESPACE::XercesDOMParser* mParser;
	};

}

#endif
