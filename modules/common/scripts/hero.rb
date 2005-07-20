require 'globals.rb'

class Hero < Person
    def initialize
        super(10000, "Alrik", "Der Testheld");
        
        setEigenschaft(6, 13); #KO
        setEigenschaft(7, 11); #KK
        setWert(2, 10); # LE-Mod
        
        modifyLe(1000); # LeP = LE
        modifyLe(-20); # 10 LeP abziehen

        $CORE.log("Heldenaktor erstellen...");
        $heldaktor = $AF.createMeshActor("Held","held.mesh", PhysicsManager::GT_CAPSULE, -1.0);
        setActor($heldaktor);
        $CORE.log("done.");
    end
end

