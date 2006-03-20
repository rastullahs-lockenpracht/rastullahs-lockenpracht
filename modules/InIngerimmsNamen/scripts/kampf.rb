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
  def initialize(combat, group, opponent)
    super(combat, group);
    @mMe = me;
    @mOpponent = opponent;
  end

  def config()
    setActionOption(Combat::ATTACK);
    setAttackTarget(opponent);
    setPareeTarget(opponent);
  end

  def notifyActionStart()
    mCombat.tick();
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
    
 #   combat.addController(SchwesterSteuerung.new(combat, 2, held))

#    $UI.startCombat(combat)
  end
end

class Schwester < Person
  def initialize
	super("Deine Schwester", "Deine äußerst passiv-aggressive Zwillingsschwester")
	actor = $AM.createMeshActor("Schwester", "men_alrike.mesh" )
	setActor(actor)
  end
end