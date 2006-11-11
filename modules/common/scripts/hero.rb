load "embed.rb"

class Hero < Person
    def initialize(id)
        super(id);
	setName("Alrike");
	setDescription("Heldin von Berufung");
	$SCRIPT.log("Gute Eigenschaften zuweisen...");

        setEigenschaft("MU", 12);
        setEigenschaft("KL", 8);
        setEigenschaft("IN", 9);
        setEigenschaft("CH", 10);
        setEigenschaft("FF", 11);
        setEigenschaft("GE", 12);
        setEigenschaft("KO", 13); #KO
        setEigenschaft("KK", 14); #KK
        setWert(2, 10); # LE-Mod

	$SCRIPT.log("LeP und AuP auffüllen");
        modifyLe(1000, false); # LeP = LE
        modifyAu(1000); # Au = voll
        # modifyLe(-20); # 10 LeP abziehen

	$SCRIPT.log("Talente hinzufügen");
	addTalent("Athletik", 3);
	#addTalent("Körperbeherrschung", 3);

        $SCRIPT.log("Heldenaktor erstellen...");
        setProperty("str_meshfile", "men_alrike.mesh")
	setProperty("int_physicsbody", PhysicsManager::GT_ELLIPSOID)
	setProperty("real_mass", 75.0);
 
#       addSounds()
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

