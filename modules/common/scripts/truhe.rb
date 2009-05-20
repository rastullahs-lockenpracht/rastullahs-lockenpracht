# TODO Skript ausbauen in Hinblick auf Interaktion Held <-> Item.
# und Aktoren untereinander.
# Halt diese Klassen wie sie hier beschrieben sind auch benutzen.
# TODO Über Speichersparen nachdenken. Inwiefern können Instanzen
# einiger Klassen für Instanzen anderer Klassen nachgenutzt werden?
# Inwiefern ist Deferred Construction sinnvoll?
load "embed.rb"

require "gameobjectprops.rb"

class OpenContainerAction < Action
  def initialize
    super("open", "Öffnen");
  end

  # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
  def canDo(container, user, target)
    not container.isOpen();
  end

  def doAction(container, user, target)
    container.open()
  end
end

class CloseContainerAction < Action
  def initialize
    super("close", "Schließen");
  end

  # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
  def canDo(container, user, target)
    container.isOpen();
  end

  def doAction(container, user, target)
    container.close()
  end
end

class ShowContainerContentAction < Action
  def initialize
    super("showcontent", "Hineinsehen");
  end

  # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
  def canDo(container, user, target)
    true
  end

  def doAction(container, user, target)
    container.showContent(user)
  end
end


class Chest < Container

  include GameObjectProperties
  
  def initialize(id)
    super(id)

    @_prop_Open = false;
    
    # @todo: Make these actions global actions (add to ActionManager)
    addAction(OpenContainerAction.new);
    addAction(CloseContainerAction.new);
    addAction(ShowContainerContentAction.new);
    $SCRIPT.log("truhe.rb - Aktionen hinzugefuegt.");
  end
  
  def placeIntoScene()
    super()
    soundActor = $AM.createSoundSampleActor(getActor().getName()+"_knarzen","doorcreak.ogg");
    getActor().attachToSlot(soundActor,"Bone01");    
  end
  
  def setOpen( isOpen )
    @_prop_Open = isOpen;
    fireObjectStateChangeEvent();
  end

  def isOpen( )
    @_prop_Open;
  end
  
  def open()
    chestActor = getActor();
    chestActor.getControlledObject().replaceAnimation("zu", "auf", 1.0, 1);
    knarzActor = chestActor.getChildByName(chestActor.getName()+"_knarzen");
    knarzActor.getControlledObject().play();
    setOpen( true);    
  end
  
  def showContent(actor)
    if not isOpen()
        open()
    end
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

