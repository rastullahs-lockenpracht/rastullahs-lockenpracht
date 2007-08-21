require "simpleenemy.rb"

#Quest Freischalten
RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest522").setKnown(true)
RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest522").setState(Quest::ASSIGNED)


#Wölfe erschaffen
class WaldWolfTotListener < SimpleEnemyDeathListener
    def initialize( agent )
        @agent = agent
    end
    def onDie( creature )
        p "Ein Wolf tot"
	RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest522").increasePartsDone()
	
	if (RulesSubsystem.getSingleton().getQuestBook().getQuest("hauptquest522").getState() == Quest::SUCCEEDED)
		#Schwert wegstecken
			$AM.getActor("Held").detachWeapon();  
			entry = $tagebuchtexte['retten2']
			RulesSubsystem.getSingleton().getQuestBook().addJournalEntry( entry[0], entry[1] );
		load("sonja2.rb");
	end


        if( not @agent.nil? )
            @agent.clearSteeringBehaviours()
        end
    end
end

# 4 Wölfe platzieren
positions = [ 
	[ 5.94, -48.54, -260.63], 
	[ 2.41, -48.30, -266.42],  
	[ 5.78, -47.94, -272.07], 
	[11.94, -47.71, -274.41] ];
wolfnum = 1

# Die Positionen durchgehen
positions.each { |pos|
    se = SimpleEnemy.new( "Wolf_"+wolfnum.to_s,"Ein gefährlicher, ganz gefährlicher, wirklich extrem gefährlicher Wolf. Aber er hat ein schwaches Herz.","Ein sehr toter Wolf","tie_wolf_01.mesh","wolf_sterben_001.ogg");
    se.getActor().placeIntoScene( pos ) 

    agent = $AGM.createAgent(AgentManager::AGENT_STD_NPC, se )
    agent.addSteeringBehaviour(DefaultWanderBehaviour.new);
    agent.addSteeringBehaviour(AvoidObstaclesBehaviour.new);

    se.addDeathListener( WaldWolfTotListener.new( agent ) )
    wolfnum += 1
} 