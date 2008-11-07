require 'globals.rb'

def Fetch(acting_creature, item, slot = "Right Hand")
    job = FetchItemJob.new(acting_creature, targetpos, slot, 1000000)
    $JS.addJob(job)
end

def _Fetch(acting_creature, item, slot = "Right Hand")
    return FetchItemJob.new(acting_creature, targetpos, slot, 1000000)
end
