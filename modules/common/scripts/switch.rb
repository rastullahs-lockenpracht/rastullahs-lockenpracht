# TODO Skript ausbauen in Hinblick auf Interaktion Held <-> Item.
# und Aktoren untereinander.
# Halt diese Klassen wie sie hier beschrieben sind auch benutzen.
# TODO Über Speichersparen nachdenken. Inwiefern können Instanzen
# einiger Klassen für Instanzen anderer Klassen nachgenutzt werden?
# Inwiefern ist Deferred Construction sinnvoll?
load "embed.rb"

class SwitchUpAction < Action
  def initialize
    super("Oben", "Hebel nach oben schalten");
  end
  
  # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
  def canDo(switch, user, target)
    switch.getState() != Switch3Way::STATE_OBEN;
  end
  
  def doAction(switch, user, target)    
    switchMesh = switch.getActor().getControlledObject(); 

    if (switch.getState() == Switch3Way::STATE_MITTE)
      switchMesh.stopAllAnimations()
      switchMesh.startAnimation("MitteOben", 2.0, 1)
    elsif (switch.getState() == Switch3Way::STATE_UNTEN)
      switchMesh.stopAllAnimations()
      switchMesh.startAnimation("UntenOben", 4.0, 1)
    end
    switch.setState( Switch3Way::STATE_OBEN );
  end
end

class SwitchDownAction < Action
  def initialize
    super("Unten", "Hebel nach unten schalten");
  end
  
  # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
  def canDo(switch, user, target)
     switch.getState() != Switch3Way::STATE_UNTEN;
  end
  
  def doAction(switch, user, target)    
    switchMesh = switch.getActor().getControlledObject();
    if (switch.getState() == Switch3Way::STATE_MITTE)
      switchMesh.stopAllAnimations()
      switchMesh.startAnimation("MitteUnten", 2.0, 1)
    elsif (switch.getState() == Switch3Way::STATE_OBEN )
      switchMesh.stopAllAnimations()
      switchMesh.startAnimation("ObenUnten", 4.0, 1)
    end
    switch.setState( Switch3Way::STATE_UNTEN );
  end
end

class SwitchMiddleAction < Action
  def initialize
    super("Mitte", "Hebel in die Mitte schalten");
  end
  
  # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
  def canDo(switch, user, target)
     switch.getState() != Switch3Way::STATE_MITTE;
  end
  
  def doAction(switch, user, target)    
    switchMesh = switch.getActor().getControlledObject();
    if (switch.getState() == Switch3Way::STATE_OBEN)
      switchMesh.stopAllAnimations()
      switchMesh.startAnimation("ObenMitte", 2.0, 1)
    elsif (switch.getState() == Switch3Way::STATE_UNTEN)
      switchMesh.stopAllAnimations()
      switchMesh.startAnimation("UntenMitte", 2.0, 1)
    end
    switch.setState( Switch3Way::STATE_MITTE );
  end
end

class Switch3Way < GameObject

  include GameObjectProperties
  
  Switch3Way.const_set( "STATE_OBEN", 0 )
  Switch3Way.const_set( "STATE_MITTE", 1 )
  Switch3Way.const_set( "STATE_UNTEN", 2 )

  def initialize( id )
    super(id);

    @_prop_state = Switch3Way::STATE_MITTE;
    
    addAction(SwitchUpAction.new);
    addAction(SwitchDownAction.new);
    addAction(SwitchMiddleAction.new);
    $SCRIPT.log("Aktionen hinzugefuegt.");
  end

  def getState()
    @_prop_state
  end

  def setState( state )
    raise ArgumentError, "State existiert nicht", caller unless ( state >= 0 ) 

    @_prop_state = state;
    fireObjectStateChangeEvent();
  end

end

