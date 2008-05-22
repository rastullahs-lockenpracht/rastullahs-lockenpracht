

class WayPointTest2 
    def initialize(center)
	@WPGraph = $WPGM.createWayPointGraph();
    end
    def save()
	@WPGraph.save("aitest/maps/graphtest.wpg")
    end
    def load()
	@WPGraph.load("aitest/maps/graphtest.wpg")
    end
end

