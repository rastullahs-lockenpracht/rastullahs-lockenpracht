require 'embed.rb'

class ScareToDeathAction < Action
  def initialize
	super("Erschrecken", "Die Spinne erschrecken")
  end

  def doAction(spinne, player, target)
	spinne.die(player)
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
    @mErschreckenSound = $AM.createSoundSampleActor("ErschreckenSound","erschrecken_psst_buh_01.ogg");
    getActor().attach(@mErschreckenSound);
    @mSpinnenTodSound = $AM.createSoundSampleActor("SpinnenTotSound","spinne_todesschrei_01.ogg");
    getActor().attach(@mSpinnenTodSound);
  end

  def die(player)
	#Player erschreckt die Spinne
    #queue = SoundPlaylist.new()
    #queue.add(@mErschreckenSound.getControlledObject())	
	#queue.add(@mSpinnenTodSound.getControlledObject())
	#GameLoopManager.getSingleton().addAsynchronousTask(queue);
	@mErschreckenSound.getControlledObject().play();
	
	# Todesanimation
	getActor().getControlledObject().startAnimation("ko", 1.0, 1)
	# Quest erledigt
	RulesSubsystem.getSingleton().getQuestBook().getQuest("spinne").setState(Quest::COMPLETED)
	# Spieler verletzen
	player.modifyLe( -(player.getLe() - 4) )
	# Aktion ist nicht mehr möglich
	removeAction(@mScareAction)
	#
	setDescription("Eine große tote Spinne, die sich nie wieder vor irgendetwas ängstigt.")
  end
end

