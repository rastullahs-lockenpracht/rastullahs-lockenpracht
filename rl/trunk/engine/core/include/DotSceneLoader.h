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

#ifndef __DotSceneLoader_H__
#define __DotSceneLoader_H__

#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>

#include <OgreSceneNode.h>
#include <string>

namespace rl {

	class DotSceneLoader
	{
	public:
		DotSceneLoader(const std::string & filename);
		~DotSceneLoader() {};

		static std::string getNextEntityName( const std::string& baseName, const std::string& nodeName );
	private:
		void initializeScene();
		XERCES_CPP_NAMESPACE::DOMDocument* openSceneFile();

		void processNodes(XERCES_CPP_NAMESPACE::DOMElement* rootNodesXml, Ogre::SceneNode* parentNode );
		void processNode(XERCES_CPP_NAMESPACE::DOMElement* rootNodeXml, Ogre::SceneNode* parentNode );
		void processEntity( XERCES_CPP_NAMESPACE::DOMElement* rootEntityXml, Ogre::SceneNode* parentNode );

		Ogre::Vector3 processVector( XERCES_CPP_NAMESPACE::DOMElement* rootPositionXml );
		Ogre::Quaternion processRotation( XERCES_CPP_NAMESPACE::DOMElement* rootQuatXml );

		std::string m_SceneName;
		Ogre::SceneManager* m_SceneManager;
	};

}

#endif
