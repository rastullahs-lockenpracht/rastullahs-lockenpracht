

class WayPointTest2 
    def initialize(center)
	@WPGraph = $WPGM.createWayPointGraph();
    end
    def save()
	@WPGraph.save("graphtest.wpg")
    end
    def load()
	@WPGraph.load("graphtest.wpg")
    end
end

