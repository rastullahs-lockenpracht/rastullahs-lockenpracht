# TODO Skript ausbauen in Hinblick auf Interaktion Held <-> Item.
# und Aktoren untereinander.
# Halt diese Klassen wie sie hier beschrieben sind auch benutzen.
# TODO Über Speichersparen nachdenken. Inwiefern können Instanzen
# einiger Klassen für Instanzen anderer Klassen nachgenutzt werden?
# Inwiefern ist Deferred Construction sinnvoll?
load "embed.rb"

class OpenContainerAction < Action
  def initialize
    super("opencontainer", "Öffnen");
  end

  # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
  def canDo(container, user)
    not container.isOpen();
  end

  def doAction(container, user, target)
    container.open()
  end
end

class CloseContainerAction < Action
  def initialize
    super("closecontainer", "Schließen");
  end

  # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
  def canDo(container, user)
    container.isOpen();
  end

  def doAction(container, user, target)
    container.close()
  end
end

class Chest < Container
  def initialize(id)
    super(id)

    @mOpen = false;
    
    # @todo: Make these actions global actions (add to ActionManager)
    addAction(OpenContainerAction.new);
    addAction(CloseContainerAction.new);
    $SCRIPT.log("truhe.rb - Aktionen hinzugefuegt.");
  end
  
  def placeIntoScene()
    super()
    soundActor = $AM.createSoundSampleActor(getActor().getName()+"_knarzen","doorcreak.ogg");
    getActor().attachToSlot(soundActor,"Bone01");    
  end
  
  def setOpen( isOpen )
    @mOpen = isOpen;
    fireObjectStateChangeEvent();
  end

  def isOpen( )
    @mOpen;
  end
  
  def open()
    chestActor = getActor();
    chestActor.getControlledObject().replaceAnimation("zu", "auf", 1.0, 1);
    knarzActor = chestActor.getChildByName(chestActor.getName()+"_knarzen");
    knarzActor.getControlledObject().play();
    setOpen( true);
    $WF.showContainerContent(self);
  end

  def close()
    chestActor = getActor();
    chestActor.getControlledObject.replaceAnimation("auf", "zu", 1.0, 1);
    knarzActor = chestActor.getChildByName(chestActor.getName()+"_knarzen");
    knarzActor.getControlledObject().play();
    setOpen( false);
  end
end

