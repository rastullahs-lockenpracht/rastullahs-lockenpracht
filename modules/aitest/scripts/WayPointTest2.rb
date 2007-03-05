

class WayPointTest2 
    def initialize(center)
	@WPGraph = $WPGM.createWayPointGraph();
    end
    def save()
	@WPGraph.save("modules/aitest/maps/graphtest.wpg")
    end
    def load()
	@WPGraph.load("modules/aitest/maps/graphtest.wpg")
    end
end

