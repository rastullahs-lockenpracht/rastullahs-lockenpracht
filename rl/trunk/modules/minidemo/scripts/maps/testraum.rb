require 'globals.rb'
require 'player.rb'

$World = $Core.getWorld()
$World.setSkyBox(true, "rl/dsa07")

heroActor = $AF.createGameActor("Held","held.mesh")
pos = $World.getStartPoint()
#heroActor.setPosition(pos[0], pos[1], pos[2])
heroActor.setPosition(0.0, 20.0, 40.0)

heroPerson = DsaManager.getSingleton().getPerson(10000)
PlayerSettings.preparePlayer(heroPerson)
heroPerson.setActor(heroActor)
	      
$Ui.setActiveCharacter(heroPerson)

