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
        super("lighttorch", "Die Fackel anzünden für mehr Licht und so.")
    end
    
    # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
    def canDo?(torch, user)
        # Wenn die Fackel aus ist, kann sie angemacht werden.
        not torch.lit?;
    end
    
    def doAction(torch, user, target)
        return if torch.lit?
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
        torch.setLit(true);
    end
end

# Die Aktion für das Löschen der Fackel.
class PutoutTorchAction < Action
    def initialize
        super("putouttorch", "Die Fackel löschen für Verstecken und so.")
    end
    
    # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
    def canDo?(torch, user)
        # Wenn die Fackel aus ist, kann sie angemacht werden.
        torch.lit?;
    end
    
    def doAction(torch, user, target)
        return unless torch.lit?
        
        torch.setLit(false);
    end
end

# TODO Physikalische Attribute etc..
# TODO Persistenz *schreck*
class Torch < Item
    include GameObjectProperties
    attr :_prop_flames
    attr :_prop_sound
    attr :_prop_color
    attr :_prop_lit
    attr :_prop_lightable

    
    def initialize(id)
        super(id)
		addActions()
		@_prop_color = [0.8, 0.7, 0.5, 1];
    end
    
    def setLit(lit)
        @_prop_lit = lit
		updateState();
    end
    
    def lit?
        @_prop_lit
    end
    
    def setProperty(name, value)
		if (name == "lit")
			setLit(value)
		elsif (name == "sound")
			@_prop_sound = value;
		elsif (name == "flames")
			@_prop_flames = value;
		else
			super(name, value)
        end
    end
    
    def onStateChange(oldState, newState)
		p "New state"+newState.to_s()
		updateState();
    end
	
	def updateState()
		
		if (getState() == RlScript::GOS_IN_SCENE || getState() == RlScript::GOS_HELD)
			if (@_prop_sound != nil && @_prop_sound != "" && @soundObj == nil)
				@sound = $AM.createSoundSampleActor(getId().to_s()+"_sound", @_prop_sound)
		        @sound.getControlledObject().setLooping(true)
		        @sound.getControlledObject().set3d(true)
				getActor().attachToSlot(@sound, "SLOT_FAR_END")
			end
			if (@_prop_flames != nil && @_prop_flames != "" && @flammen == nil)
				@flammen = $AM.createParticleSystemActor(getId().to_s()+"_flames", @_prop_flames)
				getActor().attachToSlot(@flammen, "SLOT_FAR_END")
			end
			if (@light == nil)
				@light = $AM.createLightActor(getId().to_s()+"_light", LightObject::LT_POINT);
				@light.getControlledObject().setDiffuseColour(@_prop_color);
				@light.getControlledObject().setSpecularColour(@_prop_color);
				@light.getControlledObject().setAttenuation(5, 1, 0, 0);
				getActor().attachToSlot(@light, "SLOT_FAR_END")
			end
			
			@light.getControlledObject().setActive(@_prop_lit);
			if (@flammen != nil)
				@flammen.getControlledObject().setActive(@_prop_lit);
			end
			if (@sound != nil)
				@sound.getControlledObject().setActive(@_prop_lit);
			end
			
			
		else
		end
	end
    
    def addActions()
        @mLightAction = LightTorchAction.new
        @mPutoutAction = PutoutTorchAction.new
        if @_prop_lightable
            addAction(@mLightAction)
            addAction(@mPutoutAction)
        else
            addAction(@mLightAction, Action::ACT_DISABLED)
            addAction(@mPutoutAction, Action::ACT_DISABLED)
        end        
    end
    
    def getDefaultAction(actor)
        if not @_prop_Lightable
            super(actor)
        else
            if @_prop_lit
                @mPutoutAction
            else
                @mLightAction
            end
        end
    end
end

