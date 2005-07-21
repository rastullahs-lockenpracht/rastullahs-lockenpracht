require 'globals.rb'

class Hero < Person
    def initialize
        super(10000, "Alrik", "Der Testheld");
        
	setEigenschaft(1, 8);
	setEigenschaft(2, 9);
	setEigenschaft(3, 10);
	setEigenschaft(4, 11);
	setEigenschaft(5, 12);
        setEigenschaft(6, 13); #KO
        setEigenschaft(7, 14); #KK
        setWert(2, 10); # LE-Mod
        
        modifyLe(1000); # LeP = LE
	modifyAu(1000); # Au = voll
	modifyLe(-20); # 10 LeP abziehen


        $CORE.log("Heldenaktor erstellen...");
        $heldaktor = $AF.createMeshActor("Held","held.mesh", PhysicsManager::GT_CAPSULE, -1.0);
        setActor($heldaktor);
        $CORE.log("done.");
    end
end

