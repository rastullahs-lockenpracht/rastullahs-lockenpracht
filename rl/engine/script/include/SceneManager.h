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

#ifndef __RlSceneManager_H__
#define __RlSceneManager_H__

#include "ScriptPrerequisites.h"

#include "Properties.h"

namespace rl 
{

    class Scene;

    class _RlScriptExport SceneManager 
        : public Ogre::Singleton<SceneManager>, public Ogre::ScriptLoader//, public PropertyHolder
    {
    public:
        SceneManager();
        ~SceneManager();

        void addScene(Scene* scene);
        void loadScene(const CeGuiString& name, bool saveCurrent = true);

        const Ogre::StringVector& getScriptPatterns() const;
        void parseScript(Ogre::DataStreamPtr& data, const Ogre::String& resourceGroup);
        Ogre::Real getLoadingOrder() const;

        /*virtual const Property getProperty(const CeGuiString& key) const;
        virtual void setProperty(const CeGuiString& key, const Property& value);
        virtual PropertyKeys getAllPropertyKeys() const;*/

    private:
        std::map<CeGuiString, Scene*> mScenes;
        Ogre::StringVector mScriptPatterns;
        Scene* mCurrentScene;
        std::map<Scene*, PropertyRecordPtr> mSceneStates;
    };

}

#endif // __RlSceneManager_H__
