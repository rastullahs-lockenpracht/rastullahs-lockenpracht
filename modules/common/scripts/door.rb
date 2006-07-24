load "embed.rb"
#require 'actorupdateanimationlistener.rb'

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

        pt = doorActor.getPhysicalThing();
        p "call fitToPose for zu"
		pt.fitToPose("zu");

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
    
		pt = doorActor.getPhysicalThing();
        p "call fitToPose for auf"
		pt.fitToPose("auf");

    knarzActor = door.getSoundActor();
    knarzActor.getControlledObject().play();

    door.setOpen(false);
  end
end

class Door < GameObject
  
  def initialize(name, description, position, orientation, mesh = "arc_tuer_01.mesh", sound = "doorcreak.ogg")
    super(name, description);

    doorActor = $AM.createMeshActor(name, mesh, PhysicsManager::GT_BOX , 0.0);
    setActor(doorActor);
    doorActor.placeIntoScene(position, orientation);

    @mSoundActor = $AM.createSoundSampleActor(name+"_knarzen", sound);
    doorActor.attach(@mSoundActor);
  end

  def addActions(isOpen = false, canBeOpened = true)
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

