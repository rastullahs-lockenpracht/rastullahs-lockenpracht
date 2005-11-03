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
	super("Waldspinne", "Eine große Spinne, die sich anscheinend vor irgendetwas ängstigt")
	actor = $AM.createMeshActor("Waldspinne", "tie_waldspinne.mesh", PhysicsManager::GT_ELLIPSOID )
	setActor(actor)

	addAction(ScareToDeathAction.new())
  end

  def die(player)
	getActor().getControlledObject().startAnimation("ko", 1.0, 1)
	# TODO: Dialog/Queststatus, Player verletzen
  end
end

