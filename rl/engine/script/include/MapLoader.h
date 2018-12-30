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
#ifndef __MapLoader_H__
#define __MapLoader_H__

#include "ScriptPrerequisites.h"

#include "ContentLoader.h"
#include "CoreMessages.h"
#include "MessagePump.h"
#include "XmlProcessor.h"
#include <list>

namespace rl
{

    typedef MessageType<RLMSG_SCENE_CHANGE_REQUESTED, Ogre::StringVector> MessageType_SceneChangeRequested;

    class AbstractMapNodeProcessor;
    class ProgressWindow;

    class _RlScriptExport MapLoader : public ContentLoader, private XmlProcessor
    {
    public:
        static const CeGuiString PROPERTY_ACTIVEMAPS;

        MapLoader();
        MapLoader(const Ogre::String& resourceGroup);
        ~MapLoader();

        /** Loads a Rastullahs Lockenpracht map
         *
         * @param mapresource the name of the map file resource
         * @param loadGameObjects set <code>false</code> if the MapLoader should not load the GameObjects of the map
         * file (e.g. when loading a saved game)
         */
        void loadContent();
        void unloadContent();
        void setDefaultMaps(Ogre::StringVector maps);
        void loadMap(const Ogre::String& mapresource, bool loadGameObjects = true);
        void loadScene(Ogre::StringVector mapsresources, bool loadGameObjects = true);
        void unloadAllMaps(bool removeGameObjects); /// @todo
        void requestSceneChange(Ogre::StringVector mapsresources);

        const CeGuiString getClassName() const;

        const Property getProperty(const CeGuiString& key) const;
        void setProperty(const CeGuiString& key, const Property& value);

        PropertyKeys getAllPropertyKeys() const;

    private:
        std::list<AbstractMapNodeProcessor*> mNodeProcessors;

        Ogre::SceneNode* mRootSceneNode;
        /// Alle statischen GeometrieNodes
        Ogre::String mResourceGroup;
        ProgressWindow* mPercentageWindow;

        Ogre::StringVector mLoadedMaps;
        Ogre::StringVector mPrevLoadedMaps;
        Ogre::StringVector mDefaultMaps;

        void setRootSceneNode(Ogre::SceneNode* node);
        void processSceneNodes(const tinyxml2::XMLElement* nodesElem, bool loadGameObjects);
        void setLoadingPercentage(Ogre::Real percentage, const Ogre::String& text = "");
        bool changeScene(Ogre::StringVector mapsresources);
        void initialize(const Ogre::String& resourceGroup);

        MessagePump::ScopedConnection RequestedSceneChangeConnection;
    };

} // namespace rl

#endif //__MapLoader_H__
