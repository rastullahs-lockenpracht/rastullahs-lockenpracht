load "embed.rb"

require 'door.rb'
require 'switch.rb'

class SecretDoorOpener < ObjectStateChangeListener

  def initialize(door, switch)
    super()
    @mDoor = door
    @mSwitch = switch
    @mSwitch.addObjectStateChangeListener(self)
  end

  def objectStateChanged(event)
      p event

      if ( @mSwitch.getState() == Switch::STATE_OBEN )
        doorOpen = true
        p "Tuer sollte offen sein"
      else
        doorOpen = false
        p "Tuer sollte geschlossen sein"
      end

      if (@mDoor.isOpen() != doorOpen)
        p "Tuer muss veraendert werden"
        if (doorOpen)
          p "Oeffnen"
          @mDoor.doAction("opendoor")
          p "Offen"
        else
          p "Schliessen"
          @mDoor.doAction("closedoor")
          p "Geschlossen"
        end
      end
  end

end
