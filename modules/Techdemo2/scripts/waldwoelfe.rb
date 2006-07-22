require "simpleenemy.rb"

class WaldWolfTotListener < SimpleEnemyDeathListener
    def initialize( agent )
        @agent = agent
    end
    def onDie( creature )
        p "argfL!!! *sterb*"

        if( @agent != nil )
            @agent.clearSteeringBehaviours()
        end
    end
end

# 4 Wölfe platzieren
positions = [ [ 15.43, -68.72, -188.17], [ -20.36, -63.26, -216.89],  [ -20.79, -71.38, -113.83], [ 0.90, -47.64, -46.25 ] ]
wolfnum = 1

# Die Positionen durchgehen
positions.each { |pos|
    se = SimpleEnemy.new( "Wolf_"+wolfnum.to_s,"Ein gefährlicher, ganz gefährlicher, wirklich extrem gefährlicher Wolf","Ein sehr toter Wolf","tie_wolf_01.mesh","spinne_todesschrei_01.ogg");
    se.getActor().placeIntoScene( pos ) 

    agent = $AI.createAgent(AgentManager::AGENT_STD_NPC, se )
    agent.addSteeringBehaviour(DefaultWanderBehaviour.new);
    agent.addSteeringBehaviour(AvoidObstaclesBehaviour.new);

    se.addDeathListener( WaldWolfTotListener.new( agent ) )
    wolfnum += 1
} 


