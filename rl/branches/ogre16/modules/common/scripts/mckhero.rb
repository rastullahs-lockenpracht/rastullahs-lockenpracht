load "embed.rb"

class MCKHero < Creature
    def initialize(id)
        super(id);
        #addQueryFlag(RlScript::QUERYFLAG_PLAYER) #this is done now by setActiveCharacter
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
	setWert(WERT_MOD_LE, 10); # LE-Mod
	setWert(WERT_MOD_AU, 11); # AU-Mod
	setWert(WERT_MOD_MR, -4); # MR-Mod
	setWert(WERT_SOZIALSTATUS, 5);

	$SCRIPT.log("LeP und AuP auffüllen");
        modifyLe(1000, false); # LeP = LE
        modifyAu(1000); # Au = voll
        #modifyLe(-20); # 10 LeP abziehen

	$SCRIPT.log("Talente hinzufügen");
	#Kampffertigkeiten
	#addTalent("Armbrust", 4);
	#addTalent("Dolche", 3);
	#addTalent("Infanteriewaffen", 6);
	#addTalent("Raufen", 6);
	#addTalent("Ringen", 3);
	#addTalent("Schwerter", 4);

	#Körperliche Talente
	addTalent("Athletik", 2);
	addTalent("Klettern", 1);
	addTalent("Körperbeherrschung", 2);
	addTalent("Schwimmen", 2);
	addTalent("Selbstbeherrschung", 2);
	addTalent("Sinnenschärfe", 3);
	addTalent("Zechen", 3);

	#Gesellschaftliche Talente
	addTalent("Etikette", 3);
	addTalent("Gassenwissen", 3);
	addTalent("Menschenkenntnis", 4);
	addTalent("Überreden", 2);

	#Naturtalente
	addTalent("Fährtensuchen", 1);
	addTalent("Fesseln / Entfesseln", 3);
	addTalent("Orientierung", 1);

	#Wissenstalente
	addTalent("Götter und Kulte", 3);
	addTalent("Heraldik", 3);
	addTalent("Kriegskunst", 2);
	addTalent("Rechnen", 1);
	addTalent("Rechtskunde", 4);
	addTalent("Sagen / Legenden", 2);
	addTalent("Schätzen", 3);

	#Handwerkstalente
	addTalent("Hauswirtschaft", 1);

	#Sprachen
	# Muttersprache Garethi
	# 1 Fremdsprache +3
	# L/S Garethi 4
	
	$SCRIPT.log("Sonderfertigkeiten hinzufügen");
	addSf("Ortskenntnis", SFSTATUS_OK);
	addSf("Rüstungsgewöhnung I", SFSTATUS_OK);


        $SCRIPT.log("Heldenaktor beschreiben...");

	#addSounds()
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

