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

class UseTorchAction < Action
  def initialize
    super("use", "Benutzen")
  end

  def doAction(torch, player, target)
    RulesSubsystem.getSingleton().getQuestBook().getQuest("hoehleZeugFackel").setState(Quest::COMPLETED)
    player.getActor().attachToSlotAxisRot( torch.getActor(), "Bone15", "SLOT_HANDLE", [0.0, 0.0, 0.0], [ 1.0, 0.0, 0.0 ], 90.0 );
    torch.setLit(true); # Licht an
  end
end

class Torch < Item
  def initialize(id)
    super(id)
  end
  
  def placeIntoScene()
    super()
    if (@fackellicht == nil)
      @fackellicht = $AM.createLightActor("Das Licht der Fackel", LightObject::LT_POINT );
      @fackellicht.getControlledObject().setCastShadows(false);
      @fackellicht.getControlledObject().setDiffuseColour(1.0,0.8,0.0);
      @fackellicht.getControlledObject().setAttenuation(5.0, 0.1,  0.005, 0.0 );
      
      getActor().attachToSlot( @fackellicht, "SLOT_FAR_END" );    
      
      $SCRIPT.log("actor gesetzt");
      
      setLit(false);
      
      addAction(UseTorchAction.new);
      $SCRIPT.log("Aktion hinzugefuegt.");
    end
  end
  
  def setLit(lit)
    @fackellicht.setVisible(lit)
    @lit = lit;
  end
  
  def isLit()
    return @lit;
  end
end
