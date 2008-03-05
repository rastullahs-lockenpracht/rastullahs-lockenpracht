require 'testcase.rb'

class GameObjectDerived < GameObject
end

class MapLoadingTest < TestCase
    def execute()
        loader = MapLoader.new("regressiontest");
        loader.loadMap("regressiontest_maploadingtest.rlmap.xml");
        
		# --- Built-in GO class, created by MapLoader ---
        builtIn = GameObjectManager.getSingleton().getGameObject(10)

		# --- Built-in GO class, created by Ruby script ---"
        builtIn2 = GameObjectManager.getSingleton().createGameObject("builtin_go", 10)
        builtIn2.setName(builtIn2.getName() + "_2")
        builtIn2.setPosition([-6, 0, -10.0])
        builtIn2.setState(GOS_IN_SCENE)
        assertEquals(builtIn, builtIn2)
        assertEquals(builtIn.getActor(), builtIn2.getActor())

		# --- Custom GO class, created by MapLoader ---
        custom = GameObjectManager.getSingleton().getGameObject(11)
        
		# "--- Custom GO class, created by Ruby script ---"
        custom2 = GameObjectManager.getSingleton().createGameObject("custom_go", 11)
        custom2.setName(custom2.getName() + "_2")
        custom2.setPosition([-6, 0, -11.0])
        custom2.setState(GOS_IN_SCENE)
        assertEquals(custom, custom2)
        assertEquals(custom.getActor(), custom2.getActor())
		
		$SCRIPT.log("MapLoadingTest initialisiert")
    end
end
