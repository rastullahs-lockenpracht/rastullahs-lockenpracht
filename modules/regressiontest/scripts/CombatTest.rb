require 'testcase.rb'

class CombatTest < TestCase
    def execute()
		MapLoader.new().loadMap("combatarena.rlmap");
		
		insertWolf();
		insertTestDialog();
	end
	
	def insertWolf()
		wolf = $GOM.getGameObject(100);
		
		wolf.setAlignment(Creature::ALIGNMENT_ENEMY);
		
		wolf.modifyAu(1000)

		
		schnauze = $GOM.createGameObject("Wolfsschnauze");
		
		wolf.getInventory().ready(schnauze, "Snout");

	end
	
	def insertTestDialog()
		npc = $GOM.createGameObject("TestPerson");
		npc.setProperty("dialog", "combatdialog");
		npc.getInventory().ready($GOM.createGameObject("Kurzschwert"), "Right Hand");

		npc.setPosition(getCenter());
		npc.placeIntoScene();
        npc.getActor().getPhysicalThing().setUpConstraint();
	end
end
