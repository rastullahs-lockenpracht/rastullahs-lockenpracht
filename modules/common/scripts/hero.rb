load "embed.rb"

class Hero < Person
    def initialize(id)
        super(id);
        addQueryFlag(RlScript::QUERYFLAG_PLAYER)
		    
		setWert(Creature::WERT_MOD_LE, 10); # LE-Mod

		$SCRIPT.log("LeP und AuP auffüllen");
        modifyLe(1000, false); # LeP = LE
        modifyAu(1000); # Au = voll
        #modifyLe(-20); # 10 LeP abziehen

		$SCRIPT.log("Inventar konfigurieren.");
		inv = getInventory();
		inv.addSlot("Left Ring", "l_finger_2", Item::ITEMTYPE_RING);
		inv.addSlot("Right Ring", "r_finger_2", Item::ITEMTYPE_RING);
		inv.addSlot("Left Hand", "l_hand", Item::ITEMTYPE_ALL_ITEMS);
		inv.addSlot("Right Hand", "r_hand", Item::ITEMTYPE_ALL_ITEMS);
		inv.addSlot("Head", "kopf", Item::ITEMTYPE_HELMET);
		inv.addSlot("Back", "hals", Item::ITEMTYPE_ALL_ITEMS);

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

