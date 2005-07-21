load "embed.rb"

class SecretDoorOpener < ObjectStateChangeListener

  def initialize(door, switch)
    super()
    @door = door
    @switch = switch
    switch.addObjectStateChangeListener(self)
  end

  def objectStateChanged(event)
      if (@switch.getString("state") == "Oben")
        doorOpen = true
        p "Tuer sollte offen sein"
      else
        doorOpen = false
        p "Tuer sollte geschlossen sein"
      end

      if (@door.getBool("open") != doorOpen)
        p "Tuer muss veraendert werden"
        if (doorOpen)
          p "Oeffnen"
          @door.doAction("OpenDoorAction", "opendoor", NIL, NIL)
          p "Offen"
        else
          p "Schliessen"
          @door.doAction("CloseDoorAction", "closedoor", NIL, NIL)
          p "Geschlossen"
        end
      end
  end

end
