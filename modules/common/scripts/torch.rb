# TODO Skript ausbauen in Hinblick auf Interaktion Held <-> Item.
# und Aktoren untereinander.
# Halt diese Klassen wie sie hier beschrieben sind auch benutzen.
# TODO Über Speichersparen nachdenken. Inwiefern können Instanzen
# einiger Klassen für Instanzen anderer Klassen nachgenutzt werden?
# Inwiefern ist Deferred Construction sinnvoll?
require 'globals.rb'
require 'actions.rb'
require 'items.rb'

# Die Aktion für das Anzünden der Fackel.
class LightTorchAction < RubyAction
  def initialize
    super("Anzünden", "Die Fackel anzünden für mehr Licht und so.");
  end
  
  # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
  def canDo?(torch, user)
    # Wenn die Fackel aus ist, kann sie angemacht werden.
    not torch.lit?;
  end
  
  def doAction(torch, user, target)
    # Fackel hat ein Licht am Slot SLOT_FAR_END
    # Man könnte es auch so machen, dass dieses Licht jetzt
    # erzeugt und angeheftet wird. Spräche einiges dafür.
    # Das alles ist implizites Wissen. Deshalb sollte es
    # allgemeine Richtlinien geben und die Item-Doku sollte
    # sowas ganz klar ausweisen. Da aber alles innerhalb von Ruby
    # geschieht, ist das nicht so tragisch. Die C++-Seite ist
    # an der Stelle nur dumm und macht was Ruby sagt.
    
    torchActor = torch.getActor();
    torchActor.getChildBySlotAndIndex(Slots.SLOT_FAR_END, 0).activate();
    torchActor.getChildByName("TorchSparks").activate();
    torchActor.getChildByName("TorchCrackle").activate();
    # Activation sollte folgendermaßen geregelt sein:
    #       ist Child activated, so wird er angezeigt/erklingt
    #       wenn Parent-Activation true ist, sonst nicht
    #       Parent-Activity-Flag wird nicht an die
    #       Childs weitergereicht.
    
    #TODO timer setzen, damit die Fackel nach Ablauf ihrer Lebensdauer
    # ausgeht.
    
    torch.setLit(true);
  end
end

# TODO Physikalische Attribute etc..
# TODO Persistenz *schreck*
class Torch < RubyItem
  def initialize(name)
    super(1, "Fackel", "Handelsuebliche Fackel zum Beleuchten der Umgebung");
    # Momentan stelle ich mir die Erzeugung so vor:
    # Die Aktorerstellung über spezifische
    # Fabrikmethoden für die Dinge, die der Aktor repräsentiert.
    # Die erzeugen alle die gleiche Aktorklasse,
    # aber erzeugen auch das Ogre::Movable
    # (bzw RlMovable, was das kapselt) und hängen es ran.
    
    # Argumente hier: Name, Mesh-Name, Die physikalische Form
    # und die Dichte (g/cm^3) des Objektes.
    #torchActor = $ActorManager.createMeshActor("Torch", "Torch.mesh",
    #Physics.CAPSULE_GEOM, 1.0);
    torchActor = $AF.createMeshActor( name, "ins_fackel_01.mesh" );
    $CORE.log("actor erstellt.");
    #lightActor = $ActorManager.createLightActor("TorchLight",
    #Color.YELLOW, Light.POINTLIGHT, Light.LINEAR_ATTENUATION);
    
    #sparksActor = $ActorManager.createParticleActor("TorchSparks",
    #        "TorchSparks.particles");
    
    #sfxActor = $ActorManager.createSoundActor("TorchCrackle",
    #        "TorchCrackle.wav", Sound.LOOP, Sound.VOLUME_QUIET);        
    
    # Defaultwert für den vierten Param ist Slots.SLOT_DEFAULT
    #torchActor.attach(Slots.SLOT_FAR_END, lightActor);
    #torchActor.attach(Slots.SLOT_FAR_END, sparksActor);
    #torchActor.attach(Slots.SLOT_FAR_END, sfxActor);
    
    setActor(torchActor);
    $CORE.log("actor gesetzt");
    
    @lit = false;
    
    # RubyItem selbst impliziert ja schon ein paar Standardactions
    # (Aufnehmen, Ansehen, etc.)
    # TODO soll nicht so bleiben, dass jede Fackel ne separate
    # Action-Instanz erhält. Eine für alle sollte reichen.
    addAction(LightTorchAction.new);
    $CORE.log("Aktion hinzugefuegt.");
  end
  
  def setLit(lit)
    @lit = lit;
  end
  
  def lit?
    @lit;
  end
end

