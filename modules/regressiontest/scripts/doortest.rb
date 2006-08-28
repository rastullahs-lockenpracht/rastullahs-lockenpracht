require 'util/vector_util.rb'
require 'testcase.rb'
require 'door.rb'
require 'switch.rb'

class DoorOpener < ObjectStateChangeListener

  def initialize(door, switch)
    super()
    @door = door
    @switch = switch
    @switch.addObjectStateChangeListener(self)
  end

  def objectStateChanged(event)
      doorOpen = @switch.getState() == Switch::STATE_OBEN
      if (@door.isOpen() != doorOpen)
        if (doorOpen)
          @door.doAction("opendoor")
        else
          @door.doAction("closedoor")
        end
      end
  end
end

class DoorTest < TestCase
    def initialize(center)
        super(center)

        door1 = Door.new("DoorTest_Door1", "Eine Holztuer",
            rel_pos([0.0, 0.0, 2.0]), [0.0,0.0,0.0])
        door1.addActions(false, true)

        door2 = Door.new("DoorTest_Door2",
            "Eine Holztuer.\nDiese Tuer hat weder Klinke noch Schloesser",
            rel_pos([0.0, 0.0, -2.0]), [0.0,0.0,0.0])
        door2.addActions(false, false)

        switch = Switch.new( "DoorTest_Hebel" );
        switch.getActor().placeIntoScene(rel_pos([1.0, 0.25, -2.0]))

        doorprocessor = DoorOpener.new(door2, switch)

        $SCRIPT.log("Truhe reinsetzen")
    end
end

