load "embed.rb"

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


class SimpleEnemy < Creature
    # Maaap Mapp
    def initialize(name, alive_description, dead_description, mesh )
        super( name, alive_description );

        enemyActor = $AM.createMeshActor( name, mesh,  PhysicsManager::GT_BOX, 220.0 )
        $SCRIPT.log("simpleenemy.rb - Aktor erstellt.")
        setActor( enemyActor )
        $SCRIPT.log("simpleenemy.rb - Aktor gesetzt")

        @killAction = SimpleEnemyKillAction.new  
        @alive = true
        @deadDescription = dead_description
        
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
        removeAction(@killAction)
        setDescription(@deadDescription)
        p "arg, arfl *stirb*";
        @alive = false
    end
end


#se = SimpleEnemy.new( "Wolf","Lebendiger Wolf","Toter Wolf","kiste.mesh");
#se.getActor().placeIntoScene([1.40, 2.80, 2.20]) 