require 'globals.rb'

class Hero < Person
    def initialize
        super(10000, "Alrik", "Der Testheld");
        $CORE.log("Heldenaktor erstellen...");
        setActor($AF.createMeshActor("Held","held.mesh", 2, -1.0, 1));
        $CORE.log("done.");
    end
end

