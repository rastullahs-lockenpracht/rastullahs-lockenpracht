require "door.rb"
require "switch.rb"
require "heiltrank_tech.rb"


class CaveDoorOpener < ObjectStateChangeListener

  def initialize(door, switch)
    super()
    @mDoor = door
    @mSwitch = switch
    @mSwitch.addObjectStateChangeListener(self)
  end

  def objectStateChanged(event)
   RulesSubsystem.getSingleton().getQuestBook().getQuest("hoehleZeugHebel").setState(Quest::COMPLETED)
     if ( @mSwitch.getState() == Switch::STATE_OBEN )
        doorOpen = true
      else
        doorOpen = false
      end

      if (@mDoor.isOpen() != doorOpen)
        if (doorOpen)
          @mDoor.doAction("opendoor")
        else
          @mDoor.doAction("closedoor")
        end
      end
  end
end

