require 'testcase.rb'

class CollisionsCacheTest < TestCase
    def execute()
		MapLoader.new().loadMap("collisionscachetest.rlmap");
	end
end
