load "embed.rb"
require "steering.rb";

class SimpleEnemyKillAction < Action
    # Name der Aktion
    def initialize
        super("kill", "Töten");
    end

    # Die Methode prüft, ob die Aktion überhaupt angeboten wird.
    def canDo(enemy, user)    
        enemy.isAlive
    end

    # Führt die Aktion durch
    def doAction( enemy, user, target )    
        enemy.die
    end
end

class SimpleEnemyDeathListener
    def onDie
    end
end

class SimpleEnemy < Creature
    # Maaap Mapp
    def initialize(name, alive_description, dead_description, mesh, sound )
        super( name, alive_description );

        enemyActor = $AM.createMeshActor( name, mesh,  PhysicsManager::GT_ELLIPSOID, 30.0 )
        @soundActor = $AM.createSoundSampleActor( name+"_stirbt", sound )
        @soundActor.getControlledObject.load()
        @soundActor.getControlledObject.set3d(true)
        $SCRIPT.log("simpleenemy.rb - Aktor erstellt.")
        setActor( enemyActor )
        $SCRIPT.log("simpleenemy.rb - Aktor gesetzt")

        @killAction = SimpleEnemyKillAction.new  
        @alive = true
        @deadDescription = dead_description
        @listeners = Array.new
        
        addAction(@killAction)
    end

    # Gehts dem Wesen noch gut?
    def isAlive
        @alive
    end

    # Standardaktion ist TÖÖÖTEN
    def getDefaultAction(actor)
        if @alive
            @killAction
        else
           super(actor)
        end
    end

    # Hier stirbt das Vieh
    def die
        # Wir haben nen Sound
        if( @soundActor != nil )
            # Ist noch nicht in der Szene?
            if( getActor().getChildByName( @soundActor.getName() ) == nil )
                getActor().attach( @soundActor )
            end
            # Abspielen
            @soundActor.getControlledObject().play()
        end
        removeAction(@killAction)
        setDescription(@deadDescription)
        getActor().getControlledObject().stopAllAnimations()
        @alive = false
        # Listener benachrichtigen
        @listeners.each { |l| l.onDie() }
    end

    def addDeathListener( listener )
        if( listener.kind_of? SimpleEnemyDeathListener )
            @listeners.push(listener)
        end
    end

    def removeDeathListener( listener )
        @listeners.pop(listener)
    end

end