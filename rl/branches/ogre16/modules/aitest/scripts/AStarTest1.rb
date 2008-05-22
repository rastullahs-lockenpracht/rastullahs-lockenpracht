class AStarTest1
    def initialize(center)
	@WPGraph = $WPGM.createWayPointGraph();
    end
    def save()
	@WPGraph.save("aitest/maps/astargrid.wpg")
    end
    def load()
	@WPGraph.load("aitest/maps/astargrid.wpg")
    end
    def create()
	# this function defines a waypoint grid
	height = 0.5;
	xcoord = -9
	zcoord = -9
	xnr = 19
	znr = 19

	# waypoints
	xnr.times do
	    zcoord = -9
	    znr.times do
		@WPGraph.addWayPoint([xcoord, height, zcoord],
				     WayPointNode::WP_EXTERIOR);
	    	zcoord += 1
	    end
	    xcoord += 1
	end

	# connections (gridlike)
	
	znr -= 1
	xcoord = 0
	indexx = 0
	xoffset = 19
	xnr.times do
	    znr.times do
		@WPGraph.addConnection(@WPGraph.getWayPointAt(indexx),
				       @WPGraph.getWayPointAt(indexx+1));
		indexx += 1
	    end
	    xcoord += 1
	    indexx = xcoord * xoffset
	end

	xnr -= 1
	znr += 1
	zcoord = 0
	indexz = 0
	zoffset = 19
	znr.times do
	    xnr.times do
 	        @WPGraph.addConnection(@WPGraph.getWayPointAt(indexz),
		         	       @WPGraph.getWayPointAt(indexz+zoffset));
		indexz += zoffset
	    end
	    zcoord += 1
	    indexz = zcoord
	end
    end
    def initastar()
	test = Ogrevec3Vector.new()
	@astarcosts = AStarCostsDefault.new(ManhattanDistance.new(), 1.001)
	@astar = AStar.new(@astarcosts, @WPGraph)
	# [9.5, 0.5, 9.5], [5, 0.4, -10])
	@astar.search(test);
    end
    def calc()
	test = Ogrevec3Vector.new()
	@astar.search(test);
    end
    def calcFromTo(p)
	vec1 = [p[0], 0.5, p[1]]
	vec2 = [p[2], 0.5, p[3]]
	test = Ogrevec3Vector.new()
	@astar.searchFromTo(test, vec1, vec2)
    end
end

