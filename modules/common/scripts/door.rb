load "embed.rb"
require 'actorupdateanimationlistener.rb'

class OpenDoorAction < Action
  def initialize
    super("opendoor", "Tür öffnen");
  end
  
  # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
  def canDo(door, user)    
    not door.isOpen
  end
  
  def doAction(door, user, target)    
    doorActor = door.getActor(); 
    doorActor.getControlledObject().replaceAnimation("zu", "auf", 1.0, 1);
    animListener = ActorUpdateAnimationListener.new();
    anim = doorActor.getControlledObject().getAnimation("auf");
    anim.addAnimationListener(animListener);

    knarzActor = door.getSoundActor();
    knarzActor.getControlledObject().play();
    door.setOpen(true);
  end
end

class CloseDoorAction < Action
  def initialize
    super("closedoor", "Tür schließen");
  end
  
  # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
  def canDo(door, user)    
    door.isOpen
  end
  
  def doAction(door, user, target)    
    doorActor = door.getActor();
    doorActor.getControlledObject.replaceAnimation("auf", "zu", 1.0, 1); 
    animListener = ActorUpdateAnimationListener.new();
    anim = doorActor.getControlledObject().getAnimation("zu");
    anim.addAnimationListener(animListener);

    knarzActor = door.getSoundActor();
    knarzActor.getControlledObject().play();

    door.setOpen(false);
  end
end

class Door < GameObject
  
  def initialize(name, description, isOpen, canBeOpened, mesh = "arc_tuer_01.mesh", sound = "doorcreak.ogg")
    super(name, description);

    doorActor = $AM.createMeshActor(name, mesh, PhysicsManager::GT_BOX , 0.0);
    setActor(doorActor);
    @mSoundActor = $AM.createSoundSampleActor(name+"_knarzen", sound);
    doorActor.attachToSlot(@mSoundActor,"Bone01");

    @mOpen = isOpen
    @mOpenAction = OpenDoorAction.new()
    @mCloseAction = CloseDoorAction.new()

    @mCanBeOpened = canBeOpened
    
    if (canBeOpened)
    	addAction(@mOpenAction);
    	addAction(@mCloseAction);
    else
    	addAction(@mOpenAction, Action::ACT_DISABLED);
    	addAction(@mCloseAction, Action::ACT_DISABLED);
    end

    @mDoor.doAction("opendoor") unless not @mOpen
  end
  
  def setOpen(isOpen)
    @mOpen = isOpen
    fireObjectStateChangeEvent();
  end
  
  def isOpen()
    @mOpen
  end
  
  def getDefaultAction(actor)
     if (not @mCanBeOpened)
       super(actor)
     else
       if (@mOpen)
         @mCloseAction
       else
         @mOpenAction
       end
     end
  end

  def getSoundActor()
     return @mSoundActor
  end

end

