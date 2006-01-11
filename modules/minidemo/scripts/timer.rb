require 'embed.rb'

class FackelAenderung < TimerListener
   def initialize(on, particleActor)
       super()
       @mOn = on
       @mFireParticle = particleActor
   end

   def timerFired(event)
       @mFireParticle.getControlledObject().setActive(@mOn)
       @mOn = !@mOn
   end
end

fackel = $AM.getActor("Das fitzelnde Leuchten")

fackelAnTimer = TimerEventSource.new($CORE.getClock() + 5000, 10000)
fackelAnTimer.addTimerListener(FackelAenderung.new(true, fackel))