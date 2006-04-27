#require 'RlScript'

class CampAction < Action
  def initialize
    super("makecamp", "Das Zelt aufschlagen und rasten")
  end

  def doAction(object, actor, target)
    $WF.showMessageWindow("Hier kannst du nicht schlafen")
  end
end

class RTCombatAttackAction < Action
  def initialize
    super("rtcombatattack", "Im Echtzeitkampf angreifen")
  end

  def doAction(object, actor, target)
    controller = $UI.getCharacterController();
    if (controller.getType() == CharacterController::CTRL_RTCOMBAT)
	controller.attack()
    end
  end
end

class RTCombatBlockAction < Action
  def initialize
    super("rtcombatblock", "Im Echtzeitkampf die Parade ändern")
  end

  def doAction(object, actor, target)
    controller = $UI.getCharacterController();
    if (controller.getType() == CharacterController::CTRL_RTCOMBAT)
	controller.block()
    end
  end
end

class PlayerSettings

  def PlayerSettings.addPlayerActions(player)
    $grpGrp1 = ActionGroup.new("GameActions")
    $grpCombat = ActionGroup.new("Kampf")

    $act2 = CampAction.new
    $act25 = RTCombatAttackAction.new()
    $act26 = RTCombatBlockAction.new()

    player.addActionInGroup($act2, $grpGrp1)
    player.addActionInGroup($act25, $grpCombat)
    player.addActionInGroup($act26, $grpCombat)

  end
  
  def PlayerSettings.preparePlayer(player)
    PlayerSettings.addPlayerActions(player)        # mal gucken, was noch so kommt
  end

end
