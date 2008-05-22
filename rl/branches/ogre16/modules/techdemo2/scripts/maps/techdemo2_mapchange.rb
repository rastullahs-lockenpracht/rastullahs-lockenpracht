class Td2LevelwechselListener < GameAreaListener
	def initialize( targetscene, resourcegrp, targetrbfile )
		super()
		@targetScene = targetscene
		@targetRbFile = targetrbfile
		@resourcegrp = resourcegrp 
	end
 
	def areaLeft(anEvent)
	end
 
	# Zone betreten
	def areaEntered(anEvent)
		questBook = RulesSubsystem.getSingleton().getQuestBook()
		if (questBook.getQuest("aufenthaltsort").getState() == Quest::SUCCEEDED && 
			(questBook.getQuest("hauptquest42").getState() == Quest::ASSIGNED || 
			 questBook.getQuest("hauptquest41").getState() == Quest::ASSIGNED) )
			$AM.getActor("Held").detachWeapon();  
			$CORE.loadMap("Octree", @targetScene, @resourcegrp, @targetRbFile); 
		else
			$WF.showMessageWindow( "Ihr wisst noch nicht wohin ihr wollt - und was ihr an diesem Ort suchen wollt. Findet es erst heraus!" );
		end
	end
end
