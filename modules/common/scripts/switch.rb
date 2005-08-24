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
    super("Oben", "Hebel nach oben schalten");
  end
  
  # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
  def canDo(switch, user) 
    switch.getState() != Switch::STATE_OBEN;
  end
  
  def doAction(switch, user, target)    
    switchMesh = switch.getActor().getControlledObject(); 

    if (switch.getState() == Switch::STATE_MITTE)
      switchMesh.stopAllAnimations()
      switchMesh.startAnimation("MitteOben", 2.0, 1)
    elsif (switch.getState() == Switch::STATE_UNTEN)
      switchMesh.stopAllAnimations()
      switchMesh.startAnimation("UntenOben", 4.0, 1)
    end
    switch.setState( Switch::STATE_OBEN );
  end
end

class SwitchDownAction < RubyAction
  def initialize
    super("Unten", "Hebel nach unten schalten");
  end
  
  # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
  def canDo(switch, user)
     switch.getState() != Switch::STATE_UNTEN;
  end
  
  def doAction(switch, user, target)    
    switchMesh = switch.getActor().getControlledObject();
    if (switch.getState() == Switch::STATE_MITTE)
      switchMesh.stopAllAnimations()
      switchMesh.startAnimation("MitteUnten", 2.0, 1)
    elsif (switch.getState() == Switch::STATE_OBEN )
      switchMesh.stopAllAnimations()
      switchMesh.startAnimation("ObenUnten", 4.0, 1)
    end
    switch.setState( Switch::STATE_UNTEN );
  end
end

class SwitchMiddleAction < RubyAction
  def initialize
    super("Mitte", "Hebel in die Mitte schalten");
  end
  
  # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
  def canDo(switch, user)    
     switch.getState() != Switch::STATE_MITTE;
  end
  
  def doAction(switch, user, target)    
    switchMesh = switch.getActor().getControlledObject();
    if (switch.getState() == Switch::STATE_OBEN)
      switchMesh.stopAllAnimations()
      switchMesh.startAnimation("ObenMitte", 2.0, 1)
    elsif (switch.getState() == Switch::STATE_UNTEN)
      switchMesh.stopAllAnimations()
      switchMesh.startAnimation("UntenMitte", 2.0, 1)
    end
    switch.setState( Switch::STATE_MITTE );
  end
end

class Switch < RubyItem
  Switch.const_set( "STATE_OBEN", 0 )
  Switch.const_set( "STATE_MITTE", 1 )
  Switch.const_set( "STATE_UNTEN", 2 )

  def initialize( name )
    super(10, name, "Ein Hebel");

    switchActor = $AM.createMeshActor(name, "arc_hebel_01.mesh" );
    $CORE.log("switch-actor erstellt.");
    setActor(switchActor);
    $CORE.log("actor gesetzt");
    
    @state = Switch::STATE_MITTE;
    
    addAction(SwitchUpAction.new);
    addAction(SwitchDownAction.new);
    addAction(SwitchMiddleAction.new);
    $CORE.log("Aktionen hinzugefuegt.");
  end

  def getState()
    @state
  end

  def setState( state )
    raise ArgumentError, "State existiert nicht", caller unless ( state >= 0 ) 

    @state = state;
    fireObjectStateChangeEvent();
  end

end

