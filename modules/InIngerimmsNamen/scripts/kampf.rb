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

class SchwesterSteuerung < CombatController
  def initialize(combat, group, own, opponent)
    super(combat, group);
    @mOpponent = opponent;
    @mOwn = own;
  end

  def config()
    setCurrentCreature(@mOwn);
    setActionOption(Combat::ACTION_ATTACK);
    setAttackTarget(@mOpponent);
    setPareeTarget(@mOpponent);
  end

  def notifyActionStart()
    getCombat().tick();
  end
end

class CombatTrigger < GameAreaListener
  def areaLeft(event)

  end

  def areaEntered(event)
    held = event.getProvokingActor().getGameObject()
    schwertH = Kurzschwert.new()
    held.addWeapon(schwertH)
    held.switchToWeapon(schwertH.getId())
    
    schwester = event.getSource().getActor().getGameObject()
    schwertS = Kurzschwert.new()
    schwester.addWeapon(schwertS)
    schwester.switchToWeapon(schwertS.getId())

    combat = Combat.new()
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

	addKampftechnik("Schwerter", [3, 2]);
  end
end