#require 'RlScript'

class CampAction < Action
  def initialize
    super("makecamp", "Das Zelt aufschlagen und rasten")
  end

  def doAction(object, actor, target)
    $WF.showMessageWindow("Hier kannst du nicht schlafen")
  end
end


class PlayerSettings

  def PlayerSettings.addPlayerActions(player)
    $grpGrp1 = ActionGroup.new("GameActions")
    $act2 = CampAction.new

    player.addActionInGroup($act2, $grpGrp1)
  end
  
  def PlayerSettings.preparePlayer(player)
    PlayerSettings.addPlayerActions(player)        # mal gucken, was noch so kommt
  end

end
