

class WayPointTest 
    def initialize(center)
	
	height = 0.5;

	@WPGraph = $WPGM.createWayPointGraph();
	@WPGraph.addWayPoint([3, height, -8.5], WayPointNode::WP_EXTERIOR);
	@WPGraph.addWayPoint([3, height, -4], WayPointNode::WP_EXTERIOR);
	@WPGraph.addWayPoint([3, height, 0], WayPointNode::WP_EXTERIOR);
	@WPGraph.addWayPoint([3, height, 1], WayPointNode::WP_EXTERIOR);
	@WPGraph.addWayPoint([3, height, 5], WayPointNode::WP_EXTERIOR);
	@WPGraph.addWayPoint([3, height, 9.5], WayPointNode::WP_EXTERIOR);

	@WPGraph.addWayPoint([4, height, -10], WayPointNode::WP_EXTERIOR);
	@WPGraph.addWayPoint([4, height, -7], WayPointNode::WP_EXTERIOR);
	@WPGraph.addWayPoint([4, height, -1], WayPointNode::WP_EXTERIOR);
	@WPGraph.addWayPoint([4, height, 2], WayPointNode::WP_EXTERIOR);
	@WPGraph.addWayPoint([4, height, 4], WayPointNode::WP_EXTERIOR);
	@WPGraph.addWayPoint([4, height, 7], WayPointNode::WP_EXTERIOR);

	@WPGraph.addWayPoint([5, height, -6], WayPointNode::WP_EXTERIOR);
	@WPGraph.addWayPoint([5, height, -3], WayPointNode::WP_EXTERIOR);
	@WPGraph.addWayPoint([5, height, 1], WayPointNode::WP_EXTERIOR);
	@WPGraph.addWayPoint([5, height, 5], WayPointNode::WP_EXTERIOR);
	@WPGraph.addWayPoint([5, height, 6], WayPointNode::WP_EXTERIOR);
	@WPGraph.addWayPoint([5, height, 9.5], WayPointNode::WP_EXTERIOR);

	@WPGraph.addWayPoint([6, height, -9], WayPointNode::WP_EXTERIOR);
	@WPGraph.addWayPoint([6, height, -2], WayPointNode::WP_EXTERIOR);
	@WPGraph.addWayPoint([6, height, -1], WayPointNode::WP_EXTERIOR);
	@WPGraph.addWayPoint([6, height, 4], WayPointNode::WP_EXTERIOR);
	@WPGraph.addWayPoint([6, height, 8], WayPointNode::WP_EXTERIOR);
	@WPGraph.addWayPoint([6, height, 9.5], WayPointNode::WP_EXTERIOR);

	@WPGraph.addWayPoint([7, height, -8], WayPointNode::WP_EXTERIOR);
	@WPGraph.addWayPoint([7, height, -4], WayPointNode::WP_EXTERIOR);

	number = 4
	inner = 5
	count = 0
	outercount = 0

	number.times do
		count = outercount * 6
		inner.times do
			wp2 = count + 1
			@WPGraph.addConnection(@WPGraph.getWayPointAt(count),
					       @WPGraph.getWayPointAt(wp2));
			count += 1
		end
		outercount += 1
	end

	number -= 1
	count = 0
	outercount = 0
	outer = 6
	
	outer.times do
		count = outercount
		number.times do
			wp2 = count + outer
			@WPGraph.addConnection(@WPGraph.getWayPointAt(count),
					       @WPGraph.getWayPointAt(wp2));
			count += outer
		end
		outercount += 1
	end

    end
    def save()
	@WPGraph.save("aitest/maps/graphtest.wpg")
    end
    def load()
	@WPGraph.load("aitest/maps/graphtest.wpg")
    end
#def clear()
#	@WPGraph.
end

