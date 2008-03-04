require 'util/vector_util.rb'
require 'testcase.rb'

class SelectorTest < TestCase

    @@radius = 2.5
    
    def execute()
        min_base = [-@@radius, 0.05, -@@radius]
        max_base = [@@radius, 0.3, @@radius]
        base = $AM.createBoxPrimitiveActor("SelectorTestBase",
                                           min_base, max_base, "alpha_blue")
        base.placeIntoScene(getCenter());

		go1 = $GOM.createGameObject("SelectorTest")
		go1.setPosition(rel_pos([0.0, 0.4, 0.0]))
		go1.placeIntoScene()
		go2 = $GOM.createGameObject("SelectorTest")
		go2.setPosition(rel_pos([2.0, 0.4, 0.0]))
		go2.placeIntoScene()

		testRaySelector()
		testHalfSphereSelector()
        
        $SCRIPT.log("SelectorTest successful.");
        
    end
	
	def testRaySelector()
		testObj = RaySelector.new(QUERYFLAG_ITEM)
		testObj.setRay(rel_pos([-0.5, 0.5, 0.0]), rel_pos([1.0, 0.5, 0.0]))
		testObj.updateSelection()
		assertEquals(1, testObj.getSelectionCount(), "One element ray selection")
		
		testObj2 = RaySelector.new(QUERYFLAG_ITEM)
		testObj2.setRay(rel_pos([-0.5, 0.5, 0.0]), rel_pos([2.5, 0.5, 0.0]))
		testObj2.updateSelection()
		assertEquals(2, testObj2.getSelectionCount(), "Two element ray selection")
	end
  
	def testHalfSphereSelector()
		testObj = HalfSphereSelector.new(QUERYFLAG_ITEM)
		testObj.setPosition(rel_pos([0.0, 0.5, 0.0]))
		testObj.setRadius(1)
		testObj.updateSelection()
		assertEquals(1, testObj.getSelectionCount(), "One element sphere selection")
		
		testObj2.setPosition(rel_pos([0.0, 0.5, 0.0]))
		testObj2.setRadius(3)
		testObj2.updateSelection()
		assertEquals(2, testObj2.getSelectionCount(), "Two element sphere selection")
	end
  
end
