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
    def canDo?(torch, user, target)
        # Wenn die Fackel aus ist, kann sie angemacht werden.
        not torch.lit?;
    end
    
    def doAction(torch, user, target)
        torch.setLit(true) unless torch.lit?;
    end
end

# Die Aktion für das Löschen der Fackel.
class PutoutTorchAction < Action
    def initialize
        super("putouttorch", "Die Fackel löschen für Verstecken und so.")
    end
    
    # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
    def canDo?(torch, user, target)
        # Wenn die Fackel aus ist, kann sie angemacht werden.
        torch.lit?;
    end
    
    def doAction(torch, user, target)
        torch.setLit(false) if torch.lit?;
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
    
    def initialize(id)
        super(id)
		@_prop_color = [0.6, 0.5, 0.3, 4];
		addActions()
    end
    
    def setLit(lit)
        @_prop_lit = lit;
		updateTorch();
    end
    
    def lit?
        @_prop_lit;
    end
    
    def setProperty(name, value)
		if (name == "lit")
			setLit(value);
		elsif (name == "sound")
			@_prop_sound = value;
		elsif (name == "flames")
			@_prop_flames = value;
		elsif (name == "color")
			@_prop_color = value;
			if (@light != nil)
				@light.getControlledObject().setDiffuseColour(@_prop_color);
				@light.getControlledObject().setSpecularColour(@_prop_color);
			end
		else
			super(name, value)
        end
    end
    
    def getProperty(name)
		if (name == "lit")
			return lit?
		elsif (name == "sound")
			return @_prop_sound
		elsif (name == "flames")
			return @_prop_flames
		elsif (name == "color")
			return @_prop_color
		else
			return super(name, value)
        end
    end

    def onBeforeStateChange(oldstate, newstate)
		if (newstate == RlScript::GOS_LOADED || newstate == RlScript::GOS_IN_POSSESSION) && 
		(oldstate == RlScript::GOS_IN_SCENE || oldstate == RlScript::GOS_HELD || oldstate == RlScript::GOS_READY)
			if @sound != nil
				getActor().detach(@sound)
				$AM.destroyActor(@sound)
				@sound = nil
			end
			if @light != nil
				getActor().detach(@light)
				$AM.destroyActor(@light)
				@light = nil
			end
			if @flammen != nil
				getActor().detach(@flammen)
				$AM.destroyActor(@flammen)
				@flammen = nil
			end
		end
    end

    def onAfterStateChange(oldstate, newstate)
		if newstate == RlScript::GOS_IN_SCENE || newstate == RlScript::GOS_HELD || newstate == RlScript::GOS_READY
			if @_prop_sound != nil && @_prop_sound != "" && @sound == nil
				@sound = $AM.createSoundSampleActor(getId().to_s()+"_sound", @_prop_sound)
				@sound.getControlledObject().setVolume(0.5);
				@sound.getControlledObject().setLooping(true)
				@sound.getControlledObject().set3d(true)
				#getActor().attachToSlot(@sound, "SLOT_FAR_END")
                                getActor().attach(@sound) # here the position is set correctly!
			end

			if @light == nil
				@light = $AM.createLightActor(getId().to_s()+"_light", LightObject::LT_POINT);
				@light.getControlledObject().setDiffuseColour(@_prop_color);
				@light.getControlledObject().setSpecularColour(@_prop_color);
				@light.getControlledObject().setAttenuation(5, 1, 0, 0);
				getActor().attachToSlot(@light, "SLOT_FAR_END")
			end

			if @_prop_flames != nil && @_prop_flames != "" && @flammen == nil
				@flammen = $AM.createParticleSystemActor(getId().to_s()+"_flames", @_prop_flames)
				getActor().attachToSlot(@flammen, "SLOT_FAR_END")
			end
		end
		updateTorch()
    end
	
	def updateTorch()
		if @sound != nil
			if lit?
				@sound.getControlledObject().play();
			else
				@sound.getControlledObject().stop();
			end
		end
		
		if @light != nil
			@light.getControlledObject().setActive(@_prop_lit);
		end

		if @flammen != nil
			@flammen.getControlledObject().setActive(@_prop_lit);
		end
	end
	
    def addActions()
        addAction(LightTorchAction.new)
        addAction(PutoutTorchAction.new)
    end
    
    def getDefaultAction(actor)
        if @_prop_lit
            @mPutoutAction
        else
            @mLightAction
        end
    end
end

