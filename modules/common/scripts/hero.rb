load "embed.rb"

class Hero < Person
    def initialize
        super("Alrik", "Der Testheld");

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
	# modifyLe(-20); # 10 LeP abziehen


        $SCRIPT.log("Heldenaktor erstellen...");
        $heldaktor = $AM.createMeshActor("Held","held.mesh", PhysicsManager::GT_ELLIPSOID, 75.0);
        setActor($heldaktor);
	$heldaktor.setQueryMask(Actor::QGF_PLAYER);

        addSounds()
        $SCRIPT.log("done.");
    end

    def addSounds
        heroActor = getActor()
        soundActor = $AM.createSoundSampleActor(heroActor.getName()+"_schlucken","schlucken.ogg");
        heroActor.attachToSlot(soundActor, "Bone01");
        @mSchmerzSchreiSound = $AM.createSoundSampleActor("SchmerzSchreiSound","schmerz_schrei_au_01.ogg");
        heroActor.attachToSlot(@mSchmerzSchreiSound, "Bone01"); 
    end
    
end

