load "embed.rb"

class Hero < Person
    def initialize
        super("Alrike", "Heldin von Berufung");

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

	addTalent("Athletik", 3);
	#addTalent("Körperbeherrschung", 3);

        $SCRIPT.log("Heldenaktor erstellen...");
        $heldaktor = $AM.createMeshActor("Held","men_alrike.mesh", PhysicsManager::GT_ELLIPSOID, 75.0);
        setActor($heldaktor);
        $heldaktor.setQueryMask(Actor::QGF_PLAYER);

        addSounds()
        $SCRIPT.log("done.");
    end

    def addSounds
        heroActor = getActor()
        soundActor = $AM.createSoundSampleActor(heroActor.getName()+"_schlucken","schlucken.ogg");
        heroActor.attachToSlot(soundActor, "huefte");
        @mSchmerzSchreiSound = $AM.createSoundSampleActor("SchmerzSchreiSound","schmerz_schrei_au_01.ogg");
        heroActor.attachToSlot(@mSchmerzSchreiSound, "huefte"); 
    end

end

