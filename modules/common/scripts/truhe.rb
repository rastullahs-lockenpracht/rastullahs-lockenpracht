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
    super("openchest", "Truhe oeffnen");
  end

  # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
  def canDo?(chest, user)
    not chest.getBool("open");
  end

  def doAction(chest, user, target)
    doorActor = chest.getActor();
    doorActor.getControlledObject().replaceAnimation("zu", "auf", 1.0, 1);
    knarzActor = doorActor.getChildByName(doorActor.getName()+"_knarzen");
    knarzActor.getControlledObject().play(0);
    chest.setBool("open", true);
    $UI.showContainerContent(chest);
  end
end

class CloseChestAction < RubyAction
  def initialize
    super("closechest", "Truhe schliessen");
  end

  # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
  def canDo?(chest, user)
    chest.getBool("open");
  end

  def doAction(chest, user, target)
    chestActor = chest.getActor();
    chestActor.getControlledObject.replaceAnimation("auf", "zu", 1.0, 1);
    knarzActor = chestActor.getChildByName(doorActor.getName()+"_knarzen");
    knarzActor.getControlledObject().play(0);
    chest.setBool("open", false);
  end
end

class Chest < Container
  def initialize(name, isOpen, canBeOpened)
    super(10, name, "Eine Truhe");

    chestActor = $AF.createMeshActor( name, "ver_truhe_gross01.mesh",  0, 0.0 ); #PhysicsManager::GT_BOX , 6.0);
    $CORE.log("truhe.rb - Aktor erstellt.");
    setActor(chestActor);
    $CORE.log("truhe.rb - Aktor gesetzt");
    soundActor = $AF.createSoundActor(name+"_knarzen","doorcreak.ogg");
    chestActor.attachToSlot(soundActor,"Bone01");
    $CORE.log("truhe.rb - Sound hinzugefuegt");

    setBool("open", isOpen);

    if (canBeOpened)
    	addAction(OpenChestAction.new);
    	addAction(CloseChestAction.new);
    	$CORE.log("truhe.rb - Aktionen hinzugefuegt.");
    end
  end

end

