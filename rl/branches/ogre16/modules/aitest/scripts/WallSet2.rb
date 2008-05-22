require 'testcase.rb'

class WallSet2 < TestCase
    def initialize(center)
        super(center)

        height = 1.0
        min_base = [-10.0, 0.05, -10.0]
        max_base = [10.0, height, -10.3]
        base = $AM.createBoxPrimitiveActor("WallSet2_1",
                                           min_base, max_base, "white")
        base.placeIntoScene(center);

	min_base = [-10.0, 0.05, 10.0]
	max_base = [10.0, height, 10.3]
	base = $AM.createBoxPrimitiveActor("WallSet2_2",
                                           min_base, max_base, "white")
        base.placeIntoScene(center);

	min_base = [-10.0, 0.05, -10.0]
	max_base = [-10.3, height, 10.0]
	base = $AM.createBoxPrimitiveActor("WallSet2_3",
                                           min_base, max_base, "white")
        base.placeIntoScene(center);

	min_base = [10.0, 0.05, -10.0]
	max_base = [10.3, height, 10.0]
	base = $AM.createBoxPrimitiveActor("WallSet2_4",
                                           min_base, max_base, "white")


        base.placeIntoScene(center);

    end
end
