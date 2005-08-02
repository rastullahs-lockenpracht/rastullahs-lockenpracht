# TODO Skript ausbauen in Hinblick auf Interaktion Held <-> Item.
# und Aktoren untereinander.
# Halt diese Klassen wie sie hier beschrieben sind auch benutzen.
# TODO Über Speichersparen nachdenken. Inwiefern können Instanzen
# einiger Klassen für Instanzen anderer Klassen nachgenutzt werden?
# Inwiefern ist Deferred Construction sinnvoll?
require 'globals.rb'
require 'actions.rb'
require 'items.rb'

class SwitchUpAction < RubyAction
  def initialize
    super("NachOben", "Hebel nach oben schalten");
  end
  
  # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
  def canDo?(switch, user) 
    p "CanDo"+switch
    switch.getString("state") != "Hoch";
  end
  
  def doAction(switch, user, target)    
    switchMesh = switch.getActor().getControlledObject(); 
    p switch.getString("state")
    if (switch.getString("state") == "Mitte")
      switchMesh.stopAllAnimations()
      switchMesh.startAnimation("MitteOben", 1.0, 1)
    elsif (switch.getString("state") == "Unten")
      switchMesh.stopAllAnimations()
      switchMesh.startAnimation("UntenOben", 1.0, 1)
    end
    switch.setString("state", "Oben") 
    
    p switch
  end
end

class SwitchDownAction < RubyAction
  def initialize
    super("NachUnten", "Hebel nach unten schalten");
  end
  
  # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
  def canDo?(switch, user)
     p "CanDo"+switch
     switch.getString("state") != "Runter";
  end
  
  def doAction(switch, user, target)    
    switchMesh = switch.getActor().getControlledObject();
    if (switch.getString("state") == "Mitte")
    	switchMesh.stopAllAnimations()
      switchMesh.startAnimation("MitteUnten", 1.0, 1)
    elsif (switch.getString("state") == "Oben")
    	switchMesh.stopAllAnimations()
      switchMesh.startAnimation("ObenUnten", 1.0, 1)
    end
    switch.setString("state", "Unten") 
    p switch
  end
end

class SwitchMiddleAction < RubyAction
  def initialize
    super("Mitteln", "Hebel in die Mitte schalten");
  end
  
  # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
  def canDo?(switch, user)    
     p "CanDo"+switch
     switch.getString("state") != "Mitte";
  end
  
  def doAction(switch, user, target)    
    switchMesh = switch.getActor().getControlledObject();
    if (switch.getString("state") == "Oben")
    	switchMesh.stopAllAnimations()
      # switchMesh.startAnimation("ObenMitte", 1.0, 1)
    elsif (switch.getString("state") == "Unten")
    	switchMesh.stopAllAnimations()
      # switchMesh.startAnimation("UntenMitte", 1.0, 1)
    end
    switch.setString("state", "Mitte") 
    p switch
    p user
    p target
  end
end


class Switch < RubyItem
  def initialize(name )
    super(10, name, "Ein Hebel");

    switchActor = $AM.createMeshActor(name, "arc_hebel_01.mesh" );
    $CORE.log("switch-actor erstellt.");
    setActor(switchActor);
    $CORE.log("actor gesetzt");
    
    setString("state", "Mitte");
    
    addAction(SwitchUpAction.new);
    addAction(SwitchDownAction.new);
    addAction(SwitchMiddleAction.new);
    $CORE.log("Aktionen hinzugefuegt.");
  end
end

