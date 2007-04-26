require 'testcase.rb'

class GameObjectDerived < GameObject
end

class MapLoadingTest < TestCase
    def initialize(center)
        super(center)

		loader = MapLoader.new("regressiontest");
        loader.loadMap("regressiontest_maploadingtest.rlmap.xml");
        
		p "--- Built-in GO class, created by MapLoader ---"
        builtIn = GameObjectManager.getSingleton().getGameObject(10)
        p builtIn
        p builtIn
        p builtIn.getActor()

		p "--- Custom GO class, created by MapLoader ---"
        custom = GameObjectManager.getSingleton().getGameObject(11)
        p custom
        p custom
        p custom.getActor()
        
		p "--- Built-in GO class, created by Ruby script ---"
        builtIn2 = GameObjectManager.getSingleton().createGameObject("builtin_go", 10)
        builtIn2.setName(builtIn2.getName() + "_2")
        builtIn2.setPosition([-6, 0, -10.0])
        builtIn2.setState(GOS_IN_SCENE)
        p builtIn2
        p builtIn2
        p builtIn2.getActor()

		p "--- Custom GO class, created by Ruby script ---"
        custom2 = GameObjectManager.getSingleton().createGameObject("custom_go", 11)
        custom2.setName(custom2.getName() + "_2")
        custom2.setPosition([-6, 0, -11.0])
        custom2.setState(GOS_IN_SCENE)
        p custom2
        p custom2
        p custom2.getActor()
    end
end
