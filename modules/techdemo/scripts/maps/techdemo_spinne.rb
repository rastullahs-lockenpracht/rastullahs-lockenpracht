require 'embed.rb'

class ScareToDeathAction < Action
  def initialize
	super("Erschrecken", "Die Spinne erschrecken")
  end

  def doAction(spinne, player, target)
	spinne.getScared(player)
  end
end

class SpiderDieAnimationListener < AnimationListener
	def initialize(player)
	        super()
		@mPlayer = player
	end

	def animationFinished(anEvent)
		$SCRIPT.log("SpiderDie animationFinished.");
		anEvent.getAnimation().getMeshObject().getActor().getGameObject().die(@mPlayer)
	end
	
	def animationPaused(anEvent)
		$SCRIPT.log("SpiderDie animationPaused.");
	end
	
	def animationUnpaused(anEvent)
		$SCRIPT.log("SpiderDie animationUnpaused.");
	end
end


class Waldspinne < Creature
    def initialize
	super("Waldspinne", "Eine große Spinne, die sich anscheinend vor irgendetwas ängstigt.")
	actor = $AM.createMeshActor("Waldspinne", "tie_waldspinne.mesh", PhysicsManager::GT_SPHERE, 0.0)
	setActor(actor)
	getActor().placeIntoScene(-2040.0, 343.0, -8200.0, 1.0, 0.0, 0.0, 0.0)	
	getActor().setScale(5.0, 5.0, 5.0)
	@mScareAction = ScareToDeathAction.new()
	addAction(@mScareAction)
	@mErschreckenSound = $AM.createSoundSampleActor("ErschreckenSound","erschrecken_buh_01.ogg");
	getActor().attach(@mErschreckenSound);
	@mSpinnenTodSound = $AM.createSoundSampleActor("SpinnenTotSound","spinne_todesschrei_01.ogg");
	getActor().attach(@mSpinnenTodSound);
    end

  def getScared(player)
	#Player erschreckt die Spinne
	@mErschreckenSound.getControlledObject().play();
	
	# Listener auf Todesanimation, läßt die Spinne entgültig sterben
	animListener = SpiderDieAnimationListener.new(player)
	
	# Todesanimation starten
	getActor().getControlledObject().startAnimation("ko", 0.33, 1)
	getActor().getControlledObject().getAnimation("ko").addAnimationListener(animListener)
  end
	
  def die(player)
	@mSpinnenTodSound.getControlledObject().play();
	
	# Quest erledigt
	RulesSubsystem.getSingleton().getQuestBook().getQuest("spinne").setState(Quest::COMPLETED)
	# Spieler verletzen
	player.modifyLe( -(player.getLe() - 4) )
	# Aktion ist nicht mehr möglich
	removeAction(@mScareAction)
	# Beschreibung der Spinne ändern
	setDescription("Eine große tote Spinne, die sich nie wieder vor irgendetwas ängstigt.")
  end
end

