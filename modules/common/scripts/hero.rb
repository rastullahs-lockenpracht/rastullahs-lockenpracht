require 'globals.rb'

class Hero < Person
    def initialize
        super(10000, "Alrik", "Der Testheld");

        setEigenschaft("MU", 12);
	    setEigenschaft("KL", 8);
     	setEigenschaft("IN", 9);
        setEigenschaft("CH", 10);
	    setEigenschaft("FF", 11);
	    setEigenschaft("GE", 12);
        setEigenschaft("KO", 13); #KO
        setEigenschaft("KK", 14); #KK
        setWert(2, 10); # LE-Mod

        modifyLe(1000); # LeP = LE
	    modifyAu(1000); # Au = voll
	    modifyLe(-20); # 10 LeP abziehen


        $CORE.log("Heldenaktor erstellen...");
        $heldaktor = $AM.createMeshActor("Held","held.mesh", PhysicsManager::GT_CAPSULE, -1.0);
        setActor($heldaktor);

        addSounds()
        $CORE.log("done.");
    end

    def addSounds
        heroActor = getActor()
        soundActor = $AM.createSoundSampleActor(heroActor.getName()+"_schlucken","schlucken.ogg");
        heroActor.attachToSlot(soundActor, "Bone01");
    end
    
end

