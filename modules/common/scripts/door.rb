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
    
    #p "call fitToPose for zu"
    doorActor.getPhysicalThing().fitToPose("zu");

    doorActor.getControlledObject().replaceAnimation("zu", "auf", 1.0, 1);

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
   
    #p "call fitToPose for auf"
    doorActor.getPhysicalThing().fitToPose("auf");

    doorActor.getControlledObject.replaceAnimation("auf", "zu", 1.0, 1); 

    knarzActor = door.getSoundActor();
    knarzActor.getControlledObject().play();

    door.setOpen(false);
  end
end

class Door < GameObject
  
  def initialize(id)
    super(id);
  end

  def setProperty(name, value)
    if (name == "sound")
      @mSound = value;
    elsif (name == "open")
      @mOpen = value;
    elsif (name == "openable")
      @mCanBeOpened = value;
    else
      super(name, value)
    end
  end

  def placeIntoScene()
    super()
    @mSoundActor = $AM.createSoundSampleActor("door_" + getId().to_s + "_knarzen", @mSound);
    getActor().attach(@mSoundActor);
    addActions();
  end

  def addActions()
    @mOpenAction = OpenDoorAction.new()
    @mCloseAction = CloseDoorAction.new()
    if (@mCanBeOpened)
    	addAction(@mOpenAction);
    	addAction(@mCloseAction);
    else
    	addAction(@mOpenAction, Action::ACT_DISABLED);
    	addAction(@mCloseAction, Action::ACT_DISABLED);
    end

    doAction("opendoor") unless not @mOpen
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

