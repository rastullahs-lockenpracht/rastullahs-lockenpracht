load "embed.rb"

class MCKHero < Person
    def initialize(id)
        super(id);
        addQueryFlag(RlScript::QUERYFLAG_PLAYER)
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
	setWert(Creature::WERT_MOD_LE, 10); # LE-Mod

	$SCRIPT.log("LeP und AuP auffüllen");
        modifyLe(1000, false); # LeP = LE
        modifyAu(1000); # Au = voll
        #modifyLe(-20); # 10 LeP abziehen

	$SCRIPT.log("Talente hinzufügen");
	addTalent("Athletik", 3);
	#addTalent("Körperbeherrschung", 3);

        $SCRIPT.log("Heldenaktor beschreiben...");

	$SCRIPT.log("Inventar konfigurieren.");
	inv = getInventory();
	inv.addSlot("Left Ring", "Bip01 L Finger21", Item::ITEMTYPE_RING, Item::ITEMTYPE_RING);
	inv.addSlot("Right Ring", "Bip01 R Finger21", Item::ITEMTYPE_RING, Item::ITEMTYPE_RING);
	inv.addSlot("Left Hand", "Bip01 L SlotHand", Item::ITEMTYPE_WEAPON, Item::ITEMTYPE_ALL_ITEMS);
	inv.addSlot("Right Hand", "Bip01 R SlotHand", Item::ITEMTYPE_WEAPON, Item::ITEMTYPE_ALL_ITEMS);
	inv.addSlot("Head", "Bip01 Head", Item::ITEMTYPE_HELMET, Item::ITEMTYPE_HELMET, RlScript::SLOT_SUBMESH);
	inv.addSlot("Back", "back", 0, Item::ITEMTYPE_ALL_ITEMS, RlScript::SLOT_SUBMESH);
	#inv.addSlot("Back", "Bip01 Neck", Item::ITEMTYPE_ALL_ITEMS);
    inv.addSlot("Armor", "armor", Item::ITEMTYPE_ARMOR, Item::ITEMTYPE_ARMOR, RlScript::SLOT_SUBMESH);
	inv.addSlot("Torso", "men_human_female_torso", Item::ITEMTYPE_ARMOR, Item::ITEMTYPE_ARMOR, RlScript::SLOT_MATERIAL);
	inv.addSlot("Clothes", "clothes", Item::ITEMTYPE_ARMOR,  Item::ITEMTYPE_ARMOR, RlScript::SLOT_SUBMESH);
	inv.addSlot("Boots", "boots", Item::ITEMTYPE_BOOTS,  Item::ITEMTYPE_BOOTS, RlScript::SLOT_SUBMESH);
	inv.addSlot("Trousers", "trousers", Item::ITEMTYPE_TROUSERS,  Item::ITEMTYPE_TROUSERS, RlScript::SLOT_SUBMESH);

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

