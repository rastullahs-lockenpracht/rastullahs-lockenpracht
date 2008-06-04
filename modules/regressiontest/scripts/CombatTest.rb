require 'testcase.rb'

class CombatTest < TestCase
    def execute()
		MapLoader.new().loadMap("combatarena.rlmap");
		
		wolf = $GOM.getGameObject(100);
		wolf.setAlignment(Creature::ALIGNMENT_ENEMY);
		wolf.modifyAu(1000)

		schnauze = $GOM.createGameObject("Wolfsschnauze");
		wolf.getInventory().ready(schnauze, "Snout");

	end
end