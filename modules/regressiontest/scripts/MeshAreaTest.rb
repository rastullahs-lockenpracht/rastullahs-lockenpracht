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
    testObj.getActor().setScale(0.5, 0.2, 1.0)

    listener = MeshAreaListener.new()
    $GameEveMgr.addMeshAreaListener(testObj.getActor(), GT_BOX, listener)
  end
end
