require 'globals.rb'
#require 'startup.rb'
$World = $Core.getWorld();
$World.setSkyBox(true, "rl/dsa07");

pos = $World.getStartPoint();
heroActor = $AF.createGameActor("Held","held.mesh");
#heroActor.setPosition(pos[0], pos[1], pos[2]);
heroActor.setPosition(0.0, 20.0, 40.0);

heroPerson = DsaManager.getSingleton().getPerson(10000);
heroPerson.setActor(heroActor);
	      
$Ui.setActiveCharacter(heroPerson);

