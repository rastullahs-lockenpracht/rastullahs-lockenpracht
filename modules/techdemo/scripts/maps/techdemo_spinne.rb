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
  end

  def die(player)
	# Todesanimation
	getActor().getControlledObject().startAnimation("ko", 1.0, 1)
	# Quest erledigt
	RulesSubsystem.getSingleton().getQuestBook().getQuest("spinne").setState(Quest::COMPLETED)
	# Spieler verletzen
	player.modifyLe( -(player.getLe() - 1) )
	# Aktion ist nicht mehr möglich
	removeAction(@mScareAction)
	#
	setDescription("Eine große tote Spinne, die sich nie wieder vor irgendetwas ängstigt.")
  end
end

