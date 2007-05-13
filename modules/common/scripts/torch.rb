# TODO Skript ausbauen in Hinblick auf Interaktion Held <-> Item.
# und Aktoren untereinander.
# Halt diese Klassen wie sie hier beschrieben sind auch benutzen.
# TODO Über Speichersparen nachdenken. Inwiefern können Instanzen
# einiger Klassen für Instanzen anderer Klassen nachgenutzt werden?
# Inwiefern ist Deferred Construction sinnvoll?
load "embed.rb"
require 'gameobjectprops.rb'
require 'jobs/soundjobs.rb'
require 'jobs/animationjobs.rb'

# Die Aktion für das Anzünden der Fackel.
class LightTorchAction < Action
    def initialize()
        super("lighttorch", "Die Fackel anzünden für mehr Licht und so.");
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
        #torchActor.getChildByName("TorchSparks").activate();
        #torchActor.getChildByName("TorchCrackle").activate();
        # Activation sollte folgendermaßen geregelt sein:
        #       ist Child activated, so wird er angezeigt/erklingt
        #       wenn Parent-Activation true ist, sonst nicht
        #       Parent-Activity-Flag wird nicht an die
        #       Childs weitergereicht.
        
        #TODO timer setzen, damit die Fackel nach Ablauf ihrer Lebensdauer
        # ausgeht.
        torch.sound.getSound().setPosition(torchActor.getPosition())
        torch.sound.play()
        torch.setLit(true);
    end
end

# Die Aktion für das Löschen der Fackel.
class PutoutTorchAction < Action
    def initialize
        super("putouttorch", "Die Fackel löschen für Verstecken und so.");
    end
    
    # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
    def canDo?(torch, user)
        # Wenn die Fackel aus ist, kann sie angemacht werden.
        torch.lit?;
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
        #torchActor.getChildByName("TorchSparks").deactivate();
        #torchActor.getChildByName("TorchCrackle").deactivate();
        # Activation sollte folgendermaßen geregelt sein:
        #       ist Child activated, so wird er angezeigt/erklingt
        #       wenn Parent-Activation true ist, sonst nicht
        #       Parent-Activity-Flag wird nicht an die
        #       Childs weitergereicht.
        
        #TODO timer setzen, damit die Fackel nach Ablauf ihrer Lebensdauer
        # ausgeht.
        torch.sound.stop();
        torch.setLit(false);
    end
end

# TODO Physikalische Attribute etc..
# TODO Persistenz *schreck*
class Torch < GameObject
    include GameObjectProperties
    
    def initialize(id)
        super(id)
    end
    
    def setLit(lit)
        @_prop_Lit = lit
    end
    
    def lit?
        @_prop_Lit
    end
    
    def sound
        @_prop_Sound
    end
    
    def setProperty(name, value)
        if name == "sound"
            @_prop_Sound = value
        elsif name == "lit"
            @_prop_Lit = value
        elsif name == "lightable"
            @_prop_Lightable = value
        else
            super(name, value)
        end
    end
    
    def placeIntoScene()
        super()
        sound = $SM.createSound("feuer_knisternd_01.ogg")
        sound.setLooping(true)
        sound.set3d(true)
        @_prop_Sound = SoundObject.new(sound, getId().to_s())
        addActions()
    end
    
    def addActions()
        @mLightAction = LightTorchAction.new
        @mPutoutAction = PutoutTorchAction.new
        if @_prop_Lightable
            addAction(@mLightAction)
            addAction(@mPutoutAction)
        else
            addAction(@mLightAction, Action::ACT_DISABLED)
            addAction(@mPutoutAction, Action::ACT_DISABLED)
        end
        doAction("lighttorch") unless @_prop_Lit
    end
    
    def getDefaultAction(actor)
        if not @_prop_Lightable
            super(actor)
        else
            if @_prop_Lit
                @mPutoutAction
            else
                @mLightAction
            end
        end
    end
end

