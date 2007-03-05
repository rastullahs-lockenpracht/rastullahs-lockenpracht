require 'testcase.rb'

class WallSet1 < TestCase
    def initialize(center)
        super(center)

        height = 1.0
        min_base = [-3.0, 0.05, 0]
        max_base = [3.0, height, 0.3]
        base = $AM.createBoxPrimitiveActor("WallSet1_1",
                                           min_base, max_base, "alpha_blue")
        base.placeIntoScene(center);

	min_base = [-10.0, 0.05, 0]
	max_base = [-4, height, 0.3]
	base = $AM.createBoxPrimitiveActor("WallSet1_2",
                                           min_base, max_base, "alpha_blue")
        base.placeIntoScene(center);

	min_base = [4.0, 0.05, 0]
	max_base = [10.0, height, 0.3]
	base = $AM.createBoxPrimitiveActor("WallSet1_3",
                                           min_base, max_base, "alpha_blue")
        base.placeIntoScene(center);

	min_base = [10.0, 0.05, 2]
	max_base = [4, height, 2.3]
	base = $AM.createBoxPrimitiveActor("WallSet1_4",
                                           min_base, max_base, "alpha_blue")
        base.placeIntoScene(center);

	min_base = [-10.0, 0.05, 2]
	max_base = [3, height, 2.3]
	base = $AM.createBoxPrimitiveActor("WallSet1_5",
                                           min_base, max_base, "alpha_blue")


        base.placeIntoScene(center);

	min_base = [4.0, 0.05, 0.0]
	max_base = [4.3, height, -7.0]
	base = $AM.createBoxPrimitiveActor("WallSet1_6",
                                           min_base, max_base, "alpha_blue")
        base.placeIntoScene(center);

	min_base = [4.0, 0.05, -8.0]
	max_base = [4.3, height, -10.0]
	base = $AM.createBoxPrimitiveActor("WallSet1_7",
                                           min_base, max_base, "alpha_blue")
        base.placeIntoScene(center);

    end
end
