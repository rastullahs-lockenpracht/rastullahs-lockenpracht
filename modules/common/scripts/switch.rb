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
    switch.getState != "Hoch";
  end
  
  def doAction(switch, user, target)    
    switchActor = switch.getActor(); 
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
     switch.getState != "Runter";
  end
  
  def doAction(switch, user, target)    
    switchActor = switch.getActor();
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
     switch.getState != "Mitte";
  end
  
  def doAction(switch, user, target)    
    switchActor = switch.getActor();
    p switch
    p user
    p target
  end
end


class Switch < RubyItem
  def initialize(name )
    super(10, name, "Ein Hebel");

    switchActor = $AF.createMeshActor(name, "arc_hebel_01.mesh" );
    $CORE.log("switch-actor erstellt.");
    setActor(switchActor);
    $CORE.log("actor gesetzt");
    
    @state = "Mitte";    
    
    addAction(SwitchUpAction.new);
    addAction(SwitchDownAction.new);
    addAction(SwitchMiddleAction.new);
    $CORE.log("Aktionen hinzugefuegt.");
  end
  
  def setState( state )
    @state = state;
  end
  
  def getState()
    @state;
  end
end

