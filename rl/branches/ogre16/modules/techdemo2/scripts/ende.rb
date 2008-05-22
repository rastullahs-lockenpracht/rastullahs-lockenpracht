#Nach Ende der TD2 wird ins Hauptmenü zurückgekehrt.
load 'embed.rb'

class ArmageddonTimer < TimerListener
    def initialize()
        super()

        @mTimer = TimerEventSource.new($CORE.getClock() + 6000 )
        @mTimer.addTimerListener( self )
    end

    def timerFired(event)
	    $CORE.loadMap("Octree", "intro.scene", "General", "intro.rb")
        $WF.showMainMenu()
    end
end

ArmageddonTimer.new