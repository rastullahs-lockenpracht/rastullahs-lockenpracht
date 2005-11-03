require "door.rb"
require "switch.rb"

class CaveDoorOpener < ObjectStateChangeListener

  def initialize(door, switch)
    super()
    @mDoor = door
    @mSwitch = switch
    @mSwitch.addObjectStateChangeListener(self)
  end

  def objectStateChanged(event)
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
    player.getActor().attachToSlotAxisRot( torch.getActor(), "Bone15", "SLOT_HANDLE", [0.0, 0.0, 0.0], [ 1.0, 0.0, 0.0 ], 90.0 );
    torch.getActor().getChildBySlotAndIndex(Slots.SLOT_FAR_END, 0).activate(); # Licht an
  end
end

class Torch < Item
  def initialize(name)
    super("Fackel", "Handelsuebliche Fackel zum Beleuchten der Umgebung");

    torchActor = $AM.createMeshActor( name, "ins_fackel_01.mesh" );
    $CORE.log("actor erstellt.");

    fackellicht = $AM.createLightActor("Das Licht der Fackel", LightObject::LT_POINT );
    fackellicht.getControlledObject().setCastShadows(false);
    fackellicht.getControlledObject().setDiffuseColour(1.0,0.8,0.0);
    fackellicht.getControlledObject().setAttenuation(500.0, 1.0,  0.005, 0.0 );
    torchActor.attachToSlot( fackellicht, "SLOT_FAR_END" );    
    
    setActor(torchActor);
    $CORE.log("actor gesetzt");
    
    @lit = false;
    
    addAction(UseTorchAction.new);
    $CORE.log("Aktion hinzugefuegt.");
  end
  
  def setLit(lit)
    @lit = lit;
  end
  
  def lit?
    @lit;
  end
end
