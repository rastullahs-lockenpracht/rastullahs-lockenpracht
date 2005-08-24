# TODO Skript ausbauen in Hinblick auf Interaktion Held <-> Item.
# und Aktoren untereinander.
# Halt diese Klassen wie sie hier beschrieben sind auch benutzen.
# TODO Über Speichersparen nachdenken. Inwiefern können Instanzen
# einiger Klassen für Instanzen anderer Klassen nachgenutzt werden?
# Inwiefern ist Deferred Construction sinnvoll?
require 'globals.rb'
require 'actions.rb'
require 'items.rb'

class OpenChestAction < RubyAction
  def initialize
    super("openchest", "Truhe öffnen");
  end

  # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
  def canDo(chest, user)
    not chest.isOpen();
  end

  def doAction(chest, user, target)
    chestActor = chest.getActor();
    chestActor.getControlledObject().replaceAnimation("zu", "auf", 1.0, 1);
    knarzActor = chestActor.getChildByName(chestActor.getName()+"_knarzen");
    knarzActor.getControlledObject().play(0);
    chest.setOpen( true);
    $UI.showContainerContent(chest);
  end
end

class CloseChestAction < RubyAction
  def initialize
    super("closechest", "Truhe schließen");
  end

  # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
  def canDo(chest, user)
    chest.isOpen();
  end

  def doAction(chest, user, target)
    chestActor = chest.getActor();
    chestActor.getControlledObject.replaceAnimation("auf", "zu", 1.0, 1);
    knarzActor = chestActor.getChildByName(chestActor.getName()+"_knarzen");
    knarzActor.getControlledObject().play(0);
    chest.setOpen( false);
  end
end

class Chest < Container
  def initialize(name, isOpen )
    super(10, name, "Eine Truhe");

    chestActor = $AM.createMeshActor( name, "ver_truhe_gross01.mesh",  0, 0.0 ); 
    $CORE.log("truhe.rb - Aktor erstellt.");
    setActor(chestActor);
    $CORE.log("truhe.rb - Aktor gesetzt");
    soundActor = $AM.createSoundSampleActor(name+"_knarzen","doorcreak.ogg");
    chestActor.attachToSlot(soundActor,"Bone01");
    $CORE.log("truhe.rb - Sound hinzugefuegt");

    @mOpen = isOpen;

    addAction(OpenChestAction.new);
    addAction(CloseChestAction.new);
    $CORE.log("truhe.rb - Aktionen hinzugefuegt.");
  end
  
  def setOpen( isOpen )
    @mOpen = isOpen;
    fireObjectStateChangeEvent();
  end

  def isOpen( )
    @mOpen;
  end

  def close()
    doAction("CloseChestAction", "closechest")
  end
end

