require 'testcase.rb'

class GameObjectDerived < GameObject
end

class MapLoadingTest < TestCase
    def initialize(center)
        super(center)

		loader = MapLoader.new("regressiontest");
        loader.loadMap("regressiontest_maploadingtest.rlmap.xml");
        
		p "--- Built-in GO class, created by MapLoader ---"
        builtIn = GameObjectManager.getSingleton().getGameObjectProxy(10)
        p builtIn
        p builtIn.getGameObject()
        p builtIn.getGameObject().getActor()

		p "--- Custom GO class, created by MapLoader ---"
        custom = GameObjectManager.getSingleton().getGameObjectProxy(11)
        p custom
        p custom.getGameObject()
        p custom.getGameObject().getActor()
        
		p "--- Built-in GO class, created by Ruby script ---"
        builtIn2 = GameObjectManager.getSingleton().createGameObjectProxy("builtin_go", 10)
        builtIn2.getGameObject().setName(builtIn2.getGameObject().getName() + "_2")
        builtIn2.getGameObject().setPosition([-6, 0, -10.0])
        builtIn2.getGameObject().setState(GOS_IN_SCENE)
        p builtIn2
        p builtIn2.getGameObject()
        p builtIn2.getGameObject().getActor()

		p "--- Custom GO class, created by Ruby script ---"
        custom2 = GameObjectManager.getSingleton().createGameObjectProxy("custom_go", 11)
        custom2.getGameObject().setName(custom2.getGameObject().getName() + "_2")
        custom2.getGameObject().setPosition([-6, 0, -11.0])
        custom2.getGameObject().setState(GOS_IN_SCENE)
        p custom2
        p custom2.getGameObject()
        p custom2.getGameObject().getActor()
    end
end
