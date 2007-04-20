require 'testcase.rb'

class MapLoadingTest < TestCase
    def initialize(center)
        super(center)

        MapLoader.new().loadMap("regressiontest_lights.rlmap.xml", "regressiontest");
    end
end
