require "simpleenemy.rb"

class WaldWolfTotListener < SimpleEnemyDeathListener
    def initialize( agent )
        @agent = agent
    end
    def onDie( creature )
        p "argfL!!! *sterb*"

        if( not @agent.nil? )
            @agent.clearSteeringBehaviours()
        end
    end
end

# 4 Wölfe platzieren
positions = [ [ 15.43, -68.72, -188.17], [ -20.36, -63.26, -216.89],  [ -20.79, -71.38, -113.83], [ 0.90, -47.64, -46.25 ] ]
wolfnum = 1

# Die Positionen durchgehen
positions.each { |pos|
    se = $GOM.createGameObject("Wolf");
    
    se.setPosition(pos);
    se.getActor().placeIntoScene();

    se.addDeathListener( WaldWolfTotListener.new( agent ) )
    wolfnum += 1
} 


