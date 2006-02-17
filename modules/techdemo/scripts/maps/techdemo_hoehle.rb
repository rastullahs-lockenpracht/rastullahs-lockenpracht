require "door.rb"
require "switch.rb"
require "heiltrank_tech.rb"


trank = Heiltrank.new( "Trank", "Eine Flasche mit einer geheimnisvollen Fluessigkeit", "obj_heiltrank01.mesh", "A" );
trank.getActor().placeIntoScene( 155.593017578125, 6.74539489746094, 20.964052734375, 1.0, 0.0, 0.0, 0.0 );

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
    torch.getActor().getChildBySlotAndIndex(Slots.SLOT_FAR_END, 0).setVisible(true); # Licht an
  end
end

class Torch < Item
  def initialize(name)
    super("Fackel", "Handelsuebliche Fackel zum Beleuchten der Umgebung");

    torchActor = $AM.createMeshActor( name, "ins_fackel_01.mesh" );
    $SCRIPT.log("actor erstellt.");

    fackellicht = $AM.createLightActor("Das Licht der Fackel", LightObject::LT_POINT );
    fackellicht.getControlledObject().setCastShadows(false);
    fackellicht.getControlledObject().setDiffuseColour(1.0,0.8,0.0);
    fackellicht.getControlledObject().setAttenuation(5.0, 0.1,  0.005, 0.0 );
    fackellicht.getControlledObject().setActive(false)
    torchActor.attachToSlot( fackellicht, "SLOT_FAR_END" );    
    
    setActor(torchActor);
    $SCRIPT.log("actor gesetzt");
    
    @lit = false;
    
    addAction(UseTorchAction.new);
    $SCRIPT.log("Aktion hinzugefuegt.");
  end
  
  def setLit(lit)
    fackellicht.setVisible(lit)
    @lit = lit;
  end
  
  def lit?
    @lit;
  end
end
