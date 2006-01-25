require 'embed.rb'

class CombatTrigger < GameAreaListener
  def areaLeft(event)
    print( "Raus - " +  event.getProvokingActor().getName() );
  end
  def areaEntered(event)
    print( "Rein - " +  event.getProvokingActor().getName() );
    spinne = event.getSource().getActor().getGameObject()
    held = event.getProvokingActor().getGameObject()
    
    combat = Combat.new()
    combat.add(spinne, 2)
    combat.add(held, 1)

    $UI.startCombat(combat)
  end
end

class Waldspinne < Creature
  def initialize
	super("Waldspinne", "Eine gräßliche, riesige, äußerst passiv-aggressive Waldspinne")
	actor = $AM.createMeshActor("Waldspinne", "tie_waldspinne.mesh" )
	setActor(actor)
  end
end