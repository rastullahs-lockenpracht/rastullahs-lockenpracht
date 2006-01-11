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
	actor = $AM.createMeshActor("Waldspinne", "tie_waldspinne.mesh", PhysicsManager::GT_ELLIPSOID )
	setActor(actor)
	@mScareAction = ScareToDeathAction.new()
	addAction(@mScareAction)
	@mErschreckenSound = $AM.createSoundSampleActor("ErschreckenSound","erschrecken_psst_buh_01.ogg");
	@mErschreckenSound.placeIntoScene( -2040.0, 343.0, -8200.0, 1.0, 0.0, 0.0, 0.0);
	
	@mSpinnenTodSound = $AM.createSoundSampleActor("SpinnenTotSound","spinne_todesschrei_01.ogg");
	@mSpinnenTodSound.placeIntoScene( -2040.0, 343.0, -8200.0, 1.0, 0.0, 0.0, 0.0);

	@mSchmerzSchreiSound = $AM.createSoundSampleActor("SchmerzSchreiSound","schmerz_schrei_au_01.ogg");
	@mSchmerzSchreiSound.placeIntoScene( -2040.0, 343.0, -8200.0, 1.0, 0.0, 0.0, 0.0); 
  end

  def die(player)
	#Player erschreckt die Spinne
	@mErschreckenSound.getControlledObject().play();	
	# Todesanimation
	getActor().getControlledObject().startAnimation("ko", 1.0, 1)
	@mSpinnenTodSound.getControlledObject().play();
	# Quest erledigt
	RulesSubsystem.getSingleton().getQuestBook().getQuest("spinne").setState(Quest::COMPLETED)
	# Spieler verletzen
	player.modifyLe( -(player.getLe() - 1) )
	@mSchmerzSchreiSound.getControlledObject().play();	
	# Aktion ist nicht mehr möglich
	removeAction(@mScareAction)
	#
	setDescription("Eine große tote Spinne, die sich nie wieder vor irgendetwas ängstigt.")
  end
end

