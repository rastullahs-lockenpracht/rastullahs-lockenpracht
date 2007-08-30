require 'testcase.rb'

class MeshAreaListener < GameAreaListener
    def initialize()
        super()
    end

    def areaEntered( anEvent )
        print "Area Entered"
    end
    
    def areaLeft( anEvent )
        print "Area Left"
    end
end

class MeshAreaTest < TestCase
  def execute()
    testObj = $GOM.createGameObject("MeshAreaTestHouse")
    testObj.setPosition(@center)
    testObj.placeIntoScene()

    listener = MeshAreaListener.new()
    $GameEveMgr.addMeshAreaListener(testObj.getActor(), GT_CONVEXHULL, listener)
  end
end
