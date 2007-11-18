require 'embed.rb'

class ScareToDeathAction < Action
  def initialize
	super("Erschrecken", "Die Spinne erschrecken")
  end

  def doAction(spinne, player, target)
	spinne.getScared(player)
  end
end

#class SpiderDieAnimationListener < AnimationListener
#	def initialize(player)
#	        super()
#		@mPlayer = player
#	end
#
#	def animationFinished(anEvent)
#		$SCRIPT.log("SpiderDie animationFinished.");
#		anEvent.getAnimation().getMeshObject().getActor().die(@mPlayer)
#	end
#	
#	def animationPaused(anEvent)
#		$SCRIPT.log("SpiderDie animationPaused.");
#	end
#	
#	def animationUnpaused(anEvent)
#		$SCRIPT.log("SpiderDie animationUnpaused.");
#	end
#end


class Waldspinne < Creature
    def initialize(id)
		super(id)
		@mScareAction = ScareToDeathAction.new()
		addAction(@mScareAction)
    end
    

  def initSounds()    
   	@mErschreckenSound = $AM.createSoundSampleActor("ErschreckenSound","erschrecken_buh_01.ogg");
	getActor().attach(@mErschreckenSound);
	@mSpinnenTodSound = $AM.createSoundSampleActor("SpinnenTotSound","spinne_todesschrei_01.ogg");
	getActor().attach(@mSpinnenTodSound);
  end
  

  def getScared(player)
    if (@mErschreckenSound == nil)
      initSounds()
    end
  
  	#Player erschreckt die Spinne
  	@mErschreckenSound.getControlledObject().play();
  	
  	# Listener auf Todesanimation, läßt die Spinne entgültig sterben
    #	animListener = SpiderDieAnimationListener.new(player)
  	# Aktion entfernen
  	removeAction(@mScareAction)
  	# Todesanimation starten
  	getActor().getControlledObject().startAnimation("death", 0.33, 1)
  	#getActor().getControlledObject().getAnimation("death").addAnimationListener(animListener)
  end
	
  def die(player)
  	@mSpinnenTodSound.getControlledObject().play();
  	
  	# Quest erledigt
  	RulesSubsystem.getSingleton().getQuestBook().getQuest("spinne").setState(Quest::COMPLETED)
  	# Spieler verletzen
  	player.modifyLe( -(player.getLe() - 4) )
  	# Beschreibung der Spinne ändern
  	setDescription("Eine große tote Spinne, die sich nie wieder vor irgendetwas ängstigt.")
  end
end

