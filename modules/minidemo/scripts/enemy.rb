require "simpleenemy.rb"

class TextOnSimpleEnemyDeath < SimpleEnemyDeathListener
    def initialize( agent )
        @agent = agent
    end
    def onDie
        p "argfL!!! *sterb*"
        if( not @agent.nil? )
            @agent.clearSteeringBehaviours()
        end
    end
end

se = SimpleEnemy.new( "Wolf","Lebendiger Wolf","Toter Wolf","kiste.mesh","spinne_todesschrei_01.ogg");
se.getActor().placeIntoScene([1.40, 2.80, 2.20]) 

agent = $AI.createAgent(AgentManager::AGENT_STD_NPC, se )
agent.addSteeringBehaviour(DefaultWanderBehaviour.new);
agent.addSteeringBehaviour(AvoidObstaclesBehaviour.new);

se.addDeathListener( TextOnSimpleEnemyDeath.new( agent ) )