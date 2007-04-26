require 'testcase.rb'

class MapLoadingTest < TestCase
    def initialize(center)
        super(center)

	loader = MapLoader.new("regressiontest");
        loader.loadMap("regressiontest_lights.rlmap.xml");
    end
end
