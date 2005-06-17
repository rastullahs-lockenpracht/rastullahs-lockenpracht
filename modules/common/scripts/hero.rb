require 'globals.rb'

class Hero < Person
    def initialize
        super(10000, "Alrik", "Der Testheld");
        $CORE.log("Heldenaktor erstellen...");
	$heldaktor = $AF.createMeshActor("Held","held.mesh", PhysicsManager::GT_CAPSULE, -1.0);
        setActor($heldaktor);
        $CORE.log("done.");
    end
end

