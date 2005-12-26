require 'globals.rb'
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
    $SCRIPT.log("doAction");
    print "doAction";
    doorActor = door.getActor(); 
    doorActor.getControlledObject().replaceAnimation("zu", "auf", 1.0, 1);
    animListener = ActorUpdateAnimationListener.new();
    print animListener;    
    anim = doorActor.getControlledObject().getAnimation("auf");
    print anim;
    anim.addAnimationListener(animListener);
    print anim;
    print animListener;
    knarzActor = doorActor.getChildByName(doorActor.getName()+"_knarzen");
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

    knarzActor = doorActor.getChildByName(doorActor.getName()+"_knarzen");
    knarzActor.getControlledObject().play();

    door.setOpen(false);
  end
end

class Door < GameObject
  def initialize(name, description, isOpen, canBeOpened)
    super(name, description);

    doorActor = $AM.createMeshActor( name, "arc_tuer_01.mesh",  PhysicsManager::GT_CONVEXHULL, 0.0 ); #PhysicsManager::GT_BOX , 6.0);
    $SCRIPT.log("door.rb - Aktor erstellt.");
    setActor(doorActor);
    $SCRIPT.log("door.rb - Aktor gesetzt");
    soundActor = $AM.createSoundSampleActor(name+"_knarzen","doorcreak.ogg");
    doorActor.attachToSlot(soundActor,"Bone01");
    $SCRIPT.log("door.rb - Sound hinzugefuegt");

    @mOpen = isOpen
    @mOpenAction = OpenDoorAction.new()
    @mCloseAction = CloseDoorAction.new()

    @mCanBeOpened = canBeOpened
    
    if (canBeOpened)
    	addAction(@mOpenAction);
    	addAction(@mCloseAction);
    	$SCRIPT.log("door.rb - Aktionen hinzugefuegt.");
    else
    	addAction(@mOpenAction, Action::ACT_DISABLED);
    	addAction(@mCloseAction, Action::ACT_DISABLED);
    	$SCRIPT.log("door.rb - Aktionen versteckt hinzugefuegt.");
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

end

