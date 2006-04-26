require 'embed.rb'

class Kurzschwert < Weapon
  def initialize()
    super("Kurzschwert", "Ein kurzes, gerades Schwert");

    setTp(1, 0, 2);
    setTpKk(11, 4);
    setBf(3);
    setIni(-1);
    setDk(Weapon::DK_N);
    setKampftechnik("Schwerter");

    setActor($AM.createMeshActor("kurzschwert", "waf_kurzschwert_01.mesh"));
  end
end

class SchwesterSteuerung < RBCombatController
  def initialize(combat, group, own, opponent)
    super(combat, group);
    @mOpponent = opponent;
    @mOwn = own;
  end

  def config()
    setCurrentCreature(@mOwn);
    setActionOption(RBCombat::ACTION_ATTACK);
    setAttackTarget(@mOpponent);
    setPareeTarget(@mOpponent);
  end

  def notifyActionStart()
    getCombat().tick();
  end

  def notifyCombatEnd()
  end
end

class CombatTrigger < GameAreaListener
  def areaLeft(event)

  end

  def areaEntered(event)
    $GameEveMgr.removeAreaListener(self)

    held = event.getProvokingActor().getGameObject()
    schwertH = Kurzschwert.new()
    held.addWeapon(schwertH)
    held.switchToWeapon(schwertH.getId())
    
    schwester = event.getSource().getActor().getGameObject()
    schwertS = Kurzschwert.new()
    schwester.addWeapon(schwertS)
    schwester.switchToWeapon(schwertS.getId())

    combat = RBCombat.new()
    combat.add(schwester, 2)
    combat.add(held, 1)
    
    steuerung = SchwesterSteuerung.new(combat, 2, schwester, held)
    steuerung.config()
    combat.addController(steuerung)

    $UI.startCombat(combat)
  end
end

class Schwester < Person
  def initialize
	super("Deine Schwester", "Deine äußerst passiv-aggressive Zwillingsschwester")

	actor = $AM.createMeshActor("Schwester", "men_alrike.mesh" )
	setActor(actor)

	setEigenschaft("MU", 10);
        setEigenschaft("KL", 6);
        setEigenschaft("IN", 7);
        setEigenschaft("CH", 16);
        setEigenschaft("FF", 9);
        setEigenschaft("GE", 10);
        setEigenschaft("KO", 11); 
        setEigenschaft("KK", 12); 
        setWert(2, 10); # LE-Mod
	modifyLe(1000);
	modifyAu(1000);

	addKampftechnik("Schwerter", [3, 2]);


  end
end