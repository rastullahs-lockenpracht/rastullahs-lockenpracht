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

namespace rl 
{
	class ScriptSubsystem
	{
	public:
		static rl::ScriptSubsystem& getSingleton();

		void log(const rl::CeGuiString& message);
        void logError(const rl::CeGuiString& message);
		void setTriggerFactory(rl::TriggerFactory* factory);
	};
	  
    class MapLoader
    {
    public:
        MapLoader(const Ogre::String& resourceGroup);
        void loadMap(const Ogre::String& mapresource, bool loadGameObjects = true);
    };
 
	%feature("director") TriggerFactory;    
    class TriggerFactory
	{
	public:
		virtual ~TriggerFactory();
		virtual rl::Trigger* createTrigger(const Ogre::String& classname) = 0;
	};

	%feature("director") UnifiedFactory;    
    class UnifiedFactory :
        public rl::TriggerFactory, 
        public rl::EffectFactory, 
        public rl::GameObjectFactory,
        public rl::BehaviourFactory
    {
    public:
        virtual ~UnifiedFactory() {}

		virtual rl::Trigger* createTrigger(const Ogre::String& classname) = 0;
        virtual rl::Effect* createEffect(const Ogre::String& name, int stufe) = 0;
        virtual rl::GameObject* createRubyGameObject(const Ogre::String& classname, unsigned int id) = 0;
        virtual rl::SteeringBehaviour* createBehaviour(const Ogre::String& classname) = 0;
    };

}